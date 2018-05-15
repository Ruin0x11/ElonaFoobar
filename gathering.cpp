#include "gathering.hpp"
#include "action.hpp"
#include "character.hpp"
#include "variables.hpp"

namespace elona
{

void label_2154()
{
    if (rowactre != 0)
    {
        return;
    }
    ci = cdata[0].continuous_action_item;
    int dbmax = 0;
    int dbsum = 0;
    for (const auto fish : the_fish_db)
    {
        if (fish.no_generate)
        {
            continue;
        }
        i = clamp(inv[ci].param4 + (rnd(5) == 0) - (rnd(5) == 0), 0, 5);
        if (fish.rank != i)
        {
            continue;
        }
        dblist(0, dbmax) = fish.id;
        dblist(1, dbmax) = fish.rarity + dbsum;
        dbsum += fish.rarity;
        ++dbmax;
    }
    fish = 1;
    if (dbsum != 0)
    {
        int dbtmp;
        exrand_rnd(dbtmp, dbsum);
        for (int cnt = 0, cnt_end = (dbmax); cnt < cnt_end; ++cnt)
        {
            if (dblist(1, cnt) > dbtmp)
            {
                fish = dblist(0, cnt);
                break;
            }
        }
    }
    return;
}



void label_2155()
{
    flt();
    itemcreate(0, the_fish_db[fish]->item_id, -1, -1, 0);
    inv[ci].subname = fish;
    inv[ci].value = the_fish_db[fish]->value;
    inv[ci].weight = the_fish_db[fish]->weight;
    txt(lang(
        itemname(ci, 1) + u8"を釣り上げた！"s,
        u8"You get "s + itemname(ci, 1) + u8"!"s));
    item_stack(0, ci, 1);
    return;
}



void spot_fishing()
{
    int fishstat = 0;
    if (cdata[cc].continuous_action_id == 0)
    {
        txt(lang(u8"釣りを始めた。"s, u8"You start fishing."s));
        snd(87);
        if (rowactre == 0)
        {
            cdata[cc].continuous_action_item = ci;
        }
        cdata[cc].continuous_action_id = 7;
        cdata[cc].continuous_action_turn = 100;
        racount = 0;
        fishstat = 0;
        gsel(9);
        pos(0, 0);
        picload(filesystem::dir::graphic() / u8"fishing.bmp");
        gsel(0);
        return;
    }
    if (rowactre != 0)
    {
        do_gather_material_or_dig();
        return;
    }
    if (cdata[cc].continuous_action_turn > 0)
    {
        if (rnd(5) == 0)
        {
            fishstat = 1;
            label_2154();
        }
        if (fishstat == 1)
        {
            if (rnd(5) == 0)
            {
                for (int cnt = 0, cnt_end = (4 + rnd(4)); cnt < cnt_end; ++cnt)
                {
                    fishanime(0) = 1;
                    fishanime(1) = 3 + rnd(3);
                    addefmap(fishx, fishy, 4, 2);
                    ++scrturn;
                    update_screen();
                    redraw();
                    await(config::instance().wait1 * 2);
                }
                if (rnd(3) == 0)
                {
                    fishstat = 2;
                }
                if (rnd(6) == 0)
                {
                    fishstat = 0;
                }
                fishanime = 0;
            }
        }
        if (fishstat == 2)
        {
            fishanime = 2;
            snd(46);
            cdata[0].emotion_icon = 220;
            for (int cnt = 0, cnt_end = (8 + rnd(10)); cnt < cnt_end; ++cnt)
            {
                ++scrturn;
                update_screen();
                redraw();
                await(config::instance().wait1 * 2);
            }
            if (rnd(10))
            {
                fishstat = 3;
            }
            else
            {
                fishstat = 0;
            }
            fishanime = 0;
        }
        if (fishstat == 3)
        {
            fishanime = 3;
            for (int cnt = 0, cnt_end = (28 + rnd(15)); cnt < cnt_end; ++cnt)
            {
                if (cnt % 7 == 0)
                {
                    snd(89);
                }
                fishanime(1) = cnt;
                ++scrturn;
                update_screen();
                addefmap(fishx, fishy, 5, 2);
                redraw();
                await(config::instance().wait1 * 2);
            }
            if (the_fish_db[fish]->difficulty >= rnd(sdata(185, 0) + 1))
            {
                fishstat = 0;
            }
            else
            {
                fishstat = 4;
            }
            fishanime = 0;
        }
        if (fishstat == 4)
        {
            fishanime = 4;
            snd(88);
            for (int cnt = 0; cnt < 21; ++cnt)
            {
                fishanime(1) = cnt;
                if (cnt == 1)
                {
                    addefmap(fishx, fishy, 1, 3);
                }
                ++scrturn;
                update_screen();
                redraw();
                await(config::instance().wait1 * 2);
            }
            snd(14 + rnd(2));
            fishanime = 0;
            rowactend(cc);
            label_2155();
            label_1470(0);
            cdata[0].emotion_icon = 306;
        }
        if (rnd(10) == 0)
        {
            dmgsp(cc, 1);
        }
        return;
    }
    txt(lang(u8"何も釣れなかった…"s, u8"A waste of a time..."s));
    rowactend(cc);
    return;
}



void spot_material()
{
    if (cdata[cc].continuous_action_id == 0)
    {
        cdata[cc].continuous_action_id = 8;
        cdata[cc].continuous_action_turn = 40;
        txt(lang(u8"採取を始めた。"s, u8"You start to search the spot."s));
        racount = 0;
        return;
    }
    if (rowactre != 0)
    {
        do_gather_material_or_dig();
        return;
    }
    rowactend(cc);
    return;
}



void spot_digging()
{
    if (cdata[cc].continuous_action_id == 0)
    {
        cdata[cc].continuous_action_id = 9;
        cdata[cc].continuous_action_turn = 20;
        if (rowactre == 0)
        {
            txt(lang(
                u8"地面を掘り始めた。"s, u8"You start to dig the ground."s));
        }
        else
        {
            txt(lang(u8"探索を始めた。"s, u8"You start to dig the spot."s));
        }
        racount = 0;
        return;
    }
    if (rowactre != 0)
    {
        do_gather_material_or_dig();
        return;
    }
    if (cdata[cc].continuous_action_turn > 0)
    {
        if (cdata[cc].turn % 5 == 0)
        {
            txtef(4);
            txt(lang(u8" *ざくっ* "s, u8"*clink*"s),
                lang(u8" *カキン* "s, u8"*smash*"s),
                lang(u8" *ごつっ* "s, u8"*thud*"s),
                lang(u8" *じゃり* "s, u8"*sing*"s),
                lang(u8" *♪* "s, u8"*sigh*"s));
        }
        return;
    }
    txt(lang(u8"地面を掘り終えた。"s, u8"You finish digging."s));
    if (mdata(6) == 1)
    {
        for (const auto& cnt : items(0))
        {
            if (inv[cnt].number == 0)
            {
                continue;
            }
            if (inv[cnt].id == 621)
            {
                if (inv[cnt].param1 != 0)
                {
                    if (inv[cnt].param1 == cdata[0].position.x)
                    {
                        if (inv[cnt].param2 == cdata[0].position.y)
                        {
                            snd(23);
                            txtef(5);
                            txt(lang(
                                u8" *ガチッ* …何かがある！"s,
                                u8"*click* ...something is there!"s));
                            msg_halt();
                            snd(24);
                            flt();
                            itemcreate(
                                -1,
                                622,
                                cdata[0].position.x,
                                cdata[0].position.y,
                                2 + rnd(3));
                            flt();
                            itemcreate(
                                -1,
                                55,
                                cdata[0].position.x,
                                cdata[0].position.y,
                                1 + rnd(3));
                            flt();
                            itemcreate(
                                -1,
                                54,
                                cdata[0].position.x,
                                cdata[0].position.y,
                                rnd(10000) + 2000);
                            for (int cnt = 0, cnt_end = (4); cnt < cnt_end;
                                 ++cnt)
                            {
                                flt(calcobjlv(cdata[0].level + 10),
                                    calcfixlv(3));
                                if (cnt == 0)
                                {
                                    fixlv = 5;
                                }
                                flttypemajor =
                                    fsetchest(rnd(length(fsetchest)));
                                itemcreate(
                                    -1,
                                    0,
                                    cdata[0].position.x,
                                    cdata[0].position.y,
                                    0);
                            }
                            txt(lang(
                                u8"何かが足元に転がってきた。"s,
                                u8"Something is put on the ground."s));
                            autosave = 1 * (gdata_current_map != 35);
                            --inv[cnt].number;
                            break;
                        }
                    }
                }
            }
        }
    }
    spillfrag(refx, refy, 1);
    rowactend(cc);
    return;
}



void spot_mining_or_wall()
{
    int countdig = 0;
    if (cdata[cc].continuous_action_id == 0)
    {
        cdata[cc].continuous_action_id = 5;
        cdata[cc].continuous_action_turn = 40;
        if (rowactre == 0)
        {
            txt(lang(u8"壁を掘りはじめた。"s, u8"You start to dig the wall."s));
        }
        else
        {
            txt(lang(
                u8"鉱石を掘り始めた。"s,
                u8"You start to dig the mining spot."s));
        }
        if (chipm(0, map(refx, refy, 0)) == 6)
        {
            txt(lang(
                u8"この壁はとても固そうだ！"s,
                u8"These walls look pretty hard!"s));
        }
        countdig = 0;
        racount = 0;
        return;
    }
    if (rowactre != 0)
    {
        do_gather_material_or_dig();
        return;
    }
    if (cdata[cc].continuous_action_turn > 0)
    {
        if (rnd(5) == 0)
        {
            dmgsp(cc, 1);
        }
        ++countdig;
        f = 0;
        if (chipm(0, map(refx, refy, 0)) == 6)
        {
            if (rnd(12000) < sdata(10, cc) + sdata(163, cc) * 10)
            {
                f = 1;
            }
            p = 30 - sdata(163, cc) / 2;
            if (p > 0)
            {
                if (countdig <= p)
                {
                    f = 0;
                }
            }
        }
        else
        {
            if (rnd(1500) < sdata(10, cc) + sdata(163, cc) * 10)
            {
                f = 1;
            }
            p = 20 - sdata(163, cc) / 2;
            if (p > 0)
            {
                if (countdig <= p)
                {
                    f = 0;
                }
            }
        }
        if (f == 1 || gdata_tutorial_flag == 2)
        {
            rtval = 0;
            if (rnd(5) == 0)
            {
                rtval = 54;
            }
            if (rnd(8) == 0)
            {
                rtval = -1;
            }
            if (map(refx, refy, 6) != 0)
            {
                cell_featread(refx, refy);
                if (feat(1) == 22)
                {
                    discover_hidden_path();
                }
            }
            map(refx, refy, 0) = tile_tunnel;
            spillfrag(refx, refy, 2);
            FIXME_dig_animation_x = refx;
            FIXME_dig_animation_y = refy;
            snd(45);
            aniref = 5;
            play_animation(14);
            txt(lang(
                u8"壁を掘り終えた。"s, u8"You finished digging the wall."s));
            if (gdata_tutorial_flag == 2 && gdata_current_map == 7)
            {
                flt();
                itemcreate(-1, 208, digx, digy, 0);
                inv[ci].curse_state = curse_state_t::cursed;
                txt(lang(
                    u8"何かを見つけた。"s,
                    u8"You found something out of crushed heaps of rock."s));
                gdata_tutorial_flag = 3;
            }
            else if (rtval != 0 && gdata_current_map != 30)
            {
                if (rtval > 0)
                {
                    flt();
                    itemcreate(-1, rtval, digx, digy, 0);
                }
                else if (rtval == -1)
                {
                    flt(calcobjlv(gdata_current_dungeon_level), calcfixlv(3));
                    flttypemajor = 77000;
                    itemcreate(-1, 0, digx, digy, 0);
                }
                txt(lang(
                    u8"何かを見つけた。"s,
                    u8"You found something out of crushed heaps of rock."s));
            }
            label_1457();
            rowactend(cc);
        }
        else if (cdata[cc].turn % 5 == 0)
        {
            txtef(4);
            txt(lang(u8" *ざくっ* "s, u8"*clink*"s),
                lang(u8" *カキン* "s, u8"*smash*"s),
                lang(u8" *ごつっ* "s, u8"*thud*"s),
                lang(u8" *じゃり* "s, u8"*sing*"s),
                lang(u8" *♪* "s, u8"*sigh*"s));
        }
        return;
    }
    txt(lang(
        u8"背中が痛い…掘るのを諦めた。"s,
        u8"Your back hurts...You give up digging."s));
    rowactend(cc);
    return;
}


int do_gather_material_or_dig()
{
    if (map(cdata[0].position.x, cdata[0].position.y, 6) == 0)
    {
        return 0;
    }
    if (feat(1) < 24 || 28 < feat(1))
    {
        return 0;
    }
    atxspot = 11;
    atxlv = gdata_current_dungeon_level;
    if (mdata(6) == 20)
    {
        atxspot = 9;
    }
    if (mdata(6) == 21)
    {
        atxspot = 12;
    }
    if (mdata(6) == 22)
    {
        atxspot = 10;
    }
    if (mdata(6) == 23)
    {
        atxspot = 12;
    }
    if (mdata(6) == 1)
    {
        atxlv = cdata[0].level / 2 + rnd(10);
        if (atxlv > 30)
        {
            atxlv = 30 + rnd((rnd(atxlv - 30) + 1));
        }
        if (4 <= gdata(62) && gdata(62) < 9)
        {
            atxspot = 10;
        }
        if (264 <= gdata(62) && gdata(62) < 363)
        {
            atxspot = 11;
        }
        if (9 <= gdata(62) && gdata(62) < 13)
        {
            atxspot = 10;
        }
        if (13 <= gdata(62) && gdata(62) < 17)
        {
            atxspot = 11;
        }
    }
    cell_featread(cdata[0].position.x, cdata[0].position.y);
    if (feat(1) == 27)
    {
        atxlv += sdata(161, 0) / 3;
    }
    if (feat(1) == 26)
    {
        atxspot = 13;
    }
    if (feat(1) == 25)
    {
        atxspot = 14;
    }
    if (feat(1) == 28)
    {
        atxspot = 15;
    }
    if (rnd(7) == 0)
    {
        for (int cnt = 0; cnt < 1; ++cnt)
        {
            i = 5;
            if (atxspot == 14)
            {
                if (sdata(163, 0) < rnd(atxlv * 2 + 1) || rnd(10) == 0)
                {
                    txt(lang(
                        u8"採掘に失敗した。"s,
                        u8"Your mining attempt fails."s));
                    break;
                }
                i = 1;
                skillexp(163, 0, 40);
            }
            if (atxspot == 13)
            {
                if (sdata(185, 0) < rnd(atxlv * 2 + 1) || rnd(10) == 0)
                {
                    txt(lang(
                        u8"釣りに失敗した。"s,
                        u8"Your fishing attempt fails."s));
                    break;
                }
                i = 2;
                skillexp(185, 0, 40);
            }
            if (atxspot == 15)
            {
                if (sdata(180, 0) < rnd(atxlv * 2 + 1) || rnd(10) == 0)
                {
                    txt(lang(
                        u8"採取に失敗した。"s,
                        u8"Your searching attempt fails."s));
                    break;
                }
                i = 3;
                skillexp(180, 0, 30);
            }
            matgetmain(random_material(atxlv, 0), 1, i);
        }
    }
    if (rnd(50 + trait(159) * 20) == 0)
    {
        s = lang(u8"もう何もない。"s, u8"You can't find anything anymore."s);
        if (feat(1) == 26)
        {
            s = lang(u8"泉は干上がった。"s, u8"The spring dries up."s);
        }
        if (feat(1) == 25)
        {
            s = lang(
                u8"鉱石を掘りつくした。"s, u8"There's no more ore around."s);
        }
        if (feat(1) == 28)
        {
            s = lang(
                u8"もう目ぼしい植物は見当たらない。"s,
                u8"There's no more plant around."s);
        }
        txt(s);
        rowactend(cc);
        map(cdata[0].position.x, cdata[0].position.y, 6) = 0;
    }
    return 0;
}

} // namespace elona
