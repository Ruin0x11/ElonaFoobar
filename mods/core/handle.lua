local memoizedFuncs = {}

local Handle = {}
local inspect = require "mods/core/inspect"

Handle.CharaHandles = {}
Handle.ItemHandles = {}

local function generate_metatable(core_table, prefix)
   local mt = {}
   memoizedFuncs[prefix] = {}
   mt.__index = function(handle, key)
      if not handle.is_valid then
         print(debug.traceback())
         error("Error: handle is not valid!", 2)
      end

      -- Try to get a property
      local v = handle.cpp_ref[key]
      if v ~= nil then
         if type(v) == "function" then
            return v()
         else
            return v
         end
      end

      -- If that fails, try calling a function
      local f = memoizedFuncs[prefix][key]
      if not f then
         -- Return the function on the usertype's original table
         f = function(handle, ...) return core_table[key](handle.cpp_ref, ...) end
         memoizedFuncs[key] = f
      end
      return f
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


function Handle.on_chara_removal(cpp_ref)
    local handle = Handle.CharaHandles[cpp_ref.idx];
    handle.cpp_ref = nil
    handle.is_valid = false
    Handle.CharaHandles[cpp_ref.idx] = nil
end

function Handle.on_item_removal(cpp_ref)
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
