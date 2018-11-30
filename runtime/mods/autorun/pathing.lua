local Chara = Elona.require("Chara")
local FOV = Elona.require("FOV")
local GUI = Elona.require("GUI")
local Iter = Elona.require("Iter")
local Map = Elona.require("Map")

local function is_tile_memorized(pos)
   local memory = Map.get_memory(pos.x, pos.y)
   local tile = Map.get_tile(pos.x, pos.y)

   return memory ~= 0 and memory == tile
end

local CLOSED_DOOR = 726

-- Like Map.is_solid(), but returns false for closed doors.
local function is_solid(pos)
   if Map.is_solid(pos.x, pos.y) then
      return Map.get_feat(pos.x, pos.y) ~= CLOSED_DOOR
   end

   return false
end

local function points_equal(a, b)
   return a.x == b.x and a.y == b.y
end

local function is_valid_explore_target(target)
   if points_equal(target, Chara.player().position) then
      return false
   end

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

local function chara_blocks(pos)
   local chara = Map.get_chara(pos.x, pos.y)
   return chara
      and not Chara.is_player(chara)
      and FOV.you_see(chara.position)
      and (chara.relationship == "Aggressive"
              or chara.relationship == "Nonagressive"
              or chara.relationship == "Neutral")
end

local function mef_blocks(pos)
   return Map.get_mef(pos.x, pos.y) ~= 0
end

local function is_safe_to_travel(pos)
   if is_solid(pos) then
      return false
   end

   if not is_tile_memorized(pos) then
      return false
   end

   if chara_blocks(pos) then
      return false
   end

   if mef_blocks(pos) then
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

function Pathfinder:is_exploring()
   return self.dest == nil
end

function Pathfinder:update_unexplored_pos(pos, next_pos)
   -- HACK: For some reason, by the time the destination is reached
   -- the memorized tiles might not be updated, causing the reached
   -- destination to still be marked as unexplored.
   if points_equal(pos, Chara.player().position) then
      return
   end

   if not is_tile_memorized(next_pos) then
      if self.traveled_dist < self.unexplored_dist or self.unexplored_dist < 0 then
         self.unexplored_dist = self.traveled_dist
         self.unexplored_place = pos
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

   if self:is_exploring() then
      self:update_unexplored_pos(pos, next_pos)

      if self.unexplored_dist >= 0 then
         return true
      end
   end

   if not self:is_exploring() and points_equal(self.dest, next_pos) then
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

   self.dest = dest

   if not self:is_exploring() and points_equal(start, dest) then
      return start
   end

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
            if self:is_exploring() then
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

   if self:is_exploring() then
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

local function print_halt_reason(dest)
   if not Map.valid(dest.x, dest.y) then
      GUI.txt("There's no unblocked path available. ")
   elseif not Pathing.is_tile_memorized(dest) then
      GUI.txt("You can't go there. ")
   elseif is_solid(dest)  then
      GUI.txt("The destination is blocked. ")
   elseif chara_blocks(dest) then
      local chara = Map.get_chara(dest.x, dest.y)
      GUI.txt(chara.name .. " is in the way. ")
   elseif mef_blocks(dest) then
      GUI.txt("Something dangerous is there. ")
   else
      GUI.txt("There's no unblocked path available. ")
   end
end

function Pathing:on_halt()
   local pos = Chara.player().position
   local not_reached = not points_equal(self.dest, pos)

   if not_reached then
      print_halt_reason(self.dest)
      GUI.txt("Aborting travel. ")
   else
      GUI.txt("Travel finished. ")
   end
end

function Pathing:get_action()
   local player = Chara.player()
   local pos = {x = player.position.x, y = player.position.y}

   if self.explore and not self:has_explore_target() then
      self.dest = find_target_square(pos)
   end

   local move = calc_move_towards_dest(pos, self.dest)

   if move.x == 0 and move.y == 0 then
      self:on_halt()
      return nil
   end

   return delta_to_command(move.x, move.y)
end

Pathing.is_tile_memorized = is_tile_memorized
Pathing.is_safe_to_travel = is_safe_to_travel
Pathing.chara_blocks = chara_blocks
Pathing.print_halt_reason = print_halt_reason

return Pathing
