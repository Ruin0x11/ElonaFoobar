local function prepare_tileset(id)
   local tileset = data.raw["core.map_tileset"][id]
   if tileset == nil then
      error("No such tileset " .. id)
   end

   tiles = tileset.tiles
   if tiles == nil then
      error("No tiles on tileset " .. id)
   end

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

return prepare_tileset
