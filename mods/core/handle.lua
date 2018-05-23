--- Lua-side storage for safe references to C++ objects.
---
--- Whenever a new object is initialized, a corresponding handle will
--- be created here to track the object's lifetime in an isolated Lua
--- environment managed by a C++ handle manager. If the object is no
--- longer valid for use (a character died, or an item was destroyed),
--- the C++ side will set the Lua side's handle to be invalid. An
--- error will be thrown on trying to access or write to anything on
--- an invalid handle. Since objects are identified by simple integer
--- ids, this also allows for relatively simple serialization of such
--- references to C++ objects from Lua, allowing us to save the state
--- of any mods that are in use along with the base save data.
---
--- Borrowed from https://eliasdaler.github.io/game-object-references/

local Handle = {}
local inspect = require "mods/core/inspect"

Handle.CharaHandles = {}
Handle.ItemHandles = {}

-- Cache for function closures resolved when indexing a handle.
-- Creating a new closure for every method lookup is expensive.
-- Indexed by [class_name][method_name].
local memoizedFuncs = {}

local function generate_metatable(core_table, prefix)
   local mt = {}
   memoizedFuncs[prefix] = {}
   mt.__index = function(handle, key)
      if not handle.is_valid then
         print("Error: handle is not valid!")
         print("Indexing: " .. tostring(key))
         print(debug.traceback())
         error("Error: handle is not valid!", 2)
      end

      -- Try to get a property out of the C++ reference.
      local v = handle.cpp_ref[key]
      if v ~= nil then
         if type(v) ~= "function" then
            return v
         end
      end

      -- If that fails, try calling a function by the name given.
      local f = memoizedFuncs[prefix][key]
      if not f then
         -- Look up the function on the usertype table generated by
         -- sol2.
         f = function(handle, ...) return core_table[key](handle.cpp_ref, ...) end

         -- Cache it so we don't incur the overhead of creating a
         -- closure on every lookup.
         memoizedFuncs[prefix][key] = f
      end
      return f
   end
   mt.__newindex = function(handle, key, value)
      if not handle.is_valid then
         print("Error: handle is not valid!")
         print("Indexing: " .. tostring(key))
         print(debug.traceback())
         error("Error: handle is not valid!", 2)
      end

      handle.cpp_ref[key] = value
   end
   return mt
end

local chara_mt = generate_metatable(LuaCharacter, "LuaCharacter")
local item_mt = generate_metatable(LuaItem, "LuaItem")


function Handle.create_chara_handle(cpp_ref)
    local handle = {
        cpp_ref = cpp_ref,
        is_valid = true
    }

    setmetatable(handle, chara_mt)
    Handle.CharaHandles[cpp_ref.idx] = handle
    return handle
end

function Handle.create_item_handle(cpp_ref)
    local handle = {
        cpp_ref = cpp_ref,
        is_valid = true
    }

    setmetatable(handle, item_mt)
    Handle.ItemHandles[cpp_ref.idx] = handle
    return handle
end


function Handle.remove_chara_handle(cpp_ref)
    local handle = Handle.CharaHandles[cpp_ref.idx];
    handle.cpp_ref = nil
    handle.is_valid = false
    Handle.CharaHandles[cpp_ref.idx] = nil
end

function Handle.remove_item_handle(cpp_ref)
    local handle = Handle.ItemHandles[cpp_ref.idx];

    handle.cpp_ref = nil
    handle.is_valid = false
    Handle.ItemHandles[cpp_ref.idx] = nil
end


function Handle.assert_chara_valid(cpp_ref)
   local handle = Handle.CharaHandles[cpp_ref.idx];
   assert(handle.is_valid)
end

function Handle.assert_item_valid(cpp_ref)
   local handle = Handle.ItemHandles[cpp_ref.idx];
   assert(handle.is_valid)
end


function Handle.assert_chara_invalid(cpp_ref)
   local handle = Handle.CharaHandles[cpp_ref.idx];
   assert(handle == nil or not handle.is_valid)
end

function Handle.assert_item_invalid(cpp_ref)
   local handle = Handle.ItemHandles[cpp_ref.idx];
   assert(handle == nil or not handle.is_valid)
end


return Handle
