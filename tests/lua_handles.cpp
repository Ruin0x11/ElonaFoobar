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
    start_in_debug_map();

    SECTION("Characters")
    {
        REQUIRE(chara_create(-1, PUTIT_PROTO_ID, 4, 8));
        character& chara = elona::cdata[elona::rc];
        auto handle = elona::lua::lua.get_handle_manager().get_chara_handle(chara);
        elona::lua::lua.get_state()->set("chara", handle);

        chara_delete(chara.idx);

        {
            auto result = elona::lua::lua.get_state()->safe_script(R"(print(chara.idx))", &sol::script_pass_on_error);
            REQUIRE(!result.valid());
        }
        {
            auto result = elona::lua::lua.get_state()->safe_script(R"(chara.position.x = 2)", &sol::script_pass_on_error);
            REQUIRE(!result.valid());
        }
        {
            auto result = elona::lua::lua.get_state()->safe_script(R"(chara:damage_hp(1))", &sol::script_pass_on_error);
            REQUIRE(!result.valid());
        }
    }
    SECTION("Items")
    {
        REQUIRE(itemcreate(-1, PUTITORO_PROTO_ID, 4, 8, 3));
        item& item = elona::inv[elona::ci];
        auto handle = elona::lua::lua.get_handle_manager().get_item_handle(item);
        elona::lua::lua.get_state()->set("item", handle);

        item_delete(item.idx);

        {
            auto result = elona::lua::lua.get_state()->safe_script(R"(print(item.number))", &sol::script_pass_on_error);
            REQUIRE(!result.valid());
        }
        {
            auto result = elona::lua::lua.get_state()->safe_script(R"(item.position.x = 2)", &sol::script_pass_on_error);
            REQUIRE(!result.valid());
        }
    }
}

TEST_CASE("Test invalid references to handles in store table", "[Lua: Handles]")
{
    start_in_debug_map();

    SECTION("Characters")
    {
        REQUIRE(chara_create(-1, PUTIT_PROTO_ID, 4, 8));
        character& chara = elona::cdata[elona::rc];
        auto handle = elona::lua::lua.get_handle_manager().get_chara_handle(chara);
        elona::lua::lua.get_state()->set("chara", handle);

        REQUIRE_NOTHROW(elona::lua::lua.load_mod_from_script("test", "Store.charas = {[0]=chara}"));

        chara_delete(chara.idx);

        REQUIRE_THROWS(elona::lua::lua.run_in_mod("test", "print(Store.charas[0].idx)"));
    }
    SECTION("Items")
    {
        REQUIRE(itemcreate(-1, PUTITORO_PROTO_ID, 4, 8, 3));
        item& item = elona::inv[elona::ci];
        auto handle = elona::lua::lua.get_handle_manager().get_item_handle(item);
        elona::lua::lua.get_state()->set("item", handle);

        REQUIRE_NOTHROW(elona::lua::lua.load_mod_from_script("test2", "Store.items = {[0]=item}"));

        item_delete(item.idx);

        REQUIRE_THROWS(elona::lua::lua.run_in_mod("test2", "print(Store.items[0].idx)"));
    }
}

TEST_CASE("Test invalid references to handles from Lua side", "[Lua: Handles]")
{
    start_in_debug_map();

    SECTION("Characters")
    {
        REQUIRE_NOTHROW(elona::lua::lua.load_mod_from_script("test_invalid_chara", R"(
local Chara = Elona.require("Chara")
local chara = Chara.create(0, 0, 3)
idx = chara.idx
Store.charas = {[0]=chara}
)"));
        int idx = elona::lua::lua.get_mod("test_invalid_chara")->env["idx"];

        chara_delete(idx);

        REQUIRE_THROWS(elona::lua::lua.run_in_mod("test_invalid_chara", "print(Store.charas[0].idx)"));
    }
    SECTION("Items")
    {
        REQUIRE_NOTHROW(elona::lua::lua.load_mod_from_script("test_invalid_item", R"(
local Item = Elona.require("Item")
local item = Item.create(0, 0, 792, 3)
idx = item.idx
Store.items = {[0]=items}
)"));
        int idx = elona::lua::lua.get_mod("test_invalid_item")->env["idx"];

        item_delete(idx);

        REQUIRE_THROWS(elona::lua::lua.run_in_mod("test_invalid_item", "print(Store.items[0].idx)"));
    }
}

TEST_CASE("Test calling C++ functions taking handles as arguments")
{
    start_in_debug_map();

    SECTION("Characters")
    {
        REQUIRE(chara_create(-1, PUTIT_PROTO_ID, 4, 8));
        character& chara = elona::cdata[elona::rc];
        auto handle = elona::lua::lua.get_handle_manager().get_chara_handle(chara);
        elona::lua::lua.get_state()->set("chara", handle);

        REQUIRE_NOTHROW(elona::lua::lua.load_mod_from_script("test_chara_arg", "Store.charas = {[0]=chara}"));
        REQUIRE_NOTHROW(elona::lua::lua.run_in_mod("test_chara_arg", R"(
local Chara = Elona.require("Chara")
print(Chara.is_ally(Store.charas[0]))
)"));

        chara_delete(chara.idx);

        REQUIRE_THROWS(elona::lua::lua.run_in_mod("test_chara_arg", R"(
local Chara = Elona.require("Chara")
print(Chara.is_ally(Store.charas[0]))
)"));
    }
    SECTION("Items")
    {
        REQUIRE(itemcreate(-1, PUTITORO_PROTO_ID, 4, 8, 3));
        item& item = elona::inv[elona::ci];
        auto handle = elona::lua::lua.get_handle_manager().get_item_handle(item);
        elona::lua::lua.get_state()->set("item", handle);

        REQUIRE_NOTHROW(elona::lua::lua.load_mod_from_script("test_item_arg", "Store.items = {[0]=item}"));
        REQUIRE_NOTHROW(elona::lua::lua.run_in_mod("test_item_arg", R"(
local Item = Elona.require("Item")
Item.has_enchantment(Store.items[0], 20)
)"));

        item_delete(item.idx);

        REQUIRE_THROWS(elona::lua::lua.run_in_mod("test_item_arg", R"(
local Item = Elona.require("Item")
Item.has_enchantment(Store.items[0], 20)
)"));
    }
}
