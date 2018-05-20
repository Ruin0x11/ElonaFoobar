#include "../thirdparty/catch/catch.hpp"

#include "../thirdparty/sol2/sol.hpp"
#include "../lua_store.hpp"

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
