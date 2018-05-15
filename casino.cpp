#include "casino.hpp"
#include "ability.hpp"
#include "audio.hpp"
#include "calc.hpp"
#include "casino_card.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "config.hpp"
#include "elona.hpp"
#include "i18n.hpp"
#include "item.hpp"
#include "itemgen.hpp"
#include "macro.hpp"
#include "menu.hpp"
#include "ui.hpp"
#include "variables.hpp"

namespace elona
{

elona_vector1<int> atxpic;
int atxap;
int txtadvmsgfix;
elona_vector1<int> mattile;

void casino_dealer()
{
    begintempinv();
    atxthrough = 0;
    atxpic = 0;
    snd(39);
    mode = 9;
    atxbg = u8"bg13"s;
    atxbgbk = "";
    SDIM3(atxinfon, 80, 5);
    if (atxid == 2)
    {
        casino_random_site();
        return;
    }
    if (atxid == 1)
    {
        txt(lang(u8"ディーラーに話しかけた。"s, u8"You talk to the dealer."s));
        play_music(77);
        casino_wrapper();
        return;
    }
    if (atxid == 4)
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
    if (atxid >= 2)
    {
        txtadvmsgfix = 136;
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
        pos(170, cnt * 20 + 120 + txtadvmsgfix);
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
    gcopy(0, 0, 0, windoww, windowh);
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
    label_1870();
label_1868_internal:
    x(0) = 170;
    x(1) = 400;
    y(0) = noteinfo() * 20 + 120 + txtadvmsgfix + 16;
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
            170, noteinfo() * 20 + 120 + txtadvmsgfix + 16 + cnt * 20, cnt);
        s = listn(0, p);
        cs_list(
            cs == cnt,
            s,
            200,
            noteinfo() * 20 + 120 + txtadvmsgfix + 16 + cnt * 20,
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
        label_1871();
        atxpic = 0;
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

void label_1870()
{
    label_1872();
    if (mattile != -1)
    {
        mattile = rnd(mattile(2)) + mattile(1);
    }
    for (int cnt = 0; cnt < 12; ++cnt)
    {
        x(0) = 170;
        x(1) = 300;
        y(0) = noteinfo() * 20 + 120 + txtadvmsgfix + 16;
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
        if (atxpic != 0)
        {
            x(0) = 345;
            x(1) = atxpic(2) + 120;
            y(0) = 170;
            y(1) = atxpic(3);
            gmode(0);
            pos(x - atxpic(2) / 2, y - atxpic(3) / 2);
            gcopy(2, x - atxpic(2) / 2, y - atxpic(3) / 2, x(1), y(1));
            pos(x, y);
            gmode(2, inf_tiles, inf_tiles);
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
            if (atxpic == 2)
            {
                p(1) = 5;
            }
            grotate(
                p(1),
                atxpic(1) % 33 * 32,
                atxpic(1) / 33 * 32,
                p_double,
                cnt * (atxpic(2) / 10),
                cnt * (atxpic(3) / 10));
        }
        if (mattile != -1)
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
                gmode(2, inf_tiles, inf_tiles);
                grotate(
                    1,
                    mattile % 33 * 32,
                    mattile / 33 * 32,
                    0,
                    cnt2 * 9,
                    cnt2 * 9);
            }
        }
        await(20);
        redraw();
    }
    gmode(2);
    atxpic = 0;
    return;
}

void label_1871()
{
    label_1872();
    for (int cnt = 0; cnt < 11; ++cnt)
    {
        x = 170;
        y(0) = noteinfo() * 20 + 120 + txtadvmsgfix + 16;
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

void label_1872()
{
    x(1) = 300;
    cs = -1;
    boxf(
        170,
        noteinfo() * 20 + 120 + txtadvmsgfix + 16,
        170 + x(1),
        noteinfo() * 20 + 120 + txtadvmsgfix + 16 + 20 * listmax);
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
            170, noteinfo() * 20 + 120 + txtadvmsgfix + 16 + cnt * 20, cnt);
        s = listn(0, p);
        gmode(2);
        cs_list(
            cs == cnt,
            s,
            200,
            noteinfo() * 20 + 120 + txtadvmsgfix + 16 + cnt * 20,
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
        0, 170, noteinfo() * 20 + 120 + txtadvmsgfix + 16, x(1), 20 * listmax);
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
    atxid = 0;
    exittempinv();
    await(100);
    snd(39);
    play_music();
    return;
}

void casino_random_site()
{
    int atxrefval1 = 0;
    atxap = 10;
    atxspot = 19;
    atxinfon(0) = u8"ランダムサイト"s;
    atxinit();
    atxthrough = 1;
    if (atxid(1) == 0)
    {
        atxid(1) = 3;
        atxlv = gdata_current_dungeon_level;
        if (mdata(6) == 20)
        {
            atxid(1) = 1;
        }
        if (mdata(6) == 21)
        {
            atxid(1) = 4;
        }
        if (mdata(6) == 22)
        {
            atxid(1) = 2;
        }
        if (mdata(6) == 23)
        {
            atxid(1) = 4;
        }
        if (mdata(6) == 1)
        {
            atxlv = cdata[0].level;
            if (4 <= gdata(62) && gdata(62) < 9)
            {
                atxid(1) = 2;
            }
            if (264 <= gdata(62) && gdata(62) < 363)
            {
                atxid(1) = 3;
            }
            if (9 <= gdata(62) && gdata(62) < 13)
            {
                atxid(1) = 2;
            }
            if (13 <= gdata(62) && gdata(62) < 17)
            {
                atxid(1) = 3;
            }
        }
    }
    if (atxid(1) == 8)
    {
        atxbg = u8"bg21"s;
        mattile(0) = 0;
        mattile(1) = 495;
        mattile(2) = 3;
        atxspot = 16;
        noteadd(u8"この辺りは残骸やら遺品やらでごちゃごちゃだ。"s);
    }
    if (atxid(1) == 7)
    {
        atxbg = u8"bg20"s;
        mattile(0) = 0;
        mattile(1) = 495;
        mattile(2) = 3;
        atxspot = 15;
        noteadd(u8"この辺りには見たこともない植物がたくさんある。"s);
    }
    if (atxid(1) == 6)
    {
        atxbg = u8"bg19"s;
        mattile(0) = 0;
        mattile(1) = 495;
        mattile(2) = 3;
        atxspot = 14;
        noteadd(u8"天然の鉱石の宝庫だ。"s);
    }
    if (atxid(1) == 5)
    {
        atxbg = u8"bg18"s;
        mattile(0) = 0;
        mattile(1) = 495;
        mattile(2) = 3;
        atxspot = 13;
        noteadd(u8"綺麗な泉がある。"s);
    }
    if (atxid(1) == 1)
    {
        atxbg = u8"bg13"s;
        mattile(0) = 0;
        mattile(1) = 495;
        mattile(2) = 3;
        atxspot = 9;
    }
    if (atxid(1) == 4)
    {
        atxbg = u8"bg17"s;
        mattile(0) = 0;
        mattile(1) = 495;
        mattile(2) = 3;
        atxspot = 12;
    }
    if (atxid(1) == 2)
    {
        atxbg = u8"bg15"s;
        mattile(0) = 0;
        mattile(1) = 495;
        mattile(2) = 3;
        atxspot = 10;
    }
    if (atxid(1) == 3)
    {
        atxbg = u8"bg16"s;
        mattile(0) = 0;
        mattile(1) = 495;
        mattile(2) = 3;
        atxspot = 11;
    }
label_1875:
    if (atxap <= 0 || cdata[0].hp < 0)
    {
        label_1877();
        return;
    }
    atxinit();
    noteadd(u8"何をしよう？"s);
    list(0, listmax) = 1;
    listn(0, listmax) = u8"探索"s;
    ++listmax;
    list(0, listmax) = 3;
    listn(0, listmax) = u8"去る"s;
    ++listmax;
    atxinfon(1) = u8"行動回数残り "s + atxap + u8"回\n"s;
    chatesc = 3;
    txtadvmsgfix = 0;
    txtadvscreenupdate = 1;
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
        label_1877();
        return;
    }
    goto label_1875;
label_1876_internal:
    --atxap;
    atxinit();
    if (rnd(1) == 0)
    {
        atxrefval1 = -1;
        if (atxid(1) == 7)
        {
            atxpic(0) = 1;
            atxpic(1) = 171;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"茂みを見つけた。"s);
            list(0, listmax) = 1;
            listn(0, listmax) = u8"調べる"s;
            ++listmax;
            list(0, listmax) = 2;
            listn(0, listmax) = u8"採取する("s
                + i18n::_(u8"ability", std::to_string(180), u8"name") + u8": "s
                + sdata(180, 0) + u8")"s;
            ++listmax;
            atxrefval1 = 7;
        }
        if (atxid(1) == 6)
        {
            atxpic(0) = 1;
            atxpic(1) = 219;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"鉱石の岩がある。"s);
            list(0, listmax) = 1;
            listn(0, listmax) = u8"調べる"s;
            ++listmax;
            list(0, listmax) = 2;
            listn(0, listmax) = u8"掘る("s
                + i18n::_(u8"ability", std::to_string(163), u8"name") + u8": "s
                + sdata(163, 0) + u8")"s;
            ++listmax;
            atxrefval1 = 7;
        }
        if (atxid(1) == 5)
        {
            atxpic(0) = 1;
            atxpic(1) = 439;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"泉がある。"s);
            list(0, listmax) = 1;
            listn(0, listmax) = u8"飲む"s;
            ++listmax;
            list(0, listmax) = 2;
            listn(0, listmax) = u8"釣る("s
                + i18n::_(u8"ability", std::to_string(185), u8"name") + u8": "s
                + sdata(185, 0) + u8")"s;
            ++listmax;
            atxrefval1 = 7;
        }
        if (atxid(1) == 8)
        {
            atxpic(0) = 1;
            atxpic(1) = 199;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"残骸を見つけた。"s);
            list(0, listmax) = 1;
            listn(0, listmax) = u8"あさる"s;
            ++listmax;
            list(0, listmax) = 2;
            listn(0, listmax) = u8"解剖する("s
                + i18n::_(u8"ability", std::to_string(161), u8"name") + u8": "s
                + sdata(161, 0) + u8")"s;
            ++listmax;
            atxrefval1 = 7;
        }
        if (atxrefval1 == -1)
        {
            noteadd(u8"壁に何やら怪しいひび割れがある…"s);
            list(0, listmax) = 1;
            listn(0, listmax) = u8"叩く(筋力)"s;
            ++listmax;
            list(0, listmax) = 2;
            listn(0, listmax) = u8"調べる(感知)"s;
            ++listmax;
        }
        atxinfon(1) = u8"行動回数残り "s + atxap + u8"回\n"s;
        chatesc = 1;
        txtadvmsgfix = 0;
        txtadvscreenupdate = 1;
        casino_choose_card();
        atxinit();
        if (rtval == 1)
        {
            for (int cnt = 0; cnt < 3; ++cnt)
            {
                p = random_material(atxlv);
                atxpic(0) = 1;
                atxpic(1) = matref(2, p);
                atxpic(2) = 96;
                atxpic(3) = 96;
                snd(41);
                mat(p) += 1;
                noteadd(lang(
                    u8"@BL"s + matname(p) + u8"を"s + 1
                        + u8"個手に入れた！(所持数:"s + mat(p) + u8"個)"s,
                    u8"@BLYou get "s + 1 + u8" "s + matname(p)
                        + u8"(s)! (Total:"s + mat(p) + u8")"s));
            }
            atxthrough = 1;
            goto label_1875;
        }
        if (rtval == 2)
        {
            atxthrough = 1;
            goto label_1875;
        }
    }
    if (rnd(8) == 0)
    {
        if (rnd(4) == 0)
        {
            noteadd(u8"あれ…？"s);
            noteadd(u8"道に迷った！ (行動回数-2)"s);
            atxap -= 2;
            atxthrough = 1;
            goto label_1875;
        }
        if (rnd(2) == 0)
        {
            atxpic(0) = 2;
            atxpic(1) = 205;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"幽霊に脅かされた！ (行動回数-1)"s);
            snd(70);
            atxap -= 1;
            atxthrough = 1;
            goto label_1875;
        }
        if (rnd(2) == 0)
        {
            atxpic(0) = 1;
            atxpic(1) = 424;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"小石につまづいてころんでしまった！ (行動回数-1)"s);
            snd(70);
            atxap -= 1;
            atxthrough = 1;
            goto label_1875;
        }
    }
    if (rnd(8) == 0)
    {
        if (rnd(4) == 0)
        {
            atxpic(0) = 1;
            atxpic(1) = 200;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"ふかふかの藁を見つけた。"s);
            noteadd(u8"ふかふかして気持ちいい。（行動回数+3)"s);
            atxap += 2;
            atxthrough = 1;
            goto label_1875;
        }
        if (rnd(2) == 0)
        {
            atxpic(0) = 1;
            atxpic(1) = 294;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"切り株がある。"s);
            noteadd(u8"腰を下ろして疲れを癒した。（行動回数+2)"s);
            atxap += 2;
            atxthrough = 1;
            goto label_1875;
        }
        if (rnd(2) == 0)
        {
            atxpic(0) = 1;
            atxpic(1) = 127;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"綺麗な花を見つけた。"s);
            noteadd(u8"心がなごんだ…（行動回数+2)"s);
            atxap += 2;
            atxthrough = 1;
            goto label_1875;
        }
    }
    if (rnd(7) == 0)
    {
        if (rnd(3))
        {
            atxpic(0) = 2;
            atxpic(1) = 210;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"痛っ！蚊に刺された。"s);
            snd(2);
            dmghp(0, cdata[0].max_hp * 5 / 100, -10);
        }
        else
        {
            atxpic(0) = 2;
            atxpic(1) = 216;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"うっ！ヘビに噛まれた。"s);
            snd(2);
            dmghp(0, cdata[0].max_hp * 10 / 100, -10);
        }
        atxthrough = 1;
        goto label_1875;
    }
    if (rnd(3) == 0)
    {
        if (rnd(3))
        {
            atxpic(0) = 2;
            atxpic(1) = 210;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"痛っ！蚊に刺された。"s);
            snd(2);
            dmghp(0, cdata[0].max_hp * 5 / 100, -10);
        }
        else
        {
            atxpic(0) = 2;
            atxpic(1) = 216;
            atxpic(2) = 96;
            atxpic(3) = 96;
            noteadd(u8"うっ！ヘビに噛まれた。"s);
            snd(2);
            dmghp(0, cdata[0].max_hp * 10 / 100, -10);
        }
        atxthrough = 1;
        goto label_1875;
    }
    if (rnd(2) == 0)
    {
        atxpic(0) = 1;
        atxpic(1) = 220;
        atxpic(2) = 96;
        atxpic(3) = 96;
        noteadd(u8"宝箱がある。"s);
        list(0, listmax) = 1;
        listn(0, listmax) = u8"錠を解体する("s
            + i18n::_(u8"ability", std::to_string(158), u8"name") + u8": "s
            + sdata(158, 0) + u8")"s;
        ++listmax;
        list(0, listmax) = 3;
        listn(0, listmax) = u8"叩き割る(筋力: "s + sdata(10, 0) + u8")"s;
        ++listmax;
        atxinfon(1) = u8"行動回数残り "s + atxap + u8"回\n"s;
        chatesc = 1;
        txtadvmsgfix = 0;
        txtadvscreenupdate = 1;
        casino_choose_card();
        atxinit();
        goto label_1875;
    }
    if (rnd(2) == 0)
    {
        p = random_material(atxlv);
        atxpic(0) = 1;
        atxpic(1) = matref(2, p);
        atxpic(2) = 96;
        atxpic(3) = 96;
        snd(41);
        mat(p) += 1;
        noteadd(lang(
            u8"@BL"s + matname(p) + u8"を"s + 1 + u8"個手に入れた！(所持数:"s
                + mat(p) + u8"個)"s,
            u8"@BLYou get "s + 1 + u8" "s + matname(p) + u8"(s)! (Total:"s
                + mat(p) + u8")"s));
        atxthrough = 1;
        goto label_1875;
    }
    if (rnd(3) == 0)
    {
        noteadd(u8"何も見つからなかった…"s);
        atxthrough = 1;
        goto label_1875;
    }
    noteadd(u8"何も見つからなかった…"s);
    atxthrough = 1;
    goto label_1875;
}

