#pragma once

#include <string>

namespace elona
{

enum class turn_result_t;

struct menu_result
{
   bool succeeded; // true if stat was 1, false if it was 0
   bool pressed_f1; // true if menu_feats_internal returned -1
   turn_result_t turn_result;
};

void text_set();
int cnvjkey(const std::string&);
void show_quick_menu();
void show_ex_help();
void label_2702();
void show_game_help();
turn_result_t show_chat_history();
turn_result_t show_message_log();
void initialize_jkey();
void label_2720();
void set_option();
turn_result_t play_scene();

turn_result_t show_spell_list();
void label_2031();
void label_2032();
menu_result menu_character_sheet();
int change_appearance();
menu_result menu_feats();
menu_result menu_materials();

int select_alias(int);
menu_result menu_feats();
menu_result menu_feats_internal();
void deco_traits_menu();
menu_result label_1969();
menu_result menu_journal();
turn_result_t show_quest_board();
void list_adventurers();
turn_result_t blending_menu();
menu_result menu_equipment();
int show_hire_menu();
int show_spell_writer_menu();
void showeconomy(int, int, const std::string&, int = 0, int = 0);
void show_city_chart();
void show_economy_window();
void show_politics_window();
void begin_to_believe_god();
void label_1958();
void label_1964();
int ctrl_ally();
void label_2022();
int change_npc_tone();

menu_result ctrl_inventory();

} // namespace elona
