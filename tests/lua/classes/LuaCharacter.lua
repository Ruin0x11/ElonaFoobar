require "tests/lua/support/minctest"

lrun("test LuaCharacter:damage_hp", function()
        Testing.start_in_debug_map()

        local putit = Elona.Chara.create(0, 0, 3)

        putit:damage_hp(1)
        lequal(putit.hp, putit.max_hp - 1)
end)

lrun("test LuaCharacter:recruit_as_ally", function()
        Testing.start_in_debug_map()

        local player = Elona.Chara.player()
        lequal(player:recruit_as_ally(), false)

        local putit = Elona.Chara.create(0, 0, 3)
        putit:damage_hp(putit.max_hp + 1)
        lequal(putit:recruit_as_ally(), false)

        local putit = Elona.Chara.create(0, 0, 3)
        lequal(putit:recruit_as_ally(), true)
        lequal(putit:recruit_as_ally(), false)
end)

assert(lresults())
