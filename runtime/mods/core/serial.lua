-- The handle environment is preloaded before this code is run, so
-- serializable references can be resolved.
assert(Handle)

local serpent = require "mods/core/serpent"

local Serial = {}

local function resolve_handles(data)
   for key, value in data do
      if type(value) == "table" then
         if value.__kind ~= nil then
            -- Resolve the handle reference to a currently valid handle.
            data[key] = Handle[value.__kind][value.index]
            assert(data[key].is_valid)
         else
            resolve_handles(value)
         end
      end
   end
end

Serial.save = serpent.dump

function Serial.load(dump)
   local raw_data = serpent.load(dump)
   resolve_handles(raw_data)
   return dump
end

return Serial
