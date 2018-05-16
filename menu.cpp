#include "menu.hpp"
#include "ability.hpp"
#include "audio.hpp"
#include "calc.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "class.hpp"
#include "command.hpp"
#include "config.hpp"
#include "draw.hpp"
#include "enchantment.hpp"
#include "equipment.hpp"
#include "i18n.hpp"
#include "input.hpp"
#include "item.hpp"
#include "item_db.hpp"
#include "macro.hpp"
#include "menu.hpp"
#include "quest.hpp"
#include "trait.hpp"
#include "ui.hpp"
#include "variables.hpp"

namespace elona
{

std::string strhint6;

void text_set()
{
    if (jp)
    {
        strhint1 = u8"ｶｰｿﾙ [選択]  "s;
        strhint2 =
            ""s + key_pageup + u8","s + key_pagedown + u8" [ページ切替]  "s;
        strhint3 = u8"Shift,Esc [閉じる]  "s;
        strhint3b = u8"Shift,Esc [戻る]  "s;
        strhint4 = u8"決定、"s;
        strhint5 = ""s + key_identify + u8" [既知の情報]  "s;
        strhint5b = ""s + key_mode + u8" [情報切替]  "s;
        strhint6 = u8"p [ポートレイト変更]  "s;
        strhint7 = u8"0〜9 [ショートカット]  "s;
    }
    else
    {
        strhint1 = u8"Cursor [Select]  "s;
        strhint2 = ""s + key_pageup + u8","s + key_pagedown + u8" [Page]  "s;
        strhint3 = u8"Shift,Esc [Close]  "s;
        strhint3b = u8"Shift,Esc [Back]  "s;
        strhint4 = u8"Enter,"s;
        strhint5 = ""s + key_identify + u8" [Known info]  "s;
        strhint5b = ""s + key_mode + u8" [Mode]  "s;
        strhint6 = u8"p [Portrait]  "s;
        strhint7 = u8"0~9 [Shortcut]  "s;
    }
    SDIM4(_melee, 20, 3, 8);
    if (jp)
    {
        _melee(0, 0) = u8"殴って"s;
        _melee(1, 0) = u8"殴られた。"s;
        _melee(2, 0) = u8"手"s;
    }
    else
    {
        _melee(0, 0) = u8"punch"s;
        _melee(1, 0) = u8"punch"s;
        _melee(2, 0) = u8"hand"s;
    }
    if (jp)
    {
        _melee(0, 3) = u8"噛み付いて"s;
        _melee(1, 3) = u8"噛み付かれた。"s;
        _melee(2, 3) = u8"牙"s;
    }
    else
    {
        _melee(0, 3) = u8"bite"s;
        _melee(1, 3) = u8"bite"s;
        _melee(2, 3) = u8"fang"s;
    }
    if (jp)
    {
        _melee(0, 4) = u8"睨んで"s;
        _melee(1, 4) = u8"睨まれた。"s;
        _melee(2, 4) = u8"眼"s;
    }
    else
    {
        _melee(0, 4) = u8"gaze"s;
        _melee(1, 4) = u8"gaze"s;
        _melee(2, 4) = u8"eye"s;
    }
    if (jp)
    {
        _melee(0, 1) = u8"引っ掻き"s;
        _melee(1, 1) = u8"引っ掻かれた。"s;
        _melee(2, 1) = u8"爪"s;
    }
    else
    {
        _melee(0, 1) = u8"claw"s;
        _melee(1, 1) = u8"claw"s;
        _melee(2, 1) = u8"claw"s;
    }
    if (jp)
    {
        _melee(0, 5) = u8"刺し"s;
        _melee(1, 5) = u8"刺された。"s;
        _melee(2, 5) = u8"針"s;
    }
    else
    {
        _melee(0, 5) = u8"sting"s;
        _melee(1, 5) = u8"sting"s;
        _melee(2, 5) = u8"needle"s;
    }
    if (jp)
    {
        _melee(0, 6) = u8"触って"s;
        _melee(1, 6) = u8"触られた。"s;
        _melee(2, 6) = u8"手"s;
    }
    else
    {
        _melee(0, 6) = u8"touch"s;
        _melee(1, 6) = u8"touch"s;
        _melee(2, 6) = u8"hand"s;
    }
    if (jp)
    {
        _melee(0, 7) = u8"胞子を撒き散らし"s;
        _melee(1, 7) = u8"胞子を飛ばされた。"s;
        _melee(2, 7) = u8"胞子"s;
    }
    else
    {
        _melee(0, 7) = u8"attack"s;
        _melee(1, 7) = u8"attack"s;
        _melee(2, 7) = u8"spore"s;
    }
    randcolor(0) = 0;
    randcolor(1) = 4;
    randcolor(2) = 2;
    randcolor(3) = 5;
    randcolor(4) = 6;
    _randcolor(0) = 0;
    _randcolor(1) = 2;
    _randcolor(2) = 4;
    _randcolor(3) = 5;
    _randcolor(4) = 6;
    _randcolor(5) = 3;
    homepage = lang(
        u8"http://homepage3.nifty.com/rfish/index.html"s,
        u8"http://homepage3.nifty.com/rfish/index_e.html"s);
    strblank = lang("", u8" "s);
}

void show_quick_menu()
{
    page = quickpage;
    listmax = 0;
    snd(5);
    cs = -1;
label_2698:
    listmax = 0;
    if (page == 3)
    {
        page = 2;
    }
    if (page == -1)
    {
        page = 0;
    }
    if (page == 0)
    {
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"ヘルプ"s, u8"Help"s);
        listn(1, listmax) = key_help;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"ログ"s, u8"Log"s);
        listn(1, listmax) = key_msglog;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"シート"s, u8"Chara"s);
        listn(1, listmax) = key_charainfo;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"日誌"s, u8"Journal"s);
        listn(1, listmax) = key_journal;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
    }
    if (page == 1)
    {
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"装備"s, u8"Wear"s);
        listn(1, listmax) = key_wear;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"休息"s, u8"Rest"s);
        listn(1, listmax) = key_rest;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"魔法"s, u8"Spell"s);
        listn(1, listmax) = key_cast;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"技能"s, u8"Skill"s);
        listn(1, listmax) = key_skill;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"射撃"s, u8"Fire"s);
        listn(1, listmax) = key_fire;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"掘る"s, u8"Dig"s);
        listn(1, listmax) = key_dig;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
    }
    if (page == 2)
    {
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"祈る"s, u8"Pray"s);
        listn(1, listmax) = key_pray;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"装填"s, u8"Ammo"s);
        listn(1, listmax) = key_ammo;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"干渉"s, u8"Interact"s);
        listn(1, listmax) = key_interact;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = lang(u8"体当り"s, u8"Bash"s);
        listn(1, listmax) = key_bash;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
        list(0, listmax) = listmax;
        listn(0, listmax) = "";
        listn(1, listmax) = u8"aaa"s;
        ++listmax;
    }
    s(0) = lang(u8"情報"s, u8"Info"s);
    s(1) = lang(u8"行動"s, u8"Action"s);
    s(2) = lang(u8"特殊"s, u8"Etc"s);
    s(3) = u8"null"s;
    p = page - 1;
    if (p == -1)
    {
        p = 3;
    }
    listn(0, 0) = s(p);
    listn(0, 4) = s(page);
    p = page + 1;
    if (p == 3)
    {
        p = 3;
    }
    listn(0, 8) = s(p);
    t = 0;
label_2699_internal:
    font(12 + sizefix - en * 2);
    tx = 50;
    ty = windowh - 255;
    x(0) = 25;
    x(1) = 50;
    x(2) = 50;
    x(3) = 100;
    x(4) = 100;
    x(5) = 100;
    x(6) = 150;
    x(7) = 150;
    x(8) = 175;
    y(0) = 50;
    y(1) = 15;
    y(2) = 85;
    y(3) = 0;
    y(4) = 50;
    y(5) = 100;
    y(6) = 15;
    y(7) = 85;
    y(8) = 50;
    s = u8"○コマンド"s;
    gmode(2);
    for (int cnt = 0; cnt < 9; ++cnt)
    {
        p = cnt;
        if (listn(0, p) == u8"null"s)
        {
            continue;
        }
        pos(x(cnt) + tx, y(cnt) + ty);
        if (cnt == 0 || cnt == 4 || cnt == 8)
        {
            gcopy(3, 360, 192, 48, 48);
        }
        else
        {
            gcopy(3, 360, 144, 48, 48);
        }
        gmode(
            5 - 1,
            -1,
            (t + cnt) % 10 * (t + cnt) % 10 * 12 * ((t + cnt) % 50 < 10));
        if (cs == cnt)
        {
            gmode(5, -1, -1, 140);
        }
        pos(x(cnt) + tx, y(cnt) + ty);
        if (cnt == 0 || cnt == 4 || cnt == 8)
        {
            gcopy(3, 360, 192, 48, 48);
        }
        else
        {
            gcopy(3, 360, 144, 48, 48);
        }
        gmode(2);
        s = listn(0, p);
        pos(x(cnt) + tx + 25 - strlen_u(s) * 3, y(cnt) + ty + 19);
        bmes(s, 255, 255, 255);
    }
    ++t;
    redraw();
    await(config::instance().wait1);
    key_check(2);
    if (key == key_north)
    {
        key = listn(1, 3);
        cs = 3;
    }
    if (key == key_south)
    {
        key = listn(1, 5);
        cs = 5;
    }
    if (key == key_west)
    {
        cs = 0;
    }
    if (key == key_east)
    {
        cs = 8;
    }
    if (key == key_northwest)
    {
        key = listn(1, 1);
        cs = 1;
    }
    if (key == key_northeast)
    {
        key = listn(1, 6);
        cs = 6;
    }
    if (key == key_southwest)
    {
        key = listn(1, 2);
        cs = 2;
    }
    if (key == key_southeast)
    {
        key = listn(1, 7);
        cs = 7;
    }
    if (key == key_cancel)
    {
        cs = -1;
        snd(5);
        key = "";
        goto label_2700_internal;
        return;
    }
    if (key != ""s)
    {
        if (cs == -1)
        {
            snd(5);
            key = "";
            goto label_2700_internal;
            return;
        }
        if (key == key_enter)
        {
            key = listn(1, cs);
            goto label_2700_internal;
            return;
        }
    }
    if (key == ""s || cs == 1 || cs == 6 || cs == 2 || cs == 7)
    {
        if (cs != -1)
        {
            key = listn(1, cs);
            goto label_2700_internal;
            return;
        }
    }
    goto label_2699_internal;
label_2700_internal:
    if (cs == 0 || key == key_northeast)
    {
        cs = -1;
        snd(5);
        --page;
        screenupdate = -1;
        update_screen();
        goto label_2698;
    }
    if (cs == 8 || key == key_northwest)
    {
        cs = -1;
        snd(5);
        ++page;
        screenupdate = -1;
        update_screen();
        goto label_2698;
    }
    quickpage = page;
    cs = 0;
    update_screen();
    quickkeywait = 1;
    return;
}

void show_ex_help()
{
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_help.bmp", 1);
    gsel(0);
    page = 0;
    notesel(buff);
    {
        buff(0).clear();
        std::ifstream in{(filesystem::dir::data() / u8"exhelp.txt").native(),
                         std::ios::binary};
        std::string tmp;
        while (std::getline(in, tmp))
        {
            buff(0) += tmp + '\n';
        }
    }
    p = instr(buff, 0, u8"%"s + ghelp + u8","s + lang(u8"JP"s, u8"EN"s));
    if (p == -1)
    {
        dialog(
            u8"help index not found %"s + ghelp + u8","s
            + lang(u8"JP"s, u8"EN"s));
        return;
    }
    buff = strmid(buff, p, instr(buff, p, u8"%END"s));
    notedel(0);
    if (noteinfo() == 0)
    {
        return;
    }
    snd(53);
    while (1)
    {
        gmode(2);
        dx = 480;
        dy = 175;
        window2((windoww - dx) / 2 + inf_screenx, winposy(dy), dx, dy, 4, 0);
        wx = (windoww - dx) / 2 + inf_screenx;
        wy = winposy(dy);
        window2(
            (windoww - 325) / 2 + inf_screenx, winposy(dy) + 6, 325, 32, 0, 1);
        pos(wx + 5, wy + 4);
        gcopy(3, 960, 96, 48, 48);
        pos(wx + dx - 55, wy + 4);
        gcopy(3, 960, 96, 48, 48);
        pos(wx + 10, wy + 42);
        gcopy(3, 960, 144, 96, 120);
        font(16 - en * 2, snail::font_t::style_t::bold);
        pos(wx + 142, wy + 13);
        color(80, 60, 50);
        bmes(
            lang(u8"- ノルンの冒険ガイド -"s, u8"- Norne's travel guide -"s),
            255,
            245,
            235);
        color(0, 0, 0);
        tx = wx + 120;
        ty = wy + 55;
        font(15 - en * 2);
        {
            int y = ty;
            int cnt = 0;
            for (int cnt_end = cnt + (10); cnt < cnt_end; ++cnt)
            {
                noteget(s, page);
                ++page;
                if (page > noteinfo() || s == ""s)
                {
                    break;
                }
                color(30, 30, 30);
                const auto ny = gmes(s, tx, y, 330, {30, 30, 30}, true).y;
                color(0, 0, 0);
                y = ny;
            }
        }
        gmode(2);
        redraw();
        help_halt();
        if (page >= noteinfo())
        {
            break;
        }
    }
    return;
}

void label_2702()
{
    if (jp)
    {
        return;
    }
    for (int cnt = 0; cnt < 24; ++cnt)
    {
        if (!strutil::contains(s(cnt), u8"("))
        {
            continue;
        }
        s(cnt) = cnven(strmid(
            s(cnt),
            instr(s(cnt), 0, u8"("s) + 1,
            instr(s(cnt), 0, u8")"s) - instr(s(cnt), 0, u8"("s) - 1));
    }
    return;
}

void show_game_help()
{
    snd(26);
    listmax = 0;
    page = 0;
    pagesize = 18;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    page_bk = 0;
    cs_bk2 = 0;
    if (mode != 1)
    {
        gsel(4);
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            pos(cnt % 4 * 180, cnt / 4 * 300);
            picload(
                filesystem::dir::graphic() / (u8"g"s + (cnt + 1) + u8".bmp"),
                1);
        }
        gsel(0);
    }
    else
    {
        cs = 3;
        cs_bk2 = cs;
    }
    notesel(buff);
    {
        buff(0).clear();
        std::ifstream in{(filesystem::dir::data()
                          / lang(u8"manual_JP.txt", u8"manual_ENG.txt"))
                             .native(),
                         std::ios::binary};
        std::string tmp;
        while (std::getline(in, tmp))
        {
            buff(0) += tmp + '\n';
        }
    }
    list(0, 0) = 0;
    listn(0, 0) = lang(u8"キーの一覧"s, u8"Key List"s);
    ++listmax;
    for (int cnt = 0, cnt_end = (noteinfo()); cnt < cnt_end; ++cnt)
    {
        noteget(q, cnt);
        p = instr(q, 0, u8"{}"s);
        if (p != -1)
        {
            list(0, listmax) = cnt + 1;
            listn(0, listmax) = strmid(q, instr(q, 0, u8"}"s) + 2, 999);
            ++listmax;
        }
    }
    windowshadow = 1;
label_2704_internal:
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
label_2705_internal:
    s(0) = u8"Elona In-Game Help"s;
    s(1) = strhint2 + strhint3b;
    if (mode == 1)
    {
        display_window(
            (windoww - 780) / 2 + inf_screenx, winposy(496, 1), 780, 496);
    }
    else
    {
        display_window(
            (windoww - 780) / 2 + inf_screenx, winposy(496) - 24, 780, 496);
    }
    display_topic(lang(u8"項目"s, u8"Topic"s), wx + 34, wy + 36);
    if (mode == 1)
    {
        p = 2;
    }
    else
    {
        p = 4;
    }
    cmbg = page % 5;
    x = ww / 5 * 2;
    y = wh - 80;
    gmode(4, 180, 300, 50);
    pos(wx + ww / 4, wy + wh / 2);
    grotate(p, cmbg % 4 * 180, cmbg / 4 % 2 * 300, 0, x, y);
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
        display_key(wx + 38, wy + 66 + cnt * 19 - 2, cnt);
    }
    if (cs_bk2 == 0 && page_bk == 0)
    {
        x = wx + 188;
        y = wy + 6;
        display_topic(
            lang(u8"アイテムに関するキー"s, u8"Item Keys"s), x + 18, y + 30);
        display_topic(
            lang(u8"行動に関するキー"s, u8"Action Keys"s), x + 18, y + 142);
        display_topic(
            lang(u8"情報に関するキー"s, u8"Information Keys"s),
            x + 18,
            y + 256);
        display_topic(
            lang(u8"その他のキー"s, u8"Other Keys"s), x + 18, y + 328);
        s(0) = u8"アイテムを取る(get)"s;
        s(1) = key_get;
        s(2) = u8"アイテムを置く(drop)"s;
        s(3) = key_drop;
        s(4) = u8"アイテムを調べる(examine)"s;
        s(5) = key_inventory;
        s(6) = u8"装備する(wear,wield)"s;
        s(7) = key_wear;
        s(8) = u8"食べる(eat)"s;
        s(9) = key_eat;
        s(10) = u8"飲む(quaff)"s;
        s(11) = key_drink;
        s(12) = u8"読む(read)"s;
        s(13) = key_read;
        s(14) = u8"振る(zap)"s;
        s(15) = key_zap;
        s(16) = u8"道具を使う(tool)"s;
        s(17) = key_use;
        s(18) = u8"調合(blend)"s;
        s(19) = key_dip;
        s(20) = u8"投げる(throw)"s;
        s(21) = key_throw;
        s(22) = u8"装填(ammo)"s;
        s(23) = key_ammo;
        label_2702();
        for (int cnt = 0; cnt < 12; ++cnt)
        {
            font(13 - en * 2);
            pos(x + 38 + cnt / 6 * 290, y + 58 + cnt % 6 * 14);
            mes(s(cnt * 2));
            font(15 - en * 2);
            pos(x + 248 + cnt / 6 * 290, y + 57 + cnt % 6 * 14);
            mes(s(cnt * 2 + 1));
        }
        s(0) = u8"周囲を調べる(search)"s;
        s(1) = key_search;
        s(2) = u8"魔法を唱える(cast)"s;
        s(3) = key_cast;
        s(4) = u8"干渉する(interact)"s;
        s(5) = key_interact;
        s(6) = u8"階段を降りる,入る(go down)"s;
        s(7) = key_godown;
        s(8) = u8"階段を昇る(go up)"s;
        s(9) = key_goup;
        s(10) = u8"その場で待機(wait)"s;
        s(11) = key_wait;
        s(12) = u8"ターゲットを指定(target)"s;
        s(13) = key_target;
        s(14) = u8"射撃する(fire)"s;
        s(15) = key_fire;
        s(16) = u8"能力を使う(apply)"s;
        s(17) = key_skill;
        s(18) = u8"体当たりする(bash)"s;
        s(19) = key_bash;
        s(20) = u8"穴を掘る(dig)"s;
        s(21) = key_dig;
        s(22) = u8"鍵を開ける(open)"s;
        s(23) = key_open;
        s(24) = "";
        s(25) = "";
        label_2702();
        for (int cnt = 0; cnt < 12; ++cnt)
        {
            font(13 - en * 2);
            pos(x + 38 + cnt / 6 * 290, y + 170 + cnt % 6 * 14);
            mes(s(cnt * 2));
            font(15 - en * 2);
            pos(x + 248 + cnt / 6 * 290, y + 169 + cnt % 6 * 14);
            mes(s(cnt * 2 + 1));
        }
        s(0) = u8"能力・スキル情報(chara)"s;
        s(1) = key_charainfo;
        s(2) = u8"冒険日誌を表示(journal)"s;
        s(3) = key_journal;
        s(4) = u8"ヘルプを表示(help)"s;
        s(5) = key_help;
        s(6) = u8"メッセージログを表示(log)"s;
        s(7) = key_msglog;
        s(8) = u8"マテリアル表示(material)"s;
        s(9) = key_material;
        s(10) = u8"特徴の表示(feat)"s;
        s(11) = key_trait;
        label_2702();
        for (int cnt = 0; cnt < 6; ++cnt)
        {
            font(13 - en * 2);
            pos(x + 38 + cnt / 3 * 290, y + 284 + cnt % 3 * 14);
            mes(s(cnt * 2));
            font(15 - en * 2);
            pos(x + 248 + cnt / 3 * 290, y + 283 + cnt % 3 * 14);
            mes(s(cnt * 2 + 1));
        }
        s(0) = u8"セーブして終了(save)"s;
        s(1) = key_save;
        s(2) = u8"神に祈る(pray)"s;
        s(3) = key_pray;
        s(4) = u8"神に捧げる(offer)"s;
        s(5) = key_offer;
        s(6) = u8"ドアを閉める(close)"s;
        s(7) = key_close;
        s(8) = u8"与える(give)"s;
        s(9) = key_give;
        s(10) = "";
        s(11) = "";
        s(12) = "";
        label_2702();
        for (int cnt = 0; cnt < 6; ++cnt)
        {
            font(13 - en * 2);
            pos(x + 38 + cnt / 3 * 290, y + 356 + cnt % 3 * 14);
            mes(s(cnt * 2));
            font(15 - en * 2);
            pos(x + 248 + cnt / 3 * 290, y + 355 + cnt % 3 * 14);
            mes(s(cnt * 2 + 1));
        }
        font(13 - en * 2);
        pos(x + 38, y + 408);
        mes(lang(
            u8"F9 インタフェース非表示  F11 キャラ情報の出力  F12 コンソールの表示"s,
            u8"F9 Hide interface  F11 Export chara sheet  F12 Console"s));
    }
    else
    {
        s(1) = listn(0, pagesize * page_bk + cs_bk2);
        display_topic(s(1), wx + 206, wy + 36);
        font(14 - en * 2);
        p = list(0, pagesize * page_bk + cs_bk2);
        {
            int y = wy + 60;
            int cnt = p;
            for (int cnt_end = cnt + (noteinfo() - p); cnt < cnt_end; ++cnt)
            {
                noteget(s1, cnt);
                i = instr(s1, 0, u8"{"s);
                if (i != -1)
                {
                    break;
                }
                const auto ny =
                    gmes(s1, wx + 216, y, 510, {30, 30, 30}, false).y;
                y = ny;
            }
        }
    }
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
        s = listn(0, p);
        cs_list(cs == cnt, s, wx + 66, wy + 66 + cnt * 19 - 1, 0);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, cs = i);
    if (p != -1)
    {
        snd(20);
        page_bk = page;
        cs_bk2 = cs;
        goto label_2704_internal;
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2704_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2704_internal;
        }
    }
    if (key == key_cancel)
    {
        return;
    }
    goto label_2705_internal;
}

