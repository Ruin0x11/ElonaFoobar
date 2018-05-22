require "tests/lua/minctest"

local Map = Elona.Map

lrun("test Map.width/Map.height", function()
        Testing.start_in_debug_map()
        lequal(Map.width(), 50)
        lequal(Map.height(), 50)
end)

lrun("test Map.valid", function()
        Testing.start_in_debug_map()
        lequal(Map.valid(0, 0), true)
        lequal(Map.valid(35, 1), true)
        lequal(Map.valid(305, 1), false)
        lequal(Map.valid(-1, 0), false)
end)

lrun("test Map.can_access", function()
        Testing.start_in_debug_map()
        lequal(Map.can_access(5, 5), true)
        Map.set_tile(5, 5, Elona.Defines.Enums.TileKind.Wall)
        lequal(Map.can_access(5, 5), false)
end)

assert(lresults())
