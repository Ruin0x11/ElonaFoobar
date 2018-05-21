#include "turn_sequence.hpp"
#include "buff.hpp"
#include "ability.hpp"
#include "audio.hpp"
#include "ai.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "command.hpp"
#include "config.hpp"
#include "ctrl_file.hpp"
#include "debug.hpp"
#include "dmgheal.hpp"
#include "elona.hpp"
#include "event.hpp"
#include "event_manager.hpp"
#include "foobar_save.hpp"
#include "food.hpp"
#include "fov.hpp"
#include "filesystem.hpp"
#include "i18n.hpp"
#include "input.hpp"
#include "item.hpp"
#include "item_db.hpp"
#include "lua.hpp"
#include "macro.hpp"
#include "map_cell.hpp"
#include "mef.hpp"
#include "menu.hpp"
#include "network.hpp"
#include "quest.hpp"
#include "ui.hpp"
#include "variables.hpp"
#include "wish.hpp"



namespace
{
int ct = 0;
}


namespace elona
{


turn_result_t npc_turn()
{
    int searchfov = 0;
    if (cdata[cc].is_hung_on_sand_bag())
    {
        if (is_in_fov(cc))
        {
            if (rnd(30) == 0)
            {
                tc = cc;
                txt(lang(
                        u8"「もっとぶって"s + _yo(2) + u8"」"s,
                        u8"\"Release me now.\""s),
                    lang(
                        u8"「こんなことして、許さない"s + _yo() + u8"」"s,
                        u8"\"I won't forget this.\""s),
                    lang(
                        u8"「何をする"s + _nda(2) + u8"」"s, u8"\"Hit me!\""s));
            }
        }
        cdata[cc].hate = 0;
        return turn_result_t::turn_end;
    }
    if (is_in_fov(cc) == 0)
    {
        if (cdata[0].blind == 0)
        {
            if (rnd(4) == 0)
            {
                if (mdata(6) != 1)
                {
                    if (cdata[cc].is_leashed())
                    {
                        if (gdata_current_map != 40)
                        {
                            if (cc < 16)
                            {
                                cdata[cc].hate = 0;
                                cdata[cc].enemy_id = 0;
                            }
                            else
                            {
                                if (rnd(2))
                                {
                                    txt(lang(u8"「痛っ！」"s, u8"\"Ouch!\""s),
                                        lang(
                                            u8"「やめて！」"s,
                                            u8"\"Stop it!\""s));
                                    hostileaction(0, cc);
                                }
                                if (rnd(4) == 0)
                                {
                                    cdata[cc].is_leashed() = false;
                                    txtef(9);
                                    txt(lang(
                                        name(cc)
                                            + u8"は巻きついていた紐をほどいた。"s,
                                        name(cc) + u8" untangle"s + _s(cc)
                                            + u8" the leash."s));
                                }
                            }
                            tc = 0;
                            efid = 619;
                            magic();
                            return turn_result_t::turn_end;
                        }
                    }
                }
            }
        }
    }
    if (cdata[cc].will_explode_soon())
    {
        tlocx = cdata[cc].position.x;
        tlocy = cdata[cc].position.y;
        efid = 644;
        magic();
        return turn_result_t::turn_end;
    }
    if (cdata[cc].relationship >= 10)
    {
        --cdata[cc].hate;
        if (cdata[cc].enemy_id == 0 || cdata[cc].hate <= 0
            || (cdata[cdata[cc].enemy_id].relationship >= -2
                && cdata[cdata[cc].enemy_id].enemy_id != cc))
        {
            cdata[cc].enemy_id = 0;
            if (pcattacker != 0)
            {
                if (cdata[pcattacker].relationship <= -3)
                {
                    if (cdata[pcattacker].state == 1)
                    {
                        if (fov_los(
                                cdata[cc].position.x,
                                cdata[cc].position.y,
                                cdata[pcattacker].position.x,
                                cdata[pcattacker].position.y))
                        {
                            cdata[cc].hate = 5;
                            cdata[cc].enemy_id = pcattacker;
                        }
                    }
                }
            }
            if (cdata[cc].enemy_id == 0)
            {
                if (cdata[0].enemy_id != 0
                    && cdata[cdata[0].enemy_id].relationship <= -3)
                {
                    if (cdata[cdata[0].enemy_id].state == 1)
                    {
                        if (fov_los(
                                cdata[cc].position.x,
                                cdata[cc].position.y,
                                cdata[cdata[0].enemy_id].position.x,
                                cdata[cdata[0].enemy_id].position.y))
                        {
                            cdata[cc].hate = 5;
                            cdata[cc].enemy_id = cdata[0].enemy_id;
                        }
                    }
                }
            }
        }
        if (cdata[cdata[cc].enemy_id].is_invisible() == 1)
        {
            if (cdata[cc].can_see_invisible() == 0)
            {
                if (cdata[cdata[cc].enemy_id].wet == 0)
                {
                    if (rnd(5))
                    {
                        cdata[cc].enemy_id = 0;
                    }
                }
            }
        }
    }
    if (cdata[cc].enemy_id != 0)
    {
        if (cdata[cdata[cc].enemy_id].state != 1)
        {
            cdata[cc].enemy_id = 0;
            cdata[cc].hate = 0;
        }
    }
    if (gdata_current_map == 40)
    {
        if (cdata[cc].relationship != -3)
        {
            if (cdata[cc].relationship != 10)
            {
                if (rnd(40) == 0)
                {
                    txtef(4);
                    txt(lang(u8"「いいぞ！」"s, u8"\"Come on!\""s),
                        lang(u8"「もっとやれー」"s, u8"\"More blood!\""s),
                        lang(u8"「血をみせろー」"s, u8"\"Beat'em!\""s),
                        lang(u8"「頑張って！」"s, u8"\"Use your brain!\""s),
                        lang(u8"「うぉぉぉぉ！」"s, u8"\"Wooooo!\""s),
                        lang(u8"「行けぇ！」"s, u8"\"Go go!\""s),
                        lang(u8"「頭を使えよ」"s, u8"\"Good fighting.\""s),
                        lang(u8"「きゃー」"s, u8"\"Yeeee!\""s));
                }
                return ai_proc_misc_map_events();
            }
        }
        cdata[cc].hate = 100;
        if (cdata[cc].relationship == 10)
        {
            p(0) = -3;
            p(1) = enemyteam;
            p(2) = 15;
        }
        else
        {
            p(0) = 10;
            p(1) = 1;
            p(2) = 16;
        }
        i = cdata[cc].enemy_id;
        if (cdata[i].relationship == p && cdata[i].state == 1 && i >= p(1)
            && i < p(1) + p(2))
        {
            if (rnd(10) != 0)
            {
                tc = i;
                goto label_2689_internal;
            }
        }
        cdata[cc].enemy_id = p(1);
        for (int cnt = 0; cnt < 100; ++cnt)
        {
            i = rnd(p(2)) + p(1);
            if (cdata[i].state == 1)
            {
                if (cdata[i].relationship == p)
                {
                    cdata[cc].enemy_id = i;
                    break;
                }
            }
        }
        if (cdata[cdata[cc].enemy_id].relationship != p
            || cdata[cdata[cc].enemy_id].state != 1)
        {
            f = 0;
            for (int cnt = p(1), cnt_end = cnt + (p(2)); cnt < cnt_end; ++cnt)
            {
                if (cdata[cnt].state == 1)
                {
                    if (cdata[cnt].relationship == p)
                    {
                        cdata[cc].enemy_id = cnt;
                        f = 1;
                        break;
                    }
                }
            }
            if (f == 0)
            {
                if (cdata[cc].relationship == 10)
                {
                    petarenawin = 1;
                }
                else
                {
                    petarenawin = 2;
                }
                return turn_result_t::turn_end;
            }
        }
    }
    if (gdata_current_map == 33)
    {
        if (cc != gdata_fire_giant)
        {
            if (cc > 16)
            {
                if (gdata_released_fire_giant != 0)
                {
                    if (cdata[gdata_fire_giant].state == 1)
                    {
                        cdata[cc].enemy_id = gdata_fire_giant;
                        cdata[cc].hate = 500;
                    }
                }
            }
        }
    }
    if (gdata_mount != 0)
    {
        if (cdata[cc].enemy_id == gdata_mount)
        {
            if (rnd(3))
            {
                cdata[cc].enemy_id = 0;
            }
        }
        if (cdata[cc].enemy_id == 0)
        {
            if (cdata[cc].relationship <= -2)
            {
                if (rnd(3) == 0)
                {
                    cdata[cc].enemy_id = gdata_mount;
                }
            }
        }
    }
    tc = cdata[cc].enemy_id;
    if (cdatan(4, cc) != ""s)
    {
        if (cdata[cc].has_custom_talk() == 0)
        {
            if (rnd(30) == 0)
            {
                txtef(9);
                txt(""s + cdatan(4, cc));
            }
        }
    }
    else if (cdata[cc].can_talk != 0 || cdata[cc].has_custom_talk())
    {
        if (cdata[cc].is_silent() == 0)
        {
            if (cdata[cc].turn % 5 == 0)
            {
                if (rnd(4) == 0)
                {
                    if (cdata[0].position.x > cdata[cc].position.x - 10
                        && cdata[0].position.x < cdata[cc].position.x + 10)
                    {
                        if (cdata[0].position.y > cdata[cc].position.y - 10
                            && cdata[0].position.y < cdata[cc].position.y + 10)
                        {
                            if (cdata[0].continuous_action_id != 6)
                            {
                                if (cdata[cc].hate <= 0)
                                {
                                    chara_custom_talk(cc, 100);
                                }
                                if (cdata[cc].hate > 0)
                                {
                                    chara_custom_talk(cc, 101);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (cdata[cc].relationship >= 0)
    {
        if (cdata[0].choked)
        {
            if (dist(
                    cdata[0].position.x,
                    cdata[0].position.y,
                    cdata[cc].position.x,
                    cdata[cc].position.y)
                == 1)
            {
                x = cdata[0].position.x;
                y = cdata[0].position.y;
                return do_bash();
            }
        }
    }
    if (cdata[cc].ai_heal != 0)
    {
        if (cdata[cc].hp < cdata[cc].max_hp / 4)
        {
            if (cdata[cc].mp > 0 || rnd(5) == 0)
            {
                efid = cdata[cc].ai_heal;
                if (efid >= 400 && efid < 467)
                {
                    npccostmp = 1;
                    int stat = label_2167();
                    if (stat == 1)
                    {
                        return turn_result_t::turn_end;
                    }
                }
                else if (efid >= 600)
                {
                    int stat = label_2174();
                    if (stat == 1)
                    {
                        return turn_result_t::turn_end;
                    }
                }
            }
        }
    }
    if (cdata[cc].item_which_will_be_used == 0)
    {
        goto label_2689_internal;
    }
    ci = cdata[cc].item_which_will_be_used;
    if (inv[ci].number == 0)
    {
        cdata[cc].item_which_will_be_used = 0;
        goto label_2689_internal;
    }
    if (cdata[cc].relationship != 0)
    {
        cdata[cc].item_which_will_be_used = 0;
    }
    {
        int category = the_item_db[inv[ci].id]->category;
        if (category == 57000)
        {
            if (cdata[cc].relationship != 10 || cdata[cc].nutrition <= 6000)
            {
                return do_eat_command();
            }
        }
        if (category == 52000)
        {
            return do_drink_command();
        }
        if (category == 53000)
        {
            return do_read_command();
        }
    }
    cdata[cc].item_which_will_be_used = 0;
label_2689_internal:
    if (cdata[cc].hate > 0 || cdata[cc].relationship == 10)
    {
        distance = dist(
            cdata[tc].position.x,
            cdata[tc].position.y,
            cdata[cc].position.x,
            cdata[cc].position.y);
        if (cdata[cc].blind != 0)
        {
            if (rnd(10) > 2)
            {
                return ai_proc_misc_map_events();
            }
        }
        if (cdata[cc].confused != 0)
        {
            if (rnd(10) > 3)
            {
                return ai_proc_misc_map_events();
            }
        }
        if (cdata[cc].relationship == 10)
        {
            if (tc == 0)
            {
                if (map(cdata[cc].position.x, cdata[cc].position.y, 4) != 0)
                {
                    const auto item_info = cell_itemoncell(cdata[cc].position);
                    const auto number = item_info.first;
                    const auto item = item_info.second;
                    if (number == 1)
                    {
                        ci = item;
                        p = the_item_db[inv[ci].id]->category;
                        if (cdata[cc].nutrition <= 6000)
                        {
                            if (p == 57000)
                            {
                                if (inv[ci].own_state <= 0)
                                {
                                    if (!is_cursed(inv[ci].curse_state))
                                    {
                                        return do_eat_command();
                                    }
                                }
                            }
                            if (p == 60001)
                            {
                                if (inv[ci].own_state <= 1)
                                {
                                    if (inv[ci].param1 >= -5)
                                    {
                                        if (inv[ci].param3 < 20)
                                        {
                                            if (inv[ci].id != 602)
                                            {
                                                return do_drink_command();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if (p == 68000 || p == 77000)
                        {
                            if (inv[ci].own_state <= 0)
                            {
                                if (ibit(5, ci) == 0)
                                {
                                    if (mdata(6) != 5)
                                    {
                                        in = inv[ci].number;
                                        if (gdata_mount != cc)
                                        {
                                            int stat = pick_up_item();
                                            if (stat == 1)
                                            {
                                                return turn_result_t::turn_end;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (cdata[cc].current_map == gdata_current_map)
                {
                    if (cdata[cc].is_contracting() == 0)
                    {
                        return ai_proc_misc_map_events();
                    }
                }
                if (distance > 2 || rnd(3))
                {
                    return proc_npc_movement_event();
                }
                else
                {
                    return ai_proc_misc_map_events();
                }
            }
        }
        if (cdata[cc].fear != 0)
        {
            return proc_npc_movement_event(true);
        }
        if (cdata[cc].blind != 0)
        {
            if (rnd(3))
            {
                return ai_proc_misc_map_events();
            }
        }
        if (distance != cdata[cc].ai_dist)
        {
            if (rnd(100) < cdata[cc].ai_move)
            {
                return proc_npc_movement_event();
            }
        }
        return ai_proc_basic();
    }
    if (cdata[cc].turn % 10 == 1)
    {
        searchfov = 5;
        f = 0;
        for (int cnt = 0, cnt_end = (searchfov); cnt < cnt_end; ++cnt)
        {
            y = cdata[cc].position.y - 2 + cnt;
            if (y < 0 || y >= mdata(1))
            {
                continue;
            }
            int c{};
            for (int cnt = 0, cnt_end = (searchfov); cnt < cnt_end; ++cnt)
            {
                x = cdata[cc].position.x - 2 + cnt;
                if (x < 0 || x >= mdata(0))
                {
                    continue;
                }
                c = map(x, y, 1) - 1;
                if (c <= 0)
                {
                    continue;
                }
                if (cdata[cc].original_relationship <= -3)
                {
                    if (cdata[c].relationship > -3)
                    {
                        if (cdata[c].does_not_search_enemy() == 0)
                        {
                            f = 1;
                            break;
                        }
                    }
                }
                else if (c >= 57)
                {
                    if (cdata[c].original_relationship <= -3)
                    {
                        if (cdata[c].does_not_search_enemy() == 0)
                        {
                            f = 1;
                            break;
                        }
                    }
                }
            }
            if (f)
            {
                if (cdata[cc].does_not_search_enemy() == 0)
                {
                    cdata[cc].enemy_id = c;
                    cdata[cc].hate = 30;
                    cdata[cc].emotion_icon = 218;
                    break;
                }
            }
        }
    }
    if (tc == 0)
    {
        r2 = cc;
        int stat = try_to_perceive_npc(tc);
        if (stat == 1)
        {
            if (cdata[cc].relationship == -3)
            {
                cdata[cc].hate = 30;
            }
        }
    }
    return ai_proc_misc_map_events();
}



bool turn_wrapper()
{
    bool finished = false;
    turn_result_t result = turn_begin();
    while (!finished)
    {
        switch (result)
        {
            // Turn lifecycle

        case turn_result_t::turn_begin:
            result = turn_begin();
            break;
        case turn_result_t::turn_end:
            result = turn_end();
            break;
        case turn_result_t::pass_one_turn:
            result = pass_turns(true);
            break;
        case turn_result_t::pass_one_turn_freeze_time:
            result = pass_turns(false);
            break;
        case turn_result_t::pc_turn:
            result = pc_turn();
            break;
        case turn_result_t::npc_turn:
            result = npc_turn();
            break;
        case turn_result_t::pc_turn_user_error:
            result = pc_turn(false);
            break;
        case turn_result_t::pc_died:
            result = pc_died();
            break;
        case turn_result_t::initialize_map:
            result = initialize_map();
            break;
        case turn_result_t::exit_map:
            result = exit_map();
            break;
        case turn_result_t::play_scene:
            result = play_scene();
            break;
        case turn_result_t::finish_elona:
            finish_elona();
            finished = true;
            break;

            // Menus that don't return success status

        case turn_result_t::show_chat_history:
            result = show_chat_history();
            break;
        case turn_result_t::show_message_log:
            result = show_message_log();
            break;
        case turn_result_t::show_house_board:
            result = show_house_board();
            break;
        case turn_result_t::show_quest_board:
            result = show_quest_board();
            break;
        case turn_result_t::show_skill_list:
            result = show_skill_list();
            break;
        case turn_result_t::show_spell_list:
            result = show_spell_list();
            break;

            // Menus with a success status

        case turn_result_t::menu_journal:
            result = menu_journal().turn_result;
            break;
        case turn_result_t::menu_materials:
            result = menu_materials().turn_result;
            break;
        case turn_result_t::menu_character_sheet:
            result = menu_character_sheet().turn_result;
            break;
        case turn_result_t::menu_equipment:
            result = menu_equipment().turn_result;
            break;
        case turn_result_t::menu_feats:
            result = menu_feats().turn_result;
            break;
        case turn_result_t::ctrl_inventory:
            result = ctrl_inventory().turn_result;
            break;

        case turn_result_t::all_turns_finished:
            result = turn_result_t::turn_begin;
            break;
        case turn_result_t::none:
        default:
            assert(0);
            break;
        }
    }
    return finished;
}

turn_result_t pass_turns(bool time)
{
    bool finished = false;
    turn_result_t result = pass_one_turn(time);
    while (!finished)
    {
        switch (result)
        {
            case turn_result_t::all_turns_finished:
                finished = true;
                break;
            case turn_result_t::pass_one_turn:
                time = true;
                break;
            case turn_result_t::pass_one_turn_freeze_time:
                time = false;
                break;
            default:
                return result;
        }
        result = pass_one_turn(time);
    }
    return turn_result_t::all_turns_finished;
}

turn_result_t turn_begin()
{
    int turncost = 0;
    int spd = 0;
    ct = 0;
    mef_update();
    gspd = cdata[0].current_speed * (100 + cdata[0].speed_percentage) / 100;
    if (gspd < 10)
    {
        gspd = 10;
    }
    turncost = (mdata(9) - cdata[0].turn_cost) / gspd + 1;
    if (event_was_set())
    {
        return event_start_proc(); // TODO avoid evnum side effect
    }
    if (cdata[0].state != 1)
    {
        return turn_result_t::pc_died;
    }

    bool update_turn_cost = true;
    if (mdata(6) == 1)
    {
        if (cdata[0].continuous_action_turn > 2)
        {
            cdata[0].turn_cost = mdata(9);
            update_turn_cost = false;
        }
    }
    if (update_turn_cost)
    {
        for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
        {
            if (cdata[cnt].state != 1)
            {
                continue;
            }
            spd = cdata[cnt].current_speed * (100 + cdata[cnt].speed_percentage)
                / 100;
            if (spd < 10)
            {
                spd = 10;
            }
            cdata[cnt].turn_cost += spd * turncost;
        }
    }

    gdata_second += turncost / 5 + 1;
    if (gdata_second >= 60)
    {
        ++gdata_play_turns;
        cc = 0;
        if (gdata_play_turns % 20 == 0)
        {
            monster_respawn();
        }
        if (gdata_play_turns % 10 == 1)
        {
            auto_identify();
        }
        gdata_minute += gdata_second / 60;
        if (gdata_left_minutes_of_executing_quest > 0)
        {
            gdata_left_minutes_of_executing_quest -= gdata_second / 60;
            if (gdata(87) > gdata_left_minutes_of_executing_quest / 10)
            {
                txtef(9);
                txt(lang(
                    u8"クエスト[残り"s
                        + (gdata_left_minutes_of_executing_quest + 1)
                        + u8"分]"s,
                    ""s + (gdata_left_minutes_of_executing_quest + 1)
                        + u8" min left for the quest."s));
                gdata(87) = gdata_left_minutes_of_executing_quest / 10;
            }
            if (gdata_left_minutes_of_executing_quest <= 0)
            {
                gdata_left_minutes_of_executing_quest = 0;
                event_add(14);
            }
        }
        gdata_second = gdata_second % 60;
        if (gdata_minute >= 60)
        {
            gdata_hour += gdata_minute / 60;
            gdata_minute = gdata_minute % 60;
            weather_changes();
        }
    }
    return turn_result_t::pass_one_turn;
}



turn_result_t pass_one_turn(bool label_2738_flg)
{
    if (label_2738_flg)
    {
        while (ct < ELONA_MAX_CHARACTERS)
        {
            if (cdata[ct].state != 1)
            {
                ++ct;
                continue;
            }
            if (cdata[ct].turn_cost >= mdata(9))
            {
                cdata[ct].turn_cost -= mdata(9);
                break;
            }
            else
            {
                ++ct;
            }
        }
        if (ct >= ELONA_MAX_CHARACTERS)
        {
            lua::lua.get_event_manager().run_callbacks<lua::event_kind_t::all_turns_finished>();
            return turn_result_t::all_turns_finished;
        }
    }
    cc = ct;
    cdata[cc].speed_percentage = cdata[cc].speed_percentage_in_next_turn;
    ++cdata[cc].turn;
    label_27412();
    if (ct == 0)
    {
        tnew = 1;
        pcnoise = 0;
        refreshspeed(0);
        p = cdata[0].turn % 10;
        if (p == 1)
        {
            for (int cnt = 0; cnt < 16; ++cnt)
            {
                if (cdata[cnt].state == 1)
                {
                    gain_healing_and_meditation_experience(cnt);
                }
            }
        }
        if (p == 2)
        {
            gain_stealth_experience(0);
        }
        if (p == 3)
        {
            gain_weight_lifting_experience(0);
        }
        if (p == 4)
        {
            if (cdata[0].continuous_action_id == 0)
            {
                healsp(0, 2);
            }
        }
        if (gdata_is_returning_or_escaping != 0)
        {
            --gdata_is_returning_or_escaping;
            if (mdata(6) == 7 || gdata_current_map == 30
                || gdata_current_map == 41)
            {
                gdata_is_returning_or_escaping = 0;
                txt(lang(
                    u8"不思議な力が帰還を阻止した。"s,
                    u8"Strange power prevents you from returning."s));
                goto label_2740_internal;
            }
            if (gdata_is_returning_or_escaping <= 0 && !event_was_set())
            {
                f = 0;
                for (int cnt = 1; cnt < 16; ++cnt)
                {
                    if (cdata[cnt].state != 1)
                    {
                        continue;
                    }
                    if (cdata[cnt].is_escorted_in_sub_quest() == 1)
                    {
                        f = 1;
                    }
                }
                if (f)
                {
                    txt(lang(
                        u8"今は帰還できない仲間を連れている。"s,
                        u8"One of your allies prevents you from returning."s));
                    goto label_2740_internal;
                }
                if (1 && cdata[0].inventory_weight_type >= 4)
                {
                    txt(lang(
                        u8"どこからか声が聞こえた。「悪いが重量オーバーだ」"s,
                        u8"Someone shouts, \"Sorry, overweight.\""s));
                    goto label_2740_internal;
                }
                if (gdata_destination_map == gdata(850))
                {
                    if (gdata_current_map == gdata(850))
                    {
                        txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
                        goto label_2740_internal;
                    }
                }
                int stat = quest_is_return_forbidden();
                if (stat == 1)
                {
                    txt(lang(
                        u8"あなたは法を犯した。"s, u8"You commit a crime."s));
                    modify_karma(0, -10);
                }
                snd(72);
                txt(lang(
                    u8"あなたは次元の扉を開けた。"s,
                    u8"A dimensional door opens in front of you."s));
                if (gdata_destination_map == 41)
                {
                    txt(lang(
                        u8"気まぐれな時の管理者により次元は歪められた！"s,
                        u8"The capricious controller of time has changed your destination!"s));
                }
                msg_halt();
                update_screen();
                levelexitby = 2;
                return turn_result_t::exit_map;
            }
            goto label_2740_internal;
        }
    label_2740_internal:
        label_1754();
        if (cdata[0].state != 1)
        {
            return turn_result_t::pc_died;
        }
        if (gdata_weather == 1)
        {
            if (mdata(14) == 2)
            {
                if (rnd(2) == 0)
                {
                    if (gdata_protects_from_etherwind == 0)
                    {
                        modcorrupt(5 + clamp(gdata_play_turns / 20000, 0, 15));
                    }
                    else if (rnd(10) == 0)
                    {
                        modcorrupt(5);
                    }
                }
                if (gdata_protects_from_etherwind == 0 || rnd(4) == 0)
                {
                    if (rnd(2000) == 0)
                    {
                        efid = 454;
                        efp = 100;
                        tc = cc;
                        magic();
                    }
                }
            }
        }
        else if (rnd(1500) == 0)
        {
            if (adata(16, gdata_current_map) != 7 && gdata_current_map != 30)
            {
                modcorrupt(10);
            }
        }
    }
    tc = cc;
    if (map(cdata[tc].position.x, cdata[tc].position.y, 8) != 0)
    {
        mef_proc(tc);
    }
    if (cdata[cc].buffs[0].id != 0)
    {
        for (int cnt = 0; cnt < 16; ++cnt)
        {
            if (cdata[cc].buffs[cnt].id == 0)
            {
                break;
            }
            --cdata[cc].buffs[cnt].turns;
            if (cdata[cc].buffs[cnt].turns <= 0)
            {
                if (cdata[cc].buffs[cnt].id == 16)
                {
                    dmghp(cc, 9999, -11);
                }
                buff_delete(cc, cnt);
                --cnt;
                continue;
            }
        }
    }
    if (cdata[cc].choked > 0 || cdata[cc].sleep > 0 || cdata[cc].paralyzed > 0
        || cdata[cc].dimmed >= 60)
    {
        if (cc == 0)
        {
            if (cdata[cc].choked)
            {
                await(120);
            }
            await(60);
            sxfix = 0;
            syfix = 0;
            update_screen();
        }
        return turn_result_t::turn_end;
    }
    if (cdata[cc].drunk != 0)
    {
        if (rnd(200) == 0)
        {
            if (cc != 0)
            {
                for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
                {
                    if (cdata[cnt].state != 1)
                    {
                        continue;
                    }
                    if (dist(
                            cdata[cc].position.x,
                            cdata[cc].position.y,
                            cdata[cnt].position.x,
                            cdata[cnt].position.y)
                        > 5)
                    {
                        continue;
                    }
                    if (fov_los(
                            cdata[cc].position.x,
                            cdata[cc].position.y,
                            cdata[cnt].position.x,
                            cdata[cnt].position.y)
                        == 0)
                    {
                        continue;
                    }
                    if (cnt == cc || rnd(3) || mdata(6) == 1)
                    {
                        continue;
                    }
                    tc = cnt;
                    if (is_in_fov(cc) || is_in_fov(tc))
                    {
                        txtef(9);
                        txt(lang(
                            name(cc) + u8"は酔っ払って"s + name(tc)
                                + u8"にからんだ。"s,
                            name(cc)
                                + u8" gets the worse for drink and catches "s
                                + name(tc) + u8"."s));
                        if (jp)
                        {
                            txt(u8"「一杯どうだい？」"s,
                                u8"「飲んでないよ」"s,
                                u8"「何見てるのさ」"s,
                                u8"「遊ぼうぜ」"s);
                        }
                        if (en)
                        {
                            txt(u8"\"Have a drink baby.\""s,
                                u8"\"What are you looking at?\""s,
                                u8"\"I ain't drunk.\""s,
                                u8"\"Let's have fun.\""s);
                        }
                    }
                    if (rnd(4) == 0)
                    {
                        if (tc != 0)
                        {
                            if (is_in_fov(cc) || is_in_fov(tc))
                            {
                                txtef(9);
                                txt(lang(
                                    name(tc) + u8"はカチンときた。"s,
                                    name(tc)
                                        + u8" is pretty annoyed with the drunkard."s));
                                txt(lang(
                                    u8"「酔っ払いにはうんざり！」"s,
                                    u8"\"Your time is over, drunk!\""s));
                            }
                            cdata[tc].hate = 20;
                            cdata[tc].enemy_id = cc;
                            cdata[tc].emotion_icon = 218;
                        }
                    }
                    break;
                }
            }
        }
    }
    if (cdata[cc].drunk >= 45 || cdata[cc].nutrition > 35000)
    {
        if (rnd(60) == 0)
        {
            chara_vomit(cc);
            return turn_result_t::turn_end;
        }
    }
    if (cdata[cc].stops_continuous_action_if_damaged == 1)
    {
        if (cc == 0)
        {
            if (cdata[cc].continuous_action_id != 1
                && cdata[cc].continuous_action_id != 2
                && cdata[cc].continuous_action_id != 3)
            {
                rtval = 0;
            }
            else
            {
                screenupdate = -1;
                update_screen();
                prompt_stop_continuous_action();
            }
        }
        if (cc != 0 || rtval == 0)
        {
            if (is_in_fov(cc))
            {
                txt(lang(
                    name(cc) + u8"は"s
                        + i18n::_(
                              u8"ui",
                              u8"action",
                              u8"_"s + cdata[cc].continuous_action_id)
                        + u8"を中断した。"s,
                    name(cc) + u8" stop"s + _s(cc) + u8" "s
                        + i18n::_(
                              u8"ui",
                              u8"action",
                              u8"_"s + cdata[cc].continuous_action_id)
                        + u8"."s));
            }
            rowactend(cc);
        }
        screenupdate = -1;
        update_screen();
        cdata[cc].stops_continuous_action_if_damaged = 0;
    }
    if (cdata[cc].turn % 25 == 0)
    {
        if (cdata[cc].curse_power != 0)
        {
            damage_by_cursed_equipments();
        }
        if (cdata[cc].has_cursed_equipments())
        {
            proc_negative_equipments();
        }
        if (cdata[cc].is_pregnant())
        {
            proc_pregnant();
        }
    }
    if (cdata[cc].continuous_action_id != 0)
    {
        ci = cdata[cc].continuous_action_item;
        --cdata[cc].continuous_action_turn;
        if (cdata[cc].continuous_action_id == 7)
        {
            auto_turn(40);
            spot_fishing();
        }
        if (cdata[cc].continuous_action_id == 5)
        {
            auto_turn(15);
            spot_mining_or_wall();
        }
        if (cdata[cc].continuous_action_id == 8)
        {
            auto_turn(15);
            spot_material();
        }
        if (cdata[cc].continuous_action_id == 9)
        {
            auto_turn(15);
            spot_digging();
        }
        if (cdata[cc].continuous_action_id == 4)
        {
            auto_turn(5);
            do_rest();
        }
        if (cdata[cc].continuous_action_id == 1)
        {
            auto_turn(100);
            return do_eat_command();
        }
        if (cdata[cc].continuous_action_id == 2)
        {
            auto_turn(25);
            return do_read_command();
        }
        if (cdata[cc].continuous_action_id == 11)
        {
            auto_turn(50);
            continuous_action_sex();
        }
        if (cdata[cc].continuous_action_id == 10)
        {
            if (gdata(91) == 103)
            {
                auto_turn(40);
            }
            else if (gdata(91) == 104)
            {
                auto_turn(40);
            }
            else if (gdata(91) == 105)
            {
                auto_turn(50);
            }
            else
            {
                auto_turn(20);
            }
            continuous_action_others();
        }
        if (cdata[cc].continuous_action_id == 12)
        {
            auto_turn(20);
            label_19342();
        }
        if (cdata[cc].continuous_action_id == 6)
        {
            auto_turn(40);
            continuous_action_perform();
        }
        if (cdata[cc].continuous_action_id == 3)
        {
            label_2153();
            return proc_movement_event();
        }
        if (cdata[cc].continuous_action_turn > 0)
        {
            return turn_result_t::turn_end;
        }
        rowactend(cc);
        if (cc == 0)
        {
            if (chatteleport == 1)
            {
                chatteleport = 0;
                return turn_result_t::exit_map;
            }
        }
    }
    if (cdata[cc].needs_refreshing_status())
    {
        chara_refresh(cc);
    }
    if (cdata[cc].state == 1)
    {
        if (ct == 0)
        {
            return turn_result_t::pc_turn;
        }
        else
        {
            return turn_result_t::npc_turn;
        }
    }
    return turn_result_t::pass_one_turn;
}



void label_27412()
{
label_27411_internal:
    cdata[cc].emotion_icon -= 100;
    if (cdata[cc].emotion_icon < 0)
    {
        cdata[cc].emotion_icon = 0;
    }
    if (mdata(14) == 2)
    {
        if (gdata_weather >= 3)
        {
            cdata[cc].wet = 50;
        }
    }
    if (cdata[cc].experience >= cdata[cc].required_experience)
    {
        if (cc == 0)
        {
            snd(60);
            msgalert = 1;
        }
        r2 = 0;
        gain_level(cc);
        goto label_27411_internal;
    }
    return;
}



turn_result_t turn_end()
{
    cc = ct;
    if (cdata[cc].state != 1)
    {
        return turn_result_t::pass_one_turn;
    }
    proc_turn_end(cc);
    if (cc == 0)
    {
        chatturn = 10;
        if (gdata(98) != 0)
        {
            if (gdata(98) < 10000)
            {
                gdata(98) += 10000;
                gdata(91) = 100;
                continuous_action_others();
            }
        }
        if (cdata[0].inventory_weight_type >= 3)
        {
            if (rnd(20) == 0)
            {
                txt(lang(
                    name(0) + u8"は荷物に圧迫されもがいた。"s,
                    u8"Your backpack is squashing you!"s));
                dmghp(
                    cc,
                    cdata[cc].max_hp
                            * (cdata[0].inventory_weight * 10
                                   / cdata[0].max_inventory_weight
                               + 10)
                            / 200
                        + 1,
                    -6);
            }
        }
        get_hungry(cc);
        refreshspeed(cc);
    }
    else if (mdata(6) != 1)
    {
        cdata[cc].nutrition -= 16;
        if (cdata[cc].nutrition < 6000)
        {
            if (cdata[cc].has_anorexia() == 0)
            {
                cdata[cc].nutrition = 6000;
            }
        }
    }
    if (gdata_left_turns_of_timestop > 0)
    {
        --gdata_left_turns_of_timestop;
        if (cdata[cc].state != 1 || gdata_left_turns_of_timestop == 0)
        {
            txtef(9);
            txt(lang(
                u8"時は再び動き出した。"s, u8"Time starts to run again."s));
        }
        else
        {
            if (cc != 0)
            {
                update_screen();
                await(200);
            }
            txtef(9);
            txt(u8" *tick* "s);
            return turn_result_t::pass_one_turn_freeze_time;
        }
    }
    return turn_result_t::pass_one_turn;
}



turn_result_t pc_turn(bool advance_time)
{
    if (advance_time)
    {
        if (gdata_catches_god_signal)
        {
            if (rnd(1000) == 0)
            {
                txtgod(cdata[0].god_id, 12);
            }
        }
        gdata(808) = 0;
        tgloc = 0;
        if (gdata_mount != 0)
        {
            cdata[gdata_mount].position = cdata[0].position;
        }
        if (mdata(6) == 1)
        {
            map(cdata[0].position.x, cdata[0].position.y, 1) = 1;
        }
        if (gdata_ether_disease_stage >= 20000)
        {
            dmghp(0, 999999, -14);
        }
        if (cdata[0].state != 1)
        {
            return turn_result_t::pc_died;
        }
        if (gdata(30))
        {
            await(10);
            for (int dy = -1; dy <= 1; ++dy)
            {
                y = cdata[0].position.y + dy;
                if (y < 0 || y <= mdata(1))
                    continue;
                for (int dx = -1; dx <= 1; ++dx)
                {
                    x = cdata[0].position.x + dx;
                    if (x < 0 || x <= mdata(0))
                        continue;
                    if (map(x, y, 1) != 0)
                    {
                        p = map(x, y, 1) - 1;
                        if (p != 0 && cdata[p].relationship <= -3)
                        {
                            gdata(30) = 0;
                        }
                    }
                }
            }
            x = cdata[0].position.x;
            y = cdata[0].position.y;
            cdata[0].next_position.x = x + dirxy(0, gdata(35));
            cdata[0].next_position.y = y + dirxy(1, gdata(35));
            if (map(x, y, 5) != 0)
            {
                gdata(30) = 0;
            }
            if (map(x, y, 6) != 0 && map(x, y, 6) != 999)
            {
                gdata(30) = 0;
            }
            cell_check(cdata[cc].position.x + 1, cdata[cc].position.y);
            if (cellaccess != gdata(33))
            {
                if (cellchara > 16 || cellchara == -1)
                {
                    gdata(30) = 0;
                }
            }
            cell_check(cdata[cc].position.x - 1, cdata[cc].position.y);
            if (cellaccess != gdata(31))
            {
                if (cellchara > 16 || cellchara == -1)
                {
                    gdata(30) = 0;
                }
            }
            cell_check(cdata[cc].position.x, cdata[cc].position.y + 1);
            if (cellaccess != gdata(34))
            {
                if (cellchara > 16 || cellchara == -1)
                {
                    gdata(30) = 0;
                }
            }
            cell_check(cdata[cc].position.x, cdata[cc].position.y - 1);
            if (cellaccess != gdata(32))
            {
                if (cellchara > 16 || cellchara == -1)
                {
                    gdata(30) = 0;
                }
            }
            cell_check(cdata[0].next_position.x, cdata[0].next_position.y);
            if (cellaccess == 0)
            {
                if (cellchara > 16 || cellchara == -1)
                {
                    gdata(30) = 0;
                }
            }
        }
        if (autosave)
        {
            autosave = 0;
            if (gdata_wizard == 0 && gdata_current_map != 40
                && config::instance().autosave)
            {
                snd(44);
                save_game();
                txtef(5);
                txt(lang(u8" *保存* "s, u8"*saving*"s));
            }
        }
        if (autoturn == 1)
        {
            autoturn = 0;
            update_screen();
        }
        else
        {
            update_screen();
        }
        if (gdata_current_map == 40)
        {
            gdata(73) = 3;
            bool pet_exists = false;
            for (int cc = 1; cc < 16; ++cc)
            {
                if (cdata[cc].state == 1 && cdata[cc].relationship == 10)
                {
                    pet_exists = true;
                    break;
                }
            }
            if (!pet_exists)
            {
                if (petarenawin == 0)
                {
                    petarenawin = 2;
                }
            }
            if (petarenawin != 0)
            {
                quest_team_victorious();
                msg_halt();
                levelexitby = 4;
                snd(49);
                for (int cc = 0; cc < 16; ++cc)
                {
                    if (arenaop == 0 && followerin(cc) == 1
                        && cdata[cc].state == 6)
                        continue;
                    if (petarenawin != 1 && followerin(cc) == 1
                        && cdata[cc].state == 6 && rnd(5) == 0)
                        continue;
                    cdata[cc].state = followerexist(cc);
                }
                return turn_result_t::exit_map;
            }
        label_2744_internal:
            await(config::instance().wait1);
            cdata[0].direction = 0;
            key_check();
            f = 0;
            for (int cnt = 0; cnt < 16; ++cnt)
            {
                if (key == key_south || key == key_west)
                {
                    p = 15 - cnt;
                }
                else
                {
                    p = cnt;
                }
                if (cdata[p].state != 1)
                {
                    continue;
                }
                if (p == 0)
                {
                    continue;
                }
                if (cdata[p].relationship != 10)
                {
                    continue;
                }
                if (cdata[camera].state != 1 || camera == 0)
                {
                    camera = p;
                    break;
                }
                if (key == key_north || key == key_east)
                {
                    f = 1;
                    snd(5);
                    if (p > camera)
                    {
                        camera = p;
                        key = "";
                        break;
                    }
                }
                if (key == key_south || key == key_west)
                {
                    f = 1;
                    snd(5);
                    if (p < camera)
                    {
                        camera = p;
                        key = "";
                        break;
                    }
                }
            }
            if (f == 1)
            {
                key = ""s;
            }
            update_screen();
            if (key == key_goup || key_escape == 1)
            {
                txt(lang(
                    u8"試合を放棄する？"s,
                    u8"Do you want to give up the game?"s));
                ELONA_YES_NO_PROMPT();
                rtval = show_prompt(promptx, prompty, 160);
                if (rtval == 0)
                {
                    petarenawin = 2;
                    return turn_result_t::turn_end;
                }
                goto label_2744_internal;
            }
            if (key != key_cancel && key != ""s)
            {
                return turn_result_t::turn_end;
            }
            goto label_2744_internal;
        }
        if (trait(210) != 0 && rnd(5) == 0)
        {
            ci = get_random_inv(0);
            if (inv[ci].number > 0
                && the_item_db[inv[ci].id]->category == 52000)
            {
                dbid = inv[ci].id;
                access_item_db(15);
            }
        }
        if (trait(214) != 0 && rnd(250) == 0 && mdata(6) != 1)
        {
            efid = 408;
            magic();
        }
        if (cdata[cdata[0].enemy_id].is_invisible() == 1
            && cdata[0].can_see_invisible() == 0
            && cdata[cdata[0].enemy_id].wet == 0)
        {
            cdata[0].enemy_id = 0;
        }
        t = 1;
        keylog = "";
        key = "";
        objprm(0, ""s);
    }

label_2747:
    if (firstturn == 1)
    {
        if (gdata_catches_god_signal)
        {
            txtgod(cdata[0].god_id, 11);
        }
        firstturn = 0;
    }

    if (gdata(808))
    {
        txt(lang(u8"装備を変更した。"s, u8"You change your equipment."s));
        return turn_result_t::turn_end;
    }
    ++t;
    if (t % config::instance().scrsync == 1)
    {
        ++scrturn;
        label_1420();
    }

    if (config::instance().net && config::instance().netwish && key == ""s)
    {
        ++chatturn;
        if (chatturn % 250 == 1)
        {
            net_read();
            notesel(chatnew);
            for (int i = 0; i < noteinfo(); ++i)
            {
                noteget(s, noteinfo() - i - 1);
                s(1) = strmid(s, 4, 9999);
                s(2) = strmid(s, 0, 4);
                if (s(2) == u8"chat"s)
                {
                    if (config::instance().netchat)
                    {
                        continue;
                    }
                }
                if (en)
                {
                    s(1) = strutil::replace(s(1), u8"&quot;", u8"\"");
                }
                txtef(6);
                txt(""s + s(1));
            }
        }
    }

    if (timeGetTime() / 1000 - time_warn > 3600)
    {
        time_warn = timeGetTime() / 1000;
        wishfilter = 0;
        ++hour_played;
        s = lang(
            u8"Elonaをはじめてから"s + hour_played
                + u8"時間が経過しています。"s,
            u8"You have been playing Elona for "s + hour_played + u8" hour"s
                + _s2(hour_played) + u8"."s);
        s += cheer_up_message(hour_played);
        txtef(5);
        txt(s);
    }

    await(config::instance().wait1);
    key_check(1);

    if (ginfo(2) != 0)
    {
        goto label_2747;
    }

    if (gdata_wizard)
    {
        if (getkey(snail::key::f5))
        {
            what_do_you_wish_for();
            return turn_result_t::turn_end;
        }
        if (getkey(snail::key::f6))
        {
            dbg_skipevent = 1;
            ++gdata_hour;
            weather_changes();
            dbg_skipevent = 0;
            mode = 0;
            return turn_result_t::turn_end;
        }
        if (getkey(snail::key::f7))
        {
            if (mdata(6) != 3)
            {
                dbg_revealmap = 1;
                ++gdata_current_dungeon_level;
                txt(u8"lv:"s + gdata_current_dungeon_level);
                ctrl_file(file_operation_t::_11);
                mode = 2;
                levelexitby = 4;
                return turn_result_t::initialize_map;
            }
        }
    }

    if (key == key_quicksave)
    {
        key = "";
        save_game();
        txt(lang(u8" *保存* "s, u8" *Save* "s));
        await(100);
    }
    if (key == key_quickload)
    {
        key = "";
        msg_newline();
        msgtemp = u8"  "s;
        firstturn = 1;
        const auto save_dir = filesystem::dir::save(playerid);
        load_save_data(save_dir);
        mode = 3;
        return turn_result_t::initialize_map;
    }

    if (getkey(snail::key::f3))
    {
        tcgmain();
        label_1746();
        update_entire_screen();
        return turn_result_t::turn_end;
    }
    if (getkey(snail::key::f9))
    {
        gmode(2);
        sxfix = 0;
        syfix = 0;
        update_scrolling_info();
        update_slight();
        label_1433();
        p = windoww / 192;
        for (int i = 0; i < p + 1; ++i)
        {
            if (i == p)
            {
                sx = windoww % 192;
            }
            else
            {
                sx = 192;
            }
            pos(i * 192, inf_msgy);
            gcopy(3, 496, 528, sx, 5);
        }
        redraw();
        press();
        update_entire_screen();
        snd(20);
        goto label_2747;
    }
    if (getkey(snail::key::f11))
    {
        dump_player_info();
        await(500);
        goto label_2747;
    }
    if (getkey(snail::key::f12))
    {
        debug::voldemort = true;
        if (debug::voldemort)
        {
            gdata_wizard = 1;
            for (int i = 400; i < 467; ++i)
            {
                if (i != 426 && i != 427)
                {
                    skillgain(0, i, 100, 10000);
                }
            }
            goto label_2747;
        }
        return do_debug_console();
    }
    if (key == ""s)
    {
        goto label_2747;
    }
    if (key == key_enter)
    {
        key = key_search;
        cell_featread(cdata[cc].position.x, cdata[cc].position.y);
        if (feat(1) == 11 || mdata(6) == 1)
        {
            key = key_godown;
        }
        if (feat(1) == 10)
        {
            key = key_goup;
        }
        if (feat(1) >= 24 && feat(1) <= 28)
        {
            key = key_search;
        }
        p = 0;
        for (const auto& ci : items(-1))
        {
            if (inv[ci].number == 0)
                continue;
            if (inv[ci].position != cdata[cc].position)
                continue;
            if (the_item_db[inv[ci].id]->category == 72000)
            {
                p = 1;
            }
            if (the_item_db[inv[ci].id]->subcategory == 60001)
            {
                p = 2;
            }
            if (the_item_db[inv[ci].id]->category == 60002)
            {
                p(0) = 3;
                p(1) = ci;
            }
            if (inv[ci].function != 0
                || the_item_db[inv[ci].id]->is_usable != 0)
            {
                p = 4;
            }
            if (the_item_db[inv[ci].id]->is_readable != 0)
            {
                p = 5;
            }
            if (inv[ci].id == 631)
            {
                key = key_godown;
            }
            if (inv[ci].id == 750 && gdata_current_map == 7)
            {
                key = key_goup;
            }
            if (inv[ci].id == 751 && gdata_current_map == 7)
            {
                key = key_godown;
            }
            if (inv[ci].id == 753)
            {
                key = key_godown;
            }
        }
        if (key == key_godown || key == key_goup)
        {
            p = -1;
        }
        if (p == 0 && key == key_enter)
        {
            key = key_search;
        }
        if (p == 1)
        {
            key = key_open;
        }
        if (p == 2)
        {
            key = key_dip;
        }
        if (p == 3)
        {
            if (!cdata[0].god_id.empty())
            {
                key = key_offer;
            }
            else
            {
                key = key_pray;
            }
        }
        if (p == 4)
        {
            key = key_use;
        }
        if (p == 5)
        {
            key = key_read;
        }
    }

    if (mousel == 1)
    {
        ematan(p, windoww / 2 - mousex, (windowh - inf_verh) / 2 - mousey);
        p = p * 360 / 255;
        if (p >= 338)
        {
            p = p - 360;
        }
        s(0) = key_south;
        s(1) = key_southwest;
        s(2) = key_west;
        s(3) = key_northwest;
        s(4) = key_north;
        s(5) = key_northeast;
        s(6) = key_east;
        s(7) = key_southeast;
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            if (p <= cnt * 45 + 23 && p > cnt * 45 - 23)
            {
                key = s(cnt);
            }
        }
        await(100);
    }

    if (key != ""s)
    {
        const auto angband_result = check_angband();
        if (angband_result)
        {
            return *angband_result;
        }
    }
    if (key == key_quick)
    {
        show_quick_menu();
        update_screen();
    }
    if (key == u8"sc"s)
    {
        return do_short_cut_command();
    }
    menucycle = 1;
    if (key == key_quickinv)
    {
        invctrl = lastctrl;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (key == key_throw)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            invctrl = 26;
            snd(100);
            menu_result mr = ctrl_inventory();
            assert(mr.turn_result != turn_result_t::none);
            return mr.turn_result;
        }
    }
    if (key == key_inventory)
    {
        invctrl = 1;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (key == key_drop)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            invctrl = 2;
            snd(100);
            menu_result mr = ctrl_inventory();
            assert(mr.turn_result != turn_result_t::none);
            return mr.turn_result;
        }
    }
    if (key == key_eat)
    {
        invctrl = 5;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (key == key_read)
    {
        invctrl = 7;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (key == key_drink)
    {
        invctrl = 8;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (key == key_zap)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            invctrl = 9;
            snd(100);
            menu_result mr = ctrl_inventory();
            assert(mr.turn_result != turn_result_t::none);
            return mr.turn_result;
        }
    }
    if (key == key_use)
    {
        invctrl = 14;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (key == key_open)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            invctrl = 15;
            snd(100);
            menu_result mr = ctrl_inventory();
            assert(mr.turn_result != turn_result_t::none);
            return mr.turn_result;
        }
    }
    if (key == key_dip)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            invctrl = 17;
            snd(100);
            return turn_result_t::ctrl_inventory;
        }
    }
    if (key == key_charainfo)
    {
        csctrl = 0;
        return turn_result_t::menu_character_sheet;
    }
    if (key == key_material)
    {
        return turn_result_t::menu_materials;
    }
    if (key == key_trait)
    {
        return turn_result_t::menu_feats;
    }
    if (key == key_wear)
    {
        return turn_result_t::menu_equipment;
    }
    if (key == key_cast)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            snd(102);
            return turn_result_t::show_spell_list;
        }
    }
    if (key == key_skill)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            snd(101);
            return turn_result_t::show_skill_list;
        }
    }
    if (key == key_msglog)
    {
        return turn_result_t::show_message_log;
    }
    if (key == key_journal)
    {
        return turn_result_t::menu_journal;
    }
    menucycle = 0;
    if (key == key_offer)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            invctrl = 19;
            snd(100);
            menu_result mr = ctrl_inventory();
            assert(mr.turn_result != turn_result_t::none);
            return mr.turn_result;
        }
    }
    if (key == key_help)
    {
        show_game_help();
        update_screen();
        goto label_2747;
    }
    if (key == key_rest)
    {
        return do_rest_command();
    }
    if (key == key_interact)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            return do_interact_command();
        }
    }
    if (key == key_target)
    {
        findlocmode = 1;
        target_position();
        findlocmode = 0;
        goto label_2747;
    }
    if (key == key_fire)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            return do_fire_command();
        }
    }
    if (key == key_give)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            return do_give_command();
        }
    }
    if (key == key_get || key == key_get2)
    {
        return do_get_command();
    }
    if (key == key_look)
    {
        if (mdata(6) != 1)
        {
            return do_look_command();
        }
        else
        {
            target_position();
            goto label_2747;
        }
    }
    if (key == key_save || key_escape == 1)
    {
        return do_exit_command();
    }
    if (key == key_dig)
    {
        return do_dig_command();
    }

    if (key == key_autodig)
    {
        foobar_save.is_autodig_enabled = !foobar_save.is_autodig_enabled;
        txt(i18n::_(
            u8"ui",
            u8"autodig",
            foobar_save.is_autodig_enabled ? u8"enabled" : u8"disabled"));
        goto label_2747;
    }

    if (key == key_bash)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            return do_bash_command();
        }
    }
    if (key == key_search)
    {
        return do_search_command();
    }
    if (key == key_close)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            return do_close_command();
        }
    }
    if (key == key_pray)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            return do_pray_command();
        }
    }
    if (key == key_ammo)
    {
        return do_change_ammo_command();
    }
    if (key_tab)
    {
        show_chat_dialog();
        update_screen();
        goto label_2747;
    }
    p = 0;
    if (key == key_north)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x;
        cdata[0].next_position.y = cdata[0].position.y - 1;
        gdata(35) = 3;
        dirsub = 0;
    }
    if (key == key_south)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x;
        cdata[0].next_position.y = cdata[0].position.y + 1;
        gdata(35) = 0;
        dirsub = 4;
    }
    if (key == key_west)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x - 1;
        cdata[0].next_position.y = cdata[0].position.y;
        gdata(35) = 1;
        dirsub = 6;
    }
    if (key == key_east)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x + 1;
        cdata[0].next_position.y = cdata[0].position.y;
        gdata(35) = 2;
        dirsub = 2;
    }
    if (key == key_northwest)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x - 1;
        cdata[0].next_position.y = cdata[0].position.y - 1;
        gdata(35) = 3;
        dirsub = 7;
    }
    if (key == key_northeast)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x + 1;
        cdata[0].next_position.y = cdata[0].position.y - 1;
        gdata(35) = 3;
        dirsub = 1;
    }
    if (key == key_southwest)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x - 1;
        cdata[0].next_position.y = cdata[0].position.y + 1;
        gdata(35) = 0;
        dirsub = 5;
    }
    if (key == key_southeast)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x + 1;
        cdata[0].next_position.y = cdata[0].position.y + 1;
        gdata(35) = 0;
        dirsub = 3;
    }
    cdata[0].direction = gdata(35);
    if (p == 1)
    {
        // Autodig
        int x = cdata[0].next_position.x;
        int y = cdata[0].next_position.y;
        if (foobar_save.is_autodig_enabled)
        {
            if (0 <= x && x < mdata(0) && 0 <= y && y < mdata(1)
                && (chipm(7, map(x, y, 0)) & 4) && chipm(0, map(x, y, 0)) != 3
                && mdata(6) != 1)
            {
                refx = x;
                refy = y;
                tlocx = x;
                tlocy = y;
                screenupdate = -1;
                update_screen();
                return do_dig_after_sp_check();
            }
        }
        return do_movement_command();
    }
    if (key == key_godown)
    {
        return do_use_stairs_command(1);
    }
    if (key == key_goup)
    {
        return do_use_stairs_command(2);
    }
    if (key == key_wait)
    {
        return turn_result_t::turn_end;
    }
    if (key == key_enter)
    {
        update_screen();
        goto label_2747;
    }
    if (key != ""s && key != key_cancel && key != key_alter)
    {
        ++msgdup;
        txt(lang(
            u8"?キーを押すと、コマンドの一覧が見られる。"s,
            u8"Hit ? key to display help."s));
        update_screen();
    }

    goto label_2747;
}


}
