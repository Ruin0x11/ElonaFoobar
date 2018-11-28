require "tests/lua/support/minctest"

local Macro = Elona.require("Macro")

local Chara = Elona.require("Chara")
local Enums = Elona.require("Enums")
local Map = Elona.require("Map")

lrun("test Macro.enqueue", function()
        Testing.start_in_debug_map()

        local pos = Chara.player().position

        Map.set_tile(pos.x, pos.y + 2, Map.generate_tile(Enums.TileKind.Wall))
        FOV.refresh()
        lequal(FOV.you_see(chara), false)
        lequal(FOV.you_see(chara.position), false)
        lequal(FOV.you_see(chara.position.x, chara.position.y), false)
end)
