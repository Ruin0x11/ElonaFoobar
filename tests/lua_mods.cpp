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

    REQUIRE_NOTHROW(lua.run_mod_from_script(R"(
Store.thing = 1
assert(Store.thing == 1)
)"));
}

TEST_CASE("Test invalid usage of store in main state", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.run_mod_from_script("Store.thing = 1"));

    sol::object obj = (*lua.get_state())["Store"]["thing"];
    REQUIRE(obj == sol::nil);
}
