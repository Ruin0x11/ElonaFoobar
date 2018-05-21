#include "../thirdparty/catch/catch.hpp"
#include "../thirdparty/sol2/sol.hpp"

#include "tests.hpp"
#include "../character.hpp"
#include "../item.hpp"
#include "../itemgen.hpp"
#include "../lua_store.hpp"
#include "../testing.hpp"
#include "../variables.hpp"

using namespace std::literals::string_literals;

TEST_CASE("Test that strings can be set/retrieved", "[Lua: Store]")
{
    sol::state sol;
    sol.open_libraries(sol::lib::base);
    elona::lua::store store;
    store.init(sol);

    auto view = sol::state_view(sol);
    store.set("my_string"s, sol::make_object(sol, "dood"), view);

    std::string my_string = sol["Store"]["my_string"];
    REQUIRE(my_string == std::string("dood"));
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store["my_string"] == "dood"))"));

    REQUIRE_NOTHROW(sol.safe_script(R"(Store["my_string"] = "putit")"));
    my_string = sol["Store"]["my_string"];
    REQUIRE(my_string == std::string("putit"));
}

TEST_CASE("Test that booleans can be set/retrieved", "[Lua: Store]")
{
    sol::state sol;
    sol.open_libraries(sol::lib::base);
    elona::lua::store store;
    store.init(sol);

    auto view = sol::state_view(sol);
    store.set("my_bool"s, sol::make_object(sol, false), view);

    bool my_bool = sol["Store"]["my_bool"];
    REQUIRE(my_bool == false);
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store["my_bool"] == false))"));

    REQUIRE_NOTHROW(sol.safe_script(R"(Store["my_bool"] = true)"));
    my_bool = sol["Store"]["my_bool"];
    REQUIRE(my_bool == true);
}


TEST_CASE("Test that integers can be set/retrieved", "[Lua: Store]")
{
    sol::state sol;
    sol.open_libraries(sol::lib::base);
    elona::lua::store store;
    store.init(sol);

    auto view = sol::state_view(sol);
    store.set("my_int"s, sol::make_object(sol, 42), view);

    int my_int = sol["Store"]["my_int"];
    REQUIRE(my_int == 42);
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store["my_int"] == 42))"));

    REQUIRE_NOTHROW(sol.safe_script(R"(Store["my_int"] = 84)"));
    my_int = sol["Store"]["my_int"];
    REQUIRE(my_int == 84);
}

TEST_CASE("Test that tables can be set", "[Lua: Store]")
{
    sol::state sol;
    sol.open_libraries(sol::lib::base);
    elona::lua::store store;
    store.init(sol);

    REQUIRE_NOTHROW(sol.safe_script(R"(Store["my_table"] = { [0]=42, [1]="Scut!", [2]=false, [3]=nil, bell=" *リン* " })"));

    sol::table my_table = sol["Store"]["my_table"];

    std::size_t tablesize = 4;
    std::size_t iterations = 0;
    auto fx = [&iterations](sol::object key, sol::object value) {
        ++iterations;
        sol::type keytype = key.get_type();
        switch (keytype) {
        case sol::type::number:
            switch (key.as<int>()) {
            case 0:
                REQUIRE((value.as<int>() == 42));
                break;
            case 1:
                REQUIRE((value.as<std::string>() == "Scut!"));
                break;
            case 2:
                REQUIRE((value.as<bool>() == false));
                break;
            case 3:
                REQUIRE((value.is<sol::lua_nil_t>()));
                break;
            }
            break;
        case sol::type::string:
            if (key.as<std::string>() == "bell") {
                REQUIRE((value.as<std::string>() == " *リン* "));
            }
            break;
        default:
            break;
        }
    };
    my_table.for_each(fx);
    REQUIRE(iterations == tablesize);

    auto fxpair = [&fx](std::pair<sol::object, sol::object> kvp) { fx(kvp.first, kvp.second); };
    iterations = 0;
    my_table.for_each(fxpair);
    REQUIRE(iterations == tablesize);
}

