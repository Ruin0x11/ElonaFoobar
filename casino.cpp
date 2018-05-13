#include "casino.hpp"
#include "atx.hpp"
#include "audio.hpp"
#include "calc.hpp"
#include "character.hpp"
#include "config.hpp"
#include "damage.hpp"
#include "elona.hpp"
#include "i18n.hpp"
#include "item.hpp"
#include "macro.hpp"
#include "ability.hpp"
#include "ui.hpp"
#include "variables.hpp"

namespace elona
{

void casino_dealer()
{
    begintempinv();
    atx.through = 0;
    atx.pic = 0;
    snd(39);
    mode = 9;
    atx.bg = u8"bg13"s;
    atx.bgbk = "";
    SDIM3(atx.infon, 80, 5);
    if (atx.id == 2)
    {
        casino_random_site();
        return;
    }
    if (atx.id == 1)
    {
        txt(lang(u8"ディーラーに話しかけた。"s, u8"You talk to the dealer."s));
        play_music(77);
        casino_wrapper();
        return;
    }
    if (atx.id == 4)
    {
        play_music(77);
        casino_wrapper();
        return;
    }
    casino_acquire_items();
    return;
}



void casino_choose_card()
{
label_18671_internal:
    screenupdate = -1;
    update_screen();
    if (atx.id >= 2)
    {
        atx.msgfix = 136;
    }
    for (int cnt = 0, cnt_end = (noteinfo()); cnt < cnt_end; ++cnt)
    {
        noteget(s, cnt);
        if (strmid(s, 0, 1) == u8"@"s)
        {
            s(1) = strmid(s, 1, 2);
            s = strmid(s, 3, s(0).size() - 3);
            font(16 - en * 2);
            color(250, 240, 230);
            if (s(1) == u8"BL"s)
            {
                color(130, 130, 250);
            }
            if (s(1) == u8"GR"s)
            {
                color(130, 250, 130);
            }
            if (s(1) == u8"QM"s)
            {
                color(0, 100, 0);
            }
        }
        else
        {
            font(16 - en * 2);
            color(250, 240, 230);
        }
        pos(170, cnt * 20 + 120 + atx.msgfix);
        mes(s);
        color(0, 0, 0);
    }
    cs_bk = -1;
    pagemax = (listmax - 1) / pagesize;
    if (page < 0)
    {
        page = pagemax;
    }
    else if (page > pagemax)
    {
        page = 0;
    }
    gsel(2);
    gmode(0);
    pos(0, 0);
    gcopy(0, 0, 0, ui.windoww, ui.windowh);
    gsel(0);
    gmode(2);
    keyrange = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
    }
    casino_something_a();
label_1868_internal:
    x(0) = 170;
    x(1) = 400;
    y(0) = noteinfo() * 20 + 120 + atx.msgfix + 16;
    y(1) = 20 * listmax;
    gmode(0);
    pos(x, y);
    gcopy(2, x, y, x(1), y(1));
    gmode(2);
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        i = list(0, p);
        display_key(
            170, noteinfo() * 20 + 120 + atx.msgfix + 16 + cnt * 20, cnt);
        s = listn(0, p);
        cs_list(
            cs == cnt,
            s,
            200,
            noteinfo() * 20 + 120 + atx.msgfix + 16 + cnt * 20,
            0,
            2);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(rtval, snd(40));
    if (chatesc != -1)
    {
        if (key == key_cancel)
        {
            snd(40);
            rtval = chatesc;
        }
    }
    if (rtval != -1)
    {
        casino_something_b();
        atx.pic = 0;
        return;
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_18671_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_18671_internal;
        }
    }
    goto label_1868_internal;
}



void casino_something_a()
{
    casino_something_c();
    if (atx.mattile != -1)
    {
        atx.mattile = rnd(atx.mattile(2)) + atx.mattile(1);
    }
    for (int cnt = 0; cnt < 12; ++cnt)
    {
        x(0) = 170;
        x(1) = 300;
        y(0) = noteinfo() * 20 + 120 + atx.msgfix + 16;
        y(1) = 20 * listmax;
        gmode(0);
        pos(x - 50, y - 50);
        gcopy(2, x - 50, y - 50, 100 + x(1), y(1) + 100);
        if (cnt == 11)
        {
            break;
        }
        gmode(4, x(1), y(1), cnt * 25);
        pos(x + x(1) / 2 - 10 + cnt, y + y(1) / 2);
        grotate(2, 0, 0, 0, x(1), y(1));
        if (atx.pic != 0)
        {
            x(0) = 345;
            x(1) = atx.pic(2) + 120;
            y(0) = 170;
            y(1) = atx.pic(3);
            gmode(0);
            pos(x - atx.pic(2) / 2, y - atx.pic(3) / 2);
            gcopy(2, x - atx.pic(2) / 2, y - atx.pic(3) / 2, x(1), y(1));
            pos(x, y);
            gmode(2, ui.tiles, ui.tiles);
            double p_double;
            if (cnt == 10)
            {
                p_double = 0;
            }
            else
            {
                p_double = 0.6222 * cnt;
            }
            p(1) = 1;
            if (atx.pic == 2)
            {
                p(1) = 5;
            }
            grotate(
                p(1),
                atx.pic(1) % 33 * 32,
                atx.pic(1) / 33 * 32,
                p_double,
                cnt * (atx.pic(2) / 10),
                cnt * (atx.pic(3) / 10));
        }
        if (atx.mattile != -1)
        {
            int cnt2 = cnt;
            for (int cnt = 0; cnt < 2; ++cnt)
            {
                x(0) = cnt * 250 + 170;
                x(1) = 96;
                y(0) = 120;
                y(1) = 96;
                gmode(0);
                pos(x, y);
                gcopy(2, x, y, x(1), y(1));
                pos(x + x(1) / 2, y + y(1) / 2);
                gmode(2, ui.tiles, ui.tiles);
                grotate(
                    1,
                    atx.mattile % 33 * 32,
                    atx.mattile / 33 * 32,
                    0,
                    cnt2 * 9,
                    cnt2 * 9);
            }
        }
        await(20);
        redraw();
    }
    gmode(2);
    atx.pic = 0;
    return;
}



