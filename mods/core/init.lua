local Event = require 'mods/core/event'

function hoge()
   Elona.GUI.txt("test ")
   Elona.GUI.txt("Calling rnd: " .. Elona.Rand.rnd(20) .. " ")
   if Elona.Rand.one_in(10) then
      local pos = Elona.Map.random_pos()
      Elona.Magic.cast(432, 100, pos) -- ball
      Elona.Chara.player():damage_hp(10, 1)
   end
end
--
-- function foo()
--    Elona.GUI.txt("Second caller ")
--    if Elona.Rand.one_in(10) then
--       local pos = Elona.Map.random_pos()
--       Elona.Magic.cast(623, 100, pos) -- heal
--    end
-- end

local function my_movement_handler(args)
   local chara = args.chara
   local zxc = Elona.Registry.Data["core"]
   local data = zxc.Chara[chara]
   local asd = data.steps
   Elona.Registry.Data["core"].Chara[chara].steps = asd + 1
   Elona.GUI.txt("Steps taken by " .. chara .. ": " .. Elona.Registry.Data["core"].Chara[chara].steps .. " ")
end

local function my_character_init(id)
   Elona.log("Initing char " .. id)
  local defaults = {
    steps = 0
  }
  return defaults
end

Elona.Registry.register_chara_init(my_character_init)
Event.register("chara_moved", my_movement_handler)
Event.register("player_turn", hoge)
