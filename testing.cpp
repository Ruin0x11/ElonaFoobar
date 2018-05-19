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
    load_config(fs::path("tests/data/config.json"));
    config::instance().is_test = true;

    initialize_cat_db();

    title(u8"Elona Foobar version "s + latest_version.short_string());
}

void post_run()
{
    finish_elona();
}

void reset_state()
{
    foobar_save.initialize();
    lua::init();
    lua::load_mod("core");
    initialize_elona();
}

void start_game()
{
}

}
}
