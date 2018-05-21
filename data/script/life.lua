local Map = Elona.Map
local Enums = Elona.Defines.Enums
local EventKind = Elona.Defines.EventKind

local function my_map_init(w, h)
   local grid = {}
   for i = 1, w do
      grid[i] = {}

      for j = 1, h do
         grid[i][j] = Elona.Rand.rnd(2)
      end
   end
   return {grid=grid} -- Now you can use Storage.Map.grid
end

-- Appropriated from https://rosettacode.org/wiki/Conway%27s_Game_of_Life#Lua
-- It's a bit messy.
local function evolve(cell)
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
   return cell
end

local function run_life()
   local grid = Store.grid
   if grid == nil then
      Store.grid = my_map_init(Map.width(), Map.height()).grid
      assert(Store.grid ~= nil)
      grid = Store.grid
   end
   for y = 1, Map.width() do
      for x = 1, Map.height() do
         if Store.grid[x][y] == 1 and Map.can_access(x, y) then
            Map.set_tile(x, y, Enums.TileKind.Wall)
            Map.set_tile_memory(x, y, Enums.TileKind.Wall)
         else
            Map.set_tile(x, y, Enums.TileKind.Room)
            Map.set_tile_memory(x, y, Enums.TileKind.Room)
         end
      end
   end
   Store.grid = evolve(grid)
end

Elona.Event.register(EventKind.MapInitialized, run_life)
Elona.Event.register(EventKind.AllTurnsFinished, run_life)
