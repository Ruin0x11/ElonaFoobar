require "tests/lua/support/minctest"

local Item = Elona.require("Item")

lrun("test Item.create", function()
        Testing.start_in_debug_map()

        local item_pos = LuaPosition.new(4, 8)
        local item = Item.create(item_pos, "core.putitoro", 3)
        lok(item ~= nil, "Item was nil")
        lequal(item.number, 3)
        lequal(item.position.x, item_pos.x)
        lequal(item.position.y, item_pos.y)

        item = Item.create(item_pos.x, item_pos.y + 1, "core.putitoro", 3)
        lok(item ~= nil, "Item was nil")
        lequal(item.number, 3)
        lequal(item.position.x, item_pos.x)
        lequal(item.position.y, item_pos.y + 1)
end)

lrun("test Item.has_enchantment", function()
        Testing.start_in_debug_map()

        local item_pos = LuaPosition.new(4, 8)
        local item = Item.create(item_pos, "core.dragon_slayer", 3) -- spawns with fixed enchantments
        lequal(Item.has_enchantment(item, 57), true)
        lequal(Item.has_enchantment(item, 20), false)
end)

lrun("test Item.count", function()
        Testing.start_in_debug_map()

        Item.create(0, 0, "core.putitoro", 3)
        Item.create(0, 1, "core.putitoro", 3)

        lequal(Item.count(), 2)

        Item.create(0, 2, "core.putitoro", 3)

        lequal(Item.count(), 3)
end)

local function tally()
   print("=====")
   local count = 0
   for _, i in Item.iter(5080, 5480) do
      print(i.basename)
      count = count + 1
   end
   return count
end

lrun("test Item.iter", function()
        Testing.start_in_debug_map()

        Item.create(0, 0, "core.putitoro", 3)
        Item.create(0, 1, "core.putitoro", 3)

        local count = tally()
        lequal(count, 2)

        Item.create(0, 2, "core.putitoro", 3)

        count = tally()
        lequal(count, 3)
end)