turn_result_t show_chat_history()
{
    curmenu = 2;
    key_list(0) = key_enter;
    keyrange = 0;
    pagesize = 0;
    gsel(7);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"ie_scroll.bmp");
    gsel(0);
    windowshadow = 1;
    snd(92);
    drawmenu(2);
    ww = clamp(windoww - 90, windoww - 90, 720);
    wh = 440;
    wx = (windoww - ww) / 2 + inf_screenx;
    wy = winposy(wh);
    windowanime(wx, wy, ww, wh, 10, 4);
    s = lang(u8"何かキーを押すと閉じる"s, u8"Hit any key to close"s);
    showscroll(s, wx, wy, ww, wh);
    net_read();
    buff = "";
    notesel(buff);
    header = instr(netbuf, 0, u8"<!--START-->"s) + 13;
    while (1)
    {
        s = "";
        tail = instr(netbuf, header, u8"%"s);
        if (tail == -1)
        {
            break;
        }
        header += tail + 1;
        tail = instr(netbuf, header, u8"%"s);
        s += strmid(netbuf, header, tail) + u8"  "s;
        header += tail + 1;
        tail = instr(netbuf, header, u8"%"s);
        s += strmid(netbuf, header + 4, tail - 4);
        header += tail + 1;
        tail = instr(netbuf, header, u8"%"s);
        header += tail + 2;
        s += u8"\n"s;
        buff += ""s + s;
    }
    font(13 - en * 2);
    i = 0;
    for (int cnt = 0; cnt < 20; ++cnt)
    {
        if (i >= 20)
        {
            break;
        }
        noteget(s, cnt);
        if (en)
        {
            s = strutil::replace(s, u8"&quot;", u8"\"");
        }
        if (s == ""s)
        {
            if (cnt == 0)
            {
                s = u8"No new messages received."s;
            }
            break;
        }
        i += talk_conv(s, (ww - 110 - en * 50) / 7);
        pos(wx + 48, (19 - i) * 16 + wy + 48);
        color(30, 20, 10);
        mes(s);
        color(0, 0, 0);
        ++i;
    }
    redraw();
label_2708_internal:
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 2)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 2;
                }
            }
            key = "";
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    return turn_result_t::show_message_log;
                }
                if (curmenu == 1)
                {
                    return turn_result_t::menu_journal;
                }
                if (curmenu == 2)
                {
                    return turn_result_t::show_chat_history;
                }
            }
        }
    }
    if (key != ""s)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    goto label_2708_internal;
}

turn_result_t show_message_log()
{
    curmenu = 0;
    windowshadow = 1;
    key_list(0) = key_enter;
    keyrange = 0;
    pagesize = 0;
    wx = inf_msgx - 2;
    wy = inf_msgy - (inf_maxlog - 3) * inf_msgspace - 1;
    ww = windoww - inf_msgx + 6;
    wh(0) = (inf_maxlog - 3) * inf_msgspace;
    wh(1) = 1;
    wh(2) = -1;
    snd(93);
    showtitle(
        lang(u8"過去のメッセージ"s, u8"Log"s),
        lang(u8"何かキーを押すと閉じる"s, u8"Hit any key to close"s),
        236,
        1);
    drawmenu(2);
    windowanimecorner(wx, wy, ww, wh, 8, 4);

    p = (windoww - inf_msgx) / 192;
    window2(wx, wy, ww, wh, 1, -1);
    for (int cnt = 0, cnt_end = (inf_maxlog - 3); cnt < cnt_end; ++cnt)
    {
        int cnt2 = cnt;
        pos(cnt);
        for (int cnt = 0, cnt_end = (p + 1); cnt < cnt_end; ++cnt)
        {
            if (cnt == p)
            {
                x = (windoww - inf_msgx) % 192;
            }
            else
            {
                x = 192;
            }
            pos(cnt * 192 + inf_msgx, inf_msgy - (cnt2 + 1) * inf_msgspace);
            gcopy(3, 496, 536 + cnt2 % 4 * inf_msgspace, x, inf_msgspace);
        }
    }
    for (int cnt = 0, cnt_end = (p + 1); cnt < cnt_end; ++cnt)
    {
        if (cnt == p)
        {
            sx = (windoww - inf_msgx) % 192;
        }
        else
        {
            sx = 192;
        }
        pos(cnt * 192 + inf_msgx, inf_msgy);
        gcopy(3, 496, 528, sx, 6);
    }
    gsel(4);
    gmode(0);
    boxf();
    for (int cnt = 0, cnt_end = (inf_maxlog - 3); cnt < cnt_end; ++cnt)
    {
        p = msgline - cnt - 3;
        if (p < 0)
        {
            p += inf_maxlog;
        }
        else if (p >= inf_maxlog)
        {
            p -= inf_maxlog;
        }
        if (p < 0)
        {
            continue;
        }
        pos(inf_msgx, inf_msgy - cnt * inf_msgspace);
        gcopy(8, 0, p * inf_msgspace, windoww - inf_msgx, inf_msgspace);
    }
    gsel(0);
    gmode(2);
    pos(0, -3);
    gcopy(4, 0, 0, windoww, inf_msgy);

    redraw();
label_2711_internal:
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 2)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 2;
                }
            }
            key = "";
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    return turn_result_t::show_message_log;
                }
                if (curmenu == 1)
                {
                    return turn_result_t::menu_journal;
                }
                if (curmenu == 2)
                {
                    return turn_result_t::show_chat_history;
                }
            }
        }
    }
    if (key != ""s)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    goto label_2711_internal;
}

void initialize_jkey()
{
    SDIM3(jkey, 2, 12);
    return;
}

void label_2720()
{
    notesel(headtemp);
    noteget(s, 1);
    username = ""s + s;
    noteget(s, 5);
    usermsg = ""s + s;
    noteget(s, 6);
    userrelation = elona::stoi(s(0));
    return;
}

int cnvjkey(const std::string& prm_1092)
{
    int p_at_m198 = 0;
    p_at_m198 = -2;
    for (int cnt = 0; cnt < 12; ++cnt)
    {
        if (prm_1092 == jkey(cnt))
        {
            p_at_m198 = cnt;
            break;
        }
    }
    return p_at_m198 + 1;
}

turn_result_t play_scene()
{
    snd(59);
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"book.bmp", 1);
    gsel(7);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"g1.bmp", 0);
    gsel(0);
    listmax = 0;
    page = 0;
    pagesize = 12;
    cs = 0;
    cc = 0;
    notesel(buff);
    {
        buff(0).clear();
        std::ifstream in{lang(u8"scene1.hsp"s, u8"scene2.hsp"s),
                         std::ios::binary};
        std::string tmp;
        while (std::getline(in, tmp))
        {
            buff(0) += tmp + '\n';
        }
    }
    p = 0;
    for (int cnt = 0; cnt < 200; ++cnt)
    {
        s = u8"{"s + cnt + u8"}"s;
        p = instr(buff, 0, s);
        if (p != -1)
        {
            if (cnt <= gdata_played_scene)
            {
                list(0, listmax) = cnt;
                list(1, listmax) = p;
                ++listmax;
            }
        }
    }
    txt(lang(
        u8"どのシーンを再生する？"s, u8"Which scene do you want to replay?"s));
label_2728_internal:
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
    wx = (windoww - 720) / 2 + inf_screenx;
    wy = winposy(468);
label_2729_internal:
    pos(wx, wy);
    gcopy(4, 0, 0, 736, 448);
    x = 240;
    y = 320;
    gmode(4, 180, 300, 100);
    pos(wx + 190, wy + 220);
    grotate(7, 0, 0, 0, x, y);
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
        display_key(wx + 394, wy + 91 + cnt * 22 - 2, cnt);
    }
    font(
        12 - en * 2,
        snail::font_t::style_t::italic | snail::font_t::style_t::underline);
    pos(wx + 90, wy + 50);
    mes(u8"Elona - Scene playback"s);
    font(12 - en * 2);
    pos(wx + 390, wy + 50);
    mes(lang(
        u8"アンロックされたシーンを再生できます。\nシーンNoは連続していません。"s,
        u8"You can play the unlocked scenes."s));
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
        cs_list(
            cs == cnt,
            lang(u8"シーン No."s, u8"Scene No. "s) + i,
            wx + 424,
            wy + 91 + cnt * 22 - 1);
    }
    font(12 - en * 2, snail::font_t::style_t::bold);
    pos(wx + 500, wy + 375);
    mes(u8"- "s + (page + 1) + u8" -"s);
    if (page < pagemax)
    {
        pos(wx + 590, wy + 375);
        mes(u8"(more)"s);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        sceneid = p;
        do_play_scene();
        screenupdate = -1;
        update_entire_screen();
        txtnew();
        txt(lang(u8"シーンの再生を終えた。"s, u8"The scene has been played."s));
        return turn_result_t::play_scene;
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(71);
            ++page;
            goto label_2728_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(71);
            --page;
            goto label_2728_internal;
        }
    }
    if (key == key_cancel || key == key_enter)
    {
        play_music();
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    goto label_2729_internal;
}

turn_result_t show_spell_list()
{
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    cs = commark(1) % 1000;
    page = commark(1) / 1000;
    curmenu = 0;
    for (int cnt = 0; cnt < 200; ++cnt)
    {
        if (spell(cnt) > 0)
        {
            list(0, listmax) = cnt + 400;
            list(1, listmax) =
                the_ability_db[400 + cnt]->related_basic_attribute; // TODO coupling
            ++listmax;
        }
    }
    sort_list_by_column1();
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_spell.bmp", 1);
    gsel(0);
    windowshadow = 1;
label_2028_internal:
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
    drawmenu(1);
label_2029_internal:
    s(0) = lang(u8"魔法の詠唱"s, u8"Spell"s);
    s(1) = strhint2 + strhint3 + strhint7;
    display_window((windoww - 720) / 2 + inf_screenx, winposy(438), 720, 438);
    display_topic(lang(u8"魔法の名称"s, u8"Name"s), wx + 28, wy + 36);
    display_topic(
        lang(u8"消費MP(ｽﾄｯｸ) Lv/成功"s, u8"Cost(Stock) Lv/Chance"s),
        wx + 220,
        wy + 36);
    display_topic(lang(u8"効果"s, u8"Effect"s), wx + 400, wy + 36);
    pos(wx + 46, wy - 16);
    gcopy(3, 912, 48, 48, 48);
    pos(wx + ww - 78, wy);
    gcopy(3, 960, 96, 72, 144);
    pos(wx + ww - 180, wy);
    gcopy(3, 1032, 96, 72, 96);
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
        if (cnt % 2 == 0)
        {
            pos(wx + 70, wy + 66 + cnt * 19);
            gfini(620, 18);
            gfdec2(12, 14, 16);
        }
        display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
    }
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
        pos(wx + 40, wy + 74 + cnt * 19);
        gmode(2, inf_tiles, inf_tiles);
        grotate(
            1,
            (the_ability_db[i]->related_basic_attribute - 10) * inf_tiles,
            672,
            0,
            inf_tiles,
            inf_tiles);
        s = "";
        for (int cnt = 0; cnt < 20; ++cnt)
        {
            if (gdata(40 + cnt) == list(0, p))
            {
                s = u8"{"s + cnt + u8"}"s;
            }
        }
        cs_list(
            cs == cnt,
            i18n::_(u8"ability", std::to_string(i), u8"name") + s,
            wx + 84,
            wy + 66 + cnt * 19 - 1);
        s = ""s + calcspellcostmp(i, cc) + u8" ("s + spell((i - 400)) + u8")"s;
        pos(wx + 328 - strlen_u(s) * 7, wy + 66 + cnt * 19 + 2);
        mes(s);
        label_2031();
        s = strmid(s, 0, 40);
        pos(wx + 340, wy + 66 + cnt * 19 + 2);
        mes(""s + sdata(i, cc) + u8"/"s + calcspellfail(i, cc) + u8"%"s);
        pos(wx + 420, wy + 66 + cnt * 19 + 2);
        mes(s);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    commark(1) = page * 1000 + cs;
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        menucycle = 0;
        efid = p;
        return do_cast_command();
    }
    if (key == u8"sc"s)
    {
        snd(20);
        p = list(0, pagesize * page + cs);
        if (gdata(40 + sc) == p)
        {
            gdata(40 + sc) = 0;
            goto label_2028_internal;
        }
        for (int cnt = 0; cnt < 20; ++cnt)
        {
            if (gdata(40 + cnt) == p)
            {
                gdata(40 + cnt) = 0;
            }
        }
        gdata(40 + sc) = p;
        txt(lang(
            u8"{"s + sc + u8"}キーにショートカットを割り当てた。"s,
            u8"You have assigned the shortcut to {"s + sc + u8"} key."s));
        display_msg(inf_screeny + inf_tiles);
        goto label_2028_internal;
    }
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 1)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 1;
                }
            }
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    snd(102);
                    return turn_result_t::show_spell_list;
                }
                if (curmenu == 1)
                {
                    snd(101);
                    return turn_result_t::show_skill_list;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2028_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2028_internal;
        }
    }
    if (key == key_cancel)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    goto label_2029_internal;
}

void label_2031()
{
    s = "";
    if (the_ability_db[i]->sdataref1 / 1000 == 1)
    {
        p = the_ability_db[i]->sdataref1 % 1000;
        const auto duration = calc_buff_duration(p, calcspellpower(i, cc));
        const auto description = get_buff_description(p, calcspellpower(i, cc));
        s = ""s + duration + lang(u8"ﾀｰﾝ "s, u8"t "s) + description;
        return;
    }
    const auto damage = calc_skill_damage(i, cc, calcspellpower(i, cc));
    if (damage)
    {
        dice1 = damage->dice_x;
        dice2 = damage->dice_y;
        bonus = damage->damage_bonus;
        ele = damage->element;
        elep = damage->element_power;
        if (cc == 0)
        {
            if (trait(165) != 0)
            {
                if (ele == 50 || ele == 51 || ele == 52)
                {
                    dice2 = dice2 * 125 / 100;
                }
            }
        }
        if (dice1 != 0)
        {
            s += ""s + dice1 + u8"d"s + dice2;
            if (bonus != 0)
            {
                if (bonus > 0)
                {
                    s += u8"+"s + bonus;
                }
                else
                {
                    s += bonus;
                }
            }
        }
        else if (i == 461)
        {
            s += ""s + clamp(bonus, 1, 100) + u8"%"s;
        }
        else
        {
            s += lang(u8"ﾊﾟﾜｰ"s, u8"Power:"s) + bonus;
        }
        s += u8" "s;
    }
    s += i18n::_(u8"ability", std::to_string(i), u8"description");
    return;
}

void label_2032()
{
    int dbmax = 0;
    dblist(0, dbmax) = 159;
    ++dbmax;
    dblist(0, dbmax) = 173;
    ++dbmax;
    if (gdata_current_map == 12)
    {
        dblist(0, dbmax) = 154;
        ++dbmax;
        dblist(0, dbmax) = 162;
        ++dbmax;
        dblist(0, dbmax) = 184;
        ++dbmax;
        dblist(0, dbmax) = 176;
        ++dbmax;
        dblist(0, dbmax) = 177;
        ++dbmax;
        dblist(0, dbmax) = 180;
        ++dbmax;
        dblist(0, dbmax) = 301;
        ++dbmax;
        dblist(0, dbmax) = 167;
        ++dbmax;
        dblist(0, dbmax) = 153;
        ++dbmax;
    }
    if (gdata_current_map == 5)
    {
        dblist(0, dbmax) = 178;
        ++dbmax;
        dblist(0, dbmax) = 154;
        ++dbmax;
        dblist(0, dbmax) = 184;
        ++dbmax;
        dblist(0, dbmax) = 163;
        ++dbmax;
        dblist(0, dbmax) = 182;
        ++dbmax;
        dblist(0, dbmax) = 185;
        ++dbmax;
        dblist(0, dbmax) = 158;
        ++dbmax;
    }
    if (gdata_current_map == 15)
    {
        dblist(0, dbmax) = 151;
        ++dbmax;
        dblist(0, dbmax) = 150;
        ++dbmax;
        dblist(0, dbmax) = 161;
        ++dbmax;
        dblist(0, dbmax) = 179;
        ++dbmax;
        dblist(0, dbmax) = 301;
        ++dbmax;
        dblist(0, dbmax) = 156;
        ++dbmax;
        dblist(0, dbmax) = 160;
        ++dbmax;
        dblist(0, dbmax) = 172;
        ++dbmax;
        dblist(0, dbmax) = 178;
        ++dbmax;
    }
    if (gdata_current_map == 33)
    {
        dblist(0, dbmax) = 181;
        ++dbmax;
        dblist(0, dbmax) = 150;
        ++dbmax;
        dblist(0, dbmax) = 162;
        ++dbmax;
        dblist(0, dbmax) = 184;
        ++dbmax;
        dblist(0, dbmax) = 182;
        ++dbmax;
        dblist(0, dbmax) = 155;
        ++dbmax;
        dblist(0, dbmax) = 153;
        ++dbmax;
    }
    if (gdata_current_map == 36)
    {
        if (gdata_current_dungeon_level == 1)
        {
            dblist(0, dbmax) = 172;
            ++dbmax;
            dblist(0, dbmax) = 154;
            ++dbmax;
            dblist(0, dbmax) = 150;
            ++dbmax;
            dblist(0, dbmax) = 174;
            ++dbmax;
            dblist(0, dbmax) = 185;
            ++dbmax;
            dblist(0, dbmax) = 171;
            ++dbmax;
            dblist(0, dbmax) = 160;
            ++dbmax;
            dblist(0, dbmax) = 167;
            ++dbmax;
        }
    }
    if (gdata_current_map == 36)
    {
        if (gdata_current_dungeon_level == 3)
        {
            dblist(0, dbmax) = 172;
            ++dbmax;
            dblist(0, dbmax) = 165;
            ++dbmax;
            dblist(0, dbmax) = 155;
            ++dbmax;
            dblist(0, dbmax) = 164;
            ++dbmax;
            dblist(0, dbmax) = 178;
            ++dbmax;
            dblist(0, dbmax) = 188;
            ++dbmax;
            dblist(0, dbmax) = 171;
            ++dbmax;
            dblist(0, dbmax) = 187;
            ++dbmax;
        }
    }
    if (gdata_current_map == 14)
    {
        if (gdata_current_dungeon_level == 1)
        {
            dblist(0, dbmax) = 158;
            ++dbmax;
            dblist(0, dbmax) = 157;
            ++dbmax;
            dblist(0, dbmax) = 170;
            ++dbmax;
            dblist(0, dbmax) = 171;
            ++dbmax;
            dblist(0, dbmax) = 176;
            ++dbmax;
            dblist(0, dbmax) = 174;
            ++dbmax;
            dblist(0, dbmax) = 183;
            ++dbmax;
            dblist(0, dbmax) = 163;
            ++dbmax;
        }
    }
    if (gdata_current_map == 14)
    {
        if (gdata_current_dungeon_level == 3)
        {
            dblist(0, dbmax) = 300;
            ++dbmax;
            dblist(0, dbmax) = 175;
            ++dbmax;
            dblist(0, dbmax) = 158;
            ++dbmax;
            dblist(0, dbmax) = 157;
            ++dbmax;
            dblist(0, dbmax) = 189;
            ++dbmax;
        }
    }
    if (gdata_current_map == 11)
    {
        if (gdata_current_dungeon_level == 1)
        {
            dblist(0, dbmax) = 156;
            ++dbmax;
            dblist(0, dbmax) = 160;
            ++dbmax;
            dblist(0, dbmax) = 183;
            ++dbmax;
            dblist(0, dbmax) = 185;
            ++dbmax;
            dblist(0, dbmax) = 174;
            ++dbmax;
            dblist(0, dbmax) = 151;
            ++dbmax;
        }
    }
    if (gdata_current_map == 11)
    {
        if (gdata_current_dungeon_level == 3)
        {
            dblist(0, dbmax) = 153;
            ++dbmax;
            dblist(0, dbmax) = 166;
            ++dbmax;
            dblist(0, dbmax) = 167;
            ++dbmax;
            dblist(0, dbmax) = 169;
            ++dbmax;
            dblist(0, dbmax) = 152;
            ++dbmax;
            dblist(0, dbmax) = 189;
            ++dbmax;
            dblist(0, dbmax) = 168;
            ++dbmax;
            dblist(0, dbmax) = 186;
            ++dbmax;
        }
    }
    for (int cnt = 0, cnt_end = (dbmax); cnt < cnt_end; ++cnt)
    {
        p = dblist(0, cnt);
        f = 0;
        if (sdata.get(p, cc).original_level == 0)
        {
            if (the_ability_db[p]->related_basic_attribute != 0)
            {
                f = 1;
            }
        }
        if (f)
        {
            list(0, listmax) = p;
            list(1, listmax) =
                the_ability_db[p]->related_basic_attribute + 21000;
            ++listmax;
        }
    }
    return;
}

