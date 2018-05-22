#include "../thirdparty/catch/catch.hpp"

#include "../filesystem.hpp"
#include "../lua.hpp"
#include "../testing.hpp"

using namespace std::literals::string_literals;

void lua_testcase(const std::string& filename)
{
    elona::testing::reset_state();
    elona::lua::lua.get_state()->open_libraries(sol::lib::io, sol::lib::os);
    REQUIRE_NOTHROW(elona::lua::lua.get_state()->safe_script_file("tests/lua/"s + filename));
}

TEST_CASE("Test that all core API tables are read-only", "[Lua: API]")
{
    lua_testcase("api_readonly.lua");
}

TEST_CASE("Core API: chara", "[Lua: API]")
{
    lua_testcase("chara.lua");
}

TEST_CASE("Core API: FOV", "[Lua: API]")
{
    lua_testcase("fov.lua");
}

TEST_CASE("Core API: Item", "[Lua: API]")
{
    lua_testcase("item.lua");
}

TEST_CASE("Core API: Map", "[Lua: API]")
{
    lua_testcase("map.lua");
}