void casino_something_b()
{
    casino_something_c();
    for (int cnt = 0; cnt < 11; ++cnt)
    {
        x = 170;
        y(0) = noteinfo() * 20 + 120 + atx.msgfix + 16;
        y(1) = 20 * listmax;
        gmode(0);
        pos(x - 50, y - 50);
        gcopy(2, x - 50, y - 50, 100 + x(1), y(1) + 100);
        gmode(4, x(1), y(1), 250 - cnt * 25);
        pos(x + x(1) / 2 - 2 * cnt, y + y(1) / 2);
        grotate(2, 0, 0, 0, x(1), y(1));
        await(15);
        redraw();
    }
    return;
}



void casino_something_c()
{
    x(1) = 300;
    cs = -1;
    boxf(
        170,
        noteinfo() * 20 + 120 + atx.msgfix + 16,
        170 + x(1),
        noteinfo() * 20 + 120 + atx.msgfix + 16 + 20 * listmax);
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        i = list(0, p);
        display_key(
            170, noteinfo() * 20 + 120 + atx.msgfix + 16 + cnt * 20, cnt);
        s = listn(0, p);
        gmode(2);
        cs_list(
            cs == cnt,
            s,
            200,
            noteinfo() * 20 + 120 + atx.msgfix + 16 + cnt * 20,
            0,
            2);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    gsel(2);
    gmode(0);
    pos(0, 0);
    gcopy(
        0, 170, noteinfo() * 20 + 120 + atx.msgfix + 16, x(1), 20 * listmax);
    gsel(0);
    gmode(2);
    cs = 0;
    return;
}



void casino_acquire_items()
{
    mtilefilecur = -1;
    label_1746();
    f = 0;
    for (const auto& cnt : items(-1))
    {
        if (inv[cnt].number != 0)
        {
            f = 1;
        }
    }
    if (f == 1)
    {
        if (cdata[0].hp >= 0)
        {
            txt(lang(
                u8"幾つかの戦利品がある。"s,
                u8"There're some items you can acquire."s));
            screenupdate = -1;
            update_screen();
            invsubroutine = 1;
            invctrl(0) = 22;
            invctrl(1) = 0;
            ctrl_inventory();
        }
    }
    mode = 0;
    atx.id = 0;
    exittempinv();
    await(100);
    snd(39);
    play_music();
    return;
}



