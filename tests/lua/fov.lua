require "tests/lua/minctest"

local FOV = Elona.FOV

lrun("test FOV.refresh", function()
        Testing.start_in_debug_map()

        local pos = Elona.Chara.player().position
        local other = LuaPosition.new(pos.x, pos.y + 5)
        lequal(FOV.you_see(other), true)

        Elona.Map.set_tile(pos.x, pos.y + 2, Elona.Defines.Enums.TileKind.Wall)
        lequal(FOV.you_see(other), true)

        FOV.refresh()
        lequal(FOV.you_see(other), false)
end)

lrun("test FOV.los", function()
        Testing.start_in_debug_map()

        local first = LuaPosition.new(5, 5)
        local second = LuaPosition.new(5, 10)
        lequal(FOV.los(first, second), true)
        lequal(FOV.los(first.x, first.y, second.x, second.y), true)

        Elona.Map.set_tile(5, 7, Elona.Defines.Enums.TileKind.Wall)
        FOV.refresh()
        lequal(FOV.los(first, second), false)
        lequal(FOV.los(first.x, first.y, second.x, second.y), false)
end)

lrun("test FOV.you_see", function()
        Testing.start_in_debug_map()

        local pos = Elona.Chara.player().position
        local chara = Elona.Chara.create(pos.x, pos.y + 5, 3) -- putit
        FOV.refresh()
        lequal(FOV.you_see(chara), true)
        lequal(FOV.you_see(chara.position), true)
        lequal(FOV.you_see(chara.position.x, chara.position.y), true)
        Elona.Map.set_tile(pos.x, pos.y + 2, Elona.Defines.Enums.TileKind.Wall)

        FOV.refresh()
        lequal(FOV.you_see(chara), false)
        lequal(FOV.you_see(chara.position), false)
        lequal(FOV.you_see(chara.position.x, chara.position.y), false)
end)

assert(lresults())
