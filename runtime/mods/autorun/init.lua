local Event = Elona.require("Event")
local GUI = Elona.require("GUI")
local Rand = Elona.require("Rand")
local Chara = Elona.require("Chara")
local Macro = Elona.require("Macro")

local function delta_to_command(dx, dy)
   local lookup = {
      [-1] = { [-1] = "northwest", [0] = "north", [1] = "northeast"},
      [0]  = { [-1] = "west",                     [1] = "east"},
      [1]  = { [-1] = "southwest", [0] = "south", [1] = "southeast"},
   }
   return lookup[dy][dx]
end


local Exports = {}
Exports.on_use = {}

function Exports.on_use.autorun_tester()
   for i=0,5 do
      local dx = Rand.rnd(3) - 1
      local dy = Rand.rnd(3) - 1
      local command = delta_to_command(dx, dy)
      if not command then
         command = "wait"
      end
      GUI.txt("Queue: " .. command)
      Macro.enqueue(command)
      Macro.ignore_wait()
   end
   return true
end

Event.register(Event.EventKind.CharaDamaged,
               function(chara)
                  -- TODO: Check damage source
                  if Chara.is_player(chara) then
                     GUI.txt("Stopping autorun.")
                     Macro.clear_queue()
                  end
end)
-- Event.register(Event.EventKind.MenuEntered, Macro.clear_queue)


return {
   Exports = Exports
}
