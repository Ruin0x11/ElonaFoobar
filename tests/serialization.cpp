#include "../thirdparty/catch/catch.hpp"

#include "../testing.hpp"

#include "../ability.hpp"
#include "../character.hpp"
#include "../filesystem.hpp"
#include "../foobar_save.hpp"
#include "../init.hpp"
#include "../item.hpp"
#include "../item.hpp"
#include "../itemgen.hpp"
#include "../lua.hpp"
#include "../variables.hpp"

using namespace Catch;

const fs::path save_dir("tests/data/save");
int putit_proto_id = 3;
int putitoro_proto_id = 792;

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

void start_in_debug_map()
{
    elona::testing::reset_state();
    initialize_debug_globals();
    elona::playerid = "sav_ruin";

    elona_delete(save_dir / elona::playerid);

    gdata_current_map = 9999; // Debug map
    gdata_current_dungeon_level = 2;
    initialize_map();
}

void save_and_reload()
{
    save_game(save_dir);
    elona::firstturn = 1;
    load_save_data(save_dir);
}

TEST_CASE("Test character saving and reloading", "[C++: Serialization]")
{
    start_in_debug_map();
    int x = 4;
    int y = 8;
    REQUIRE(chara_create(-1, putit_proto_id, x, y));
    int index = elona::rc;

    save_and_reload();

    REQUIRE(elona::cdata(index).state != 0);
    REQUIRE(elona::cdata(index).position.x == x);
    REQUIRE(elona::cdata(index).position.y == y);
    REQUIRE_THAT(elona::cdatan(0, index), Contains("プチ"));
    REQUIRE(elona::cdatan(2, index) == "slime");
}

TEST_CASE("Test party character index preservation", "[C++: Serialization]")
{
    start_in_debug_map();
    REQUIRE(chara_create(-1, putit_proto_id, 0, 0));
    REQUIRE(new_ally_joins());
    int index = elona::rc;

    save_and_reload();

    REQUIRE(elona::cdata(index).idx == index);
}

TEST_CASE("Test other character index preservation", "[C++: Serialization]")
{
    start_in_debug_map();
    REQUIRE(chara_create(-1, putit_proto_id, 0, 0));
    int index = elona::rc;

    save_and_reload();

    REQUIRE(elona::cdata(index).idx == index);
}

TEST_CASE("Test item index preservation", "[C++: Serialization]")
{
    start_in_debug_map();
    REQUIRE(itemcreate(-1, putitoro_proto_id, 0, 0, 0));
    int index = elona::ci;

    save_and_reload();

    REQUIRE(elona::inv(index).idx == index);
}

TEST_CASE("Test character data compatibility", "[C++: Serialization]")
{
    int player_idx = 0;
    load_previous_savefile();
    REQUIRE(elona::cdata(player_idx).idx == player_idx);
    REQUIRE(elona::cdatan(0, player_idx) == "foobar_test");
}

TEST_CASE("Test other character data compatibility", "[C++: Serialization]")
{
    int chara_idx = 57;
    load_previous_savefile();
    REQUIRE(elona::cdata(chara_idx).idx == chara_idx);
    REQUIRE(elona::cdatan(0, chara_idx) == "風を聴く者『ラーネイレ』");
}

TEST_CASE("Test item data compatibility (in inventory)", "[C++: Serialization]")
{
    int item_idx = 0;
    load_previous_savefile();
    REQUIRE(elona::inv(item_idx).idx == item_idx);
    REQUIRE(elona::itemname(item_idx) == "ブロンズの兜 [0,1]");
}

TEST_CASE("Test item data compatibility (on ground)", "[C++: Serialization]")
{
    int item_idx = 5080;
    load_previous_savefile();
    REQUIRE(elona::inv(item_idx).idx == item_idx);
    REQUIRE(elona::itemname(item_idx) == "割れたつぼ");
}

TEST_CASE("Test ability data compatibility", "[C++: Serialization]")
{
    int ability_idx = 170; // Medium Armor
    int chara_idx = 57;
    load_previous_savefile();
    REQUIRE(elona::sdata.get(ability_idx, chara_idx).current_level == 28);
    REQUIRE(elona::sdata.get(ability_idx, chara_idx).original_level == 28);
    REQUIRE(elona::sdata.get(ability_idx, chara_idx).experience == 0);
    REQUIRE(elona::sdata.get(ability_idx, chara_idx).potential == 22);
}

TEST_CASE("Test foobar save data compatibility", "[C++: Serialization]")
{
    load_previous_savefile();
    REQUIRE(elona::foobar_save.is_autodig_enabled == 0);
}

TEST_CASE("Ensure struct sizes are unchanged", "[C++: Serialization]")
{
    // Just to be sure nothing was accidentally changed.
    // This test can be changed so long as we are aware of the implications.
    REQUIRE(sizeof(character) == 872);
    REQUIRE(sizeof(item) == 160);
    REQUIRE(sizeof(ability) == 16);
    REQUIRE(sizeof(foobar_save) == 120);
}
