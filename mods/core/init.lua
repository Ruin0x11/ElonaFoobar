require "mods/core/setup"

local Defines = require 'mods/core/defines'
local Event = require 'mods/core/event'

function hoge()
   Elona.GUI.txt("hoge ")
   Elona.GUI.txt("Calling rnd: " .. Elona.Rand.rnd(20) .. " ")
   if Elona.Rand.one_in(10) then
      local pos = Elona.Map.random_pos()
      Elona.Magic.cast(432, 100, pos) -- ball
      Elona.Chara.player():damage_hp(10, 1)
   end
end

function foo()
   Elona.GUI.txt("foo ")
   if Elona.Rand.one_in(10) then
      local pos = Elona.Map.random_pos()
      Elona.Magic.cast(623, 100, pos) -- heal
   end
end

local function my_movement_handler(args)
   local chara_idx = args.chara_idx
   Elona.log(Elona.Debug.inspect(Elona))
   Elona.log("It is " .. chara_idx)
   local data = Elona.Registry.Data["core"].Chara[chara_idx]
   Elona.Registry.Data["core"].Chara[chara_idx].steps = data.steps + 1
   Elona.GUI.txt("Steps taken by " .. chara_idx .. ": " .. Elona.Registry.Data["core"].Chara[chara_idx].steps .. " ")
end

local function my_character_init()
  local defaults = {
    steps = 0
  }
  return defaults
end

Elona.Registry.register_chara_init(my_character_init)
Event.register(Defines.Events.chara_moved, my_movement_handler)
Event.register(Defines.Events.player_turn, hoge)
Event.register(Defines.Events.player_turn, foo)
