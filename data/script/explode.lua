local Chara = Elona.require("Chara")
local Enums = Elona.require("Enums")
local Event = Elona.require("Event")
local Item = Elona.require("Item")
local Iter = Elona.require("Iter")
local GUI = Elona.require("GUI")

local function arm_bomb(chara)
   if not Chara.is_player(chara) and not Chara.is_ally(chara) then
      chara:set_flag(Enums.CharaFlag.Explodes, true)
   end
end

local function spawn_charas()
   for pos in Iter.rectangle_iter(20, 20, 24, 24) do
      Chara.create(pos.x, pos.y, 3)
   end
   Item.create(Chara.player().position, 236, 1)
end

Event.register(Event.EventKind.CharaCreated, arm_bomb)
Event.register(Event.EventKind.CharaRefreshed, arm_bomb) -- because flags are reset on chara refresh
Event.register(Event.EventKind.GameInitialized, spawn_charas)
