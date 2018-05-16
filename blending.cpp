#include "blending.hpp"
#include "ability.hpp"
#include "audio.hpp"
#include "character.hpp"
#include "config.hpp"
#include "draw.hpp"
#include "elona.hpp"
#include "enchantment.hpp"
#include "enums.hpp"
#include "i18n.hpp"
#include "input.hpp"
#include "item.hpp"
#include "itemgen.hpp"
#include "item_db.hpp"
#include "macro.hpp"
#include "map_cell.hpp"
#include "ui.hpp"
#include "variables.hpp"

namespace elona
{

elona_vector2<int> rpdata;
elona_vector2<std::string> rfnameorg;
elona_vector1<std::string> rpdatan;
elona_vector1<int> rpref;
int rpid = 0;
int rpmode = 0;
elona_vector1<int> rppage;
int rpresult = 0;
elona_vector1<int> inhlist_at_m184;

void initialize_recipememory()
{
    for (int cnt = 0; cnt < 8; ++cnt)
    {
        recipememory(200 + cnt) = 1;
    }
    return;
}

void initialize_recipe()
{
    DIM3(rpdata, 100, 1200);
    SDIM4(rfnameorg, 20, 2, 6);
    SDIM3(rpdatan, 40, 1200);
    DIM2(rpref, 100);
    rpsourcelist(0) = 0;
    rpsourcelist(1) = 898;
    rpsourcelist(2) = 1156;
    rpsourcelist(3) = 1187;
    rpsourcelist(4) = 209;
    rpsourcelist(5) = 210;
    rpid = 200;
    rpdatan(rpid) = lang(u8"媚薬混入食品"s, u8"love food"s);
    rpdata(0, rpid) = 10000;
    rpdata(1, rpid) = 10;
    rpdata(2, rpid) = 1;
    rpdata(10, rpid) = 184;
    rpdata(11, rpid) = 8;
    rpdata(12, rpid) = 178;
    rpdata(13, rpid) = 3;
    rpdata(20, rpid) = 57000;
    rpdata(21, rpid) = 620;
    rpid = 201;
    rpdatan(rpid) = lang(u8"染色"s, u8"dyeing"s);
    rpdata(0, rpid) = 10001;
    rpdata(1, rpid) = 4;
    rpdata(2, rpid) = 1;
    rpdata(10, rpid) = 12;
    rpdata(11, rpid) = 6;
    rpdata(20, rpid) = 9004;
    rpdata(21, rpid) = 519;
    rpid = 202;
    rpdatan(rpid) = lang(u8"特製毒入り食品"s, u8"poisoned food"s);
    rpdata(0, rpid) = 10002;
    rpdata(1, rpid) = 7;
    rpdata(2, rpid) = 1;
    rpdata(10, rpid) = 184;
    rpdata(11, rpid) = 3;
    rpdata(20, rpid) = 57000;
    rpdata(21, rpid) = 262;
    rpid = 203;
    rpdatan(rpid) = lang(u8"耐火コーティング"s, u8"fireproof coating"s);
    rpdata(0, rpid) = 10003;
    rpdata(1, rpid) = 15;
    rpdata(2, rpid) = 1;
    rpdata(10, rpid) = 12;
    rpdata(11, rpid) = 18;
    rpdata(20, rpid) = 9004;
    rpdata(21, rpid) = 736;
    rpid = 204;
    rpdatan(rpid) = lang(u8"耐酸コーティング"s, u8"acidproof coating"s);
    rpdata(0, rpid) = 10004;
    rpdata(1, rpid) = 15;
    rpdata(2, rpid) = 1;
    rpdata(10, rpid) = 12;
    rpdata(11, rpid) = 10;
    rpdata(20, rpid) = 9004;
    rpdata(21, rpid) = 566;
    rpid = 205;
    rpdatan(rpid) = lang(u8"釣り餌の装着"s, u8"bait attachment"s);
    rpdata(0, rpid) = 10005;
    rpdata(1, rpid) = 10;
    rpdata(2, rpid) = 1;
    rpdata(10, rpid) = 185;
    rpdata(11, rpid) = 2;
    rpdata(12, rpid) = 12;
    rpdata(13, rpid) = 10;
    rpdata(20, rpid) = 342;
    rpdata(21, rpid) = 617;
    rpid = 206;
    rpdatan(rpid) = lang(u8"アイテムの祝福"s, u8"blessed item"s);
    rpdata(0, rpid) = 10006;
    rpdata(1, rpid) = 5;
    rpdata(2, rpid) = 1;
    rpdata(10, rpid) = 12;
    rpdata(11, rpid) = 5;
    rpdata(20, rpid) = 9004;
    rpdata(21, rpid) = 516;
    rpid = 207;
    rpdatan(rpid) = lang(u8"井戸水の回復"s, u8"well refill"s);
    rpdata(0, rpid) = 10007;
    rpdata(1, rpid) = 3;
    rpdata(2, rpid) = 2;
    rpdata(10, rpid) = 12;
    rpdata(11, rpid) = 3;
    rpdata(20, rpid) = 60001;
    rpdata(21, rpid) = 52000;
    rpid = 208;
    rpdatan(rpid) = lang(u8"天然ポーション"s, u8"natural potion"s);
    rpdata(0, rpid) = 10008;
    rpdata(1, rpid) = 16;
    rpdata(2, rpid) = 2;
    rpdata(10, rpid) = 12;
    rpdata(11, rpid) = 24;
    rpdata(20, rpid) = 60001;
    rpdata(21, rpid) = 601;
    rpid = 209;
    rpdatan(rpid) = lang(u8"2種アーティファクト合成"s, u8"2 artifacts fusion"s);
    rpdata(0, rpid) = 10009;
    rpdata(1, rpid) = 16;
    rpdata(2, rpid) = 2;
    rpdata(10, rpid) = 178;
    rpdata(11, rpid) = 999;
    rpdata(20, rpid) = 9004;
    rpdata(21, rpid) = 9004;
    rpid = 210;
    rpdatan(rpid) = lang(u8"3種アーティファクト合成"s, u8"3 artifacts fusion"s);
    rpdata(0, rpid) = 10009;
    rpdata(1, rpid) = 16;
    rpdata(2, rpid) = 2;
    rpdata(10, rpid) = 178;
    rpdata(11, rpid) = 999;
    rpdata(20, rpid) = 9004;
    rpdata(21, rpid) = 9004;
    rpdata(22, rpid) = 9004;
    rpid = 898;
    rpdata(0, rpid) = 498;
    rpdata(1, rpid) = 150060;
    rpdata(10, rpid) = 175;
    rpdata(11, rpid) = 4;
    rpdata(12, rpid) = 457;
    rpdata(13, rpid) = 20;
    rpdata(14, rpid) = 12;
    rpdata(15, rpid) = 8;
    rpdata(30, rpid) = 0;
    rpdata(20, rpid) = 260;
    rpdata(21, rpid) = 9001;
    rpid = 1156;
    rpdata(0, rpid) = 756;
    rpdata(1, rpid) = 20020;
    rpdata(10, rpid) = 184;
    rpdata(11, rpid) = 4;
    rpdata(12, rpid) = 179;
    rpdata(13, rpid) = 20;
    rpdata(14, rpid) = 157;
    rpdata(15, rpid) = 8;
    rpdata(30, rpid) = 0;
    rpdata(20, rpid) = 260;
    ++rpid;
    rpid = 1187;
    rpdata(0, rpid) = 787;
    rpdata(1, rpid) = 30;
    rpdata(10, rpid) = 184;
    rpdata(11, rpid) = 10;
    rpdata(12, rpid) = 178;
    rpdata(13, rpid) = 5;
    rpdata(30, rpid) = 0;
    rpdata(20, rpid) = 9004;
    rpid = 1191;
    rpdata(0, rpid) = 791;
    rpdata(1, rpid) = 160045;
    rpdata(10, rpid) = 184;
    rpdata(11, rpid) = 10;
    rpdata(12, rpid) = 178;
    rpdata(13, rpid) = 65;
    rpdata(14, rpid) = 10;
    rpdata(15, rpid) = 105;
    rpdata(30, rpid) = 0;
    rpdata(20, rpid) = 10000;
    rpdata(21, rpid) = 748;
    rpdata(22, rpid) = 716;
    rpid = 1192;
    rpdata(0, rpid) = 792;
    rpdata(1, rpid) = 10024;
    rpdata(10, rpid) = 184;
    rpdata(11, rpid) = 20;
    rpdata(12, rpid) = 0;
    rpdata(13, rpid) = 10;
    rpdata(14, rpid) = 151;
    rpdata(15, rpid) = 5;
    rpdata(30, rpid) = 0;
    rpdata(20, rpid) = 204;
    rpdata(21, rpid) = 9005;
    rpdata(40, rpid) = 10003;
    rpdata(50, rpid) = 10017;
    rpdata(51, rpid) = 500;
    rpdata(52, rpid) = 60017;
    rpdata(53, rpid) = 500;
    rpdata(54, rpid) = 10010;
    rpdata(55, rpid) = 100;
    rpdata(56, rpid) = 60010;
    rpdata(57, rpid) = 100;
    rpdata(58, rpid) = 10016;
    rpdata(59, rpid) = 300;
    rpdata(60, rpid) = 60016;
    rpdata(61, rpid) = 300;
    rpdata(62, rpid) = 10011;
    rpdata(63, rpid) = 200;
    rpdata(64, rpid) = 60011;
    rpdata(65, rpid) = 200;
    rfnameorg(0, 1) = u8"flavor"s;
    rfnameorg(1, 1) = lang(u8"適当な調味料"s, u8"suitable flavoring"s);
    rfnameorg(0, 2) = u8"ore"s;
    rfnameorg(1, 2) = lang(u8"適当な鉱石"s, u8"any ore"s);
    rfnameorg(0, 3) = u8"wood"s;
    rfnameorg(1, 3) = lang(u8"木材を含む何か"s, u8"something made of wood"s);
    rfnameorg(0, 5) = u8"fish"s;
    rfnameorg(1, 5) = lang(u8"魚介類"s, u8"fish"s);
    rfnameorg(0, 4) = "";
    rfnameorg(1, 4) = lang(u8"何か物体"s, u8"any item"s);
    return;
}

void window_recipe2(int val0)
{
    int x_at_m183 = 0;
    int w_at_m183 = 0;
    int dx_at_m183 = 0;
    int dy_at_m183 = 0;
    std::string s_at_m183;
    int p_at_m183 = 0;
    x_at_m183 = wx + ww;
    w_at_m183 = 400;
    gmode(2);
    pos(x_at_m183 + w_at_m183 - 520, 0);
    gcopy(3, 960, 288, 480, 68);
    dx_at_m183 = x_at_m183 + w_at_m183 - 500;
    dy_at_m183 = 10;
    font(15 - en * 2, snail::font_t::style_t::bold);
    s_at_m183 = ""s + rpsuccessrate(rpdiff(rpid, step, -1));
    pos(dx_at_m183 + 140, dy_at_m183);
    color(30, 30, 30);
    bmes(lang(u8"成功率: "s, u8"Success Rate: "s) + s_at_m183, 235, 235, 235);
    color(0, 0, 0);
    p_at_m183 = rpdata(1, rpid);
    if (rpmode)
    {
        if (p_at_m183 < 10000)
        {
            p_at_m183 = p_at_m183 * val0;
        }
        else
        {
            p_at_m183 = p_at_m183 % 10000;
        }
        p_at_m183 += rpdata(1, rpid) / 10000 * val0 * 10000;
    }
    s_at_m183 = ""s + p_at_m183 % 10000 + lang(u8"ターン"s, u8" turns"s);
    if (p_at_m183 >= 10000)
    {
        s_at_m183 += lang(u8"と"s, u8" and "s) + p_at_m183 / 10000
            + lang(u8"時間"s, u8" hours"s);
    }
    pos(dx_at_m183 + 140, dy_at_m183 + 20);
    color(40, 40, 40);
    bmes(lang(u8"必要時間: "s, u8"Time: "s) + s_at_m183, 235, 235, 235);
    color(0, 0, 0);
    return;
}

void window_recipe_(
    int prm_1050,
    int prm_1051,
    int prm_1052,
    int prm_1053,
    int prm_1054)
{
    elona_vector1<std::string> s_at_m184;
    int xfix2_at_m184 = 0;
    int dx_at_m184 = 0;
    int dy_at_m184 = 0;
    int i_at_m184 = 0;
    int p_at_m184 = 0;
    int cnt2_at_m184 = 0;
    SDIM1(s_at_m184);
    if (windowshadow == 1)
    {
        window(
            prm_1051 + 4,
            prm_1052 + 4,
            prm_1053,
            prm_1054 - prm_1054 % 8,
            0,
            -1);
        pos(prm_1051 + prm_1053 - 522, 0);
        gfini(486, 69);
        gfdec(40, 40, 40);
        windowshadow = 0;
    }
    window(prm_1051, prm_1052, prm_1053, prm_1054 - prm_1054 % 8, 0, 0);
    window_recipe2();
    gmode(2);
    color(194, 170, 146);
    line(
        prm_1051 + 50 + 0,
        prm_1052 + prm_1054 - 48 - prm_1054 % 8,
        prm_1051 + prm_1053 - 40,
        prm_1052 + prm_1054 - 48 - prm_1054 % 8);
    color(0, 0, 0);
    color(234, 220, 188);
    line(
        prm_1051 + 50 + 0,
        prm_1052 + prm_1054 - 49 - prm_1054 % 8,
        prm_1051 + prm_1053 - 40,
        prm_1052 + prm_1054 - 49 - prm_1054 % 8);
    color(0, 0, 0);
    s_at_m184(0) = u8"Page."s + (rppage + 1) + u8"/"s + (rppage(1) + 1);
    s_at_m184(1) = ""s + key_prev + u8","s + key_next + ""s
        + lang(u8"[ページ切替]  "s, u8"[Page]  "s);
    if (step == -1)
    {
        s_at_m184(1) += strhint3;
    }
    else
    {
        s_at_m184(1) += strhint3b;
    }
    font(12 + sizefix - en * 2);
    pos(prm_1051 + 25 + 0, prm_1052 + prm_1054 - 43 - prm_1054 % 8);
    mes(s_at_m184(1));
    font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
    pos(prm_1051 + prm_1053 - strlen_u(s_at_m184) * 7 - 40 - xfix2_at_m184,
        prm_1052 + prm_1054 - 65 - prm_1054 % 8);
    mes(s_at_m184);
    dx_at_m184 = prm_1051 + 35;
    dy_at_m184 = y + 48;
    font(12 - en * 2, snail::font_t::style_t::bold);
    pos(dx_at_m184 - 10, dy_at_m184);
    mes(lang(u8"調合の手順"s, u8"Blending Procedure"s));
    dy_at_m184 = dy_at_m184 + 18;
    font(13 - en * 2);
    i_at_m184 = 1;
    pos(dx_at_m184 - 10, dy_at_m184 - 2);
    gfini(prm_1053 - 60, 17);
    if (step == i_at_m184 - 2)
    {
        gfdec2(10, 20, 30);
    }
    else if (step > i_at_m184 - 2)
    {
        gfdec2(25, 30, 35);
    }
    if (step == -1)
    {
        pos(dx_at_m184, dy_at_m184);
        mes(""s + i_at_m184 + u8"."s
            + lang(u8"レシピを選ぶ"s, u8"Choose a recipe"s));
    }
    else
    {
        pos(dx_at_m184, dy_at_m184);
        mes(""s + i_at_m184 + u8"."s
            + lang(
                  (rpname(rpid) + u8"のレシピを選んだ"s),
                  (u8"Chose the recipe of "s + rpname(rpid))));
    }
    dy_at_m184 += 17;
    ++i_at_m184;
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        if (rpdata(20 + cnt, rpid) == 0)
        {
            break;
        }
        pos(dx_at_m184 - 10, dy_at_m184 - 2);
        gfini(prm_1053 - 60, 17);
        if (step == i_at_m184 - 2)
        {
            gfdec2(10, 20, 30);
        }
        else if (step > i_at_m184 - 2)
        {
            gfdec2(25, 30, 35);
        }
        if (step <= cnt)
        {
            int stat = blendmatnum(rpdata(20 + cnt, rpid), cnt);
            p_at_m184 = stat;
            s_at_m184 = lang(
                            rpmatname(cnt) + u8"を加える"s,
                            u8"Add "s + rpmatname(cnt) + ""s)
                + u8"("s + lang(u8"所持:"s, u8"Stock:"s) + p_at_m184 + u8")"s;
        }
        else
        {
            s_at_m184 = lang(""s, u8"Selected "s)
                + itemname(rpref((10 + cnt * 2))) + lang(u8"を選んだ"s, ""s);
            s_at_m184 = strmid(s_at_m184, 0, 44);
        }
        pos(dx_at_m184, dy_at_m184);
        mes(""s + i_at_m184 + u8"."s + s_at_m184);
        dy_at_m184 += 17;
        ++i_at_m184;
    }
    pos(wx + ww + 243, wy - 4);
    gcopy(3, 1040, 96, 160, 70);
    pos(dx_at_m184 - 10, dy_at_m184 - 2);
    gfini(prm_1053 - 60, 17);
    if (step == i_at_m184 - 2)
    {
        gfdec2(10, 20, 30);
    }
    else if (step > i_at_m184 - 2)
    {
        gfdec2(25, 30, 35);
    }
    pos(dx_at_m184, dy_at_m184);
    mes(""s + i_at_m184 + u8"."s
        + lang(u8"調合を始める！"s, u8"Start blending!"s));
    dy_at_m184 += 30;
    if (rppage == 0)
    {
        font(12 - en * 2, snail::font_t::style_t::bold);
        pos(dx_at_m184 - 10, dy_at_m184);
        mes(lang(
            rpname(rpid) + u8"のレシピ"s, u8"The recipe of "s + rpname(rpid)));
        dy_at_m184 += 20;
        pos(dx_at_m184 - 10, dy_at_m184);
        mes(lang(u8"必要なスキル:"s, u8"Required Skills:"s));
        dy_at_m184 = dy_at_m184 + 18;
        font(13 - en * 2);
        for (int cnt = 0; cnt < 5; ++cnt)
        {
            if (rpdata(10 + cnt * 2, rpid) == 0)
            {
                break;
            }
            if (rpdata(11 + cnt * 2, rpid)
                > sdata(rpdata(10 + cnt * 2, rpid), 0))
            {
                color(150, 0, 0);
            }
            else
            {
                color(0, 120, 0);
            }
            pos(dx_at_m184 + cnt % 2 * 140, dy_at_m184 + cnt / 2 * 17);
            mes(i18n::_(
                    u8"ability",
                    std::to_string(rpdata(10 + cnt * 2, rpid)),
                    u8"name")
                + u8"  "s + rpdata((11 + cnt * 2), rpid) + u8"("s
                + sdata(rpdata((10 + cnt * 2), rpid), 0) + u8")"s);
            color(0, 0, 0);
        }
        dy_at_m184 += 50;
        font(12 - en * 2, snail::font_t::style_t::bold);
        pos(dx_at_m184 - 10, dy_at_m184);
        mes(lang(u8"必要な機材:"s, u8"Required equipment:"s));
        return;
    }
    if (prm_1050 == -1)
    {
        return;
    }
    font(12 - en * 2, snail::font_t::style_t::bold);
    pos(dx_at_m184 - 10, dy_at_m184);
    mes(itemname(prm_1050));
    dy_at_m184 += 20;
    font(13 - en * 2);
    if (inv[prm_1050].identification_state
        <= identification_state_t::partly_identified)
    {
        pos(dx_at_m184, dy_at_m184);
        mes(lang(
            u8"このアイテムは鑑定されていない。"s,
            u8"You haven't identified it yet."s));
        dy_at_m184 += 16;
        return;
    }
    getinheritance(prm_1050, inhlist_at_m184, p_at_m184);
    if (p_at_m184 > 0)
    {
        for (int cnt = 0, cnt_end = (p_at_m184); cnt < cnt_end; ++cnt)
        {
            cnt2_at_m184 = inhlist_at_m184(cnt);
            if (inv[prm_1050].enchantments[cnt2_at_m184].id == 0)
            {
                break;
            }
            get_enchantment_description(
                inv[prm_1050].enchantments[cnt2_at_m184].id,
                inv[prm_1050].enchantments[cnt2_at_m184].power,
                the_item_db[inv[prm_1050].id]->category);
            color(0, 0, 100);
            if (inv[prm_1050].enchantments[cnt2_at_m184].power < 0)
            {
                color(180, 0, 0);
            }
            pos(dx_at_m184, dy_at_m184);
            mes(cnven(s));
            color(0, 0, 0);
            dy_at_m184 += 16;
        }
    }
    else
    {
        pos(dx_at_m184, dy_at_m184);
        mes(lang(u8"継承効果なし"s, u8"No inheritance effects"s));
        dy_at_m184 += 16;
        ++p_at_m184;
    }
    return;
}

