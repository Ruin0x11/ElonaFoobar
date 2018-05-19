require "mods/core/setup"
local Defines = Elona.Defines
local Storage = Elona.Registry.Data["core"]

local function hoge()
   Elona.GUI.txt("hoge ")
   Elona.GUI.txt("Calling rnd: " .. Elona.Rand.rnd(20) .. " ")
   if Elona.Rand.one_in(10) then
      local pos = Elona.Map.random_pos()
      Elona.Magic.cast(432, 100, pos) -- Ball magic
      Elona.Chara.player():damage_hp(10, 1)
   end
end

local function foo()
   Elona.GUI.txt("foo ")
   if Elona.Rand.one_in(10) then
      local pos = Elona.Map.random_pos()
      Elona.Magic.cast(623, 100, pos) -- Heal
      Elona.Chara.create(pos, 3) -- *ぷちゅ*
   end
end

local function my_movement_handler(args)
   local chara_idx = args.chara_idx
   local data = Storage.Chara[chara_idx]
   Storage.Chara[chara_idx].steps = data.steps + 1
   Elona.GUI.txt("Steps taken by " .. chara_idx .. ": " .. Storage.Chara[chara_idx].steps .. " ")
end

local function my_character_init()
  return {steps = 0} -- Storage.Chara[chara_idx].steps
end

Elona.Registry.register_chara_init(my_character_init)
Elona.Event.register(Defines.Events.chara_moved, my_movement_handler)
Elona.Event.register(Defines.Events.player_turn, hoge)
Elona.Event.register(Defines.Events.player_turn, foo)