void casino_random_site()
{
    int refval1 = 0;
    atx.ap = 10;
    atx.spot = 19;
    atx.infon(0) = u8"ランダムサイト"s;
    atx_init();
    atx.through = 1;
    if (atx.id(1) == 0)
    {
        atx.id(1) = 3;
        atx.lv = gdata_current_dungeon_level;
        if (mdata(6) == 20)
        {
            atx.id(1) = 1;
        }
        if (mdata(6) == 21)
        {
            atx.id(1) = 4;
        }
        if (mdata(6) == 22)
        {
            atx.id(1) = 2;
        }
        if (mdata(6) == 23)
        {
            atx.id(1) = 4;
        }
        if (mdata(6) == 1)
        {
            atx.lv = cdata[0].level;
            if (4 <= gdata(62) && gdata(62) < 9)
            {
                atx.id(1) = 2;
            }
            if (264 <= gdata(62) && gdata(62) < 363)
            {
                atx.id(1) = 3;
            }
            if (9 <= gdata(62) && gdata(62) < 13)
            {
                atx.id(1) = 2;
            }
            if (13 <= gdata(62) && gdata(62) < 17)
            {
                atx.id(1) = 3;
            }
        }
    }
    if (atx.id(1) == 8)
    {
        atx.bg = u8"bg21"s;
        atx.mattile(0) = 0;
        atx.mattile(1) = 495;
        atx.mattile(2) = 3;
        atx.spot = 16;
        noteadd(u8"この辺りは残骸やら遺品やらでごちゃごちゃだ。"s);
    }
    if (atx.id(1) == 7)
    {
        atx.bg = u8"bg20"s;
        atx.mattile(0) = 0;
        atx.mattile(1) = 495;
        atx.mattile(2) = 3;
        atx.spot = 15;
        noteadd(u8"この辺りには見たこともない植物がたくさんある。"s);
    }
    if (atx.id(1) == 6)
    {
        atx.bg = u8"bg19"s;
        atx.mattile(0) = 0;
        atx.mattile(1) = 495;
        atx.mattile(2) = 3;
        atx.spot = 14;
        noteadd(u8"天然の鉱石の宝庫だ。"s);
    }
    if (atx.id(1) == 5)
    {
        atx.bg = u8"bg18"s;
        atx.mattile(0) = 0;
        atx.mattile(1) = 495;
        atx.mattile(2) = 3;
        atx.spot = 13;
        noteadd(u8"綺麗な泉がある。"s);
    }
    if (atx.id(1) == 1)
    {
        atx.bg = u8"bg13"s;
        atx.mattile(0) = 0;
        atx.mattile(1) = 495;
        atx.mattile(2) = 3;
        atx.spot = 9;
    }
    if (atx.id(1) == 4)
    {
        atx.bg = u8"bg17"s;
        atx.mattile(0) = 0;
        atx.mattile(1) = 495;
        atx.mattile(2) = 3;
        atx.spot = 12;
    }
    if (atx.id(1) == 2)
    {
        atx.bg = u8"bg15"s;
        atx.mattile(0) = 0;
        atx.mattile(1) = 495;
        atx.mattile(2) = 3;
        atx.spot = 10;
    }
    if (atx.id(1) == 3)
    {
        atx.bg = u8"bg16"s;
        atx.mattile(0) = 0;
        atx.mattile(1) = 495;
        atx.mattile(2) = 3;
        atx.spot = 11;
    }
label_1875:
    if (atx.ap <= 0 || cdata[0].hp < 0)
    {
        casino_search();
        return;
    }
    atx_init();
    noteadd(u8"何をしよう？"s);
    list(0, listmax) = 1;
    listn(0, listmax) = u8"探索"s;
    ++listmax;
    list(0, listmax) = 3;
    listn(0, listmax) = u8"去る"s;
    ++listmax;
    atx.infon(1) = u8"行動回数残り "s + atx.ap + u8"回\n"s;
    chatesc = 3;
    atx.msgfix = 0;
    atx.screenupdate = 1;
    casino_choose_card();
    if (rtval == 1)
    {
        goto label_1876_internal;
        return;
    }
    if (rtval == 2)
    {
        goto label_1876_internal;
        return;
    }
    if (rtval == 3)
    {
        casino_search();
        return;
    }
    goto label_1875;
label_1876_internal:
    --atx.ap;
    atx_init();
    if (rnd(1) == 0)
    {
        refval1 = -1;
        if (atx.id(1) == 7)
        {
            atx.pic(0) = 1;
            atx.pic(1) = 171;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"茂みを見つけた。"s);
            list(0, listmax) = 1;
            listn(0, listmax) = u8"調べる"s;
            ++listmax;
            list(0, listmax) = 2;
            listn(0, listmax) = u8"採取する("s
                + i18n::_(u8"ability", std::to_string(180), u8"name") + u8": "s
                + sdata(180, 0) + u8")"s;
            ++listmax;
            refval1 = 7;
        }
        if (atx.id(1) == 6)
        {
            atx.pic(0) = 1;
            atx.pic(1) = 219;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"鉱石の岩がある。"s);
            list(0, listmax) = 1;
            listn(0, listmax) = u8"調べる"s;
            ++listmax;
            list(0, listmax) = 2;
            listn(0, listmax) = u8"掘る("s
                + i18n::_(u8"ability", std::to_string(163), u8"name") + u8": "s
                + sdata(163, 0) + u8")"s;
            ++listmax;
            refval1 = 7;
        }
        if (atx.id(1) == 5)
        {
            atx.pic(0) = 1;
            atx.pic(1) = 439;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"泉がある。"s);
            list(0, listmax) = 1;
            listn(0, listmax) = u8"飲む"s;
            ++listmax;
            list(0, listmax) = 2;
            listn(0, listmax) = u8"釣る("s
                + i18n::_(u8"ability", std::to_string(185), u8"name") + u8": "s
                + sdata(185, 0) + u8")"s;
            ++listmax;
            refval1 = 7;
        }
        if (atx.id(1) == 8)
        {
            atx.pic(0) = 1;
            atx.pic(1) = 199;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"残骸を見つけた。"s);
            list(0, listmax) = 1;
            listn(0, listmax) = u8"あさる"s;
            ++listmax;
            list(0, listmax) = 2;
            listn(0, listmax) = u8"解剖する("s
                + i18n::_(u8"ability", std::to_string(161), u8"name") + u8": "s
                + sdata(161, 0) + u8")"s;
            ++listmax;
            refval1 = 7;
        }
        if (refval1 == -1)
        {
            noteadd(u8"壁に何やら怪しいひび割れがある…"s);
            list(0, listmax) = 1;
            listn(0, listmax) = u8"叩く(筋力)"s;
            ++listmax;
            list(0, listmax) = 2;
            listn(0, listmax) = u8"調べる(感知)"s;
            ++listmax;
        }
        atx.infon(1) = u8"行動回数残り "s + atx.ap + u8"回\n"s;
        chatesc = 1;
        atx.msgfix = 0;
        atx.screenupdate = 1;
        casino_choose_card();
        atx_init();
        if (rtval == 1)
        {
            for (int cnt = 0; cnt < 3; ++cnt)
            {
                p = random_material(atx.lv);
                atx.pic(0) = 1;
                atx.pic(1) = matref(2, p);
                atx.pic(2) = 96;
                atx.pic(3) = 96;
                snd(41);
                mat(p) += 1;
                noteadd(lang(
                    u8"@BL"s + matname(p) + u8"を"s + 1
                        + u8"個手に入れた！(所持数:"s + mat(p) + u8"個)"s,
                    u8"@BLYou get "s + 1 + u8" "s + matname(p)
                        + u8"(s)! (Total:"s + mat(p) + u8")"s));
            }
            atx.through = 1;
            goto label_1875;
        }
        if (rtval == 2)
        {
            atx.through = 1;
            goto label_1875;
        }
    }
    if (rnd(8) == 0)
    {
        if (rnd(4) == 0)
        {
            noteadd(u8"あれ…？"s);
            noteadd(u8"道に迷った！ (行動回数-2)"s);
            atx.ap -= 2;
            atx.through = 1;
            goto label_1875;
        }
        if (rnd(2) == 0)
        {
            atx.pic(0) = 2;
            atx.pic(1) = 205;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"幽霊に脅かされた！ (行動回数-1)"s);
            snd(70);
            atx.ap -= 1;
            atx.through = 1;
            goto label_1875;
        }
        if (rnd(2) == 0)
        {
            atx.pic(0) = 1;
            atx.pic(1) = 424;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"小石につまづいてころんでしまった！ (行動回数-1)"s);
            snd(70);
            atx.ap -= 1;
            atx.through = 1;
            goto label_1875;
        }
    }
    if (rnd(8) == 0)
    {
        if (rnd(4) == 0)
        {
            atx.pic(0) = 1;
            atx.pic(1) = 200;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"ふかふかの藁を見つけた。"s);
            noteadd(u8"ふかふかして気持ちいい。（行動回数+3)"s);
            atx.ap += 2;
            atx.through = 1;
            goto label_1875;
        }
        if (rnd(2) == 0)
        {
            atx.pic(0) = 1;
            atx.pic(1) = 294;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"切り株がある。"s);
            noteadd(u8"腰を下ろして疲れを癒した。（行動回数+2)"s);
            atx.ap += 2;
            atx.through = 1;
            goto label_1875;
        }
        if (rnd(2) == 0)
        {
            atx.pic(0) = 1;
            atx.pic(1) = 127;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"綺麗な花を見つけた。"s);
            noteadd(u8"心がなごんだ…（行動回数+2)"s);
            atx.ap += 2;
            atx.through = 1;
            goto label_1875;
        }
    }
    if (rnd(7) == 0)
    {
        if (rnd(3))
        {
            atx.pic(0) = 2;
            atx.pic(1) = 210;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"痛っ！蚊に刺された。"s);
            snd(2);
            damage_hp(0, cdata[0].max_hp * 5 / 100, -10);
        }
        else
        {
            atx.pic(0) = 2;
            atx.pic(1) = 216;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"うっ！ヘビに噛まれた。"s);
            snd(2);
            damage_hp(0, cdata[0].max_hp * 10 / 100, -10);
        }
        atx.through = 1;
        goto label_1875;
    }
    if (rnd(3) == 0)
    {
        if (rnd(3))
        {
            atx.pic(0) = 2;
            atx.pic(1) = 210;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"痛っ！蚊に刺された。"s);
            snd(2);
            damage_hp(0, cdata[0].max_hp * 5 / 100, -10);
        }
        else
        {
            atx.pic(0) = 2;
            atx.pic(1) = 216;
            atx.pic(2) = 96;
            atx.pic(3) = 96;
            noteadd(u8"うっ！ヘビに噛まれた。"s);
            snd(2);
            damage_hp(0, cdata[0].max_hp * 10 / 100, -10);
        }
        atx.through = 1;
        goto label_1875;
    }
    if (rnd(2) == 0)
    {
        atx.pic(0) = 1;
        atx.pic(1) = 220;
        atx.pic(2) = 96;
        atx.pic(3) = 96;
        noteadd(u8"宝箱がある。"s);
        list(0, listmax) = 1;
        listn(0, listmax) = u8"錠を解体する("s
            + i18n::_(u8"ability", std::to_string(158), u8"name") + u8": "s
            + sdata(158, 0) + u8")"s;
        ++listmax;
        list(0, listmax) = 3;
        listn(0, listmax) = u8"叩き割る(筋力: "s + sdata(10, 0) + u8")"s;
        ++listmax;
        atx.infon(1) = u8"行動回数残り "s + atx.ap + u8"回\n"s;
        chatesc = 1;
        atx.msgfix = 0;
        atx.screenupdate = 1;
        casino_choose_card();
        atx_init();
        goto label_1875;
    }
    if (rnd(2) == 0)
    {
        p = random_material(atx.lv);
        atx.pic(0) = 1;
        atx.pic(1) = matref(2, p);
        atx.pic(2) = 96;
        atx.pic(3) = 96;
        snd(41);
        mat(p) += 1;
        noteadd(lang(
            u8"@BL"s + matname(p) + u8"を"s + 1 + u8"個手に入れた！(所持数:"s
                + mat(p) + u8"個)"s,
            u8"@BLYou get "s + 1 + u8" "s + matname(p) + u8"(s)! (Total:"s
                + mat(p) + u8")"s));
        atx.through = 1;
        goto label_1875;
    }
    if (rnd(3) == 0)
    {
        noteadd(u8"何も見つからなかった…"s);
        atx.through = 1;
        goto label_1875;
    }
    noteadd(u8"何も見つからなかった…"s);
    atx.through = 1;
    goto label_1875;
}