menu_result menu_character_sheet()
{
    menu_result result = { false, false, turn_result_t::none };
    int cs_buff = 0;
    int returnfromportrait = 0;
    int cs_buffmax = 0;
label_20331:
    page = 0;
    pagesize = 16;
    listmax = 0;
    cs = 0;
    if (csctrl != 4)
    {
        cc = 0;
    }
    csskill = -1;
    cs_buff = 0;
    curmenu = 0;
    if (csctrl == 2 || csctrl == 3)
    {
        page = 1;
    }
    list(0, listmax) = -1;
    list(1, listmax) = 20000;
    listn(0, listmax) = lang(u8"◆ スキルと特殊能力"s, u8"Skill"s);
    ++listmax;
    if (csctrl == 3)
    {
        label_2032();
    }
    else
    {
        for (int cnt = 150; cnt < 400; ++cnt)
        {
            f = 0;
            if (sdata(cnt, cc) != 0)
            {
                f = 1;
                if (csctrl == 2)
                {
                    if (sdata.get(cnt, cc).original_level == 0)
                    {
                        f = 0;
                    }
                }
            }
            if (f)
            {
                list(0, listmax) = cnt;
                list(1, listmax) =
                    the_ability_db[cnt]->related_basic_attribute + 21000;
                ++listmax;
            }
        }
    }
    list(0, listmax) = -1;
    list(1, listmax) = 30000;
    listn(0, listmax) = lang(u8"◆ 武器の専門"s, u8"Weapon Proficiency"s);
    ++listmax;
    for (int cnt = 100; cnt < 150; ++cnt)
    {
        f = 0;
        if (csctrl != 3)
        {
            if (sdata(cnt, cc) != 0)
            {
                f = 1;
            }
        }
        else if (sdata(cnt, cc) == 0)
        {
            if (the_ability_db[cnt])
            {
                f = 1;
            }
        }
        if (f)
        {
            list(0, listmax) = cnt;
            list(1, listmax) =
                the_ability_db[cnt]->related_basic_attribute + 31000;
            ++listmax;
        }
    }
    if (csctrl != 2 && csctrl != 3)
    {
        list(0, listmax) = -1;
        list(1, listmax) = 40000;
        listn(0, listmax) = lang(u8"◆ 耐性と抵抗力"s, u8"Resistance"s);
        ++listmax;
        for (int cnt = 50; cnt < 100; ++cnt)
        {
            if (sdata(cnt, cc) != 0)
            {
                list(0, listmax) = cnt;
                list(1, listmax) =
                    the_ability_db[cnt]->related_basic_attribute + 41000;
                ++listmax;
            }
        }
    }
    sort_list_by_column1();
    gsel(7);
    picload(filesystem::dir::graphic() / u8"ie_sheet.bmp");
    gsel(0);
    wx = (windoww - 700) / 2 + inf_screenx;
    wy = winposy(400) - 10;
    ww = 700;
    wh = 400;
    s = lang(u8"キャラクターシート"s, u8"Character Sheet"s);
    if (csctrl == 2)
    {
        s = lang(u8"能力訓練"s, u8"Skill Training"s);
    }
    if (csctrl == 3)
    {
        s = lang(u8"能力の習得"s, u8"Skill Learning"s);
    }
    drawmenu(0);
    if (mode != 1)
    {
        snd(94);
    }
    windowanime(wx, wy, ww, wh, 10, 4);
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"face1.bmp", 1);
    if (cdata[cc].portrait < 0) // TODO coupling
    {
        const auto filepath = filesystem::dir::user() / u8"graphic"
            / (u8"face"s + std::abs(cdata[cc].portrait + 1) + u8".bmp");
        if (cdata[cc].portrait != -1)
        {
            if (fs::exists(filepath))
            {
                pos(0, 0);
                picload(filepath, 1);
            }
        }
    }
    gsel(0);
    if (returnfromportrait == 0)
    {
        gmode(6, -1, -1, 80);
        pos(wx + 4, wy + 4);
        gcopy(7, 0, 0, 700, 400);
        gmode(2);
    }
    if (csctrl == 2)
    {
        txtnew();
        txt(lang(u8"どの能力を訓練する？"s, u8"Train which skill?"s));
    }
    returnfromportrait = 0;
label_2034_internal:
    if (csctrl != 1)
    {
        display_msg(inf_tiles + inf_screeny);
    }
    pagemax = (listmax - 1) / pagesize + 1;
    if (page < 0)
    {
        page = pagemax;
    }
    else if (page > pagemax)
    {
        page = 0;
    }
    if (csctrl == 1)
    {
        pagemax = 0;
    }
    cs_bk = -1;
    if (csctrl == 0)
    {
        if (page == 0)
        {
            s = lang(u8"ｶｰｿﾙ [祝福と呪いの情報]  "s, u8"Cursor [Hint]  "s)
                + strhint6 + strhint2 + strhint3;
        }
        else
        {
            s = lang(u8"決定 [ボーナスの分配]  "s, u8"Enter [Spend Bonus]  "s)
                + strhint2 + strhint3;
        }
    }
    if (csctrl == 1)
    {
        s = lang(
            u8"決定ｷｰ [リロール]  "s + strhint6 + u8"Shift,Esc [最終確認]"s,
            u8"Enter Key [Reroll]  "s + strhint6 + u8"Shift,Esc [Confirm]"s);
    }
    if (csctrl == 2)
    {
        if (page == 0)
        {
            s = strhint6 + strhint2 + strhint3;
        }
        else
        {
            s = lang(u8"決定 [スキルを訓練]  "s, u8"Enter [Train Skill]  "s)
                + strhint2 + strhint3;
        }
    }
    if (csctrl == 3)
    {
        if (page == 0)
        {
            s = strhint6 + strhint2 + strhint3;
        }
        else
        {
            s = lang(u8"決定 [スキルを習得]  "s, u8"Enter [Learn Skill]  "s)
                + strhint2 + strhint3;
        }
    }
    if (csctrl == 4)
    {
        if (page == 0)
        {
            s = u8"ｶｰｿﾙ [祝福と呪いの情報] "s + strhint6 + strhint2 + strhint3;
        }
        else
        {
            s = strhint2 + strhint3;
        }
    }
    if (csctrl != 1)
    {
        if (page != 0)
        {
            s += ""s + key_mode2 + u8" ["s
                + lang(u8"スキルトラック"s, u8"Track Skill"s) + u8"]"s;
        }
    }
    showtitle(s, s, 236, 1);
label_2035_internal:
    s = "";
    if (csctrl == 0)
    {
        if (page != 0)
        {
            s = lang(
                u8"残り "s + cdata[cc].skill_bonus
                    + u8" のボーナスをスキルに分配できる"s,
                u8"You can spend "s + cdata[cc].skill_bonus
                    + u8" bonus points."s);
        }
    }
    display_window2(
        (windoww - 700) / 2 + inf_screenx, winposy(400) - 10, 700, 400, 7);
    if (page == 0)
    {
        keyrange = 0;
        key_list = key_enter;
        display_topic(
            lang(
                u8"能力(元の値)  - 潜在能力"s,
                u8"Attributes(Org) - Potential"s),
            wx + 28,
            wy + 122);
        display_topic(
            lang(u8"各種修正"s, u8"Combat Rolls"s), wx + 400, wy + 253);
        display_topic(lang(u8"冒険の軌跡"s, u8"Trace"s), wx + 28, wy + 273);
        display_topic(
            lang(u8"祝福と呪い"s, u8"Blessing and Hex"s), wx + 400, wy + 122);
        display_topic(lang(u8"その他"s, u8"Extra Info"s), wx + 220, wy + 273);
        if (cdata[cc].portrait >= 0)
        {
            p = cdata[cc].sex * 64 + cdata[cc].portrait;
            pos(wx + 560, wy + 27);
            gzoom(4, p % 16 * 48, p / 16 * 72, 48, 72, 80, 112);
        }
        else
        {
            const auto filepath = filesystem::dir::user() / u8"graphic"
                / (u8"face"s + std::abs(cdata[cc].portrait + 1) + u8".bmp");
            if (cdata[cc].portrait != -1)
            {
                if (fs::exists(filepath))
                {
                    pos(wx + 560, wy + 27);
                    gzoom(4, 0, 0, 80, 112, 80, 112);
                }
            }
        }
        window2(wx + 557, wy + 23, 87, 120, 1, 10);
        if (cdata[cc].has_own_sprite() == 1)
        {
            pos(wx + 596 + 22, wy + 86 + 24);
            gmode(2, 32, 48);
            grotate(10 + cc, 32, 0, 0, 24, 40);
        }
        else
        {
            i = cdata[cc].image % 1000;
            chara_preparepic(cdata[cc].image % 1000, cdata[cc].image / 1000);
            pos(wx + 596 + 22, wy + 86 + 24);
            gmode(2, chipc(2, i), chipc(3, i));
            grotate(
                5,
                0,
                960,
                0,
                chipc(2, i) / (1 + (chipc(3, i) > inf_tiles)),
                inf_tiles);
        }
        font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
        s(0) = lang(u8"レベル"s, u8"Level"s);
        s(1) = lang(u8"経験"s, u8"EXP"s);
        s(2) = lang(u8"必要値"s, u8"Next Lv"s);
        s(3) = lang(u8"信仰"s, u8"God"s);
        s(4) = lang(u8"所属"s, u8"Guild"s);
        for (int cnt = 0; cnt < 5; ++cnt)
        {
            pos(wx + 355, wy + 46 + cnt * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        s(0) = lang(u8"名前"s, u8"Name"s);
        s(1) = lang(u8"異名"s, u8"Aka"s);
        s(2) = lang(u8"種族"s, u8"Race"s);
        s(3) = lang(u8"性別"s, u8"Sex"s);
        s(4) = lang(u8"職業"s, u8"Class"s);
        s(5) = lang(u8"年齢"s, u8"Age"s);
        s(6) = lang(u8"身長"s, u8"Height"s);
        s(7) = lang(u8"体重"s, u8"Weight"s);
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            pos(wx + 30 + cnt / 4 * 190, wy + 61 + cnt % 4 * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            pos(wx + 37, wy + 157 + cnt * 15);
            gmode(2, inf_tiles, inf_tiles);
            grotate(1, cnt * inf_tiles, 672, 0, inf_tiles, inf_tiles);
            pos(wx + 54, wy + 151 + cnt * 15);
            color(20, 10, 0);
            mes(i18n::_(u8"ui", u8"attribute", u8"_"s + cnt));
            color(0, 0, 0);
        }
        s(0) = lang(u8"生命力"s, u8"Life"s);
        s(1) = lang(u8"マナ"s, u8"Mana"s);
        s(2) = lang(u8"狂気度"s, u8"Sanity"s);
        s(3) = lang(u8"速度"s, u8"Speed"s);
        s(4) = "";
        s(5) = lang(u8"名声"s, u8"Fame"s);
        s(6) = lang(u8"カルマ"s, u8"Karma"s);
        s(7) = "";
        s(8) = lang(u8"打撃修正"s, u8"Melee"s);
        s(9) = lang(u8"射撃修正"s, u8"Shoot"s);
        s(10) = "";
        s(11) = "";
        s(12) = "";
        s(13) = "";
        s(14) = "";
        s(15) = "";
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            pos(wx + 255, wy + 151 + cnt * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        s(0) = lang(u8"ターン"s, u8"Turns"s);
        s(1) = lang(u8"経過日"s, u8"Days"s);
        s(2) = lang(u8"殺害数"s, u8"Kills"s);
        s(3) = lang(u8"総時間"s, u8"Time"s);
        s(4) = "";
        s(5) = "";
        for (int cnt = 0; cnt < 5; ++cnt)
        {
            pos(wx + 32, wy + 301 + cnt * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        s(0) = lang(u8"荷車重量"s, u8"Cargo Wt"s);
        s(1) = lang(u8"荷車限界"s, u8"Cargo Lmt"s);
        s(2) = lang(u8"装備重量"s, u8"Equip Wt"s);
        s(3) = lang(u8"最深到達"s, u8"Deepest Lv"s);
        for (int cnt = 0; cnt < 4; ++cnt)
        {
            pos(wx + 224, wy + 301 + cnt * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        font(14 - en * 2);
        s(0) = ""s + cdata[cc].level;
        s(1) = ""s + cdata[cc].experience;
        s(2) = ""s + cdata[cc].required_experience;
        s(3) = i18n::_(u8"god", cdata[cc].god_id, u8"name");
        s(4) = guildname();
        for (int cnt = 0; cnt < 5; ++cnt)
        {
            pos(wx + 410 + en * 5, wy + 45 + cnt * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        s(0) = cdatan(0, cc);
        s(1) = cdatan(1, cc);
        s(2) = cnven(lang(
            ""s + i18n::_(u8"race", cdatan(2, cc), u8"name"), cdatan(2, cc)));
        access_class_info(2, cdatan(3, cc));
        s(4) = cnven(lang(""s + classname, cdatan(3, cc)));
        if (cdata[cc].sex == 0)
        {
            s(3) = cnven(i18n::_(u8"ui", u8"male"));
        }
        else
        {
            s(3) = cnven(i18n::_(u8"ui", u8"female"));
        }
        s(5) = ""s + calcage(cc) + u8" "s + lang(u8"歳"s, ""s);
        s(6) = ""s + cdata[cc].height + u8" cm"s;
        s(7) = ""s + cdata[cc].weight + u8" kg"s;
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            pos(wx + 68 + cnt / 4 * 190 + en * ((cnt > 3) * 12),
                wy + 60 + cnt % 4 * 15);
            color(20, 10, 0);
            mes(s(cnt));
            color(0, 0, 0);
        }
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            s = u8"("s + sdata.get(10 + cnt, cc).original_level + u8")"s;
            if (encfind(cc, 60010 + cnt) != -1)
            {
                s += u8"*"s;
            }
            pos(wx + 92, wy + 151 + cnt * 15);
            color(20, 10, 0);
            mes(""s + sdata((10 + cnt), cc));
            color(0, 0, 0);
            pos(wx + 124, wy + 151 + cnt * 15);
            color(20, 10, 0);
            mes(s);
            color(0, 0, 0);
            p = sdata.get(10 + cnt, cc).potential;
            pos(wx + 176, wy + 152 + cnt * 15);
            if (p >= 200)
            {
                color(20, 10, 0);
                mes(u8"Superb"s);
                color(0, 0, 0);
                continue;
            }
            if (p >= 150)
            {
                color(20, 10, 0);
                mes(u8"Great"s);
                color(0, 0, 0);
                continue;
            }
            if (p >= 100)
            {
                color(20, 10, 0);
                mes(u8"Good"s);
                color(0, 0, 0);
                continue;
            }
            if (p >= 50)
            {
                color(20, 10, 0);
                mes(u8"Bad"s);
                color(0, 0, 0);
                continue;
            }
            mes(u8"Hopeless"s);
            color(0, 0, 0);
        }
        s(0) = ""s + sdata(2, cc) + u8"("s + sdata.get(2, cc).original_level
            + u8")"s;
        s(1) = ""s + sdata(3, cc) + u8"("s + sdata.get(3, cc).original_level
            + u8")"s;
        s(2) = ""s + cdata[cc].insanity;
        s(3) = ""s + cdata[cc].current_speed + u8"("s
            + sdata.get(18, cc).original_level + u8")"s;
        s(4) = "";
        s(5) = ""s + cdata[cc].fame;
        s(6) = ""s + cdata[cc].karma;
        s(7) = "";
        s(8) = "";
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            color(20, 10, 0);
            pos(wx + 310, wy + 151 + cnt * 15);
            mes(s(cnt));
            color(0, 0, 0);
        }
        append_accuracy_info(0);
        tc = cc;
        font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
        color(20, 10, 0);
        pos(wx + 417, wy + 281 + p(2) * 16);
        mes(lang(u8"軽減"s, u8"Prot"s));
        color(0, 0, 0);
        color(20, 10, 0);
        pos(wx + 590 - en * 16, wy + 281 + p(2) * 16);
        mes(lang(u8"回避"s, u8"Evade"s));
        color(0, 0, 0);
        attackskill = 106;
        int evade = calc_evasion(tc);
        prot = calcattackdmg(2);
        font(14 - en * 2);
        pos(wx + 460 + en * 8, wy + 279 + p(2) * 16);
        mes(""s + (100 - 10000 / (prot + 100)) + u8"% + "s + protdice1 + u8"d"s
            + protdice2);
        pos(wx + 625 - en * 8, wy + 279 + p(2) * 16);
        mes(""s + evade + u8"%"s);
        ++p(2);
        s(0) = ""s + gdata_play_turns + ""s + lang(u8"ターン"s, u8" Turns"s);
        s(1) = ""s + gdata_play_days + ""s + lang(u8"日"s, u8" Days"s);
        s(2) = ""s + gdata_kill_count;
        s(3) = ""s
            + cnvplaytime(
                   (gdata_play_time + timeGetTime() / 1000 - time_begin));
        s(4) = "";
        s(5) = "";
        for (int cnt = 0; cnt < 5; ++cnt)
        {
            pos(wx + 80, wy + 299 + cnt * 15);
            mes(s(cnt));
        }
        s(0) = ""s + cnvweight(gdata_cargo_weight);
        s(1) = cnvweight(gdata_current_cart_limit);
        s(2) = cnvweight(cdata[cc].sum_of_equipment_weight) + u8" "s
            + cnveqweight(cc);
        s(3) = cnvrank(gdata_deepest_dungeon_level)
            + lang(u8"階相当"s, u8" Level"s);
        for (int cnt = 0; cnt < 4; ++cnt)
        {
            pos(wx + 287 + en * 14, wy + 299 + cnt * 15);
            mes(s(cnt));
        }
        cs_buffmax = 0;
        for (int cnt = 0; cnt < 15; ++cnt)
        {
            x = wx + 430 + cnt / 3 * 40;
            y = wy + 151 + cnt % 3 * 32;
            if (cdata[cc].buffs[cnt].id == 0)
            {
                gmode(4, -1, -1, 120);
                pos(x, y);
                gcopy(3, 320, 160, 32, 32);
                gmode(2);
                continue;
            }
            ++cs_buffmax;
            pos(x, y);
            gcopy(5, cdata[cc].buffs[cnt].id * 32, 1120, 32, 32);
            if (cs_buff == cnt)
            {
                pos(x, y);
                gfini(32, 32);
                gfinc(20, 20, 50);
            }
        }
        if (cs_buffmax != 0)
        {
            const auto duration = calc_buff_duration(
                cdata[cc].buffs[cs_buff].id, cdata[cc].buffs[cs_buff].power);
            const auto description = get_buff_description(
                cdata[cc].buffs[cs_buff].id, cdata[cc].buffs[cs_buff].power);
            s = ""s
                + i18n::_(
                      u8"buff",
                      std::to_string(cdata[cc].buffs[cs_buff].id),
                      u8"name")
                + u8": "s + cdata[cc].buffs[cs_buff].turns
                + lang(
                      (u8"("s + duration + u8")ﾀｰﾝの間、"s),
                      (u8"("s + duration + u8") "s))
                + description;
        }
        else
        {
            s = lang(
                u8"今は持続効果を受けていない"s,
                u8"This character isn't currently blessed or hexed."s);
        }
        font(13 - en * 2);
        pos(wx + 108, wy + 366);
        mes(s);
        font(11 + sizefix * 2 - en * 2, snail::font_t::style_t::bold);
        color(20, 10, 0);
        pos(wx + 70, wy + 369 - en * 3);
        mes(lang(u8"説明:"s, u8"Hint:"s));
        color(0, 0, 0);
    }
    else
    {
        display_topic(lang(u8"能力の名称"s, u8"Name"s), wx + 28, wy + 36);
        display_topic(
            lang(u8"Lv(潜在)"s, u8"Lv(Potential)"s), wx + 182, wy + 36);
        display_topic(lang(u8"効果"s, u8"Detail"s), wx + 320, wy + 36);
        keyrange = 0;
        p(1) = 0;
        for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
        {
            p = pagesize * (page - 1) + cnt;
            if (p >= listmax)
            {
                break;
            }
            key_list(cnt) = key_select(cnt);
            ++keyrange;
            if (list(0, p) < 0)
            {
                p(1) = 1;
                continue;
            }
            ++p(1);
            if (list(0, p) < 100)
            {
                x = 52;
                dx = 18;
            }
            else
            {
                x = 70;
                dx = 0;
            }
            if (p(1) % 2 == 0)
            {
                pos(wx + x, wy + 66 + cnt * 19);
                gfini(595 + dx, 18);
                gfdec2(12, 14, 16);
            }
            if (list(0, p) < 100)
            {
                continue;
            }
            display_key(wx + 58, wy + 64 + cnt * 19, cnt);
        }
    }
    if (page > 0)
    {
        font(14 - en * 2);
        cs_listbk();
        for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
        {
            p = pagesize * (page - 1) + cnt;
            if (p >= listmax)
            {
                break;
            }
            if (list(0, p) >= 0)
            {
                i = list(0, p);
                if (i < 100)
                {
                    x = 54;
                    p(1) = 10;
                }
                else
                {
                    x = 84;
                    p(1) = the_ability_db[i]->related_basic_attribute - 10;
                }
                pos(wx + 38, wy + 75 + cnt * 19);
                gmode(2, inf_tiles, inf_tiles);
                grotate(1, p(1) * inf_tiles, 672, 0, inf_tiles, inf_tiles);
                s = i18n::_(u8"ability", std::to_string(i), u8"name");
                if (i >= 50 && i < 100)
                {
                    s = lang(""s, u8"Resist "s) + cnven(s)
                        + lang(u8"耐性"s, ""s);
                }
                for (int cnt = 0; cnt < 3; ++cnt)
                {
                    if (gdata(750 + cnt) == cc * 10000 + i)
                    {
                        s = u8"*"s + s;
                    }
                }
                cs_list(cs == cnt, s, wx + x, wy + 66 + cnt * 19 - 1);
                if (list(0, p) >= 50 && list(0, p) < 100)
                {
                    p(1) = clamp(sdata(list(0, p), cc) / 50, 0, 6);
                    s = i18n::_(u8"ui", u8"resistance", u8"_"s + p(1));
                }
                else
                {
                    s = ""s + sdata.get(i, cc).original_level + u8"."s
                        + std::to_string(
                              1000
                              + sdata.get(list(0, p), cc).experience % 1000)
                              .substr(1);
                    if (sdata.get(i, cc).original_level != sdata(i, cc))
                    {
                        p(1) = sdata(i, cc) - sdata.get(i, cc).original_level;
                    }
                    s += u8"("s + sdata.get(i, cc).potential + u8"%)"s;
                }
                pos(wx + 280 - strlen_u(s) * 7, wy + 66 + cnt * 19 + 2);
                mes(s);
                pos(wx + 330, wy + 66 + cnt * 19 + 2);
                mes(i18n::_(u8"ability", std::to_string(i), u8"description"));
                if (csctrl == 2 || csctrl == 3)
                {
                    if (csctrl == 2)
                    {
                        s = ""s + calctraincost(i, cc) + u8"p "s;
                    }
                    else
                    {
                        s = ""s + calclearncost(i, cc) + u8"p "s;
                    }
                    pos(wx + 322 - strlen_u(s) * 7, wy + 66 + cnt * 19 + 2);
                    mes(s);
                }
                else if (sdata.get(i, cc).original_level != sdata(i, cc))
                {
                    i = sdata(i, cc) - sdata.get(i, cc).original_level;
                    if (list(0, p) >= 50)
                    {
                        i = i / 50;
                    }
                    else
                    {
                        i = i / 5;
                    }
                    s = "";
                    enchantment_print_level(i);
                    pos(wx + 282, wy + 66 + cnt * 19 + 2);
                    mes(s);
                }
            }
            else
            {
                font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
                cs_list(cs == cnt, listn(0, p), wx + 88, wy + 66 + cnt * 19);
                font(14 - en * 2);
            }
        }
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (page == 0)
    {
        if (key == u8"p"s)
        {
            if (cc < 16)
            {
                change_appearance();
                if (mode != 1)
                {
                    nowindowanime = 1;
                }
                returnfromportrait = 1;
                goto label_20331;
            }
        }
        if (key == key_north)
        {
            --cs_buff;
            if (cs_buff < 0)
            {
                cs_buff = cs_buffmax - 1;
            }
            goto label_2034_internal;
        }
        if (key == key_south)
        {
            ++cs_buff;
            if (cs_buff >= cs_buffmax)
            {
                cs_buff = 0;
            }
            goto label_2034_internal;
        }
    }
    else if (csctrl != 1)
    {
        if (key == key_mode2)
        {
            for (int cnt = 0, cnt_end = (keyrange); cnt < cnt_end; ++cnt)
            {
                i = list(0, pagesize * (page - 1) + cs);
                break;
            }
            if (i != -1)
            {
                p = 750;
                for (int cnt = 750; cnt < 753; ++cnt)
                {
                    if (gdata(cnt) % 10000 == 0)
                    {
                        p = cnt;
                    }
                    if (gdata(cnt) == cc * 10000 + i)
                    {
                        p = cnt;
                        i = 0;
                        break;
                    }
                }
                gdata(p) = cc * 10000 + i;
                snd(20);
            }
        }
    }
    if (csctrl == 1)
    {
        if (key == key_enter)
        {
            snd(103);
            result.succeeded = false;
            return result;
        }
        if (key == key_cancel)
        {
            result.succeeded = true;
            return result;
        }
        goto label_2035_internal;
    }
    p = -1;
    for (int cnt = 0, cnt_end = (keyrange); cnt < cnt_end; ++cnt)
    {
        if (key == key_select(cnt))
        {
            p = list(0, pagesize * (page - 1) + cnt);
            break;
        }
    }
    if (p != -1)
    {
        if (csctrl != 4)
        {
            csskill = p;
            if (csctrl == 2 || csctrl == 3)
            {
                screenupdate = -1;
                update_screen();
                tc = tcbk;
                result.succeeded = false;
                return result;
            }
            if (cdata[0].skill_bonus < 1 || p < 0 || p < 100)
            {
                goto label_2034_internal;
            }
            if (sdata.get(csskill, 0).original_level == 0)
            {
                snd(27);
                goto label_2034_internal;
            }
            --cdata[0].skill_bonus;
            snd(19);
            skillexp(csskill, cc, 400, 2, 1000);
            modify_potential(
                cc,
                csskill,
                clamp(15 - sdata.get(csskill, cc).potential / 15, 2, 15));
            render_hud();
            goto label_2034_internal;
        }
    }
    if (key == key_pageup)
    {
        ++page;
        snd(1);
        goto label_2034_internal;
    }
    if (key == key_pagedown)
    {
        --page;
        snd(1);
        goto label_2034_internal;
    }
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 3)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 3;
                }
            }
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    csctrl = 0;
                    goto label_20331;
                }
                if (curmenu == 1)
                {
                    result.turn_result = turn_result_t::menu_equipment;
                    return result;
                }
                if (curmenu == 2)
                {
                    result.turn_result = turn_result_t::menu_feats;
                    return result;
                }
                if (curmenu == 3)
                {
                    result.turn_result = turn_result_t::menu_materials;
                    return result;
                }
            }
        }
    }
    if (key == key_cancel)
    {
        menucycle = 0;
        if (csctrl == 0)
        {
            update_screen();
            result.turn_result = turn_result_t::pc_turn_user_error;
            return result;
        }
        else
        {
            screenupdate = -1;
            update_screen();
            tc = tcbk;
            result.succeeded = false;
            return result;
        }
    }
    goto label_2035_internal;
}

