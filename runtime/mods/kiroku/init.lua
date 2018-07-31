local Enums = Elona.require("Enums")
local Chara = Elona.require("Chara")
local Event = Elona.require("Event")
local GUI = Elona.require("GUI")
local I18N = Elona.require("I18N")
-- local Registry = Elona.require("Registry")

local function on_chara_killed(_victim, killer)
   if killer and Chara.is_player(killer) then
      Store.global.killed = Store.global.killed + 1
      if Store.global.report then
         GUI.txt("Killed: " .. Store.global.killed .. " so far. ")
      end
   end
end

local function report()
   GUI.txt(I18N.get("kiroku.locale.on_killed", Store.global.killed))
end

local function init_store()
   print(Elona.require("Debug").inspect.inspect(Store.global))
   if not Store.global.killed then
      Store.global.killed = 0
   end
   Store.global.report = true
end

local Exports = {}

Exports.on_use = {}

function Exports.on_use.counter(item, chara)
   GUI.txtef(Enums.Color.Gold)
   GUI.txt(item.name .. " : " .. chara.name)
   report()
   return true
end

Event.register(Event.EventKind.CharaKilled, on_chara_killed)
Event.register(Event.EventKind.MapInitialized, function() if Store.global.report then report() end end)
Event.register(Event.EventKind.GameSaved, report)
Event.register(Event.EventKind.GameLoaded, report)
Event.register(Event.EventKind.GameInitialized, init_store)

return {
   Store = Store,
   Exports = Exports
}