void casino_search()
{
    atx_init();
    if (cdata[0].hp >= 0)
    {
        noteadd(u8"探索を終えた。"s);
        list(0, listmax) = 0;
        listn(0, listmax) = u8"戻る"s;
        ++listmax;
    }
    else
    {
        noteadd(u8"ぐふ…"s);
        list(0, listmax) = 0;
        listn(0, listmax) = u8"(断末魔の叫び)"s;
        ++listmax;
    }
    atx.infon(1) = u8"行動回数残り "s + atx.ap + u8"回\n"s;
    chatesc = 0;
    atx.msgfix = 0;
    atx.screenupdate = 1;
    casino_choose_card();
    casino_acquire_items();
}


void casino_wrapper()
{
    bool finished = false;
    while(!finished)
    {
        finished = casino_start();
    }
}

bool casino_start()
{
    bool finished = false;
    atx.bg = u8"bg14"s;
    atx.mattile = -1;
    atx.infon(0) = lang(
        u8"カジノ《フォーチュンクッキー》"s, u8"Casino <<Fortune Cookie>>"s);
    atx_init();
    noteadd(lang(
        u8"カジノ《フォーチュンクッキー》へようこそ。"s,
        u8"Welcome to the casino, Fortune cookie!"s));
    noteadd(lang(
        u8"チップマテリアルと引き換えにゲームをすることができます。"s,
        u8"You can bet the casino chips you have and play some games."s));
    noteadd(lang(u8"ごゆっくりお楽しみ下さい。"s, u8"Enjoy your stay."s));
    if (gdata_used_casino_once == 0)
    {
        noteadd(""s);
        noteadd(lang(
            u8"お客様は初めてのご利用のようですね。"s,
            u8"Looks like you play for the first time, sir."s));
        noteadd(lang(
            u8"当店からチップマテリアルを10枚進呈します。"s,
            u8"We're offering you 10 free casino chips to try our games."s));
        gdata_used_casino_once = 1;
        snd(41);
        mat(1) += 10;
        noteadd(lang(
            u8"@BL"s + matname(1) + u8"を"s + 10 + u8"個手に入れた！(所持数:"s
                + mat(1) + u8"個)"s,
            u8"@BLYou get "s + 10 + u8" "s + matname(1) + u8"(s)! (Total:"s
                + mat(1) + u8")"s));
    }
    atx.infon(1) = lang(
        u8"カジノチップ残り "s + mat(1) + u8"枚\n"s,
        u8"Casino chips left: "s + mat(1) + u8"\n"s);
    atx.infon(2) = "";
    list(0, listmax) = 0;
    listn(0, listmax) = lang(u8"店を出る"s, u8"Later."s);
    ++listmax;
    list(0, listmax) = 1;
    listn(0, listmax) =
        lang(u8"ブラックジャック"s, u8"I want to play Blackjack."s);
    ++listmax;
    chatesc = 0;
    atx.msgfix = 0;
    atx.screenupdate = 1;
    casino_choose_card();
    if (rtval == 0)
    {
        casino_acquire_items();
        return true;
    }
    else if (rtval == 1)
    {
        while(!finished)
        {
            finished = casino_blackjack();
        }
        return false;
    }
    return true;
}