menu_result menu_equipment()
{
    menu_result result = { false, false, turn_result_t::none };
    int cs_prev = 0;
    int mainhand = 0;
    cc = 0;
    page = 0;
    pagesize = 14;
    listmax = 0;
    cs = 0;
    cs_bk = -1;
    if (cs_prev != 0)
    {
        cs = cs_prev;
        cs_prev = 0;
    }
    curmenu = 1;
    mainhand = 0;
    for (int i = 0; i < 30; ++i)
    {
        if (cdata_body_part(cc, i) != 0)
        {
            if (trait(206) != 0)
            {
                if (cdata_body_part(cc, i) / 10000 == 2)
                {
                    continue;
                }
            }
            if (trait(203) != 0)
            {
                if (cdata_body_part(cc, i) / 10000 == 9)
                {
                    continue;
                }
            }
            if (trait(205) != 0)
            {
                if (cdata_body_part(cc, i) / 10000 == 3)
                {
                    continue;
                }
            }
            if (mainhand == 0)
            {
                if (cdata_body_part(cc, i) / 10000 == 5)
                {
                    mainhand = i + 100;
                }
            }
            list(0, listmax) = i + 100;
            list(1, listmax) = cdata_body_part(cc, i) / 10000;
            ++listmax;
        }
    }
    sort_list_by_column1();
    drawmenu();
    ww = 690;
    wh = 380;
    wx = (windoww - 690) / 2 + inf_screenx;
    wy = winposy(380);
    if (nowindowanime == 0)
    {
        snd(95);
    }
    windowanime(wx, wy, ww, wh, 10, 4);
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_wear.bmp", 1);
    gsel(0);
    windowshadow = 1;
label_2051_internal:
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
label_2052_internal:
    s(0) = lang(u8"装備品"s, u8"Equipment"s);
    s(1) = strhint5 + strhint5b + strhint3;
    display_window(
        (windoww - 690) / 2 + inf_screenx, winposy(428), 690, 428, 64);
    display_topic(
        lang(u8"部位/装備品名称"s, u8"Category/Name"s), wx + 28, wy + 30);
    if (showresist == 0)
    {
        display_topic(lang(u8"重さ"s, u8"Weight"s), wx + 524, wy + 30);
    }
    pos(wx + 46, wy - 16);
    gcopy(3, 768, 48, 48, 48);
    pos(wx + ww - 106, wy);
    gcopy(3, 960, 96, 96, 120);
    pos(wx, wy + wh - 164);
    gcopy(3, 960, 216, 72, 144);
    if (showresist)
    {
        pos(wx + 320, wy + 40);
        mes(lang(
            u8"火 冷 雷 闇 幻 毒 獄 音 神 沌 魔"s,
            u8"Fi Co Li Da Mi Po Nt So Nr Ch Ma"s));
    }
    display_note(
        lang(u8"装備重量: "s, u8"Equip weight: "s)
        + cnvweight(cdata[cc].sum_of_equipment_weight) + cnveqweight(cc)
        + lang(u8" 命中修正:"s, u8" Hit Bonus:"s) + cdata[cc].hit_bonus
        + lang(u8" ダメージ修正:"s, u8" Damage Bonus:"s)
        + cdata[cc].damage_bonus + u8"  DV/PV:"s + cdata[cc].dv + u8"/"s
        + cdata[cc].pv);
    font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
    gmode(2);
    keyrange = 0;
    f = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        if (cnt % 2 == 0)
        {
            pos(wx + 100, wy + 60 + cnt * 19);
            gfini(558, 18);
            gfdec(12, 14, 16);
        }
        q = i18n::_(u8"ui", u8"body_part", u8"_"s + list(1, p));
        if (list(0, p) == mainhand)
        {
            q = lang(u8"利手"s, u8"Hand*"s);
        }
        pos(wx + 22, wy + 60 + cnt * 19 - 4);
        gcopy(3, 600 + (list(1, p) - 1) * 24, 336, 24, 24);
        pos(wx + 46, wy + 60 + cnt * 19 + 3);
        mes(q);
    }
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        display_key(wx + 88, wy + 60 + cnt * 19 - 2, cnt);
        p(1) = cdata_body_part(cc, list(0, p));
        s(0) = u8"-    "s;
        s(1) = u8"-"s;
        if (p(1) % 10000 != 0)
        {
            p(1) = p(1) % 10000 - 1;
            s(0) = itemname(p(1));
            s(1) = cnvweight(inv[p(1)].weight);
            p(2) = inv[p(1)].image;
            prepare_item_image(p(2), inv[p(1)].color, inv[p(1)].param1);
            pos(wx + 126, wy + 70 + cnt * 19);
            gmode(2, inf_tiles, inf_tiles);
            grotate(1, 0, 960, 0, chipi(2, p(2)), chipi(3, p(2)));
            if (showresist)
            {
                equipinfo(p(1), wx + 320, wy + 60 + cnt * 19 + 2);
                s = strmid(s, 0, 22);
            }
        }
        else
        {
            p(1) = -1;
        }
        cs_list(
            cs == cnt,
            s,
            wx + 140 - 26,
            wy + 60 + cnt * 19 - 1,
            30,
            p(1) != -1,
            p(1));
        pos(wx + 640 - strlen_u(s(1)) * 7, wy + 60 + cnt * 19 + 2);
        mes(s(1));
    }
    cs_bk = cs;
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        cs_prev = cs;
        body = p;
        if (cdata_body_part(cc, body) % 10000 != 0)
        {
            gdata(808) = 1;
            ci = cdata_body_part(cc, body) % 10000 - 1;
            if (is_cursed(inv[ci].curse_state))
            {
                txt(lang(
                    itemname(ci) + u8"は外せない。"s,
                    ""s + itemname(ci) + u8" can't be taken off."s));
                goto label_2051_internal;
            }
            unequip_item(cc);
            chara_refresh(cc);
            snd(13);
            txtnew();
            txt(lang(
                itemname(ci) + u8"を外した。"s,
                u8"You unequip "s + itemname(ci) + u8"."s));
            if (cdata_body_part(cc, body) / 10000 == 5)
            {
                equip_melee_weapon();
            }
            render_hud();
            goto label_2051_internal;
        }
        nowindowanime = 1;
        menucycle = 0;
        invctrl = 6;
        snd(100);
        result.turn_result = turn_result_t::ctrl_inventory;
        return result;
    }
    if (key == key_identify)
    {
        p = list(0, pagesize * page + cs);
        if (cdata_body_part(cc, p) % 10000 != 0)
        {
            ci = cdata_body_part(cc, p) % 10000 - 1;
            cs_prev = cs;
            show_item_description();
            nowindowanime = 1;
            returnfromidentify = 0;
            screenupdate = -1;
            update_screen();
            result.turn_result = turn_result_t::menu_equipment;
            return result;
        }
    }
    if (key == key_mode)
    {
        if (showresist == 1)
        {
            showresist = 0;
        }
        else
        {
            showresist = 1;
        }
        snd(1);
        goto label_2051_internal;
    }
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 3)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 3;
                }
            }
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    csctrl = 0;
                    result.turn_result = turn_result_t::menu_character_sheet;
                    return result;
                }
                if (curmenu == 1)
                {
                    result.turn_result = turn_result_t::menu_equipment;
                    return result;
                }
                if (curmenu == 2)
                {
                    result.turn_result = turn_result_t::menu_feats;
                    return result;
                }
                if (curmenu == 3)
                {
                    result.turn_result = turn_result_t::menu_materials;
                    return result;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2051_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2051_internal;
        }
    }
    if (key == key_cancel)
    {
        menucycle = 0;
        create_pcpic(cc, true);
        update_screen();
        result.turn_result = turn_result_t::pc_turn_user_error;
        return result;
    }
    goto label_2052_internal;
}

menu_result menu_materials()
{
    menu_result result = { false, false, turn_result_t::none };
    listmax = 0;
    page = 0;
    pagesize = 15;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    curmenu = 3;
    for (int cnt = 0; cnt < 400; ++cnt)
    {
        if (mat(cnt) != 0)
        {
            list(0, listmax) = cnt;
            ++listmax;
        }
    }
    gsel(7);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"ie_scroll.bmp");
    gsel(0);
    snd(92);
    drawmenu();
    wx = (windoww - 600) / 2 + inf_screenx;
    wy = winposy(430);
    ww = 600;
    wh = 430;
    windowanime(wx, wy, ww, wh, 10, 4);
    windowshadow = 1;
label_1860_internal:
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
    drawmenu();
label_1861_internal:
    s = strhint2 + strhint3b;
    showscroll(s, wx, wy, ww, wh);
    display_topic(lang(u8"所持マテリアル"s, u8"Name"s), wx + 38, wy + 36);
    display_topic(lang(u8"説明"s, u8"Detail"s), wx + 296, wy + 36);
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
        if (cnt % 2 == 0)
        {
            pos(wx + 70, wy + 66 + cnt * 19);
            gfini(490, 18);
            gfdec2(12, 14, 16);
        }
        display_key(wx + 68, wy + 66 + cnt * 19 - 2, cnt);
    }
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
        s = ""s + matname(i) + lang(u8" × "s, u8" x "s) + mat(i);
        cs_list(cs == cnt, s, wx + 96, wy + 66 + cnt * 19 - 1, 0, 0);
        s = matdesc(i);
        pos(wx + 308, wy + 66 + cnt * 19 + 2);
        mes(s);
        p(1) = matref(2, i);
        prepare_item_image(p(1), 0);
        pos(wx + 47, wy + 69 + cnt * 19 + 2);
        gmode(2, inf_tiles, inf_tiles);
        grotate(1, 0, 960, 0, chipi(2, p(1)), chipi(3, p(1)));
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 3)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 3;
                }
            }
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    csctrl = 0;
                    result.turn_result = turn_result_t::menu_character_sheet;
                    return result;
                }
                if (curmenu == 1)
                {
                    result.turn_result = turn_result_t::menu_equipment;
                    return result;
                }
                if (curmenu == 2)
                {
                    result.turn_result = turn_result_t::menu_feats;
                    return result;
                }
                if (curmenu == 3)
                {
                    result.turn_result = turn_result_t::menu_materials;
                    return result;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_1860_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1860_internal;
        }
    }
    if (key == key_cancel)
    {
        update_screen();
        result.turn_result = turn_result_t::pc_turn_user_error;
        return result;
    }
    goto label_1861_internal;
}

void set_pcc_info(int val0)
{
    rtval = -2;
    if (page == 0)
    {
        if (val0 == 0)
        {
            rtval = -2;
        }
        if (val0 == 1)
        {
            rtval(0) = 100;
            rtval(1) = 0;
            rtval(2) = cdata[cc].portrait;
        }
        if (val0 == 2)
        {
            rtval(0) = 1;
            rtval(1) = 0;
            rtval(2) = pcc(1, cc) % 1000;
            rtvaln = u8"hair"s;
        }
        if (val0 == 3)
        {
            rtval(0) = 10;
            rtval(1) = 0;
            rtval(2) = pcc(10, cc) % 1000;
            rtvaln = u8"subhair"s;
        }
        if (val0 == 4)
        {
            rtval(0) = 1;
            rtval(1) = 1;
            rtval(2) = pcc(1, cc) / 1000;
        }
        if (val0 == 5)
        {
            rtval(0) = 15;
            rtval(1) = 0;
            rtval(2) = pcc(15, cc) % 1000;
            rtvaln = u8"body"s;
        }
        if (val0 == 6)
        {
            rtval(0) = 9;
            rtval(1) = 0;
            rtval(2) = pcc(9, cc) % 1000;
            rtvaln = u8"cloth"s;
        }
        if (val0 == 7)
        {
            rtval(0) = 7;
            rtval(1) = 0;
            rtval(2) = pcc(7, cc) % 1000;
            rtvaln = u8"pants"s;
        }
        if (val0 == 8)
        {
            rtval = -1;
            rtvaln = "";
        }
        if (val0 == 9)
        {
            if (cc != 0)
            {
                rtval(0) = 101;
                rtval(1) = 0;
                rtval(2) = cdata[cc].has_own_sprite();
            }
            else
            {
                rtval(0) = 16;
                rtval(1) = 0;
                rtval(2) = pcc(16, cc) % 1000;
                rtvaln = u8"ride"s;
            }
        }
    }
    else
    {
        if (val0 == 0)
        {
            rtval(0) = 15;
            rtval(1) = 1;
            rtval(2) = pcc(15, cc) / 1000;
        }
        if (val0 == 1)
        {
            rtval(0) = 9;
            rtval(1) = 1;
            rtval(2) = pcc(9, cc) / 1000;
        }
        if (val0 == 2)
        {
            rtval(0) = 7;
            rtval(1) = 1;
            rtval(2) = pcc(7, cc) / 1000;
        }
        if (val0 == 3)
        {
            rtval(0) = 11;
            rtval(1) = 0;
            rtval(2) = pcc(11, cc) % 1000;
            rtvaln = u8"etc"s;
        }
        if (val0 == 4)
        {
            rtval(0) = 12;
            rtval(1) = 0;
            rtval(2) = pcc(12, cc) % 1000;
            rtvaln = u8"etc"s;
        }
        if (val0 == 5)
        {
            rtval(0) = 13;
            rtval(1) = 0;
            rtval(2) = pcc(13, cc) % 1000;
            rtvaln = u8"etc"s;
        }
        if (val0 == 6)
        {
            rtval(0) = 14;
            rtval(1) = 0;
            rtval(2) = pcc(14, cc) % 1000;
            rtvaln = u8"eye"s;
        }
        if (val0 == 7)
        {
            rtval = -1;
            rtvaln = "";
        }
    }
    return;
}