turn_result_t blending_menu()
{
    elona_vector1<int> blendchecklist;
    step = -1;
    rpid = 0;
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_blend.bmp", 1);
    gsel(0);
    clear_rprefmat();
label_1923:
    if (step != -1)
    {
        if (rpdata(20 + step, rpid) == 0)
        {
            rppage = 0;
            window_recipe(list2, -1, wx + ww, wy, 400, wh);
            txtnew();
            txt(lang(
                u8"幾つ作る？"s, u8"How many items do you want to create?"s));
            ELONA_APPEND_PROMPT(
                lang(u8"調合を始める"s, u8"Start blending"s),
                u8"a"s,
                ""s + promptmax);
            ELONA_APPEND_PROMPT(
                lang(u8"前に戻る"s, u8"Go back"s), u8"b"s, ""s + promptmax);
            ELONA_APPEND_PROMPT(
                lang(u8"最初から"s, u8"From the start"s),
                u8"c"s,
                ""s + promptmax);
            p = 10;
            for (int cnt = 0; cnt < 10; ++cnt)
            {
                if (rpref(10 + cnt * 2) == -1)
                {
                    break;
                }
                if (rpdata(2, rpid) == 2 && cnt == 0)
                {
                    continue;
                }
                if (inv[rpref(10 + cnt * 2)].number < p)
                {
                    p = inv[rpref(10 + cnt * 2)].number;
                }
            }
            rpmode = 1;
            rtval = show_prompt(
                promptx, prompty, 220, show_prompt_type::with_number, p);
            rpmode = 0;
            if (rtval == 0)
            {
                rpref(1) = TODO_show_prompt_val;
                rpref(2) = rpdata(1, rpid);
                rpref(3) = rpdiff(rpid, step, -1);
                label_19342();
                return turn_result_t::turn_end;
            }
            if (rtval == 2)
            {
                step = -1;
                goto label_1923;
            }
            --step;
            goto label_1923;
        }
    }
    page = 0;
    pagesize = 16;
    listmax = 0;
    cs = 0;
    cs_bk = -1;
    cc = 0;
    screenupdate = -1;
    update_screen();
    if (step == -1)
    {
        rppage(0) = 0;
        rppage(1) = 0;
        listmax = 0;
        for (int cnt = 0; cnt < 1200; ++cnt)
        {
            if (rpdata(0, cnt) == 0)
            {
                continue;
            }
            if (recipememory(cnt) > 0)
            {
                list(0, listmax) = cnt;
                list(1, listmax) = cnt;
                ++listmax;
            }
        }
        sort_list_by_column1();
        windowshadow(1) = 1;
        txtnew();
        txt(lang(
            u8"(製作中)だめまだ"s,
            u8"(*) The feature is not implemented yet."s));
        txtnew();
        txt(lang(
            u8"どのレシピを使う？"s, u8"Which recipe do you want to use?"s));
        goto label_1924_internal;
    }
    rppage = 1;
    {
        int stat = blendlist(list, step);
        listmax = stat;
    }
    sort_list_by_column1();
    goto label_1927_internal;
label_1924_internal:
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
    DIM2(blendchecklist, pagesize);
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        blendchecklist(cnt) = blendcheckmat(list(0, p));
    }
