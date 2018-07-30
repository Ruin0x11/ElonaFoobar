--- Lua-side storage for safe references to C++ objects.
--
-- Whenever a new object is initialized, a corresponding handle will
-- be created here to track the object's lifetime in an isolated Lua
-- environment managed by a C++ handle manager. If the object is no
-- longer valid for use (a character died, or an item was destroyed),
-- the C++ side will set the Lua side's handle to be invalid. An
-- error will be thrown on trying to access or write to anything on
-- an invalid handle. Since objects are identified by simple integer
-- ids, this also allows for relatively simple serialization of such
-- references to C++ objects from Lua, allowing us to save the state
-- of any mods that are in use along with the base save data.
--
-- This mechanism is a solution for the problem of what happens when a
-- user assigns a C++ object reference to a deeply nested Lua table,
-- then that reference goes invalid on the C++ side. Originally an
-- approach was attempted where the C++ storage mechanism bound to Lua
-- attempted to detect invalid references when they were accessed.
-- This didn't work, because it didn't solve the problem of what
-- happens when the reference is assigned to a nested Lua table which
-- is referenced in the C++ storage. The C++ code would have to
-- iterate every possible nested table value to check for any invalid
-- references. This solution is more lightweight and robust since a
-- simple flag can be set on a handle and all references to it will be
-- updated automatically.
--
-- Borrowed from https://eliasdaler.github.io/game-object-references/

local Handle = {}

-- Stores a map of handle -> C++ object reference. These should not be
-- directly accessable outside this chunk.
local refs = {}

-- Stores a map of integer ID -> handle.
-- NOTE: If integer indices as IDs are completely removed, then this
-- may have to be indexed by another value.
local handles_by_index = {}

local function print_handle_error(key)
   if _IS_TEST then return end

   if Elona.core and Elona.core.GUI then
      Elona.core.GUI.txt_color(3)
      Elona.core.GUI.txt("Error: handle is not valid! ")
      if key ~= nil then
         Elona.core.GUI.txt("Indexing: " .. tostring(key) .. " ")
      end
      Elona.core.GUI.txt("This means the character/item got removed. ")
      Elona.core.GUI.txt_color(0)
   end
   print("Error: handle is not valid!")
   print(debug.traceback())
end

-- Cache for function closures resolved when indexing a handle.
-- Creating a new closure for every method lookup is expensive.
-- Indexed by [class_name][method_name].
local memoizedFuncs = {}


function Handle.is_valid(handle)
   return handle ~= nil and refs[handle.__kind][handle.__uuid] ~= nil
end


local function generate_metatable(prefix)
   -- The userdata table is bound by sol2 as a global.
   local userdata_table = _ENV[prefix]

   local mt = {}
   memoizedFuncs[prefix] = {}
   mt.__index = function(handle, key)
      if not Handle.is_valid(handle)then
         print_handle_error(key)
         error("Error: handle is not valid!", 2)
      end

      -- Try to get a property out of the C++ reference.
      local ref = refs[prefix][handle.__uuid][key]

      if ref ~= nil then
         if type(ref) ~= "function" then
            return ref
         end
      end

      -- If that fails, try calling a function by the name given.
      local f = memoizedFuncs[prefix][key]
      if not f then
         -- Look up the function on the usertype table generated by
         -- sol2.
         f = function(h, ...) return userdata_table[key](refs[prefix][h.__uuid], ...) end

         -- Cache it so we don't incur the overhead of creating a
         -- closure on every lookup.
         memoizedFuncs[prefix][key] = f
      end
      return f
   end
   mt.__newindex = function(handle, key, value)
      if not Handle.is_valid(handle) then
         print_handle_error(key)
         error("Error: handle is not valid!", 2)
      end

      refs[prefix][handle.__uuid][key] = value
   end
   mt.__eq = function(lhs, rhs)
      return lhs.__kind == rhs.__kind and lhs.__uuid == rhs.__uuid
   end
   mt.__serialize = function(handle)
      local data = {
         __uuid = handle.__uuid,
         __kind = handle.__kind,
         __index = handle.__index,
         __handle = true
      }
      return data
   end

   refs[prefix] = {}
   handles_by_index[prefix] = {}

   return mt
end

local metatables = {}
metatables.LuaCharacter = generate_metatable("LuaCharacter")
metatables.LuaItem = generate_metatable("LuaItem")