int change_appearance()
{
    create_pcpic(cc, false);
    page = 0;
    pagesize = 19;
    cs = 0;
    cs_bk = -1;
    ww = 380;
    wh = 340;
    wx = (windoww - ww) / 2 + inf_screenx;
    wy = winposy(wh);
    snd(97);
    windowanime(wx, wy, ww, wh, 10, 7);
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"face1.bmp", 1);
    buffer(7, 800, 112);
    boxf();
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        const auto filepath = filesystem::dir::user() / u8"graphic"
            / (u8"face"s + (cnt + 1) + u8".bmp");
        if (fs::exists(filepath))
        {
            pos(cnt * 80, 0);
            picload(filepath, 1);
        }
    }
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_mirror.bmp", 1);
    gsel(0);
    windowshadow = 1;
label_2040_internal:
    listmax = 0;
    if (page == 0)
    {
        if (jp)
        {
            s(0) = u8"決定    "s;
            s(1) = u8"肖像　　"s;
            s(2) = u8"髪　　　"s;
            s(3) = u8"副髪　　"s;
            s(4) = u8"髪の色　"s;
            s(5) = u8"体　　　"s;
            s(6) = u8"服　　　"s;
            s(7) = u8"ﾊﾟﾝﾂ　　"s;
            s(8) = u8"詳細設定"s;
        }
        if (en)
        {
            s(0) = u8"Done    "s;
            s(1) = u8"Portrait"s;
            s(2) = u8"Hair    "s;
            s(3) = u8"Sub Hair"s;
            s(4) = u8"Hair CL "s;
            s(5) = u8"Body    "s;
            s(6) = u8"Cloth   "s;
            s(7) = u8"Pants   "s;
            s(8) = u8"Set Detail"s;
        }
        if (cc != 0)
        {
            s(9) = lang(u8"ｶｽﾀﾑｷｬﾗ "s, u8"Custom  "s);
        }
        else
        {
            s(9) = lang(u8"乗馬時　"s, u8"Riding  "s);
        }
        p = 9 + (cc != 0) + (cc == 0) * (gdata_mount != 0);
    }
    else
    {
        if (jp)
        {
            s(0) = u8"体の色　"s;
            s(1) = u8"服の色　"s;
            s(2) = u8"ﾊﾟﾝﾂの色"s;
            s(3) = u8"ｱｸｾｻﾘ1　"s;
            s(4) = u8"ｱｸｾｻﾘ2　"s;
            s(5) = u8"ｱｸｾｻﾘ3　"s;
            s(6) = u8"目　　　"s;
            s(7) = u8"基本設定"s;
        }
        if (en)
        {
            s(0) = u8"Body CL "s;
            s(1) = u8"Cloth CL"s;
            s(2) = u8"Pants CL"s;
            s(3) = u8"Etc1    "s;
            s(4) = u8"Etc2    "s;
            s(5) = u8"Etc3    "s;
            s(6) = u8"Eyes    "s;
            s(7) = u8"Set Basic"s;
        }
        p = 8;
    }
    for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
    {
        list(0, cnt) = cnt;
        listn(0, cnt) = s(cnt);
        ++listmax;
    }
    keyrange = 0;
    for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
    {
        p = cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_enter;
        ++keyrange;
    }
label_2041_internal:
    pagesize = 0;
    s(0) = lang(u8"肖像の変更"s, u8"Appearance"s);
    s(1) = lang(
        u8"左右キー [変更]  ｷｬﾝｾﾙ [閉じる]"s,
        u8"Right,left [Change]  Shift,Esc [Close]"s);
    display_window(
        (windoww - 380) / 2 + inf_screenx, winposy(340) - 12, 380, 340);
    pagesize = listmax;
    display_topic(lang(u8"項目"s, u8"Category"s), wx + 34, wy + 36);
    pos(wx + ww - 40, wy);
    gcopy(3, 960, 96, 48, 120);
    ++i;
    if (i % 100 < 45)
    {
        f = i % 16;
    }
    else
    {
        ++f;
    }
    window2(wx + 234, wy + 71, 88, 120, 1, 1);
    if (cs == 1 && page == 0)
    {
        if (cdata[cc].portrait >= 0)
        {
            p = cdata[cc].sex * 64 + cdata[cc].portrait;
            pos(wx + 238, wy + 75);
            gzoom(4, p % 16 * 48, p / 16 * 72, 48, 72, 80, 112);
        }
        else if (cdata[cc].portrait != -1)
        {
            pos(wx + 238, wy + 75);
            gzoom(
                7,
                std::abs((cdata[cc].portrait + 2)) * 80,
                0,
                80,
                112,
                80,
                112);
        }
    }
    else if (cdata[cc].has_own_sprite() == 1)
    {
        pos(wx + 280, wy + 130);
        gmode(2, 32, 48);
        grotate(10 + cc, f / 4 % 4 * 32, f / 16 % 4 * 48, 0, 48, 80);
    }
    else
    {
        i = cdata[cc].image % 1000;
        chara_preparepic(cdata[cc].image % 1000, cdata[cc].image / 1000);
        pos(wx + 280, wy + 130);
        gmode(2, chipc(2, i), chipc(3, i));
        grotate(5, 0, 960, 0, chipc(2, i), chipc(3, i));
    }
    gmode(2);
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = cnt;
        if (p >= listmax)
        {
            break;
        }
        set_pcc_info(cnt);
        s = listn(0, p);
        if (rtval >= 0)
        {
            if (rtval(2) >= 0)
            {
                s += u8" "s + rtval(2);
            }
            else if (rtval(2) == -1)
            {
                s += u8" N/A"s;
            }
            else
            {
                s += u8" u"s + (std::abs(rtval(2)) - 1);
            }
        }
        cs_list(cs == cnt, s, wx + 60, wy + 66 + cnt * 21 - 1, 0);
        if (rtval != -2)
        {
            pos(wx + 30, wy + 66 + cnt * 21 - 5);
            gcopy(3, 312, 336, 24, 24);
            pos(wx + 175, wy + 66 + cnt * 21 - 5);
            gcopy(3, 336, 336, 24, 24);
        }
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    set_pcc_info(cs);
    p = 0;
    if (rtval == -2)
    {
        if (key == key_enter)
        {
            create_pcpic(cc, true);
            return 1;
        }
        if (key == key_pageup || key == key_pagedown)
        {
            key = ""s;
        }
    }
    else if (key == key_enter)
    {
        key = key_pageup;
    }
    if (rtval == -1)
    {
        if (key == key_pageup || key == key_pagedown)
        {
            snd(5);
            if (page == 0)
            {
                page = 1;
            }
            else
            {
                page = 0;
                cs = 8;
            }
            goto label_2040_internal;
        }
    }
    if (key == key_pageup)
    {
        snd(5);
        if (rtval == 100)
        {
            if (cdata[cc].portrait < 31)
            {
                ++cdata[cc].portrait;
            }
            goto label_2041_internal;
        }
        if (rtval == 101)
        {
            cdata[cc].has_own_sprite() = true;
            goto label_2041_internal;
        }
        if (rtval(1) == 0)
        {
            if (fs::exists(
                    filesystem::dir::graphic()
                    / (u8"pcc_"s + rtvaln + u8"_" + (pcc(rtval, cc) % 1000 + 1)
                       + u8".bmp")))
            {
                ++pcc(rtval, cc);
                p = 1;
            }
        }
        else if (pcc(rtval, cc) / 1000 < 21)
        {
            pcc(rtval, cc) += 1000;
            p = 1;
        }
    }
    if (key == key_pagedown)
    {
        snd(5);
        if (rtval == 100)
        {
            if (cdata[cc].portrait > -10)
            {
                --cdata[cc].portrait;
            }
            goto label_2041_internal;
        }
        if (rtval == 101)
        {
            cdata[cc].has_own_sprite() = false;
            goto label_2041_internal;
        }
        if (rtval(1) == 0)
        {
            if ((pcc(rtval, cc) % 1000 == 1 && rtval != 15)
                || fs::exists(
                       filesystem::dir::graphic()
                       / (u8"pcc_"s + rtvaln + u8"_"s
                          + (pcc(rtval, cc) % 1000 - 1) + u8".bmp"s)))
            {
                --pcc(rtval, cc);
                p = 1;
            }
        }
        else if (pcc(rtval, cc) / 1000 > 0)
        {
            pcc(rtval, cc) -= 1000;
            p = 1;
        }
    }
    create_pcpic(cc, false);
    if (key == key_cancel)
    {
        create_pcpic(cc, true);
        return 0;
    }
    if (mode == 1)
    {
        if (getkey(snail::key::f1))
        {
            return -1;
        }
    }
    goto label_2041_internal;
}

int label_2044()
{
    create_pcpic(cc, true);
    snd(26);
    page = 0;
    pagesize = 18;
    cs = 0;
    cs_bk = -1;
    windowshadow = 1;
    listmax = 0;
    if (jp)
    {
        s(0) = u8"決定    "s;
        s(1) = u8"胸部鎧　"s;
        s(2) = u8"脚部鎧　"s;
        s(3) = u8"ベルト　"s;
        s(4) = u8"ｸﾞﾛｰﾌﾞ　"s;
        s(5) = u8"マント　"s;
    }
    if (en)
    {
        s(0) = u8"Done    "s;
        s(1) = u8"Chest   "s;
        s(2) = u8"Leg     "s;
        s(3) = u8"Belt    "s;
        s(4) = u8"Glove   "s;
        s(5) = u8"Mantle  "s;
    }
    p = 6;
    for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
    {
        list(0, cnt) = cnt;
        listn(0, cnt) = s(cnt);
        ++listmax;
    }
    keyrange = 0;
    for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
    {
        p = cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_enter;
        ++keyrange;
    }

    while (1)
    {
        pagesize = 0;
        s(0) = lang(u8"装備表示の変更"s, u8"Parts to hide"s);
        s(1) = lang(
            u8"左右キー [変更]  ｷｬﾝｾﾙ [閉じる]"s,
            u8"Right,left [Change]  Shift,Esc [Close]"s);
        display_window(
            (windoww - 360) / 2 + inf_screenx, winposy(289) - 12, 360, 289);
        s = lang(u8"項目"s, u8"Part"s);
        pagesize = listmax;
        display_topic(s, wx + 34, wy + 36);
        ++i;
        if (i % 100 < 45)
        {
            f = i % 16;
        }
        else
        {
            ++f;
        }
        window2(wx + 234, wy + 60, 88, 120, 1, 1);
        pos(wx + 280, wy + 120);
        gmode(2, 32, 48);
        grotate(10 + cc, f / 4 % 4 * 32, f / 16 % 4 * 48, 0, 48, 80);
        gmode(2);
        font(14 - en * 2);
        cs_listbk();
        for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
        {
            p = cnt;
            if (p >= listmax)
            {
                break;
            }
            s = listn(0, p);
            if (cnt != 0)
            {
                if (pcc(20 + cnt - 1, cc) == 0)
                {
                    s += u8"On"s;
                }
                else
                {
                    s += u8"Off"s;
                }
                pos(wx + 30, wy + 66 + cnt * 21 - 5);
                gcopy(3, 312, 336, 24, 24);
                pos(wx + 175, wy + 66 + cnt * 21 - 5);
                gcopy(3, 336, 336, 24, 24);
            }
            cs_list(cs == cnt, s, wx + 60, wy + 66 + cnt * 21 - 1, 0);
        }
        if (keyrange != 0)
        {
            cs_bk = cs;
        }
        redraw();
        await(config::instance().wait1);
        key_check();
        cursor_check();
        if (cs != 0)
        {
            if (key == key_enter)
            {
                key = key_pageup;
            }
            if (key == key_pageup || key == key_pagedown)
            {
                if (pcc(20 + cs - 1, cc) == 0)
                {
                    pcc(20 + cs - 1, cc) = 1;
                }
                else
                {
                    pcc(20 + cs - 1, cc) = 0;
                }
                create_pcpic(cc, true);
                snd(5);
            }
        }
        if ((cs == 0 && key == key_enter) || key == key_cancel)
        {
            snd(20);
            create_pcpic(cc, true);
            return 1;
        }
    }
}

void append_accuracy_info(int val0)
{
    p(1) = 0;
    p(2) = 0;
    attackskill = 106;
    ammo = -1;
    attacknum = 0;
    for (int cnt = 0; cnt < 30; ++cnt)
    {
        body = 100 + cnt;
        if (cdata_body_part(cc, cnt) % 10000 == 0)
        {
            continue;
        }
        if (cdata_body_part(cc, cnt) / 10000 == 10)
        {
            continue;
        }
        if (cdata_body_part(cc, cnt) / 10000 == 11)
        {
            continue;
        }
        cw = cdata_body_part(cc, cnt) % 10000 - 1;
        if (inv[cw].dice_x > 0)
        {
            attackskill = inv[cw].skill;
            ++p(1);
            s(1) = lang(u8"武器"s, u8"Melee"s) + p(1);
            ++attacknum;
            show_weapon_dice(val0);
        }
    }
    if (attackskill == 106)
    {
        s(1) = lang(u8"格闘"s, u8"Unarmed"s);
        show_weapon_dice(val0);
    }
    attacknum = 0;
    int stat = can_do_ranged_attack();
    if (stat == 1)
    {
        s(1) = lang(u8"射撃"s, u8"Dist"s);
        show_weapon_dice(val0);
    }
    return;
}

void show_weapon_dice(int val0)
{
    tc = cc;
    font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
    color(20, 10, 0);
    if (val0 == 0)
    {
        pos(wx + 590, wy + 281 + p(2) * 16);
        mes(lang(u8"命中"s, u8"Hit"s));
        pos(wx + 417, wy + 281 + p(2) * 16);
        mes(s(1));
    }
    color(0, 0, 0);
    attackrange = 0;
    if (the_item_db[inv[cw].id]->category == 24000) // TODO coupling
    {
        attackrange = 1;
    }
    attackvar = 0;
    int tohit = calc_accuracy(false);
    dmg = calcattackdmg(1);
    font(14 - en * 2);
    s(2) = ""s + dmgmulti;
    s = ""s + tohit + u8"%"s;
    if (val0 == 0)
    {
        pos(wx + 625 - en * 8, wy + 279 + p(2) * 16);
        mes(s);
    }
    else
    {
        s(3) = s;
    }
    s = ""s + dice1 + u8"d"s + dice2 + cnvfix(dmgfix) + u8" x"s
        + strmid(
              s(2),
              0,
              3 + (elona::stoi(s(2)) >= 10) + (elona::stoi(s(2)) >= 100));
    if (val0 == 0)
    {
        pos(wx + 460 + en * 8, wy + 279 + p(2) * 16);
        mes(s);
    }
    else
    {
        noteadd(s(1) + "   : " + fixtxt(s(3), 12) + " " + fixtxt(s, 20));
    }
    ++p(2);
    return;
}

menu_result menu_feats()
{
    deco_traits_menu();
    return menu_feats_internal();
}

menu_result label_1969()
{
    return menu_feats_internal();
}

void deco_traits_menu()
{
    listmax = 0;
    page = 0;
    pagesize = 15;
    cs = 0;
    tc = 0;
    cs_bk = -1;
    curmenu = 2;
    snd(96);
    drawmenu();
    ww = 700;
    wh = 400;
    wx = (windoww - ww) / 2 + inf_screenx;
    wy = winposy(wh);
    windowanime(wx, wy, ww, wh, 10, 4);
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_feat.bmp", 1);
    gsel(0);
    windowshadow = 1;
}