label_1925_internal:
    s(0) = lang(u8"レシピの選択"s, u8"Choose a recipe"s);
    s(1) = strhint2;
    windowshadow = windowshadow(1);
    display_window(
        (windoww - 780) / 2 + inf_screenx, winposy(445), 380, 432, 74);
    display_topic(lang(u8"レシピの名称"s, u8"Name"s), wx + 28, wy + 30);
    s = ""s + listmax + u8" recipes"s;
    font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
    pos(wx + 130, wy + wh - 65 - wh % 8);
    mes(s);
    keyrange = 0;
    gmode(2);
    prepare_item_image(550, 0);
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
            pos(wx + 70, wy + 60 + cnt * 19);
            gfini(ww - 100, 18);
            gfdec2(12, 14, 16);
        }
        pos(wx + 37, wy + 70 + cnt * 19);
        gmode(2, chipi(2, 550), chipi(3, 550));
        grotate(1, 0, 960, 0, inf_tiles, inf_tiles);
        pos(wx + 330, wy + 53 + cnt * 19);
        if (blendchecklist(cnt) == 1)
        {
            gcopy(3, 336, 360, 24, 24);
        }
        rpid = list(0, p);
        pos(wx + 317, wy + 60 + cnt * 19);
        gcopy(3, 64 + (4 - rpdiff(rpid, -1, -1) / 25) * 16, 624, 16, 16);
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
        p = list(0, p);
        rpid = p;
        s = lang(""s, u8"Recipe of "s) + cnven(rpname(rpid))
            + lang(u8"のレシピ"s, ""s);
        display_key(wx + 58, wy + 60 + cnt * 19 - 2, cnt);
        cs_list(cs == cnt, s, wx + 84, wy + 60 + cnt * 19 - 1, 0, 0, p);
    }
    if (cs_bk != cs)
    {
        rpid = list(0, pagesize * page + cs);
        windowshadow = windowshadow(1);
        window_recipe(list2, -1, wx + ww, wy, 400, wh);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    windowshadow(1) = 0;
    pos(wx + 10, wy + wh - 100);
    gcopy(3, 960, 96, 80, 90);
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        rpid = p;
        step = 0;
        rppage(0) = 1;
        rppage(1) = 1;
        rpref(0) = rpid;
        goto label_1923;
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_1924_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1924_internal;
        }
    }
    if (key == key_cancel)
    {
        screenupdate = 0;
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    goto label_1925_internal;
label_1927_internal:
    windowshadow(1) = 1;
    txtnew();
    txt(lang(
        rpmatname(step) + u8"を追加しよう。"s,
        u8"Add \""s + rpmatname(step) + u8"\"."s));
label_1928_internal:
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
    if (jp)
    {
        s = rpmatname(step) + u8"を追加"s;
    }
    else
    {
        s = u8"Add "s + rpmatname(step);
    }
    s(1) = strhint2;
    windowshadow = windowshadow(1);
    display_window(
        (windoww - 780) / 2 + inf_screenx, winposy(445), 380, 432, 74);
    display_topic(lang(u8"アイテムの名称"s, u8"Name"s), wx + 28, wy + 30);
    s = ""s + listmax + u8" items"s;
    font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
    pos(wx + 130, wy + wh - 65 - wh % 8);
    mes(s);
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
            pos(wx + 70, wy + 60 + cnt * 19);
            gfini(ww - 100, 18);
            gfdec2(12, 14, 16);
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
        p = list(0, p);
        s = itemname(p, inv[p].number);
        s = strmid(s, 0, 28);
        if (p >= 5080)
        {
            s += lang(u8"(地面)"s, u8" (Ground)"s);
        }
        display_key(wx + 58, wy + 60 + cnt * 19 - 2, cnt);
        p(1) = inv[p].image % 1000;
        prepare_item_image(p(1), inv[p].color, inv[p].param1);
        pos(wx + 37, wy + 69 + cnt * 19);
        gmode(2, chipi(2, p(1)), chipi(3, p(1)));
        grotate(
            1,
            0,
            960,
            0,
            chipi(2, p(1)) * inf_tiles / chipi(3, p(1)),
            inf_tiles);
        if (inv[p].body_part != 0)
        {
            pos(wx + 46, wy + 72 + cnt * 18 - 3);
            gcopy(3, 12, 348, 12, 12);
        }
        cs_list(cs == cnt, s, wx + 84, wy + 60 + cnt * 19 - 1, 0, 1, p);
    }
    p = list(0, pagesize * page + cs);
    if (listmax == 0)
    {
        p = -1;
    }
    if (cs_bk != cs)
    {
        windowshadow = windowshadow(1);
        window_recipe(list2, p, wx + ww, wy, 400, wh);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    windowshadow(1) = 0;
    pos(wx + 10, wy + wh - 100);
    gcopy(3, 960, 96, 80, 90);
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
            goto label_1928_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1928_internal;
        }
    }
    if (p != -1)
    {
        snd(17);
        ci = p;
        rpref(10 + step * 2 + 0) = ci;
        rpref(10 + step * 2 + 1) = inv[ci].id;
        txt(lang(
            itemname(ci) + u8"を選んだ。"s,
            u8"You add "s + itemname(ci) + u8"."s));
        ++step;
        p = rpdiff(rpid, step, step - 1);
        goto label_1923;
    }
    if (key == key_prev)
    {
        snd(1);
        --rppage;
        cs_bk = -1;
        if (rppage < 0)
        {
            rppage = rppage(1);
        }
    }
    if (key == key_next)
    {
        snd(1);
        ++rppage;
        cs_bk = -1;
        if (rppage > rppage(1))
        {
            rppage = 0;
        }
    }
    if (key == key_cancel)
    {
        --step;
        goto label_1923;
    }
    goto label_1928_internal;
}

