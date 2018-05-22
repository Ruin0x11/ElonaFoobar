#include "../thirdparty/catch/catch.hpp"
#include "../thirdparty/sol2/sol.hpp"

#include "tests.hpp"
#include "../character.hpp"
#include "../item.hpp"
#include "../itemgen.hpp"
#include "../lua.hpp"
#include "../lua_store.hpp"
#include "../testing.hpp"
#include "../variables.hpp"

using namespace elona::testing;
using namespace std::literals::string_literals;

TEST_CASE("Test that handle properties can be read", "[Lua: Handles]")
{
    start_in_debug_map();

    SECTION("Characters")
    {
        REQUIRE(chara_create(-1, PUTIT_PROTO_ID, 4, 8));
        int idx = elona::rc;
        character& chara = elona::cdata[idx];
        auto handle = elona::lua::lua.get_handle_manager().get_chara_handle(chara);
        elona::lua::lua.get_state()->set("chara", handle);
        elona::lua::lua.get_state()->set("idx", idx);
        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(assert(chara.idx == idx))"));
        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(assert(chara.position.x == 4))"));
        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(assert(chara.position.y == 8))"));
    }
    SECTION("Items")
    {
        REQUIRE(itemcreate(-1, PUTITORO_PROTO_ID, 4, 8, 3));
        int idx = elona::ci;
        item& item = elona::inv[idx];
        auto handle = elona::lua::lua.get_handle_manager().get_item_handle(item);
        elona::lua::lua.get_state()->set("item", handle);
        elona::lua::lua.get_state()->set("idx", idx);
        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(assert(item.idx == idx))"));
        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(assert(item.position.x == 4))"));
        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(assert(item.position.y == 8))"));
        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(assert(item.number == 3))"));
    }
}

TEST_CASE("Test that handle properties can be written", "[Lua: Handles]")
{
    start_in_debug_map();

    SECTION("Characters")
    {
        character& chara = elona::cdata[0];
        auto handle = elona::lua::lua.get_handle_manager().get_chara_handle(chara);
        elona::lua::lua.get_state()->set("chara", handle);
        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(chara.shop_rank = 50)"));
        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(chara.position.x = 4)"));
        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(chara.position.y = 8)"));

        REQUIRE(chara.shop_rank == 50);
        REQUIRE(chara.position.x == 4);
        REQUIRE(chara.position.y == 8);
    }
    SECTION("Items")
    {
        REQUIRE(itemcreate(-1, PUTITORO_PROTO_ID, 4, 8, 3));
        item& item = elona::inv[elona::ci];
        auto handle = elona::lua::lua.get_handle_manager().get_item_handle(item);
        elona::lua::lua.get_state()->set("item", handle);

        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(item.number = 3)"));
        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(item.position.x = 4)"));
        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(item.position.y = 8)"));

        REQUIRE(item.number == 3);
        REQUIRE(item.position.x == 4);
        REQUIRE(item.position.y == 8);
    }
}

TEST_CASE("Test that handle methods can be called", "[Lua: Handles]")
{
    start_in_debug_map();

    {
        REQUIRE(chara_create(-1, PUTIT_PROTO_ID, 4, 8));
        character& chara = elona::cdata[elona::rc];
        auto handle = elona::lua::lua.get_handle_manager().get_chara_handle(chara);
        elona::lua::lua.get_state()->set("chara", handle);

        int hp_before = chara.hp;

        REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script(R"(chara:damage_hp(7))"));

        REQUIRE(chara.hp == (hp_before - 7));
    }
}

TEST_CASE("Test that handles go invalid", "[Lua: Handles]")
{
    SECTION("Characters")
    {

    }
    SECTION("Items")
    {

    }
}

TEST_CASE("Test invalid references to handles in store table", "[Lua: Handles]")
{
    REQUIRE(0);
    SECTION("Characters")
    {

    }
    SECTION("Items")
    {

    }
}