menu_result menu_feats_internal()
{
    menu_result result = { false, false, turn_result_t::none };
    int featrq = 0;
label_196901_internal:
    listmax = 0;
    if (tc == 0 && gdata_acquirable_feat_count > 0)
    {
        list(0, listmax) = -1;
        list(1, listmax) = 0;
        ++listmax;
    }
    f = 0;
    for (int cnt = 0; cnt < 217; ++cnt)
    {
        if (tc != 0)
        {
            break;
        }
        int stat = get_trait_info(0, cnt);
        if (stat == 0)
        {
            continue;
        }
        if (stat == 1)
        {
            if (traitref == 0)
            {
                if (gdata_acquirable_feat_count > 0)
                {
                    list(0, listmax) = cnt;
                    list(1, listmax) = cnt + 1;
                    ++listmax;
                }
            }
        }
        if (trait(cnt) != 0)
        {
            list(0, listmax) = cnt;
            list(1, listmax) = 10000 + cnt + 1;
            ++listmax;
            ++f;
        }
    }
    if (f != 0)
    {
        list(0, listmax) = -2;
        list(1, listmax) = 10000;
        ++listmax;
    }
    sort_list_by_column1();
    for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
    {
        i = list(0, cnt);
        if (i < 0)
        {
            if (i == -1)
            {
                if (jp)
                {
                    s = u8"◆ 取得できるフィート"s;
                }
                else
                {
                    s = u8"[Available feats]"s;
                }
            }
            if (i == -2)
            {
                if (jp)
                {
                    s = u8"◆ 特徴と体質"s;
                }
                else
                {
                    s = u8"[Feats and traits]"s;
                }
            }
            listn(0, cnt) = s;
            continue;
        }
        int tid = i;
        int stat = get_trait_info(0, tid);
        featrq = stat;
        s = "";
        if (list(1, cnt) < 10000)
        {
            if (trait(tid) < traitref(2))
            {
                s = traitrefn2(trait(tid));
            }
            else
            {
                s = traitrefn2(traitref(2) - 1) + u8"(MAX)"s;
            }
            if (featrq == -1)
            {
                if (jp)
                {
                    s += u8"(条件不足)"s;
                }
                else
                {
                    s += u8"(requirement)"s;
                }
            }
            pos(wx + 30, wy + 61 + cnt * 19);
            x = 84;
        }
        else
        {
            pos(wx + 45, wy + 61 + cnt * 19);
            x = 70;
            if (traitref == 0)
            {
                if (jp)
                {
                    s = u8"[フィート]"s;
                }
                else
                {
                    s = u8"[Feat]"s;
                }
            }
            if (traitref == 1)
            {
                if (jp)
                {
                    s = u8"[変異]"s;
                }
                else
                {
                    s = u8"[Mutation]"s;
                }
            }
            if (traitref == 2)
            {
                if (jp)
                {
                    s = u8"[先天]"s;
                }
                else
                {
                    s = u8"[Race]"s;
                }
            }
            if (traitref == 3)
            {
                if (jp)
                {
                    s = u8"[ｴｰﾃﾙ病]"s;
                }
                else
                {
                    s = u8"[Disease]"s;
                }
            }
            s += traitrefn(2 + std::abs(trait(tid)));
        }
        listn(0, cnt) = s;
    }
    if (cdata[tc].is_incognito() == 1)
    {
        list(0, listmax) = 1;
        list(1, listmax) = 99999;
        listn(0, listmax) = lang(u8"[その他]"s, u8"[ETC]"s)
            + lang(u8"あなたは変装している"s,
                   u8"You are disguising yourself."s);
        ++listmax;
    }
    if (cdata[tc].is_pregnant() == 1)
    {
        list(0, listmax) = 1;
        list(1, listmax) = 99999;
        listn(0, listmax) = lang(u8"[その他]"s, u8"[ETC]"s)
            + lang(u8"あなたは寄生されている"s, u8"You are pregnant."s);
        ++listmax;
    }
    if (cdata[tc].has_anorexia() == 1)
    {
        list(0, listmax) = 1;
        list(1, listmax) = 99999;
        listn(0, listmax) = lang(u8"[その他]"s, u8"[ETC]"s)
            + lang(u8"あなたは拒食症だ"s, u8"You have anorexia."s);
        ++listmax;
    }
    if (cdata[tc].speed_correction_value != 0)
    {
        list(0, listmax) = 1;
        list(1, listmax) = 99999;
        listn(0, listmax) = lang(u8"[その他]"s, u8"[ETC]"s)
            + lang((u8"あなたは複雑な体をしている[速度-"s
                    + cdata[tc].speed_correction_value + u8"%]"s),
                   (u8"Your body is complicated. [SPD-"s
                    + cdata[tc].speed_correction_value + u8"%]"s));
        ++listmax;
    }
    if (tc == 0 && gdata_ether_disease_speed != 0)
    {
        if (gdata_ether_disease_speed > 0)
        {
            list(0, listmax) = 1;
            list(1, listmax) = 99999;
            listn(0, listmax) = lang(u8"[その他]"s, u8"[ETC]"s)
                + lang(u8"あなたのエーテル病の進行は早い。"s,
                       u8"Your Ether disease grows fast."s);
            ++listmax;
        }
        else
        {
            list(0, listmax) = 1;
            list(1, listmax) = 99999;
            listn(0, listmax) = lang(u8"[その他]"s, u8"[ETC]"s)
                + lang(u8"あなたのエーテル病の進行は遅い。"s,
                       u8"Your Ether disease grows slow."s);
            ++listmax;
        }
    }
    if (dump_return == 1)
    {
        dump_return = 0;
        result.succeeded = false;
        return result;
    }
    xnotesel(buff);
    buff = "";
    for (int i = 0; i < 30; ++i)
    {
        if (cdata_body_part(tc, i) % 10000 != 0)
        {
            ci = cdata_body_part(tc, i) % 10000 - 1;
            int cnt = 0;
            for (int cnt_end = cnt + (15); cnt < cnt_end; ++cnt)
            {
                if (inv[ci].enchantments[cnt].id == 0)
                {
                    break;
                }
                get_enchantment_description(
                    inv[ci].enchantments[cnt].id,
                    inv[ci].enchantments[cnt].power,
                    0,
                    true);
                if (s == ""s)
                {
                    continue;
                }
                xnoteadd(s);
            }
        }
    }
    notesel(buff);
    for (int cnt = 0, cnt_end = (noteinfo()); cnt < cnt_end; ++cnt)
    {
        noteget(s, cnt);
        list(0, listmax) = 1;
        list(1, listmax) = 99999;
        listn(0, listmax) =
            lang(his(tc, 1) + u8"装備は"s, cnven(his(tc, 1)) + u8" equipment "s)
            + s;
        ++listmax;
    }
    if (tc != 0)
    {
        for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
        {
            if (jp)
            {
                listn(0, cnt) = strutil::replace(listn(0, cnt), u8"あなた", he(tc, 1));
            }
            if (en)
            {
                listn(0, cnt) = strutil::replace(listn(0, cnt), u8" your", his(tc, 1));
                listn(0, cnt) = strutil::replace(listn(0, cnt), u8" you", him(tc, 1));
            }
        }
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
    if (cs < 0)
    {
        for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
        {
            if (list(1, cnt) >= 10000)
            {
                if (list(0, cnt) - 10000 == cs)
                {
                    page = cnt / pagesize;
                    cs = cnt % pagesize;
                    break;
                }
            }
        }
        if (cs < 0)
        {
            cs = 0;
        }
    }
label_1970_internal:
    s(0) = lang(u8"特徴と体質"s, u8"Feats and Traits"s);
    s(1) = lang(
               u8"決定 [フィート取得]  "s + strhint2 + strhint3,
               u8"Enter [Gain Feat]  "s + strhint2 + strhint3)
        + u8"z,x ["s + lang(u8"仲間"s, u8"Ally"s) + u8"]"s;
    if (mode == 1)
    {
        i = 1;
    }
    else
    {
        i = 0;
    }
    display_window(
        (windoww - 730) / 2 + inf_screenx,
        winposy(430, i) + i * 15,
        730,
        430,
        55,
        40);
    if (jp)
    {
        s(0) = u8"特徴の名称"s;
        s(1) = u8"段階"s;
        s(2) = u8"特徴の効果"s;
    }
    else
    {
        s(0) = u8"Name"s;
        s(1) = u8"Level"s;
        s(2) = u8"Detail"s;
    }
    display_topic(s, wx + 46, wy + 36);
    display_topic(s(2), wx + 255, wy + 36);
    pos(wx + 46, wy - 16);
    gcopy(3, 816, 48, 48, 48);
    pos(wx + ww - 56, wy + wh - 198);
    gcopy(3, 960, 96, 48, 192);
    pos(wx, wy);
    gcopy(3, 1008, 96, 48, 144);
    pos(wx + ww - 108, wy);
    gcopy(3, 960, 288, 96, 72);
    pos(wx, wy + wh - 70);
    gcopy(3, 1008, 240, 96, 48);
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
        if (list(0, p) < 0)
        {
            continue;
        }
        if (cnt % 2 == 0)
        {
            pos(wx + 57, wy + 66 + cnt * 19);
            gfini(640, 18);
            gfdec2(12, 14, 16);
        }
        if (list(1, p) >= 10000 || list(0, p) < 0)
        {
            continue;
        }
        display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
    }
    if (tc == 0)
    {
        s = lang(
            u8"残り "s + gdata_acquirable_feat_count
                + u8"個のフィートを取得できる"s,
            u8"You can acquire "s + gdata_acquirable_feat_count + u8" feats"s);
    }
    else
    {
        s = cnven(cdatan(0, tc)) + lang(u8"の特性"s, u8"'s Trait"s);
    }
    display_note(s, 50);
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
        if (i < 0)
        {
            cs_list(
                cs == cnt,
                listn(0, p),
                wx + 114,
                wy + 66 + cnt * 19 - 1,
                0,
                -1);
            continue;
        }
        if (list(1, p) != 99999)
        {
            int stat = get_trait_info(0, i);
            featrq = stat;
            if (trait(i) == 0)
            {
            }
            else if (trait(i) > 0)
            {
                color(0, 0, 200);
            }
            else
            {
                color(200, 0, 0);
            }
        }
        else
        {
            traitref = 5;
        }
        if (list(1, p) < 10000)
        {
            pos(wx + 30, wy + 61 + cnt * 19);
            x = 84;
        }
        else
        {
            pos(wx + 45, wy + 61 + cnt * 19);
            x = 70;
        }
        gcopy(3, 384 + traitref * 24, 336, 24, 24);
        cs_list(cs == cnt, listn(0, p), wx + x, wy + 66 + cnt * 19 - 1, 0, -1);
        if (list(1, p) < 10000)
        {
            pos(wx + 270, wy + 66 + cnt * 19 + 2);
            mes(traitrefn(2));
        }
        color(0, 0, 0);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_INDEX(p);
    if (tc == 0)
    {
        if (gdata_acquirable_feat_count > 0)
        {
            if (p > 0)
            {
                if (list(1, p) < 10000)
                {
                    int tid = list(0, p);
                    get_trait_info(0, tid);
                    if (traitref(2) <= trait(tid))
                    {
                        if (mode != 1)
                        {
                            txt(lang(
                                u8"これ以上取得できない。"s,
                                u8"You already have maxed out the feat."s));
                        }
                        goto label_196901_internal;
                    }
                    --gdata_acquirable_feat_count;
                    cs = -10000 + tid;
                    snd(61);
                    ++trait(tid);
                    chara_refresh(tc);
                    if (mode == 1)
                    {
                        if (gdata_acquirable_feat_count == 0)
                        {
                            result.succeeded = true;
                            return result;
                        }
                    }
                    else
                    {
                        render_hud();
                    }
                    goto label_196901_internal;
                }
            }
        }
    }
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 3)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 3;
                }
            }
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    csctrl = 0;
                    result.turn_result = turn_result_t::menu_character_sheet;
                    return result;
                }
                if (curmenu == 1)
                {
                    result.turn_result = turn_result_t::menu_equipment;
                    return result;
                }
                if (curmenu == 2)
                {
                    result.turn_result = turn_result_t::menu_feats;
                    return result;
                }
                if (curmenu == 3)
                {
                    result.turn_result = turn_result_t::menu_materials;
                    return result;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_196901_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_196901_internal;
        }
    }
    if (key == u8"z"s || key == u8"x"s)
    {
        p = tc;
        for (int cnt = 0; cnt < 16; ++cnt)
        {
            if (key == u8"z"s)
            {
                --p;
                if (p < 0)
                {
                    p = 15;
                }
            }
            if (key == u8"x"s)
            {
                ++p;
                if (p == 16)
                {
                    p = 0;
                }
            }
            if (cdata[p].state != 1)
            {
                continue;
            }
            break;
        }
        tc = p;
        snd(1);
        page = 0;
        cs = 0;
        goto label_196901_internal;
    }
    if (key == key_cancel)
    {
        if (mode == 1)
        {
            result.succeeded = false;
            return result;
        }
        update_screen();
        result.turn_result = turn_result_t::pc_turn_user_error;
        return result;
    }
    if (mode == 1)
    {
        if (getkey(snail::key::f1))
        {
            show_game_help();
            result.pressed_f1 = true;
            return result;
        }
    }
    goto label_1970_internal;
}

menu_result menu_journal()
{
    menu_result result = { false, false, turn_result_t::none };
    curmenu = 1;
    listmax = 0;
    page = 99;
    pagesize = 40;
    cs = 0;
    cc = 0;
    listmax = noteinfo();
    keyrange = 0;
    key_list(0) = key_enter;
    buff = newsbuff;
    if (newsbuff == ""s)
    {
        buff = u8"No news"s;
    }
    notesel(buff);
    noteadd(u8" - News - "s, 0);
    noteadd(""s, 1);
    if (noteinfo() / (pagesize / 2) % 2 == 1)
    {
        for (int cnt = 0, cnt_end = (pagesize / 2); cnt < cnt_end; ++cnt)
        {
            noteadd(""s, 2);
        }
    }
    page = noteinfo() / pagesize;
    for (int cnt = 0,
             cnt_end = cnt + (pagesize / 2 - noteinfo() % (pagesize / 2));
         cnt < cnt_end;
         ++cnt)
    {
        noteadd(""s);
    }
    noteadd(u8" - Quest - "s);
    noteadd(""s);
    quest_update_main_quest_journal();
    for (int cnt = 0, cnt_end = (gdata_number_of_existing_quests);
         cnt < cnt_end;
         ++cnt)
    {
        if (qdata(8, cnt) == 0)
        {
            continue;
        }
        rq = cnt;
        quest_set_data(2);
    }
    append_subquest_journal(0);
    for (int cnt = 0,
             cnt_end = cnt + (pagesize / 2 - noteinfo() % (pagesize / 2));
         cnt < cnt_end;
         ++cnt)
    {
        noteadd(""s);
    }
    noteadd(u8" - Quest Item - "s);
    noteadd(""s);
    append_quest_item_journal();
    for (int cnt = 0,
             cnt_end = cnt + (pagesize / 2 - noteinfo() % (pagesize / 2));
         cnt < cnt_end;
         ++cnt)
    {
        noteadd(""s);
    }
    gold = 0;
    p = clamp(cdata[0].fame / 10, 100, 25000);
    if (cdata[0].fame >= 25000)
    {
        p += (cdata[0].fame - 25000) / 100;
    }
    gold += p;
    noteadd(u8" - Title & Ranking - "s);
    noteadd(""s);
    noteadd(lang(u8"名声: "s, u8"Fame: "s) + cdata[0].fame);
    noteadd(""s);
    for (int cnt = 0; cnt < 9; ++cnt)
    {
        if (gdata(120 + cnt) < 10000)
        {
            noteadd(
                ""s + ranktitle(cnt) + u8" Rank."s + gdata((120 + cnt)) / 100);
            s = lang(
                u8"給料: 約 "s + calcincome(cnt) + u8" gold  "s,
                u8"Pay: About "s + calcincome(cnt) + u8" gold pieces "s);
            gold += calcincome(cnt);
            if (cnt != 3 && cnt != 4 && cnt != 5 && cnt != 8)
            {
                s += lang(u8"ノルマ: "s, u8"\nDeadline: "s) + gdata((140 + cnt))
                    + lang(u8"日以内"s, u8" Days left"s);
            }
            noteadd(s);
            noteadd(""s);
        }
    }
    noteadd(lang(
        u8"EXバトル: 勝利 "s + gdata(802) + u8"回 最高Lv"s
            + gdata_ex_arena_level,
        u8"EX Arena Wins:"s + gdata(802) + u8"  Highest Level:"s
            + cnvrank(gdata_ex_arena_level)));
    noteadd(""s);
    for (int cnt = 0,
             cnt_end = cnt + (pagesize / 2 - noteinfo() % (pagesize / 2));
         cnt < cnt_end;
         ++cnt)
    {
        noteadd(""s);
    }
    noteadd(u8" - Income & Expense - "s);
    noteadd(""s);
    if (jp)
    {
        noteadd(u8"◆ 給料(毎月1日と15日に支給)"s);
        noteadd(u8"@BL　合計　　 : 約 "s + gold + i18n::_(u8"ui", u8"gold"));
        noteadd(""s);
        noteadd(u8"◆ 請求書内訳(毎月1日に発行)"s);
        noteadd(
            u8"@RE　人件費  : 約 "s + gdata_cost_to_hire
            + i18n::_(u8"ui", u8"gold"));
        noteadd(
            u8"@RE　運営費  : 約 "s + calccostbuilding()
            + i18n::_(u8"ui", u8"gold"));
        noteadd(
            u8"@RE　税金    : 約 "s + calccosttax()
            + i18n::_(u8"ui", u8"gold"));
        noteadd(
            u8"@RE　合計　  : 約 "s
            + (gdata_cost_to_hire + calccostbuilding() + calccosttax())
            + i18n::_(u8"ui", u8"gold"));
        noteadd(""s);
        noteadd(u8"現在未払いの請求書は"s + gdata_left_bill + u8"枚"s);
    }
    else
    {
        noteadd(u8"Salary (Paid every 1st and 15th day)"s);
        noteadd(u8"@BL  Sum    : About "s + gold + u8" GP"s);
        noteadd(""s);
        noteadd(u8"Bills  (Issued every 1st day)"s);
        noteadd(u8"@RE  Labor  : About "s + gdata_cost_to_hire + u8" GP"s);
        noteadd(u8"@RE  Maint. : About "s + calccostbuilding() + u8" GP"s);
        noteadd(u8"@RE  Tax    : About "s + calccosttax() + u8" GP"s);
        noteadd(
            u8"@RE  Sum    : About "s
            + (gdata_cost_to_hire + calccostbuilding() + calccosttax())
            + u8" GP"s);
        noteadd(""s);
        noteadd(u8"You have "s + gdata_left_bill + u8" unpaid bills."s);
    }
    for (int cnt = 0,
             cnt_end = cnt + (pagesize / 2 - noteinfo() % (pagesize / 2));
         cnt < cnt_end;
         ++cnt)
    {
        noteadd(""s);
    }
    noteadd(u8" - Completed Quests - "s);
    noteadd(""s);
    append_subquest_journal(1);
    listmax = noteinfo();
    showtitle(lang(u8"ジャーナル"s, u8"Journal"s), strhint2 + strhint3, 236, 1);
    drawmenu(2);
    wx = (windoww - 736) / 2 + inf_screenx;
    wy = winposy(448);
    snd(59);
    windowanime(wx, wy, 736, 448, 10, 4);
label_1973_internal:
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
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"book.bmp", 1);
    gsel(0);
    pos(wx, wy);
    gcopy(4, 0, 0, 736, 448);
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        x = wx + 80 + cnt / 20 * 306;
        y = wy + 45 + cnt % 20 * 16;
        noteget(s, p);
        if (strmid(s, 0, 1) == u8"@"s)
        {
            s(1) = strmid(s, 1, 2);
            s = strmid(s, 3, s(0).size() - 3);
            font(10 + en - en * 2, snail::font_t::style_t::bold);
            color(0, 0, 200);
            if (s(1) == u8"QL"s)
            {
                color(100, 100, 0);
            }
            if (s(1) == u8"QC"s)
            {
                color(0, 100, 100);
            }
            if (s(1) == u8"QM"s)
            {
                color(0, 100, 0);
            }
            if (s(1) == u8"RE"s)
            {
                color(100, 0, 0);
                font(12 + sizefix - en * 2);
            }
            if (s(1) == u8"BL"s)
            {
                color(0, 0, 100);
                font(12 + sizefix - en * 2);
            }
        }
        else
        {
            font(12 + sizefix - en * 2);
        }
        pos(x, y);
        mes(s);
        color(0, 0, 0);
        if (p % 20 == 0)
        {
            font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
            pos(x + 90, y + 330);
            mes(u8"- "s + (p / 20 + 1) + u8" -"s);
            if (p % 40 == 20)
            {
                if (page < pagemax)
                {
                    pos(x + 200, y + 330);
                    mes(u8"(more)"s);
                }
            }
        }
    }
label_1974_internal:
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 2)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 2;
                }
            }
            key = "";
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    result.turn_result = turn_result_t::show_message_log;
                    return result;
                }
                if (curmenu == 1)
                {
                    result.turn_result = turn_result_t::menu_journal;
                    return result;
                }
                if (curmenu == 2)
                {
                    result.turn_result = turn_result_t::show_chat_history;
                    return result;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(71);
            ++page;
            goto label_1973_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(71);
            --page;
            goto label_1973_internal;
        }
    }
    if (key == key_cancel || key == key_enter)
    {
        menucycle = 0;
        update_screen();
        result.turn_result = turn_result_t::pc_turn_user_error;
        return result;
    }
    goto label_1974_internal;
}

turn_result_t show_quest_board()
{
    if (config::instance().extrahelp)
    {
        if (gdata(204) == 0)
        {
            if (mode == 0)
            {
                if (cdata[0].continuous_action_turn == 0)
                {
                    gdata(204) = 1;
                    ghelp = 4;
                    show_ex_help();
                    screenupdate = -1;
                    update_screen();
                }
            }
        }
    }
    quest_refresh_list();
    listmax = 0;
    page = 0;
    pagesize = 4;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    for (int cnt = 0, cnt_end = (gdata_number_of_existing_quests);
         cnt < cnt_end;
         ++cnt)
    {
        if (qdata(1, cnt) != gdata_current_map)
        {
            continue;
        }
        if (qdata(3, cnt) == 0)
        {
            continue;
        }
        if (qdata(8, cnt) != 0)
        {
            continue;
        }
        rc = qdata(0, cnt);
        if (cdata[rc].state != 1)
        {
            continue;
        }
        list(0, listmax) = cnt;
        list(1, listmax) = qdata(5, cnt);
        ++listmax;
    }
    if (listmax == 0)
    {
        txt(lang(
            u8"新しい依頼は掲示されていないようだ。"s,
            u8"It seems there are no new notices."s));
        return turn_result_t::turn_end;
    }
    sort_list_by_column1();
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_board.bmp", 1);
    gsel(0);
    gsel(4);
    fillbg(3, 960, 96, 128, 128);
    ww = 560;
    int h = 140;
    wh = h * 4;
    wx = (windoww - ww) / 2 + inf_screenx;
    wy = winposy(wh);
    for (int cnt = 0; cnt < 4; ++cnt)
    {
        y = wy + cnt * 120;
        window(wx + 4, y + 4, ww, h, 0, -1);
        window(wx, y, ww, h);
        pos(wx + 20, y + 8);
        gcopy(3, 960, 240, 48, 84);
    }
    render_hud();
    gsel(0);
label_1977_internal:
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
label_1978_internal:
    s(0) = lang(u8"掲載されている依頼"s, u8"Notice Board"s);
    s(1) = strhint2 + strhint3b;
    gmode(0);
    pos(0, 0);
    gcopy(4, 0, 0, windoww, inf_ver);
    gmode(2);
    font(16 - en * 2);
    pos(wx + ww + 20, wy);
    bmes(u8"Page "s + (page + 1) + u8"/"s + (pagemax + 1), 255, 255, 255);
    keyrange = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        y = wy + cnt * 120 + 20;
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        pos(wx + 70, y);
        gfini(460, 18);
        gfdec2(12, 14, 16);
        display_key(wx + 70, y - 2, cnt);
    }
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        y = wy + cnt * 120 + 20;
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        rq = list(0, p);
        tc = qdata(0, rq);
        quest_set_data(0);
        p = pagesize * page + cnt;
        font(14 - en * 2);
        cs_list(cs == cnt, s(3), wx + 96, y - 1, 19);
        s(2) = u8"("s + nquestdate + u8")"s;
        pos(wx + 344, y + 2);
        mes(s(2));
        s(4) = cdatan(0, tc);
        cutname(s(4), 20);
        pos(wx + 392, y + 2);
        mes(s(4));
        talk_conv(buff, 70);
        p(0) = qdata(5, rq) / 5 + 1;
        p(1) = 14;
        for (int cnt = 0; cnt < 1; ++cnt)
        {
            if (cdata[0].level * 2 < qdata(5, rq))
            {
                color(205, 0, 0);
                break;
            }
            if (cdata[0].level * 3 / 2 < qdata(5, rq))
            {
                color(140, 80, 0);
                break;
            }
            if (cdata[0].level < qdata(5, rq))
            {
                color(0, 0, 205);
                break;
            }
            color(0, 155, 0);
        }
        if (p < 11)
        {
            dy = 0;
            if (p > 5)
            {
                font(10 - en * 2);
                dy = -3;
            }
            for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
            {
                pos(wx + 270 + cnt % 5 * 13, y + dy + cnt / 5 * 8 + 2);
                mes(lang(u8"★"s, u8"$"s));
            }
        }
        else
        {
            pos(wx + 270, y + 2);
            mes(lang(u8"★?"s, u8"$ x "s) + p);
        }
        color(0, 0, 0);
        font(13 - en * 2);
        pos(wx + 20, y + 20);
        mes(buff);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        txtnew();
        txt(lang(u8"依頼主に会う？"s, u8"Do you want to meet the client?"s));
        ELONA_YES_NO_PROMPT();
        rtval = show_prompt(promptx, prompty, 160);
        if (rtval != 0)
        {
            goto label_1977_internal;
        }
        // TODO move the below somewhere else to decouple questteleport
        tc = qdata(0, p);
        rq = p;
        client = tc;
        efid = 619;
        magic();
        tc = client;
        questteleport = 1;
        talk_to_npc();
        if (chatteleport == 1)
        {
            chatteleport = 0;
            return turn_result_t::exit_map;
        }
        return turn_result_t::turn_end;
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_1977_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1977_internal;
        }
    }
    if (key == key_cancel)
    {
        return turn_result_t::turn_end;
    }
    goto label_1978_internal;
}

