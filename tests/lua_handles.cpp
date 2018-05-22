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
    SECTION("Characters")
    {

    }
    SECTION("Items")
    {

    }
}