std::string rpmatname(int prm_1037)
{
    std::string s_at_m177;
    int p_at_m177 = 0;
    s_at_m177 = u8"?????"s;
    if (rpdata(20 + prm_1037, rpid) < 9000)
    {
        s_at_m177 = ioriginalnameref(rpdata(20 + prm_1037, rpid));
    }
    else if (rpdata(20 + prm_1037, rpid) < 10000)
    {
        s_at_m177 = rfnameorg(1, rpdata(20 + prm_1037, rpid) - 9000);
    }
    else
    {
        s_at_m177 = fltname(rpdata(20 + prm_1037, rpid));
    }
    if (rpdata(40 + prm_1037, rpid) == 0)
    {
        return s_at_m177;
    }
    if (rpdata(40 + prm_1037, rpid) >= 10000)
    {
        p_at_m177 = rpdata((40 + prm_1037), rpid) % 10000;
        if (p_at_m177 < 0 || p_at_m177 >= 800)
        {
            s_at_m177 += u8"/bugged/"s;
            return s_at_m177;
        }
        s_at_m177 =
            chara_refstr(p_at_m177, 2) + lang(u8"の死体"s, u8" corpse"s);
        return s_at_m177;
    }
    return s_at_m177;
}


std::string rpsuccessrate(int prm_1040)
{
    if (prm_1040 == 100)
    {
        return lang(u8"もう完璧！"s, u8"Perfect!"s);
    }
    if (prm_1040 >= 90)
    {
        return lang(u8"朝飯前！"s, u8"A piece of cake!"s);
    }
    if (prm_1040 >= 80)
    {
        return lang(u8"かんたんね"s, u8"Very likely"s);
    }
    if (prm_1040 >= 70)
    {
        return lang(u8"まず大丈夫"s, u8"No problem"s);
    }
    if (prm_1040 >= 60)
    {
        return lang(u8"たぶんいける"s, u8"Probably OK"s);
    }
    if (prm_1040 >= 50)
    {
        return lang(u8"ちょっと不安"s, u8"Maybe"s);
    }
    if (prm_1040 >= 40)
    {
        return lang(u8"だめかも"s, u8"Bad"s);
    }
    if (prm_1040 >= 30)
    {
        return lang(u8"やばい"s, u8"Very bad"s);
    }
    if (prm_1040 >= 20)
    {
        return lang(u8"まず無理"s, u8"Almost impossible"s);
    }
    return lang(u8"絶対ムリ！"s, u8"Impossible!"s);
}

