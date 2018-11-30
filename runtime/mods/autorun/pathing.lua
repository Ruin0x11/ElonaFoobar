local Chara = Elona.require("Chara")
local FOV = Elona.require("FOV")
local Iter = Elona.require("Iter")
local Map = Elona.require("Map")

local function is_tile_memorized(pos)
   -- Elona has no concept of never-seen tiles, because tile memory is
   -- initialized to the "wall" type on generation, so some tiles will
   -- already be correct in tile memory the first time the map is
   -- entered. The best that can be done for now is to check if the
   -- memory is different from the actual tile.
   local memory = Map.get_memory(pos.x, pos.y)
   local tile = Map.get_tile(pos.x, pos.y)

   return memory ~= -1 and memory == tile
end

local function is_valid_explore_target(target)
   for pos in Iter.rectangle_iter(-1, -1, 1, 1) do
      if pos.x ~= 0 and pos.y ~= 0 then
         if Map.valid(target.x + pos.x, target.y + pos.y) then
            if not is_tile_memorized({x = target.x + pos.x, y = target.y + pos.y}) then
               return true
            end
         end
      end
   end
   return false
end

local function points_equal(a, b)
   return a.x == b.x and a.y == b.y
end

local function chara_blocks(pos)
   local chara = Map.get_chara(pos.x, pos.y)
   return chara
      and FOV.you_see(chara.position)
      and (chara.relationship == "Aggressive" or chara.relationship == "Nonaggressive")
end

local function is_safe_to_travel(pos)
   if Map.is_solid(pos.x, pos.y) then
      return false
   end

   if not is_tile_memorized(pos) then
      return false
   end

   if chara_blocks(pos) then
      return false
   end

   return true
end

local function make_distance_map()
   local m = {}
   for i=0, Map.width() - 1 do
      m[i] = {}
      for j=0, Map.height() - 1 do
         m[i][j] = 0
      end
   end
   return m
end


local Pathfinder = {}

function Pathfinder:update_unexplored_pos(pos, next_pos)
   if not is_tile_memorized(next_pos) then
      if self.traveled_dist < self.unexplored_dist or self.unexplored_dist < 0 then
         self.unexplored_dist = self.traveled_dist
         self.unexplored_place = pos
         print("found place " .. pos.x .. "," .. pos.y)
      end
   end
end

function Pathfinder:mark_point(next_pos)
   if self.distance_map[next_pos.x][next_pos.y] == 0 then
      self.next_points = self.next_points + 1
      self.circ[not self.ring][self.next_points] = next_pos
      self.distance_map[next_pos.x][next_pos.y] = self.traveled_distance;
   end
end

function Pathfinder:flood(pos, next_pos)
   if not Map.valid(next_pos.x, next_pos.y) then
      return false
   end

   if self.explore then
      self:update_unexplored_pos(pos, next_pos)

      if self.unexplored_dist >= 0 then
         return true
      end
   end

   if not self.explore and points_equal(self.dest, next_pos) then
      self.next_move = pos
      return true
   end

   if is_safe_to_travel(next_pos) then
      self:mark_point(next_pos)
   end

   return false
end

local flood_dirs = {
   { x =  0, y = -1 },
   { x =  1, y =  0 },
   { x =  0, y =  1 },
   { x = -1, y =  0 },
   { x = -1, y = -1 },
   { x = -1, y =  1 },
   { x =  1, y = -1 },
   { x =  1, y =  1 },
   { x =  1, y =  1 },
}

function Pathfinder:examine_point(pos)
   if not Map.valid(pos.x, pos.y) then
      return false
   end

   local found_target = false

   for i=1,8 do
      local next_pos = { x = pos.x + flood_dirs[i].x, y = pos.y + flood_dirs[i].y }
      if self:flood(pos, next_pos) then
         found_target = true
      end
   end

   return found_target
end

