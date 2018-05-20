#include "testing.hpp"
#include "config.hpp"
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

    foobar_save.initialize();

    title(u8"Elona Foobar version "s + latest_version.short_string());

    initialize_config(fs::path("tests/data/config.json"));
    config::instance().is_test = true;
}

void post_run()
{
    finish_elona();
}

void reset_state()
{
    lua::init();
    lua::load_mod("core");
    initialize_elona();
}

void start_game()
{
}

}
}