std::string rpname(int prm_516)
{
    std::string s_at_m62;
    s_at_m62 = u8"?????"s;
    if (rpdatan(prm_516) != ""s)
    {
        return rpdatan(prm_516);
    }
    if (prm_516 >= 400)
    {
        return ioriginalnameref(prm_516 - 400);
    }
    return s_at_m62;
}

int rpdiff(int, int prm_1042, int prm_1043)
{
    int p1_at_m180 = 0;
    int f_at_m180 = 0;
    int f2_at_m180 = 0;
    int i_at_m180 = 0;
    int d_at_m180 = 0;
    int p_at_m180 = 0;
    p1_at_m180 = 80;
    f_at_m180 = 100;
    if (prm_1042 > 0)
    {
        for (int cnt = 0, cnt_end = (prm_1042); cnt < cnt_end; ++cnt)
        {
            f2_at_m180 = 0;
            if (rpref(10 + cnt * 2) == -1)
            {
                break;
            }
            i_at_m180 = rpref(10 + cnt * 2);
            if (inv[i_at_m180].curse_state == curse_state_t::blessed)
            {
                f2_at_m180 -= 10;
            }
            if (is_cursed(inv[i_at_m180].curse_state))
            {
                f2_at_m180 += 20;
            }
            f_at_m180 += f2_at_m180;
            if (prm_1043 == cnt)
            {
                while (1)
                {
                    if (f2_at_m180 < 0)
                    {
                        txtef(2);
                        txt(lang(
                            u8"調合の成功率が上がった。"s,
                            u8"The success rate goes up."s));
                        break;
                    }
                    if (f2_at_m180 > 0)
                    {
                        txtef(3);
                        txt(lang(
                            u8"調合の成功率が下がった。"s,
                            u8"The success rate goes down."s));
                        break;
                    }
                    break;
                }
            }
        }
    }
    for (int cnt = 0; cnt < 5; ++cnt)
    {
        if (rpdata(10 + cnt * 2, rpid) == 0)
        {
            break;
        }
        if (sdata(rpdata(10 + cnt * 2, rpid), 0) <= 0)
        {
            p1_at_m180 -= 125;
            continue;
        }
        d_at_m180 = rpdata(11 + cnt * 2, rpid);
        if (prm_1042 > 0)
        {
            d_at_m180 = d_at_m180 * f_at_m180 / 100;
            if (d_at_m180 < 1)
            {
                d_at_m180 = 1;
            }
        }
        p_at_m180 =
            (d_at_m180 * 200 / sdata(rpdata((10 + cnt * 2), rpid), 0) - 200)
            * -1;
        if (p_at_m180 > 0)
        {
            p_at_m180 /= 5;
        }
        if (p_at_m180 < -125)
        {
            p_at_m180 = -125;
        }
        p1_at_m180 += p_at_m180;
    }
    if (p1_at_m180 < 25)
    {
        p1_at_m180 = 0;
    }
    if (p1_at_m180 > 100)
    {
        p1_at_m180 = 100;
    }
    return p1_at_m180;
}