int show_hire_menu()
{
    snd(26);
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
    {
        if (allyctrl == 2)
        {
            if (cdata[cnt].state != 6 && cdata[cnt].state != 2)
            {
                continue;
            }
        }
        else
        {
            if (allyctrl == 1)
            {
                if (allyctrl == 1)
                {
                    if (cdata[cnt].state != 10)
                    {
                        continue;
                    }
                }
            }
            else if (cdata[cnt].state != 1)
            {
                continue;
            }
            if (cnt < 16)
            {
                if (cdata[cnt].current_map != gdata_current_map)
                {
                    continue;
                }
            }
        }
        if (cnt == 0)
        {
            continue;
        }
        if (cdata[cnt].is_escorted_in_sub_quest() == 1)
        {
            continue;
        }
        list(0, listmax) = cnt;
        list(1, listmax) = -cdata[cnt].level;
        ++listmax;
    }
    sort_list_by_column1();
    windowshadow = 1;
label_1981_internal:
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
label_1982_internal:
    s(0) = lang(u8"NPC一覧"s, u8"NPC List"s);
    s(1) = strhint2 + strhint3;
    display_window((windoww - 700) / 2 + inf_screenx, winposy(448), 700, 448);
    if (allyctrl == 0)
    {
        s = lang(u8"給料"s, u8"Wage"s);
    }
    else
    {
        s = lang(u8"雇用費(給料)"s, u8"Init. Cost(Wage)"s);
    }
    display_topic(lang(u8"NPCの名前"s, u8"Name"s), wx + 28, wy + 36);
    display_topic(lang(u8"情報"s, u8"Info"s), wx + 350, wy + 36);
    if (allyctrl != 2)
    {
        display_topic(s, wx + 490, wy + 36);
    }
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
        if (cnt % 2 == 0)
        {
            pos(wx + 70, wy + 66 + cnt * 19);
            gfini(600, 18);
            gfdec2(12, 14, 16);
        }
        display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
    }
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        i(0) = cdata[list(0, p)].image % 1000;
        i(1) = cdata[list(0, p)].image / 1000;
        chara_preparepic(i, i(1));
        pos(wx + 40, wy + 74 + cnt * 19 - 8);
        gmode(2, chipc(2, i), chipc(3, i));
        grotate(
            5,
            0,
            960,
            0,
            chipc(2, i) / (1 + (chipc(3, i) > inf_tiles)),
            inf_tiles);
        i = list(0, p);
        s = cdatan(0, i);
        cutname(s, 36);
        cs_list(cs == cnt, s, wx + 84, wy + 66 + cnt * 19 - 1);
        s = u8"Lv."s + cdata[i].level + u8" "s;
        if (cdata[i].sex == 0)
        {
            s += cnven(i18n::_(u8"ui", u8"male"));
        }
        else
        {
            s += cnven(i18n::_(u8"ui", u8"female"));
        }
        s += lang(u8" "s, u8"("s) + calcage(i) + ""s + lang(u8"歳"s, u8")"s);
        pos(wx + 372, wy + 66 + cnt * 19 + 2);
        mes(s);
        if (allyctrl != 2)
        {
            if (allyctrl == 1)
            {
                s = ""s + calchirecost(i) * 20 + u8"("s + calchirecost(i)
                    + u8")"s;
            }
            else
            {
                s = ""s + calchirecost(i);
            }
            pos(wx + 512, wy + 66 + cnt * 19 + 2);
            mes(s + lang(u8" gold"s, u8"gp"s));
        }
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        return p;
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_1981_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1981_internal;
        }
    }
    if (key == key_cancel)
    {
        return -1;
    }
    goto label_1982_internal;
}

int show_spell_writer_menu()
{
    snd(26);
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    for (int cnt = 0; cnt < 800; ++cnt)
    {
        if (itemmemory(2, cnt) == 0)
        {
            continue;
        }
        list(0, listmax) = cnt;
        list(1, listmax) = cnt;
        ++listmax;
    }
    sort_list_by_column1();
    windowshadow = 1;
label_1985_internal:
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
label_1986_internal:
    s(0) = lang(u8"予約リスト"s, u8"Reserve List"s);
    s(1) = strhint2 + strhint3;
    display_window((windoww - 540) / 2 + inf_screenx, winposy(448), 540, 448);
    display_topic(lang(u8"アイテムの名前"s, u8"Name"s), wx + 28, wy + 36);
    display_topic(lang(u8"予約状況"s, u8"Status"s), wx + 390, wy + 36);
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
        if (cnt % 2 == 0)
        {
            pos(wx + 70, wy + 66 + cnt * 19);
            gfini(440, 18);
            gfdec2(12, 14, 16);
        }
        display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
    }
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
        prepare_item_image(429, 0);
        pos(wx + 38, wy + 73 + cnt * 19);
        gmode(2, inf_tiles, inf_tiles);
        grotate(1, 0, 960, 0, inf_tiles, inf_tiles);
        s = ioriginalnameref(i);
        cs_list(cs == cnt, s, wx + 84, wy + 66 + cnt * 19 - 1);
        pos(wx + 400, wy + 66 + cnt * 19 + 2);
        if (itemmemory(2, i) == 1)
        {
            color(120, 120, 120);
            mes(lang(u8"入荷なし"s, u8"-"s));
            color(0, 0, 0);
        }
        else
        {
            color(55, 55, 255);
            mes(lang(u8"入荷予定"s, u8"Reserved"s));
            color(0, 0, 0);
        }
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        if (p == 289 || p == 732)
        {
            snd(27);
            txt(lang(
                u8"「その本は入手できないね」"s,
                u8"\"Ah, that book is unavailable.\""s));
            goto label_1985_internal;
        }
        snd(20);
        if (itemmemory(2, p) == 1)
        {
            itemmemory(2, p) = 2;
        }
        else
        {
            itemmemory(2, p) = 1;
        }
        goto label_1985_internal;
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_1985_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1985_internal;
        }
    }
    if (key == key_cancel)
    {
        return -1;
    }
    goto label_1986_internal;
}

void list_adventurers()
{
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    for (int cnt = 0; cnt < 56; ++cnt)
    {
        if (cdata[cnt].state == 0)
        {
            continue;
        }
        list(0, listmax) = cnt;
        list(1, listmax) = -cdata[cnt].fame;
        ++listmax;
    }
    sort_list_by_column1();
    windowshadow = 1;
label_1989_internal:
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
label_1990_internal:
    s(0) = lang(u8"冒険者ランク"s, u8"Adventurer Rank"s);
    s(1) = strhint2 + strhint3;
    display_window((windoww - 640) / 2 + inf_screenx, winposy(448), 640, 448);
    display_topic(
        lang(u8"冒険者の名前とランク"s, u8"Name and Rank"s), wx + 28, wy + 36);
    display_topic(lang(u8"名声(Lv)"s, u8"Fame(Lv)"s), wx + 320, wy + 36);
    display_topic(lang(u8"現在地"s, u8"Location"s), wx + 420, wy + 36);
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
        if (cnt % 2 == 0)
        {
            pos(wx + 70, wy + 66 + cnt * 19);
            gfini(540, 18);
            gfdec2(12, 14, 16);
        }
        display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
    }
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        i(0) = cdata[list(0, p)].image % 1000;
        i(1) = cdata[list(0, p)].image / 1000;
        chara_preparepic(i, i(1));
        pos(wx + 40, wy + 74 + cnt * 19 - 8);
        gmode(2, chipc(2, i), chipc(3, i));
        grotate(
            5,
            0,
            960,
            0,
            chipc(2, i) / (1 + (chipc(3, i) > inf_tiles)),
            inf_tiles);
        pos(wx + 84, wy + 66 + cnt * 19 + 2);
        mes(cnvrank(p + 1) + lang(u8"位"s, ""s));
        i = list(0, p);
        s = ""s + cdatan(1, i) + u8" "s + cdatan(0, i);
        cutname(s, 26);
        cs_list(cs == cnt, s, wx + 118, wy + 66 + cnt * 19 - 1);
        s = ""s + cdata[i].fame + u8"("s + cdata[i].level + u8")"s;
        pos(wx + 402 - strlen_u(s) * 7, wy + 66 + cnt * 19 + 2);
        mes(s);
        s = mapname(cdata[i].current_map);
        if (s == ""s)
        {
            s = lang(u8"不明"s, u8"Unknown"s);
        }
        if (cdata[i].state == 4)
        {
            s = lang(u8"病院"s, u8"Hospital"s);
        }
        pos(wx + 435, wy + 66 + cnt * 19 + 2);
        mes(s);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_1989_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1989_internal;
        }
    }
    if (key == key_cancel)
    {
        return;
    }
    goto label_1990_internal;
}

int select_alias(int val0)
{
    cs = 0;
    cs_bk = -1;
    list(0, 0) = -1;
    windowshadow = 1;
    i = 10500;

    while (1)
    {
        if (cs != cs_bk)
        {
            s(0) = lang(u8"異名の選択"s, u8"Alias Selection"s);
            s(1) = strhint3b;
            display_window(
                (windoww - 400) / 2 + inf_screenx, winposy(458), 400, 458);
            display_topic(
                lang(u8"異名の候補"s, u8"Alias List"s), wx + 28, wy + 30);
            font(14 - en * 2);
            for (int cnt = 0; cnt < 17; ++cnt)
            {
                key_list(cnt) = key_select(cnt);
                keyrange = cnt + 1;
                if (val0 == 3)
                {
                    randomize(i + cnt);
                }
                if (list(0, 0) == -1)
                {
                    listn(0, cnt) = random_title(val0);
                    list(1, cnt) = i + cnt;
                }
                if (cnt == 0)
                {
                    listn(0, cnt) = lang(u8"別の名前を考える"s, u8"Reroll"s);
                }
                pos(wx + 38, wy + 66 + cnt * 19 - 2);
                gcopy(3, cnt * 24 + 72, 30, 24, 18);
                cs_list(
                    cs == cnt, listn(0, cnt), wx + 64, wy + 66 + cnt * 19 - 1);
            }
            i += 17;
            cs_bk = cs;
            list(0, 0) = 0;
        }
        redraw();
        await(config::instance().wait1);
        key_check();
        cursor_check();
        ELONA_GET_SELECTED_INDEX_THIS_PAGE(p);
        if (p != -1)
        {
            if (key == key_select(0))
            {
                list(0, 0) = -1;
                snd(103);
                cs_bk = -1;
            }
            else
            {
                if (val0 == 3)
                {
                    return p;
                }
                cmaka = listn(0, p);
                return 1;
            }
        }
        if (key == key_cancel)
        {
            snd(26);
            return 0;
        }
    }
}

void show_city_chart()
{
label_22711:
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    snd(99);
    curmenu = 0;
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_politics.bmp", 1);
    gsel(0);
    fillbg(3, 960, 96, 128, 128);
    render_hud();
    drawmenu(3);
    windowshadow = 1;
    city = adata(28, gdata_current_map);
    lv = 0;
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
label_2272_internal:
    s(0) = lang(u8"ポストチャート"s, u8"City Chart"s);
    s(1) = strhint3b;
    display_window((windoww - 580) / 2 + inf_screenx, winposy(400), 580, 400);
    keyrange = 0;
    int j0 = 0;
    int n = 0;
    cs_listbk();
    if (adata(28, gdata_current_map) == 0 || gdata_current_dungeon_level != 1)
    {
        font(14 - en * 2);
        pos(wx + 40, wy + 50);
        mes(lang(
            u8"この場所には経済活動がない。"s,
            u8"There's no economy running in this area."s));
    }
    else
    {
        display_topic(
            lang(
                mapname(adata(16, gdata_current_map)) + u8"のチャート"s,
                ""s + mapname(adata(16, gdata_current_map)) + u8" City Chart"s),
            wx + 40,
            wy + 34);
        for (int cnt = 0;; ++cnt)
        {
            if (pochart(j0, n, lv) == 0 || cnt == 0)
            {
                if (cnt != 0)
                {
                    ++n;
                }
                j0 = 0;
                i = 0;
                for (int cnt = 0; cnt < 10; ++cnt)
                {
                    if (pochart(cnt, n, lv) != 0)
                    {
                        ++i;
                    }
                }
                if (i == 0)
                {
                    break;
                }
                y = wy + 70 + n * 55;
            }
            x = wx + (ww - 70) / (i + 1) * (j0 + 1);
            pos(x - 26, y - 3);
            gcopy(3, 960, 288, 144, 24);
            p = pochart(j0, n, lv);
            key_list(cnt) = key_select(cnt);
            ++keyrange;
            display_key(x - 30, y + 21, cnt);
            font(12 + sizefix - en * 2);
            pos(x - 2, y + jp * 2);
            bmes(cnven(popostname(p)), 255, 255, 255);
            font(14 - en * 2);
            if (podata(0 + cnt, city) == 0)
            {
                s = lang(u8"不在"s, u8"Empty"s);
            }
            cs_list(cs == cnt, s, x - 2, y + 20);
            ++j0;
        }
        if (keyrange != 0)
        {
            cs_bk = cs;
        }
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        return;
    }
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 2)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 2;
                }
            }
            key = "";
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    goto label_22711;
                }
                if (curmenu == 1)
                {
                    show_economy_window();
                    return;
                }
                if (curmenu == 2)
                {
                    show_politics_window();
                    return;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_22711;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_22711;
        }
    }
    if (key == key_cancel)
    {
        update_screen();
        return;
    }
    goto label_2272_internal;
}

void showeconomy(
    int prm_1086,
    int prm_1087,
    const std::string& prm_1088,
    int prm_1089,
    int prm_1090)
{
    int p_at_m195 = 0;
    pos(prm_1086, prm_1087);
    mes(prm_1088);
    pos(prm_1086 + 130, prm_1087);
    mes(""s + prm_1089);
    p_at_m195 = prm_1089 - prm_1090;
    if (p_at_m195 >= 0)
    {
        color(0, 0, 150);
    }
    else
    {
        color(150, 0, 0);
    }
    pos(prm_1086 + 130 + ginfo(14) + 12, prm_1087);
    mes(u8"("s + p_at_m195 + u8")"s);
    color(0, 0, 0);
    return;
}

void show_economy_window()
{
    curmenu = 1;
    key_list(0) = key_enter;
    keyrange = 0;
    pagesize = 1;
    listmax = 2;
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_politics.bmp", 1);
    gsel(0);
    fillbg(3, 960, 96, 128, 128);
    render_hud();
    gsel(7);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"ie_scroll.bmp");
    gsel(0);
    windowshadow = 1;
    snd(92);
    drawmenu(3);
    city = 1;
    ww = 540;
    wh = 440;
    wx = (windoww - ww) / 2 + inf_screenx;
    wy = winposy(wh);
label_2277_internal:
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
label_2278_internal:
    s = strhint2 + strhint3b;
    showscroll(s, wx, wy, ww, wh);
    font(14 - en * 2);
    if (adata(28, gdata_current_map) == 0 || gdata_current_dungeon_level != 1)
    {
        pos(wx + 40, wy + 60);
        mes(lang(
            u8"この場所には経済活動がない。"s,
            u8"There's no economy running in this area."s));
    }
    else
    {
        if (page == 0)
        {
            display_topic(
                lang(u8"街の概要"s, u8"Town Information"s), wx + 65, wy + 50);
            display_topic(
                lang(u8"街の財政"s, u8"Town Finance"s), wx + 65, wy + 150);
            font(14 - en * 2);
            x = wx + 50;
            y = wy + 80;
            showeconomy(
                x,
                y,
                lang(u8"人口"s, u8"Population"s),
                podata(100, city),
                podata(101, city));
            x = wx + 50;
            y = wy + 180;
            showeconomy(
                x,
                y,
                lang(u8"基本税"s, u8"Basic Tax"s) + u8" ("s + gdata(820)
                    + u8"%)"s,
                podata(102, city),
                podata(103, city));
            showeconomy(
                x,
                y + 16,
                lang(u8"消費税"s, u8"Excise Tax"s) + u8" ("s + podata(150, city)
                    + u8"%)"s,
                podata(104, city),
                podata(105, city));
        }
        if (page == 1)
        {
            display_topic(
                lang(u8"人口推移の詳細"s, u8"Population Detail"s),
                wx + 65,
                wy + 50);
            display_topic(
                lang(u8"収支の詳細"s, u8"Finance Detail"s), wx + 65, wy + 200);
            font(14 - en * 2);
        }
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 2)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 2;
                }
            }
            key = "";
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    show_city_chart();
                    return;
                }
                if (curmenu == 1)
                {
                    show_economy_window();
                    return;
                }
                if (curmenu == 2)
                {
                    show_politics_window();
                    return;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2277_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2277_internal;
        }
    }
    if (key != ""s)
    {
        update_screen();
        return;
    }
    goto label_2278_internal;
}

void show_politics_window()
{
    // TODO: untranslated
    listmax = 0;
    page = 0;
    pagesize = 13;
    cs = 0;
    cs_bk = -1;
    curmenu = 2;
    city = adata(28, gdata_current_map);
    list(0, listmax) = 1;
    listn(0, listmax) = u8"この国の首都は"s + mapname(gdata(815)) + u8"だ。"s;
    ++listmax;
    if (mdata(6) != 3)
    {
        goto label_2281_internal;
    }
    list(0, listmax) = 0;
    listn(0, listmax) = u8"この街の消費税は"s + podata(150, city) + u8"%だ。"s;
    ++listmax;
    p = rnd(1000);
    list(0, listmax) = 0;
    listn(0, listmax) =
        u8"この街の井戸水の汚染は深刻だ(死者"s + p + u8"人）。"s;
    ++listmax;
    list(0, listmax) = 0;
    listn(0, listmax) = u8"この街では殺人が許される。"s;
    ++listmax;
label_2281_internal:
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_politics.bmp", 1);
    gsel(0);
    fillbg(3, 960, 96, 128, 128);
    render_hud();
    gsel(7);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"ie_scroll.bmp");
    gsel(0);
    windowshadow = 1;
    snd(92);
    drawmenu(3);
    city = 1;
    ww = 480;
    wh = 400;
    wx = (windoww - ww) / 2 + inf_screenx;
    wy = winposy(wh);
label_2282_internal:
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
label_2283_internal:
    s = strhint2 + strhint3b;
    showscroll(s, wx, wy, ww, wh);
    display_topic(lang(u8"法律"s, u8"Law"s), wx + 65, wy + 45);
    font(12 + sizefix - en * 2);
    pos(wx + 185, wy + 52);
    mes(lang(u8"国法"s, u8"Global"s));
    if (mdata(6) == 3)
    {
        pos(wx + 285, wy + 52);
        mes(lang(
            ""s + mapname(gdata_current_map) + u8"の法"s,
            u8"Law of "s + mapname(gdata_current_map) + u8")"s));
    }
    pos(wx + 155, wy + 46);
    gmode(2);
    gcopy(3, 312, 360, 24, 24);
    pos(wx + 255, wy + 46);
    gmode(2);
    gcopy(3, 288, 360, 24, 24);
    font(14 - en * 2);
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
        if (cnt % 2 == 0)
        {
            pos(wx + 74, wy + 76 + cnt * 19);
            gfini(365, 18);
            gfdec2(12, 14, 16);
        }
        display_key(wx + 72, wy + 76 + cnt * 19 - 2, cnt);
    }
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
        s = listn(0, p);
        cs_list(cs == cnt, s, wx + 100, wy + 76 + cnt * 19 - 1, 0, 0);
        pos(wx + 42, wy + 68 + cnt * 19 + 2);
        gmode(2);
        gcopy(3, 288 + list(0, p) * 24, 360, 24, 24);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 2)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 2;
                }
            }
            key = "";
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    show_city_chart();
                    return;
                }
                if (curmenu == 1)
                {
                    show_economy_window();
                    return;
                }
                if (curmenu == 2)
                {
                    show_politics_window();
                    return;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2282_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2282_internal;
        }
    }
    if (key == key_cancel)
    {
        update_screen();
        return;
    }
    goto label_2283_internal;
}

