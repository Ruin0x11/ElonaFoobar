#include "../thirdparty/catch/catch.hpp"
#include "../thirdparty/sol2/sol.hpp"

#include "tests.hpp"
#include "../character.hpp"
#include "../item.hpp"
#include "../itemgen.hpp"
#include "../lua.hpp"
#include "../testing.hpp"

TEST_CASE("Test usage of store in mod", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    lua.run_mod_from_script("Store.thing = 1");

    int thing = (*lua.get_state())["thing"];
    REQUIRE(thing == 1);
}
