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

    store.set("my_string"s, sol::make_object(sol, "dood"));

    std::string my_string = sol["Store"]["get"]("my_string");
    REQUIRE(my_string == std::string("dood"));
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store.get("my_string") == "dood"))"));

    REQUIRE_NOTHROW(sol.safe_script(R"(Store.set("my_string", "putit"))"));
    my_string = sol["Store"]["get"]("my_string");
    REQUIRE(my_string == std::string("putit"));
}

TEST_CASE("Test that booleans can be set/retrieved", "[Lua: Store]")
{
    sol::state sol;
    sol.open_libraries(sol::lib::base);
    elona::lua::store store;
    store.init(sol);

    store.set("my_bool"s, sol::make_object(sol, false));

    bool my_bool = sol["Store"]["get"]("my_bool");
    REQUIRE(my_bool == false);
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store.get("my_bool") == false))"));

    REQUIRE_NOTHROW(sol.safe_script(R"(Store.set("my_bool", true))"));
    my_bool = sol["Store"]["get"]("my_bool");
    REQUIRE(my_bool == true);
}


TEST_CASE("Test that integers can be set/retrieved", "[Lua: Store]")
{
    sol::state sol;
    sol.open_libraries(sol::lib::base);
    elona::lua::store store;
    store.init(sol);

    store.set("my_int"s, sol::make_object(sol, 42));

    int my_int = sol["Store"]["get"]("my_int");
    REQUIRE(my_int == 42);
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store.get("my_int") == 42))"));

    REQUIRE_NOTHROW(sol.safe_script(R"(Store.set("my_int", 84))"));
    my_int = sol["Store"]["get"]("my_int");
    REQUIRE(my_int == 84);
}

TEST_CASE("Test that tables can be set", "[Lua: Store]")
{
    sol::state sol;
    sol.open_libraries(sol::lib::base);
    elona::lua::store store;
    store.init(sol);

    REQUIRE_NOTHROW(sol.safe_script(R"(Store.set("my_table", { [0]=42, [1]="Scut!", [2]=false, [3]=nil, bell=" *リン* " }))"));

    sol::table my_table = sol["Store"]["get"]("my_table");

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
    store.set("my_table"s, sol::object(my_table));

    my_table = sol["Store"]["get"]("my_table");

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
    elona::character& my_chara = elona::cdata(elona::rc);

    store.set("my_chara"s, sol::make_object(sol, my_chara));

    my_chara = sol["Store"]["get"]("my_chara");
    REQUIRE(my_chara.idx == 57);
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store.get("my_chara").idx == 57))"));
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

    store.set("my_item"s, sol::make_object(sol, my_item));

    my_item = sol["Store"]["get"]("my_item");
    REQUIRE(my_item.idx == idx);
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store.get("my_item").idx == idx))"));
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

    store.set("my_position"s, sol::make_object(sol, my_position));

    my_position = sol["Store"]["get"]("my_position");
    REQUIRE(my_position.x == 24);
    REQUIRE(my_position.y == 47);
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store.get("my_position").x == 24))"));
    REQUIRE_NOTHROW(sol.safe_script(R"(assert(Store.get("my_position").y == 47))"));
}