void begin_to_believe_god()
{
    cs = 0;
    page = 0;
    pagemax = 0;
    pagesize = 16;
    cs_bk = -1;
    key = "";
    objprm(0, ""s);
    keylog = "";
    listmax = 0;
    chatesc = 2;
    if (!cdata[0].god_id.empty())
    {
        if (inv[ci].param1 == 0)
        {
            s = lang(u8"信仰を捨てる"s, u8"Abandon God"s);
        }
        else
        {
            s = lang(
                i18n::_(u8"god", core_god::int2godid(inv[ci].param1), u8"name")
                    + u8"に改宗する"s,
                u8"Convert to "s
                    + i18n::_(
                          u8"god",
                          core_god::int2godid(inv[ci].param1),
                          u8"name"));
        }
        list(0, listmax) = 0;
        listn(0, listmax) = s;
        ++listmax;
    }
    else
    {
        list(0, listmax) = 0;
        listn(0, listmax) = lang(
            i18n::_(u8"god", core_god::int2godid(inv[ci].param1), u8"name")
                + u8"を信仰する"s,
            u8"Believe in "s
                + i18n::_(
                      u8"god", core_god::int2godid(inv[ci].param1), u8"name"));
        ++listmax;
    }
    list(0, listmax) = 2;
    listn(0, listmax) = lang(u8"やめる"s, u8"Cancel"s);
    ++listmax;
    snd(62);
    gsel(4);
    gmode(0);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"bg_altar.bmp", 1);
    pos(0, 0);
    gzoom(4, 0, 0, 600, 400, windoww, windowh - inf_verh);
    gsel(0);
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
label_1887_internal:
    gmode(0);
    pos(0, 0);
    gcopy(4, 0, 0, windoww, windowh - inf_verh);
    gmode(2);
    render_hud();
    dx = 520;
    dy = 270;
    window2((windoww - dx) / 2 + inf_screenx, winposy(dy), dx, dy, 4, 6);
    wx = (windoww - dx) / 2 + inf_screenx;
    wy = winposy(dy);
    font(18 - en * 2, snail::font_t::style_t::bold);
    pos(wx + 20, wy + 20);
    bmes(
        lang(u8"《 "s, u8"< "s)
            + i18n::_(u8"god", core_god::int2godid(inv[ci].param1), u8"name")
            + lang(u8" 》"s, u8" >"s),
        255,
        255,
        255);
    get_god_description();
    gmes(buff, wx + 23, wy + 60, dx - 60, {30, 30, 30}, true);
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
    {
        p = cnt;
        i = list(0, p);
        display_key(wx + 50, wy + dy + cnt * 20 - listmax * 20 - 18, cnt);
        s = listn(0, p);
        cs_list(cs == cnt, s, wx + 80, wy + dy + cnt * 20 - listmax * 20 - 18);
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
        label_1888();
        return;
    }
    goto label_1887_internal;
}


void label_1958()
{
    screenupdate = -1;
    update_screen();
    return;
}



int ctrl_ally()
{
    snd(26);
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    if (allyctrl == 2)
    {
        list(0, listmax) = 99;
        list(1, listmax) = -9999;
        ++listmax;
    }
    for (int cnt = 0; cnt < 16; ++cnt)
    {
        if (cdata[cnt].state == 0)
        {
            continue;
        }
        if (allyctrl == 1)
        {
            if (cdata[cnt].state == 6)
            {
                continue;
            }
        }
        if (allyctrl == 3 || allyctrl == 5)
        {
            if (cdata[cnt].state != 1)
            {
                continue;
            }
        }
        if (allyctrl == 5)
        {
            if (cnt == rc)
            {
                continue;
            }
        }
        if (cdata[cnt].current_map != 0)
        {
            if (allyctrl == 1 || allyctrl == 2)
            {
                continue;
            }
        }
        if (allyctrl != 1)
        {
            if (cdata[cnt].is_escorted() == 1)
            {
                continue;
            }
        }
        if (cdata[cnt].is_ridden())
        {
            continue;
        }
        if (cnt == 0)
        {
            continue;
        }
        list(0, listmax) = cnt;
        list(1, listmax) = -cdata[cnt].level;
        ++listmax;
    }
    sort_list_by_column1();
    if (allyctrl == 2)
    {
        p = 0;
        DIM2(followerin, 16);
        for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
        {
            if (cnt == 0)
            {
                continue;
            }
            i = list(0, cnt);
            if (cdata[i].state != 6)
            {
                followerin(i) = 1;
                ++p;
                if (p == arenaop(1))
                {
                    break;
                }
            }
        }
    }
    windowshadow = 1;
label_1960_internal:
    cs_bk = -1;
    pagemax = (listmax - 1) / pagesize;
    if (page < 0)
    {
        page = pagemax;
    }
    else if (page > pagemax)
    {
        page = 0;
        txtnew();
    }
    if (allyctrl == 0)
    {
        txt(lang(u8"誰を呼び戻す？"s, u8"Call who?"s));
        s(10) = lang(u8"呼び戻す仲間"s, u8"Ally List"s);
        s(11) = strhint2 + strhint3;
        s(12) = lang(u8"仲間の情報"s, u8"Name"s);
        s(13) = lang(u8"状態"s, u8"Status"s);
        x = 0;
    }
    if (allyctrl == 1)
    {
        txt(lang(u8"誰を売り飛ばす？"s, u8"Sell who?"s));
        s(10) = lang(u8"売り飛ばす仲間"s, u8"Ally List"s);
        s(11) = strhint2 + strhint3;
        s(12) = lang(u8"仲間の情報"s, u8"Name"s);
        s(13) = lang(u8"値段"s, u8"Value"s);
        x = 20;
    }
    if (allyctrl == 2)
    {
        i = 0;
        for (int cnt = 0; cnt < 16; ++cnt)
        {
            i += followerin(cnt) == 1;
        }
        if (i < arenaop(1))
        {
            txtef(3);
        }
        else
        {
            txtef(4);
        }
        txt(lang(u8"試合の規定人数: "s, u8"Participant: "s) + i + u8" / "s
            + arenaop(1));
        s(10) = lang(u8"出場する仲間"s, u8"Ally List"s);
        s(11) = strhint2 + strhint3;
        s(12) = lang(u8"仲間の情報"s, u8"Name"s);
        s(13) = lang(u8"状態"s, u8"Status"s);
        x = 20;
    }
    if (allyctrl == 3)
    {
        if (adata(16, gdata_current_map) == 102)
        {
            txt(lang(
                u8"誰を店長にする？"s, u8"Who takes the role of shopkeeper?"s));
            s(10) = lang(u8"店長候補"s, u8"Ally List"s);
            s(11) = strhint2 + strhint3;
            s(12) = lang(u8"仲間の情報"s, u8"Name"s);
            s(13) = lang(u8"魅力/交渉"s, u8"CHR/Negotiation"s);
        }
        if (adata(16, gdata_current_map) == 31)
        {
            txt(lang(
                u8"誰をブリーダーにする？"s,
                u8"Who takes the role of breeder?"s));
            s(10) = lang(u8"ブリーダー候補"s, u8"Ally List"s);
            s(11) = strhint2 + strhint3;
            s(12) = lang(u8"仲間の情報"s, u8"Name"s);
            s(13) = lang(u8"繁殖力"s, u8"Breed Power"s);
        }
        if (gdata_current_map == 7)
        {
            txt(lang(u8"誰を滞在させる？"s, u8"Who stays in your home?"s));
            s(10) = lang(u8"滞在状態の変更"s, u8"Ally List"s);
            s(11) = strhint2 + strhint3;
            s(12) = lang(u8"仲間の情報"s, u8"Name"s);
            s(13) = lang(u8"状態"s, u8"Status"s);
        }
        x = 20;
    }
    if (allyctrl == 4 || allyctrl == 5)
    {
        txt(lang(u8"対象にする仲間は？"s, u8"Who is the subject?"s));
        s(10) = lang(u8"仲間"s, u8"Ally List"s);
        s(11) = strhint2 + strhint3;
        s(12) = lang(u8"仲間の情報"s, u8"Name"s);
        s(13) = lang(u8"状態"s, u8"Status"s);
        if (allyctrl == 5)
        {
            if (rc != 0)
            {
                s(13) = lang(u8"獲得部位/技能"s, u8"Body/Skill"s);
            }
        }
        x = 0;
    }
label_1961_internal:
    s(0) = s(10);
    s(1) = s(11);
    display_window((windoww - 620) / 2 + inf_screenx, winposy(400), 620, 400);
    display_topic(s(12), wx + 28, wy + 36);
    display_topic(s(13), wx + 350 + x, wy + 36);
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
        if (cnt % 2 == 0)
        {
            pos(wx + 70, wy + 66 + cnt * 19);
            gfini(500, 18);
            gfdec2(12, 14, 16);
        }
        display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
    }
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        if (list(0, p) != 99)
        {
            i(0) = cdata[list(0, p)].image % 1000;
            i(1) = cdata[list(0, p)].image / 1000;
            chara_preparepic(i, i(1));
            pos(wx + 40, wy + 74 + cnt * 19 - 8);
            gmode(2, chipc(2, i), chipc(3, i));
            grotate(
                5,
                0,
                960,
                0,
                chipc(2, i) / (1 + (chipc(3, i) > inf_tiles)),
                inf_tiles);
        }
        i = list(0, p);
        if (i == 99)
        {
            cs_list(
                cs == cnt,
                lang(u8"決定"s, u8"Proceed"s),
                wx + 84,
                wy + 66 + cnt * 19 - 1);
            continue;
        }
        if (allyctrl == 0 || allyctrl == 2 || allyctrl == 3 || allyctrl == 4
            || allyctrl == 5)
        {
            s = ""s + cdatan(1, i) + u8" "s + cdatan(0, i);
            if (cdata[i].current_map != 0)
            {
                s = s + u8"("s + mapname(cdata[i].current_map) + u8")"s;
            }
            if (allyctrl == 2)
            {
                if (followerin(i) == 1)
                {
                    s += lang(u8" *出場* "s, u8" *In* "s);
                }
            }
            int n = 0;
            if (allyctrl == 5)
            {
                if (cdata[i].level > sdata(151, 0) + 5)
                {
                    n = 3;
                }
            }
            cs_list(cs == cnt, s, wx + 84, wy + 66 + cnt * 19 - 1, 0, n);
            if (allyctrl != 3 || (allyctrl == 3 && gdata_current_map == 7))
            {
                s = u8"Lv."s + cdata[i].level + u8" "s;
                if (cdata[i].state == 6)
                {
                    s += lang(u8"(死んでいる)"s, u8"(Dead"s);
                }
                if (cdata[i].state == 7)
                {
                    if (allyctrl == 0)
                    {
                        s += lang(u8"(待機している)"s, u8"(Waiting)"s);
                    }
                    else
                    {
                        s += u8"(Hp: "s + cdata[i].hp * 100 / cdata[i].max_hp
                            + u8"%) "s + lang(u8"待機"s, u8"Waiting"s);
                    }
                }
                if (cdata[i].state == 1)
                {
                    if (allyctrl == 0)
                    {
                        s += lang(u8"(生きている)"s, u8"(Alive)"s);
                    }
                    else
                    {
                        s += u8"(Hp: "s + cdata[i].hp * 100 / cdata[i].max_hp
                            + u8"%)"s;
                    }
                }
            }
            else
            {
                if (adata(16, gdata_current_map) == 102)
                {
                    s = u8"   "s + sdata(17, i) + u8" / " + s + sdata(156, i);
                }
                if (adata(16, gdata_current_map) == 31)
                {
                    s = u8"   "s + cbreeder(i);
                }
            }
            if (allyctrl == 5)
            {
                if (rc != 0)
                {
                    tc = i;
                    {
                        int stat = transplant_body_parts();
                        if (stat == -1)
                        {
                            s = lang(u8"なし"s, u8"None"s);
                        }
                        else
                        {
                            s = i18n::_(u8"ui", u8"body_part", u8"_"s + rtval);
                        }
                    }
                    s += u8"/"s;
                    {
                        int stat = gain_skills_by_geen_engineering();
                        if (stat == 0)
                        {
                            s += lang(u8"なし"s, u8"None"s);
                        }
                        else
                        {
                            s += ""s
                                + i18n::_(
                                      u8"ability",
                                      std::to_string(rtval),
                                      u8"name");
                            if (rtval(1) != -1)
                            {
                                s += u8", "s
                                    + i18n::_(
                                          u8"ability",
                                          std::to_string(rtval(1)),
                                          u8"name");
                            }
                        }
                    }
                }
            }
            pos(wx + 370, wy + 66 + cnt * 19 + 2);
            mes(s);
        }
        if (allyctrl == 1)
        {
            s = ""s + cdatan(1, i) + u8" "s + cdatan(0, i);
            s += u8" Lv."s + cdata[i].level;
            cs_list(cs == cnt, s, wx + 84, wy + 66 + cnt * 19 - 1);
            s = ""s + calcslavevalue(i) * 2 / 3 + i18n::_(u8"ui", u8"gold");
            pos(wx + 390, wy + 66 + cnt * 19 + 2);
            mes(s);
        }
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        if (allyctrl == 5)
        {
            if (cdata[p].level > sdata(151, 0) + 5)
            {
                snd(27);
                txt(lang(
                    u8"遺伝子学のスキルが足りない。"s,
                    u8"You need to be a better gene engineer."s));
                goto label_1960_internal;
            }
        }
        if (allyctrl == 2)
        {
            i = 0;
            for (int cnt = 0; cnt < 16; ++cnt)
            {
                if (followerin(cnt) == 1)
                {
                    ++i;
                }
            }
            if (p == 99)
            {
                if (i == 0)
                {
                    txt(lang(
                        u8"最低でも一人の参加者が必要だ。"s,
                        u8"You need at least 1 pet to start the battle."s));
                    snd(27);
                    goto label_1961_internal;
                }
                snd(20);
                return 1;
            }
            if (cdata[p].state == 6)
            {
                txt(lang(
                    u8"死んでいる。"s, he(p) + u8" "s + is(p) + u8" dead."s));
                snd(27);
                goto label_1961_internal;
            }
            if (followerin(p) == 0)
            {
                if (i >= arenaop(1))
                {
                    snd(27);
                    txt(lang(
                        u8"参加枠を超えている。"s,
                        u8"Too many participants."s));
                    goto label_1961_internal;
                }
                else
                {
                    followerin(p) = 1;
                    snd(20);
                }
            }
            else
            {
                followerin(p) = 0;
                snd(20);
            }
            goto label_1960_internal;
        }
        return p;
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_1960_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1960_internal;
        }
    }
    if (key == key_cancel)
    {
        return -1;
    }
    goto label_1961_internal;
}



void label_1964()
{
    // TODO: untranslated
    if (rc < 0)
    {
        rc = tc;
        if (rc < 0)
        {
            rc = 0;
        }
    }
    listmax = 0;
    page = 0;
    pagesize = 14;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    snd(26);
    buff = "";
    notesel(buff);
    chara_delete(56);
    cdata[56].piety_point = cdata[0].piety_point;
    cdata[56].god_id = cdata[0].god_id;
    for (int cnt = 0; cnt < 600; ++cnt)
    {
        sdata(cnt, rc) = 1;
    }
    apply_god_blessing(56);
    if (!cdata[0].god_id.empty())
    {
        buff += u8"<title1>◆ "s + i18n::_(u8"god", cdata[0].god_id, u8"name")
            + u8"による能力の恩恵<def>\n"s;
        for (int cnt = 0; cnt < 600; ++cnt)
        {
            p = sdata(cnt, rc) - 1;
            cnvbonus(cnt, p);
        }
    }
    refreshmode = 1;
    chara_refresh(0);
    refreshmode = 0;
    buff += u8"\n"s;
    buff += u8"<title1>◆ 特徴と特殊状態による能力の恩恵<def>\n"s;
    listmax = noteinfo();
label_1965_internal:
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
    s(0) = u8"自己の分析"s;
    s(1) = strhint2 + strhint3b;
    display_window((windoww - 400) / 2 + inf_screenx, winposy(448), 400, 448);
    s = u8"分析結果"s;
    display_topic(s, wx + 28, wy + 36);
    font(14 - en * 2);
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        noteget(s, p);
        pos(wx + 54, wy + 66 + cnt * 19 + 2);
        gmes(s, wx, wy + 66 + cnt * 19 + 2, 600, {30, 30, 30}, false);
    }
    redraw();
label_1966_internal:
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    p = -1;
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_1965_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1965_internal;
        }
    }
    if (key == key_cancel)
    {
        return;
    }
    goto label_1966_internal;
}

int change_npc_tone()
{
    gsel(4);
    for (int cnt = 0; cnt < 8; ++cnt)
    {
        pos(cnt % 4 * 180, cnt / 4 * 300);
        picload(
            filesystem::dir::graphic() / (u8"g"s + (cnt + 1) + u8".bmp"), 1);
    }
    gsel(0);
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    list(0, 0) = -999;
    listn(0, 0) = lang(u8"デフォルトの口調"s, u8"Default Tone"s);
    ++listmax;
    const auto base_dir = filesystem::dir::user() / u8"talk";
    for (const auto& entry :
         filesystem::dir_entries{base_dir,
                                 filesystem::dir_entries::type::file,
                                 std::regex{u8R"(.*\.txt)"}})
    {
        list(0, listmax) = listmax;
        listn(0, listmax) =
            filesystem::to_utf8_path(fs::relative(entry.path(), base_dir));
        ++listmax;
    }
    windowshadow = 1;
label_2015_internal:
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
label_2016_internal:
    s(0) = lang(u8"口調一覧"s, u8"Tone of Voice"s);
    s(1) = lang(u8"決定 [口調の変更]  "s, u8"Enter [Change Tone] "s) + strhint2
        + strhint3;
    display_window((windoww - 500) / 2 + inf_screenx, winposy(400), 500, 400);
    x = ww / 5 * 3;
    y = wh - 80;
    gmode(4, 180, 300, 50);
    pos(wx + ww / 3 * 2, wy + wh / 2);
    grotate(4, cmbg / 4 % 4 * 180, cmbg / 4 / 4 % 2 * 300, 0, x, y);
    gmode(2);
    display_topic(lang(u8"題名"s, u8"Title"s), wx + 28, wy + 36);
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
        if (cnt % 2 == 0)
        {
            pos(wx + 70, wy + 66 + cnt * 19);
            gfini(ww - 100, 18);
            gfdec2(12, 14, 16);
        }
        display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
    }
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
        s = ""s + listn(0, p);
        cs_list(cs == cnt, s, wx + 138, wy + 66 + cnt * 19 - 1);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, cs = i);
    if (p != -1)
    {
        snd(20);
        txt(lang(
            name(tc) + u8"の口調が変わった気がする。"s,
            name(tc) + u8" is somewhat different."s));
        if (p == -999)
        {
            cdata[tc].has_custom_talk() = false;
            cdatan(4, tc) = "";
            return 1;
        }
        cdata[tc].has_custom_talk() = true;
        cdatan(4, tc) = listn(0, p);
        return 1;
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2015_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2015_internal;
        }
    }
    if (key == key_cancel)
    {
        return -1;
    }
    goto label_2016_internal;
}



void label_2022()
{
    snd(59);
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"book.bmp", 1);
    gsel(0);
    notesel(buff);
    {
        buff(0).clear();
        std::ifstream in{(filesystem::dir::data() / u8"book.txt").native(),
                         std::ios::binary};
        std::string tmp;
        while (std::getline(in, tmp))
        {
            buff(0) += tmp + '\n';
        }
    }
    p = instr(buff, 0, ""s + inv[ci].param1 + u8","s + lang(u8"JP"s, u8"EN"s));
    if (p == -1)
    {
        return;
    }
    buff = strmid(buff, p, instr(buff, p, u8"%END"s));
    notedel(0);
    listmax = 0;
    page = 0;
    pagesize = 40;
    cs = 0;
    cc = 0;
    listmax = noteinfo();
    keyrange = 0;
    key_list(0) = key_enter;
label_2023_internal:
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
    wx = (windoww - 720) / 2 + inf_screenx;
    wy = winposy(468);
    pos(wx, wy);
    gcopy(4, 0, 0, 736, 448);
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        x = wx + 80 + cnt / 20 * 306;
        y = wy + 45 + cnt % 20 * 16;
        noteget(s, p);
        if (p == 0)
        {
            font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
        }
        if (p == 1)
        {
            font(10 + sizefix - en * 2);
        }
        if (p > 2)
        {
            font(12 + sizefix - en * 2);
        }
        pos(x, y);
        mes(s);
        if (p % 20 == 0)
        {
            font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
            pos(x + 90, y + 330);
            mes(u8"- "s + (p / 20 + 1) + u8" -"s);
        }
    }
label_2024_internal:
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(71);
            ++page;
            goto label_2023_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(71);
            --page;
            goto label_2023_internal;
        }
    }
    if (key == key_cancel || key == key_enter)
    {
        update_screen();
        return;
    }
    goto label_2024_internal;
}

}
