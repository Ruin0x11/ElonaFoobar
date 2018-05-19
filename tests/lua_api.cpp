#include "../thirdparty/catch/catch.hpp"

#include "../lua.hpp"
#include "../filesystem.hpp"
#include "../testing.hpp"

using namespace std::literals::string_literals;

void lua_testcase(const std::string& filename)
{
    elona::testing::reset_state();
    elona::lua::run_file(fs::path("tests/lua/"s + filename));
}

TEST_CASE("Core API: Map", "[Elona.Map]")
{
    //lua_testcase("map.lua");
}
