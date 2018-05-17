require "mods/core/setup.lua"

local Defines = require 'mods/core/defines'
local Event = require 'mods/core/event'
local Storage = Elona.Registry.Data["core"]

function hoge()
   Elona.GUI.txt("hoge ")
   Elona.GUI.txt("Calling rnd: " .. Elona.Rand.rnd(20) .. " ")
   if Elona.Rand.one_in(10) then
      local pos = Elona.Map.random_pos()
      Elona.Magic.cast(432, 100, pos) -- ball
      Elona.Chara.player():damage_hp(10, 1)
   end
end


local function dump(o)
   if type(o) == 'table' then
      local s = '{ '
      for k,v in pairs(o) do
         if type(k) ~= 'number' then k = '"'..k..'"' end
         s = s .. '['..k..'] = ' .. dump(v) .. ','
      end
      return s .. '} '
   else
      return tostring(o)
   end
end


function foo()
   Elona.GUI.txt("foo ")
   if Elona.Rand.one_in(10) then
      local pos = Elona.Map.random_pos()
      Elona.Magic.cast(623, 100, pos) -- heal
   end
end

local function my_movement_handler(chara_idx)
   Elona.GUI.txt(dump(Storage))
   local data = Storage.Chara[chara_idx]
   local asd = data.steps
   Storage.Chara[chara_idx].steps = asd + 1
   Elona.GUI.txt("Steps taken by " .. chara_idx .. ": " .. Storage.Chara[chara_idx].steps .. " ")
end

local function my_character_init()
  --  Elona.log("Initing char " .. id)
  -- local defaults = {
  --   steps = 0
  -- }
  -- return defaults
   return {}
end

Elona.Registry.register_chara_init(my_character_init)
Event.register(Defines.Events.chara_moved, my_movement_handler)
Event.register(Defines.Events.player_turn, hoge)
Event.register(Defines.Events.player_turn, foo)
