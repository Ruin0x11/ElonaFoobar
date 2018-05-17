local Iter = require 'mods/core/iter'
local Map = Elona.Map
local Enums = Elona.Defines.Enums
local Storage = Elona.Registry.Data["core"]

function my_map_init(w, h)
   grid = {}
   for i = 1, w do
      grid[i] = {}

      for j = 1, h do
         grid[i][j] = 0
      end
   end
   return {grid=grid}
end

-- Appropriated from https://rosettacode.org/wiki/Conway%27s_Game_of_Life#Lua
-- It's a bit messy.
function evolve(grid)
   local m = #cell
   local cell2 = {}
   for i = 1, m do
      cell2[i] = {}
      for j =1, m do
         cell2[i][j] = cell[i][j]
      end
   end

   for i = 1, m do
      for j = 1, m do
         local count
         if cell2[i][j] == 0 then count = 0 else count = -1 end
         for x = -1, 1 do
            for y = -1, 1 do
               if i+x >= 1 and i+x <= m
                  and j+y >= 1 and j+y <= m
                  and cell2[i+x][j+y] == 1
               then
                  count = count + 1
               end
            end
         end
         if count < 2 or count > 3 then cell[i][j] = 0 end
         if count == 3 then cell[i][j] = 1 end
      end
   end
end

function run_life()
   local grid = Storage.Map.grid
   for i = 1, Map.width() do
      for j = 1, Map.height() do
         if Storage.Map.grid[i][j] == 1 and Map.can_access(i, j) then
            -- TODO: replace with items?
            Map.set_tile(i, j, Enums.TileType.Wall)
            Map.set_tile_memory(i, j, Enums.TileType.Wall)
         else
            Map.set_tile_memory(i, j, Enums.TileType.Normal)
            Map.set_tile(i, j, Enums.TileType.Normal)
         end
      end
   end
   Storage.Map.grid = evolve(grid)
end

Elona.Registry.register_map_init(my_map_init)
Event.register("all_turns_passed", run_life)