bool casino_blackjack()
{
    card_state state = {};
    int stake = 0;
    int winrow = 0;
    int cardround = 0;
    int winner = 0;
    atx_init();
    noteadd(lang(
        u8"ブラックジャックは、カードの合計を21に近づけるゲームです。"s,
        u8"In Blackjack, the hand with the highest total wins as long as it"s));
    noteadd(lang(
        u8"J,Q,Kは10に、Aは1または11に数えられます。21を越えると負けです。"s,
        u8"doesn't exceed 21. J,Q,K are counted as 10 and A is counted as 1 or 11."s));
    noteadd(lang(
        u8"では、賭けるチップを宣言してください。"s,
        u8"More bets means better rewards."s));
    noteadd(lang(
        u8"チップが多いほど、景品の質があがります。"s,
        u8"How many tips would you like to bet?"s));
    atx.infon(1) = lang(
        u8"カジノチップ残り "s + mat(1) + u8"枚\n"s,
        u8"Casino chips left: "s + mat(1) + u8"\n"s);
    atx.infon(2) = "";
    if (mat(1) <= 0)
    {
        noteadd(""s);
        noteadd(lang(
            u8"お客様はチップをもっていません。"s,
            u8"Sorry sir, you don't seem to have casino chips."s));
    }
    list(0, listmax) = 0;
    listn(0, listmax) = lang(u8"やめる"s, u8"I quit."s); // BUG cannot quit
    ++listmax;
    if (mat(1) >= 1)
    {
        list(0, listmax) = 1;
        listn(0, listmax) =
            lang(""s + 1 + u8"枚賭ける"s, u8"Bet "s + 1 + u8" chips."s);
        ++listmax;
    }
    if (mat(1) >= 5)
    {
        list(0, listmax) = 5;
        listn(0, listmax) =
            lang(""s + 5 + u8"枚賭ける"s, u8"Bet "s + 5 + u8" chips."s);
        ++listmax;
    }
    if (mat(1) >= 20)
    {
        list(0, listmax) = 20;
        listn(0, listmax) =
            lang(""s + 20 + u8"枚賭ける"s, u8"Bet "s + 20 + u8" chips."s);
        ++listmax;
    }
    chatesc = 0;
    atx.msgfix = 0;
    atx.screenupdate = 1;
    casino_choose_card();
    if (rtval == 0)
    {
        return false;
    }
    stake = rtval;
    winrow = 0;
    cardround = 0;
    autosave = 1 * (gdata_current_map != 35);
    for (int cnt = 0;; ++cnt)
    {
        screenupdate = -1;
        update_screen();
        atx_init();
        if (cnt == 0)
        {
            mat(1) -= stake;
            noteadd(lang(
                matname(1) + u8"を"s + stake + u8"個失った(残り:"s + mat(1)
                    + u8"個)"s,
                u8"You lose "s + stake + u8" "s + matname(1) + u8"(s). (Total:"s
                    + mat(1) + u8")"s));
        }
        if (cardround == 0)
        {
            state = card_init(60, 160);
            state.no_joker = 1;
            card_player_init(state);
            card_player_add(state, 0, 220, 124);
            card_player_add(state, 1, 220, 240);
        }
        font(14 - en * 2, snail::font_t::style_t::bold);
        color(255, 255, 255);
        pos(152, 154);
        mes(lang(u8"　親"s, u8"Dealer"s));
        pos(152, 270);
        mes(lang(u8"あなた"s, u8"   You"s));
        color(0, 0, 0);
        card_show_pile(state);
        card_showholder(state);
        card_show(state);

        if (cardround == 0)
        {
            for (int cnt = 0; cnt < 4; ++cnt)
            {
                int stat = card_serve(state, cnt % 2);
                if (cnt != 2)
                {
                    card_open2(state, stat);
                }
            }
            for (int cnt = 0; cnt < 1; ++cnt)
            {
                if (card_cp_num(state, 0) >= 5)
                {
                    break;
                }
                if (card_cp_score(state, 0) < card_cp_score(state, 1) || card_cp_score(state, 0) - rnd(5) <= 11)
                {
                    int stat = card_serve(state, 0);
                    card_open2(state, stat);
                    --cnt;
                    continue;
                }
            }
        }
        noteadd(lang(
            u8"あなたの合計は"s + card_cp_score(state, 1) + u8"です。"s,
            u8"Your hand is "s + card_cp_score(state, 1) + u8"."s));
        if (cardround == -1)
        {
            noteadd(lang(
                u8"親の合計は"s + card_cp_score(state, 0) + u8"です。"s,
                u8"The dealer's hand is "s + card_cp_score(state, 0) + u8"."s));
            winner = -1;
            if (card_cp_score(state, 0) <= 21)
            {
                if (card_cp_score(state, 1) > 21 || card_cp_score(state, 0) > card_cp_score(state, 1))
                {
                    winner = 0;
                }
            }
            if (card_cp_score(state, 1) <= 21)
            {
                if (card_cp_score(state, 0) > 21 || card_cp_score(state, 1) > card_cp_score(state, 0))
                {
                    winner = 1;
                }
            }
            if (winner == -1)
            {
                noteadd(
                    lang(u8"勝負は引き分けです。"s, u8"The match is a draw."s));
            }
            if (winner == 0)
            {
                noteadd(lang(u8"あなたの負けです。"s, u8"You lose."s));
            }
            if (winner == 1)
            {
                noteadd(lang(
                    u8"おめでとうございます。あなたの勝ちです。"s,
                    u8"Congratulations, you win."s));
            }
        }
        atx.infon(1) = lang(
            u8"カジノチップ残り "s + mat(1) + u8"枚\n"s,
            u8"Casino chips left: "s + mat(1) + u8"\n"s);
        atx.infon(2) = lang(
            u8"賭けチップ "s + stake + u8"枚 現在"s + winrow + u8"連勝中"s,
            u8"Bets: "s + stake + u8" Wins: "s + winrow + ""s);
        if (cardround == -1)
        {
            if (winner == 1)
            {
                ++winrow;
            }
            if (winner != 0)
            {
                list(0, listmax) = 0;
                listn(0, listmax) =
                    lang(u8"次の勝負へ"s, u8"To the next round."s);
                ++listmax;
            }
            if (winner == 0)
            {
                list(0, listmax) = 0;
                listn(0, listmax) = lang(u8"戻る"s, u8"Bah...!"s);
                ++listmax;
            }
            chatesc = -1;
            atx.msgfix = 240;
            atx.screenupdate = 0;
            casino_choose_card();
            if (winner == 0)
            {
                break;
            }
            cardround = 0;
            continue;
        }
        list(0, listmax) = 0;
        listn(0, listmax) = lang(u8"これに決める"s, u8"Stay."s);
        ++listmax;
        if (card_pile_remain(state) > 10)
        {
            if (card_cp_num(state, 1) < 5)
            {
                list(0, listmax) = 1;
                listn(0, listmax) =
                    lang(u8"もう一枚引く(運)"s, u8"Hit me. (Luck)"s);
                ++listmax;
            }
            list(0, listmax) = 2;
            listn(0, listmax) = lang(u8"イカサマ(器用"s, u8"Cheat. (Dex:"s)
                + sdata(12, 0) + u8")"s;
            ++listmax;
        }
        chatesc = -1;
        atx.msgfix = 240;
        atx.screenupdate = 0;
        casino_choose_card();
        if (rtval == 0)
        {
            card_open2(state, card_cp_black_card(state, 0));
            cardround = -1;
            continue;
        }
        if (rtval == 1)
        {
            int stat = card_serve(state, 1);
            p = stat;
            for (int cnt = 0; cnt < 3; ++cnt)
            {
                if (card_cp_score(state, 1) >= 22)
                {
                    if (card_pile_remain(state) > 10)
                    {
                        if (rnd(sdata(19, 0)) > 40)
                        {
                            txt(lang(
                                u8"このカードは悪い予感がする…"s,
                                u8"I have a bad feeling about this card..."s));
                            trashcard(state, p);
                            int stat = card_serve(state, 1);
                            p = stat;
                        }
                    }
                }
            }
            card_open2(state, p);
        }
        if (rtval == 2)
        {
            trashcard(state, card_last_index(state, 1));
            int stat = card_serve(state, 1);
            card_open2(state, stat);
            p = 20;
            if (stake == 5)
            {
                p = 40;
            }
            if (stake == 20)
            {
                p = 60;
            }
            if (rnd(sdata(12, 0)) < rnd(p))
            {
                atx_init();
                noteadd(lang(u8"イカサマだ！"s, u8"Cheater!"s));
                atx.infon(1) = lang(
                    u8"カジノチップ残り "s + mat(1) + u8"枚\n"s,
                    u8"Casino chips left: "s + mat(1) + u8"\n"s);
                atx.infon(2) = lang(
                    u8"賭けチップ "s + stake + u8"枚 現在"s + winrow
                        + u8"連勝中"s,
                    u8"Bets: "s + stake + u8" Wins: "s + winrow + ""s);
                winrow = 0;
                txt(lang(
                    u8"イカサマが見つかってしまった…"s,
                    u8"You are caught in cheating..."s));
                modify_karma(0, -5);
                list(0, listmax) = 0;
                listn(0, listmax) = lang(u8"濡れ衣だ！"s, u8"I didn't do it!"s);
                ++listmax;
                chatesc = -1;
                atx.msgfix = 0;
                atx.screenupdate = 1;
                casino_choose_card();
                break;
            }
        }
        ++cardround;
    }
    if (winrow > 0)
    {
        atx_init();
        noteadd(lang(
            u8"おめでとうございます。あなたは"s + winrow + u8"連勝しました。"s,
            u8"Congratulations! You've won "s + winrow
                + u8" times in a row."s));
        for (int cnt = 0; cnt < 1; ++cnt)
        {
            i = 2;
            if (winrow > 2)
            {
                i = 3;
            }
            if (winrow > 7)
            {
                if (stake >= 5)
                {
                    i = 4;
                }
            }
            if (winrow > 15)
            {
                if (stake >= 20)
                {
                    i = 5;
                }
            }
            flt(calcobjlv(rnd(stake + winrow * 2) + winrow * 3 / 2 + stake / 2),
                i);
            flttypemajor = fsetwear(rnd(length(fsetwear)));
            itemcreate(-1, 0, -1, -1, 0);
            if (inv[ci].number == 0)
            {
                --cnt;
                continue;
            }
        }
        snd(41);
        noteadd(lang(
            u8"@GRアイテム："s + itemname(ci, inv[ci].number)
                + u8"を戦利品に加えた！"s,
            u8"@GR"s + itemname(ci, inv[ci].number)
                + u8" has been added to your loot list!"s));
        if (winrow > 3)
        {
            if (rnd(200) < winrow * 5 + 5)
            {
                flt();
                itemcreate(-1, 559, -1, -1, 0);
                snd(41);
                noteadd(lang(
                    u8"@GRアイテム："s + itemname(ci, inv[ci].number)
                        + u8"を戦利品に加えた！"s,
                    u8"@GR"s + itemname(ci, inv[ci].number)
                        + u8" has been added to your loot list!"s));
            }
        }
        list(0, listmax) = 0;
        listn(0, listmax) = lang(u8"戻る"s, u8"Great."s);
        ++listmax;
        chatesc = 0;
        atx.msgfix = 0;
        atx.screenupdate = 1;
        casino_choose_card();
        winrow = 0;
    }
    return true;
}



