package.path = package.path .. ";./?.lua"
local Event = require 'event'

function hoge()
   Elona.GUI.txt("test")
   Elona.GUI.txt("Calling rnd: " .. Elona.Rand.rnd(20))
   if Elona.Rand.one_in(10) then
      local pos = Elona.Map.random_pos()
      Elona.Magic.cast(432, 100, pos) -- ball
   end
end

function foo()
   Elona.GUI.txt("Second caller")
   if Elona.Rand.one_in(10) then
      local pos = Elona.Map.random_pos()
      Elona.Magic.cast(623, 100, pos) -- heal
   end
end

Event.register("player_turn", hoge)
Event.register("player_turn", foo)