int blendcheckext(int prm_1038, int prm_1039)
{
    int p_at_m178 = 0;
    if (rpdata(40 + prm_1039, rpid) >= 10000)
    {
        p_at_m178 = rpdata((40 + prm_1039), rpid) % 10000;
        if (p_at_m178 < 0 || p_at_m178 >= 800)
        {
            return 0;
        }
        if (inv[prm_1038].subname == p_at_m178)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

int blendcheckmat(int prm_1044)
{
    int f_at_m181 = 0;
    int step_at_m181 = 0;
    int id_at_m181 = 0;
    int rp_at_m181 = 0;
    int o_at_m181 = 0;
    rpid = prm_1044;
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        if (rpdata(20 + cnt, rpid) == 0)
        {
            break;
        }
        f_at_m181 = 0;
        step_at_m181 = cnt;
        id_at_m181 = rpdata(20 + cnt, rpid);
        rp_at_m181 = cnt;
        for (int cnt = 0; cnt < 2; ++cnt)
        {
            if (cnt == 0)
            {
                o_at_m181 = -1;
            }
            if (cnt == 1)
            {
                o_at_m181 = 0;
            }
            for (const auto& cnt : items(o_at_m181))
            {
                if (inv[cnt].number <= 0)
                {
                    continue;
                }
                if ((rpdata(2, rpid) <= 0 || step_at_m181 != 0)
                    && inv[cnt].own_state > 0)
                {
                    continue;
                }
                if (o_at_m181 == -1)
                {
                    if (dist(
                            inv[cnt].position.x,
                            inv[cnt].position.y,
                            cdata[0].position.x,
                            cdata[0].position.y)
                        > 4)
                    {
                        continue;
                    }
                }
                if (rpdata(40 + rp_at_m181, rpid))
                {
                    int stat = blendcheckext(cnt, rp_at_m181);
                    if (stat == 0)
                    {
                        continue;
                    }
                }
                if (id_at_m181 < 9000)
                {
                    if (inv[cnt].id == id_at_m181)
                    {
                        f_at_m181 = 1;
                        break;
                    }
                    continue;
                }
                if (id_at_m181 < 10000)
                {
                    if (instr(
                            the_item_db[inv[cnt].id]->rffilter,
                            0,
                            u8"/"s + rfnameorg(0, (id_at_m181 - 9000)) + u8"/"s)
                            != -1
                        || id_at_m181 == 9004)
                    {
                        f_at_m181 = 1;
                        break;
                    }
                    continue;
                }
                if (the_item_db[inv[cnt].id]->category == id_at_m181)
                {
                    f_at_m181 = 1;
                    break;
                }
            }
            if (f_at_m181 == 1)
            {
                break;
            }
        }
        if (f_at_m181 == 0)
        {
            break;
        }
    }
    return f_at_m181;
}

int blendmatnum(int prm_1045, int prm_1046)
{
    int m_at_m182 = 0;
    int o_at_m182 = 0;
    m_at_m182 = 0;
    for (int cnt = 0; cnt < 2; ++cnt)
    {
        if (cnt == 0)
        {
            o_at_m182 = -1;
        }
        if (cnt == 1)
        {
            o_at_m182 = 0;
        }
        for (const auto& cnt : items(o_at_m182))
        {
            if (inv[cnt].number <= 0)
            {
                continue;
            }
            if ((rpdata(2, rpid) <= 0 || prm_1046 != 0)
                && inv[cnt].own_state > 0)
            {
                continue;
            }
            if (o_at_m182 == -1)
            {
                if (dist(
                        inv[cnt].position.x,
                        inv[cnt].position.y,
                        cdata[0].position.x,
                        cdata[0].position.y)
                    > 4)
                {
                    continue;
                }
            }
            if (rpdata(40 + prm_1046, rpid))
            {
                int stat = blendcheckext(cnt, prm_1046);
                if (stat == 0)
                {
                    continue;
                }
            }
            if (prm_1045 < 9000)
            {
                if (inv[cnt].id == prm_1045)
                {
                    m_at_m182 += inv[cnt].number;
                }
                continue;
            }
            if (prm_1045 < 10000)
            {
                if (instr(
                        the_item_db[inv[cnt].id]->rffilter,
                        0,
                        u8"/"s + rfnameorg(0, (prm_1045 - 9000)) + u8"/"s)
                        != -1
                    || prm_1045 == 9004)
                {
                    m_at_m182 += inv[cnt].number;
                }
                continue;
            }
            if (the_item_db[inv[cnt].id]->category == prm_1045)
            {
                m_at_m182 += inv[cnt].number;
                continue;
            }
        }
    }
    return m_at_m182;
}

int blendlist(elona_vector2<int>& prm_1047, int prm_1048)
{
    int id_at_m183 = 0;
    int m_at_m183 = 0;
    int o_at_m183 = 0;
    int reftype_at_m183 = 0;
    int f_at_m183 = 0;
    id_at_m183 = rpdata(20 + prm_1048, rpid);
    m_at_m183 = 0;
    for (int cnt = 0; cnt < 2; ++cnt)
    {
        if (cnt == 0)
        {
            o_at_m183 = -1;
        }
        if (cnt == 1)
        {
            o_at_m183 = 0;
        }
        for (const auto& cnt : items(o_at_m183))
        {
            if (m_at_m183 >= 500)
            {
                break;
            }
            if (inv[cnt].number <= 0)
            {
                continue;
            }
            if ((rpdata(2, rpid) <= 0 || step != 0) && inv[cnt].own_state > 0)
            {
                continue;
            }
            if (o_at_m183 == -1)
            {
                if (dist(
                        inv[cnt].position.x,
                        inv[cnt].position.y,
                        cdata[0].position.x,
                        cdata[0].position.y)
                    > 4)
                {
                    continue;
                }
            }
            reftype_at_m183 = the_item_db[inv[cnt].id]->category;
            if (rpdata(40 + prm_1048, rpid))
            {
                int stat = blendcheckext(cnt, prm_1048);
                if (stat == 0)
                {
                    continue;
                }
            }
            if (id_at_m183 < 9000)
            {
                if (inv[cnt].id != id_at_m183)
                {
                    continue;
                }
            }
            else if (id_at_m183 < 10000)
            {
                if (instr(
                        the_item_db[inv[cnt].id]->rffilter,
                        0,
                        u8"/"s + rfnameorg(0, (id_at_m183 - 9000)) + u8"/"s)
                        == -1
                    && id_at_m183 != 9004)
                {
                    continue;
                }
            }
            else if (reftype_at_m183 != id_at_m183)
            {
                continue;
            }
            if (step > 0)
            {
                f_at_m183 = cnt;
                for (int cnt = 0, cnt_end = (step); cnt < cnt_end; ++cnt)
                {
                    if (rpref(10 + cnt * 2) == f_at_m183)
                    {
                        f_at_m183 = -999;
                        break;
                    }
                }
                if (f_at_m183 == -999)
                {
                    continue;
                }
            }
            prm_1047(0, m_at_m183) = cnt;
            prm_1047(1, m_at_m183) = reftype_at_m183 * 1000 + inv[cnt].id;
            ++m_at_m183;
        }
    }
    return m_at_m183;
}

void clear_rprefmat()
{
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        rpref(10 + cnt * 2) = -1;
    }
    return;
}


