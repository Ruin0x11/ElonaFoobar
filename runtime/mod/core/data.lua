require("data/chara")
require("data/item")
require("data/chara_chip")
require("data/item_chip")
require("data/portrait")
require("data/music")
require("data/sound")
require("data/dialog")
require("data/chara_drop")
require("data/shop_inventory")
require("data/map")
require("data/trait")
require("data/fish")
require("data/ability")
require("data/god")
require("data/item_material")
require("data/race")
require("data/class")
require("data/buff")
require("data/asset")
require("data/map_chip")
require("data/event")
require("data/map_tileset")


-- TODO: this should be moved to a data editing stage when that
-- feature gets added.
local function prepare_tileset(id)
   local tileset = data.raw["core.map_tileset"][id]
   if tileset == nil then
      error("No such tileset " .. id)
   end

   local tiles = tileset.tiles
   if tiles == nil then
      error("No tiles on tileset " .. id)
   end

   local table = Elona.require("table")
   local this = tileset
   while this.parent ~= nil do
      local parent = data.raw["core.map_tileset"][this.parent]
      if parent == nil then
         error("Could not find parent tileset " .. this.parent)
      end
      if type(parent) == "function" then
         error("Parent tileset cannot be function: " .. this.parent)
      end

      if type(tiles) == "function" then
         local old_tiles = tiles
         tiles = function(e)
            return table.merge(parent.tiles, old_tiles(e))
         end
      else
         tiles = table.merge(parent.tiles, tiles)
      end

      this = parent
   end

   return tiles
end

for k, v in pairs(data.raw["core.map_tileset"]) do
   v.tiles = prepare_tileset(k)
end
