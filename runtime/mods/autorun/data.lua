local table = Elona.require("table")

require("data/item")
require("data/waypoint")

-- Make the autorun tester appear in the general vendor's inventory.
local general_vendor = data.raw["core.shop_inventory"]["core.general_vendor"]
table.insert(general_vendor.rules, { index = 0, id = "autorun.autorun_tester" })
