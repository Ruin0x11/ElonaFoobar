#include "../thirdparty/catch/catch.hpp"

#include "../filesystem.hpp"
#include "../lua.hpp"
#include "../testing.hpp"

using namespace std::literals::string_literals;

void lua_testcase(const std::string& filename)
{
    elona::testing::reset_state();
    REQUIRE_NOTHROW(elona::lua::run_file(fs::path("tests/lua/"s + filename)));
}


TEST_CASE("Test that all core API tables are read-only", "[Lua: Misc.]")
{
    lua_testcase("api_readonly.lua");
}

//  TEST_CASE("Test calling certain API methods before it is safe to do so", "[Lua: Misc.]")
//  {
//      // Prevent access to most of the API when loading new mods.
//      // Only allow access to things besides Defines, Event and Registry after initialization.
//      // TODO: Define a clear data lifecycle.
//      REQUIRE(0);
//  }
//
//  TEST_CASE("Test improper modifications to registry", "[Lua: Misc.]")
//  {
//      REQUIRE(0);
//  }
//
//  TEST_CASE("Test improper map initialization routine", "[Lua: Misc.]")
//  {
//      REQUIRE(0);
//  }
//
//  TEST_CASE("Test improper character initialization routine", "[Lua: Misc.]")
//  {
//      REQUIRE(0);
//  }
//
//  TEST_CASE("Test invalid character arguments", "[Lua: Misc.]")
//  {
//      REQUIRE(0);
//  }
//
//  TEST_CASE("Test invalid item arguments", "[Lua: Misc.]")
//  {
//      REQUIRE(0);
//  }
//
//  TEST_CASE("Test invalid enum value arguments", "[Lua: Misc.]")
//  {
//      REQUIRE(0);
//  }
//
//
//  TEST_CASE("Test event callback triggering", "[Lua: Events]")
//  {
//      REQUIRE(0);
//  }
//
//  TEST_CASE("Test ordering of multiple event callbacks", "[Lua: Events]")
//  {
//      REQUIRE(0);
//  }
//
//  TEST_CASE("Test event hook triggering", "[Lua: Events]")
//  {
//      REQUIRE(0);
//  }
//
//  TEST_CASE("Test ordering of multiple event hooks", "[Lua: Events]")
//  {
//      REQUIRE(0);
//  }



TEST_CASE("Core API: Map", "[Lua: Elona.Map]")
{
    lua_testcase("map.lua");
}



TEST_CASE("Test unsupported registry data type assignment", "[Lua: Serialization]")
{
    REQUIRE(0);
}

TEST_CASE("Test serialization/deserialization of basic data", "[Lua: Serialization]")
{
    REQUIRE(0);
}

TEST_CASE("Test serialization/deserialization of tables", "[Lua: Serialization]")
{
    REQUIRE(0);
}

TEST_CASE("Test no serialization/deserialization of metatables", "[Lua: Serialization]")
{
    REQUIRE(0);
}

TEST_CASE("Test serialization/deserialization of LuaCharacter", "[Lua: Serialization]")
{
    REQUIRE(0);
}

TEST_CASE("Test serialization/deserialization of LuaItem", "[Lua: Serialization]")
{
    REQUIRE(0);
}

TEST_CASE("Test serialization/deserialization of maps", "[Lua: Serialization]")
{
    REQUIRE(0);
}

TEST_CASE("Test serialization/deserialization of globals", "[Lua: Serialization]")
{
    REQUIRE(0);
}

TEST_CASE("Test validation of dead/removed LuaCharacter", "[Lua: Serialization]")
{
    REQUIRE(0);
}

TEST_CASE("Test validation of removed LuaItem", "[Lua: Serialization]")
{
    REQUIRE(0);
}