void label_1877()
{
    atxinit();
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
    atxinfon(1) = u8"行動回数残り "s + atxap + u8"回\n"s;
    chatesc = 0;
    txtadvmsgfix = 0;
    txtadvscreenupdate = 1;
    casino_choose_card();
    casino_acquire_items();
    return;
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
    atxbg = u8"bg14"s;
    mattile = -1;
    atxinfon(0) = lang(
        u8"カジノ《フォーチュンクッキー》"s, u8"Casino <<Fortune Cookie>>"s);
    atxinit();
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
    atxinfon(1) = lang(
        u8"カジノチップ残り "s + mat(1) + u8"枚\n"s,
        u8"Casino chips left: "s + mat(1) + u8"\n"s);
    atxinfon(2) = "";
    list(0, listmax) = 0;
    listn(0, listmax) = lang(u8"店を出る"s, u8"Later."s);
    ++listmax;
    list(0, listmax) = 1;
    listn(0, listmax) =
        lang(u8"ブラックジャック"s, u8"I want to play Blackjack."s);
    ++listmax;
    chatesc = 0;
    txtadvmsgfix = 0;
    txtadvscreenupdate = 1;
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
    int stake = 0;
    int winrow = 0;
    int cardround = 0;
    int winner = 0;
    atxinit();
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
    atxinfon(1) = lang(
        u8"カジノチップ残り "s + mat(1) + u8"枚\n"s,
        u8"Casino chips left: "s + mat(1) + u8"\n"s);
    atxinfon(2) = "";
    if (mat(1) <= 0)
    {
        noteadd(""s);
        noteadd(lang(
            u8"お客様はチップをもっていません。"s,
            u8"Sorry sir, you don't seem to have casino chips."s));
    }
    list(0, listmax) = 0;
    listn(0, listmax) = lang(u8"やめる"s, u8"I quit."s);
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
    txtadvmsgfix = 0;
    txtadvscreenupdate = 1;
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
        atxinit();
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
            nojoker = 1;
            initcard(60, 160);
            cardplayerinit(2, 5);
            cardplayeradd(0, 220, 124);
            cardplayeradd(1, 220, 240);
        }
        font(14 - en * 2, snail::font_t::style_t::bold);
        color(255, 255, 255);
        pos(152, 154);
        mes(lang(u8"　親"s, u8"Dealer"s));
        pos(152, 270);
        mes(lang(u8"あなた"s, u8"   You"s));
        color(0, 0, 0);
        showcardpile();
        showcardholder();
        showcard();
        if (cardround == 0)
        {
            for (int cnt = 0; cnt < 4; ++cnt)
            {
                int stat = servecard(cnt % 2);
                if (cnt != 2)
                {
                    opencard2(stat);
                }
            }
            for (int cnt = 0; cnt < 1; ++cnt)
            {
                if (cpcardnum(0) >= 5)
                {
                    break;
                }
                if (cpscore(0) < cpscore(1) || cpscore(0) - rnd(5) <= 11)
                {
                    int stat = servecard(0);
                    opencard2(stat);
                    --cnt;
                    continue;
                }
            }
        }
        noteadd(lang(
            u8"あなたの合計は"s + cpscore(1) + u8"です。"s,
            u8"Your hand is "s + cpscore(1) + u8"."s));
        if (cardround == -1)
        {
            noteadd(lang(
                u8"親の合計は"s + cpscore(0) + u8"です。"s,
                u8"The dealer's hand is "s + cpscore(0) + u8"."s));
            winner = -1;
            if (cpscore(0) <= 21)
            {
                if (cpscore(1) > 21 || cpscore(0) > cpscore(1))
                {
                    winner = 0;
                }
            }
            if (cpscore(1) <= 21)
            {
                if (cpscore(0) > 21 || cpscore(1) > cpscore(0))
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
        atxinfon(1) = lang(
            u8"カジノチップ残り "s + mat(1) + u8"枚\n"s,
            u8"Casino chips left: "s + mat(1) + u8"\n"s);
        atxinfon(2) = lang(
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
            txtadvmsgfix = 240;
            txtadvscreenupdate = 0;
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
        if (pileremain() > 10)
        {
            if (cpcardnum(1) < 5)
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
        txtadvmsgfix = 240;
        txtadvscreenupdate = 0;
        casino_choose_card();
        if (rtval == 0)
        {
            opencard2(cpblackcard(0));
            cardround = -1;
            continue;
        }
        if (rtval == 1)
        {
            int stat = servecard(1);
            p = stat;
            for (int cnt = 0; cnt < 3; ++cnt)
            {
                if (cpscore(1) >= 22)
                {
                    if (pileremain() > 10)
                    {
                        if (rnd(sdata(19, 0)) > 40)
                        {
                            txt(lang(
                                u8"このカードは悪い予感がする…"s,
                                u8"I have a bad feeling about this card..."s));
                            trashcard(p);
                            int stat = servecard(1);
                            p = stat;
                        }
                    }
                }
            }
            opencard2(p);
        }
        if (rtval == 2)
        {
            trashcard(lastcard(1));
            int stat = servecard(1);
            opencard2(stat);
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
                atxinit();
                noteadd(lang(u8"イカサマだ！"s, u8"Cheater!"s));
                atxinfon(1) = lang(
                    u8"カジノチップ残り "s + mat(1) + u8"枚\n"s,
                    u8"Casino chips left: "s + mat(1) + u8"\n"s);
                atxinfon(2) = lang(
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
                txtadvmsgfix = 0;
                txtadvscreenupdate = 1;
                casino_choose_card();
                break;
            }
        }
        ++cardround;
    }
    if (winrow > 0)
    {
        atxinit();
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
        txtadvmsgfix = 0;
        txtadvscreenupdate = 1;
        casino_choose_card();
        winrow = 0;
    }
    return true;
}

} // namespace elona
