local Iter = Elona.require("Iter")
local Item = Elona.require("Item")
local Enums = Elona.require("Enums")
local Debug = Elona.require("Debug")
local Chara = Elona.require("Chara")
local Event = Elona.require("Event")

function spawn_items()
   for _, curse_state in pairs({Enums.CurseState.Blessed, Enums.CurseState.None, Enums.CurseState.Cursed}) do
      for pos in Iter.rectangle_iter(20, 20, 30, 30) do
         local items = Item.create(pos.x, pos.y, pos.x + (pos.y * 10) + 1, 1)
         if items then
            items.curse_state = curse_state
            items.identify_state = Enums.IdentifyState.Completely
         end
      end
   end

   Debug.dump_items()
   Chara.player():damage_hp(10, 1)
end

Event.register(Event.EventKind.GameInitialized, spawn_items)