function Pathfinder:pathfind(start, dest)
   if not is_safe_to_travel(start) then
      return { x = -1, y = -1 }
   end

   if not self.explore and points_equal(start, dest) then
      return start
   end

   self.dest = dest
   self.distance_map = make_distance_map()
   self.next_move = { x = -1, y = -1 }
   self.unexplored_place = { x = -1, y = -1 }
   self.unexplored_dist = -1
   self.next_points = 0
   self.traveled_distance = 1
   self.ring = false

   self.circ = {}
   self.circ[false] = {}
   self.circ[true] = {}
   self.circ[self.ring][1] = start

   local points = 1

   while points > 0 do
      for i=1,points do
         local pos = self.circ[self.ring][i]
         local reached = self:examine_point(pos)
         if reached then
            if self.explore then
               return self.unexplored_place
            else
               return self.next_move
            end
         end
      end

      points = self.next_points
      self.next_points = 0
      self.ring = not self.ring
      self.traveled_distance = self.traveled_distance + 1
   end

   if self.explore then
      return self.unexplored_place
   else
      return self.next_move
   end
end

function Pathfinder.new()
   local s = {}
   s.dest = nil
   s.avoid_danger = true
   s.circ = nil
   s.next_points = 0
   s.points = 0
   s.distance_map = make_distance_map()
   s.next_move = { x = -1, y = -1 }
   s.unexplored_place = { x = -1, y = -1 }
   s.unexplored_dist = -1
   s.traveled_dist = 0

   for k, v in pairs(Pathfinder) do
      s[k] = v
   end

   return s
end


local Pathing = {}

local function find_target_square(pos)
   local state = Pathfinder.new()
   local target = state:pathfind(pos)

   if target.x == -1 then
      -- Path through dangerous tiles.
      state = Pathfinder.new()
      state.avoid_danger = false
      target = state:pathfind(pos)
   end

   if target.x ~= -1 then
      -- Fail if tile is unreachable
      if state.distance_map[target.x][target.y] <= 0 and not points_equal(pos, target) then
         target.x = -1
         target.y = -1
      end
   end

   if target.x ~= -1 then
      return target
   else
      return { x = -1, y = -1 }
   end
end

local function calc_move_towards_dest(pos, dest)
   local state = Pathfinder.new()

   if dest.x == -1 and dest.y == -1 then
      return { x = 0, y = 0 }
   end

   local target = state:pathfind(dest, pos)
   if target.x == -1 then
      state = Pathfinder.new()
      state.avoid_danger = false
      target = state:pathfind(dest, pos)
   end

   if target.x ~= -1 then
      return { x = target.x - pos.x, y = target.y - pos.y }
   else
      return { x = 0, y = 0 }
   end
end

local function delta_to_command(dx, dy)
   local lookup = {
      [-1] = { [-1] = "northwest", [0] = "north", [1] = "northeast"},
      [0]  = { [-1] = "west",                     [1] = "east"},
      [1]  = { [-1] = "southwest", [0] = "south", [1] = "southeast"},
   }
   return lookup[dy][dx]
end

function Pathing.new(dest)
   local e = {}
   e.explore = dest == nil
   if e.explore then
      e.dest = { x = -1, y = -1 }
   else
      e.dest = dest
   end

   for k, v in pairs(Pathing) do
      e[k] = v
   end

   return e
end

function Pathing:has_explore_target()
   return self.dest and self.dest.x ~= -1 and is_valid_explore_target(self.dest)
end

function Pathing:get_action()
   local player = Chara.player()

   if self.explore and not self:has_explore_target() then
      self.dest = find_target_square(player.position)
   end

   local move = calc_move_towards_dest(player.position, self.dest)

   if move.x == 0 and move.y == 0 then
      return nil
   end

   return delta_to_command(move.x, move.y)
end

Pathing.is_tile_memorized = is_tile_memorized
Pathing.chara_blocks = chara_blocks

return Pathing
