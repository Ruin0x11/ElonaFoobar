#pragma once

#include <string>
#include "snail/application.hpp"

namespace elona
{

struct position_t;

position_t gmes(const std::string&, int, int, int, const snail::color&, bool);
void initialize_ui_constants();
void display_customkey(const std::string&, int, int);
void display_key(int = 0, int = 0, int = 0);
void display_msg(int = 0, int = 0);
void display_note(const std::string&, int = 0);



void display_topic(const std::string& topic, int x, int y);



void display_window(int = 0, int = 0, int = 0, int = 0, int = 0, int = 0);
void display_window2(int = 0, int = 0, int = 0, int = 0, int = 0, int = 0);



void fillbg(
    int tile_window_id,
    int tile_x,
    int tile_y,
    int tile_width,
    int tile_height);



void drawmenu(int = 0);
void screen_txtadv();

void update_entire_screen();
void update_screen();
void update_minimap();
void label_1420();
void render_hud();
void load_continuous_action_animation();
void render_autoturn_animation();
void draw_caption();
void update_scrolling_info();
void update_slight();
void label_1433();
void label_1438();
void label_1439();
void fade_out();
void label_1442();
void animation_fade_in();
void label_1444();
void label_1445();
void render_fishing_animation();
void load_background_variants(int buffer);
void clear_background_in_character_making();
void clear_background_in_continue();

void draw_scroll(int x, int y, int width, int height);
void cs_listbk();
void cs_list(
    bool is_selected,
    const std::string& text,
    int x,
    int y,
    int x_offset = 0,
    int color_mode = 0,
    int ci = 0);
void showscroll(const std::string& title, int x, int y, int width, int height);



void window(int x, int y, int width, int height, bool shadow = false);



void window2(
    int x,
    int y,
    int width,
    int height,
    int frame_style,
    int fill_style);



void window_animation(
    int x,
    int y,
    int width,
    int height,
    int duration,
    int temporary_window_id);



void window_animation_corner(
    int x,
    int y,
    int width,
    int height,
    int duration,
    int temporary_window_id);



void show_title(const std::string& title);



void handle_window_resize();



} // namespace elona