--- Given a valid handle and kind, retrieves the underlying C++
--- userdata reference.
function Handle.get_ref(handle, kind)
   if not Handle.is_valid(handle) then
      print_handle_error()
      error("Error: handle is not valid!", 2)
      return nil
   end

   if not handle.__kind == kind then
      print(debug.traceback())
      error("Error: handle is of wrong type: wanted " .. kind .. ", got " .. handle.__kind)
      return nil
   end

   return refs[kind][handle.__uuid]
end

function Handle.set_ref(handle, ref)
   refs[handle.__kind][handle.__uuid] = ref
end

function Handle.get_metatable(kind)
   return metatables[kind]
end

--- Given a C++ userdata reference and kind, retrieves the handle that
--- references it.
function Handle.get_handle(cpp_ref, kind)
   local handle = handles_by_index[kind][cpp_ref.index]

   if handle and handle.__kind ~= kind then
      print(debug.traceback())
      error("Error: handle is of wrong type: wanted " .. kind .. ", got " .. handle.__kind)
      return nil
   end

   return handle
end

function Handle.create_handle(cpp_ref, kind, uuid)
   -- TEMP until serialization feature is added
   -- if handles_by_index[kind][cpp_ref.index] ~= nil then
   --    error("Handle already exists: " .. kind .. ":" .. cpp_ref.index, 2)
   --    return nil
   -- end

   local handle = {
      __uuid = uuid,
      __kind = kind,
      __index = cpp_ref.index,
      __handle = true,
      is_valid = function(self) return Handle.is_valid(self) end
   }

   setmetatable(handle, metatables[handle.__kind])
   refs[handle.__kind][handle.__uuid] = cpp_ref
   handles_by_index[handle.__kind][cpp_ref.index] = handle

   return handle
end


function Handle.remove_handle(cpp_ref, kind)
   local handle = handles_by_index[kind][cpp_ref.index]

   if handle == nil then
      return
   end

   assert(handle.__kind == kind)

   refs[handle.__kind][handle.__uuid] = nil
   handles_by_index[handle.__kind][cpp_ref.index] = nil
end


function Handle.assert_valid(cpp_ref, kind)
   local handle = handles_by_index[kind][cpp_ref.index]

   assert(Handle.is_valid(handle))
end


function Handle.assert_invalid(cpp_ref, kind)
   local handle = handles_by_index[kind][cpp_ref.index]

   assert(not Handle.is_valid(handle))
end


-- Functions for deserialization. The steps are as follows.
-- 1. Deserialize mod data that contains the list of handles.
-- 2. Clear out existing handles/references in handles_by_index and refs.
-- 3. Place handles into the handles_by_index table.
-- 4. In C++, for each object loaded, add its reference to the refs
--    table using handles_by_index.
function Handle.get_handle_range(kind, index_start, index_end)
   local ret = {}
   print("RANGE " .. index_start .. " " .. index_end)
   for index, handle in Handle.iter(kind, index_start, index_end) do
      ret[index] = handle
   end
   return ret
end

function Handle.merge_handles(kind, handles_)
   for index, handle in pairs(handles_) do
      if handle ~= nil then
         if handles_by_index[kind][index-1] ~= nil then
            error("Attempt to overwrite handle " .. kind .. ":" .. index-1, 2)
         end
         handles_by_index[kind][index-1] = handle
      end
   end
end


function Handle.clear_handle_range(kind, index_start, index_end)
   print("CLEARRANGE " .. index_start .. " " .. index_end)
   for index=index_start, index_end do
      local handle = handles_by_index[kind][index]
      if handle ~= nil then
         refs[kind][handle.__uuid] = nil
         handles_by_index[kind][index] = nil
      end
   end
end


function Handle.clear()
   for kind, _ in pairs(refs) do
      refs[kind] = {}
   end

   for kind, _ in pairs(handles_by_index) do
      handles_by_index[kind] = {}
   end
end


local function iter(a, i)
   local v = a.handles[i]
   while not (v and Handle.is_valid(v)) do
      if i >= a.to then
         return nil
      end
      i = i + 1
      v = a.handles[i]
   end
   i = i + 1
   return i, v
end

function Handle.iter(kind, from, to)
   if from > to then
      return nil
   end
   return iter, {handles=handles_by_index[kind], to=to}, from
end

-- These functions exist in the separate handle environment because I
-- couldn't quite figure out how to make a valid custom C++/Lua
-- iterator with Sol that returns Lua table references as values.

-- Chara.iter(from, to)
Handle.iter_charas = function(from, to) return Handle.iter("LuaCharacter", from, to) end

-- Item.iter(from, to)
Handle.iter_items = function(from, to) return Handle.iter("LuaItem", from, to) end


return Handle