void card_player_init(card_state& state)
{
    int card_holder = 100;
    int player_max = 2;
    int holder_max = 5;

    DIM3(state.card_player, card_holder, player_max);
    state.card_player_max = player_max;
    state.card_holder_max = holder_max;
}



void card_player_add(card_state& state, int prm_419, int prm_420, int prm_421)
{
    state.card_player(1, prm_419) = prm_420;
    state.card_player(2, prm_419) = prm_421;
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        state.card_player(10 + cnt, prm_419) = -1;
    }
}



card_state card_init(int pilex, int piley, int)
{
    card_state state = {};
    state.pilex = pilex;
    state.piley = piley;
    DIM3(state.card, 6, 53);
    state.card_max = 53;
    if (state.no_joker)
    {
        state.card_max = 52;
    }
    for (int cnt = 0, cnt_end = (state.card_max); cnt < cnt_end; ++cnt)
    {
        state.card(0, cnt) = cnt % 13 + 1;
        state.card(1, cnt) = cnt / 13;
        state.card(2, cnt) = 1;
        if (cnt == 52)
        {
            state.card(1, cnt) = 4;
        }
        state.card(5, cnt) = -1;
    }
    int a = 0;
    int b = 0;
    int i = 0;
    int j = 0;
    for (int cnt = 0; cnt < 200; ++cnt)
    {
        // TODO knuth shuffle?
        a = rnd(state.card_max);
        b = rnd(state.card_max);
        i = state.card(0, a);
        j = state.card(1, a);
        state.card(0, a) = state.card(0, b);
        state.card(1, a) = state.card(1, b);
        state.card(0, b) = i;
        state.card(1, b) = j;
    }
    return state;
}



