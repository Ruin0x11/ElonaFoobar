#include "../thirdparty/catch/catch.hpp"
#include "../thirdparty/sol2/sol.hpp"

#include "tests.hpp"
#include "../character.hpp"
#include "../filesystem.hpp"
#include "../item.hpp"
#include "../itemgen.hpp"
#include "../lua.hpp"
#include "../testing.hpp"
#include "../variables.hpp"

using namespace elona::testing;

TEST_CASE("Test MOD_NAME is defined", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.load_mod_from_script("my_mod", ""));

    REQUIRE_NOTHROW(lua.run_in_mod("my_mod", R"(assert(_MOD_NAME == "my_mod"))"));
}

TEST_CASE("Test sandboxing removes unsafe functions", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.load_mod_from_script("my_mod", ""));

    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(rawset(_G, "assert", nil))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(rawget(_G, "assert"))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(rawequal(1, 1))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(rawlen({}))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(require "mods/core/init")"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(collectgarbage())"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(loadstring("i = 1"))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(dofile("tests/lua/item.lua"))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(loadfile("tests/lua/item.lua"))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(load("return function(a,b) return a+b end"))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(setmetatable(_G, {}))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(getmetatable(_G))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(module(..., package.seeall))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(setfenv(print, {}))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(getfenv(print))"));
    REQUIRE_NOTHROW(lua.run_in_mod("my_mod", R"(assert(package == nil))"));
    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(package.loadlib("LibSDL2.dll", "main")())"));

    REQUIRE_THROWS(lua.run_in_mod("my_mod", R"(rawlen = nil; rawlen({}))"));
}

TEST_CASE("Test no access to os/io", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.load_mod_from_script("my_mod", ""));

    REQUIRE_NOTHROW(lua.run_in_mod("my_mod", R"(assert(os == nil))"));
    REQUIRE_NOTHROW(lua.run_in_mod("my_mod", R"(assert(io == nil))"));
}

TEST_CASE("Test usage of store in mod", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.load_mod_from_script("test", "Store.thing = 1"));

    REQUIRE_NOTHROW(lua.run_in_mod("test", "assert(Store.thing == 1)"));
    sol::state_view view(*lua.get_state());
    int thing = lua.get_mod("test")->store->get("thing", view).as<int>();
    assert(thing == 1);
}

TEST_CASE("Test invalid usage of store in main state", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.load_mod_from_script("test", "Store.thing = 1"));

    // Accessed from main state, not the mod's environment
    sol::object obj = (*lua.get_state())["Store"]["thing"];
    REQUIRE(obj == sol::nil);
}

TEST_CASE("Test modification of store inside callback", "[Lua: Mods]")
{
    elona::lua::lua_env lua;

    REQUIRE_NOTHROW(lua.load_mod_from_script("test", R"(
local Event = Elona.require("Event")

function my_turn_handler()
  Store.thing = Store.thing + 1
end

Store.thing = 1

Event.register(Event.EventKind.AllTurnsFinished, my_turn_handler)
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
local Event = Elona.require("Event")

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

Event.register(Event.EventKind.AllTurnsFinished, my_turn_handler)
)"));

    REQUIRE_NOTHROW(lua.run_in_mod("test", "assert(Store.grid[1][1] == 0)"));

    lua.get_event_manager().run_callbacks<elona::lua::event_kind_t::all_turns_finished>();
    REQUIRE_NOTHROW(lua.run_in_mod("test", "assert(Store.grid[1][1] == 1)"));
}

TEST_CASE("Test character created callback", "[Lua: Mods]")
{
    start_in_debug_map();

    REQUIRE_NOTHROW(elona::lua::lua.load_mod_from_script("test_chara_created", R"(
local Event = Elona.require("Event")

function my_chara_created_handler(chara)
   Store.charas[chara.idx] = chara
end

Store.charas = {}

Event.register(Event.EventKind.CharaCreated, my_chara_created_handler)
)"));

    REQUIRE(elona::chara_create(-1, PUTIT_PROTO_ID, 4, 8));
    int idx = elona::rc;
    REQUIRE(idx != -1);
    elona::character& chara = elona::cdata[idx];
    elona::lua::lua.get_mod("test_chara_created")->env.set("idx", idx);

    REQUIRE_NOTHROW(elona::lua::lua.run_in_mod("test_chara_created", R"(assert(Store.charas[idx].idx == idx))"));
}

TEST_CASE("Test character removed callback", "[Lua: Mods]")
{
    start_in_debug_map();

    REQUIRE_NOTHROW(elona::lua::lua.load_mod_from_script("test_chara_removed", R"(
local Event = Elona.require("Event")

function my_chara_removed_handler(chara)
   Store.removed_idx = chara.idx
end

Store.removed_idx = -1

Event.register(Event.EventKind.CharaRemoved, my_chara_removed_handler)
)"));

    REQUIRE(elona::chara_create(-1, PUTIT_PROTO_ID, 4, 8));
    int idx = elona::rc;
    elona::character& chara = elona::cdata[idx];
    elona::lua::lua.get_mod("test_chara_removed")->env.set("idx", idx);

    elona::chara_delete(idx);

    REQUIRE_NOTHROW(elona::lua::lua.run_in_mod("test_chara_removed", R"(assert(Store.removed_idx == idx))"));
}


TEST_CASE("Test item created callback", "[Lua: Mods]")
{
    start_in_debug_map();

    REQUIRE_NOTHROW(elona::lua::lua.load_mod_from_script("test_item_created", R"(
local Event = Elona.require("Event")

function my_item_created_handler(item)
   Store.items[item.idx] = item
end

Store.items = {}

Event.register(Event.EventKind.ItemCreated, my_item_created_handler)
)"));

    REQUIRE(elona::itemcreate(-1, PUTITORO_PROTO_ID, 4, 8, 3));
    int idx = elona::ci;
    REQUIRE(idx != -1);
    elona::item& item = elona::inv[idx];
    elona::lua::lua.get_mod("test_item_created")->env.set("idx", idx);

    REQUIRE_NOTHROW(elona::lua::lua.run_in_mod("test_item_created", R"(assert(Store.items[idx].idx == idx))"));
}

TEST_CASE("Test item removed callback", "[Lua: Mods]")
{
    start_in_debug_map();

    REQUIRE_NOTHROW(elona::lua::lua.load_mod_from_script("test_item_removed", R"(
local Event = Elona.require("Event")

function my_item_removed_handler(item)
   Store.removed_idx = item.idx
end

Store.removed_idx = -1

Event.register(Event.EventKind.ItemRemoved, my_item_removed_handler)
)"));

    REQUIRE(elona::itemcreate(-1, PUTITORO_PROTO_ID, 4, 8, 3));
    int idx = elona::ci;
    elona::item& item = elona::inv[idx];
    REQUIRE(elona::inv[idx].idx != -1);
    elona::lua::lua.get_mod("test_item_removed")->env.set("idx", idx);

    elona::item_delete(idx);

    REQUIRE_NOTHROW(elona::lua::lua.run_in_mod("test_item_removed", R"(assert(Store.removed_idx == idx))"));
}
