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

    REQUIRE_NOTHROW(lua.load_mod_from_script("test", "Store.thing = 1"));

    REQUIRE_NOTHROW(lua.run_in_mod("test", "assert(Store.thing == 1)"));
}

TEST_CASE("Test invalid usage of store in main state", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.load_mod_from_script("test", "Store.thing = 1"));

    // Accessed from main state, not the mod's environment
    sol::object obj = (*lua.get_state())["Store"]["thing"];
    REQUIRE(obj == sol::nil);
}

TEST_CASE("Test invalid reassignment of store inside mod", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_THROWS(lua.load_mod_from_script("test", R"(Store = "dood")"));
    REQUIRE_NOTHROW(lua.run_in_mod("test", R"(assert(Store ~= "dood"))"));
}

TEST_CASE("Test invalid reassignment of Elona table inside mod", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_THROWS(lua.load_mod_from_script("test", R"(Elona = "dood")"));
    REQUIRE_NOTHROW(lua.run_in_mod("test", R"(assert(Elona ~= "dood"))"));
}

TEST_CASE("Test invalid nested reassignment of Elona table inside mod", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_THROWS(lua.load_mod_from_script("test", R"(Elona.Rnd = "dood")"));
    REQUIRE_NOTHROW(lua.run_in_mod("test", R"(assert(Elona.Rnd ~= "dood"))"));
}


TEST_CASE("Test modification of store inside callback", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.load_mod_from_script("test", R"(
function my_turn_handler()
  Store.thing = Store.thing + 1
end

Store.thing = 1

Elona.Event.register(Elona.Defines.EventKind.AllTurnsFinished, my_turn_handler)
)"));

    REQUIRE_NOTHROW(lua.run_in_mod("test", "assert(Store.thing == 1)"));

    lua.get_event_manager().run_callbacks<elona::lua::event_kind_t::all_turns_finished>();
    REQUIRE_NOTHROW(lua.run_in_mod("test", "assert(Store.thing == 2)"));

    lua.get_event_manager().run_callbacks<elona::lua::event_kind_t::all_turns_finished>();
    lua.get_event_manager().run_callbacks<elona::lua::event_kind_t::all_turns_finished>();
    REQUIRE_NOTHROW(lua.run_in_mod("test", "assert(Store.thing == 4)"));
}

TEST_CASE("Test isolation of mod environments", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.load_mod_from_script("first", R"(Store.thing = 42)"));
    REQUIRE_NOTHROW(lua.load_mod_from_script("second", R"(Store.thing = "dood")"));

    REQUIRE_NOTHROW(lua.run_in_mod("first", R"(assert(Store.thing == 42))"));
    REQUIRE_NOTHROW(lua.run_in_mod("second", R"(assert(Store.thing == "dood"))"));

    REQUIRE_NOTHROW(lua.run_in_mod("second", R"(Store.thing = false)"));

    REQUIRE_NOTHROW(lua.run_in_mod("first", R"(assert(Store.thing == 42))"));
    REQUIRE_NOTHROW(lua.run_in_mod("second", R"(assert(Store.thing == false))"));
}


TEST_CASE("Test complex nested table assignment", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.load_mod_from_script("test", R"(
function my_turn_handler()
   for x = 1, 20 do
      for y = 1, 20 do
         Store.grid[x][y] = 1
      end
   end
end

local grid = {}
for i = 1, 20 do
   grid[i] = {}

   for j = 1, 20 do
      grid[i][j] = 0
   end
end

Store.grid = grid

Elona.Event.register(Elona.Defines.EventKind.AllTurnsFinished, my_turn_handler)
)"));

    REQUIRE_NOTHROW(lua.run_in_mod("test", "assert(Store.grid[1][1] == 0)"));

    lua.get_event_manager().run_callbacks<elona::lua::event_kind_t::all_turns_finished>();
    REQUIRE_NOTHROW(lua.run_in_mod("test", "assert(Store.grid[1][1] == 1)"));
}