void card_show2(card_state& state, int prm_425, int prm_426)
{
    elona_vector1<int> card_color;
    std::string card_abbrev;
    int card_text_offset = 0;
    font(43 - en * 2, snail::font_t::style_t::bold);
    gmode(2, 64, 96);
    pos(state.card(3, prm_425), state.card(4, prm_425));
    if (state.card(2, prm_425) == 1)
    {
        gcopy(3, 736, 216, 64, 96);
    }
    else
    {
        gcopy(3, 672, 216, 64, 96);
        if (prm_426 == 0)
        {
            gmode(4, ui.tiles, ui.tiles, 220);
            if (state.card(1, prm_425) == 0)
            {
                pos(state.card(3, prm_425) + 32,
                    state.card(4, prm_425) + 36);
                grotate(5, 144, 240, 0, 64, 104);
                card_color(0) = 140;
                card_color(1) = 140;
                card_color(2) = 255;
            }
            if (state.card(1, prm_425) == 1)
            {
                pos(state.card(3, prm_425) + 32,
                    state.card(4, prm_425) + 40);
                grotate(5, 1104, 288, 0, 64, 104);
                card_color(0) = 255;
                card_color(1) = 140;
                card_color(2) = 140;
            }
            if (state.card(1, prm_425) == 2)
            {
                pos(state.card(3, prm_425) + 32,
                    state.card(4, prm_425) + 50);
                grotate(5, 480, 336, 0, 64, 84);
                card_color(0) = 240;
                card_color(1) = 240;
                card_color(2) = 240;
            }
            if (state.card(1, prm_425) == 3)
            {
                pos(state.card(3, prm_425) + 28,
                    state.card(4, prm_425) + 48);
                grotate(5, 1200, 288, 0, 64, 80);
                card_color(0) = 140;
                card_color(1) = 255;
                card_color(2) = 140;
            }
            if (state.card(1, prm_425) == 4)
            {
                pos(state.card(3, prm_425) + 28,
                    state.card(4, prm_425) + 44);
                grotate(5, 1296, 336, 0, 72, 86);
                card_color(0) = 250;
                card_color(1) = 250;
                card_color(2) = 105;
            }
            gmode(2);
            card_abbrev = std::to_string(state.card(0, prm_425));
            card_text_offset = 2;
            if (state.card(0, prm_425) == 1)
            {
                card_abbrev = u8"A"s;
                card_text_offset = 3;
            }
            if (state.card(0, prm_425) == 11)
            {
                card_abbrev = u8"J"s;
                card_text_offset = 13;
            }
            if (state.card(0, prm_425) == 12)
            {
                card_abbrev = u8"Q"s;
                card_text_offset = 14;
            }
            if (state.card(0, prm_425) == 13)
            {
                card_abbrev = u8"K"s;
                card_text_offset = 14;
            }
            if (state.card(1, prm_425) == 4)
            {
                card_abbrev = u8"Jo"s;
                card_text_offset = -12;
            }
            pos(state.card(3, prm_425) + 18
                    - (state.card(0, prm_425) >= 10) * 12
                    + card_text_offset,
                state.card(4, prm_425) + 28);
            color(10, 10, 10);
            bmes(
                card_abbrev,
                card_color(0),
                card_color(1),
                card_color(2));
            color(0, 0, 0);
        }
        else
        {
            font(12 - en * 2);
        }
    }
}



void card_show_pile(card_state& state)
{
    int pile_stack = 0;
    pos(state.pilex - 8, state.piley - 8);
    gcopy(3, 528, 216, 80, 112);
    pile_stack = 0;
    for (int cnt = 0, cnt_end = (state.card_max); cnt < cnt_end; ++cnt)
    {
        if (state.card(5, cnt) == -1)
        {
            ++pile_stack;
            if (pile_stack > 7)
            {
                continue;
            }
            state.card(3, cnt) =
                state.pilex - pile_stack;
            state.card(4, cnt) =
                state.piley - pile_stack;
            card_show2(state, cnt);
        }
    }
    font(16 - en * 2, snail::font_t::style_t::bold);
    color(10, 10, 10);
    pos(state.pilex + 8, state.piley + 70);
    bmes(u8"X "s + pile_stack, 240, 240, 240);
    color(0, 0, 0);
    return;
}



void card_show(card_state& state)
{
    card_show_pile(state);
    for (int cnt = 0; cnt < state.card_max; ++cnt)
    {
        if (state.card(5, cnt) == -1
            || state.card(5, cnt) == -2)
        {
            continue;
        }
        card_show2(state, cnt);
    }
}