TEST_CASE("Test that tables can be retrieved", "[Lua: Store]")
{
    sol::state sol;
    sol.open_libraries(sol::lib::base);
    elona::lua::store store;
    store.init(sol);

    sol::table my_table = sol.create_table_with(0, 42,
                                                1, "Scut!",
                                                2, false,
                                                3, sol::nil, // does nothing
                                                "bell"," *リン* ");
    auto view = sol::state_view(sol);
    store.set("my_table"s, sol::object(my_table), view);

    my_table = sol["Store"]["my_table"];

    std::size_t tablesize = 4;
    std::size_t iterations = 0;
    auto fx = [&iterations](sol::object key, sol::object value) {
        ++iterations;
        sol::type keytype = key.get_type();
        switch (keytype) {
        case sol::type::number:
            switch (key.as<int>()) {
            case 0:
                REQUIRE((value.as<int>() == 42));
                break;
            case 1:
                REQUIRE((value.as<std::string>() == "Scut!"));
                break;
            case 2:
                REQUIRE((value.as<bool>() == false));
                break;
            case 3:
                REQUIRE((value.is<sol::lua_nil_t>()));
                break;
            }
            break;
        case sol::type::string:
            if (key.as<std::string>() == "bell") {
                REQUIRE((value.as<std::string>() == " *リン* "));
            }
            break;
        default:
            break;
        }
    };
    my_table.for_each(fx);
    REQUIRE(iterations == tablesize);

    auto fxpair = [&fx](std::pair<sol::object, sol::object> kvp) { fx(kvp.first, kvp.second); };
    iterations = 0;
    my_table.for_each(fxpair);
    REQUIRE(iterations == tablesize);
}

TEST_CASE("Test that character references can be set", "[Lua: Store]")
{
    sol::state sol;
    sol.open_libraries(sol::lib::base);
    elona::lua::store store;
    store.init(sol);

    sol.new_usertype<character>( "LuaCharacter",
                                 "idx", sol::readonly(&character::idx)
        );

    elona::testing::start_in_debug_map();
    REQUIRE(elona::chara_create(-1, PUTIT_PROTO_ID, 0, 0));
    int idx = elona::rc;
    elona::character& my_chara = elona::cdata(idx);
    sol.set("idx", idx);

    auto view = sol::state_view(sol);
    store.set("my_chara"s, sol::make_object(sol, my_chara), view);

    SECTION("valid reference")
    {
        my_chara = sol["Store"]["my_chara"];
        REQUIRE(my_chara.idx == idx);
        REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store["my_chara"].idx == idx))"));
    }
    SECTION("invalid reference")
    {
        elona::chara_delete(idx);
        sol::object thing = sol["Store"]["my_chara"];
        REQUIRE(thing == sol::nil);
        REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store["my_chara"] == nil))"));
    }
}

TEST_CASE("Test that item references can be set", "[Lua: Store]")
{
    sol::state sol;
    sol.open_libraries(sol::lib::base);
    elona::lua::store store;
    store.init(sol);

    sol.new_usertype<item>( "LuaItem",
                            "idx", sol::readonly(&item::idx)
        );

    elona::testing::start_in_debug_map();
    REQUIRE(itemcreate(-1, PUTITORO_PROTO_ID, 0, 0, 3));
    int idx = elona::ci;
    elona::item& my_item = elona::inv(idx);
    sol.set("idx", idx);

    auto view = sol::state_view(sol);
    store.set("my_item"s, sol::make_object(sol, my_item), view);

    SECTION("valid reference")
    {
        my_item = sol["Store"]["my_item"];
        REQUIRE(my_item.idx == idx);
        REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store["my_item"].idx == idx))"));
    }

    SECTION("invalid reference")
    {
        elona::item_delete(idx);
        sol::object thing = sol["Store"]["my_item"];
        REQUIRE(thing == sol::nil);
        REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store["my_item"] == nil))"));
    }
}

TEST_CASE("Test that positions can be set", "[Lua: Store]")
{
    sol::state sol;
    sol.open_libraries(sol::lib::base);
    elona::lua::store store;
    store.init(sol);

    sol.new_usertype<position_t>( "LuaPosition",
                                  "x", &position_t::x,
                                  "y", &position_t::y
        );

    position_t my_position = { 24, 47 };

    auto view = sol::state_view(sol);
    store.set("my_position"s, sol::make_object(sol, my_position), view);

    my_position = sol["Store"]["my_position"];
    REQUIRE(my_position.x == 24);
    REQUIRE(my_position.y == 47);
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store["my_position"].x == 24))"));
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store["my_position"].y == 47))"));
}

TEST_CASE("Test isolation between environments")
{
    sol::state sol;
    sol.open_libraries(sol::lib::base);

    sol::environment first_env(sol, sol::create, sol.globals());
    sol::environment second_env(sol, sol::create, sol.globals());
    elona::lua::store first_store;
    elona::lua::store second_store;
    first_store.init(sol, first_env);
    second_store.init(sol, second_env);

    REQUIRE_NOTHROW(sol.safe_script(R"(Store["message"] = "dood")", first_env));
    REQUIRE_NOTHROW(sol.safe_script(R"(Store["message"] = "putit")", second_env));

    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store["message"] == "dood"))", first_env));
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store["message"] == "putit"))", second_env));
}
