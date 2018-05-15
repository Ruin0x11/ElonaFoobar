#pragma once

// TODO refactor continuous action handlers to have try_start, do_turn and finish phases
// TODO separate actions/commands that only ever make sense for the player and never for just any character (ally joins, etc.)
// TODO keep action defs in header, move large hunks of logic to single .cpp files
// TODO separate attacking logic
// TODO separate continuous action variables into "activity" struct
namespace elona
{

enum class turn_result_t;

void do_disarm_trap();
void do_perform();
void do_thing_that_feels_good();

turn_result_t do_enter_strange_gate();
turn_result_t do_debug_console();
turn_result_t do_exit_debug_console();
turn_result_t do_bash();
turn_result_t do_use_stairs_command(int);
turn_result_t try_to_open_locked_door();
turn_result_t do_dig_after_sp_check();
void open_box();
void open_new_year_gift();
void unequip_item(int);
void try_to_melee_attack();
void do_physical_attack();
int equip_item(int);
int do_cast_spell();
int do_gather_material_or_dig();
int do_decode_book();
int do_read_normal_book();
void do_load_save_data(); // TODO move
void do_save_game(); // TODO move
void continuous_action_others();
void do_rest();
int pick_up_item();
int drop_item();
int unlock_box(int);
int do_drink_potion();
int drink_well();
int read_scroll();
int label_2172();
void dipcursed(int = 0, int = 0);
int label_2083();
int label_2167();
int label_2217();
void label_2220();
void discover_trap();
void discover_hidden_path();
void do_play_scene(); // TODO move
void prompt_stop_continuous_action(); // TODO move
void label_2081();
int target_position(); // TODO move
turn_result_t do_gatcha();
turn_result_t step_into_gate();
turn_result_t call_npc();
turn_result_t try_interact_with_npc();
void start_stealing();
turn_result_t do_pray();
turn_result_t show_house_board(); // TODO move

} // namespace elona