int label_1931()
{
    f = 1;
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        if (rpref(10 + cnt * 2) == -1)
        {
            break;
        }
        if (rpref(10 + cnt * 2) == -2)
        {
            f = 0;
            break;
        }
        if (inv[rpref(10 + cnt * 2)].number <= 0
            || inv[rpref(10 + cnt * 2)].id != rpref(11 + cnt * 2))
        {
            f = 0;
            break;
        }
    }
    return f;
}

int label_1932()
{
    p = 0;
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        if (rpref(10 + cnt * 2) == -1)
        {
            break;
        }
        if (rpref(10 + cnt * 2) == -2)
        {
            continue;
        }
        if ((rpdata(2, rpid) <= 0 || cnt != 0) && rpresult)
        {
            --inv[rpref(10 + cnt * 2)].number;
        }
        else if (rnd(3) == 0)
        {
            txt(lang(
                itemname(rpref(10 + cnt * 2), 1) + u8"を失った。"s,
                u8"You lose "s + itemname(rpref((10 + cnt * 2)), 1) + u8"."s));
            --inv[rpref(10 + cnt * 2)].number;
        }
        if (chara_unequip(rpref(10 + cnt * 2)))
        {
            chara_refresh(0);
        }
        cell_refresh(
            inv[rpref(10 + cnt * 2)].position.x,
            inv[rpref(10 + cnt * 2)].position.y);
    }
    refresh_burden_state();
    return 1;
}

void label_1933()
{
    rpresult = 1;
    if (rpdiff(rpid, -1, -1) < rnd(100))
    {
        rpresult = 0;
        txtef(3);
        txt(lang(u8"調合失敗！"s, u8"The blending attempt failed!"s));
    }
    else
    {
        if (rpdata(0, rpid) >= 10000)
        {
            label_1935();
        }
        else
        {
            flt();
            nostack = 1;
            int stat = itemcreate(
                -1,
                rpdata(0, rpid),
                cdata[0].position.x,
                cdata[0].position.y,
                0);
            if (stat != 0)
            {
                for (int cnt = 0;; ++cnt)
                {
                    if (rpdata(50 + cnt * 2, rpid) == 0)
                    {
                        break;
                    }
                    enchantment_add(
                        ci,
                        rpdata(50 + cnt * 2, rpid),
                        rpdata(51 + cnt * 2, rpid),
                        0,
                        1);
                }
            }
            txtef(2);
            txt(lang(
                itemname(ci, 1) + u8"の作成に成功した！"s,
                u8"You successfully create "s + itemname(ci, 1) + u8"!"s));
            snd(17);
        }
        for (int cnt = 0; cnt < 5; ++cnt)
        {
            if (rpdata(10 + cnt * 2, rpid) == 0)
            {
                break;
            }
            skillexp(
                rpdata(10 + cnt * 2, rpid),
                0,
                50 + rpdata((11 + cnt * 2), rpid) + rpref(2) / 10000 * 25,
                2,
                50);
        }
    }
    --rpref(1);
    label_1932();
    return;
}

void label_19342()
{
label_19341_internal:
    rpid = rpref(0);
    if (rpid == 0)
    {
        rowactend(cc);
        return;
    }
    if (cdata[cc].continuous_action_id == 0)
    {
        txtnew();
        txt(lang(
            name(cc) + u8"は"s + rpname(rpid) + u8"の調合をはじめた。"s,
            name(cc) + u8" start"s + _s(cc) + u8" blending of "s + rpname(rpid)
                + u8"."s));
        cdata[cc].continuous_action_id = 12;
        cdata[cc].continuous_action_turn = rpref(2) % 10000;
        return;
    }
    if (cdata[cc].continuous_action_turn > 0)
    {
        if (rnd(30) == 0)
        {
            txtef(4);
            txt(lang(u8" *こねこね* "s, u8"*pug*"s),
                lang(u8" *トントン* "s, u8"*clank*"s));
        }
        return;
    }
    if (rpref(2) >= 10000)
    {
        cdata[cc].continuous_action_turn = rpref(2) / 10000;
        for (int cnt = 0;; ++cnt)
        {
            mode = 12;
            ++gdata_hour;
            weather_changes();
            render_hud();
            if (cnt % 5 == 0)
            {
                txtef(4);
                txt(lang(u8" *こねこね* "s, u8"*pug*"s),
                    lang(u8" *トントン* "s, u8"*clank*"s));
            }
            for (int cnt = 0; cnt < 20; ++cnt)
            {
                redraw();
                await(30);
            }
            gdata_minute = 0;
            cc = 0;
            --cdata[cc].continuous_action_turn;
            if (cdata[cc].continuous_action_turn <= 0)
            {
                int stat = label_1931();
                if (stat == 0)
                {
                    txt(lang(
                        u8"調合に必要な材料が見つからない。"s,
                        u8"A required material cannot be found."s));
                    break;
                }
                label_1933();
                if (rpref(1) > 0)
                {
                    cdata[cc].continuous_action_turn = rpref(2) / 10000;
                    cnt = 0 - 1;
                    continue;
                }
                else
                {
                    break;
                }
            }
        }
        rowactend(cc);
        mode = 0;
        return;
    }
    int stat = label_1931();
    if (stat == 0)
    {
        txt(lang(
            u8"調合に必要な材料が見つからない。"s,
            u8"A required material cannot be found."s));
        rowactend(cc);
        return;
    }
    label_1933();
    if (rpref(1) > 0)
    {
        cdata[cc].continuous_action_id = 0;
        goto label_19341_internal;
    }
    rowactend(cc);
    return;
}

