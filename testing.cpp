#include "testing.hpp"
#include "foobar_save.hpp"
#include "init.hpp"
#include "lua.hpp"
#include "variables.hpp"
#include "version.hpp"

namespace elona
{
namespace testing
{

void pre_init()
{
    initialize_cat_db();

    title(u8"Elona Foobar version "s + latest_version.short_string());
}

void reset_state()
{
    foobar_save.initialize();
    lua::init();
    lua::load_mod("core");
    initialize_elona();
    mode = 5;
    initialize_game();
}

void start_game()
{
}

}
}
