local Iter = require 'mods/core/iter'
local Item = Elona.Item
local Enums = Elona.Defines.Enums

for _, curse_state in pairs({Enums.CurseState.Blessed, Enums.CurseState.None, Enums.CurseState.Cursed}) do
   for pos in Iter.rectangle_iter(20, 20, 30, 30) do
      local items = Item.create(pos.x + (pos.y * 10) + 1, pos.x, pos.y, 1)
      if items then
         items.curse_state = curse_state
         items.identify_state = Enums.IdentifyState.Completely
      end
   end
end

Elona.Debug.dump_items()
Elona.Chara.player():damage_hp(10, 1)
