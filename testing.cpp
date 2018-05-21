#include "testing.hpp"
#include "config.hpp"
#include "foobar_save.hpp"
#include "init.hpp"
#include "variables.hpp"
#include "version.hpp"

namespace elona
{
namespace testing
{

const fs::path save_dir("tests/data/save");

void load_previous_savefile()
{
    elona::testing::reset_state();
    // This file was saved directly after the dialog at the start of the game.
    elona::playerid = "sav_foobar_test";
    load_save_data(save_dir);
    elona::firstturn = 1;
    elona::mode = 3; // begin the game as if it were loaded from a save; load inv_xxx.s2
    initialize_map();
}

void save_and_reload()
{
    save_game(save_dir);
    elona::testing::reset_state();
    elona::firstturn = 1;
    load_save_data(save_dir);
}

void start_in_debug_map()
{
    reset_state();
    initialize_debug_globals();
    elona::playerid = "sav_ruin";

    elona_delete(save_dir / elona::playerid);

    gdata_current_map = 9999; // Debug map
    gdata_current_dungeon_level = 2;
    initialize_map();
}

void configure_lua()
{
    // lua::sol.get()->open_libraries(sol::lib::os,
    //                                sol::lib::io);
    // sol::table Elona = (*lua::sol.get())["Elona"];
    // sol::table Testing = Elona.create_named("Testing");
    // Testing.set_function("initialize_map", initialize_map);
    // Testing.set_function("start_in_debug_map", start_in_debug_map);
    // Testing.set_function("reset_state", reset_state);
}

void pre_init()
{
    initialize_cat_db();
    configure_lua();

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
    //lua::clear();
    //lua::load_mod("core");
    initialize_elona();
}

}
}
