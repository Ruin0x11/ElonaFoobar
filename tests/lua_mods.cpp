#include "../thirdparty/catch/catch.hpp"
#include "../thirdparty/sol2/sol.hpp"

#include "tests.hpp"
#include "../character.hpp"
#include "../filesystem.hpp"
#include "../item.hpp"
#include "../itemgen.hpp"
#include "../lua.hpp"
#include "../testing.hpp"

TEST_CASE("Test usage of store in mod", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.run_mod_from_script("Store.thing = 1"));
    REQUIRE_NOTHROW(lua.run_in_mod("testing_mod", "assert(Store.thing == 1)"));
}

TEST_CASE("Test invalid usage of store in main state", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.run_mod_from_script("Store.thing = 1"));

    sol::object obj = (*lua.get_state())["Store"]["thing"];
    REQUIRE(obj == sol::nil);
}

TEST_CASE("Test modification of store inside callback", "[Lua: Mods]")
{
    elona::lua::lua_env lua;
    lua.load_all_mods(elona::filesystem::dir::mods()); // TODO load core only

    REQUIRE_NOTHROW(lua.run_mod_from_script(R"(
function my_movement_handler()
  Store.thing = Store.thing + 1
end

Store.thing = 1

Elona.Event.register(Elona.Defines.EventKind.CharaMoved, my_movement_handler)
)"));

    REQUIRE_NOTHROW(lua.run_in_mod("testing_mod", "assert(Store.thing == 1)"));
}
