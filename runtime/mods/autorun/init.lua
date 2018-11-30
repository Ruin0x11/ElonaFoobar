local Chara = Elona.require("Chara")
local Event = Elona.require("Event")
local GUI = Elona.require("GUI")
local Input = Elona.require("Input")
local Iter = Elona.require("Iter")
local Macro = Elona.require("Macro")
local Map = Elona.require("Map")
local table = Elona.require("table")

local Pathing = require "pathing"

local autorun = false
local attacked = false
local pathing = nil

local function start_autorun(dest, waypoint)
   attacked = false
   autorun = true
   pathing = Pathing.new(dest, waypoint)
end

local function stop_autorun()
   attacked = false
   autorun = false
end

local function step_autorun()
   if not autorun then
      return
   end

   if Chara.player():get_ailment("Confused") > 0 then
      GUI.txt("You are confused. Stopping. ")
      stop_autorun()
      return
   end

   if attacked then
      stop_autorun()
      return
   end

   -- local esc = Input.is_key_held("escape")

   local next_action = pathing:get_action()

   if next_action then
      Macro.enqueue(next_action)
      Macro.ignore_wait()
   else
      stop_autorun()
   end
end


local function travel()
   local pos = Chara.player().position
   local dest = Input.prompt_position("Where to go? ", pos)
   if dest then
      if dest.x == pos.x and dest.y == pos.y then
         GUI.txt("You're already there! ")
         return false
      end
      if not Pathing.is_safe_to_travel(dest) then
         Pathing.print_halt_reason(dest)
         return false
      end

      GUI.txt("Traveling. ")
      start_autorun(dest, nil)
      return true
   end

   return false
end

local function explore()
   if Map.is_overworld() then
      GUI.txt("You can't explore in the overworld. ")
      stop_autorun()
      return false
   end

   start_autorun(nil, nil)
   return true
end

-- TODO: Make magic constants more understandable.
local FEAT_STAIRS_UP = 232
local FEAT_STAIRS_DOWN = 231

local function waypoints()
   local keys = {}
   local waypoint_list = {}

   for key, waypoint in pairs(data.raw["autorun.waypoint"]) do
      if Map.new_id() == waypoint.map_id and Map.dungeon_level() == waypoint.map_level then
         waypoint_list[#keys+1] = waypoint
         keys[#keys+1] = key
      end
   end

   for pos in Iter.rectangle_iter(0, 0, Map.width() - 1, Map.height() - 1) do
      if Pathing.is_tile_memorized(pos) then
         local feat = Map.get_feat(pos.x, pos.y)
         if feat > 0 then
            print(feat)
         end
         if feat == FEAT_STAIRS_UP then
            waypoint_list[#keys+1] = { pos = { x = pos.x, y = pos.y } }
            keys[#keys+1] = "Stairs up (" .. pos.x .. "," .. pos.y .. ")"
         elseif feat == FEAT_STAIRS_DOWN then
            waypoint_list[#keys+1] = { pos = { x = pos.x, y = pos.y } }
            keys[#keys+1] = "Stairs down (" .. pos.x .. "," .. pos.y .. ")"
         end
      end
   end

   if #keys == 0 then
      GUI.txt("No waypoints available for this area. ")
      return false
   end

   GUI.txt("Which? ")
   local choice = Input.prompt_choice(table.unpack(keys))

   if choice then
      local waypoint = waypoint_list[choice]
      if waypoint then
         print(waypoint.pos.x .. "," .. waypoint.pos.y)
         GUI.txt("Traveling. ")
         start_autorun(waypoint.pos, waypoint)
         return true
      end
   end

   return false
end


local function on_damaged(chara)
   if Chara.is_player(chara) then
      attacked = true
   end
end

local function on_killed(chara)
   if chara and Chara.is_player(chara) then
      stop_autorun()
   end
end

local function on_map_initialized()
   stop_autorun()
   Elona.require("Item").create(Chara.player().position, "autorun.autorun_tester")
end

local Exports = {}
Exports.on_use = {}

function Exports.on_use.autorun_tester()
   GUI.txt("What to do? ")
   local result = Input.prompt_choice("Travel", "Explore", "Waypoints")

   -- TODO: Needs to restart the player's turn without running all
   -- other character turns, in order for the macro to trigger
   if result == 1 then
      return travel()
   elseif result == 2 then
      return explore()
   elseif result == 3 then
      return waypoints()
   end

   return false
end

-- TODO: It would be far better to integrate this with the continuous
-- action/activity system, to allow things like player damage to be
-- consistently handled. It makes logical sense since control is taken
-- away from the player.
Event.register(Event.EventKind.PlayerTurn, step_autorun)

Event.register(Event.EventKind.CharaDamaged, on_damaged)
Event.register(Event.EventKind.CharaKilled, on_killed)
Event.register(Event.EventKind.MapInitialized, on_map_initialized)
Event.register(Event.EventKind.GameInitialized, on_map_initialized)
-- Event.register(Event.EventKind.MenuEntered, Macro.clear_queue)


return {
   Exports = Exports,
   Autorun = {
      travel = travel,
      explore = explore,
      waypoints = waypoints,
      stop = stop_autorun
   }
}