void label_1935()
{
    int cibk = ci;
    ci = rpref(10);
    ti = rpref(12);
    if (rpdata(2, rpid) == 2)
    {
        item_separate(ci);
    }
    else if (inv[ci].number <= 1)
    {
        rpref(10) = -2;
    }
    else
    {
        int stat = item_separate(ci);
        if (rpref(10) == stat)
        {
            rpref(10) = -2;
        }
        else
        {
            rpref(10) = stat;
        }
    }
    switch (rpdata(0, rpid))
    {
    case 10000:
        ibitmod(6, ci, 1);
        txtef(2);
        txt(lang(
            itemname(ci, 1) + u8"の作成に成功した！"s,
            u8"You successfully create "s + itemname(ci, 1) + u8"!"s));
        txt(lang(
            u8"あなたはうしろめたさを感じた…"s,
            u8"You kind of feel guilty..."s));
        snd(65);
        break;
    case 10001:
        inv[ci].color = inv[ti].color;
        txtef(2);
        txt(lang(
            u8"あなたは"s + itemname(ci) + u8"を染めた。"s,
            u8"You dye "s + itemname(ci) + u8"."s));
        snd(17);
        break;
    case 10002:
        ibitmod(14, ci, 1);
        txtef(2);
        txt(lang(
            itemname(ci, 1) + u8"の作成に成功した！"s,
            u8"You successfully create "s + itemname(ci, 1) + u8"!"s));
        txt(lang(u8"あなたはにやりと口元を歪めた。"s, u8"You grin."s));
        snd(65);
        break;
    case 10003:
        txtef(2);
        txt(lang(
            itemname(ci) + u8"に"s + itemname(ti, 1) + u8"を塗りたくった。"s,
            u8"You put "s + itemname(ti, 1) + u8" on "s + itemname(ci)
                + u8"."s));
        if (inv[ci].id == 567)
        {
            txt(lang(u8"いいアイデアだ！しかし…"s, u8"A good idea! But..."s));
        }
        else
        {
            ibitmod(2, ci, 1);
            txt(lang(
                itemname(ci) + u8"は熱から守られた。"s,
                itemname(ci) + u8" gain"s + _s2(in) + u8" fireproof."s));
        }
        snd(17);
        break;
    case 10004:
        txtef(2);
        txt(lang(
            itemname(ci) + u8"に"s + itemname(ti, 1) + u8"を塗りたくった。"s,
            u8"You put "s + itemname(ti, 1) + u8" on "s + itemname(ci)
                + u8"."s));
        ibitmod(1, ci, 1);
        txt(lang(
            itemname(ci) + u8"は酸から守られた。"s,
            itemname(ci) + u8" gain"s + _s2(in) + u8" acidproof."s));
        snd(17);
        break;
    case 10005:
        txtef(2);
        txt(lang(
            itemname(ti, 1) + u8"を"s + itemname(ci) + u8"に装着した。"s,
            u8"You bait "s + itemname(ci) + u8" with "s + itemname(ti, 1)
                + u8"."s));
        if (inv[ci].param4 == inv[ti].param1)
        {
            inv[ci].count += rnd(10) + 15;
        }
        else
        {
            inv[ci].count = rnd(10) + 15;
            inv[ci].param4 = inv[ti].param1;
        }
        snd(13);
        break;
    case 10006:
        txtef(2);
        txt(lang(
            itemname(ti, 1) + u8"を"s + itemname(ci) + u8"に降りかけた。"s,
            u8"You shower "s + itemname(ci) + u8" on "s + itemname(ti, 1)
                + u8"."s));
        if (inv[ti].curse_state == curse_state_t::blessed)
        {
            txtef(5);
            txt(lang(
                itemname(ci) + u8"は銀色に輝いた。"s,
                itemname(ci) + u8" shine"s + _s2(inv[ci].number)
                    + u8" silvery."s));
            inv[ci].curse_state = curse_state_t::blessed;
        }
        if (is_cursed(inv[ti].curse_state))
        {
            txtef(8);
            txt(lang(
                itemname(ci) + u8"は黒いオーラに包まれた。"s,
                itemname(ci) + u8" "s + is2(inv[ci].number)
                    + u8" wrapped by a dark aura."s));
            inv[ci].curse_state = curse_state_t::cursed;
        }
        snd(17);
        break;
    case 10007:
        txt(lang(
            itemname(ti, 1) + u8"を"s + itemname(ci) + u8"に放り込んだ。"s,
            u8"You throw "s + itemname(ci) + u8" into "s + itemname(ti, 1)
                + u8"."s));
        if (inv[ti].id == 601)
        {
            txt(lang(
                u8"空き瓶の割れる音がした。"s,
                u8"You hear the sound of the empty bottle shatters."s));
            break;
        }
        snd(17);
        if (inv[ci].id == 602)
        {
            txt(lang(u8"井戸は汚れた。"s, u8"The holy well is polluted."s));
            break;
        }
        if (inv[ci].param3 >= 20)
        {
            txt(lang(
                itemname(ci) + u8"は完全に枯れている。"s,
                itemname(ci) + u8" is completely dry."s));
            break;
        }
        txtef(2);
        txt(lang(
            itemname(ci) + u8"は一瞬輝いた。"s,
            itemname(ci) + u8" shines for a moment."s));
        if (inv[ti].id == 587)
        {
            txt(lang(u8"しかしこんな量では… "s, u8"But the snow just melts."s));
        }
        else
        {
            inv[ci].param1 += rnd(3);
        }
        break;
    case 10008:
        if (inv[ci].param1 < -5 || inv[ci].param3 >= 20
            || (inv[ci].id == 602 && gdata_holy_well_count <= 0))
        {
            const auto valn = itemname(ci);
            txt(lang(valn + u8"は涸れている。"s, valn + u8" is dry."s));
            txt(lang(
                u8"あっ！空き瓶を井戸に落としてしまった…"s,
                u8"Ops! You drop the empty bottle into the well..."s));
            break;
        }
        cibk = ci;
        if (inv[ci].id == 602)
        {
            --gdata_holy_well_count;
            flt();
            int stat = itemcreate(0, 516, -1, -1, 0);
            if (stat != 0)
            {
                inv[ci].curse_state = curse_state_t::blessed;
            }
        }
        else
        {
            inv[ci].param1 -= 3;
            flt(20);
            flttypemajor = 52000;
            itemcreate(0, 0, -1, -1, 0);
        }
        txt(lang(
            u8"空き瓶に水をすくった。"s,
            u8"You draw water from the well into the empty bottle."s));
        txtef(2);
        txt(lang(
            itemname(ci, 1) + u8"を手に入れた。"s,
            u8"You get "s + itemname(ci, 1) + u8"."s));
        item_stack(0, ci, 1);
        item_stack(0, ci);
        ci = cibk;
        snd(17);
        break;
    }

    item_stack(0, ci);
    if (inv[ci].body_part != 0)
    {
        create_pcpic(0, true);
    }
    if (inv_getowner(ci) == -1)
    {
        cell_refresh(inv[ci].position.x, inv[ci].position.y);
    }
    chara_refresh(0);
    return;
}

} // namespace elona