int card_serve(card_state& state, int prm_427)
{
    int card_id = -1;
    for (int cnt = 0; cnt < state.card_max; ++cnt)
    {
        if (state.card(5, cnt) == -1)
        {
            card_id = cnt;
            break;
        }
    }
    if (card_id == -1)
    {
        return 0;
    }
    int open_idx = 0;
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        if (state.card_player(10 + cnt, prm_427) == -1)
        {
            open_idx = cnt;
            break;
        }
    }
    state.dx = state.pilex - state.card_player(1, prm_427) - open_idx * 88;
    state.dy = state.piley - state.card_player(2, prm_427);
    state.card(5, card_id) = prm_427;
    state.card_player(10 + open_idx, prm_427) = card_id;
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        if (cnt != 0)
        {
            pos(state.card(3, card_id),
                state.card(4, card_id));
            gcopy(3, 608, 216, 64, 96);
        }
        state.card(3, card_id) =
            state.pilex - state.dx / 10 * cnt;
        if (cnt == 9)
        {
            state.card(3, card_id) =
                state.pilex - state.dx;
        }
        state.card(4, card_id) =
            state.piley - state.dy / 10 * cnt;
        if (cnt == 9)
        {
            state.card(4, card_id) =
                state.piley - state.dy;
        }
        gmode(0);
        gsel(3);
        pos(608, 216);
        gcopy(
            0,
            state.card(3, card_id),
            state.card(4, card_id),
            64,
            96);
        gsel(0);
        gmode(2);
        card_show2(state, card_id);
        card_show_pile(state);
        await(10);
        redraw();
    }
    return card_id;
}



void card_showholder(card_state& state)
{
    for (int card_idx = 0; card_idx < state.card_player_max; ++card_idx)
    {
        for (int holder_idx = 0; holder_idx < state.card_holder_max; ++holder_idx)
        {
            state.dx = state.card_player(1, card_idx) + holder_idx * 88;
            state.dy = state.card_player(2, card_idx);
            pos(state.dx - 8, state.dy - 8);
            gcopy(3, 528, 216, 80, 112);
        }
    }
}



int card_open2(card_state& state, int prm_428, int prm_429)
{
    if (prm_429 == 1)
    {
        if (state.card(2, prm_428) == 0)
        {
            return prm_428;
        }
    }
    snd(71);
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        if (prm_429 == 0)
        {
            pos(state.card(3, prm_428) - 8,
                state.card(4, prm_428) - 8);
            gcopy(3, 528, 216, 80, 112);
            gmode(2, 64, 96);
        }
        else
        {
            pos(state.card(3, prm_428),
                state.card(4, prm_428));
            gcopy(
                4,
                state.card(3, prm_428) - wx - 4,
                state.card(4, prm_428) - wy - 4,
                80,
                112);
            gmode(2, 64, 96);
        }
        pos(state.card(3, prm_428) + 32,
            state.card(4, prm_428) + 48);
        grotate(3, 736, 216, 0, 64 - cnt * 14, 96);
        await(10);
        redraw();
    }
    state.card(2, prm_428) = 0;
    card_show2(state, prm_428, prm_429);
    redraw();
    return prm_428; // TODO never modified
}



int trashcard(card_state& state, int prm_430)
{
    for (int cnt = 0; cnt < 21; ++cnt)
    {
        pos(state.card(3, prm_430) - 8,
            state.card(4, prm_430) - 8);
        gcopy(3, 528, 216, 80, 112);
        gmode(2, 64, 96);
        if (cnt == 20)
        {
            redraw();
            break;
        }
        pos(state.card(3, prm_430) + 32,
            state.card(4, prm_430) + 48);
        grotate(3, 736, 216, 0.015 * cnt * cnt, 64 - cnt * 3, 96 - cnt * 4);
        await(10);
        redraw();
    }
    for (int card_idx = 0; card_idx < state.card_player_max; ++card_idx)
    {
        for (int holder_idx = 0; holder_idx < state.card_holder_max; ++holder_idx)
        {
            if (state.card_player(10 + holder_idx, card_idx) == prm_430)
            {
                state.card_player(10 + holder_idx, card_idx) = -1;
            }
        }
    }
    return prm_430; // TODO never modified
}



int card_cp_score(card_state& state, int prm_431)
{
    int ace = 0;
    int score = 0;
    int current_card_score = 0;
    for (int holder_idx = 0; holder_idx < state.card_holder_max; ++holder_idx)
    {
        current_card_score = state.card_player(10 + holder_idx, prm_431);
        if (current_card_score == -1)
        {
            break;
        }
        current_card_score = state.card(0, current_card_score);
        if (current_card_score >= 10)
        {
            current_card_score = 10;
        }
        if (current_card_score == 1)
        {
            ++ace;
            current_card_score = 11;
        }
        score += current_card_score;
    }
    for (int holder_idx = 0; holder_idx < ace; ++holder_idx)
    {
        if (score > 21)
        {
            score -= 10;
        }
    }
    return score;
}



int card_last_index(card_state& state, int prm_432)
{
    int card_last_idx = 0;
    for (int holder_idx = 0; holder_idx < state.card_holder_max; ++holder_idx)
    {
        if (state.card_player(10 + holder_idx, prm_432) == -1)
        {
            break;
        }
        card_last_idx = state.card_player(10 + holder_idx, prm_432);
    }
    return card_last_idx;
}



int card_cp_black_card(card_state& state, int prm_433)
{
    int card_idx = 0;
    for (int holder_idx = 0; holder_idx < state.card_holder_max; ++holder_idx)
    {
        card_idx = state.card_player(10 + holder_idx, prm_433);
        if (state.card(2, card_idx) == 1)
        {
            break;
        }
    }
    return card_idx;
}


int card_cp_num(card_state& state, int prm_434)
{
    int card_count = 0;
    for (int holder_idx = 0; holder_idx < state.card_holder_max; ++holder_idx)
    {
        if (state.card_player(10 + holder_idx, prm_434) != -1)
        {
            ++card_count;
        }
    }
    return card_count;
}


int card_pile_remain(card_state& state)
{
    int remain = 0;
    for (int card_idx = 0; card_idx < state.card_max; ++card_idx)
    {
        if (state.card(5, card_idx) == -1)
        {
            ++remain;
        }
    }
    return remain;
}

} // namespace elona
