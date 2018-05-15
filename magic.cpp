#include "ability.hpp"
#include "animation.hpp"
#include "audio.hpp"
#include "buff.hpp"
#include "calc.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "config.hpp"
#include "ctrl_file.hpp"
#include "debug.hpp"
#include "elona.hpp"
#include "enchantment.hpp"
#include "food.hpp"
#include "fov.hpp"
#include "i18n.hpp"
#include "input.hpp"
#include "item.hpp"
#include "item_db.hpp"
#include "itemgen.hpp"
#include "macro.hpp"
#include "map.hpp"
#include "map_cell.hpp"
#include "mef.hpp"
#include "menu.hpp"
#include "quest.hpp"
#include "status_ailment.hpp"
#include "trait.hpp"
#include "ui.hpp"
#include "variables.hpp"
#include "wish.hpp"


namespace elona
{


int magic()
{
    int efcibk = 0;
    int efbad = 0;
    int chainbomb = 0;
    elona_vector1<int> chainbomblist;
    int tcprev = 0;
    int telex = 0;
    int teley = 0;
    int efidprev = 0;
    int ccprev = 0;
    int fltbk = 0;
    int valuebk = 0;
    efcibk = ci;
    efcancel = 0;
    obvious = 1;
    if (efsource != 4 && efsource != 1 && efsource != 2)
    {
        efstatus = curse_state_t::none;
    }
    efsource = 0;
    efbad = 0;


    if (efid >= 300)
    {
        if (efid < 661)
        {
            f = 0;
            if (the_ability_db[efid]->sdataref1 / 1000 == 1)
            {
                f = 1;
                p = the_ability_db[efid]->sdataref1 % 1000;
                if (the_buff_db[p]->type == buff_data::type_t::hex)
                {
                    efbad = 1;
                }
            }
            if (the_ability_db[efid]->sdataref1 == 7)
            {
                efbad = 1;
            }
            if (efbad == 0)
            {
                if (efstatus == curse_state_t::blessed)
                {
                    efp = efp * 150 / 100;
                }
                if (is_cursed(efstatus))
                {
                    efp = 50;
                }
            }
            else
            {
                if (efstatus == curse_state_t::blessed)
                {
                    efp = 50;
                }
                if (is_cursed(efstatus))
                {
                    efp = efp * 150 / 100;
                }
            }
            if (f)
            {
                if (the_buff_db[p]->type == buff_data::type_t::buff)
                {
                    animeload(11, tc);
                }
                else if (the_buff_db[p]->type == buff_data::type_t::hex)
                {
                    play_animation(6);
                }
                if (efid == 625 || efid == 446)
                {
                    if ((tc == 0 && cc == 0) || cc == gdata_mount)
                    {
                        if (gdata_mount != 0)
                        {
                            tc = gdata_mount;
                        }
                    }
                }
                buff_add(tc, p, efp, calc_buff_duration(p, efp));
                if (efid == 447)
                {
                    if (efstatus == curse_state_t::blessed)
                    {
                        cdata[tc].birth_year += rnd(3) + 1;
                        if (cdata[tc].birth_year + 12 > gdata_year)
                        {
                            cdata[tc].birth_year = gdata_year - 12;
                        }
                        if (is_in_fov(tc))
                        {
                            txtef(2);
                            txt(lang(
                                name(tc) + u8"の老化は遅くなった。"s,
                                name(tc) + your(tc)
                                    + u8" aging process slows down."s));
                        }
                    }
                }
                if (efid == 446)
                {
                    if (is_cursed(efstatus))
                    {
                        cdata[tc].birth_year -= rnd(3) + 1;
                        if (is_in_fov(tc))
                        {
                            txtef(8);
                            txt(lang(
                                name(tc) + u8"の老化は速くなった。"s,
                                name(tc) + your(tc)
                                    + u8" aging process speeds up."s));
                        }
                    }
                }
                if (efid == 458)
                {
                    if (tc == 0)
                    {
                        incognitobegin();
                    }
                }
                goto the_end;
            }
            if (const auto damage = calc_skill_damage(efid, cc, efp))
            {
                dice1 = damage->dice_x;
                dice2 = damage->dice_y;
                bonus = damage->damage_bonus;
                ele = damage->element;
                elep = damage->element_power;
            }
            if (cc == 0)
            {
                if (trait(165))
                {
                    if (ele == 50 || ele == 51 || ele == 52)
                    {
                        dice2 = dice2 * 125 / 100;
                    }
                }
            }
            if (rapidmagic)
            {
                efp = efp / 2 + 1;
                dice1 = dice1 / 2 + 1;
                dice2 = dice2 / 2 + 1;
                bonus = bonus / 2 + 1;
            }
            switch (the_ability_db[efid]->sdataref1)
            {
            case 10:
                play_animation(18);
                try_to_melee_attack();
                goto the_end;
            case 1:
            {
                int stat = get_route(
                    cdata[cc].position.x, cdata[cc].position.y, tlocx, tlocy);
                if (stat == 0)
                {
                    goto the_end;
                }
            }
                play_animation(0);
                dx = cdata[cc].position.x;
                dy = cdata[cc].position.y;
                for (int cnt = 0; cnt < 20; ++cnt)
                {
                    int stat = route_info(dx, dy, cnt);
                    if (stat == 0)
                    {
                        break;
                    }
                    else if (stat == -1)
                    {
                        continue;
                    }
                    if (dist(dx, dy, cdata[cc].position.x, cdata[cc].position.y)
                        > the_ability_db[efid]->sdataref3 % 1000 + 1)
                    {
                        break;
                    }
                    if (dx == cdata[cc].position.x
                        && dy == cdata[cc].position.y)
                    {
                        continue;
                    }
                    if (ele == 50)
                    {
                        mapitem_fire(dx, dy);
                    }
                    if (ele == 51)
                    {
                        mapitem_cold(dx, dy);
                    }
                    if (map(dx, dy, 1) != 0)
                    {
                        tc = map(dx, dy, 1) - 1;
                        if (cc != tc)
                        {
                            if (gdata_mount != 0)
                            {
                                if (gdata_mount == cc)
                                {
                                    if (tc == 0)
                                    {
                                        continue;
                                    }
                                }
                            }
                            dmg = roll(dice1, dice2, bonus);
                            int stat = calcmagiccontrol(cc, tc);
                            if (stat == 1)
                            {
                                continue;
                            }
                            if (is_in_fov(tc))
                            {
                                if (tc >= 16)
                                {
                                    gdata(809) = 2;
                                    txt3rd = 1;
                                    txt(lang(
                                        u8"ボルトは"s + name(tc)
                                            + u8"に命中し"s,
                                        u8"The bolt hits "s + name(tc)
                                            + u8" and"s));
                                }
                                else
                                {
                                    txt(lang(
                                        u8"ボルトが"s + name(tc)
                                            + u8"に命中した。"s,
                                        u8"The bolt hits "s + name(tc)
                                            + u8"."s));
                                }
                            }
                            dmghp(tc, dmg, cc, ele, elep);
                        }
                    }
                }
                goto the_end;
            case 3:
                chainbomb = 0;
                ccbk = cc;
                if (efid == 644)
                {
                    stxt(
                        cc,
                        lang(
                            name(cc) + u8"は爆発した。"s,
                            name(cc) + u8" explode"s + _s(cc) + u8"."s));
                }
            label_2177_internal:
                cdata[cc].will_explode_soon() = false;
                range_ = the_ability_db[efid]->sdataref3 % 1000 + 1;
                if (debug::voldemort && cc == 0)
                {
                    range_ *= 2;
                }
                if (efid == 644)
                {
                    range_ = 2;
                }
                if (efid != 404 && efid != 637)
                {
                    aniref = range_;
                    anix = tlocx;
                    aniy = tlocy;
                    play_animation(2);
                }
                for (int cnt = 0, cnt_end = (range_ * 2 + 1); cnt < cnt_end;
                     ++cnt)
                {
                    dy = tlocy - range_ + cnt;
                    if (dy < 0 || dy >= mdata(1))
                    {
                        continue;
                    }
                    for (int cnt = 0, cnt_end = (range_ * 2 + 1); cnt < cnt_end;
                         ++cnt)
                    {
                        dx = tlocx - range_ + cnt;
                        if (dx < 0 || dx >= mdata(0))
                        {
                            continue;
                        }
                        if (dist(tlocx, tlocy, dx, dy) > range_)
                        {
                            continue;
                        }
                        if (fov_los(tlocx, tlocy, dx, dy) == 0)
                        {
                            continue;
                        }
                        if (map(dx, dy, 1) == 0)
                        {
                            continue;
                        }
                        tc = map(dx, dy, 1) - 1;
                        if (efid == 404)
                        {
                            f = 0;
                            if (cc == 0 || cdata[cc].relationship >= 0)
                            {
                                if (cdata[tc].relationship >= 0)
                                {
                                    f = 1;
                                }
                            }
                            else if (cdata[tc].relationship <= -1)
                            {
                                f = 1;
                            }
                            if (f == 1)
                            {
                                play_animation(11);
                                if (is_in_fov(tc))
                                {
                                    txt(lang(
                                        name(tc) + u8"は回復した。"s,
                                        name(tc) + u8" "s + is(tc)
                                            + u8" healed."s));
                                }
                                label_2187();
                            }
                            continue;
                        }
                        if (efid == 637)
                        {
                            f = 0;
                            if (cc == 0 || cdata[cc].relationship >= 0)
                            {
                                if (cdata[tc].relationship >= 0)
                                {
                                    f = 1;
                                }
                            }
                            else if (cdata[tc].relationship <= -1)
                            {
                                f = 1;
                            }
                            if (f == 1)
                            {
                                play_animation(11);
                                txt(lang(
                                    name(tc) + u8"の狂気は消え去った。"s,
                                    name(tc) + u8" "s + is(tc)
                                        + u8" completely sane again."s));
                                healsan(tc, efp / 10);
                                healcon(tc, 11, 9999);
                            }
                            continue;
                        }
                        if (dx == cdata[cc].position.x
                            && dy == cdata[cc].position.y)
                        {
                            continue;
                        }
                        if (gdata_mount != 0)
                        {
                            if (gdata_mount == cc)
                            {
                                if (tc == 0)
                                {
                                    continue;
                                }
                            }
                        }
                        if (ele == 50)
                        {
                            mapitem_fire(dx, dy);
                        }
                        if (ele == 51)
                        {
                            mapitem_cold(dx, dy);
                        }
                        if (cc != tc)
                        {
                            dmg = roll(dice1, dice2, bonus) * 100
                                / (75 + dist(tlocx, tlocy, dx, dy) * 25);
                            int stat = calcmagiccontrol(cc, tc);
                            if (stat == 1)
                            {
                                continue;
                            }
                            if (efid == 644)
                            {
                                if (is_in_fov(tc))
                                {
                                    if (tc >= 16)
                                    {
                                        gdata(809) = 2;
                                        txt3rd = 1;
                                        txt(lang(
                                            u8"爆風は"s + name(tc)
                                                + u8"に命中し"s,
                                            u8"The explosion hits "s + name(tc)
                                                + u8" and"s));
                                    }
                                    else
                                    {
                                        txt(lang(
                                            u8"爆風が"s + name(tc)
                                                + u8"に命中した。"s,
                                            u8"The explosion hits "s + name(tc)
                                                + u8"."s));
                                    }
                                }
                                if (cdata[tc].explodes())
                                {
                                    chainbomblist(chainbomb) = tc;
                                    ++chainbomb;
                                    continue;
                                }
                            }
                            else if (is_in_fov(tc))
                            {
                                if (tc >= 16)
                                {
                                    gdata(809) = 2;
                                    txt3rd = 1;
                                    txt(lang(
                                        u8"ボールは"s + name(tc)
                                            + u8"に命中し"s,
                                        u8"The ball hits "s + name(tc)
                                            + u8" and"s));
                                }
                                else
                                {
                                    txt(lang(
                                        u8"ボールが"s + name(tc)
                                            + u8"に命中した。"s,
                                        u8"The Ball hits "s + name(tc)
                                            + u8"."s));
                                }
                            }
                            dmghp(tc, dmg, cc, ele, elep);
                        }
                    }
                }
                if (efid == 644)
                {
                    dmghp(cc, 99999, -16);
                }
                if (chainbomb > 0)
                {
                    while (1)
                    {
                        --chainbomb;
                        if (chainbomb < 0)
                            break;
                        cc = chainbomblist(chainbomb);
                        tlocx = cdata[cc].position.x;
                        tlocy = cdata[cc].position.y;
                        if (cdata[cc].state == 1)
                        {
                            const auto damage =
                                calc_skill_damage(efid, cc, efp);
                            dice1 = damage->dice_x;
                            dice2 = damage->dice_y;
                            bonus = damage->damage_bonus;
                            ele = damage->element;
                            elep = damage->element_power;
                            stxt(
                                cc,
                                lang(
                                    name(cc) + u8"は誘爆した。"s,
                                    name(cc) + u8" explode"s + _s(cc)
                                        + u8"."s));
                            goto label_2177_internal;
                        }
                    }
                }
                cc = ccbk;
                goto the_end;
            case 2:
                play_animation(1);
                dmg = roll(dice1, dice2, bonus);
                if (is_in_fov(tc))
                {
                    if (tc >= 16)
                    {
                        gdata(809) = 2;
                        txt3rd = 1;
                        txt(lang(
                            u8"矢は"s + name(tc) + u8"に命中し"s,
                            u8"The arrow hits "s + name(tc) + u8" and"s));
                    }
                    else
                    {
                        txt(lang(
                            u8"矢が"s + name(tc) + u8"に命中した。"s,
                            u8"The arrow hits "s + name(tc) + u8"."s));
                    }
                }
                dmghp(tc, dmg, cc, ele, elep);
                goto the_end;
            case 4:
                if (efid == 400)
                {
                    if (is_in_fov(tc))
                    {
                        txt(lang(
                            name(tc) + u8"の傷はふさがった。"s,
                            name(tc) + u8" "s + is(tc)
                                + u8" slightly healed."s));
                    }
                }
                if (efid == 401 || efid == 405)
                {
                    if (is_in_fov(tc))
                    {
                        txt(lang(
                            name(tc) + u8"は回復した。"s,
                            name(tc) + u8" "s + is(tc) + u8" healed."s));
                    }
                }
                if (efid == 402)
                {
                    if (is_in_fov(tc))
                    {
                        txt(lang(
                            name(tc) + u8"の身体に生命力がみなぎった。"s,
                            name(tc) + u8" "s + is(tc)
                                + u8" greatly healed."s));
                    }
                }
                if (efid == 403)
                {
                    if (is_in_fov(tc))
                    {
                        txt(lang(
                            name(tc) + u8"は完全に回復した。"s,
                            name(tc) + u8" "s + is(tc)
                                + u8" completely healed."s));
                    }
                }
                label_2187();
                if (efstatus == curse_state_t::blessed)
                {
                    healcon(tc, 12, 5 + rnd(5));
                }
                sickifcursed(efstatus, tc, 3);
                play_animation(5);
                goto the_end;
            case 6:
                if (cdata[cc].special_attack_type != 0)
                {
                    if (is_in_fov(cc))
                    {
                        if (cc == 0)
                        {
                            txt(lang(
                                name(cc) + u8"は"s
                                    + i18n::_(
                                          u8"ability",
                                          std::to_string(efid),
                                          u8"name")
                                    + u8"の"s
                                    + i18n::_(
                                          u8"ui",
                                          u8"cast_style",
                                          u8"_"s
                                              + cdata[cc].special_attack_type),
                                name(cc) + u8" cast "s
                                    + i18n::_(
                                          u8"ability",
                                          std::to_string(efid),
                                          u8"name")
                                    + u8"."s));
                        }
                        else
                        {
                            txt(lang(
                                name(cc) + u8"は"s
                                    + i18n::_(
                                          u8"ui",
                                          u8"cast_style",
                                          u8"_"s
                                              + cdata[cc].special_attack_type),
                                name(cc) + ""s
                                    + i18n::_(
                                          u8"ui",
                                          u8"cast_style",
                                          u8"_"s
                                              + cdata[cc]
                                                    .special_attack_type)));
                        }
                    }
                }
                else if (efid == 601)
                {
                    if (is_in_fov(cc))
                    {
                        if (tc >= 16)
                        {
                            gdata(809) = 2;
                            txt(lang(
                                aln(cc) + name(tc) + u8"の血を吸い"s,
                                name(cc) + u8" suck"s + _s(cc) + u8" "s
                                    + name(tc) + your(tc) + u8" blood and"s));
                        }
                        else
                        {
                            txt(lang(
                                name(cc) + u8"に血を吸われた。"s,
                                name(cc) + u8" suck"s + _s(cc) + u8" "s
                                    + name(tc) + your(tc) + u8" blood."s));
                        }
                    }
                }
                else if (is_in_fov(cc))
                {
                    if (tc >= 16)
                    {
                        gdata(809) = 2;
                        txt(lang(
                            aln(cc) + name(tc) + u8"を"s + elename(ele)
                                + _melee(2, cdata[cc].melee_attack_type)
                                + u8"で"s
                                + _melee(0, cdata[cc].melee_attack_type),
                            name(cc) + u8" touch"s + _s(cc) + u8" "s + name(tc)
                                + u8" with "s + his(cc) + u8" "s + elename(ele)
                                + u8" "s
                                + _melee(2, cdata[cc].melee_attack_type)
                                + u8" and"s));
                    }
                    else
                    {
                        txt(lang(
                            name(tc) + u8"は"s + name(cc) + u8"に"s
                                + elename(ele)
                                + _melee(2, cdata[cc].melee_attack_type)
                                + u8"で"s
                                + _melee(1, cdata[cc].melee_attack_type),
                            name(cc) + u8" touch"s + _s(cc) + u8" "s + name(tc)
                                + u8" with "s + his(cc) + u8" "s + elename(ele)
                                + u8" "s
                                + _melee(2, cdata[cc].melee_attack_type)
                                + u8"."s));
                    }
                }
                if (efid == 660)
                {
                    txt(lang(
                        u8"「余分な機能は削除してしまえ」"s, u8"\"Delete.\""s));
                    cdata[tc].hp = cdata[tc].max_hp / 12 + 1;
                    goto the_end;
                }
                dmghp(tc, roll(dice1, dice2, bonus), cc, ele, elep);
                if (efid == 617)
                {
                    dmgcon(tc, status_ailment_t::fear, elep);
                }
                if (efid == 618)
                {
                    dmgcon(tc, status_ailment_t::sleep, elep);
                }
                if (efid == 614)
                {
                    cdata[tc].nutrition -= 800;
                    if (is_in_fov(tc))
                    {
                        txtef(8);
                        txt(lang(
                            name(tc) + u8"はお腹が減った。"s,
                            u8"Suddenly "s + name(tc) + u8" feel"s + _s(tc)
                                + u8" hungry."s));
                    }
                    get_hungry(tc);
                }
                if (efid == 613)
                {
                    p = rnd(10);
                    if ((cdata[tc].quality >= 4 && rnd(4))
                        || encfind(tc, 60010 + p) != -1)
                    {
                        p = -1;
                    }
                    if (p != -1)
                    {
                        i = sdata.get(10 + p, tc).original_level
                            - cdata[tc].attr_adjs[p];
                        if (i > 0)
                        {
                            i = i * efp / 2000 + 1;
                            cdata[tc].attr_adjs[p] -= i;
                        }
                        if (is_in_fov(tc))
                        {
                            txtef(8);
                            txt(lang(
                                name(tc) + u8"は弱くなった。"s,
                                name(tc) + u8" "s + is(tc) + u8" weakened."s));
                        }
                        chara_refresh(tc);
                    }
                }
                goto the_end;
            case 7:
                if (cc == 0)
                {
                    if (gdata_other_character_count + 100
                        >= ELONA_MAX_OTHER_CHARACTERS)
                    {
                        txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
                        obvious = 0;
                        goto the_end;
                    }
                }
                p = 3;
                efp = (efp / 25 + efp * efp / 10000 + cdata[cc].level) / 2;
                if (efp < 1)
                {
                    efp = 1;
                }
                if (efid == 641)
                {
                    efp = 15 + rnd(8);
                }
                if (efid == 639)
                {
                    efp = 2 + rnd(18);
                }
                if (efid == 642)
                {
                    efp = 15 + rnd(15);
                }
                if (efid == 640)
                {
                    efp = 5 + rnd(12);
                }
                if (efid == 643)
                {
                    p = 10;
                }
                for (int cnt = 0, cnt_end = (1 + rnd(p)); cnt < cnt_end; ++cnt)
                {
                    flt(calcobjlv(efp), 2);
                    dbid = 0;
                    if (efid == 425)
                    {
                        fltn(u8"wild"s);
                    }
                    if (efid == 642)
                    {
                        fltn(u8"fire"s);
                    }
                    if (efid == 641)
                    {
                        fltn(u8"pawn"s);
                    }
                    if (efid == 639)
                    {
                        fltn(u8"cat"s);
                    }
                    if (efid == 640)
                    {
                        fltn(u8"yeek"s);
                    }
                    if (efid == 643)
                    {
                        dbid = 176;
                    }
                    chara_create(
                        -1, dbid, cdata[tc].position.x, cdata[tc].position.y);
                    if (efid != 643)
                    {
                        if (cdata[rc].id == cdata[cc].id)
                        {
                            chara_vanquish(rc);
                            --cnt;
                            continue;
                        }
                    }
                }
                if (is_in_fov(cc))
                {
                    txt(lang(
                        u8"魔法でモンスターが召喚された。"s,
                        u8"Several monsters come out from a portal."s));
                }
                goto the_end;
            case 5:
                tcprev = tc;
                if (gdata_mount != 0)
                {
                    if (gdata_mount == tc)
                    {
                        goto the_end;
                    }
                }
                if (efid == 408)
                {
                    tc = cc;
                }
                if (efid == 619)
                {
                    telex = cdata[tc].position.x;
                    teley = cdata[tc].position.y;
                    tc = cc;
                }
                if (efid == 620)
                {
                    telex = cdata[cc].position.x;
                    teley = cdata[cc].position.y;
                }
                if (efid == 409 || efid == 635)
                {
                    if (map(tlocx, tlocy, 1) == 0)
                    {
                        txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
                        obvious = 0;
                        goto the_end;
                    }
                    tc = map(tlocx, tlocy, 1) - 1;
                }
                if (gdata_current_map == 40 || mdata(6) == 1
                    || gdata_current_map == 37 || gdata_current_map == 41)
                {
                    if (is_in_fov(tc))
                    {
                        txt(lang(
                            u8"魔法の力がテレポートを防いだ。"s,
                            u8"Magical field prevents teleportation."s));
                    }
                    goto the_end;
                }
                if (efid != 619 && encfind(tc, 22) != -1)
                {
                    if (is_in_fov(tc))
                    {
                        txt(lang(
                            u8"魔法の力がテレポートを防いだ。"s,
                            u8"Magical field prevents teleportation."s));
                    }
                    goto the_end;
                }
                if (efid == 635)
                {
                    if (tc == cc)
                    {
                        if (is_in_fov(tc))
                        {
                            txt(lang(
                                u8"魔法の力がテレポートを防いだ。"s,
                                u8"Magical field prevents teleportation."s));
                        }
                        goto the_end;
                    }
                    p = rnd(cdata[tc].gold / 10 + 1);
                    if (rnd(sdata(13, tc)) > rnd(sdata(12, cc) * 4)
                        || cdata[tc].is_protected_from_thieves() == 1)
                    {
                        txt(lang(
                            name(tc) + u8"は自分の財布を守った。"s,
                            name(tc) + u8" guard"s + _s(tc) + u8" "s + his(tc)
                                + u8" wallet from a thief."s));
                        p = 0;
                    }
                    if (p != 0)
                    {
                        snd(12);
                        cdata[tc].gold -= p;
                        txt(lang(
                            name(cc) + u8"は"s + name(tc) + u8"から"s + p
                                + u8"枚の金貨を奪った。"s,
                            name(cc) + u8" steal"s + _s(cc) + u8" "s + p
                                + u8" gold pieces from "s + name(tc) + u8"."s));
                        cdata[cc].gold += p;
                    }
                    tc = cc;
                    if (gdata_mount != 0)
                    {
                        if (gdata_mount == tc)
                        {
                            goto the_end;
                        }
                    }
                }
                if (is_in_fov(tc))
                {
                    snd(72);
                }
                tx = cdata[tc].position.x;
                ty = cdata[tc].position.y;
                efidprev = efid;
                for (int cnt = 0; cnt < 200; ++cnt)
                {
                    if (efidprev == 410 || efidprev == 627)
                    {
                        p(0) = -1;
                        p(1) = 1;
                        cdata[tc].next_position.x = cdata[tc].position.x
                            + (3 - cnt / 70 + rnd(5)) * p(rnd(2));
                        cdata[tc].next_position.y = cdata[tc].position.y
                            + (3 - cnt / 70 + rnd(5)) * p(rnd(2));
                    }
                    else if (efidprev == 619)
                    {
                        cdata[tc].next_position.x =
                            telex + rnd((cnt / 8 + 2)) - rnd((cnt / 8 + 2));
                        cdata[tc].next_position.y =
                            teley + rnd((cnt / 8 + 2)) - rnd((cnt / 8 + 2));
                    }
                    else if (efidprev == 620)
                    {
                        cdata[tc].next_position.x =
                            telex + rnd((cnt / 8 + 2)) - rnd((cnt / 8 + 2));
                        cdata[tc].next_position.y =
                            teley + rnd((cnt / 8 + 2)) - rnd((cnt / 8 + 2));
                    }
                    else
                    {
                        cdata[tc].next_position.x = rnd(mdata(0) - 2) + 1;
                        cdata[tc].next_position.y = rnd(mdata(1) - 2) + 1;
                    }
                    cell_check(
                        cdata[tc].next_position.x, cdata[tc].next_position.y);
                    if (cellaccess == 1)
                    {
                        if (efidprev == 619)
                        {
                            if (is_in_fov(cc))
                            {
                                txt(lang(
                                    name(cc) + u8"は"s + cdatan(0, tcprev)
                                        + u8"の元に移動した。"s,
                                    name(cc) + u8" teleport"s + _s(cc)
                                        + u8" toward "s + cdatan(0, tcprev)
                                        + u8"."s));
                            }
                        }
                        else if (efidprev == 620)
                        {
                            if (is_in_fov(cc))
                            {
                                txt(lang(
                                    name(tc) + u8"は引き寄せられた。"s,
                                    name(tc) + u8" "s + is(tc) + u8" drawn."s));
                            }
                        }
                        else if (is_in_fov(cc))
                        {
                            if (efidprev == 635)
                            {
                                txt(lang(
                                    u8"泥棒は笑って逃げた。"s,
                                    u8"A thief escapes laughing."s));
                            }
                            else
                            {
                                txt(lang(
                                    name(tc) + u8"は突然消えた。"s,
                                    u8"Suddenly, "s + name(tc) + u8" disappear"s
                                        + _s(tc) + u8"."s));
                            }
                        }
                        rowactend(cc);
                        ccprev = cc;
                        cc = tc;
                        label_21452();
                        cc = ccprev;
                        if (tc == 0)
                        {
                            update_screen();
                        }
                        break;
                    }
                }
                tc = tcprev;
                goto the_end;
            case 8:
                int stat = get_route(
                    cdata[cc].position.x, cdata[cc].position.y, tlocx, tlocy);
                if (stat == 0)
                {
                    goto the_end;
                }
                std::string valn;
                if (ele)
                {
                    valn = i18n::_(u8"ability", std::to_string(ele), u8"name")
                        + lang(u8"の"s, u8" breath"s);
                }
                else
                {
                    valn = lang(""s, u8"breath"s);
                }
                if (is_in_fov(cc))
                {
                    txt(lang(
                        name(cc) + u8"は"s + valn + u8"ブレスを吐いた。"s,
                        name(cc) + u8" bellow"s + _s(cc) + u8" "s + valn
                            + u8" from "s + his(cc) + u8" mouth."s));
                }
                dx = cdata[cc].position.x;
                dy = cdata[cc].position.y;
                breath_list();
                play_animation(3);
                for (int cnt = 0, cnt_end = (maxbreath); cnt < cnt_end; ++cnt)
                {
                    dx = breathlist(0, cnt);
                    dy = breathlist(1, cnt);
                    if (fov_los(
                            cdata[cc].position.x, cdata[cc].position.y, dx, dy)
                        == 0)
                    {
                        continue;
                    }
                    if (dx == cdata[cc].position.x
                        && dy == cdata[cc].position.y)
                    {
                        continue;
                    }
                    if (gdata_mount != 0)
                    {
                        if (gdata_mount == cc)
                        {
                            if (tc == 0)
                            {
                                continue;
                            }
                        }
                    }
                    if (ele == 50)
                    {
                        mapitem_fire(dx, dy);
                    }
                    if (ele == 51)
                    {
                        mapitem_cold(dx, dy);
                    }
                    if (map(dx, dy, 1) != 0)
                    {
                        tc = map(dx, dy, 1) - 1;
                        if (cc != tc)
                        {
                            dmg = roll(dice1, dice2, bonus);
                            if (is_in_fov(tc))
                            {
                                if (tc >= 16)
                                {
                                    gdata(809) = 2;
                                    txt3rd = 1;
                                    txt(lang(
                                        u8"ブレスは"s + name(tc)
                                            + u8"に命中し"s,
                                        u8"The breath hits "s + name(tc)
                                            + u8" and"s));
                                }
                                else
                                {
                                    txt(lang(
                                        u8"ブレスは"s + name(tc)
                                            + u8"に命中した。"s,
                                        u8"The breath hits "s + name(tc)
                                            + u8"."s));
                                }
                            }
                            dmghp(tc, dmg, cc, ele, elep);
                        }
                    }
                }
                goto the_end;
            }
            goto label_2181_internal;
        }
        else
        {
            if (efstatus == curse_state_t::blessed)
            {
                efp = efp * 150 / 100;
            }
            if (is_cursed(efstatus))
            {
                efp = 50;
            }
        }
    }

label_2181_internal:

    switch (efid)
    {
    case 636:
        txtef(8);
        if (jp)
        {
            txt(name(tc) + u8"は"s + name(cc)
                    + u8"の腹の亀裂から蛆虫が沸き出るのを見た。"s,
                name(tc) + u8"は"s + name(cc) + u8"が屍を貪る姿を目撃した。"s,
                name(tc) + u8"は"s + name(cc) + u8"の恐ろしい瞳に震えた。"s,
                name(tc) + u8"は"s + name(cc)
                    + u8"の触手に絡まる臓物に吐き気を感じた。"s);
        }
        if (en)
        {
            txt(name(tc) + u8" see"s + _s(tc)
                    + u8" maggots breed in the rent stomach of "s + name(cc)
                    + u8"."s,
                name(tc) + u8" see"s + _s(tc) + u8" "s + name(cc) + u8" chow"s
                    + _s(cc) + u8" dead bodies."s,
                name(tc) + u8" shudder"s + _s(tc) + u8" at "s + name(cc)
                    + your(cc) + u8" terrifying eyes."s,
                name(tc) + u8" feel"s + _s(tc)
                    + u8" sick at entrails caught in "s + name(cc) + your(cc)
                    + u8" tentacles."s);
        }
        damage_insanity(tc, rnd(roll(dice1, dice2, bonus) + 1));
        break;
    case 1136:
        if (mdata(6) != 1)
        {
            txt(lang(
                u8"それはグローバルマップで読む必要がある。"s,
                u8"You need to read it while you are in the global map."s));
            break;
        }
        if (is_cursed(efstatus))
        {
            if (rnd(5) == 0)
            {
                txt(lang(
                    u8"呪われた地図は触れると崩れ落ちた。"s,
                    u8"The cursed map crumbles as you touch."s));
                --inv[ci].number;
                if (ci >= 5080)
                {
                    cell_refresh(inv[ci].position.x, inv[ci].position.y);
                }
                break;
            }
        }
        if (inv[ci].param1 == 0)
        {
            item_separate(ci);
            for (int cnt = 0; cnt < 1000; ++cnt)
            {
                dx = 4 + rnd((mdata(0) - 8));
                dy = 3 + rnd((mdata(1) - 6));
                if (dx >= 50 && dy >= 39 && dx <= 73 && dy <= 54)
                {
                    continue;
                }
                x(0) = 1;
                x(1) = -1;
                x(2) = 0;
                x(3) = 0;
                y(0) = 0;
                y(1) = 0;
                y(2) = 1;
                y(3) = -1;
                for (int cnt = 0; cnt < 4; ++cnt)
                {
                    f = 1;
                    int cnt2 = cnt;
                    for (int cnt = 0; cnt < 3; ++cnt)
                    {
                        p = map(dx + x(cnt2) * cnt, dy + y(cnt2) * cnt, 0);
                        if ((264 <= p && p < 363) || chipm(7, p) & 4)
                        {
                            f = 0;
                            break;
                        }
                    }
                    if (f == 1)
                    {
                        break;
                    }
                }
                if (f == 1)
                {
                    break;
                }
            }
            inv[ci].param1 = dx;
            inv[ci].param2 = dy;
        }
        txt(lang(
            u8"何かの場所を記した地図のようだ…"s,
            u8"There's a mark on the map..."s));
        snd(59);
        gsel(4);
        pos(0, 0);
        picload(filesystem::dir::graphic() / u8"paper.bmp", 1);
        gsel(0);
        ww = 400;
        wh = 300;
        wx = (windoww - ww) / 2 + inf_screenx;
        wy = winposy(wh);
        gmode(2);
        pos(wx, wy);
        gcopy(4, 0, 0, ww, wh);
        gmode(1, inf_tiles, inf_tiles);
        for (int cnt = 0; cnt < 5; ++cnt)
        {
            y = cnt + inv[ci].param2 - 2;
            sy = cnt * inf_tiles + wy + 26;
            for (int cnt = 0; cnt < 7; ++cnt)
            {
                x = cnt + inv[ci].param1 - 3;
                sx = cnt * inf_tiles + wx + 46;
                p = map(x, y, 0);
                pos(sx + 1, sy + 1);
                gcopy(2, p % 33 * inf_tiles, p / 33 * inf_tiles);
                if (x == inv[ci].param1)
                {
                    if (y == inv[ci].param2)
                    {
                        pos(sx, sy);
                        font(40 - en * 2, snail::font_t::style_t::italic);
                        color(255, 20, 20);
                        mes(lang(u8"○"s, u8"O"s));
                        color(0, 0, 0);
                    }
                }
            }
        }
        gmode(2);
        pos(wx, wy);
        gcopy(4, 400, 0, ww, wh);
        redraw();
        press();
        snd(71);
        break;
    case 1135:
        if (is_cursed(efstatus))
        {
            if (tc == 0)
            {
                eatstatus(efstatus, tc);
            }
            else
            {
                txt(lang(
                    u8"媚薬は呪われていた。"s + name(tc) + u8"は"s + name(0)
                        + u8"を軽蔑のまなざしで見つめた。"s,
                    u8"This love potion is cursed. "s + name(tc) + u8" look"s
                        + _s(tc) + u8" at "s + name(0)
                        + u8" with a contemptuous glance."s));
                chara_mod_impression(tc, -15);
            }
            obvious = 0;
            break;
        }
        cdata[tc].emotion_icon = 317;
        if (potionspill || potionthrow)
        {
            txt(lang(
                name(tc) + u8"は恋の予感がした。"s,
                name(tc) + u8" sense"s + _s(tc) + u8" a sigh of love,"s));
            chara_mod_impression(tc, clamp(efp / 15, 0, 15));
            dmgcon(tc, status_ailment_t::dimmed, 100);
            lovemiracle(tc);
            break;
        }
        if (tc == 0)
        {
            txt(lang(name(tc) + u8"は興奮した！"s, u8"You are excited!"s));
        }
        else
        {
            txt(lang(
                name(tc) + u8"は"s + name(0) + u8"を熱いまなざしで見つめた。"s,
                name(tc) + u8" give"s + _s(tc) + u8" "s + name(0)
                    + u8" the eye."s));
            lovemiracle(tc);
            chara_mod_impression(tc, clamp(efp / 4, 0, 25));
        }
        dmgcon(tc, status_ailment_t::dimmed, 500);
        break;
    case 654:
        if (is_in_fov(tc))
        {
            txt(lang(
                name(cc) + u8"は"s + name(tc)
                    + u8"の口の中に何かを送り込んだ！"s,
                name(cc) + u8" put"s + _s(cc) + u8" something into "s + name(tc)
                    + your(tc) + u8" body!"s));
        }
        get_pregnant();
        break;
    case 626:
        txt(lang(
            u8"あなたは自分の状態を調べた。"s, u8"You examine yourself."s));
        animeload(10, tc);
        label_1964();
        break;
    case 1101:
        if (is_in_fov(tc))
        {
            snd(107);
            if (is_cursed(efstatus))
            {
                if (tc == 0)
                {
                    txt(lang(
                        u8"うわ、これは呪われている。なんだかやばい味だ…"s,
                        u8"Geee it's cursed! The taste is very dangerous."s));
                }
                else
                {
                    txtef(9);
                    txt(lang(
                        u8"「ぺっぺっ、まずー」"s,
                        u8"\"Argh, the milk is cursed!\""s));
                }
            }
            else if (tc == 0)
            {
                txt(lang(
                    u8"濃厚で病み付きになりそうな味だ。"s,
                    u8"The taste is very thick, almost addictive."s));
            }
            else
            {
                txtef(9);
                txt(lang(u8"「うまー」"s, u8"\"Yummy!\""s));
            }
        }
        if (efstatus == curse_state_t::blessed)
        {
            modheight(tc, rnd(5) + 1);
        }
        if (is_cursed(efstatus))
        {
            modheight(tc, (rnd(5) + 1) * -1);
        }
        cdata[tc].nutrition += 1000 * (efp / 100);
        if (tc == 0)
        {
            label_2162();
        }
        eatstatus(efstatus, tc);
        animeload(15, tc);
        break;
    case 1102:
        if (is_in_fov(tc))
        {
            if (is_cursed(efstatus))
            {
                txtef(9);
                txt(lang(u8"「うぃっ…」"s, u8"*Hic*"s),
                    lang(u8"「まずいぜ」"s, u8"\"Ah, bad booze.\""s),
                    lang(u8"「げー♪」"s, u8"\"Ugh...\""s),
                    lang(
                        u8"「腐ったミルクみたいな味だ」"s,
                        u8"\"Bah, smells like rotten milk.\""s));
            }
            else
            {
                txtef(9);
                txt(lang(u8"「うぃっ！」"s, u8"*Hic*"s),
                    lang(u8"「うまいぜ」"s, u8"\"Ah, good booze.\""s),
                    lang(u8"「らららー♪」"s, u8"\"La-la-la-la.\""s),
                    lang(u8"「ひっく」"s, u8"\"I'm going to heaven.\""s),
                    lang(u8"「ふぅ」"s, u8"\"Whew!\""s),
                    lang(u8"「たまらないわ」"s, u8"\"I'm revived!\""s),
                    lang(u8"「んまっ♪」"s, u8"\"Awesome.\""s));
            }
        }
        dmgcon(tc, status_ailment_t::drunk, efp);
        eatstatus(efstatus, tc);
        break;
    case 1116:
        if (is_in_fov(tc))
        {
            if (tc == 0)
            {
                txt(lang(u8"ぐわぁぁ！"s, u8"Arrrrg!"s));
            }
            txt(lang(
                u8"酸が"s + name(tc) + u8"を溶かした。"s,
                u8"The sulfuric acid melts "s + name(tc) + u8"."s));
        }
        if (cdata[tc].is_pregnant())
        {
            cdata[tc].is_pregnant() = false;
            if (is_in_fov(tc))
            {
                txt(lang(
                    name(tc) + u8"の体内のエイリアンは溶けた。"s,
                    name(tc) + your(tc) + u8" alien children melt in "s
                        + his(tc) + u8" stomach."s));
            }
        }
        dmghp(tc, efp * efstatusfix(500, 400, 100, 50) / 1000, -15, 63, efp);
        break;
    case 1103:
        if (is_in_fov(tc))
        {
            if (tc == 0)
            {
                txt(lang(
                    u8"*ごくっ* 綺麗な水だ。"s,
                    u8"*quaff* The water is refreshing."s));
            }
            else
            {
                txt(lang(u8" *ごくっ* "s, u8"*quaff*"s));
            }
        }
        sickifcursed(efstatus, tc, 1);
        break;
    case 1146:
        if (is_in_fov(tc))
        {
            txt(lang(u8"*シュワワ* 刺激的！"s, u8"*quaff* Juicy!"s));
            txt(lang(
                name(tc) + u8"のスタミナが少し回復した。"s,
                name(tc) + u8" restore"s + _s(tc) + u8" some stamina."s));
        }
        healsp(tc, 25);
        sickifcursed(efstatus, tc, 1);
        break;
    case 1147:
        if (is_in_fov(tc))
        {
            txt(lang(u8"*ごくり*"s, u8"*quaff*"s));
            txt(lang(
                name(tc) + u8"のスタミナはかなり回復した。"s,
                name(tc) + u8" greatly restore"s + _s(tc) + u8" stamina."s));
        }
        healsp(tc, 100);
        sickifcursed(efstatus, tc, 1);
        break;
    case 1142:
        if (cdatan(2, tc) == u8"snail"s)
        {
            if (is_in_fov(tc))
            {
                txtef(3);
                txt(lang(
                    u8"塩だ！"s + name(tc) + u8"は溶けはじめた！"s,
                    u8"It's salt! "s + name(tc) + u8" start"s + _s(tc)
                        + u8" to melt."s));
            }
            if (cdata[tc].hp > 10)
            {
                dmghp(tc, cdata[tc].hp - rnd(10), -15);
            }
            else
            {
                dmghp(tc, rnd(20000), -15);
            }
        }
        else if (is_in_fov(tc))
        {
            txtef(9);
            txt(lang(u8"「しょっぱ〜」"s, u8"\"Salty!\""s));
        }
        break;
    case 1130:
        if (is_in_fov(tc))
        {
            if (tc == 0)
            {
                txt(lang(u8"*ごくっ* まずい！"s, u8"*quaff* Yucky!"s));
            }
            else
            {
                txt(lang(u8" *ごくっ* "s, u8"*quaff*"s));
            }
        }
        sickifcursed(efstatus, tc, 1);
        break;
    case 300:
        if (gdata_executing_immediate_quest_type == 1008
            || gdata_executing_immediate_quest_type == 1010)
        {
            txt(lang(
                u8"そんなことをしている余裕はない！"s,
                u8"You have no time for it!"s));
            return 0;
        }
        if (cc == 0)
        {
            if (cdata[0].sp < 50)
            {
                if (cdata[0].sp < rnd(75))
                {
                    txt(lang(
                        u8"疲労し過ぎて失敗した！"s,
                        u8"You are too exhausted!"s));
                    dmgsp(0, the_ability_db[efid]->cost / 2 + 1);
                    break;
                }
            }
            dmgsp(
                0,
                rnd(the_ability_db[efid]->cost / 2 + 1)
                    + the_ability_db[efid]->cost / 2 + 1);
        }
        invsubroutine = 1;
        invctrl(0) = 27;
        invctrl(1) = 0;
        snd(100);
        ctrl_inventory();
        break;
    case 301:
        if (cc == 0)
        {
            if (cdata[0].sp < 50)
            {
                if (cdata[0].sp < rnd(75))
                {
                    txt(lang(
                        u8"疲労し過ぎて失敗した！"s,
                        u8"You are too exhausted!"s));
                    dmgsp(0, the_ability_db[efid]->cost / 2 + 1);
                    break;
                }
            }
            dmgsp(
                0,
                rnd(the_ability_db[efid]->cost / 2 + 1)
                    + the_ability_db[efid]->cost / 2 + 1);
        }
        if (gdata_mount != 0)
        {
            if (tc == cc)
            {
                int stat =
                    cell_findspace(cdata[0].position.x, cdata[0].position.y, 1);
                if (stat == 0)
                {
                    txt(lang(
                        u8"降りるスペースがない。"s,
                        u8"There's no place to get off."s));
                    break;
                }
                cell_setchara(gdata_mount, rtval, rtval(1));
                txt(lang(
                    name(gdata_mount) + u8"から降りた。"s,
                    u8"You dismount from "s + name(gdata_mount) + u8"."s));
                txtef(9);
                if (jp)
                {
                    txt(name(gdata_mount) + u8"「ふぅ」"s,
                        name(gdata_mount) + u8"「乗り心地はよかった？」"s,
                        name(gdata_mount) + u8"「疲れた…」"s,
                        name(gdata_mount) + u8"「またいつでも乗ってね♪」"s);
                }
                if (en)
                {
                    txt(name(gdata_mount) + u8" "s + u8"\"Phew.\""s,
                        name(gdata_mount) + u8" "s + u8"\"How was my ride?\""s,
                        name(gdata_mount) + u8" "s + u8"\"Tired...tired...\""s,
                        name(gdata_mount) + u8" "s + u8"\"It was nice.\""s);
                }
                ride_end();
                break;
            }
        }
        if (tc >= 16)
        {
            txt(lang(
                u8"仲間にしか騎乗できない。"s,
                u8"You can only ride an ally."s));
            break;
        }
        if (cdata[tc].is_escorted() == 1
            || cdata[tc].is_escorted_in_sub_quest() == 1)
        {
            txt(lang(
                u8"護衛対象には騎乗できない。"s,
                u8"You can't ride a client."s));
            break;
        }
        if (tc == cc)
        {
            if (gdata_mount == 0)
            {
                txt(lang(
                    name(cc) + u8"は自分に乗ろうとした。"s,
                    u8"You try to ride yourself."s));
            }
            break;
        }
        if (cdata[tc].current_map != 0)
        {
            txt(lang(
                u8"その仲間はこの場所に滞在中だ。"s,
                u8"The ally currently stays in this area."s));
            break;
        }
        if (gdata_mount != 0)
        {
            txt(lang(
                u8"現在"s + name(cc) + u8"は"s + name(gdata_mount)
                    + u8"に騎乗している。"s,
                name(cc) + u8" "s + is(cc) + u8" currently riding "s
                    + name(gdata_mount) + u8"."s));
        }
        else
        {
            ride_begin(tc);
            txtef(9);
            if (jp)
            {
                txt(name(gdata_mount) + u8"「うぐぅ」"s,
                    name(gdata_mount) + u8"「ダイエットしてよ…」"s,
                    name(gdata_mount) + u8"「いくよ！」"s,
                    name(gdata_mount) + u8"「やさしくしてね♪」"s);
            }
            if (en)
            {
                txt(name(gdata_mount) + u8" "s + u8"\"Awww.\""s,
                    name(gdata_mount) + u8" "s
                        + u8"\"You should go on a diet.\""s,
                    name(gdata_mount) + u8" "s + u8"\"Let's roll!\""s,
                    name(gdata_mount) + u8" "s + u8"\"Be gentle.\""s);
            }
        }
        break;
    case 183:
        if (cc != 0)
        {
            f = 0;
            for (const auto& cnt : items(cc))
            {
                if (inv[cnt].number == 0)
                {
                    continue;
                }
                if (inv[cnt].skill == 183)
                {
                    ci = cnt;
                    f = 1;
                    break;
                }
            }
            if (f == 0)
            {
                return 0;
            }
        }
        if (sdata(183, cc) == 0)
        {
            if (is_in_fov(cc))
            {
                txt(lang(
                    name(cc) + u8"は演奏のやり方を知らない。"s,
                    name(cc) + u8" "s + does((cc == 0))
                        + u8"n't know how to play an instrument."s));
                return 0;
            }
        }
        if (cc == 0)
        {
            if (cdata[0].sp < 50)
            {
                if (cdata[0].sp < rnd(75))
                {
                    txt(lang(
                        u8"疲労し過ぎて失敗した！"s,
                        u8"You are too exhausted!"s));
                    dmgsp(0, the_ability_db[efid]->cost / 2 + 1);
                    break;
                }
            }
            dmgsp(
                0,
                rnd(the_ability_db[efid]->cost / 2 + 1)
                    + the_ability_db[efid]->cost / 2 + 1);
        }
        label_2146();
        break;
    case 184:
        if (sdata(184, 0) == 0)
        {
            txt(lang(
                u8"料理の仕方を知らない。"s, u8"You don't know how to cook."s));
            return 0;
        }
        cooktool = ci;
        invsubroutine = 1;
        invctrl = 16;
        snd(100);
        {
            menu_result result = ctrl_inventory();
            if (!result.succeeded)
            {
                return 0;
            }
        }
        if (cc == 0)
        {
            if (cdata[0].sp < 50)
            {
                if (cdata[0].sp < rnd(75))
                {
                    txt(lang(
                        u8"疲労し過ぎて失敗した！"s,
                        u8"You are too exhausted!"s));
                    dmgsp(0, the_ability_db[efid]->cost / 2 + 1);
                    break;
                }
            }
            dmgsp(
                0,
                rnd(the_ability_db[efid]->cost / 2 + 1)
                    + the_ability_db[efid]->cost / 2 + 1);
        }
        cook();
        break;
    case 185:
        if (sdata(185, 0) == 0)
        {
            txt(lang(
                u8"釣りの仕方を知らない。"s, u8"You don't know how to fish."s));
            return 0;
        }
        if (!inv_getspace(0))
        {
            txt(lang(
                u8"バックパックが一杯だ。"s, u8"Your inventory is full."s));
            return 0;
        }
        if (inv[ci].count == 0)
        {
            txt(lang(
                u8"釣竿には餌が付いていない。"s,
                u8"You need a bait to fish."s));
            return 0;
        }
        f = 0;
        for (int cnt = 0; cnt < 3; ++cnt)
        {
            y = cdata[cc].position.y + cnt - 1;
            x = cdata[cc].position.x;
            if (x < 0 || y < 0 || x >= mdata(0) || y >= mdata(1))
            {
                continue;
            }
            if (chipm(0, map(x, y, 0)) == 3)
            {
                f = 1;
                break;
            }
        }
        if (f == 0)
        {
            for (int cnt = 0; cnt < 3; ++cnt)
            {
                y = cdata[cc].position.y;
                x = cdata[cc].position.x + cnt - 1;
                if (x < 0 || y < 0 || x >= mdata(0) || y >= mdata(1))
                {
                    continue;
                }
                if (chipm(0, map(x, y, 0)) == 3)
                {
                    f = 1;
                    break;
                }
            }
        }
        if (f == 0)
        {
            ++msgdup;
            txt(lang(
                u8"釣りをする場所が見当たらない。"s,
                u8"This isn't a good place to fish."s));
            update_screen();
            return 0;
        }
        if (chipm(0, map(cdata[0].position.x, cdata[0].position.y, 0)) == 3)
        {
            txt(lang(
                u8"水の中からは釣れない。"s,
                u8"You can't fish while swimming."s));
            update_screen();
            return 0;
        }
        if (cdata[0].position.x - x > 0)
        {
            cdata[0].direction = 1;
        }
        if (cdata[0].position.x - x < 0)
        {
            cdata[0].direction = 2;
        }
        if (cdata[0].position.y - y > 0)
        {
            cdata[0].direction = 3;
        }
        if (cdata[0].position.y - y < 0)
        {
            cdata[0].direction = 0;
        }
        gdata(35) = cdata[0].direction;
        fishx = x;
        fishy = y;
        addefmap(fishx, fishy, 1, 3);
        if (cc == 0)
        {
            if (cdata[0].sp < 50)
            {
                if (cdata[0].sp < rnd(75))
                {
                    txt(lang(
                        u8"疲労し過ぎて失敗した！"s,
                        u8"You are too exhausted!"s));
                    dmgsp(0, the_ability_db[efid]->cost / 2 + 1);
                    break;
                }
            }
            dmgsp(
                0,
                rnd(the_ability_db[efid]->cost / 2 + 1)
                    + the_ability_db[efid]->cost / 2 + 1);
        }
        item_separate(ci);
        --inv[ci].count;
        rowactre = 0;
        spot_fishing();
        break;
    case 406:
    case 407:
        if (is_cursed(efstatus))
        {
            if (is_in_fov(tc))
            {
                txt(lang(
                    name(tc) + u8"は悪魔が笑う声を聞いた。"s,
                    name(tc) + u8" hear"s + _s(tc) + u8" devils laugh."s));
            }
            goto label_2183_internal;
        }
        p = 0;
        for (int cnt = 0; cnt < 16; ++cnt)
        {
            i = 16 - cnt - 1;
            if (efid == 406)
            {
                if (p >= 1)
                {
                    break;
                }
            }
            if (the_buff_db[cdata[tc].buffs[i].id]->type
                != buff_data::type_t::hex)
            {
                continue;
            }
            if (cdata[tc].buffs[i].id == 13)
            {
                continue;
            }
            if (cdata[tc].buffs[i].id == 0)
            {
                continue;
            }
            if (rnd(efp * 2 + 1) > rnd(cdata[tc].buffs[i].power + 1))
            {
                buff_delete(tc, i);
                ++p;
                --cnt;
                continue;
            }
        }
        buff_add(tc, 10, efp, 5 + efp / 30);
        animeload(11, tc);
        break;
    case 1120:
        txtef(5);
        txt(lang(
            name(tc) + u8"は黄金の輝きに包まれた！"s,
            u8"A golden aura wraps "s + name(tc) + u8"!"s));
        label_2188();
        play_animation(5);
        break;
    case 1117:
        if (tc >= 16)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        if (!is_cursed(efstatus))
        {
            s = lang(u8"素材"s, u8"materials"s);
        }
        else
        {
            s = lang(u8"クズ"s, u8"junks"s);
        }
        snd(24);
        txt(lang(
            u8"たくさんの"s + s + u8"が降ってきた！"s,
            u8"Some "s + s + u8" fall from above!"s));
        autosave = 1 * (gdata_current_map != 35);
        for (int cnt = 0,
                 cnt_end =
                     (rnd(3) + 3 + (efstatus == curse_state_t::blessed) * 6);
             cnt < cnt_end;
             ++cnt)
        {
            atxspot = 19;
            if (!is_cursed(efstatus))
            {
                p = random_material(efp / 10, efp / 50);
            }
            else
            {
                p = 0;
            }
            matgetmain(p, 1);
        }
        break;
    case 632:
    case 454:
    case 1144:
        if (tc != 0)
        {
            goto label_2184_internal;
        }
        if (efid == 632)
        {
            txt(lang(
                name(cc) + u8"は"s + name(tc)
                    + u8"を気の狂いそうな眼差しで見た。"s,
                name(cc) + u8" cast"s + _s(cc) + u8" an insane glance on "s
                    + name(tc) + u8"."s));
            if (rnd(3))
            {
                break;
            }
        }
        if (tc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            break;
        }
        if (encfind(tc, 33) != -1)
        {
            if (rnd(5))
            {
                txt(lang(
                    u8"あなたは変異を受け付けなかった。"s,
                    u8"You resist the threat of mutation."s));
                break;
            }
        }
    label_2182_internal:
        f = 0;
        p = 1;
        if (efid == 1144)
        {
            p = 2 + rnd(3);
        }
        for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
        {
            for (int cnt = 0; cnt < 100; ++cnt)
            {
                int tid = rnd(45);
                int stat = get_trait_info(0, tid);
                if (stat == 0 || traitref != 1)
                {
                    continue;
                }
                if (rnd(2))
                {
                    p = 1;
                }
                else
                {
                    p = -1;
                }
                if (trait(tid) >= traitref(2))
                {
                    p = -1;
                }
                if (trait(tid) <= traitref(1))
                {
                    p = 1;
                }
                if (is_cursed(efstatus))
                {
                    if (p == 1)
                    {
                        continue;
                    }
                }
                else if (p == -1)
                {
                    if (efstatus == curse_state_t::blessed)
                    {
                        if (rnd(3) == 0)
                        {
                            continue;
                        }
                    }
                    if (efid == 1144)
                    {
                        continue;
                    }
                }
                trait(tid) += p;
                txt(lang(u8"あなたは変容した！ "s, u8"You mutate."s));
                if (p > 0)
                {
                    txtef(2);
                    txt(traitrefn(0));
                }
                else
                {
                    txtef(3);
                    txt(traitrefn(1));
                }
                animeload(8, 0);
                f = 1;
                break;
            }
        }
        if (f == 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
        }
        chara_refresh(0);
        break;
    case 1121:
        if (tc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            break;
        }
        if (is_cursed(efstatus))
        {
            if (is_in_fov(tc))
            {
                txt(lang(u8"これは呪われている！"s, u8"It's cursed!"s));
            }
            goto label_2182_internal;
        }
        f = 0;
        for (int cnt = 0,
                 cnt_end = cnt
                 + (1 + (efstatus == curse_state_t::blessed)
                    + (!is_cursed(efstatus)) + rnd(2));
             cnt < cnt_end;
             ++cnt)
        {
            for (int cnt = 0; cnt < 100; ++cnt)
            {
                int tid = rnd(217);
                int stat = get_trait_info(0, tid);
                if (stat == 0 || traitref != 1)
                {
                    continue;
                }
                if (trait(tid) == 0)
                {
                    continue;
                }
                if (trait(tid) > 0)
                {
                    p = -1;
                }
                if (trait(tid) < 0)
                {
                    p = 1;
                }
                trait(tid) = 0;
                txt(lang(
                    u8"あなたは元の自分に近づいた気がした。"s,
                    u8"You are now one step closer to yourself."s));
                if (p > 0)
                {
                    txtef(2);
                    txt(traitrefn(0));
                }
                else
                {
                    txtef(3);
                    txt(traitrefn(1));
                }
                f = 1;
                break;
            }
        }
        if (f == 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
        }
        chara_refresh(0);
        break;
    case 411:
        if (cc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        invsubroutine = 1;
        invctrl = 13;
        snd(100);
        ctrl_inventory();
        break;
    case 461:
        if (mdata(6) == 1)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        if (is_cursed(efstatus))
        {
            txt(lang(
                u8"冥界から死霊が呼び出された！"s,
                u8"Hoards of undead raise from the hell!"s));
            for (int cnt = 0, cnt_end = (4 + rnd(4)); cnt < cnt_end; ++cnt)
            {
                flt(calcobjlv(cdata[0].level), calcfixlv(3));
                fltn(u8"undead"s);
                chara_create(-1, 0, cdata[0].position.x, cdata[0].position.y);
            }
            obvious = 0;
            break;
        }
        allyctrl = 2;
        {
            int stat = show_hire_menu();
            if (stat == -1)
            {
                txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
                obvious = 0;
                break;
            }
            if (bonus < rnd(100))
            {
                if (is_in_fov(cc))
                {
                    txt(lang(
                        name(cc) + u8"の力は冥界に及ばなかった。"s,
                        name(cc) + your(cc)
                            + u8" prayer doesn't reach the underworld."s));
                }
                break;
            }
            rc = stat;
        }
        label_1538();
        cxinit = cdata[cc].position.x;
        cyinit = cdata[cc].position.y;
        chara_place();
        cdata[rc].current_map = 0;
        txtef(5);
        txt(cnven(cdatan(0, rc))
            + lang(
                  u8"は復活した！"s,
                  (u8" "s + have(rc) + u8" been resurrected!"s)));
        txt(lang(u8"「ありがとう！」"s, u8"\"Thanks!\""s));
        animode = 100 + rc;
        play_animation(19);
        snd(120);
        cdata[rc].emotion_icon = 317;
        if (cc == 0)
        {
            chara_mod_impression(rc, 15);
            if (rc >= 16)
            {
                modify_karma(0, 2);
            }
        }
        break;
    case 412:
        if (efstatus == curse_state_t::none)
        {
            if (is_in_fov(tc))
            {
                txt(lang(
                    name(tc) + u8"の装備品は白い光に包まれた。"s,
                    name(tc) + u8" "s + your(tc)
                        + u8" equipment are surrounded by a white aura."s));
            }
        }
        if (efstatus == curse_state_t::blessed)
        {
            if (is_in_fov(tc))
            {
                txt(lang(
                    name(tc) + u8"は聖なる光に包み込まれた。"s,
                    name(tc) + u8" "s + is(tc)
                        + u8" surrounded by a holy aura."s));
            }
        }
        if (is_cursed(efstatus))
        {
            if (is_in_fov(tc))
            {
                txt(lang(
                    name(tc) + u8"は悪魔が笑う声を聞いた。"s,
                    name(tc) + u8" hear"s + _s(tc) + u8" devils laugh."s));
            }
            goto label_2183_internal;
        }
        p(1) = 0;
        p(2) = 0;
        for (const auto& cnt : items(tc))
        {
            if (inv[cnt].number == 0)
            {
                continue;
            }
            if (!is_cursed(inv[cnt].curse_state))
            {
                continue;
            }
            ci = cnt;
            p = 0;
            if (inv[ci].curse_state == curse_state_t::cursed)
            {
                p = rnd(200) + 1;
            }
            if (inv[ci].curse_state == curse_state_t::doomed)
            {
                p = rnd(1000) + 1;
            }
            if (efstatus == curse_state_t::blessed)
            {
                p = p / 2 + 1;
            }
            else if (inv[ci].body_part == 0)
            {
                continue;
            }
            if (p != 0)
            {
                if (efp >= p)
                {
                    ++p(1);
                    inv[ci].curse_state = curse_state_t::none;
                    item_stack(tc, ci, 1);
                }
                else
                {
                    ++p(2);
                }
            }
        }
        if (efstatus == curse_state_t::blessed)
        {
            if (p(1) != 0)
            {
                if (is_in_fov(tc))
                {
                    txt(lang(
                        u8"幾つかのアイテムが浄化された。"s,
                        u8"The aura uncurses some "s + his(tc) + u8" stuff."s));
                }
            }
        }
        else if (p(1) != 0)
        {
            if (is_in_fov(tc))
            {
                txt(lang(
                    u8"身に付けている装備の幾つかが浄化された。"s,
                    u8"The aura uncurses some of "s + his(tc)
                        + u8" equipment."s));
            }
        }
        if (p(2) != 0)
        {
            if (is_in_fov(tc))
            {
                txt(lang(
                    u8"幾つかのアイテムは抵抗した。"s,
                    u8"Several items resist the aura and remain cursed."s));
            }
        }
        if (p(1) == 0 && p(2) == 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
        }
        else
        {
            animeload(10, tc);
        }
        chara_refresh(tc);
        break;
    case 413:
        if (tc >= 16)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            break;
        }
        if (is_cursed(efstatus))
        {
            artifactlocation.clear();
            txt(lang(
                u8"何かがあなたの耳元でささやいたが、あなたは聞き取ることができなかった。"s,
                u8"You hear a sepulchral whisper but the voice is too small to distinguish a word."s));
            break;
        }
        if (artifactlocation.empty())
        {
            txt(lang(
                u8"まだ特殊なアイテムは生成されていない。"s,
                u8"No artifacts have been generated yet."s));
        }
        else
        {
            range::for_each(
                artifactlocation, [](const auto& line) { txt(cnven(line)); });
            artifactlocation.clear();
        }
        break;
    case 1104:
        if (tc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        f = 0;
        for (int cnt = 0, cnt_end = (1 + (efstatus == curse_state_t::blessed));
             cnt < cnt_end;
             ++cnt)
        {
            int cnt2 = cnt;
            for (int cnt = 0; cnt < 2000; ++cnt)
            {
                await();
                p = rnd(67) + 400;
                if (p == 441)
                {
                    if (rnd(10))
                    {
                        continue;
                    }
                }
                if (!is_cursed(efstatus))
                {
                    if (the_ability_db[p])
                    {
                        if (cnt2 == 0)
                        {
                            s = lang(u8"突然、"s, u8"Suddenly, "s);
                        }
                        else
                        {
                            s = lang(u8"さらに、"s, u8"Furthermore, "s);
                        }
                        skillgain(0, p, 1, 200);
                        txtef(2);
                        txt(lang(
                            s + name(0) + u8"は"s
                                + i18n::_(
                                      u8"ability", std::to_string(p), u8"name")
                                + u8"の魔法の知識を得た。"s,
                            s + u8"you gain knowledge of a spell, "s
                                + i18n::_(
                                      u8"ability", std::to_string(p), u8"name")
                                + u8"."s));
                        snd(24);
                        f = 1;
                        break;
                    }
                }
                else
                {
                    p = p - 400;
                    if (spell(p) > 0)
                    {
                        spell(p) = 0;
                        txt(lang(u8"これは呪われている！"s, u8"It's cursed!"s));
                        txtef(3);
                        txt(lang(
                            u8"突然、"s + name(0) + u8"は"s
                                + i18n::_(
                                      u8"ability",
                                      std::to_string(p + 400),
                                      u8"name")
                                + u8"の魔法の知識を失った。"s,
                            u8"Suddenly, you lose knowledge of a spell, "s
                                + i18n::_(
                                      u8"ability",
                                      std::to_string(p + 400),
                                      u8"name")
                                + u8"."s));
                        snd(117);
                        animeload(14, 0);
                        f = 1;
                        break;
                    }
                }
            }
        }
        if (f == 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        autosave = 1 * (gdata_current_map != 35);
        break;
    case 1143:
        if (efstatus == curse_state_t::blessed)
        {
            cdata[tc].experience = cdata[tc].required_experience;
            r2 = 0;
            gain_level(tc);
            if (is_in_fov(tc))
            {
                snd(60);
            }
        }
        else
        {
            if (cdata[tc].level <= 1)
            {
                txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
                obvious = 0;
                break;
            }
            --cdata[tc].level;
            cdata[tc].experience = 0;
            label_1456(tc);
            txtef(8);
            txt(lang(
                name(tc) + u8"のレベルが下がった…"s,
                name(tc) + u8" lose"s + _s(tc) + u8" a level..."s));
        }
        if (is_cursed(efstatus))
        {
            txt(lang(u8"これは呪われている！"s, u8"It's cursed!"s));
            for (int cnt = 10; cnt < 50; ++cnt)
            {
                if (rnd(3) == 0)
                {
                    if (cnt <= 17)
                    {
                        if (sdata(cnt, tc) != 0)
                        {
                            skillexp(cnt, tc, -1000);
                        }
                    }
                }
            }
            animeload(8, tc);
        }
        chara_refresh(tc);
        break;
    case 1105:
        for (int cnt = 0;; ++cnt)
        {
            await();
            p = rnd(300) + 100;
            if (the_ability_db[p])
            {
                if (!is_cursed(efstatus))
                {
                    if (cnt < efstatusfix(0, 0, 100, 2000))
                    {
                        if (sdata(p, tc) != 0)
                        {
                            continue;
                        }
                    }
                    skillgain(tc, p, 1);
                    if (is_in_fov(tc))
                    {
                        snd(24);
                        txtef(2);
                        txt(lang(
                            ""s + name(tc) + u8"は"s
                                + i18n::_(
                                      u8"ability", std::to_string(p), u8"name")
                                + u8"の技術を獲得した！"s,
                            name(tc) + u8" gain"s + _s(tc) + u8" a skill of "s
                                + i18n::_(
                                      u8"ability", std::to_string(p), u8"name")
                                + u8"!"s));
                    }
                    break;
                }
                else
                {
                    if (sdata(p, tc) == 0)
                    {
                        continue;
                    }
                    if (is_in_fov(tc))
                    {
                        snd(38);
                        txt(lang(u8"これは呪われている！"s, u8"It's cursed!"s));
                    }
                    skillexp(p, tc, -1000);
                    break;
                }
            }
        }
        chara_refresh(tc);
        autosave = 1 * (gdata_current_map != 35);
        break;
    case 1107:
        if (tc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        if (cdata[0].god_id.empty())
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        if (is_cursed(efstatus))
        {
            txt(lang(
                u8"あなたの神はあなたの信仰に疑問を抱いた。"s,
                u8"Your god doubts your faith."s));
            snd(117);
            animeload(14, 0);
            skillexp(181, 0, -1000);
            break;
        }
        if (cdata[0].god_id.empty())
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
        }
        else
        {
            txtef(2);
            txt(lang(
                u8"あなたは"s + i18n::_(u8"god", cdata[0].god_id, u8"name")
                    + u8"の暖かい眼差しを感じた。"s,
                u8"You feel as if "s
                    + i18n::_(u8"god", cdata[0].god_id, u8"name")
                    + u8" is watching you."s));
            if (efstatus == curse_state_t::blessed)
            {
                txt(lang(
                    u8"空から三つ葉のクローバーがふってきた。"s,
                    u8"A three-leaved falls from the sky."s));
            }
            animode = 100 + tc;
            play_animation(19);
            snd(120);
            cdata[0].praying_point += 500;
            modpiety(75);
            skillexp(
                181,
                0,
                1000 + (efstatus == curse_state_t::blessed) * 750,
                6,
                1000);
        }
        chara_refresh(tc);
        break;
    case 1119:
        for (int cnt = 0, cnt_end = (1 + (efstatus == curse_state_t::blessed));
             cnt < cnt_end;
             ++cnt)
        {
            int cnt2 = cnt;
            while (1)
            {
                await();
                p = rnd(300) + 100;
                if (the_ability_db[p])
                {
                    if (sdata.get(p, tc).original_level == 0)
                    {
                        continue;
                    }
                    modify_potential(
                        tc, p, efp * efstatusfix(-4, -2, 5, 5) / 100);
                    if (cnt2 == 0)
                    {
                        s = lang(""s, u8"The "s);
                    }
                    else
                    {
                        s = lang(u8"さらに"s, u8"Furthermore, the "s);
                    }
                    if (!is_cursed(efstatus))
                    {
                        if (is_in_fov(tc))
                        {
                            snd(24);
                            txtef(2);
                            txt(lang(
                                s + ""s + name(tc) + u8"の"s
                                    + i18n::_(
                                          u8"ability",
                                          std::to_string(p),
                                          u8"name")
                                    + u8"の技術の潜在能力が上昇した。"s,
                                s + u8"potential of "s + name(tc) + your(tc)
                                    + u8" "s
                                    + i18n::_(
                                          u8"ability",
                                          std::to_string(p),
                                          u8"name")
                                    + u8" skill increases."s));
                        }
                    }
                    else if (is_in_fov(tc))
                    {
                        snd(117);
                        txtef(3);
                        txt(lang(
                            ""s + name(tc) + u8"の"s
                                + i18n::_(
                                      u8"ability", std::to_string(p), u8"name")
                                + u8"の技術の潜在能力が減少した。"s,
                            u8"The potential of "s + name(tc) + your(tc)
                                + u8" "s
                                + i18n::_(
                                      u8"ability", std::to_string(p), u8"name")
                                + u8" skill decreases."s));
                    }
                    break;
                }
            }
        }
        chara_refresh(tc);
        autosave = 1 * (gdata_current_map != 35);
        break;
    case 1106:
        i = rnd(10) + 10;
        skillexp(i, tc, efstatusfix(-2000, -2000, -1000, -250));
        play_animation(6);
        chara_refresh(tc);
        break;
    case 1139:
        txt(lang(
            name(tc) + u8"の血は沸きあがるように熱くなった。"s,
            name(tc) + your(tc) + u8" blood burns and a new strength fills "s
                + his(tc) + u8" body!"s));
        skillexp(18, tc, efstatusfix(-4000, -1000, 8000, 12000));
        if (efstatus == curse_state_t::blessed)
        {
            modify_potential(tc, 18, 15);
            txtef(2);
            txt(lang(u8"あつつ！"s, u8"It really burns!"s));
        }
        chara_refresh(tc);
        break;
    case 1113:
        if (efstatus == curse_state_t::blessed)
        {
            for (int cnt = 10; cnt < 18; ++cnt)
            {
                modify_potential(
                    tc, cnt, rnd(sdata.get(cnt, tc).potential / 20 + 3) + 1);
            }
            txt(lang(
                name(tc) + u8"の全ての能力の潜在能力が上昇した。"s,
                name(tc) + your(tc)
                    + u8" potential of every attribute expands."s));
            animode = 100 + tc;
            play_animation(19);
            snd(61);
        }
        else
        {
            i = rnd(8) + 10;
            const auto valn = i18n::_(u8"ability", std::to_string(i), u8"name");
            if (efstatus == curse_state_t::none)
            {
                txt(lang(
                    name(tc) + u8"の"s + valn + u8"の潜在能力が上昇した。"s,
                    name(tc) + your(tc) + u8" potential of "s + valn
                        + u8" expands."s));
                modify_potential(
                    tc, i, rnd(sdata.get(i, tc).potential / 10 + 10) + 1);
                snd(24);
            }
            else
            {
                txt(lang(
                    name(tc) + u8"の"s + valn + u8"の潜在能力が減少した。"s,
                    name(tc) + your(tc) + u8" potential of "s + valn
                        + u8" decreases."s));
                modify_potential(
                    tc,
                    i,
                    (rnd(sdata.get(i, tc).potential / 10 + 10) + 1) * -1);
                snd(117);
            }
        }
        autosave = 1 * (gdata_current_map != 35);
        break;
    case 653:
        if (tc < 57)
        {
            break;
        }
        if (cdata[tc].quality >= 4)
        {
            break;
        }
        txt(lang(
            name(tc) + u8"は消え去った。"s,
            name(tc) + u8" vanish"s + _s(tc) + u8"."s));
        chara_vanquish(tc);
        break;
    case 430:
    case 429:
        if (tc >= 16)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        p = 1;
        if (efid == 430)
        {
            p = 2;
        }
        for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
        {
            for (int cnt = 0, cnt_end = (mdata(1)); cnt < cnt_end; ++cnt)
            {
                y = cnt;
                for (int cnt = 0, cnt_end = (mdata(0)); cnt < cnt_end; ++cnt)
                {
                    x = cnt;
                    p = dist(cdata[tc].position.x, cdata[tc].position.y, x, y);
                    if (is_cursed(efstatus))
                    {
                        if (efid == 429)
                        {
                            map(x, y, 2) = tile_default;
                        }
                        if (efid == 430)
                        {
                            map(x, y, 5) = 0;
                        }
                        continue;
                    }
                    if (p < 7 || rnd(efp + 1) > rnd(p * 8 + 1)
                        || efstatus == curse_state_t::blessed)
                    {
                        if (efid == 429)
                        {
                            map(x, y, 2) = map(x, y, 0);
                        }
                        if (efid == 430)
                        {
                            if (map(x, y, 6) != 0 || map(x, y, 5) != 0)
                            {
                                map(x, y, 2) = map(x, y, 0);
                            }
                        }
                    }
                }
            }
        }
        if (is_cursed(efstatus))
        {
            txt(lang(
                u8"あれ…？あなたは軽い記憶障害を受けた。"s,
                u8"Hmm? You suffer minor memory defect."s));
        }
        else
        {
            if (efid == 429)
            {
                txt(lang(
                    name(tc) + u8"は周囲の地形を察知した。"s,
                    name(tc) + u8" sense"s + _s(tc) + u8" nearby locations."s));
            }
            if (efid == 430)
            {
                txt(lang(
                    name(tc) + u8"周囲の物質を感知した。"s,
                    name(tc) + u8" sense"s + _s(tc) + u8" nearby objects."s));
            }
        }
        animeload(10, tc);
        update_minimap();
        update_screen();
        break;
    case 658:
        if (cdata[tc].hp > cdata[tc].max_hp / 8)
        {
            break;
        }
        if (is_in_fov(tc))
        {
            snd(105);
            txtef(3);
            txt(lang(u8" *ブシュッ* "s, u8"*Gash*"s));
            if (tc >= 16)
            {
                gdata(809) = 2;
                txt3rd = 1;
                txt(lang(
                    name(cc) + u8"は"s + name(tc) + u8"の首をちょんぎり"s,
                    name(cc) + u8" cut"s + _s(cc) + u8" "s + name(tc) + your(tc)
                        + u8" head and"s));
            }
            else
            {
                txt(lang(
                    name(cc) + u8"は"s + name(tc) + u8"の首をちょんぎった。"s,
                    name(cc) + u8" cut"s + _s(cc) + u8" "s + name(tc) + your(tc)
                        + u8" head."s));
            }
        }
        dmghp(tc, cdata[tc].max_hp, cc, 658);
        break;
    case 440:
    case 439:
        if (efid == 439)
        {
            if (is_in_fov(tc))
            {
                if (is_cursed(efstatus))
                {
                    snd(117);
                    txt(lang(
                        name(tc) + u8"の肉体は蝕まれた。"s,
                        name(tc) + your(tc) + u8" body is damaged."s));
                }
                else
                {
                    txt(lang(
                        name(tc) + u8"の肉体は復活した。"s,
                        name(tc) + your(tc) + u8" body is restored."s));
                    animeload(10, tc);
                }
                if (efstatus == curse_state_t::blessed)
                {
                    txt(lang(
                        u8"さらに、"s + name(tc) + u8"の肉体は強化された。"s,
                        u8"In addition, "s + his(tc)
                            + u8" body is enchanted."s));
                    animeload(10, tc);
                }
            }
            p(0) = 10;
            p(1) = 11;
            p(2) = 12;
            p(3) = 17;
            p(4) = 18;
            p(5) = -1;
        }
        if (efid == 440)
        {
            if (is_in_fov(tc))
            {
                if (is_cursed(efstatus))
                {
                    snd(117);
                    txt(lang(
                        name(tc) + u8"の精神は蝕まれた。"s,
                        name(tc) + your(tc) + u8" spirit is damaged."s));
                }
                else
                {
                    txt(lang(
                        name(tc) + u8"の精神は復活した。"s,
                        name(tc) + your(tc) + u8" spirit is restored."s));
                    animeload(10, tc);
                }
                if (efstatus == curse_state_t::blessed)
                {
                    txt(lang(
                        u8"さらに、"s + name(tc) + u8"の精神は強化された。"s,
                        u8"In addition, "s + his(tc)
                            + u8" spirit is enchanted."s));
                    animeload(10, tc);
                }
            }
            p(0) = 14;
            p(1) = 13;
            p(2) = 16;
            p(3) = 15;
            p(4) = 19;
            p(5) = -1;
        }
        for (int cnt = 0;; ++cnt)
        {
            if (p(cnt) == -1)
            {
                break;
            }
            const auto attr = p(cnt) - 10;
            if (is_cursed(efstatus))
            {
                if (cdata[tc].quality <= 3)
                {
                    cdata[tc].attr_adjs[attr] -=
                        rnd(sdata.get(p(cnt), tc).original_level) / 5 + rnd(5);
                    continue;
                }
            }
            if (cdata[tc].attr_adjs[attr] < 0)
            {
                cdata[tc].attr_adjs[attr] = 0;
            }
            if (efstatus == curse_state_t::blessed)
            {
                cdata[tc].attr_adjs[attr] =
                    sdata.get(p(cnt), tc).original_level / 10 + 5;
            }
        }
        chara_refresh(tc);
        break;
    case 441:
        what_do_you_wish_for();
        screenupdate = -1;
        label_1416();
        break;
    case 1141:
        if (tc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        if (gdata_is_returning_or_escaping != 0)
        {
            txt(lang(
                u8"脱出を中止した。"s,
                u8"The air around you gradually loses power."s));
            gdata_is_returning_or_escaping = 0;
        }
        else
        {
            int stat = quest_is_return_forbidden();
            if (stat == 1)
            {
                txt(lang(
                    u8"依頼請負中の帰還は法律で禁止されている。それでも帰還する？"s,
                    u8"Returning while taking a quest if forbidden. Are you sure you want to return?"s));
                ELONA_YES_NO_PROMPT();
                rtval = show_prompt(promptx, prompty, 160);
                if (rtval != 0)
                {
                    break;
                }
            }
            txt(lang(
                u8"周囲の大気がざわめきだした。"s,
                u8"The air around you becomes charged."s));
            if (adata(16, gdata_current_map) == 8)
            {
                if (gdata_current_dungeon_level == adata(10, gdata_current_map))
                {
                    if (adata(20, gdata_current_map) != -1)
                    {
                        txt(lang(
                            u8"このままダンジョンを出ると、この階のクエストは達成できない…"s,
                            u8"The lord of the dungeon might disappear if you escape now."s));
                    }
                }
            }
            gdata_destination_map = gdata(850);
            gdata_destination_dungeon_level = 1;
            if (is_cursed(efstatus))
            {
                if (rnd(3) == 0)
                {
                    gdata_destination_map = 41;
                    gdata_destination_dungeon_level = 1;
                }
            }
            gdata_is_returning_or_escaping = 5 + rnd(10);
        }
        break;
    case 428:
        if (tc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        if (gdata_is_returning_or_escaping != 0)
        {
            txt(lang(
                u8"帰還を中止した。"s,
                u8"The air around you gradually loses power."s));
            gdata_is_returning_or_escaping = 0;
        }
        else
        {
            label_2081();
            if (is_cursed(efstatus))
            {
                if (rnd(3) == 0)
                {
                    gdata_destination_map = 41;
                    gdata_destination_dungeon_level = 1;
                }
            }
        }
        break;
    case 621:
        healmp(tc, efp / 2 + rnd((efp / 2 + 1)));
        if (is_in_fov(tc))
        {
            txt(lang(
                name(tc) + u8"のマナが回復した。"s,
                name(tc) + your(tc) + u8" mana is restored."s));
            play_animation(5);
        }
        break;
    case 624:
        healmp(tc, roll(dice1, dice2, bonus));
        if (is_in_fov(tc))
        {
            txt(lang(
                name(tc) + u8"は周囲からマナを吸い取った。"s,
                name(tc) + u8" absorb"s + _s(tc) + u8" mana from the air."s));
            play_animation(5);
        }
        break;
    case 1108:
        if (is_in_fov(tc))
        {
            txt(lang(
                name(tc) + u8"は毒を浴びた！"s,
                name(tc) + u8" "s + is(tc) + u8" hit by poison!"s));
        }
        if (cdata[tc].is_pregnant())
        {
            cdata[tc].is_pregnant() = false;
            if (is_in_fov(tc))
            {
                txt(lang(
                    name(tc) + u8"の体内のエイリアンは溶けた。"s,
                    name(tc) + your(tc) + u8" alien children melt in "s
                        + his(tc) + u8" stomach."s));
            }
        }
        dmgcon(tc, status_ailment_t::poisoned, efp);
        break;
    case 1111:
        if (is_in_fov(tc))
        {
            txt(lang(
                name(tc) + u8"は墨を浴びた！"s,
                u8"Ink squirts into "s + name(tc) + your(tc) + u8" face!"s));
        }
        dmgcon(tc, status_ailment_t::blinded, efp);
        break;
    case 1109:
        if (is_in_fov(tc))
        {
            txt(lang(
                name(tc) + u8"はひどい頭痛におそわれた！"s,
                u8"A foul stench floods "s + name(tc) + your(tc)
                    + u8" nostrils!"s));
        }
        dmgcon(tc, status_ailment_t::confused, efp);
        break;
    case 1110:
        if (is_in_fov(tc))
        {
            txt(lang(
                name(tc) + u8"は痺れた！"s,
                name(tc) + u8" get"s + _s(tc) + u8" numbness!"s));
        }
        dmgcon(tc, status_ailment_t::paralyzed, efp);
        break;
    case 1112:
        if (is_in_fov(tc))
        {
            txt(lang(
                name(tc) + u8"は甘い液体を浴びた！"s,
                u8"Strange sweet liquid splashes onto "s + name(tc) + u8"!"s));
        }
        dmgcon(tc, status_ailment_t::sleep, efp);
        break;
    case 645:
    case 1114:
    label_2183_internal:
        if (efid == 645)
        {
            if (is_in_fov(tc))
            {
                txt(lang(
                    name(cc) + u8"は"s + name(tc)
                        + u8"を指差して呪いの言葉を呟いた。"s,
                    name(cc) + u8" point"s + _s(cc) + u8" "s + name(tc)
                        + u8" and mutter"s + _s(cc) + u8" a curse."s));
            }
        }
        p = 75 + sdata(19, tc);
        if (encfind(tc, 43) != -1)
        {
            p += encfind(tc, 43) / 2;
        }
        if (rnd(p) > efp / 2 + (is_cursed(efstatus)) * 100)
        {
            break;
        }
        if (tc < 16)
        {
            if (rnd(3))
            {
                if (trait(42))
                {
                    txt(lang(
                        u8"あなたは祈祷を捧げ呪いのつぶやきを無効にした。"s,
                        u8"Your prayer nullifies the curse."s));
                    break;
                }
            }
        }
        i = 0;
        for (int cnt = 100; cnt < 130; ++cnt)
        {
            if (cdata_body_part(tc, cnt) % 10000 == 0)
            {
                continue;
            }
            p(i) = cdata_body_part(tc, cnt) % 10000 - 1;
            if (inv[p(i)].curse_state == curse_state_t::blessed)
            {
                if (rnd(10))
                {
                    continue;
                }
            }
            ++i;
        }
        if (i == 0)
        {
            for (int cnt = 0; cnt < 200; ++cnt)
            {
                p = get_random_inv(tc);
                if (inv[p].number == 0)
                {
                    continue;
                }
                if (inv[p].curse_state == curse_state_t::blessed)
                {
                    if (rnd(10))
                    {
                        continue;
                    }
                }
                i = 1;
                break;
            }
        }
        if (i > 0)
        {
            i = p(rnd(i));
            const auto valn = itemname(i, 1, 1);
            if (inv[i].curse_state == curse_state_t::cursed)
            {
                inv[i].curse_state = curse_state_t::doomed;
            }
            else
            {
                inv[i].curse_state = curse_state_t::cursed;
            }
            if (is_in_fov(tc))
            {
                txt(lang(
                    name(tc) + u8"の"s + valn + u8"は黒く輝いた。"s,
                    name(tc) + your(tc) + u8" "s + valn + u8" glows black."s));
            }
            chara_refresh(tc);
            snd(117);
            animeload(14, tc);
            item_stack(tc, i, 1);
        }
        else
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
        }
        break;
    case 1118:
        f = 0;
        for (int cnt = 0; cnt < 10; ++cnt)
        {
            p = rnd(11) + 50;
            if (sdata.get(p, tc).original_level >= 150)
            {
                ++f;
                resistmod(tc, p, 50 * -1);
                if (f >= efp / 100)
                {
                    break;
                }
            }
        }
        if (f == 0)
        {
            txt(lang(u8"何も起こらなかったようだ。"s, u8"Nothing happens."s));
            obvious = 0;
        }
        else
        {
            snd(38);
        }
        chara_refresh(tc);
        break;
    case 1138:
    case 1123:
    case 1122:
    case 1137:
        if (cc != 0 && cc < 16)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            break;
        }
        flt(cdata[0].level / 2 + 5, 3);
        p = 0;
        if (rnd(3) == 0)
        {
            fltn(u8"man"s);
        }
        if (efid == 1138)
        {
            txt(lang(
                u8"なんと、あなたには生き別れた血の繋がっていないぬこの妹がいた！"s,
                u8"How...! You suddenly get a younger cat sister!"s));
            p = 210;
        }
        if (efid == 1123)
        {
            txt(lang(
                u8"なんと、あなたには生き別れた血の繋がっていない妹がいた！"s,
                u8"How...! You suddenly get a younger sister!"s));
            p = 176;
        }
        if (efid == 1137)
        {
            txt(lang(
                u8"お嬢さんが空から降ってきた！"s,
                u8"A young lady falls from the sky."s));
            p = 211;
        }
        novoidlv = 1;
        chara_create(56, p, -3, 0);
        rc = 56;
        new_ally_joins();
        break;
    case 435:
        if (cc != 0 || tc == 0 || cdata[tc].relationship == 10)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        if (gdata_current_map == 6 || gdata_current_map == 40
            || gdata_current_map == 42)
        {
            obvious = 0;
            txt(lang(
                u8"この場所では効果がない。"s,
                u8"The effect doesn't work in this area."s));
            break;
        }
        f = 1;
        {
            int stat = inv_find(663, cc);
            if (stat != -1)
            {
                efp = efp * 3 / 2;
            }
        }
        if (rnd(efp / 15 + 5) < cdata[tc].level)
        {
            f = 0;
        }
        if (cdata[tc].quality >= 4 || cdata[tc].character_role != 0
            || cdata[tc].is_lord_of_dungeon() == 1)
        {
            f = -1;
        }
        if (f == 1)
        {
            rc = tc;
            new_ally_joins();
            quest_check();
        }
        else if (f == 0)
        {
            txt(lang(
                name(tc) + u8"は抵抗した。"s,
                name(tc) + u8" resist"s + _s(tc) + u8"."s));
        }
        else
        {
            txt(lang(
                name(tc) + u8"は支配できない。"s,
                name(tc) + u8" cannot be charmed."s));
        }
        break;
    case 436:
    case 437:
    case 455:
    case 634:
    case 456:
        if (efid == 436)
        {
            p(0) = 3;
            p(1) = 2 + rnd((efp / 50 + 1));
            txt(lang(
                u8"蜘蛛の巣が辺りを覆った。"s,
                u8"The ground is covered with thick webbing."s));
        }
        if (efid == 437)
        {
            txt(lang(
                u8"辺りを濃い霧が覆った。"s,
                u8"The air is wrapped in a dense fog."s));
            p(0) = 3;
            p(1) = 2 + rnd((efp / 50 + 1));
        }
        if (efid == 455)
        {
            txt(lang(
                u8"酸の水溜りが発生した。"s, u8"Acid puddles are generated."s));
            p(0) = 2;
            p(1) = 2 + rnd((efp / 50 + 1));
        }
        if (efid == 456)
        {
            txt(lang(
                u8"火柱が発生した。"s,
                u8"Walls of fire come out from the ground."s));
            p(0) = 2;
            p(1) = 2 + rnd((efp / 50 + 1));
        }
        if (efid == 634)
        {
            txt(lang(u8"エーテルの霧が発生した。"s, u8"Ether mist spreads."s));
            p(0) = 2;
            p(1) = 1 + rnd((efp / 100 + 2));
        }
        snd(68);
        for (int cnt = 0, cnt_end = (p(1)); cnt < cnt_end; ++cnt)
        {
            x = rnd(p) + tlocx - rnd(p);
            y = rnd(p) + tlocy - rnd(p);
            f = 1;
            if (x < 0 || y < 0 || x >= mdata(0) || y >= mdata(1))
            {
                f = 0;
            }
            else if (chipm(7, map(x, y, 0)) & 4)
            {
                f = 0;
            }
            if (dist(tlocx, tlocy, x, y) >= p)
            {
                f = 0;
            }
            if (f == 0)
            {
                if (rnd(2) == 0)
                {
                    continue;
                }
                else
                {
                    --cnt;
                    continue;
                }
            }
            if (efid == 634)
            {
                mef_add(x, y, 4, 20, rnd(4) + 2, efp, cc);
            }
            if (efid == 455)
            {
                mef_add(x, y, 3, 19, rnd(10) + 5, efp, cc);
            }
            if (efid == 456)
            {
                mef_add(x, y, 5, 24, rnd(10) + 5, efp, cc);
                mapitem_fire(x, y);
            }
            if (efid == 436)
            {
                mef_add(x, y, 1, 11, -1, efp * 2);
            }
            if (efid == 437)
            {
                mef_add(x, y, 2, 30, 8 + rnd((15 + efp / 25)), efp);
            }
        }
        break;
    case 1145:
        if (cc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        invsubroutine = 1;
        invctrl(0) = 23;
        invctrl(1) = 0;
        snd(100);
        ctrl_inventory();
        if (inv[ci].quality < 4 || inv[ci].quality == 6)
        {
            txt(lang(u8"それは無理だ。"s, u8"It's impossible."s));
            obvious = 0;
            break;
        }
        txt(lang(
            u8"アーティファクトの新しい銘は？"s,
            u8"What do you want to name this artifact?"s));
        {
            int stat = select_alias(3);
            if (stat == 0)
            {
                obvious = 0;
                randomize();
                break;
            }
            p = stat;
        }
        inv[ci].subname = list(1, p) + 40000;
        randomize();
        txt(lang(
            u8"それは"s + listn(0, p) + u8"という銘になった。"s,
            u8"It's now called "s + listn(0, p) + u8"."s));
        break;
    case 49:
        if (cc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        invsubroutine = 1;
        invctrl(0) = 23;
        invctrl(1) = 7;
        snd(100);
        {
            menu_result result = ctrl_inventory();
            f = result.succeeded ? 1 : 0;
        }
        if (inv[ci].quality >= 4 || ibit(10, ci) == 1)
        {
            txt(lang(
                u8"そのアイテムに改良の余地はない。"s,
                u8"This item leaves no room for improvement."s));
            fixmaterial = 0;
            objfix = 0;
            break;
        }
        randomize(inv[efcibk].param1);
        equip = inv[ci].body_part;
        animeload(8, cc);
        inv[ci].quality = 4;
        fixmaterial = inv[ci].material;
        change_item_material();
        randomize(inv[efcibk].param1);
        inv[ci].subname = 40000 + rnd(30000);
        p = rnd(rnd(rnd(10) + 1) + 3) + 3;
        egolv = rnd(clamp(rnd(6), 0, 4) + 1);
        for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
        {
            randomize(inv[efcibk].param1);
            enchantment_add(
                ci,
                enchantment_generate(enchantment_gen_level(egolv)),
                enchantment_gen_p() + (fixlv == 5) * 100 + (ibit(15, ci) == 1) * 100,
                20 - (fixlv == 5) * 10 - (ibit(15, ci) == 1) * 20);
        }
        randomize();
        txt(lang(
            u8"それは"s + itemname(ci, 1) + u8"になった。"s,
            u8"It becomes "s + itemname(ci, 1) + u8"."s));
        if (equip != 0)
        {
            cdata_body_part(cc, equip) =
                cdata_body_part(cc, equip) / 10000 * 10000 + ci + 1;
            inv[ci].body_part = equip;
        }
        chara_refresh(cc);
        fixmaterial = 0;
        objfix = 0;
        ci = efcibk;
        --inv[ci].number;
        cell_refresh(inv[ci].position.x, inv[ci].position.y);
        autosave = 1 * (gdata_current_map != 35);
        break;
    case 21:
    case 1127:
        if (cc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        invsubroutine = 1;
        invctrl(0) = 23;
        invctrl(1) = 0;
        snd(100);
        {
            menu_result result = ctrl_inventory();
            f = result.succeeded ? 1 : 0;
        }
        if (inv[ci].quality == 5 || ibit(10, ci) == 1)
        {
            if (efid == 1127)
            {
                f = 0;
            }
        }
        equip = inv[ci].body_part;
        if (f == 1)
        {
            if (inv[ci].quality == 6)
            {
                if (efp < 350)
                {
                    txt(lang(
                        u8"アーティファクトの再生成にはパワーが足りない。"s,
                        u8"More magic power is needed to reconstruct an artifact."s));
                    break;
                }
                animeload(8, cc);
                txt(lang(
                    name(cc) + u8"の"s + itemname(ci, 1)
                        + u8"は再生成された。"s,
                    name(cc) + your(cc) + u8" "s + itemname(ci, 1, 1)
                        + u8" is reconstructed."s));
                --inv[ci].number;
                cell_refresh(inv[ci].position.x, inv[ci].position.y);
                flt();
                itemcreate(0, inv[ci].id, -1, -1, 0);
            }
            else
            {
                animeload(8, cc);
                if (efp <= 50)
                {
                    if (rnd(3) == 0)
                    {
                        fixmaterial = 35;
                    }
                }
                s = itemname(ci, 1, 1);
                objlv = efp / 10;
                objfix = efp / 100;
                randomize();
                change_item_material();
                txt(lang(
                    name(cc) + u8"の"s + s + u8"は"s + itemname(ci, 1)
                        + u8"に変化した。"s,
                    name(cc) + your(cc) + u8" "s + s + u8" transforms into "s
                        + itemname(ci, 1) + u8"."s));
            }
        }
        else
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
        }
        if (equip != 0)
        {
            cdata_body_part(cc, equip) =
                cdata_body_part(cc, equip) / 10000 * 10000 + ci + 1;
            inv[ci].body_part = equip;
        }
        chara_refresh(cc);
        fixmaterial = 0;
        objfix = 0;
        break;
    case 1128:
        if (cc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        snd(24);
        p = rnd((efp + 1)) / 100 + 1;
        gdata_rights_to_succeed_to += p;
        txtef(5);
        txt(lang(
            u8"あなたは遺産相続人として認められた(+"s + p + u8")"s,
            u8"You claim the right of succession. (+"s + p + u8")"s));
        txt(lang(
            u8"今やあなたは"s + gdata_rights_to_succeed_to
                + u8"個の遺産を相続できる。"s,
            u8"You can now inherit "s + gdata_rights_to_succeed_to
                + u8" items."s));
        break;
    case 1124:
    case 1125:
        if (cc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            break;
        }
        invsubroutine = 1;
        if (efid == 1124)
        {
            invctrl(0) = 23;
            invctrl(1) = 1;
        }
        else
        {
            invctrl(0) = 23;
            invctrl(1) = 2;
        }
        snd(100);
        {
            menu_result result = ctrl_inventory();
            if (result.succeeded)
            {
                if (inv[ci].enhancement < efp / 100)
                {
                    snd(24);
                    txt(lang(
                        itemname(ci) + u8"は黄金の光に包まれた。"s,
                        itemname(ci) + u8" "s + is2(inv[ci].number)
                            + u8" surrounded by a golden aura."s));
                    ++inv[ci].enhancement;
                }
                else
                {
                    txt(lang(
                        itemname(ci) + u8"は抵抗した。"s,
                        itemname(ci) + u8" resist"s + _s2(inv[ci].number)
                            + u8"."s));
                }
                chara_refresh(cc);
            }
            else
            {
                obvious = 0;
            }
        }
        break;
    case 630:
    case 1129:
        if (cc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        if (efid == 630)
        {
            if (gdata_charge_power < 10)
            {
                txt(lang(
                    u8"充填するには最低でも魔力の貯蓄が10必要だ。"s,
                    u8"You need at least 10 recharge powers to recharge items."s));
                break;
            }
            gdata_charge_power -= 10;
            txt(lang(
                u8"魔力の貯蓄を10消費した(残り"s + gdata_charge_power + u8")"s,
                u8"You spend 10 recharge powers. (Total:"s + gdata_charge_power
                    + u8")"s));
        }
        invsubroutine = 1;
        invctrl(0) = 23;
        invctrl(1) = 3;
        snd(100);
        {
            menu_result result = ctrl_inventory();
            if (result.succeeded)
            {
                dbid = inv[ci].id;
                access_item_db(2);
                if (ichargelevel < 1 || inv[ci].id == 290 || inv[ci].id == 480
                    || inv[ci].id == 289 || inv[ci].id == 732
                    || (inv[ci].id == 687 && inv[ci].param2 != 0))
                {
                    txt(lang(
                        u8"それは充填ができないようだ。"s,
                        u8"You can't recharge this item."s));
                    break;
                }
                f = 1;
                if (inv[ci].count > ichargelevel)
                {
                    f = -1;
                }
                if (f == -1)
                {
                    txt(lang(
                        itemname(ci) + u8"はこれ以上充填できないようだ。"s,
                        itemname(ci) + u8" cannot be recharged anymore."s));
                    break;
                }
                if (rnd(efp / 25 + 1) == 0)
                {
                    f = 0;
                }
                if (the_item_db[inv[ci].id]->category == 54000)
                {
                    if (rnd(4) == 0)
                    {
                        f = 0;
                    }
                }
                if (rnd(ichargelevel * ichargelevel + 1) == 0)
                {
                    f = 0;
                }
                if (f == 1)
                {
                    p = 1 + rnd((ichargelevel / 2 + 1));
                    if (p + inv[ci].count > ichargelevel)
                    {
                        p = ichargelevel - inv[ci].count + 1;
                    }
                    if (the_item_db[inv[ci].id]->category == 54000)
                    {
                        p = 1;
                    }
                    txt(lang(
                        itemname(ci) + u8"は充填された(+"s + p + u8")"s,
                        itemname(ci) + u8" "s + is2(inv[ci].number)
                            + u8" recharged by +"s + p + u8"."s));
                    inv[ci].count += p;
                    animeload(8, cc);
                }
                else
                {
                    if (rnd(4) == 0)
                    {
                        txt(lang(
                            itemname(ci) + u8"は破裂した。"s,
                            itemname(ci) + u8" explode"s + _s2(inv[ci].number)
                                + u8"."s));
                        --inv[ci].number;
                        refresh_burden_state();
                        break;
                    }
                    txt(lang(
                        itemname(ci) + u8"への充填は失敗した。"s,
                        u8"You fail to recharge "s + itemname(ci) + u8"."s));
                }
            }
            else
            {
                obvious = 0;
            }
        }
        break;
    case 629:
        if (cc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        invsubroutine = 1;
        invctrl(0) = 23;
        invctrl(1) = 5;
        snd(100);
        {
            menu_result result = ctrl_inventory();
            if (result.succeeded)
            {
                dbid = inv[ci].id;
                access_item_db(2);
                for (int cnt = 0; cnt < 1; ++cnt)
                {
                    if (ichargelevel == 1)
                    {
                        p = 100;
                        break;
                    }
                    if (ichargelevel == 2)
                    {
                        p = 25;
                        break;
                    }
                    if (ichargelevel <= 4)
                    {
                        p = 5;
                        break;
                    }
                    if (ichargelevel <= 6)
                    {
                        p = 3;
                        break;
                    }
                    p = 1;
                }
                animeload(8, cc);
                p = p * inv[ci].count;
                gdata_charge_power += p;
                txt(lang(
                    itemname(ci) + u8"を破壊して"s + p
                        + u8"の魔力を抽出した(計"s + gdata_charge_power
                        + u8")"s,
                    u8"You destroy "s + itemname(ci) + u8" and extract "s + p
                        + u8" recharge powers. (Total:"s + gdata_charge_power
                        + u8")"s));
                inv[ci].number = 0;
                refresh_burden_state();
            }
        }
        break;
    case 628:
    label_2184_internal:
        if (tc == 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        f = 1;
        if (efp / 10 + 10 < cdata[tc].level)
        {
            f = 0;
        }
        if (cdata[tc].quality >= 4 || cdata[tc].character_role != 0
            || cdata[tc].is_escorted() == 1
            || cdata[tc].is_lord_of_dungeon() == 1)
        {
            f = -1;
        }
        if (tc < 57)
        {
            f = 0;
        }
        if (f == 1)
        {
            animeload(8, tc);
            txt(lang(
                name(tc) + u8"は変化した。"s,
                name(tc) + u8" change"s + _s(tc) + u8"."s));
            flt(calcobjlv(cdata[tc].level + 3), 2);
            chara_create(56, 0, -3, 0);
            chara_relocate(56, tc, 1);
            cdata[tc].enemy_id = cc;
            cdata[tc].is_quest_target() = false;
            quest_check();
        }
        else if (f == 0)
        {
            txt(lang(
                name(tc) + u8"は抵抗した。"s,
                name(tc) + u8" resist"s + _s(tc) + u8"."s));
        }
        else
        {
            txt(lang(
                name(tc) + u8"は変化できない。"s,
                name(tc) + u8" cannot be changed."s));
        }
        break;
    case 1140:
        if (cc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        invsubroutine = 1;
        invctrl(0) = 23;
        invctrl(1) = 6;
        snd(100);
        {
            menu_result result = ctrl_inventory();
            if (result.succeeded)
            {
                autosave = 1 * (gdata_current_map != 35);
                animeload(8, cc);
                if (!is_cursed(efstatus))
                {
                    if (inv[ci].weight > 0)
                    {
                        inv[ci].weight = clamp(
                            inv[ci].weight * (100 - efp / 10) / 100,
                            1,
                            inv[ci].weight);
                        if (inv[ci].pv > 0)
                        {
                            inv[ci].pv -= inv[ci].pv / 10 + 1
                                + (efstatus != curse_state_t::blessed);
                        }
                        if (inv[ci].damage_bonus > 0)
                        {
                            inv[ci].damage_bonus -= inv[ci].damage_bonus / 10
                                + 1 + (efstatus != curse_state_t::blessed);
                        }
                    }
                    txt(lang(
                        ""s + itemname(ci, 1)
                            + u8"は羽が生えたように軽くなった。"s,
                        itemname(ci, 1) + u8" becomes light as a feather."s));
                }
                else
                {
                    inv[ci].weight = inv[ci].weight * 150 / 100 + 1000;
                    if (inv[ci].pv > 0)
                    {
                        inv[ci].pv += clamp(inv[ci].pv / 10, 1, 5);
                    }
                    if (inv[ci].damage_bonus > 0)
                    {
                        inv[ci].damage_bonus +=
                            clamp(inv[ci].damage_bonus / 10, 1, 5);
                    }
                    txt(lang(
                        ""s + itemname(ci, 1) + u8"はずしりと重くなった。"s,
                        itemname(ci, 1) + u8" becomes heavy."s));
                }
                refresh_burden_state();
            }
            else
            {
                txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
                obvious = 0;
            }
        }
        chara_refresh(cc);
        break;
    case 1132:
        if (cc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
            break;
        }
        invsubroutine = 1;
        invctrl(0) = 23;
        invctrl(1) = 4;
        snd(100);
        {
            menu_result result = ctrl_inventory();
            f = result.succeeded ? 1 : 0;
        }
        if (f)
        {
            if (inv[ci].quality > 4 || ibit(5, ci) == 1)
            {
                f = 0;
            }
        }
        if (f == 1)
        {
            autosave = 1 * (gdata_current_map != 35);
            animeload(8, cc);
            fltbk = the_item_db[inv[ci].id]->category;
            valuebk = calcitemvalue(ci, 0);
            inv[ci].number = 0;
            for (int cnt = 0;; ++cnt)
            {
                await();
                flt(calcobjlv(efp / 10) + 5, calcfixlv(3));
                if (cnt < 10)
                {
                    flttypemajor = fltbk;
                }
                int stat = itemcreate(0, 0, -1, -1, 0);
                if (stat == 0)
                {
                    continue;
                }
                if (inv[ci].value > valuebk * 3 / 2 + 1000)
                {
                    inv[ci].number = 0;
                    continue;
                }
                else
                {
                    break;
                }
            }
            txt(lang(
                u8"それは"s + itemname(ci, 1) + u8"に変容した。"s,
                u8"It is metamorphosed into "s + itemname(ci, 1) + u8"."s));
            refresh_burden_state();
        }
        else
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            obvious = 0;
        }
        break;
    case 457:
    case 438:
        x = tlocx;
        y = tlocy;
        f = 1;
        if (x < 0 || y < 0 || x >= mdata(0) || y >= mdata(1))
        {
            f = 0;
        }
        else
        {
            if (chipm(7, map(x, y, 0)) & 4)
            {
                if (homemapmode == 0)
                {
                    if (efid != 457)
                    {
                        f = 0;
                    }
                    else if (chipm(0, map(x, y, 0)) == 3)
                    {
                        f = 0;
                    }
                }
            }
            if (map(x, y, 1) != 0 || map(x, y, 6) != 0)
            {
                f = 0;
            }
        }
        if (f == 1)
        {
            if (efid == 438)
            {
                if (homemapmode == 0)
                {
                    txt(lang(
                        u8"床が盛り上がってきた。"s, u8"A wall appears."s));
                    p = tile_wall;
                }
                else
                {
                    p = tile;
                }
                if (map(x, y, 0) != p)
                {
                    snd(65);
                }
                map(x, y, 0) = p;
                map(x, y, 2) = p;
            }
            if (efid == 457)
            {
                snd(65);
                if (chipm(0, map(x, y, 0)) == 6)
                {
                    txt(lang(
                        u8"この壁は魔法を受け付けないようだ。"s,
                        u8"These walls seem to resist your magic."s));
                    break;
                }
                txt(lang(u8"扉が出現した。"s, u8"A door appears."s));
                cell_featset(x, y, tile_doorclosed, 21, rnd(efp / 10 + 1));
                if (chipm(7, map(x, y, 0)) & 4)
                {
                    map(x, y, 0) = tile_tunnel;
                }
            }
        }
        else
        {
            if (homemapmode == 0)
            {
                txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            }
            obvious = 0;
        }
        break;
    case 631:
        txtef(4);
        txt(lang(u8"スウォーム！"s, u8"Swarm!"s));
        for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
        {
            if (cdata[cc].state != 1)
            {
                continue;
            }
            if (cdata[cnt].state != 1)
            {
                continue;
            }
            if (cc == cnt)
            {
                continue;
            }
            if (cnt <= 16)
            {
                continue;
            }
            tc = cnt;
            dx = cdata[tc].position.x;
            dy = cdata[tc].position.y;
            if (dist(cdata[cc].position.x, cdata[cc].position.y, dx, dy)
                > the_ability_db[631]->sdataref3 % 1000 + 1)
            {
                continue;
            }
            if (fov_los(cdata[cc].position.x, cdata[cc].position.y, dx, dy)
                == 0)
            {
                continue;
            }
            play_animation(9);
            try_to_melee_attack();
        }
        break;
    case 659:
        if (mdata(6) == 1)
        {
            break;
        }
        if (map(cdata[cc].position.x, cdata[cc].position.y, 6) != 0)
        {
            break;
        }
        cell_featset(cdata[cc].position.x, cdata[cc].position.y, 0, 14, 7, cc);
        if (is_in_fov(cc))
        {
            txt(lang(
                name(cc) + u8"は何かを投下した。"s,
                name(cc) + u8" drop"s + _s(cc)
                    + u8" something on the ground."s));
        }
        break;
    case 466:
        for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
        {
            if (cdata[cnt].state != 1)
            {
                continue;
            }
            if (cc == cnt)
            {
                continue;
            }
            if (cdata[cnt].is_immune_to_mine())
            {
                continue;
            }
            tc = cnt;
            dx = cdata[tc].position.x;
            dy = cdata[tc].position.y;
            if (dist(cdata[cc].position.x, cdata[cc].position.y, dx, dy) > 4)
            {
                continue;
            }
            if (is_in_fov(tc))
            {
                txt(lang(
                    name(tc) + u8"は重力を感じた。"s,
                    name(tc) + u8" feel"s + _s(tc) + u8" gravity."s));
            }
            cdata[tc].gravity += 100 + rnd(100);
        }
        break;
    case 657:
        txtef(4);
        txt(lang(u8"うみみゃぁ！"s, u8"Mewmewmew!"s));
        animode = 0;
        play_animation(19);
        for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
        {
            if (cdata[cc].state != 1)
            {
                continue;
            }
            if (cdata[cnt].state != 1)
            {
                continue;
            }
            if (cc == cnt)
            {
                continue;
            }
            tc = cnt;
            dmghp(tc, 9999999, cc);
        }
        break;
    case 465:
        txtef(4);
        txt(lang(
            u8"隕石が落ちてきた！"s,
            u8"Innumerable meteorites fall all over the area!"s));
        play_animation(22);
        for (int cnt = 0, cnt_end = (mdata(1)); cnt < cnt_end; ++cnt)
        {
            dy = cnt;
            for (int cnt = 0, cnt_end = (mdata(0)); cnt < cnt_end; ++cnt)
            {
                dx = cnt;
                if (rnd(3) == 0)
                {
                    map(dx, dy, 0) = 12 + rnd(2);
                }
                if (rnd(40) == 0)
                {
                    mef_add(dx, dy, 5, 24, rnd(4) + 3, 50);
                }
                if (map(dx, dy, 1) != 0)
                {
                    tc = map(dx, dy, 1) - 1;
                    dmg = sdata(16, cc) * efp / 10;
                    dmghp(tc, dmg, cc, 50, 1000);
                }
            }
        }
        break;
    case 656:
        if (is_in_fov(cc))
        {
            txt(lang(
                name(cc) + u8"は仲間を鼓舞した。"s,
                name(cc) + u8" cheer"s + _s(cc) + u8"."s));
        }
        for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
        {
            if (cdata[cnt].state != 1)
            {
                continue;
            }
            if (cc == cnt)
            {
                continue;
            }
            if (cc < 16)
            {
                if (cnt >= 16)
                {
                    continue;
                }
                else if (cdata[cc].relationship != cdata[tc].relationship)
                {
                    continue;
                }
            }
            tc = cnt;
            dx = cdata[tc].position.x;
            dy = cdata[tc].position.y;
            if (dist(cdata[cc].position.x, cdata[cc].position.y, dx, dy)
                > the_ability_db[656]->sdataref3 % 1000 + 1)
            {
                continue;
            }
            if (fov_los(cdata[cc].position.x, cdata[cc].position.y, dx, dy)
                == 0)
            {
                continue;
            }
            if (is_in_fov(tc))
            {
                txtef(4);
                txt(lang(
                    name(tc) + u8"は興奮した！"s,
                    name(tc) + u8" "s + is(tc) + u8" excited!"s));
            }
            buff_add(tc, 5, sdata(17, cc) * 5 + 50, 15);
            buff_add(tc, 7, sdata(17, cc) * 5 + 100, 60);
            buff_add(tc, 18, 1500, 30);
        }
        break;
    case 1131:
        if (tc != 0)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            break;
        }
        snd(64);
        if (!is_cursed(efstatus))
        {
            txtef(2);
            txt(lang(
                u8"エーテルの抗体があなたの体内に行き渡った。"s,
                u8"Your Ether Disease is cured greatly."s));
            modcorrupt(efp * -10);
        }
        else
        {
            txtef(8);
            txt(lang(
                u8"エーテルの病菌があなたの体内に行き渡った。"s,
                u8" The Ether Disease spreads around your body."s));
            modcorrupt(200);
        }
        break;
    case 633:
        if (tc != 0)
        {
            break;
        }
        txtef(8);
        txt(lang(
            name(cc) + u8"に睨まれ、あなたはエーテルに侵食された。"s,
            name(cc) + u8" gazes at you. Your Ether Disease deteriorates."s));
        modcorrupt(100);
        break;
    case 638:
    case 648:
        if (efid == 648)
        {
            if (is_in_fov(tc))
            {
                txt(lang(
                    name(cc) + u8"は"s + name(tc) + u8"を罵倒した。"s,
                    name(cc) + u8" insult"s + _s(cc) + u8" "s + name(tc)
                        + u8"."s));
                txtef(9);
                if (jp)
                {
                    if (cdata[cc].sex == 0)
                    {
                        txt(u8"「すっこんでろ雑魚め」"s,
                            u8"「オマエ程度が戦うだと？」"s,
                            u8"「すぐに殺してやるよ」"s,
                            u8"「消えろザコめ」"s,
                            u8"「このかたつむり野郎」"s,
                            u8"「すぐにミンチにしてやるよ」"s);
                    }
                    else if (rnd(2))
                    {
                        txt(u8"「グシャグシャにしてやるわ」"s,
                            u8"「地べたを這いずりなさい」"s,
                            u8"「ウージッムシ♪ウージッムシ♪」"s,
                            u8"「目障りよ」"s,
                            u8"「もがけ。苦しめ！」"s,
                            u8"「その下品な眼をくりぬくの」"s);
                    }
                    else
                    {
                        txt(u8"「このカタツムリが」"s,
                            u8"「どうしたの？もう終わりなの？」"s,
                            u8"「潔く、くたばりなさい」"s,
                            u8"「生まれてきたことを後悔するのね」"s,
                            u8"「このブタめ」"s,
                            u8"「すぐにミンチにしてあげる」"s);
                    }
                }
                else if (rnd(2))
                {
                    txt(u8"\"You suck!\""s,
                        u8"\"You will die alone.\""s,
                        u8"\"Bow down before me.\""s,
                        u8"\"Go jump off a bridge.\""s,
                        u8"\"Bang your head against the wall!\""s,
                        u8"\"Why do you sniff under your dog's tail?\""s);
                }
                else
                {
                    txt(u8"\"The world is against you because you are a unsavory decomposing virus.\""s,
                        u8"\"You are no better than a immoral guzzling bureaucrat.\""s,
                        u8"\"You are so lowly.\""s,
                        u8"\"Get off me.\""s);
                }
            }
        }
        dmgcon(tc, status_ailment_t::dimmed, 200);
        break;
    case 652:
        if (is_in_fov(tc))
        {
            txt(lang(
                name(cc) + u8"は"s + name(tc) + u8"を睨み付けた。"s,
                name(cc) + u8" gaze"s + _s(cc) + u8" at "s + name(tc)
                    + u8"."s));
        }
        dmgmp(tc, rnd(20) + 1);
        break;
    case 1133:
        if (is_in_fov(tc))
        {
            txt(lang(
                name(tc) + u8"は炎に包まれた。"s,
                name(tc) + u8" "s + is(tc) + u8" surrounded by flames."s));
        }
        mef_add(
            cdata[tc].position.x,
            cdata[tc].position.y,
            5,
            24,
            rnd(15) + 25,
            efp,
            cc);
        mapitem_fire(cdata[tc].position.x, cdata[tc].position.y);
        return 0;
    case 651:
        if (is_in_fov(tc))
        {
            txt(lang(
                name(cc) + u8"は"s + name(tc) + u8"のバックパックを漁った。"s,
                name(cc) + u8" loot"s + _s(cc) + u8" "s + name(tc) + your(tc)
                    + u8" backpack."s));
        }
        p = -1;
        for (const auto& cnt : items(tc))
        {
            if (inv[cnt].number == 0)
            {
                continue;
            }
            if (inv[cnt].id == 618)
            {
                p = cnt;
                break;
            }
        }
        if (p == -1)
        {
            for (const auto& cnt : items(tc))
            {
                if (inv[cnt].number == 0)
                {
                    continue;
                }
                if (ibit(5, cnt))
                {
                    continue;
                }
                if (the_item_db[inv[cnt].id]->category != 57000)
                {
                    continue;
                }
                p = cnt;
                break;
            }
        }
        if (p == -1)
        {
            break;
        }
        ci = p;
        if (ibit(6, ci))
        {
            if (is_in_fov(tc))
            {
                txt(lang(
                    name(cc) + u8"は"s + itemname(ci, 1)
                        + u8"の異臭に気付き手をひっこめた。"s,
                    name(cc) + u8" notice"s + _s(cc) + u8" unusual odor from "s
                        + itemname(ci, 1) + u8" and step"s + _s(cc)
                        + u8" back."s));
            }
            break;
        }
        rowact_item(ci);
        if (is_in_fov(tc))
        {
            snd(18);
            txt(lang(
                name(cc) + u8"は"s + itemname(ci, 1) + u8"を食べた！"s,
                name(cc) + u8" eat"s + _s(cc) + u8" "s + itemname(ci, 1)
                    + u8"!"s));
        }
        healhp(cc, cdata[cc].max_hp / 3);
        continuous_action_eating_finish();
        refresh_burden_state();
        break;
    case 464:
        animeload(10, tc);
        for (int cnt = 0,
                 cnt_end = cnt + (clamp(4 + rnd((efp / 50 + 1)), 1, 15));
             cnt < cnt_end;
             ++cnt)
        {
            snd(64);
            flt(calcobjlv(efp / 10), calcfixlv(3));
            dbid = 0;
            dbid = 54;
            p = 400 + rnd(efp);
            if (rnd(30) == 0)
            {
                dbid = 55;
                p = 1;
            }
            if (rnd(80) == 0)
            {
                dbid = 622;
                p = 1;
            }
            if (rnd(2000) == 0)
            {
                dbid = 290;
                p = 1;
            }
            nostack = 1;
            itemcreate(-1, dbid, cdata[cc].position.x, cdata[cc].position.y, p);
            txt(lang(
                itemname(ci) + u8"が降ってきた！"s,
                itemname(ci) + u8" fall"s + _s2(inv[ci].number) + u8" down!"s));
            await(100);
            redraw();
        }
        break;
    case 463:
        snd(72);
        txt(lang(
            u8"あなたは四次元のポケットを召喚した。"s,
            u8"You summon 4 dimensional pocket."s));
        invfile = 8;
        ctrl_file(file_operation2_t::_4, u8"shoptmp.s2");
        if (fs::exists(
                filesystem::dir::tmp() / (u8"shop"s + invfile + u8".s2")))
        {
            ctrl_file(file_operation2_t::_3, u8"shop"s + invfile + u8".s2");
        }
        else
        {
            for (const auto& cnt : items(-1))
            {
                inv[cnt].number = 0;
            }
        }
        shoptrade = 0;
        menucycle = 1;
        invsubroutine = 1;
        invctrl(0) = 22;
        invctrl(1) = 5;
        invcontainer = clamp(efp / 10 + 10, 10, 300);
        mode = 6;
        snd(100);
        ctrl_inventory();
        invcontainer = 0;
        ctrl_file(file_operation2_t::_4, u8"shop"s + invfile + u8".s2");
        ctrl_file(file_operation2_t::_3, u8"shoptmp.s2");
        mode = 0;
        break;
    }

the_end:
    ci = efcibk;
    efstatus = curse_state_t::none;
    efsource = 0;
    return 1;
}



} // namespace elona
