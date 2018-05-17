local Item = Elona.Item

local items = Item.create(242, 24, 24, 5)
items.curse_state = Elona.Defines.Enums.CurseState.Cursed
items.identify_state = Elona.Defines.Enums.IdentifyState.Unidentified
items.count = 999
Elona.Debug.dump_items()
Elona.Chara.player():damage_hp(10, 1)
