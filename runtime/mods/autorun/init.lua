--[[
Map.iter_memory()
FOV.iter()
Map.feature_at(x, y)
Map.chara_at(x, y)
Map.tile_at(x, y)
]]

local Event = Elona.require("Event")
local FOV = Elona.require("FOV")
local GUI = Elona.require("GUI")
local Input = Elona.require("Input")
local Chara = Elona.require("Chara")
local Macro = Elona.require("Macro")
local Map = Elona.require("Map")

local Pathing = require "pathing"

local autorun = false
local attacked = false
local pathing = nil

local function start_autorun(dest)
   attacked = false
   autorun = true
   pathing = Pathing.new(dest)
end

local function stop_autorun()
   attacked = false
   autorun = false
end

local function step_autorun()
   if not autorun then
      return
   end

   if Map.is_overworld() then
      GUI.txt("Can't explore in the overworld. Stopping. ")
      stop_autorun()
   end

   -- local esc = Input.is_key_held("escape")
   local esc = false

   -- if esc or attacked then
   --    if Input.yes_no("Being attacked. Stop? ") then
   --       GUI.txt("Stopping. ")
   --       stop_autorun()
   --       return
   --    end
   -- end

   attacked = false

   local next_action = pathing:get_action()

   if next_action then
      Macro.enqueue(next_action)
      Macro.ignore_wait()
   else
      stop_autorun()
      GUI.txt("Travel finished. ")
   end
end

local function explore()
   start_autorun(nil)
   return true
end

local function travel()
   local dest = Input.prompt_position("Where to go? ")
   if dest then
      print(dest.x .. " " .. dest.y)
      if not Map.valid(dest) or not Pathing.is_tile_memorized(dest) then
         GUI.txt("You can't go there. ")
         return false
      end
      if Map.is_solid(dest) or Pathing.chara_blocks(dest) then
         GUI.txt("The destination is blocked. ")
         return false
      end

      GUI.txt("Traveling. ")
      start_autorun(dest)
      return true
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
   local result = Input.prompt_choice("Travel", "Explore")

   -- TODO: Needs to restart the player's turn without running all
   -- other character turns, in order for the macro to trigger
   if result == 1 then
      return travel()
   elseif result == 2 then
      return explore()
   end

   return false
end

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
      stop = stop_autorun
   }
}
