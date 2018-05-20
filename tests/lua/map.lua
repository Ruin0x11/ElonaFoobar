require "tests/lua/minctest"

lrun("test Map.width/Map.height", function()
        Elona.Testing.start_in_debug_map()
        lequal(Elona.Map.width(), 50)
        lequal(Elona.Map.height(), 50)
end)

lresults()
