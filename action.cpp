#include "action.hpp"
#include "ability.hpp"
#include "animation.hpp"
#include "audio.hpp"
#include "building.hpp"
#include "buff.hpp"
#include "calc.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "config.hpp"
#include "command.hpp" // TODO separate entirely
#include "ctrl_file.hpp"
#include "damage.hpp"
#include "debug.hpp"
#include "draw.hpp"
#include "enchantment.hpp"
#include "event.hpp"
#include "food.hpp"
#include "fov.hpp"
#include "gathering.hpp"
#include "i18n.hpp"
#include "input.hpp"
#include "item.hpp"
#include "item_db.hpp"
#include "itemgen.hpp"
#include "log.hpp"
#include "macro.hpp"
#include "map_cell.hpp"
#include "mef.hpp"
#include "menu.hpp"
#include "quest.hpp"
#include "ui.hpp"
#include "variables.hpp"
#include "version.hpp"

namespace elona
{

int r3 = 0;
int extraattack = 0;
int attackitem = 0;
int inumbk = 0;
std::string dbm;
int dbg_exitshowroom = 0;

void do_thing_that_feels_good()
{
    int sexhost = 0; // TODO move out into action.cpp, the rest into action_continuous.cpp
    if (cdata[cc].continuous_action_id == 0)
    {
        cdata[cc].continuous_action_id = 11;
        cdata[cc].continuous_action_turn = 25 + rnd(10);
        cdata[cc].continuous_action_target = tc;
        cdata[tc].continuous_action_id = 11;
        cdata[tc].continuous_action_turn = cdata[cc].continuous_action_turn * 2;
        cdata[tc].continuous_action_target = cc + 10000;
        if (fov_player_sees(cc))
        {
            txt(lang(
                name(cc) + u8"は服を脱ぎ始めた。"s,
                name(cc) + u8" begin"s + _s(cc) + u8" to take "s + his(cc)
                    + u8" clothes off."s));
        }
        return;
    }
    sexhost = 1;
    tc = cdata[cc].continuous_action_target;
    if (tc >= 10000)
    {
        tc -= 10000;
        sexhost = 0;
    }
    if (cdata[tc].state != 1 || cdata[tc].continuous_action_id != 11)
    {
        if (fov_player_sees(cc))
        {
            txt(lang(
                u8"「そ、その"s
                    + i18n::_(u8"ui", u8"sex2", u8"_"s + cdata[tc].sex)
                    + u8"とは体だけの関係"s + _da() + _ore(3)
                    + u8"は何も知らないから、命だけは…！」"s,
                u8"\"I-I don't really know that "s
                    + i18n::_(u8"ui", u8"sex2", u8"_"s + cdata[tc].sex)
                    + u8". Please spare my life!\""s));
        }
        rowactend(cc);
        rowactend(tc);
        return;
    }
    if (cc == 0)
    {
        if (!actionsp(0, 1 + rnd(2)))
        {
            txt(lang(u8"疲労し過ぎて失敗した！"s, u8"You are too exhausted!"s));
            rowactend(cc);
            rowactend(tc);
            return;
        }
    }
    cdata[cc].emotion_icon = 317;
    if (cdata[cc].continuous_action_turn > 0)
    {
        if (sexhost == 0)
        {
            if (cdata[cc].continuous_action_turn % 5 == 0)
            {
                if (fov_player_sees(cc))
                {
                    txtef(9);
                    if (jp)
                    {
                        txt(u8"「きくぅ」"s,
                            u8"「もふもふ」"s,
                            u8"「くやしい、でも…」"s,
                            u8"「はぁはぁ！」"s,
                            u8"「ウフフフ」"s);
                    }
                    if (en)
                    {
                        txt(u8"\"Yes!\""s,
                            u8"\"Ohhh\""s,
                            u8"*gasp*"s,
                            u8"*rumble*"s,
                            u8"\"come on!\""s);
                    }
                }
            }
        }
        return;
    }
    if (sexhost == 0)
    {
        rowactend(cc);
        return;
    }
    for (int cnt = 0; cnt < 2; ++cnt)
    {
        int c{};
        if (cnt == 0)
        {
            c = cc;
        }
        else
        {
            c = tc;
        }
        cdata[cc].drunk = 0;
        if (cnt == 1)
        {
            if (rnd(3) == 0)
            {
                dmgcon(c, 11, 500);
            }
            if (rnd(5) == 0)
            {
                dmgcon(c, 3, 500);
            }
            dmgcon(c, 11, 300);
            healsan(c, 10);
            skillexp(11, c, 250 + (c >= 57) * 1000);
            skillexp(15, c, 250 + (c >= 57) * 1000);
        }
        if (rnd(15) == 0)
        {
            dmgcon(c, 12, 200);
        }
        skillexp(17, c, 250 + (c >= 57) * 1000);
    }
    sexvalue = sdata(17, cc) * (50 + rnd(50)) + 100;
    if (fov_player_sees(cc))
    {
        txtef(9);
        if (jp)
        {
            txt(u8"「よかった"s + _yo(3),
                u8"「す、すごい"s + _yo(3),
                u8"「も、もうだめ"s + _da(3),
                u8"「は、激しかった"s + _yo(3),
                u8"「か、完敗"s + _da(3));
        }
        if (en)
        {
            txt(u8"\"You are awesome!"s,
                u8"\"Oh my god...."s,
                u8"\"Okay, okay, you win!"s,
                u8"\"Holy...!"s);
        }
        txtef(20);
        if (tc == 0)
        {
            txt(lang(u8"」"s, u8"\""s));
        }
    }
    if (tc != 0)
    {
        if (cdata[tc].gold >= sexvalue)
        {
            if (fov_player_sees(cc))
            {
                txt(lang(
                    u8"！さあ、小遣いを受け取って"s + _kure(3) + u8"」"s,
                    u8"Here, take this.\""s));
            }
        }
        else
        {
            if (fov_player_sees(cc))
            {
                txt(lang(
                    u8"！これが"s + _ore(3) + u8"の財布の中身の全て"s + _da()
                        + u8"」"s,
                    u8"Take this money, it's all I have!\""s));
                if (rnd(3) == 0)
                {
                    if (cc != 0)
                    {
                        txt(lang(
                            name(cc) + u8"は激怒した。「なめてんの？」"s,
                            name(cc)
                                + u8" gets furious, "s
                                  u8"\"And you think you can just run away?\""s));
                        cdata[cc].enemy_id = tc;
                        cdata[cc].hate = 20;
                    }
                }
            }
            if (cdata[tc].gold <= 0)
            {
                cdata[tc].gold = 1;
            }
            sexvalue = cdata[tc].gold;
        }
        cdata[tc].gold -= sexvalue;
        if (cc == 0)
        {
            chara_mod_impression(tc, 5);
            flt();
            itemcreate(
                -1, 54, cdata[cc].position.x, cdata[cc].position.y, sexvalue);
            txt(lang(
                u8"何かが足元に転がってきた。"s,
                u8"Something is put on the ground."s));
            modify_karma(0, -1);
        }
        else
        {
            cdata[cc].gold += sexvalue;
        }
    }
    rowactend(cc);
    rowactend(tc);
    return;
}



int do_decode_book()
{
    int cibkread = 0;
    if (cdata[cc].continuous_action_id == 0)
    {
        if (inv[ci].id == 687)
        {
            if (inv[ci].param2 != 0)
            {
                txt(lang(
                    u8"それは既に解読済みだ。"s,
                    u8"You already have decoded the book."s));
                return 0;
            }
        }
        if (cdata[cc].blind != 0)
        {
            if (fov_player_sees(cc))
            {
                txt(lang(
                    name(cc) + u8"は何も見えない。"s,
                    name(cc) + u8" can see nothing."s));
            }
            return 0;
        }
        cdata[cc].continuous_action_id = 2;
        if (inv[ci].id == 783)
        {
            p = 50;
        }
        else if (inv[ci].id == 687)
        {
            p = 50 + inv[ci].param1 * 50 + inv[ci].param1 * inv[ci].param1 * 20;
        }
        else
        {
            p = the_ability_db[efid]->sdataref4;
        }
        cdata[cc].continuous_action_turn = p / (2 + sdata(150, 0)) + 1;
        cdata[cc].continuous_action_item = ci;
        if (fov_player_sees(cc))
        {
            txt(lang(
                npcn(cc) + itemname(ci, 1) + u8"を読み始めた。"s,
                name(cc) + u8" start"s + _s(cc) + u8" to read "s
                    + itemname(ci, 1) + u8"."s));
        }
        item_separate(ci);
        return 0;
    }
    if (cdata[cc].continuous_action_turn > 0)
    {
        ci = cdata[cc].continuous_action_item;
        cibkread = ci;
        label_1458();
        if (inv[ci].id == 783)
        {
            return 0;
        }
        if (inv[ci].id == 687)
        {
            r2 =
                50 + inv[ci].param1 * 50 + inv[ci].param1 * inv[ci].param1 * 20;
            r3 = 0;
        }
        else
        {
            r2 = the_ability_db[efid]->sdataref4;
            r3 = efid;
        }
        if (inv[ci].curse_state == curse_state_t::blessed)
        {
            r2 = r2 * 100 / 120;
        }
        if (is_cursed(inv[ci].curse_state))
        {
            r2 = r2 * 150 / 100;
        }
        int stat = try_to_cast_spell();
        ci = cibkread;
        if (stat == 0)
        {
            rowactend(cc);
            --inv[ci].count;
            if (inv[ci].count < 0)
            {
                inv[ci].count = 0;
            }
            if (inv[ci].count == 0)
            {
                --inv[ci].number;
                if (ci >= 5080)
                {
                    cell_refresh(inv[ci].position.x, inv[ci].position.y);
                }
                else
                {
                    refresh_burden_state();
                }
                if (fov_player_sees(cc))
                {
                    txt(lang(
                        itemname(ci, 1) + u8"は塵となって崩れ落ちた。"s,
                        itemname(ci, 1) + u8" falls apart."s));
                }
            }
        }
        return 0;
    }
    if (fov_player_sees(cc))
    {
        txt(lang(
            npcn(cc) + itemname(ci, 1) + u8"を読み終えた。"s,
            name(cc) + u8" "s + have(cc) + u8" finished reading the book."s));
    }
    if (inv[ci].id == 783)
    {
        if (inv[ci].param1 == 0)
        {
            rowactend(cc);
            return 1;
        }
        txt(lang(
            itemname(ci, 1) + u8"を覚えた！"s, u8"You learned the recipe!"s));
        ++recipememory(inv[ci].subname);
        item_identify(inv[ci], identification_state_t::partly_identified);
        removeitem(ci, 1);
        if (fov_player_sees(cc))
        {
            txt(lang(
                itemname(ci, 1) + u8"は塵となって崩れ落ちた。"s,
                itemname(ci, 1) + u8" falls apart."s));
        }
        rowactend(cc);
        return 1;
    }
    if (inv[ci].id == 687)
    {
        item_identify(inv[ci], identification_state_t::completely_identified);
        txt(lang(
            itemname(ci, 1) + u8"を解読した！"s,
            u8"You finished decoding "s + itemname(ci, 1) + u8"!"s));
        inv[ci].param2 = 1;
        inv[ci].count = 1;
        ibitmod(4, ci, 0);
        item_stack(0, ci, 1);
    }
    else
    {
        skillgain(
            cc,
            efid,
            1,
            (rnd(51) + 50) * (90 + sdata(165, cc) + (sdata(165, cc) > 0) * 20)
                    / clamp((100 + spell((efid - 400)) / 2), 50, 1000)
                + 1);
        label_1471(0);
        if (itemmemory(2, inv[ci].id) == 0)
        {
            itemmemory(2, inv[ci].id) = 1;
        }
    }
    item_identify(inv[ci], identification_state_t::partly_identified);
    if (inv[ci].id != 687)
    {
        --inv[ci].count;
        if (inv[ci].count < 0)
        {
            inv[ci].count = 0;
        }
        if (inv[ci].count == 0)
        {
            removeitem(ci, 1);
            if (fov_player_sees(cc))
            {
                txt(lang(
                    itemname(ci, 1) + u8"は塵となって崩れ落ちた。"s,
                    itemname(ci, 1) + u8" falls apart."s));
            }
        }
    }
    rowactend(cc);
    return 1;
}



int do_read_normal_book()
{
    if (cdata[cc].blind != 0)
    {
        if (fov_player_sees(cc))
        {
            txt(lang(
                name(cc) + u8"は何も見えない。"s,
                name(cc) + u8" can see nothing."s));
        }
        return 0;
    }
    if (inv[ci].id == 742)
    {
        snd(59);
        txt(lang(
            u8"すくつの探索を許可する、という内容の文面が形式的に書いてある。"s,
            u8"According to the card, you are permitted to explore the void now."s));
        return 1;
    }
    if (inv[ci].id == 563)
    {
        int stat = label_2083();
        if (stat == 1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    if (inv[ci].id == 668)
    {
        snd(59);
        txt(lang(
            u8"レイチェルという作家による、心あたたまる童話集だ。"s,
            u8"It's a lovely fairy tale written by Rachel."s));
        return 1;
    }
    tc = cc;
    item_identify(inv[ci], identification_state_t::partly_identified);
    label_2022();
    return 1;
}



void do_rest()
{
    if (cdata[cc].continuous_action_id == 0)
    {
        cdata[cc].continuous_action_id = 4;
        cdata[cc].continuous_action_turn = 50;
        txt(lang(u8"あなたは横になった。"s, u8"You lie down to rest."s));
        update_screen();
        return;
    }
    if (cdata[cc].continuous_action_turn > 0)
    {
        if (cdata[cc].continuous_action_turn % 2 == 0)
        {
            healsp(cc, 1);
        }
        if (cdata[cc].continuous_action_turn % 3 == 0)
        {
            healhp(cc, 1);
            healmp(cc, 1);
        }
        return;
    }
    if (gdata_continuous_active_hours >= 30)
    {
        f = 0;
        if (gdata_continuous_active_hours >= 50)
        {
            f = 1;
        }
        else if (rnd(2) == 0)
        {
            f = 1;
        }
        if (f == 1)
        {
            txt(lang(
                u8"あなたはそのまま眠りにおちた…"s,
                u8"After a short while, you drop off to sleep."s));
            cdata[cc].continuous_action_item = -1;
            label_2151();
            rowactend(cc);
            return;
        }
    }
    txt(lang(u8"あなたは休息を終えた。"s, u8"You finished taking a rest."s));
    rowactend(cc);
    return;
}



void label_2149()
{
    pos(0, 0);
    gcopy(4, 0, 0, windoww, windowh - inf_verh);
    gmode(2);
    render_hud();
    if (screenupdate == 0)
    {
        redraw();
    }
    screenupdate = 0;
    return;
}



void label_2150()
{
    gsel(4);
    gmode(0);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"bg_night.bmp", 1);
    pos(0, 0);
    gzoom(4, 0, 0, 640, 480, windoww, windowh - inf_verh);
    gsel(0);
    return;
}

turn_result_t do_dig_after_sp_check()
{
    if (cdata[cc].sp < 0)
    {
        txt(lang(
            u8"疲れ過ぎて無理だ。"s, u8"You are too exhausted to do that."s));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    rowactre = 0;
    digx = tlocx;
    digy = tlocy;
    spot_mining_or_wall();
    return turn_result_t::turn_end;
}


optional<turn_result_t> check_angband()
{
    if (gdata_angband_flag == -1 || mdata(6) == 1)
        return none;

    switch (gdata_angband_flag)
    {
    case 0:
        if (key == u8"Q"s)
        {
            txt(lang(u8"え…", u8"What..."));
            ++gdata_angband_flag;
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        break;
    case 1:
        if (key == u8"y"s)
        {
            txt(lang(u8"まさか…", u8"No...no..."));
            ++gdata_angband_flag;
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        break;
    case 2:
        if (key == u8"@"s)
        {
            txt(lang(u8"うわぁぁぁ！！", u8"Ahhhhh!!"));
            for (int i = 0; i < 10; ++i)
            {
                flt();
                chara_create(-1, 37, cdata[0].position.x, cdata[0].position.y);
            }
            gdata_angband_flag = -1;
            update_screen();
            return turn_result_t::turn_end;
        }
        break;
    default: break;
    }

    gdata_angband_flag = 0;
    return none;
}


void do_play_scene()
{
    int scidx = 0;
    int scidxtop = 0;
    int val0{};
    if (gdata_played_scene < sceneid)
    {
        gdata_played_scene = sceneid;
    }
    if (config::instance().story == 0 || (en == 1 && sceneid != 0))
    {
        return;
    }
    scene_cut = 0;
    msgtempprev = msgtemp;
    scenemode = 1;
    SDIM4(actor, 20, 3, 10);
    std::string file = u8"void"s;
    y1 = 60;
    y2 = windowh - 60;
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
    s = u8"{"s + sceneid + u8"}"s;
    scidx = instr(buff, 0, s);
    if (scidx == -1)
    {
        label_2685();
        return;
    }
    scidx += s(0).size();
label_2681:
    await();
    int a{};
    stick(a, 128);
    if (a == 128)
    {
        scene_cut = 1;
    }
    if (scene_cut == 1)
    {
        label_2685();
        return;
    }
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
    cs = 0;
    key_list = key_enter;
    scidxtop = scidx;
    scidxtop = 0;
    val0 = 0;
label_2682_internal:
    p(1) = instr(buff, scidx, u8"{"s) + scidx;
    if (p(1) == -1)
    {
        label_2685();
        return;
    }
    p(2) = instr(buff, scidx, u8"}"s) + scidx + 1;
    if (p(2) == -1)
    {
        label_2685();
        return;
    }
    if (scidxtop != 0)
    {
        scidx = p(1);
        goto label_2684_internal;
        return;
    }
    s = strmid(buff, p(1), p(2) - p(1));
    scidx = p(2) + 1;
    if (s == u8"{txt}"s)
    {
        scidxtop = scidx;
        val0 = 0;
        goto label_2682_internal;
    }
    if (strutil::contains(s(0), u8"{chat_"))
    {
        rc = elona::stoi(strmid(s, 6, 1));
        scidxtop = scidx;
        val0 = 1;
        goto label_2682_internal;
    }
    if (s == u8"{fade}"s)
    {
        gsel(4);
        boxf();
        gsel(0);
        label_1443();
        goto label_2682_internal;
    }
    if (s == u8"{fadein}"s)
    {
        for (int cnt = 0; cnt < 25; ++cnt)
        {
            redraw();
            pos(0, 0);
            gmode(4, -1, -1, cnt * 15);
            pos(0, 0);
            gcopy(4, 0, 0, windoww, windowh);
            gmode(2);
            await(10);
        }
        goto label_2682_internal;
    }
    if (s == u8"{wait}"s)
    {
        scidxtop = scidx;
        val0 = 2;
        goto label_2682_internal;
    }
    if (s == u8"{end}"s)
    {
        await(1000);
        label_2685();
        return;
    }
    p(3) = instr(buff, scidx, u8"\""s) + scidx + 1;
    p(4) = instr(buff, p(3), u8"\""s) + p(3);
    s(1) = strmid(buff, p(3), p(4) - p(3));
    scidx = p(4) + 1;
    if (s == u8"{pic}"s)
    {
        file = s(1);
        goto label_2682_internal;
    }
    if (s == u8"{mc}"s)
    {
        auto music = -1;
        if (s(1) == u8"mcUnrest2"s)
        {
            music = 84;
        }
        if (s(1) == u8"mcTown1"s)
        {
            music = 51;
        }
        if (s(1) == u8"mcMemory"s)
        {
            music = 89;
        }
        if (s(1) == u8"mcIntro"s)
        {
            music = 90;
        }
        if (music == -1)
        {
            musicfile(91) = s(1);
            play_music(91);
        }
        else
        {
            play_music(music);
        }
        goto label_2682_internal;
    }
    if (s == u8"{se}"s)
    {
        sndload(filesystem::dir::sound() / s(1), 28);
        snd(28);
        goto label_2682_internal;
    }
    if (strutil::contains(s(0), u8"{actor_"))
    {
        rc = elona::stoi(strmid(s, 7, 1));
        csvsort(s, s(1), 44);
        actor(0, rc) = s;
        actor(1, rc) = s(1);
        actor(2, rc) = s(2);
    }
    goto label_2682_internal;
label_2684_internal:
    buff = strmid(buff, scidxtop, scidx - scidxtop);
    p = noteinfo();
    for (int cnt = 0, cnt_end = (noteinfo()); cnt < cnt_end; ++cnt)
    {
        noteget(s, p - cnt - 1);
        if (s == ""s)
        {
            notedel(p - cnt - 1);
        }
    }
    gsel(4);
    gmode(0);
    pos(0, 0);
    picload(filesystem::dir::graphic() / (u8""s + file + u8".bmp"), 1);
    pos(0, y1);
    gzoom(4, 0, 0, 640, 480, windoww, y2 - y1);
    gmode(2);
    boxf(0, 0, windoww, y1, {5, 5, 5});
    boxf(0, y2, windoww, windowh, {5, 5, 5});
    if (val0 == 1)
    {
        gsel(0);
        gmode(0);
        pos(0, 0);
        gcopy(4, 0, 0, windoww, windowh);
        gmode(2);
        tc = 0;
        talk_to_npc();
        goto label_2681;
    }
    if (val0 == 2)
    {
        gsel(0);
        gmode(0);
        pos(0, 0);
        gcopy(4, 0, 0, windoww, windowh);
        gmode(2);
        redraw();
        await(1000);
        goto label_2681;
    }
    font(16 - en * 2);
    x = 44;
    for (int cnt = 0, cnt_end = (noteinfo()); cnt < cnt_end; ++cnt)
    {
        y = y1 + 31 + (9 - noteinfo() / 2 + cnt) * 20;
        noteget(s, cnt);
        x = windoww / 2 - s(0).size() * 4;
        dx = 80 + s(0).size() * 8;
        if (dx < 180)
        {
            dx = 0;
        }
        pos(windoww / 2, y + 4);
        gmode(6, 344, 72, 70);
        grotate(3, 456, 144, 0, dx, 72);
    }
    x = 40;
    for (int cnt = 0, cnt_end = (noteinfo()); cnt < cnt_end; ++cnt)
    {
        y = y1 + 28 + (9 - noteinfo() / 2 + cnt) * 20;
        noteget(s, cnt);
        x = windoww / 2 - strlen_u(s(0)) * 4;
        color(10, 10, 10);
        pos(x, y);
        bmes(s, 240, 240, 240);
        color(0, 0, 0);
    }
    gsel(0);
    for (int cnt = 1; cnt < 16; ++cnt)
    {
        await(30);
        stick(a, 128);
        if (a == 128)
        {
            scene_cut = 1;
        }
        gmode(4, -1, -1, cnt * 16);
        pos(0, 0);
        gcopy(4, 0, 0, windoww, windowh);
        redraw();
    }
    gmode(2);
    gmode(0);
    pos(0, 0);
    gcopy(4, 0, 0, windoww, windowh);
    gmode(2);
    x_at_txtfunc = windoww - 120;
    y_at_txtfunc = windowh - 60;
    anime_halt();
    boxf(0, 0, windoww, y1, {5, 5, 5});
    boxf(0, y2, windoww, windowh, {5, 5, 5});
    goto label_2681;
}



void label_2685()
{
    gsel(4);
    boxf();
    gsel(0);
    label_1443();
    scenemode = 0;
    msgtemp = msgtempprev;
    msgtempprev = "";
    return;
}



void do_load_save_data()
{
    ELONA_LOG("Load save data: " << playerid);

    filemod = "";
    ctrl_file(file_operation_t::_10);
    const auto save_dir = filesystem::dir::save(playerid);
    buff(0).clear();
    if (!fs::exists(save_dir / u8"filelist.txt"))
    {
        ELONA_LOG("Load save data: from directory");
        for (const auto& entry :
             filesystem::dir_entries{save_dir,
                                     filesystem::dir_entries::type::file,
                                     std::regex{u8R"(.*\..*)"}})
        {
            buff += filesystem::to_utf8_path(entry.path().filename()) + '\n';
        }
    }
    else
    {
        ELONA_LOG("Load save data: from filelist.txt");
        std::ifstream in{(save_dir / u8"filelist.txt").native(),
                         std::ios::binary};
        std::string tmp;
        while (std::getline(in, tmp))
        {
            buff(0) +=
                filesystem::to_utf8_path(filesystem::path(tmp).filename())
                + '\n';
        }
    }
    notesel(buff);
    for (int cnt = 0, cnt_end = (noteinfo()); cnt < cnt_end; ++cnt)
    {
        noteget(s, cnt);
        if (strutil::contains(s(0), u8".s2"))
        {
            bcopy(save_dir / s(0), filesystem::dir::tmp() / s(0));
        }
    }
    ctrl_file(file_operation_t::_7);
    migrate_save_data();
    set_item_info();
    for (int cnt = 0; cnt < 16; ++cnt)
    {
        if (cdata[cnt].has_own_sprite() == 1 || cnt == 0)
        {
            create_pcpic(cnt, true);
        }
    }
    if (gdata_wizard == 1)
    {
        cdatan(1, 0) = u8"*Debug*"s;
    }
    refreshspeed(0);
    time_begin = timeGetTime() / 1000;
    return;
}



void do_save_game()
{
    ELONA_LOG("Save game: " << playerid);

    int save_f = 0;
    if (gdata_current_map == 35)
    {
        txtef(3);
        txt(lang(
            u8"ユーザーマップの中ではセーブできない。"s,
            u8"The game is not saved in a user map."s));
        update_screen();
        return;
    }
    ctrl_file(file_operation_t::_2);
    ctrl_file(file_operation2_t::_4, u8"inv_"s + mid + u8".s2");
    save_f = 0;
    for (const auto& entry : filesystem::dir_entries{
             filesystem::dir::save(), filesystem::dir_entries::type::dir})
    {
        if (filesystem::to_utf8_path(entry.path().filename()) == playerid)
        {
            save_f = 1;
            break;
        }
    }
    const auto save_dir = filesystem::dir::save(playerid);
    if (save_f == 0)
    {
        mkdir(save_dir);
    }
    notesel(filemod);
    for (int cnt = 0, cnt_end = (noteinfo()); cnt < cnt_end; ++cnt)
    {
        std::string save_s;
        noteget(save_s, cnt);
        if (save_s.empty())
            continue;
        const auto save_p = save_s.front() == '*';
        save_s = save_s.substr(1);
        const auto path = save_dir / filesystem::u8path(save_s);
        if (save_p)
        {
            bcopy(filesystem::dir::tmp() / filesystem::u8path(save_s), path);
        }
        else
        {
            if (fs::exists(path) && !fs::is_directory(path))
            {
                elona_delete(path);
            }
        }
    }
    ctrl_file(file_operation_t::_8);
    filemod = "";
    buff(0).clear();
    for (const auto& entry :
         filesystem::dir_entries{filesystem::dir::tmp(),
                                 filesystem::dir_entries::type::file,
                                 std::regex{u8R"(.*\..*)"}})
    {
        buff += filesystem::to_utf8_path(entry.path().filename()) + '\n';
    }
    notesel(buff);
    {
        std::ofstream out{
            (filesystem::dir::save(playerid) / u8"filelist.txt").native(),
            std::ios::binary};
        out << buff(0) << std::endl;
    }
    ELONA_LOG("Save game: finish");
}




turn_result_t do_enter_strange_gate()
{
    snd(49);
    gdata_previous_map2 = gdata_current_map;
    gdata_previous_dungeon_level = gdata_current_dungeon_level;
    gdata_previous_x = cdata[0].position.x;
    gdata_previous_y = cdata[0].position.y;
    gdata_destination_map = 35;
    gdata_destination_dungeon_level = 1;
    levelexitby = 2;
    return turn_result_t::exit_map;
}



turn_result_t do_debug_console()
{
    notesel(dbm);
    buff = "";
    if (dbm == ""s)
    {
        noteadd(latest_version.long_string());
        noteadd(u8"Debug Console    Type \"?\" for help. Hit ESC to exit."s);
        noteadd(""s);
    }
    font(14 - en * 2);
    objmode(2);
    pos(0, 24);
    mesbox(dbm);
    pos(0, 0);
    mesbox(buff);
    objsel(2);
    while (1)
    {
        await(20);
        int a{};
        stick(a);
        if (a == 128)
        {
            return do_exit_debug_console();
        }
    }
}



turn_result_t do_exit_debug_console()
{
    clrobj(1);
    clrobj(2);
    if (dbg_exitshowroom == 1)
    {
        dbg_exitshowroom = 0;
        levelexitby = 4;
        return turn_result_t::exit_map;
    }
    return turn_result_t::pc_turn_user_error;
}



void label_2151()
{
    int timeslept = 0;
    if (gdata_current_map == 13)
    {
        txt(lang(
            u8"しかし、大事な用を思い出して飛び起きた。"s,
            u8"But you can't sleep right now."s));
        gdata(98) = 0;
        return;
    }
    if (gdata_catches_god_signal)
    {
        txtgod(cdata[0].god_id, 10);
    }
    label_2150();
    musicloop = 1;
    play_music(78);
    msg_halt();
    for (int cnt = 0; cnt < 20; ++cnt)
    {
        gmode(4, -1, -1, cnt * 10);
        label_2149();
        await(200);
    }
    gmode(2);
    cc = 0;
    for (int cnt = 0; cnt < ELONA_MAX_PARTY_CHARACTERS; ++cnt)
    {
        tc = cnt;
        cdata[tc].wet = 0;
        cdata[tc].poisoned = 0;
        cdata[tc].sleep = 0;
        cdata[tc].confused = 0;
        cdata[tc].blind = 0;
        cdata[tc].paralyzed = 0;
        cdata[tc].dimmed = 0;
        cdata[tc].drunk = 0;
        cdata[tc].bleeding = 0;
        gdata_continuous_active_hours = 0;
        cdata[tc].hp = cdata[tc].max_hp;
        cdata[tc].mp = cdata[tc].max_mp;
        cdata[tc].sp = cdata[tc].max_sp;
        healcon(tc, 12, 7 + rnd(7));
        if (cdata[tc].has_anorexia())
        {
            cdata[tc].anorexia_count -= rnd(6);
        }
        else
        {
            cdata[tc].anorexia_count -= rnd(3);
        }
        if (cdata[tc].anorexia_count < 0)
        {
            cure_anorexia(tc);
            cdata[tc].anorexia_count = 0;
        }
        healsan(tc, 10);
        if (cdata[tc].has_lay_hand())
        {
            cdata[tc].is_lay_hand_available() = true;
        }
    }
    mode = 9;
    timeslept = 7 + rnd(5);
    for (int cnt = 0, cnt_end = (timeslept); cnt < cnt_end; ++cnt)
    {
        ++gdata_hour;
        label_2736();
        if (mode != 9)
        {
            label_2150();
            mode = 9;
        }
        gdata_continuous_active_hours = 0;
        gdata_minute = 0;
        cc = 0;
        label_2149();
        await(500);
    }
    if (gdata(98) != 0)
    {
        tc = -1;
        for (int cnt = 1; cnt < 16; ++cnt)
        {
            if (cdata[cnt].has_made_gene() == 1)
            {
                if (cdata[cnt].state == 1)
                {
                    tc = cnt;
                    break;
                }
            }
        }
        if (tc != -1)
        {
            s = lang(u8"遺伝子"s, u8"Gene"s);
            buff = lang(""s + name(tc) +
                    u8"とあなたは熱い一夜を供にした。新たな遺伝子が生成された。"s,
                u8"You spent a night with "s + name(tc) +
                    u8". A new gene is created."s);
            listmax = 0;
            list(0, listmax) = 1;
            listn(0, listmax) = lang(u8"ふぅ"s, u8"Sweet."s);
            ++listmax;
            cdata[tc].has_made_gene() = false;
            show_random_event_window(u8"bg_re14");
            save_gene();
        }
    }
    label_2149();
    gdata(98) = 0;
    mode = 0;
    wake_up();
    cdata[cc].nutrition -= 1500 / (trait(158) + 1);
    txtef(2);
    txt(lang(
        ""s + timeslept + u8"時間眠った。あなたはリフレッシュした。"s,
        u8"You have slept for "s + timeslept
            + u8" hours. You are refreshed."s));
    f = 0;
    if (cdata[0].continuous_action_item == -1)
    {
        f = 1;
    }
    else
    {
        ci = cdata[0].continuous_action_item;
        if (inv[ci].param1 == 0 || inv[ci].number == 0
            || the_item_db[inv[ci].id]->subcategory != 60004)
        {
            f = 1;
        }
    }
    if (f)
    {
        txt(lang(u8"まあまあの目覚めだ。"s, u8"You wake up feeling so so."s));
    }
    else
    {
        i = 0;
        for (int cnt = 10; cnt < 18; ++cnt)
        {
            i += sdata.get(cnt, 0).original_level;
        }
        i = clamp(i / 6, 10, 1000);
        exp = i * i * i / 10;
        gdata_sleep_experience = gdata_sleep_experience * inv[ci].param1 / 100;
        grown = 0;
        for (int cnt = 0;; ++cnt)
        {
            if (gdata_sleep_experience >= exp)
            {
                gdata_sleep_experience -= exp;
            }
            else if (cnt != 0)
            {
                break;
            }
            modify_potential(0, 10 + rnd(8), 1);
            ++grown;
            if (cnt > 6)
            {
                if (rnd(5) == 0)
                {
                    gdata_sleep_experience = 0;
                    break;
                }
            }
        }
        txtef(2);
        txt(lang(
            u8"心地よい目覚めだ。潜在能力が伸びた(計"s + grown + u8"%)"s,
            u8"You wake up feeling good. Your potential increases. (Total:"s
                + grown + u8"%)"s));
    }
    msg_halt();
    play_music();
    autosave = 1 * (gdata_current_map != 35);
    return;
}

int unlock_box(int difficulty)
{
    int stat = item_find(636, 3);
    if (stat == -1)
    {
        txt(lang(
            u8"ロックピックを所持していない。"s,
            u8"You don't have lockpicks."s));
        return 0;
    }
    ti = stat;
    txt(lang(u8"ロックピックを使用した。"s, u8"You use a lockpick."s));
    snd(22);
    {
        int stat = item_find(637, 3);
        if (stat != -1)
        {
            i = sdata(158, 0) * 150 / 100 + 5;
            txt(lang(
                u8"スケルトンキーも使用した。"s,
                u8"You also use the skeleton key."s));
        }
        else
        {
            i = sdata(158, 0);
        }
    }
    f = 0;
    if (i * 2 < difficulty)
    {
        txt(lang(
            u8"この鍵を開ける技術はない。"s,
            u8"The lock mechanism is beyond your skill."s));
        f = 1;
    }
    if (debug::voldemort)
    {
        f = 0;
    }
    if (f == 0)
    {
        if (i / 2 >= difficulty)
        {
            txt(lang(u8"楽勝だ。"s, u8"Easy."s));
        }
        else if (rnd(rnd(i * 2) + 1) < difficulty)
        {
            txt(lang(u8"開錠に失敗した。"s, u8"You fail to unlock it."s));
            f = 1;
        }
    }
    if (f)
    {
        if (rnd(3) == 0)
        {
            --inv[ti].number;
            cell_refresh(inv[ti].position.x, inv[ti].position.y);
            txt(lang(u8"ロックピックは壊れた。"s, u8"Your lockpick breaks."s));
        }
        txtnew();
        txt(lang(u8"もう一度試みる？"s, u8"Try again?"s));
        ELONA_YES_NO_PROMPT();
        rtval = show_prompt(promptx, prompty, 160);
        if (rtval == 0)
        {
            unlock_box(difficulty);
            return 0;
        }
        return 0;
    }
    txt(lang(u8"開錠に成功した。"s, u8"You successfully unlock it."s));
    gain_skill_experience_lock_picking(cc);
    return 1;
}



void open_box()
{
    snd(23);
    txt(lang(
        u8"あなたは"s + itemname(ci) + u8"を開けた。"s,
        u8"You open "s + itemname(ci) + u8"."s));
    msg_halt();
    ri = ci;
    if (inv[ri].id == 394)
    {
        tc = cc;
        efid = 1117;
        efp = 100 + inv[ri].param1 * 10;
        inv[ri].param1 = 0;
        magic();
        return;
    }
    p = 3 + rnd(5);
    if (inv[ri].id == 415 || inv[ri].id == 416 || inv[ri].id == 734)
    {
        p = 1;
    }
    randomize(inv[ri].param3);
    for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
    {
        dbid = 0;
        if (cnt == 0)
        {
            p = 3;
        }
        else
        {
            p = 2;
        }
        if (inv[ri].id == 239)
        {
            if (cnt == 0 && rnd(3) == 0)
            {
                p = 4;
            }
            else
            {
                p = 3;
            }
            if (rnd(60) == 0)
            {
                dbid = 559;
            }
        }
        flt(calcobjlv(inv[ri].param1), calcfixlv(p));
        flttypemajor = fsetchest(rnd(length(fsetchest)));
        if (cnt > 0)
        {
            if (rnd(3) != 0)
            {
                if (rnd(3) != 0)
                {
                    flttypeminor = 0;
                    flttypemajor = 68000;
                }
                else
                {
                    flttypemajor = 0;
                    flttypeminor = 77001;
                }
            }
        }
        if (inv[ri].id == 241)
        {
            if (rnd(3) != 0)
            {
                flttypeminor = 0;
                flttypemajor = 68000;
            }
            else
            {
                flttypemajor = 0;
                flttypeminor = 77001;
            }
        }
        if (inv[ri].id == 415 || inv[ri].id == 416)
        {
            flttypeminor = 0;
            flttypemajor = fsetwear(rnd(length(fsetwear)));
            fixlv = 3;
            if (inv[ri].id == 416)
            {
                fixlv = 4;
            }
            if (rnd(30) == 0)
            {
                dbid = 559;
            }
        }
        in = 0;
        if (inv[ri].id == 734)
        {
            dbid = 54;
            randomize();
            if (rnd(75) == 0)
            {
                in = 50 * inv[ci].value;
            }
            else
            {
                in = rnd(inv[ci].value / 10 + 1) + 1;
            }
        }
        if (inv[ri].id == 284)
        {
            dbid = 54;
            in = rnd(1000) + 1;
            if (rnd(5) == 0)
            {
                in = rnd(9) + 1;
            }
            if (rnd(10) == 0)
            {
                in = rnd(5000) + 5000;
            }
            if (rnd(20) == 0)
            {
                in = rnd(20000) + 10000;
            }
        }
        itemcreate(-1, dbid, cdata[0].position.x, cdata[0].position.y, in);
    }
    randomize();
    f = 0;
    if (inv[ri].id != 734)
    {
        if (rnd(10) == 0)
        {
            f = 1;
        }
    }
    if (inv[ri].id == 239 || inv[ri].id == 240)
    {
        if (rnd(5) == 0)
        {
            f = 1;
        }
    }
    if (f)
    {
        flt();
        itemcreate(-1, 622, cdata[0].position.x, cdata[0].position.y, 1);
    }
    snd(24);
    txt(lang(
        itemname(ri) + u8"から溢れ出た高級品が、足元に散らばった。"s,
        u8"Several quality goods spread out from "s + itemname(ri) + u8"."s));
    autosave = 1 * (gdata_current_map != 35);
    inv[ri].param1 = 0;
    if (inv[ri].id == 284)
    {
        modify_karma(0, -4);
    }
    if (inv[ri].id == 283)
    {
        modify_karma(0, -8);
    }
    return;
}



void open_new_year_gift()
{
    snd(23);
    txt(lang(
        u8"あなたは"s + itemname(ci) + u8"を開けた。"s,
        u8"You open "s + itemname(ci) + u8"."s));
    msg_halt();
    snd(24);
    randomize();
    ri = ci;
    cc = 0;
    inv[ri].param1 = 0;
    if (inv[ri].param3 < 100)
    {
        if (rnd(3) == 0)
        {
            if (fov_player_sees(cc))
            {
                txt(lang(
                    u8"悪意を持った何かが袋から飛び出してきた！"s,
                    u8"Something jumps out from the pack!"s));
            }
            for (int cnt = 0, cnt_end = (3 + rnd(3)); cnt < cnt_end; ++cnt)
            {
                flt(calcobjlv(cdata[0].level * 3 / 2 + 3), calcfixlv(2));
                chara_create(-1, 0, cdata[cc].position.x, cdata[cc].position.y);
            }
            return;
        }
        if (rnd(3) == 0)
        {
            if (fov_player_sees(cc))
            {
                txt(lang(
                    u8"罠だ！お年玉袋は発火した。"s,
                    u8"It's trap! The gift ignites."s));
            }
            for (int cnt = 0; cnt < 6; ++cnt)
            {
                tlocx = cdata[0].position.x + rnd(3) - rnd(3);
                tlocy = cdata[0].position.y - rnd(3) + rnd(3);
                if (tlocx < 0 || tlocx >= mdata(0) || tlocy < 0
                    || tlocy >= mdata(1))
                {
                    continue;
                }
                mef_add(tlocx, tlocy, 5, 24, rnd(15) + 20, 50, 0);
                mapitem_fire(tlocx, tlocy);
            }
            return;
        }
        if (fov_player_sees(cc))
        {
            txt(lang(
                u8"中には呪いの手紙が入っていた。"s,
                u8"You find a cursed letter inside."s));
        }
        efid = 1114;
        efp = 1000;
        tc = 0;
        magic();
        return;
    }
    if (inv[ri].param3 < 200)
    {
        if (rnd(4) == 0)
        {
            if (fov_player_sees(cc))
            {
                txtef(5);
                txt(lang(u8" *チリリリリーン* "s, u8"*ring ring ring*"s));
            }
            flt();
            int stat = chara_create(
                -1, 328 + rnd(2), cdata[cc].position.x, cdata[cc].position.y);
            if (stat != 0)
            {
                if (cdata[cc].relationship <= -3)
                {
                    cdata[rc].relationship = -1;
                    cdata[rc].original_relationship = -1;
                }
            }
            return;
        }
        if (rnd(5) == 0)
        {
            if (fov_player_sees(cc))
            {
                txt(lang(
                    u8"妹が入っていた！"s,
                    u8"The pack contains your younger sister!"s));
            }
            flt();
            int stat = chara_create(
                -1, 176, cdata[cc].position.x, cdata[cc].position.y);
            if (stat != 0)
            {
                cdata[rc].gold = 5000;
            }
            return;
        }
        if (fov_player_sees(cc))
        {
            txt(lang(
                u8"何かが袋から出てきた。"s,
                u8"There's something inside the pack."s));
        }
        flt();
        itemcreate(
            -1,
            isetgiftminor(rnd(length(isetgiftminor))),
            cdata[0].position.x,
            cdata[0].position.y,
            1);
        return;
    }
    if (rnd(3) == 0)
    {
        if (fov_player_sees(cc))
        {
            txtef(5);
            txt(lang(u8" *チリリリリーン* "s, u8"*ring ring ring*"s));
        }
        for (int cnt = 0, cnt_end = (2 + rnd(3)); cnt < cnt_end; ++cnt)
        {
            flt();
            int stat = chara_create(
                -1, 328 + rnd(2), cdata[cc].position.x, cdata[cc].position.y);
            if (stat != 0)
            {
                if (cdata[cc].relationship <= -3)
                {
                    cdata[rc].relationship = -1;
                    cdata[rc].original_relationship = -1;
                }
            }
        }
        return;
    }
    if (rnd(50) == 0)
    {
        if (fov_player_sees(cc))
        {
            txt(lang(
                u8"これは素晴らしい贈り物だ！"s,
                u8"This is truly a wonderful gift!"s));
        }
        flt();
        itemcreate(
            -1,
            isetgiftgrand(rnd(length(isetgiftgrand))),
            cdata[0].position.x,
            cdata[0].position.y,
            1);
        return;
    }
    if (fov_player_sees(cc))
    {
        txt(lang(
            u8"何かが袋から出てきた。"s,
            u8"There's something inside the pack."s));
    }
    flt();
    itemcreate(
        -1,
        isetgiftmajor(rnd(length(isetgiftmajor))),
        cdata[0].position.x,
        cdata[0].position.y,
        1);
    return;
}



turn_result_t try_to_open_locked_door()
{
    cell_featread(dx, dy);
    if (feat == tile_doorclosed4 && feat(2) > 0)
    {
        if (cc == 0)
        {
            int stat = unlock_box(feat(2));
            if (stat == 0)
            {
                screenupdate = -1;
                update_screen();
                await(100);
                return turn_result_t::turn_end;
            }
            feat(2) = 0;
        }
        else
        {
            return turn_result_t::turn_end;
        }
    }
    if (feat(2) > 0)
    {
        f = 0;
        if (rnd(feat(2) * 20 + 150) < sdata(158, cc) * 20 + 20)
        {
            f = 1;
        }
    }
    else
    {
        f = 1;
    }
    if (debug::voldemort)
    {
        f = 1;
    }
    if (f)
    {
        if (feat(2) > 0)
        {
            gain_skill_experience_lock_picking(cc);
        }
        cell_featset(dx, dy, tile_dooropen, 20, 0, -1);
        if (fov_player_sees(cc))
        {
            txt(lang(
                name(cc) + u8"は扉を開けた。"s,
                name(cc) + u8" open"s + _s(cc) + u8" the door."s));
            if (mdata(12) == 8)
            {
                snd(67);
            }
            else if (mdata(12) == 9)
            {
                snd(71);
            }
            else
            {
                snd(48);
            }
        }
    }
    else
    {
        ++msgdup;
        if (fov_player_sees(cc))
        {
            snd(22);
            txt(lang(
                aln(cc) + u8"開錠に失敗した。"s,
                name(cc) + u8" fail"s + _s(cc) + u8" to open the door."s));
        }
    }
    if (cc == 0)
    {
        await(100);
    }
    return turn_result_t::turn_end;
}



int label_2217()
{
    int ammox = 0;
    int ammoy = 0;
    attackrange = 1;
    attacknum = 0;
    ele = 0;
    ammoproc = -1;
    ammox = cdata[tc].position.x;
    ammoy = cdata[tc].position.y;
    attackitem = cw;
    if (ammo != -1)
    {
        if (inv[ammo].count != -1)
        {
            if (inv[ammo].enchantments[inv[ammo].count].power % 1000 <= 0)
            {
                txt(lang(u8"通常弾を装填した。"s, u8"You load normal ammo."s));
                inv[ammo].count = -1;
            }
            else
            {
                ammoproc = inv[ammo].enchantments[inv[ammo].count].id % 10000;
                if (cc == 0)
                {
                    if (cdata[0].sp < 50)
                    {
                        if (cdata[0].sp < rnd(75))
                        {
                            txt(lang(
                                u8"疲労し過ぎて失敗した！"s,
                                u8"You are too exhausted!"s));
                            dmgsp(0, encammoref(2, ammoproc) / 2 + 1);
                            ammoproc = -1;
                            return 1;
                        }
                    }
                    dmgsp(0, rnd(encammoref(2, ammoproc) + 1));
                }
                --inv[ammo].enchantments[inv[ammo].count].power;
            }
        }
    }
    if (ammoproc == 0)
    {
        for (int cnt = 0; cnt < 3; ++cnt)
        {
            can_do_ranged_attack();
            ele = 0;
            extraattack = 0;
            do_physical_attack();
            if (cdata[tc].state != 1)
            {
                int stat = label_2072();
                if (stat == 0)
                {
                    break;
                }
                else
                {
                    tc = cdata[cc].enemy_id;
                }
            }
        }
    }
    else if (ammoproc == 5)
    {
        for (int cnt = 0; cnt < 10; ++cnt)
        {
            can_do_ranged_attack();
            ele = 0;
            label_2076();
            if (listmax == 0)
            {
                break;
            }
            tc = list(0, rnd(listmax));
            if (cc == 0 || cdata[cc].relationship >= 0)
            {
                if (cdata[tc].relationship >= 0)
                {
                    if (cnt != 0)
                    {
                        cnt = cnt + (rnd(5) == 0) - 1;
                        continue;
                    }
                }
            }
            else if (cdata[tc].relationship == -3)
            {
                cnt = cnt + (rnd(5) == 0) - 1;
                continue;
            }
            extraattack = 0;
            do_physical_attack();
        }
    }
    else
    {
        extraattack = 0;
        do_physical_attack();
    }
    if (ammoproc == 1)
    {
        tlocx = ammox;
        tlocy = ammoy;
        efid = 460;
        efp = sdata(attackskill, cc) * 8 + 10;
        magic();
    }
    attackvar = 0;
    ammoproc = -1;
    return 0;
}



void try_to_melee_attack()
{
    if (cc != 0)
    {
        if (cdata[tc].damage_reaction_info)
        {
            if (distance < 6)
            {
                if (fov_los(
                        cdata[cc].position.x,
                        cdata[cc].position.y,
                        cdata[tc].position.x,
                        cdata[tc].position.y))
                {
                    int stat = can_do_ranged_attack();
                    if (stat == 1)
                    {
                        label_2217();
                    }
                }
            }
            p = cdata[tc].damage_reaction_info % 1000;
            if (p == 61)
            {
                if (cdata[cc].hp < cdata[cc].max_hp / 2)
                {
                    return;
                }
            }
        }
    }
    attacknum = 0;
    attackrange = 0;
    attackskill = 106;
    ammo = -1;
    ele = 0;
    if (cdata[cc].equipment_type & 1)
    {
        if (clamp(int(std::sqrt(sdata(168, cc)) - 3), 1, 5)
                + cdata[cc].has_power_bash() * 5
            > rnd(100))
        {
            if (fov_player_sees(cc))
            {
                txt(lang(
                    name(cc) + u8"は盾で"s + name(tc) + u8"を殴りつけた。"s,
                    name(cc) + u8" bash"s + _s(cc) + u8" "s + name(tc)
                        + u8" with "s + his(cc) + u8" shield."s));
            }
            dmghp(tc, rnd(sdata(168, cc)) + 1, cc);
            dmgcon(tc, 7, 50 + int(std::sqrt(sdata(168, cc))) * 15);
            cdata[tc].paralyzed += rnd(3);
        }
    }
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
        attackitem = cw;
        if (inv[cw].dice_x > 0)
        {
            attackskill = inv[cw].skill;
            ++attacknum;
            extraattack = 0;
            do_physical_attack();
        }
    }
    if (attackskill == 106)
    {
        extraattack = 0;
        do_physical_attack();
    }
    attackvar = 0;
    return;
}



void do_physical_attack()
{
    int attackdmg;
    int expmodifer = 0;
label_22191_internal:
    if (cdata[cc].state != 1)
    {
        return;
    }
    if (cdata[tc].state != 1)
    {
        return;
    }
    if (cdata[cc].fear != 0)
    {
        ++msgdup;
        txt(lang(
            name(cc) + u8"は怖気づいた。"s,
            name(cc) + u8" "s + is(cc) + u8" frightened."s));
        return;
    }
    if (map(cdata[tc].position.x, cdata[tc].position.y, 8) != 0)
    {
        bool return_now = mef_proc_from_physical_attack(tc);
        if(return_now)
        {
            return;
        }
    }
    if (attackrange == 1)
    {
        aniref = cw;
        play_animation(attackskill);
    }
    if (attacknum > 1 || cc != 0)
    {
    }
    expmodifer = 1 + cdata[tc].is_hung_on_sand_bag() * 15 + cdata[tc].splits()
        + cdata[tc].splits2() + (gdata_current_map == 35);
    int hit = calcattackhit();
    i = 0;
    if (hit == 1)
    {
        if (critical)
        {
            if (cc == 0)
            {
                txtef(3);
                txt(lang(u8"会心の一撃！ "s, u8"Critical Hit!"s));
            }
        }
        dmg = calcattackdmg();
        attackdmg = dmg;
        if (cc == 0)
        {
            if (config::instance().attackanime)
            {
                aniref = dmg * 100 / cdata[tc].max_hp;
                play_animation(12);
            }
        }
        if (attackskill != 106)
        {
            if (inv[cw].quality >= 4)
            {
                if (inv[cw].quality == 6)
                {
                    s(1) = lang(""s, u8"The "s) + iknownnameref(inv[cw].id);
                }
                else if (inv[cw].subname >= 40000)
                {
                    randomize(inv[cw].subname - 40000);
                    s(1) = random_title(1);
                    randomize();
                }
                else
                {
                    s(1) = lang(""s, u8"The "s) + iknownnameref(inv[cw].id);
                }
                s(1) = lang(u8"『"s, u8"<"s) + s(1) + lang(u8"』"s, u8">"s);
                if (fov_player_sees(cc))
                {
                    if (rnd(5) == 0)
                    {
                        txtef(9);
                        txt(lang(
                            name(cc) + u8"は"s + s(1)
                                + u8"を誇らしげに構えた。"s,
                            name(cc) + u8" wield"s + _s(cc) + u8" "s + s(1)
                                + u8" proudly."s));
                    }
                }
                i = 1;
            }
        }
        if (attackskill == 106)
        {
            if (cdata[cc].element_of_unarmed_attack != 0)
            {
                ele = cdata[cc].element_of_unarmed_attack / 100000;
                elep = cdata[cc].element_of_unarmed_attack % 100000;
            }
        }
        if (fov_player_sees(tc))
        {
            if (extraattack)
            {
                txt(lang(u8"さらに"s, u8"Furthermore,"s));
                txtcontinue();
            }
            if (attackskill == 106)
            {
                if (tc >= 16)
                {
                    gdata(809) = 2;
                    txt(lang(
                        aln(cc) + name(tc) + u8"を"s
                            + _melee(0, cdata[cc].melee_attack_type),
                        name(cc) + u8" "s
                            + _melee(0, cdata[cc].melee_attack_type) + _s(cc)
                            + u8" "s + name(tc) + u8" and"s));
                }
                else
                {
                    txt(lang(
                        aln(tc) + name(cc) + u8"に"s
                            + _melee(1, cdata[cc].melee_attack_type),
                        name(cc) + u8" "s
                            + _melee(1, cdata[cc].melee_attack_type) + _s(cc)
                            + u8" "s + name(tc) + u8"."s));
                }
            }
            if (attackskill == 108)
            {
                s = lang(u8"弓"s, u8"bow"s);
                if (tc >= 16)
                {
                    gdata(809) = 2;
                    txt(lang(
                        aln(cc) + name(tc) + u8"を射撃し"s,
                        name(cc) + u8" shoot"s + _s(cc) + u8" "s + name(tc)
                            + u8" and"s));
                }
                else
                {
                    txt(lang(
                        aln(tc) + name(cc) + u8"に"s + s(i) + u8"で撃たれた。"s,
                        name(cc) + u8" shoot"s + _s(cc) + u8" "s + name(tc)
                            + u8" with "s + his(cc) + u8" "s + s(i) + u8"."s));
                }
            }
            if (attackskill == 109)
            {
                s = lang(u8"クロスボウ"s, u8"crossbow"s);
                if (tc >= 16)
                {
                    gdata(809) = 2;
                    txt(lang(
                        aln(cc) + name(tc) + u8"を射撃し"s,
                        name(cc) + u8" shoot"s + _s(cc) + u8" "s + name(tc)
                            + u8" and"s));
                }
                else
                {
                    txt(lang(
                        aln(tc) + name(cc) + u8"に"s + s(i) + u8"で撃たれた。"s,
                        name(cc) + u8" shoot"s + _s(cc) + u8" "s + name(tc)
                            + u8" with "s + his(cc) + u8" "s + s(i) + u8"."s));
                }
            }
            if (attackskill == 110)
            {
                s = lang(u8"銃"s, u8"gun"s);
                if (tc >= 16)
                {
                    gdata(809) = 2;
                    txt(lang(
                        aln(cc) + name(tc) + u8"を射撃し"s,
                        name(cc) + u8" shoot"s + _s(cc) + u8" "s + name(tc)
                            + u8" and"s));
                }
                else
                {
                    txt(lang(
                        aln(tc) + name(cc) + u8"に"s + s(i) + u8"で撃たれた。"s,
                        name(cc) + u8" shoot"s + _s(cc) + u8" "s + name(tc)
                            + u8" with "s + his(cc) + u8" "s + s(i) + u8"."s));
                }
            }
            if (attackskill == 100)
            {
                s = lang(u8"長剣"s, u8"sword"s);
                if (tc >= 16)
                {
                    gdata(809) = 2;
                    txt(lang(
                        aln(cc) + name(tc) + u8"を切り払い"s,
                        name(cc) + u8" slash"s + _s(cc) + u8" "s + name(tc)
                            + u8" and"s));
                }
                else
                {
                    txt(lang(
                        aln(tc) + name(cc) + u8"に"s + s(i) + u8"で切られた。"s,
                        name(cc) + u8" slash"s + _s(cc) + u8" "s + name(tc)
                            + u8" with "s + his(cc) + u8" "s + s(i) + u8"."s));
                }
            }
            if (attackskill == 104)
            {
                s = lang(u8"槍"s, u8"spear"s);
                if (tc >= 16)
                {
                    gdata(809) = 2;
                    txt(lang(
                        aln(cc) + name(tc) + u8"を突き刺して"s,
                        name(cc) + u8" stab"s + _s(cc) + u8" "s + name(tc)
                            + u8" and"s));
                }
                else
                {
                    txt(lang(
                        aln(tc) + name(cc) + u8"に"s + s(i) + u8"で刺された。"s,
                        name(cc) + u8" stab"s + _s(cc) + u8" "s + name(tc)
                            + u8" with "s + his(cc) + u8" "s + s(i) + u8"."s));
                }
            }
            if (attackskill == 105)
            {
                s = lang(u8"杖"s, u8"staff"s);
                if (tc >= 16)
                {
                    gdata(809) = 2;
                    txt(lang(
                        aln(cc) + name(tc) + u8"を打って"s,
                        name(cc) + u8" smash"s + _s(cc) + u8" "s + name(tc)
                            + u8" and"s));
                }
                else
                {
                    txt(lang(
                        aln(tc) + name(cc) + u8"に"s + s(i) + u8"で打たれた。"s,
                        name(cc) + u8" smash"s + _s(cc) + u8" "s + name(tc)
                            + u8" with "s + his(cc) + u8" "s + s(i) + u8"."s));
                }
            }
            if (attackskill == 101)
            {
                s = lang(u8"短剣"s, u8"dagger"s);
                if (tc >= 16)
                {
                    gdata(809) = 2;
                    txt(lang(
                        aln(cc) + name(tc) + u8"を突き刺して"s,
                        name(cc) + u8" stab"s + _s(cc) + u8" "s + name(tc)
                            + u8" and"s));
                }
                else
                {
                    txt(lang(
                        aln(tc) + name(cc) + u8"に"s + s(i) + u8"で刺された。"s,
                        name(cc) + u8" stab"s + _s(cc) + u8" "s + name(tc)
                            + u8" with "s + his(cc) + u8" "s + s(i) + u8"."s));
                }
            }
            if (attackskill == 103)
            {
                s = lang(u8"鈍器"s, u8"mace"s);
                if (tc >= 16)
                {
                    gdata(809) = 2;
                    txt(lang(
                        aln(cc) + name(tc) + u8"を打って"s,
                        name(cc) + u8" smash"s + _s(cc) + u8" "s + name(tc)
                            + u8" and"s));
                }
                else
                {
                    txt(lang(
                        aln(tc) + name(cc) + u8"に"s + s(i) + u8"で打たれた。"s,
                        name(cc) + u8" smash"s + _s(cc) + u8" "s + name(tc)
                            + u8" with "s + his(cc) + u8" "s + s(i) + u8"."s));
                }
            }
            if (attackskill == 102)
            {
                s = lang(u8"斧"s, u8"axe"s);
                if (tc >= 16)
                {
                    gdata(809) = 2;
                    txt(lang(
                        aln(cc) + name(tc) + u8"を切り払い"s,
                        name(cc) + u8" slash"s + _s(cc) + u8" "s + name(tc)
                            + u8" and"s));
                }
                else
                {
                    txt(lang(
                        aln(tc) + name(cc) + u8"に"s + s(i) + u8"で切られた。"s,
                        name(cc) + u8" slash"s + _s(cc) + u8" "s + name(tc)
                            + u8" with "s + his(cc) + u8" "s + s(i) + u8"."s));
                }
            }
            if (attackskill == 107)
            {
                s = lang(u8"鎌"s, u8"scythe"s);
                if (tc >= 16)
                {
                    gdata(809) = 2;
                    txt(lang(
                        aln(cc) + name(tc) + u8"を切り払い"s,
                        name(cc) + u8" slash"s + _s(cc) + u8" "s + name(tc)
                            + u8" and"s));
                }
                else
                {
                    txt(lang(
                        aln(tc) + name(cc) + u8"に"s + s(i) + u8"で切られた。"s,
                        name(cc) + u8" slash"s + _s(cc) + u8" "s + name(tc)
                            + u8" with "s + his(cc) + u8" "s + s(i) + u8"."s));
                }
            }
            if (attackskill == 111)
            {
                s = itemname(cw, 1, 1);
                if (tc >= 16)
                {
                    gdata(809) = 2;
                    txt(lang(
                        aln(cc) + name(tc) + u8"に"s + s(i) + u8"を投げ"s,
                        name(cc) + u8" shoot"s + _s(cc) + u8" "s + name(tc)
                            + u8" and"s));
                }
                else
                {
                    txt(lang(
                        aln(tc) + name(cc) + u8"に"s + s(i)
                            + u8"で攻撃された。"s,
                        name(cc) + u8" shoot"s + _s(cc) + u8" "s + name(tc)
                            + u8" with "s + his(cc) + u8" "s + s(i) + u8"."s));
                }
            }
        }
        dmghp(tc, dmg, cc, ele, elep);
        if (critical)
        {
            skillexp(186, cc, 60 / expmodifer, 2);
            critical = 0;
        }
        if (rtdmg > cdata[tc].max_hp / 20 || rtdmg > sdata(154, tc)
            || rnd(5) == 0)
        {
            skillexp(
                attackskill,
                cc,
                clamp((sdata(173, tc) * 2 - sdata(attackskill, cc) + 1), 5, 50)
                    / expmodifer,
                0,
                4);
            if (attackrange == 0)
            {
                skillexp(152, cc, 20 / expmodifer, 0, 4);
                if (cdata[cc].equipment_type & 2)
                {
                    skillexp(167, cc, 20 / expmodifer, 0, 4);
                }
                if (cdata[cc].equipment_type & 4)
                {
                    skillexp(166, cc, 20 / expmodifer, 0, 4);
                }
            }
            else if (attackskill == 111)
            {
                skillexp(152, cc, 10 / expmodifer, 0, 4);
            }
            else
            {
                skillexp(189, cc, 25 / expmodifer, 0, 4);
            }
            if (cc == 0)
            {
                if (gdata_mount != 0)
                {
                    skillexp(301, 0, 30 / expmodifer, 0, 5);
                }
            }
            if (cdata[tc].state == 1)
            {
                skillexp(
                    chara_armor_class(tc),
                    tc,
                    clamp((250 * rtdmg / cdata[tc].max_hp + 1), 3, 100)
                        / expmodifer,
                    0,
                    5);
                if (cdata[tc].equipment_type & 1)
                {
                    skillexp(168, tc, 40 / expmodifer, 0, 4);
                }
            }
        }
        if (attackskill != 106)
        {
            label_2220();
        }
        if (cdata[tc].cut_counterattack > 0)
        {
            if (attackrange == 0)
            {
                dmghp(
                    cc,
                    attackdmg * cdata[tc].cut_counterattack / 100 + 1,
                    tc,
                    61,
                    100);
            }
        }
        if (cdata[tc].damage_reaction_info != 0)
        {
            p = cdata[tc].damage_reaction_info % 1000;
            ccbk = cc;
            for (int cnt = 0; cnt < 1; ++cnt)
            {
                if (attackrange == 0)
                {
                    if (p == 61)
                    {
                        if (fov_player_sees(cc))
                        {
                            txtef(8);
                            txt(lang(
                                u8"棘が"s + name(cc) + u8"に刺さった。"s,
                                name(cc) + u8" "s + is(cc)
                                    + u8" stuck by several thorns."s));
                        }
                        dmghp(
                            cc,
                            clamp(attackdmg / 10, 1, cdata[tc].max_hp / 10),
                            tc,
                            p,
                            cdata[tc].damage_reaction_info / 1000);
                        break;
                    }
                    if (p == 62)
                    {
                        if (fov_player_sees(cc))
                        {
                            txtef(8);
                            txt(lang(
                                u8"エーテルの棘が"s + name(cc)
                                    + u8"に刺さった。"s,
                                name(cc) + u8" "s + is(cc)
                                    + u8" stuck by several ether thorns."s));
                        }
                        dmghp(
                            cc,
                            clamp(attackdmg / 10, 1, cdata[tc].max_hp / 10),
                            tc,
                            p,
                            cdata[tc].damage_reaction_info / 1000);
                        break;
                    }
                    if (p == 63)
                    {
                        if (attackskill != 106)
                        {
                            if (rnd(5) == 0)
                            {
                                item_acid(cc, cw);
                            }
                        }
                    }
                }
                if (attackdmg > cdata[tc].max_hp / 10)
                {
                    cc = tc;
                    tlocx = cdata[cc].position.x;
                    tlocy = cdata[cc].position.y;
                    if (p == 63)
                    {
                        if (fov_player_sees(tc))
                        {
                            txtef(8);
                            txt(lang(
                                u8"酸が飛び散った。"s,
                                u8"Acids spread over the ground."s));
                        }
                        efid = 455;
                        efp = cdata[tc].damage_reaction_info / 1000;
                        magic();
                        break;
                    }
                }
            }
            cc = ccbk;
        }
    }
    else
    {
        if (cc == 0)
        {
            snd(4);
        }
        if (sdata(attackskill, cc) > sdata(173, tc) || rnd(5) == 0)
        {
            p = clamp((sdata(attackskill, cc) - sdata(173, tc) / 2 + 1), 1, 20)
                / expmodifer;
            skillexp(173, tc, p, 0, 4);
            skillexp(187, tc, p, 0, 4);
        }
    }
    if (hit == -1)
    {
        if (fov_player_sees(cc))
        {
            if (extraattack)
            {
                txt(lang(u8"さらに"s, u8"Furthermore,"s));
                txtcontinue();
            }
            if (tc < 16)
            {
                txt(lang(
                    aln(tc) + name(cc) + u8"の攻撃を避けた。"s,
                    name(tc) + u8" evade"s + _s(tc) + u8" "s + name(cc)
                        + u8"."s));
            }
            else
            {
                txt(lang(
                    aln(cc) + u8"攻撃をかわされた。"s,
                    name(cc) + u8" miss"s + _s(cc, true) + u8" "s + name(tc)
                        + u8"."s));
            }
            add_damage_popup(u8"miss", tc, {0, 0, 0});
        }
    }
    if (hit == -2)
    {
        if (fov_player_sees(cc))
        {
            if (extraattack)
            {
                txt(lang(u8"さらに"s, u8"Furthermore,"s));
                txtcontinue();
            }
            if (tc < 16)
            {
                txt(lang(
                    aln(tc) + name(cc) + u8"の攻撃を華麗に避けた。"s,
                    name(tc) + u8" skillfully evade"s + _s(tc) + u8" "s
                        + name(cc) + u8"."s));
            }
            else
            {
                txt(lang(
                    aln(cc) + u8"攻撃を華麗にかわされた。"s,
                    name(tc) + u8" skillfully evade"s + _s(tc) + u8" "s
                        + name(cc) + u8"."s));
            }
            add_damage_popup(u8"evade!!", tc, {0, 0, 0});
        }
    }
    if (tc == 0)
    {
        gdata(30) = 0;
    }
    rowact_check(tc);
    if (attackskill != 106)
    {
        if (cdata[tc].state != 1)
        {
            cw = attackitem;
            if (ibit(10, cw))
            {
                if (inv[cw].param2 < calcexpalive(inv[cw].param1))
                {
                    inv[cw].param2 += rnd(cdata[tc].level / inv[cw].param1 + 1);
                    if (inv[cw].param2 >= calcexpalive(inv[cw].param1))
                    {
                        txtef(2);
                        snd(61);
                        txt(lang(
                            itemname(cw) + u8"は十分に血を味わった！"s,
                            itemname(cw) + u8" has tasted enough blood!"s));
                    }
                }
            }
        }
    }
    ele = 0;
    if (extraattack == 0)
    {
        if (attackrange)
        {
            if (rnd(100) < cdata[cc].extra_shot)
            {
                ammoproc = -1;
                ++extraattack;
                goto label_22191_internal;
            }
        }
        else if (rnd(100) < cdata[cc].extra_attack)
        {
            ++extraattack;
            goto label_22191_internal;
        }
    }
    return;
}



void label_2220()
{
    for (int cnt = 0; cnt < 15; ++cnt)
    {
        cw = attackitem;
        if (inv[cw].enchantments[cnt].id == 0)
        {
            break;
        }
        enc = inv[cw].enchantments[cnt].id;
        if (enc == 36)
        {
            p = rnd(inv[cw].enchantments[cnt].power / 50 + 1) + 1;
            healsp(cc, p);
            dmgsp(tc, p / 2);
            continue;
        }
        if (enc == 38)
        {
            p = rnd(inv[cw].enchantments[cnt].power / 25 + 1) + 1;
            healmp(cc, p / 5);
            if (cdata[tc].state != 1)
            {
                continue;
            }
            dmgmp(tc, p);
            continue;
        }
        if (enc == 37)
        {
            if (rnd(66) == 0)
            {
                event_add(18, cc);
            }
            continue;
        }
        if (enc == 40)
        {
            if (gdata_left_turns_of_timestop == 0)
            {
                if (rnd(25) == 0)
                {
                    txtef(9);
                    txt(lang(
                        name(cc) + u8"は時を止めた。"s,
                        name(cc) + u8" stop"s + _s(cc) + u8" time."s));
                    gdata_left_turns_of_timestop =
                        inv[cw].enchantments[cnt].power / 100 + 1 + 1;
                }
                continue;
            }
        }
        if (enc == 57)
        {
            s = chara_refstr(cdata[tc].id, 8);
            if (strutil::contains(s(0), u8"/dragon/"))
            {
                gdata(809) = 1;
                dmghp(tc, orgdmg / 2, cc);
            }
            continue;
        }
        if (enc == 61)
        {
            s = chara_refstr(cdata[tc].id, 8);
            if (strutil::contains(s(0), u8"/god/"))
            {
                gdata(809) = 1;
                dmghp(tc, orgdmg / 2, cc);
            }
            continue;
        }
        if (enc == 58)
        {
            s = chara_refstr(cdata[tc].id, 8);
            if (strutil::contains(s(0), u8"/undead/"))
            {
                gdata(809) = 1;
                dmghp(tc, orgdmg / 2, cc);
            }
            continue;
        }
        i = enc / 10000;
        if (i != 0)
        {
            enc = enc % 10000;
            if (i == 7)
            {
                if (orgdmg <= 1)
                {
                    continue;
                }
                ele = enc;
                if (cdata[tc].state != 1)
                {
                    continue;
                }
                gdata(809) = 1;
                dmghp(
                    tc,
                    rnd(orgdmg * (100 + inv[cw].enchantments[cnt].power) / 1000
                        + 1)
                        + 5,
                    cc,
                    ele,
                    inv[cw].enchantments[cnt].power / 2 + 100);
                continue;
            }
            if (i == 8)
            {
                if (cdata[tc].state != 1)
                {
                    continue;
                }
                if (enc >= 26)
                {
                    continue;
                }
                tcbk = tc;
                p = encprocref(1, enc);
                if (p == 3000 || p == 10000)
                {
                    tc = cc;
                }
                p = encprocref(5, enc);
                enc = encprocref(0, enc);
                tlocx = cdata[tc].position.x;
                tlocy = cdata[tc].position.y;
                if (rnd(100) < p)
                {
                    efid = enc;
                    efp = inv[cw].enchantments[cnt].power
                        + sdata(attackskill, cc) * 10;
                    magic();
                }
                tc = tcbk;
                continue;
            }
            continue;
        }
    }
    if (ammoproc == 4)
    {
        txtef(9);
        txt(lang(
            name(cc) + u8"は時を止めた。"s,
            name(cc) + u8" stop"s + _s(cc) + u8" time."s));
        gdata_left_turns_of_timestop = 4;
    }
    if (ammoproc == 3)
    {
        if (cdata[tc].state == 1)
        {
            gdata(809) = 1;
            dmghp(
                tc,
                orgdmg * 2 / 3,
                cc,
                rnd(11) + 50,
                sdata(attackskill, cc) * 10 + 100);
        }
    }
    return;
}



void discover_trap()
{
    cell_featset(refx, refy, tile_trap, -1, -1);
    return;
}



void discover_hidden_path()
{
    map(refx, refy, 0) = tile_tunnel;
    cell_featset(refx, refy, 0, 0);
    return;
}


void dipcursed(int prm_1078, int)
{
    if (the_item_db[inv[prm_1078].id]->category == 57000)
    {
        if (inv[prm_1078].material == 35)
        {
            txt(lang(
                itemname(prm_1078) + u8"は腐ってしまった…"s,
                ""s + itemname(prm_1078) + u8" rots."s));
            inv[prm_1078].param3 = -1;
            inv[prm_1078].image = 336;
            cell_refresh(inv[prm_1078].position.x, inv[prm_1078].position.y);
            return;
        }
        else
        {
            txt(lang(
                itemname(prm_1078) + u8"に変化はない。"s,
                ""s + itemname(prm_1078) + u8" remains unchanged."s));
            return;
        }
    }
    if (the_item_db[inv[prm_1078].id]->category < 50000)
    {
        --inv[prm_1078].enhancement;
        txt(lang(
            itemname(prm_1078) + u8"は錆びてしまった…"s,
            ""s + itemname(prm_1078) + u8" rusts."s));
        if (inv_getowner(prm_1078) != -1)
        {
            chara_refresh(inv_getowner(prm_1078));
        }
        return;
    }
    txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
    return;
}

void prompt_stop_continuous_action()
{
    txt(lang(
        i18n::_(u8"ui", u8"action", u8"_"s + cdata[cc].continuous_action_id)
            + u8"を中断したほうがいいだろうか？ "s,
        u8"Do you want to cancel "s
            + i18n::_(
                  u8"ui", u8"action", u8"_"s + cdata[cc].continuous_action_id)
            + u8"? "s));
    ELONA_YES_NO_PROMPT();
    rtval = show_prompt(promptx, prompty, 160);
    return;
}



void label_2081()
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
            update_screen();
            return;
        }
    }
    p = 0;
    p = 0;
    i = 7;
    if (adata(30, i) == gdata(850))
    {
        list(0, p) = i;
        list(1, p) = 1;
        ELONA_APPEND_PROMPT(mapname(i), u8"null"s, ""s + promptmax);
        ++p;
    }
    for (int cnt = 0; cnt < 500; ++cnt)
    {
        i = 500 - (cnt + 1);
        if (adata(16, i) == 0)
        {
            continue;
        }
        if (adata(30, i) != gdata(850))
        {
            continue;
        }
        if (adata(16, i) == 7)
        {
            continue;
        }
        if (adata(6, i) == 0)
        {
            continue;
        }
        f = 0;
        if (adata(16, i) == 3 || adata(16, i) == 25 || adata(16, i) == 29
            || adata(16, i) == 42)
        {
            f = 1;
        }
        if (gdata_wizard)
        {
            if (adata(0, i) == 3 || adata(0, i) == 2)
            {
                f = 1;
            }
        }
        if (f == 1)
        {
            list(0, p) = i;
            list(1, p) = adata(6, i);
            ELONA_APPEND_PROMPT(
                mapname(i) + u8" "s + cnvrank((adata(6, i) - adata(17, i) + 1))
                    + lang(u8"階"s, u8" Lv"s),
                u8"null"s,
                ""s + promptmax);
            ++p;
        }
    }
    listmax = p;
    if (listmax == 0)
    {
        txt(lang(
            u8"この大陸には帰還できる場所が無い。"s,
            u8"You don't know any location you can return to"s));
        return;
    }
    txt(lang(u8"どの場所に帰還する？"s, u8"Where do you want to go?"s));
    display_msg(inf_screeny + inf_tiles);
    rtval = show_prompt(promptx, prompty, 240);
    update_screen();
    if (rtval >= 0)
    {
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
        gdata_destination_map = list(0, rtval);
        gdata_destination_dungeon_level = list(1, rtval);
        gdata_is_returning_or_escaping = 15 + rnd(15);
    }
    update_screen();
    return;
}

turn_result_t do_pray()
{
    if (cdata[0].god_id.empty())
    {
        txt(lang(
            name(0) + u8"は神を信仰していないが、試しに祈ってみた。"s,
            u8"You don't believe in God."s));
        return turn_result_t::turn_end;
    }
    txtnew();
    txt(lang(u8"あなたの神に祈りを乞う？"s, u8"Really pray to your God?"s));
    ELONA_YES_NO_PROMPT();
    rtval = show_prompt(promptx, prompty, 160);
    if (rtval != 0)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    txt(lang(
        i18n::_(u8"god", cdata[0].god_id, u8"name") + u8"に祈った。"s,
        u8"You pray to "s + i18n::_(u8"god", cdata[0].god_id, u8"name")
            + u8"."s));
    if (cdata[0].piety_point < 200 || cdata[0].praying_point < 1000)
    {
        txt(lang(
            i18n::_(u8"god", cdata[0].god_id, u8"name")
                + u8"はあなたに無関心だ。"s,
            i18n::_(u8"god", cdata[0].god_id, u8"name")
                + u8" is indifferent to you."s));
        return turn_result_t::turn_end;
    }
    animode = 100;
    play_animation(19);
    snd(120);
    efid = 1120;
    efp = 100;
    tc = 0;
    magic();
    efid = 451;
    efp = 200;
    tc = 0;
    magic();
    cdata[0].praying_point = 0;
    cdata[0].piety_point = cdata[0].piety_point * 85 / 100;
    if (gdata_god_rank % 2 == 1)
    {
        txtgod(cdata[0].god_id, 6);
        if (gdata_god_rank == 1)
        {
            f = 0;
            p = 0;
            for (int cnt = 1; cnt < 16; ++cnt)
            {
                if (cdata[cnt].state != 0)
                {
                    if (cdatan(2, cnt) == u8"servant"s)
                    {
                        ++p;
                        if (p >= 2)
                        {
                            f = 1;
                            txt(lang(
                                u8"神の使徒は2匹までしか仲間にできない。"s,
                                u8"No more than 2 God's servants are allowed in your party."s));
                            break;
                        }
                    }
                }
            }
            if (f == 0)
            {
                if (chara_get_free_slot_ally() == 0)
                {
                    f = 1;
                    txt(lang(
                        u8"仲間が一杯で、神からの贈り物を受け取ることができなかった。"s,
                        u8"Your party is full. The gift is reserved."s));
                }
            }
            if (f)
            {
                txt(lang(
                    u8"この贈り物を諦める？"s,
                    u8"Do you want to decline this gift?"s));
                ELONA_YES_NO_PROMPT();
                rtval = show_prompt(promptx, prompty, 160);
                if (rtval == 0)
                {
                    ++gdata_god_rank;
                }
                return turn_result_t::turn_end;
            }
            flt();
            dbid = 0;
            txtef(4);
            if (cdata[0].god_id == core_god::mani)
            {
                dbid = 262;
                txt(lang(
                    u8"このアンドロイドはブーストした時に恐るべき力を発揮するようだ。"s,
                    u8"This android shows enormous strength when boosting."s));
            }
            if (cdata[0].god_id == core_god::lulwy)
            {
                dbid = 263;
                txt(lang(
                    u8"この黒天使はブーストした時に恐るべき力を発揮するようだ。"s,
                    u8"This black angel shows enormous strength when boosting."s));
            }
            if (cdata[0].god_id == core_god::itzpalt)
            {
                dbid = 264;
                txt(lang(
                    u8"この追放者は連続魔法を使えるようだ。"s,
                    u8"This exile can cast several spells in a row."s));
            }
            if (cdata[0].god_id == core_god::ehekatl)
            {
                dbid = 260;
                txt(lang(
                    u8"この猫に舐められた武具は、エヘカトルの祝福を授かるようだ。祝福を受けた武具にはエンチャントが一つ付与される。"s,
                    u8"Weapons and armor licked by this cat receive a blessing of Ehekatl which adds an extra enchantment."s));
            }
            if (cdata[0].god_id == core_god::opatos)
            {
                dbid = 265;
                txt(lang(
                    u8"この騎士はある程度重いものをもたせても文句をいわないようだ。"s,
                    u8"This knight can hold really heavy stuff for you."s));
            }
            if (cdata[0].god_id == core_god::jure)
            {
                dbid = 266;
                txt(lang(
                    u8"この防衛者は致死ダメージを受けた仲間をレイハンドで回復できるようだ。レイハンドは眠るたびに再使用可能になる。"s,
                    u8"This defender can use Lay on hand to heal a mortally wounded ally. The ability becomes re-useable after sleeping."s));
            }
            if (cdata[0].god_id == core_god::kumiromi)
            {
                dbid = 261;
                txt(lang(
                    u8"この妖精は食後に種を吐き出すようだ。"s,
                    u8"This fairy generates a seed after eating."s));
            }
            novoidlv = 1;
            chara_create(56, dbid, -3, 0);
            rc = 56;
            new_ally_joins();
        }
        if (gdata_god_rank == 3)
        {
            flt();
            dbid = 0;
            if (cdata[0].god_id == core_god::lulwy)
            {
                dbid = 680;
            }
            if (cdata[0].god_id == core_god::jure)
            {
                dbid = 681;
            }
            if (cdata[0].god_id == core_god::kumiromi)
            {
                dbid = 682;
            }
            if (cdata[0].god_id == core_god::mani)
            {
                dbid = 683;
            }
            if (dbid != 0)
            {
                if (itemmemory(1, dbid))
                {
                    dbid = 559;
                }
                itemcreate(
                    -1, dbid, cdata[0].position.x, cdata[0].position.y, 0);
            }
            else
            {
                nostack = 1;
                itemcreate(
                    -1, 672, cdata[0].position.x, cdata[0].position.y, 0);
                if (cdata[0].god_id == core_god::itzpalt)
                {
                    inv[ci].param1 = 165;
                }
                if (cdata[0].god_id == core_god::ehekatl)
                {
                    inv[ci].param1 = 163;
                }
                if (cdata[0].god_id == core_god::opatos)
                {
                    inv[ci].param1 = 164;
                }
            }
            if (cdata[0].god_id == core_god::jure)
            {
                flt();
                nostack = 1;
                itemcreate(
                    -1, 672, cdata[0].position.x, cdata[0].position.y, 0);
                inv[ci].param1 = 166;
            }
            txt(lang(
                u8"何かが足元に転がってきた。"s,
                u8"Something is put on the ground."s));
        }
        if (gdata_god_rank == 5)
        {
            flt();
            dbid = 0;
            if (cdata[0].god_id == core_god::mani)
            {
                dbid = 674;
            }
            if (cdata[0].god_id == core_god::lulwy)
            {
                dbid = 673;
            }
            if (cdata[0].god_id == core_god::itzpalt)
            {
                dbid = 676;
            }
            if (cdata[0].god_id == core_god::ehekatl)
            {
                dbid = 678;
            }
            if (cdata[0].god_id == core_god::opatos)
            {
                dbid = 679;
            }
            if (cdata[0].god_id == core_god::jure)
            {
                dbid = 677;
            }
            if (cdata[0].god_id == core_god::kumiromi)
            {
                dbid = 675;
            }
            if (itemmemory(1, dbid))
            {
                dbid = 621;
            }
            itemcreate(-1, dbid, cdata[0].position.x, cdata[0].position.y, 0);
            txt(lang(
                u8"何かが足元に転がってきた。"s,
                u8"Something is put on the ground."s));
        }
        ++gdata_god_rank;
    }
    return turn_result_t::turn_end;
}



turn_result_t do_offer()
{
    if (cdata[0].god_id.empty())
    {
        txt(lang(
            u8"あなたは神を信仰していないが、試しに捧げてみた。"s,
            u8"You don't believe in God."s));
        return turn_result_t::turn_end;
    }
    rowact_item(ci);
    item_separate(ci);
    txt(lang(
        u8"あなたは"s + itemname(ci) + u8"を"s
            + i18n::_(u8"god", cdata[0].god_id, u8"name")
            + u8"に捧げ、その名を唱えた。"s,
        u8"You put "s + itemname(ci)
            + u8" on the altar and mutter the name of "s
            + i18n::_(u8"god", cdata[0].god_id, u8"name") + u8"."s));
    snd(121);
    play_animation(7);
    int stat = item_find(60002);
    if (stat != -1)
    {
        ti = stat;
    }
    else
    {
        return turn_result_t::turn_end;
    }
    if (inv[ci].id == 204)
    {
        i = clamp(inv[ci].weight / 200, 1, 50);
        if (inv[ci].param3 < 0)
        {
            i = 1;
        }
    }
    else
    {
        i = 25;
    }
    if (core_god::int2godid(inv[ti].param1) != cdata[0].god_id)
    {
        f = 0;
        if (inv[ti].param1 == 0)
        {
            f = 1;
            txt(lang(
                u8"異世界で、"s + i18n::_(u8"god", cdata[0].god_id, u8"name")
                    + u8"が空白の祭壇の権利を主張した。"s,
                i18n::_(u8"god", cdata[0].god_id, u8"name")
                    + u8" claims the empty altar."s));
        }
        else
        {
            txt(lang(
                u8"異様な霧が現れ、"s
                    + i18n::_(u8"god", cdata[0].god_id, u8"name") + u8"と"s
                    + i18n::_(
                          u8"god",
                          core_god::int2godid(inv[ti].param1),
                          u8"name")
                    + u8"の幻影がせめぎあった。"s,
                u8"Strange fogs surround all over the place. You see shadows of "s
                    + i18n::_(u8"god", cdata[0].god_id, u8"name") + u8" and "s
                    + i18n::_(
                          u8"god",
                          core_god::int2godid(inv[ti].param1),
                          u8"name")
                    + u8" make a fierce dance."s));
            if (rnd(17) <= i)
            {
                f = 1;
            }
            else
            {
                f = 0;
            }
        }
        if (f == 1)
        {
            modpiety(i * 5);
            cdata[0].praying_point += i * 30;
            animode = 100;
            play_animation(19);
            snd(120);
            if (inv[ti].param1 != 0)
            {
                txt(lang(
                    u8"あなたの神の幻影は、次第に色濃くなった。"s,
                    u8"The shadow of your god slowly gets bolder."s));
            }
            txtef(5);
            txt(lang(
                i18n::_(u8"god", cdata[0].god_id, u8"name") + u8"は"s
                    + itemname(ti) + u8"を支配した。"s,
                i18n::_(u8"god", cdata[0].god_id, u8"name")
                    + u8" takes over the altar."s));
            txtgod(cdata[0].god_id, 2);
            inv[ti].param1 = core_god::godid2int(cdata[0].god_id);
        }
        else
        {
            txt(lang(
                i18n::_(u8"god", core_god::int2godid(inv[ti].param1), u8"name")
                    + u8"は祭壇を守りきった。"s,
                i18n::_(u8"god", core_god::int2godid(inv[ti].param1), u8"name")
                    + u8" keeps the altar."s));
            txtgod(core_god::int2godid(inv[ti].param1), 3);
            god_defends_altar();
        }
    }
    else
    {
        txtef(2);
        for (int cnt = 0; cnt < 1; ++cnt)
        {
            if (i >= 15)
            {
                txt(lang(
                    itemname(ci) + u8"はまばゆく輝いて消えた。"s,
                    itemname(ci) + u8" shine"s + _s2(inv[ci].number)
                        + u8" all around and disappear"s + _s2(inv[ci].number)
                        + u8"."s));
                txtgod(cdata[0].god_id, 4);
                break;
            }
            if (i >= 10)
            {
                txt(lang(
                    itemname(ci)
                        + u8"は輝いて消え、三つ葉のクローバーがふってきた。"s,
                    itemname(ci) + u8" shine"s + _s2(inv[ci].number)
                        + u8" for a moment and disappear"s + _s2(inv[ci].number)
                        + u8". A three-leaved falls from the altar."s));
                break;
            }
            if (i >= 5)
            {
                txt(lang(
                    itemname(ci) + u8"は一瞬輝いて消えた。"s,
                    itemname(ci) + u8" shine"s + _s2(inv[ci].number)
                        + u8" for a moment and disappear"s + _s2(inv[ci].number)
                        + u8"."s));
                break;
            }
            if (i >= 1)
            {
                txt(lang(
                    itemname(ci) + u8"は消えた。"s,
                    itemname(ci) + u8" disappear"s + _s2(inv[ci].number)
                        + u8"."s));
                break;
            }
        }
        modpiety(i);
        cdata[0].praying_point += i * 7;
    }
    removeitem(ci, inv[ci].number);
    return turn_result_t::turn_end;
}


int target_position()
{
    if (tlocinitx != 0 || tlocinity != 0)
    {
        tlocx = tlocinitx;
        tlocy = tlocinity;
    }
    else
    {
        tlocx = cdata[cc].position.x;
        tlocy = cdata[cc].position.y;
    }
    if (homemapmode)
    {
        scposval = 0;
    }
    else
    {
        scposval = 1;
        if (cdata[0].enemy_id == 0)
        {
            label_2072();
        }
        label_2076();
        if (listmax == 0)
        {
            txt(lang(
                u8"視界内にターゲットは存在しない。"s,
                u8"You look around and find nothing."s));
        }
        for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
        {
            if (list(0, cnt) == 0)
            {
                continue;
            }
            if (list(0, cnt) == cdata[0].enemy_id)
            {
                tlocx = cdata[list(0, cnt)].position.x;
                tlocy = cdata[list(0, cnt)].position.y;
                break;
            }
        }
    }
label_1948_internal:
    screenupdate = -1;
    update_screen();
    dx = (tlocx - scx) * inf_tiles + inf_screenx;
    dy = (tlocy - scy) * inf_tiles + inf_screeny;
    if (dy + inf_tiles <= windowh - inf_verh)
    {
        pos(dx, dy * (dy > 0));
        snail::application::instance().get_renderer().set_blend_mode(
            snail::blend_mode_t::blend);
        snail::application::instance().get_renderer().set_draw_color(
            {127, 127, 255, 50});
        snail::application::instance().get_renderer().fill_rect(
            dx,
            dy * (dy > 0),
            inf_tiles - (dx + inf_tiles > windoww) * (dx + inf_tiles - windoww),
            inf_tiles + (dy < 0) * inf_screeny
                - (dy + inf_tiles > windowh - inf_verh)
                    * (dy + inf_tiles - windowh + inf_verh));
    }
    if (homemapmode == 1)
    {
        pos(windoww - 80, 20);
        gcopy(
            2,
            tile % 33 * inf_tiles,
            tile / 33 * inf_tiles,
            inf_tiles,
            inf_tiles);
    }
    else
    {
        rc = -1;
        for (int cnt = 0; cnt < 1; ++cnt)
        {
            if (map(tlocx, tlocy, 1) <= 1)
            {
                break;
            }
            rc = map(tlocx, tlocy, 1) - 1;
            if (fov_player_sees(rc) == 0)
            {
                break;
            }
            if (fov_los(
                    cdata[cc].position.x,
                    cdata[cc].position.y,
                    cdata[rc].position.x,
                    cdata[rc].position.y)
                == 0)
            {
                break;
            }
            if ((cdata[rc].is_invisible() == 0 || cdata[0].can_see_invisible()
                 || cdata[rc].wet)
                == 0)
            {
                break;
            }
            get_route(
                cdata[cc].position.x,
                cdata[cc].position.y,
                cdata[rc].position.x,
                cdata[rc].position.y);
            dx = (tlocx - scx) * inf_tiles + inf_screenx;
            dy = (tlocy - scy) * inf_tiles + inf_screeny;
            if (maxroute != 0)
            {
                dx = cdata[cc].position.x;
                dy = cdata[cc].position.y;
                for (int cnt = 0; cnt < 100; ++cnt)
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
                    sx = (dx - scx) * inf_tiles + inf_screenx;
                    sy = (dy - scy) * inf_tiles + inf_screeny;
                    if (sy + inf_tiles <= windowh - inf_verh)
                    {
                        pos(sx, sy * (sy > 0));
                        snail::application::instance()
                            .get_renderer()
                            .set_blend_mode(snail::blend_mode_t::blend);
                        snail::application::instance()
                            .get_renderer()
                            .set_draw_color({255, 255, 255, 25});
                        snail::application::instance().get_renderer().fill_rect(
                            sx,
                            sy * (sy > 0),
                            inf_tiles
                                - (sx + inf_tiles > windoww)
                                    * (sx + inf_tiles - windoww),
                            inf_tiles + (sy < 0) * inf_screeny
                                - (sy + inf_tiles > windowh - inf_verh)
                                    * (sy + inf_tiles - windowh + inf_verh));
                    }
                }
            }
        }
    }
    txttargetnpc(tlocx, tlocy);
    redraw();
    await(config::instance().wait1);
    key_check();
    if (homemapmode == 1)
    {
        if (key == key_enter)
        {
            label_1955();
            keyrelease();
            goto label_1948_internal;
        }
        int a{};
        stick(a, 768);
        if (a == 256)
        {
            key = key_enter;
        }
        if (a == 512)
        {
            if (chipm(0, map(tlocx, tlocy, 0)) == 2
                || chipm(0, map(tlocx, tlocy, 0)) == 1)
            {
                snd(27);
                keyrelease();
                goto label_1948_internal;
            }
            tile = map(tlocx, tlocy, 0);
            snd(5);
            keyrelease();
        }
        tx = clamp((mousex - inf_screenx), 0, windoww) / 48;
        ty = clamp((mousey - inf_screeny), 0, (windowh - inf_verh)) / 48;
        int stat = key_direction();
        if (stat == 1)
        {
            cdata[0].position.x += kdx;
            cdata[0].position.y += kdy;
            if (cdata[0].position.x < 0)
            {
                cdata[0].position.x = 0;
            }
            else if (cdata[0].position.x >= mdata(0))
            {
                cdata[0].position.x = mdata(0) - 1;
            }
            if (cdata[0].position.y < 0)
            {
                cdata[0].position.y = 0;
            }
            else if (cdata[0].position.y >= mdata(1))
            {
                cdata[0].position.y = mdata(1) - 1;
            }
        }
        tlocx = tx + scx;
        if (tlocx < 0)
        {
            tlocx = 0;
        }
        else if (tlocx >= mdata(0))
        {
            tlocx = mdata(0) - 1;
        }
        tlocy = ty + scy;
        if (tlocy < 0)
        {
            tlocy = 0;
        }
        else if (tlocy >= mdata(1))
        {
            tlocy = mdata(1) - 1;
        }
    }
    else
    {
        int stat = key_direction();
        if (stat == 1)
        {
            x = tlocx + kdx;
            y = tlocy + kdy;
            if (x >= 0 && y >= 0 && x < mdata(0) && y < mdata(1))
            {
                tlocx += kdx;
                tlocy += kdy;
            }
        }
    }
    if (findlocmode == 1)
    {
        if (rc == -1)
        {
            i = 0;
        }
        else
        {
            i = rc;
        }
        f = 0;
        p = 0;
        for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
        {
            if (cdata[list(0, cnt)].position.x == cdata[i].position.x)
            {
                if (cdata[list(0, cnt)].position.y == cdata[i].position.y)
                {
                    p = cnt;
                    break;
                }
            }
        }
        if (key == key_pageup)
        {
            ++p;
            f = 1;
            if (p >= listmax)
            {
                p = 0;
            }
        }
        if (key == key_pagedown)
        {
            --p;
            f = 1;
            if (p < 0)
            {
                p = listmax - 1;
                if (p < 0)
                {
                    p = 0;
                }
            }
        }
        i = list(0, p);
        if (f)
        {
            snd(5);
            tlocx = cdata[list(0, p)].position.x;
            tlocy = cdata[list(0, p)].position.y;
        }
    }
    if (key == key_enter)
    {
        if (findlocmode == 1)
        {
            if (cansee == 0 || chipm(7, map(tlocx, tlocy, 0)) & 4)
            {
                txt(lang(
                    u8"その場所は見えない。"s,
                    u8"You can't see the location."s));
                goto label_1948_internal;
            }
            snd(20);
            if (rc > 0)
            {
                cdata[0].enemy_id = rc;
                txt(lang(
                    name(rc) + u8"をターゲットにした。"s,
                    u8"You target "s + name(rc) + u8"."s));
            }
            else
            {
                tgloc = 1;
                tglocx = tlocx;
                tglocy = tlocy;
                txt(lang(
                    u8"地面をターゲットにした。"s,
                    u8"You target the ground."s));
            }
        }
        else if (homemapmode == 0)
        {
            snd(5);
        }
        scposval = 0;
        if (tlocinitx == 0 && tlocinity == 0)
        {
            scposval = 0;
            update_screen();
        }
        tlocinitx = 0;
        tlocinity = 0;
        return cansee;
    }
    if (key == key_cancel)
    {
        tlocinitx = 0;
        tlocinity = 0;
        scposval = 0;
        update_screen();
        return -1;
    }
    goto label_1948_internal;
}


turn_result_t step_into_gate()
{
    if (config::instance().extrahelp)
    {
        if (gdata(217) == 0)
        {
            if (mode == 0)
            {
                if (cdata[0].continuous_action_turn == 0)
                {
                    gdata(217) = 1;
                    ghelp = 17;
                    show_ex_help();
                    screenupdate = -1;
                    update_screen();
                    return turn_result_t::pc_turn_user_error;
                }
            }
        }
    }
    if (1 && gdata_wizard == 0)
    {
        snd(44);
        do_save_game();
        txtef(5);
        txt(lang(u8" *保存* "s, u8"*saving*"s));
    }
    txt(lang(
        u8"あなたはゲートに足を踏み入れた。ゲートはあなたの背後で消滅した。"s,
        u8"You stepped into the gate. The gate disappears."s));
    --inv[ci].number;
    cell_refresh(inv[ci].position.x, inv[ci].position.y);
    txt(lang(
        u8"ネット機能をONにする必要がある。"s,
        u8"You have to turn on network setting."s));
    update_screen();
    return turn_result_t::pc_turn_user_error;
}



turn_result_t do_gatcha()
{
    int tmat = 0;
    screenupdate = -1;
    update_screen();
    if (inv[ci].id == 413)
    {
        tmat = 40;
    }
    if (inv[ci].id == 414)
    {
        tmat = 41;
    }
    txt(lang(
        ""s + matname(tmat) + u8"を使ってガシャガシャする？"s,
        u8"Pay "s + matname(tmat) + u8" to gasha-gasha?"s));
    ELONA_YES_NO_PROMPT();
    rtval = show_prompt(promptx, prompty, 160);
    if (rtval == 0)
    {
        if (mat(tmat) > 0)
        {
            snd(56);
            matdelmain(tmat);
            if (inv[ci].id == 413)
            {
                p = 415;
            }
            else
            {
                p = 416;
            }
            int stat = itemcreate(
                -1, p, cdata[cc].position.x, cdata[cc].position.y, 0);
            if (stat != 0)
            {
                inv[ci].param2 = 0;
            }
            txt(lang(
                u8"何かが足元に転がってきた。"s,
                u8"Something is put on the ground."s));
        }
        else
        {
            txt(lang(
                ""s + matname(tmat) + u8"を持っていない…"s,
                u8"You don't have "s + matname(tmat) + u8"."s));
        }
    }
    return turn_result_t::turn_end;
}


int label_2083()
{
    if (inv[ci].id == 563)
    {
        if (sdata.get(inv[ci].param1, 0).original_level == 0)
        {
            txt(lang(
                u8"この本の内容には興味がない。それでも読む？ "s,
                u8"You are not interested in this book. Do you want to read it anyway? "s));
            ELONA_YES_NO_PROMPT();
            rtval = show_prompt(promptx, prompty, 160);
            if (rtval != 0)
            {
                return 0;
            }
        }
    }
    gdata(91) = 104;
    continuous_action_others();
    return 1;
}

int label_2167()
{
    int spellbk = 0;
    spellbk = efid;
    ccbk = cc;
    int stat = do_cast_spell();
    if (stat == 1)
    {
        cc = ccbk;
        r2 = spellbk;
        gain_skill_experience_casting(cc);
        return 1;
    }
    return 0;
}


int do_cast_spell()
{
    int mp = 0;
    efsource = 3;
    efstatus = curse_state_t::none;
    efp = calcspellpower(efid, cc);
    if (cc == 0)
    {
        if (calcspellcostmp(efid, cc) > cdata[cc].mp)
        {
            txt(lang(
                u8"マナが足りないが、それでも詠唱を試みる？"s,
                u8"You are going to over-cast the spell. Are you sure?"s));
            ELONA_YES_NO_PROMPT();
            rtval = show_prompt(promptx, prompty, 160);
            if (rtval != 0)
            {
                update_screen();
                efsource = 0;
                return 0;
            }
        }
        screenupdate = -1;
        update_screen();
    }
    int stat = query_magic_direction();
    if (stat == 0)
    {
        efsource = 0;
        return 0;
    }
    if (cc != 0)
    {
        if (the_ability_db[efid]->sdataref1 == 7)
        {
            if (cdata[cc].relationship == 10 || gdata_current_map == 40)
            {
                efsource = 0;
                return 0;
            }
            if (gdata_play_turns % 10 > 4)
            {
                efsource = 0;
                return 0;
            }
        }
    }
    if (cc == 0 || (cc != 0 && npccostmp == 1))
    {
        if (cc == 0)
        {
            spell(efid - 400) -= calcspellcoststock(efid, cc);
            if (spell(efid - 400) < 0)
            {
                spell(efid - 400) = 0;
            }
        }
        mp = calcspellcostmp(efid, cc);
        if (cc == 0)
        {
            if (cdata[0].god_id == core_god::ehekatl)
            {
                mp = rnd(mp * 140 / 100 + 1) + 1;
            }
        }
        dmgmp(cc, mp);
        if (cdata[cc].state != 1)
        {
            efsource = 0;
            return 1;
        }
    }
    if (cdata[cc].confused != 0 || cdata[cc].dimmed != 0)
    {
        if (fov_player_sees(cc))
        {
            txt(lang(
                name(cc) + u8"は混乱しながらも魔法の詠唱を試みた。"s,
                name(cc) + u8" try"s + _s(cc)
                    + u8" to cast a spell in confusion."s));
        }
        int stat = try_to_cast_spell();
        if (stat == 0)
        {
            efsource = 0;
            return 1;
        }
    }
    else if (fov_player_sees(cc))
    {
        if (cc == 0)
        {
            txt(lang(
                name(cc) + u8"は"s
                    + i18n::_(u8"ability", std::to_string(efid), u8"name")
                    + u8"の"s
                    + i18n::_(
                          u8"ui",
                          u8"cast_style",
                          u8"_"s + cdata[cc].special_attack_type),
                name(cc) + u8" cast "s
                    + i18n::_(u8"ability", std::to_string(efid), u8"name")
                    + u8"."s));
        }
        else
        {
            txt(lang(
                name(cc) + u8"は"s
                    + i18n::_(
                          u8"ui",
                          u8"cast_style",
                          u8"_"s + cdata[cc].special_attack_type),
                name(cc) + ""s
                    + i18n::_(
                          u8"ui",
                          u8"cast_style",
                          u8"_"s + cdata[cc].special_attack_type)));
        }
    }
    if (findbuff(cc, 2) != -1)
    {
        if (fov_player_sees(cc))
        {
            txt(lang(
                u8"沈黙の霧が詠唱を阻止した。"s,
                u8"The mist of silence interrupts a spell."s));
        }
        efsource = 0;
        return 1;
    }
    if (rnd(100) >= calcspellfail(efid, cc))
    {
        if (fov_player_sees(cc))
        {
            txt(lang(
                name(cc) + u8"は詠唱に失敗した。"s,
                name(cc) + u8" fail"s + _s(cc) + u8" to cast a spell."s));
            play_animation(8);
        }
        efsource = 0;
        return 1;
    }
    if (noeffect == 1)
    {
        txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
        efsource = 0;
        return 1;
    }
    efp = calcspellpower(efid, cc);
    p = encfind(cc, 34);
    if (p != -1)
    {
        efp = efp * (100 + p / 10) / 100;
    }
    rapidmagic = 0;
    if (cdata[cc].can_cast_rapid_magic()
        && the_ability_db[efid]->sdataref1 == 2)
    {
        rapidmagic = 1 + (rnd(3) != 0) + (rnd(2) != 0);
    }
    if (rapidmagic)
    {
        for (int cnt = 0, cnt_end = (rapidmagic); cnt < cnt_end; ++cnt)
        {
            magic();
            if (cdata[tc].state != 1)
            {
                int stat = label_2072();
                if (stat == 0)
                {
                    break;
                }
                else
                {
                    tc = cdata[cc].enemy_id;
                    if (relationbetween(cc, tc) != -3)
                    {
                        break;
                    }
                }
            }
        }
        rapidmagic = 0;
    }
    else
    {
        magic();
    }
    return 1;
}



int pick_up_item()
{
    int sellgold = 0;
    if (cc != -1)
    {
        if (inv[ci].id == 54 || inv[ci].id == 55)
        {
            snd(11);
            ti = ci;
            in = inv[ci].number;
            inv[ci].number = 0;
            msgkeep = 1;
            txt(lang(
                name(cc) + u8"は"s + itemname(ti, in) + u8"を拾った。"s,
                name(cc) + u8" pick"s + _s(cc) + u8" up "s + itemname(ti, in)
                    + u8"."s));
            cell_refresh(inv[ci].position.x, inv[ci].position.y);
            if (inv[ci].id == 54)
            {
                cdata[cc].gold += in;
            }
            else
            {
                cdata[cc].platinum_coin += in;
            }
            return 1;
        }
    }
    if (cc == 0)
    {
        if (gdata_mount != 0)
        {
            if (cdata[gdata_mount].continuous_action_id != 0)
            {
                if (cdata[gdata_mount].continuous_action_item == ci)
                {
                    txt(lang(
                        u8"それは"s + name(gdata_mount) + u8"が使用中だ。"s,
                        name(gdata_mount) + u8" "s + is(gdata_mount)
                            + u8" using it."s));
                    return 1;
                }
            }
        }
        if (the_item_db[inv[ci].id]->category == 57000)
        {
            if (inv[ci].own_state == 4)
            {
                if (cdata[0].continuous_action_id == 0)
                {
                    if (!inv_getspace(0))
                    {
                        txt(lang(
                            u8"バックパックが一杯だ。"s,
                            u8"Your inventory is full."s));
                        return 0;
                    }
                    gdata(91) = 103;
                    continuous_action_others();
                    return -1;
                }
            }
        }
        if (inv[ci].own_state == 3)
        {
            txt(lang(
                itemname(ci) + u8"を撤去する？ "s,
                u8"Do you want to remove "s + itemname(ci) + u8"? "s));
            ELONA_YES_NO_PROMPT();
            rtval = show_prompt(promptx, prompty, 160);
            if (rtval == 0)
            {
                snd(58);
                if (inv[ci].id == 555)
                {
                    std::string midbk = mid;
                    mid = ""s + 30 + u8"_"s + (100 + inv[ci].count);
                    if (fs::exists(
                            filesystem::dir::tmp()
                            / (u8"mdata_"s + mid + u8".s2")))
                    {
                        ctrl_file(file_operation_t::_11);
                    }
                    mid = midbk;
                    inv[ci].count = 0;
                    inv[ci].own_state = 0;
                }
            }
            else
            {
                return 0;
            }
        }
        if (inv_getfreeid(cc) == -1)
        {
            txt(lang(
                u8"バックパックには、もうアイテムを入れる余裕がない。"s,
                u8"Your inventory is full."s));
            return 0;
        }
    }
    inumbk = inv[ci].number - in;
    inv[ci].number = in;
    if (cc == 0)
    {
        if (trait(215) != 0)
        {
            if (the_item_db[inv[ci].id]->category == 56000)
            {
                if (inv[ci].count > 0)
                {
                    dbid = inv[ci].id;
                    access_item_db(14);
                    txt(lang(
                        u8"あなたは"s + itemname(ci)
                            + u8"から魔力を吸い取った。"s,
                        u8"You absorb magic from "s + itemname(ci) + u8"."s));
                    if (efid >= 400 && efid < 467)
                    {
                        spell(efid - 400) += inv[ci].count * 5 * inv[ci].number;
                    }
                    else
                    {
                        healmp(0, inv[ci].count * 5 * inv[ci].number);
                    }
                    inv[ci].count = 0;
                }
            }
        }
        if (trait(216) != 0)
        {
            if (the_item_db[inv[ci].id]->category == 52000)
            {
                if (inv[ci].id != 262 && inv[ci].id != 559)
                {
                    if (rnd(5) == 0)
                    {
                        txt(lang(
                            u8"あなたの手から毒が滴った。"s,
                            u8"Poison drips from your hands."s));
                        inv[ci].id = 262;
                    }
                }
            }
        }
    }
    if (inv[ci].own_state < 0)
    {
        inv[ci].own_state = 0;
    }
    ibitmod(12, ci, 0);
    int stat = item_stack(cc, ci);
    if (stat == 0)
    {
        ti = inv_getfreeid(cc);
        if (ti == -1)
        {
            inv[ci].number = inumbk + in;
            if (invctrl == 12)
            {
                txt(lang(
                    u8"店の倉庫が一杯のため売れない。"s,
                    u8"Shopkeeper's inventory is full."s));
            }
            else
            {
                txt(lang(
                    u8"バックパックには、もうアイテムを入れる余裕がない。"s,
                    u8"Your inventory is full."s));
            }
            return 0;
        }
        item_copy(ci, ti);
        inv[ti].number = in;
    }
    inv[ci].number = inumbk;
    if (mode == 6)
    {
        if (the_item_db[inv[ti].id]->category == 57000)
        {
            if (invctrl == 11 || invctrl == 22)
            {
                if (invctrl == 22 && invctrl(1) == 3)
                {
                    if (inv[ti].param3 > 0)
                    {
                        inv[ti].param3 += gdata_hour + gdata_day * 24
                            + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12;
                    }
                }
                else if (inv[ti].param3 != 0)
                {
                    inv[ti].param3 = gdata_hour + gdata_day * 24
                        + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12
                        + the_item_db[inv[ti].id]->expiration_date;
                    if (inv[ti].param2 != 0)
                    {
                        inv[ti].param3 += 72;
                    }
                }
            }
            if (invctrl == 24 && invctrl(1) == 3)
            {
                if (inv[ti].param3 > 0)
                {
                    inv[ti].param3 = inv[ti].param3
                        - (gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
                           + gdata_year * 24 * 30 * 12);
                }
            }
        }
        if (invctrl == 11)
        {
            msgkeep = 1;
            txt(lang(
                itemname(ti, in) + u8"を買った。"s,
                u8"You buy "s + itemname(ti, in) + u8"."s));
            sellgold = calcitemvalue(ti, 0) * in;
            snd(12);
            cdata[0].gold -= sellgold;
            cdata[tc].gold += sellgold;
            if (the_item_db[inv[ti].id]->category == 92000)
            {
                inv[ti].param2 = calcitemvalue(ti, 0);
            }
        }
        if (invctrl == 12)
        {
            msgkeep = 1;
            sellgold = calcitemvalue(ci, 1) * in;
            if (ibit(9, ti) == 0)
            {
                txt(lang(
                    itemname(ti, in) + u8"を売った。"s,
                    u8"You sell "s + itemname(ti, in) + u8"."s));
            }
            else
            {
                ibitmod(9, ti, 0);
                txt(lang(
                    u8"盗品の"s + itemname(ti, in) + u8"を売却した。"s,
                    u8"You sell "s + itemname(ti, in)
                        + u8".(Stolen item sold) "s));
                if (gdata_thieves_guild_quota > 0)
                {
                    gdata_thieves_guild_quota -= sellgold;
                    if (gdata_thieves_guild_quota < 0)
                    {
                        gdata_thieves_guild_quota = 0;
                    }
                    txt(lang(
                        u8"盗賊ギルドのノルマ達成まで、あと金貨"s
                            + gdata_thieves_guild_quota
                            + u8"枚相当の盗品を売却する必要がある。"s,
                        u8"You still need to sell stuff worth "s
                            + gdata_thieves_guild_quota
                            + u8" gold pieces in order to advance ranks in the Thieves Guild."s));
                }
            }
            snd(11);
            cdata[0].gold += sellgold;
            cdata[tc].gold -= sellgold;
            if (cdata[tc].gold < 0)
            {
                cdata[tc].gold = 0;
            }
            inv[ti].identification_state =
                identification_state_t::completely_identified;
        }
        if (invctrl == 22 || invctrl == 24)
        {
            snd(14 + rnd(2));
            msgkeep = 1;
            if (invctrl == 22)
            {
                txt(lang(
                    name(cc) + u8"は"s + itemname(ti, in) + u8"を拾った。"s,
                    name(cc) + u8" pick"s + _s(cc) + u8" up "s
                        + itemname(ti, in) + u8"."s));
            }
            else
            {
                txt(lang(
                    itemname(ti, in) + u8"を保管した。"s,
                    u8"You put "s + itemname(ti, in)
                        + u8" in the container."s));
            }
        }
        else
        {
            r2 = sellgold;
            label_1459(0);
        }
    }
    else
    {
        cell_refresh(inv[ci].position.x, inv[ci].position.y);
        map(inv[ci].position.x, inv[ci].position.y, 5) =
            map(inv[ci].position.x, inv[ci].position.y, 4);
        snd(14 + rnd(2));
        msgkeep = 1;
        txt(lang(
            name(cc) + u8"は"s + itemname(ti, in) + u8"を拾った。"s,
            name(cc) + u8" pick"s + _s(cc) + u8" up "s + itemname(ti, in)
                + u8"."s));
    }
    if (cc == 0)
    {
        if (inv[ti].id == 255)
        {
            if (mdata(20) == 1)
            {
                f = 0;
                for (const auto& cnt : items(-1))
                {
                    if (inv[cnt].number == 0)
                    {
                        continue;
                    }
                    if (inv[cnt].id == 255)
                    {
                        f = 1;
                        break;
                    }
                }
                if (f == 0)
                {
                    mdata(20) = 0;
                    play_music();
                }
            }
        }
        int stat = convertartifact(ti);
        ti = stat;
        if (adata(16, gdata_current_map) == 101)
        {
            if (mode == 0)
            {
                update_museum();
            }
        }
        if (gdata_current_map == 7)
        {
            if (mode == 0)
            {
                label_1730();
            }
        }
        refresh_burden_state();
    }
    if (cc == -1)
    {
        refresh_burden_state();
    }
    return 1;
}

int drop_item()
{
    inumbk = inv[ci].number - in;
    inv[ci].number = in;
    inv[ci].position.x = cdata[cc].position.x;
    inv[ci].position.y = cdata[cc].position.y;
    itemturn(ci);
    int stat = item_stack(-1, ci);
    if (stat == 0 || dropval == 0)
    {
        ti = inv_getfreeid(-1);
        if (ti == -1)
        {
            txt(lang(
                u8"もう周りに物を置くスペースがない！ "s,
                u8"The place is too crowded. You can't drop stuff anymore."s));
            update_screen();
            return 0;
        }
        item_copy(ci, ti);
    }
    if (dropval == 1)
    {
        inv[ti].own_state = 3;
        inv[ti].count = gdata_next_shelter_serial_id + 100;
        ++gdata_next_shelter_serial_id;
    }
    inv[ci].number = inumbk;
    cell_refresh(inv[ti].position.x, inv[ti].position.y);
    if (dropval == 0)
    {
        screenupdate = -1;
        update_screen();
        snd(16);
        txt(lang(
            itemname(ti, in) + u8"を地面に置いた。"s,
            u8"You drop "s + itemname(ti, in) + u8"."s));
    }
    refresh_burden_state();
    if (inv[ti].id == 516)
    {
        int stat = item_find(60002, 0);
        if (stat != -1)
        {
            p = stat;
            if (core_god::int2godid(inv[p].param1) == cdata[cc].god_id)
            {
                if (inv[ti].curse_state != curse_state_t::blessed)
                {
                    snd(64);
                    inv[ti].curse_state = curse_state_t::blessed;
                    txtef(2);
                    txt(lang(
                        u8"水は祝福を受けた。"s, u8"The water is blessed."s));
                }
            }
        }
    }
    if (adata(16, gdata_current_map) == 101)
    {
        if (mode == 0)
        {
            update_museum();
        }
    }
    if (gdata_current_map == 7)
    {
        if (mode == 0)
        {
            label_1730();
        }
    }
    if (inv[ti].id == 255)
    {
        mdata(20) = 1;
        play_music();
    }
    return 1;
}



turn_result_t do_bash()
{
    if (map(x, y, 5) != 0)
    {
        if (mapitemfind(x, y, 526) != -1)
        {
            ci = mapitemfind(x, y, 526);
            item_separate(ci);
            snd(73);
            txt(lang(
                itemname(ci) + u8"に体当たりした。"s,
                u8"You throw your weight against "s + itemname(ci) + u8"."s));
            if (inv[ci].own_state == 5 || inv[ci].param1 <= 0)
            {
                txt(lang(
                    u8"もう実はないようだ… "s,
                    u8"It seems there are no fruits left on the tree."s));
                return turn_result_t::turn_end;
            }
            --inv[ci].param1;
            if (inv[ci].param1 <= 0)
            {
                inv[ci].image = 592;
                cell_refresh(x, y);
            }
            if (y + 1 < mdata(1))
            {
                if ((chipm(7, map(x, y + 1, 0)) & 4) == 0)
                {
                    ++y;
                }
            }
            flt();
            itemcreate(-1, inv[ci].param2, x, y, 0);
            txt(lang(
                itemname(ci, 1) + u8"が降ってきた。"s,
                itemname(ci, 1) + u8" falls down from the tree."s));
            item_stack(-1, ci);
            return turn_result_t::turn_end;
        }
    }
    if (map(x, y, 1) != 0)
    {
        tc = map(x, y, 1) - 1;
        if (cdata[tc].sleep == 0)
        {
            if (cc == 0)
            {
                if (cdata[tc].relationship >= 0)
                {
                    int stat = prompt_really_attack();
                    if (stat == 0)
                    {
                        return turn_result_t::pc_turn_user_error;
                    }
                }
            }
            if (cdata[tc].choked)
            {
                snd(73);
                txt(lang(
                    name(cc) + u8"は"s + name(tc) + u8"に全力で体当たりした。"s,
                    name(cc) + u8" bash"s + _s(cc) + u8" up "s + name(tc)
                        + u8" at full power."s));
                dmghp(tc, sdata(10, cc) * 5, cc);
                if (cdata[tc].state == 1)
                {
                    txt(lang(
                        name(tc) + u8"はもちを吐き出した。"s,
                        name(tc) + u8" spit"s + _s(tc) + u8" mochi."s));
                    txt(lang(u8"「助かったよ！」"s, u8"\"You saved me!\""s));
                    cdata[tc].choked = 0;
                    chara_mod_impression(tc, 10);
                }
            }
            else
            {
                snd(73);
                txt(lang(
                    name(cc) + u8"は"s + name(tc) + u8"に体当たりした。"s,
                    name(cc) + u8" bash"s + _s(cc) + u8" up "s + name(tc)
                        + u8"."s));
                hostileaction(cc, tc);
            }
        }
        else
        {
            snd(73);
            txt(lang(
                name(cc) + u8"は"s + name(tc) + u8"に体当たりした。"s,
                name(cc) + u8" bash"s + _s(cc) + u8" up "s + name(tc)
                    + u8"."s));
            txt(lang(
                name(tc) + u8"は睡眠を妨害された。"s,
                name(cc) + u8" disturb"s + _s(cc) + u8" "s + his(tc)
                    + u8" sleep."s));
            modify_karma(cc, -1);
            cdata[tc].emotion_icon = 418;
        }
        cdata[tc].sleep = 0;
        return turn_result_t::turn_end;
    }
    if (map(x, y, 6) != 0)
    {
        cell_featread(x, y);
        if (feat(1) == 30)
        {
            map(x, y, 6) = 0;
            spillfrag(x, y, 2);
            flt(calcobjlv(
                    gdata_current_dungeon_level * (gdata_current_map != 30)),
                calcfixlv(2));
            flttypemajor = fsetbarrel(rnd(length(fsetbarrel)));
            itemcreate(-1, 0, x, y, 0);
            if (fov_player_sees(cc))
            {
                snd(73);
                txt(lang(
                    name(cc) + u8"は壷を割った。"s,
                    name(cc) + u8" shatter"s + _s(cc) + u8" the pot."s));
                snd(45);
                play_animation(14);
            }
            return turn_result_t::turn_end;
        }
        if (feat(1) == 21)
        {
            snd(73);
            p = feat(2) * 3 + 30;
            if (gdata_current_map == 41)
            {
                p *= 20;
            }
            if (rnd(p) < sdata(10, cc) && rnd(2))
            {
                txt(lang(
                    u8"扉に体当たりして破壊した。"s,
                    u8"You bash up the door. The door is destroyed."s));
                if (feat(2) > sdata(10, cc))
                {
                    skillexp(10, cc, (feat(2) - sdata(10, cc)) * 15);
                }
                cell_featset(x, y, 0, 0, 0, 0);
                return turn_result_t::turn_end;
            }
            else
            {
                txt(lang(u8"扉に体当たりした。"s, u8"You bash up the door."s));
                if (gdata_current_map == 41)
                {
                    txt(lang(
                        u8"さすがに牢獄の扉は頑丈だ。"s,
                        u8"As might be expected, the door of the Jail is hard."s));
                }
                if (rnd(4) == 0)
                {
                    tc = cc;
                    efid = 1109;
                    efp = 200;
                    magic();
                }
                if (rnd(3) == 0)
                {
                    tc = cc;
                    efid = 1110;
                    efp = 200;
                    magic();
                }
                if (rnd(3) == 0)
                {
                    if (cdata[cc].quality < 4 && encfind(cc, 60010) == -1)
                    {
                        --cdata[cc].attr_adjs[0];
                        chara_refresh(cc);
                        if (fov_player_sees(cc))
                        {
                            txtef(8);
                            txt(lang(
                                name(cc) + u8"は筋肉を痛めた。"s,
                                name(cc) + u8" hurt"s + _s(cc) + u8" "s
                                    + his(cc) + u8" muscle."s));
                        }
                    }
                }
                if (rnd(3) == 0)
                {
                    if (feat(2) > 0)
                    {
                        --feat(2);
                        cell_featset(x, y, feat(0), feat(1), feat(2), feat(3));
                        if (fov_player_sees(cc))
                        {
                            txt(lang(
                                u8"扉は少しだけ壊れた。"s,
                                u8"The door is cracked a bit."s));
                        }
                    }
                }
                return turn_result_t::turn_end;
            }
        }
    }
    txt(lang(
        name(cc) + u8"は空気に体当たりした。"s,
        name(cc) + u8" bash"s + _s(cc) + u8" up the air."s));
    snd(4);
    return turn_result_t::turn_end;
}




int do_drink_potion()
{
    tc = cc;
    efsource = 4;
    if (potionspill || potionthrow)
    {
        if (potionthrow)
        {
            efp = efp * potionthrow / 100;
            efstatus = inv[ci].curse_state;
        }
    }
    else
    {
        efstatus = inv[ci].curse_state;
        if (fov_player_sees(tc))
        {
            snd(17);
            txt(lang(
                npcn(tc) + itemname(ci, 1) + u8"を飲み干した。"s,
                name(tc) + u8" drink"s + _s(tc) + u8" "s + itemname(ci, 1)
                    + u8"."s));
        }
    }
    magic();
    if (potionspill || potionthrow)
    {
        potionspill = 0;
        potionthrow = 0;
    }
    else
    {
        if (obvious == 1)
        {
            if (tc == 0)
            {
                item_identify(
                    inv[ci], identification_state_t::partly_identified);
            }
        }
        --inv[ci].number;
        if (ci >= 5080)
        {
            cell_refresh(inv[ci].position.x, inv[ci].position.y);
        }
        else if (tc == 0)
        {
            refresh_burden_state();
        }
    }
    cdata[tc].nutrition += 150;
    if (tc < 16)
    {
        if (cdata[tc].nutrition > 12000)
        {
            if (rnd(5) == 0)
            {
                chara_vomit(tc);
            }
        }
    }
    return 1;
}



int drink_well()
{
    if (inv[ci].param1 < -5 || inv[ci].param3 >= 20
        || (inv[ci].id == 602 && gdata_holy_well_count <= 0))
    {
        const auto valn = itemname(ci);
        txt(lang(valn + u8"は涸れている。"s, valn + u8" is dry."s));
        return 1;
    }
    snd(17);
    const auto valn = itemname(ci);
    txt(lang(
        name(cc) + u8"は"s + valn + u8"の水をすくって飲んだ。"s,
        name(cc) + u8" draw"s + _s(cc) + u8" water from "s + valn + u8"."s));
    tc = cc;
    int cibk = ci;
    p = rnd(100);
    for (int cnt = 0; cnt < 1; ++cnt)
    {
        if (cc != 0)
        {
            if (rnd(15) == 0)
            {
                txt(lang(
                    name(cc) + u8"は井戸に落ちた！ "s,
                    name(cc) + u8" falls in the well!"s));
                txtef(9);
                txt(name(cc)
                    + lang(
                          u8"「手を伸ばせー」"s,
                          (u8" yells, "s + u8"\"G-Give me your hands!\""s)));
                if (cdata[cc].is_floating() == 1 && cdata[cc].gravity == 0)
                {
                    txt(lang(
                        u8"しかしすぐに浮いてきた… "s,
                        u8"Soon "s + he(cc) + u8" floats up to the surface."s));
                }
                else
                {
                    dmghp(cc, 9999, -1);
                }
                break;
            }
        }
        if (inv[ci].id == 602)
        {
            if (rnd(2) == 0)
            {
                efid = 1113;
                magic();
                break;
            }
        }
        if (p > 70)
        {
            p = rnd(5);
            if (p == 0)
            {
                efid = 1112;
                efp = 100;
                magic();
                break;
            }
            if (p == 1)
            {
                efid = 1110;
                efp = 100;
                magic();
                break;
            }
            if (p == 2)
            {
                efid = 1111;
                efp = 100;
                magic();
                break;
            }
            if (p == 3)
            {
                efid = 1109;
                efp = 100;
                magic();
                break;
            }
            if (p == 4)
            {
                efid = 1108;
                efp = 100;
                magic();
                break;
            }
        }
        if (p > 55)
        {
            flt();
            itemcreate(-1, 54, cdata[cc].position.x, cdata[cc].position.y, 0);
            inv[ci].number = rnd(sdata(159, cc) / 2 * 50 + 100) + 1;
            txt(lang(
                name(cc) + u8"は水の中に金貨を見つけた。"s,
                name(cc) + u8" find"s + _s(cc)
                    + u8" some gold pieces in water."s));
            break;
        }
        if (p > 45)
        {
            p = rnd(8) + 10;
            if (rnd(5) > 2)
            {
                skillmod(p, cc, 1000);
            }
            else
            {
                skillmod(p, cc, -1000);
            }
            break;
        }
        if (p > 40)
        {
            if (cdata[cc].level < 5)
            {
                break;
            }
            if (fov_player_sees(cc))
            {
                txt(lang(
                    ""s + name(cc) + u8"は何かいけないものを飲み込んだ。"s,
                    name(cc) + u8" swallow"s + _s(cc) + u8" something bad."s));
            }
            tc = cc;
            get_pregnant();
            break;
        }
        if (p > 35)
        {
            txt(lang(
                u8"井戸から何かが出てきた！"s,
                u8"Something comes out from the well!"s));
            for (int cnt = 0, cnt_end = (1 + rnd(3)); cnt < cnt_end; ++cnt)
            {
                flt(calcobjlv(cdata[cc].level * 3 / 2 + 3), calcfixlv(2));
                chara_create(-1, 0, cdata[cc].position.x, cdata[cc].position.y);
            }
            break;
        }
        if (p > 33)
        {
            efid = 1113;
            magic();
            break;
        }
        if (p > 20)
        {
            efid = 454;
            magic();
            break;
        }
        if (p == 0)
        {
            if (rnd(gdata_wish_count + 1))
            {
                txtef(5);
                txt(lang(
                    u8"ものすごい幸運が訪れた…ような気がしたが気のせいだった。"s,
                    u8"You feel as a stroke of good fortune passed by."s));
                break;
            }
            ++gdata_wish_count;
            efid = 441;
            magic();
            break;
        }
        if (cc == 0)
        {
            txt(lang(
                u8"この水は清涼だ。"s, u8"Phew, fresh water tastes good."s));
        }
    }
    ci = cibk;
    if (cc != 0)
    {
        cdata[cc].nutrition += 4000;
    }
    else
    {
        cdata[cc].nutrition += 500;
    }
    if (inv[ci].id == 602)
    {
        --gdata_holy_well_count;
    }
    else
    {
        inv[ci].param1 -= rnd(3);
        inv[ci].param3 += rnd(3);
        if (inv[ci].param3 >= 20)
        {
            txt(lang(
                itemname(ci) + u8"は完全に干上がった。"s,
                itemname(ci) + u8" has completely dried up."s));
            return 1;
        }
    }
    if (inv[ci].param1 < -5)
    {
        txt(lang(
            itemname(ci) + u8"は干上がった。"s,
            itemname(ci) + u8" has dried up."s));
    }
    return 1;
}



int read_scroll()
{
    tc = cc;
    tlocx = cdata[cc].position.x;
    tlocy = cdata[cc].position.y;
    efstatus = inv[ci].curse_state;
    efsource = 2;
    if (cdata[cc].blind != 0)
    {
        if (fov_player_sees(cc))
        {
            txt(lang(
                name(cc) + u8"は何も見えない。"s,
                name(cc) + u8" can see nothing."s));
        }
        efsource = 0;
        return 0;
    }
    if (cdata[cc].dimmed != 0 || cdata[cc].confused != 0)
    {
        if (rnd(4) != 0)
        {
            if (fov_player_sees(cc))
            {
                txt(lang(
                    name(cc) + u8"はふらふらした。"s,
                    name(cc) + u8" stagger"s + _s(cc) + u8"."s));
            }
            efsource = 0;
            return 0;
        }
    }
    if (fov_player_sees(cc))
    {
        txt(lang(
            npcn(cc) + itemname(ci, 1) + u8"を読んだ。"s,
            name(cc) + u8" read"s + _s(cc) + u8" "s + itemname(ci, 1)
                + u8"."s));
    }
    if (inv[ci].id != 621)
    {
        --inv[ci].number;
        if (ci >= 5080)
        {
            cell_refresh(inv[ci].position.x, inv[ci].position.y);
        }
        else
        {
            refresh_burden_state();
        }
        skillexp(150, cc, 25, 2);
    }
    magic();
    if (cc == 0)
    {
        if (obvious == 1)
        {
            item_identify(inv[ci], identification_state_t::partly_identified);
        }
    }
    return 1;
}



int label_2172()
{
    if (inv[ci].count <= 0)
    {
        if (fov_player_sees(cc))
        {
            txt(lang(
                itemname(ci, 1) + u8"を振った。"s,
                u8"You zap "s + itemname(ci, 1) + u8"."s));
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
        }
        return 0;
    }
    efstatus = inv[ci].curse_state;
    if (efstatus == curse_state_t::blessed)
    {
        efstatus = curse_state_t::none;
    }
    efsource = 1;
    int stat = query_magic_direction();
    if (stat == 0)
    {
        efsource = 0;
        return 0;
    }
    if (efid >= 400 && efid < 467)
    {
        if ((stat == 0 && the_ability_db[efid]->sdataref3 / 1000 * 1000 == 2000)
            || noeffect == 1)
        {
            if (fov_player_sees(cc))
            {
                txt(lang(
                    itemname(ci, 1) + u8"を振った。"s,
                    u8"You zap "s + itemname(ci, 1) + u8"."s));
                txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            }
            goto label_2173_internal;
        }
    }
    if (fov_player_sees(cc))
    {
        txt(lang(
            itemname(ci, 1) + u8"を振った。"s,
            u8"You zap "s + itemname(ci, 1) + u8"."s));
    }
    efp = efp
        * (100 + sdata(174, cc) * 10 + sdata(16, cc) / 2 + sdata(13, cc) / 2)
        / 100;
    if (efid >= 400 && efid < 467)
    {
        f = 0;
        int skill = sdata(174, cc) * 20 + 100;
        if (inv[ci].curse_state == curse_state_t::blessed)
        {
            skill = skill * 125 / 100;
        }
        if (is_cursed(inv[ci].curse_state))
        {
            skill = skill * 50 / 100;
        }
        else if (rnd(2))
        {
            f = 1;
        }
        if (rnd((the_ability_db[efid]->sdataref4 + 1)) / 2 <= skill)
        {
            f = 1;
        }
    }
    else
    {
        f = 1;
    }
    if (rnd(30) == 0)
    {
        f = 0;
    }
    if (f == 1 || inv[ci].id == 290 || cc != 0)
    {
        magic();
        if (cc == 0)
        {
            if (obvious == 1)
            {
                item_identify(
                    inv[ci], identification_state_t::partly_identified);
            }
        }
        label_1469(cc);
    }
    else if (fov_player_sees(cc))
    {
        txt(lang(
            name(cc) + u8"は杖をうまく使えなかった。"s,
            name(cc) + u8" fail to use the power of the rod."s));
    }
label_2173_internal:
    efsource = 0;
    if (inv[ci].number == 0)
    {
        if (ci >= 5080)
        {
            cell_refresh(inv[ci].position.x, inv[ci].position.y);
            return 1;
        }
    }
    item_separate(ci);
    --inv[ci].count;
    return 1;
}


void do_perform()
{
    int performtips = 0;
    if (cdata[cc].continuous_action_id == 0)
    {
        if (fov_player_sees(cc)) // TODO move rest to continuous action
        {
            txt(lang(
                name(cc) + u8"は"s + itemname(ci) + u8"の演奏をはじめた。"s,
                name(cc) + u8" start"s + _s(cc) + u8" to play "s + itemname(ci)
                    + u8"."s));
        }
        cdata[cc].continuous_action_id = 6;
        cdata[cc].continuous_action_turn = 61;
        cdata[cc].continuous_action_item = ci;
        cdata[cc].quality_of_performance = 40;
        cdata[cc].tip_gold = 0;
        performtips = 0;
        return;
    }
    if (cdata[cc].continuous_action_turn > 0)
    {
        ci = cdata[cc].continuous_action_item;
        if (cdata[cc].continuous_action_turn % 10 == 0)
        {
            if (fov_player_sees(cc))
            {
                if (rnd(10) == 0)
                {
                    txtef(4);
                    txt(lang(u8"ﾁｬﾗﾝ♪ "s, u8"*Tiki*"s),
                        lang(u8"ﾎﾟﾛﾝ♪ "s, u8"*Dan*"s),
                        lang(u8"ﾀﾞｰﾝ♪ "s, u8"*Lala*"s));
                }
                txtef(4);
                txt(lang(u8"ｼﾞｬﾝ♪ "s, u8"*Cha*"s));
            }
        }
        if (cdata[cc].continuous_action_turn % 20 == 0)
        {
            gold = 0;
            make_sound(cdata[cc].position.x, cdata[cc].position.y, 5, 1, 1, cc);
            for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
            {
                if (cdata[cnt].state != 1)
                {
                    continue;
                }
                if (gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
                        + gdata_year * 24 * 30 * 12
                    >= cdata[cnt].time_interest_revive)
                {
                    cdata[cnt].interest = 100;
                }
                if (fov_player_sees(cc))
                {
                    if (cdata[cnt].vision_flag != msync)
                    {
                        continue;
                    }
                }
                else if (
                    dist(
                        cdata[cc].position.x,
                        cdata[cc].position.y,
                        cdata[cnt].position.x,
                        cdata[cnt].position.y)
                    > 3)
                {
                    continue;
                }
                if (cdata[cnt].interest <= 0)
                {
                    continue;
                }
                if (cdata[cnt].sleep > 0)
                {
                    continue;
                }
                x = cdata[cnt].position.x;
                y = cdata[cnt].position.y;
                if (map(x, y, 1) == 0)
                {
                    continue;
                }
                tc = cnt;
                if (tc == cc)
                {
                    continue;
                }
                if (cdata[tc].relationship == -3)
                {
                    if (cdata[tc].hate == 0)
                    {
                        if (fov_player_sees(tc))
                        {
                            txt(lang(
                                name(tc) + u8"は激怒した。"s,
                                name(tc) + u8" get"s + _s(tc) + u8" angry."s));
                        }
                    }
                    cdata[tc].hate = 30;
                    continue;
                }
                if (cc == 0)
                {
                    cdata[tc].interest -= rnd(15);
                    cdata[tc].time_interest_revive = gdata_hour + gdata_day * 24
                        + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12
                        + 12;
                }
                if (cdata[tc].interest <= 0)
                {
                    if (fov_player_sees(cc))
                    {
                        txtef(9);
                        if (jp)
                        {
                            txt(u8"「飽きた」"s,
                                u8"「前にも聴いたよ」"s,
                                u8"「またこの曲か…」"s);
                        }
                        if (en)
                        {
                            txt(u8"\"Boring.\""s,
                                u8"\"I've heard this before.\""s,
                                u8"\"This song again?\""s);
                        }
                    }
                    cdata[tc].interest = 0;
                    continue;
                }
                if (sdata(183, cc) < cdata[tc].level)
                {
                    if (rnd(3) == 0)
                    {
                        cdata[cc].quality_of_performance -= cdata[tc].level / 2;
                        if (fov_player_sees(cc))
                        {
                            txtef(9);
                            if (jp)
                            {
                                txt(u8"「引っ込め！」"s,
                                    u8"「うるさい！」"s,
                                    u8"「下手っぴ！」"s,
                                    u8"「何のつもりだ！」"s);
                            }
                            if (en)
                            {
                                txt(u8"\"Boo boo!\""s,
                                    u8"\"Shut it!\""s,
                                    u8"\"What are you doing!\""s,
                                    u8"\"You can't play shit.\""s);
                            }
                            txt(lang(
                                ""s + name(tc) + u8"は石を投げた。"s,
                                name(tc) + u8" throw"s + _s(tc)
                                    + u8" a rock."s));
                        }
                        dmg = rnd(cdata[tc].level + 1) + 1;
                        if (cdata[tc].id == 29)
                        {
                            dmg = cdata[tc].level * 2 + rnd(100);
                        }
                        dmghp(cc, dmg, -8);
                        if (cdata[cc].state == 0)
                        {
                            break;
                        }
                        continue;
                    }
                }
                if (rnd(3) == 0)
                {
                    p = cdata[cc].quality_of_performance
                            * cdata[cc].quality_of_performance
                            * (100 + inv[ci].param1 / 5) / 100 / 1000
                        + rnd(10);
                    p = clamp(
                        cdata[tc].gold * clamp(p(0), 1, 100) / 125,
                        0,
                        sdata(183, cc) * 100);
                    if (tc < 16)
                    {
                        p = rnd(clamp(p(0), 1, 100)) + 1;
                    }
                    if ((cdata[tc].character_role >= 1000
                         && cdata[tc].character_role < 2000)
                        || cdata[tc].character_role == 2003)
                    {
                        p /= 5;
                    }
                    if (p > cdata[tc].gold)
                    {
                        p = cdata[tc].gold;
                    }
                    cdata[tc].gold -= p;
                    cdata[cc].gold += p;
                    gold += p;
                }
                if (cdata[tc].level > sdata(183, cc))
                {
                    continue;
                }
                p = rnd(cdata[tc].level + 1) + 1;
                if (rnd(sdata(183, cc) + 1) > rnd(cdata[tc].level * 2 + 1))
                {
                    if (gdata_executing_immediate_quest_type == 1009)
                    {
                        if (tc >= 57)
                        {
                            cdata[tc].impression += rnd(3);
                            calcpartyscore();
                        }
                    }
                    if (rnd(2) == 0)
                    {
                        cdata[cc].quality_of_performance += p;
                    }
                    else if (rnd(2) == 0)
                    {
                        cdata[cc].quality_of_performance -= p;
                    }
                }
                if (encfindspec(ci, 60))
                {
                    if (rnd(15) == 0)
                    {
                        dmgcon(tc, 8, 500);
                    }
                }
                if (rnd(sdata(183, cc) + 1) > rnd(cdata[tc].level * 5 + 1))
                {
                    if (rnd(3) == 0)
                    {
                        if (fov_player_sees(cc))
                        {
                            txtef(9);
                            if (jp)
                            {
                                txt(name(tc) + u8"は歓声を上げた。"s,
                                    name(tc) + u8"は目を輝かせた。"s,
                                    u8"「ブラボー」"s,
                                    u8"「いいぞ！」"s,
                                    name(tc) + u8"はうっとりした。"s,
                                    name(tc) + u8"は"s + name(cc)
                                        + u8"の演奏を褒め称えた。"s);
                            }
                            if (en)
                            {
                                txt(name(tc) + u8" clap"s + _s(tc) + u8"."s,
                                    name(tc) + u8" listene"s + _s(tc)
                                        + u8" to "s + name(cc) + your(cc)
                                        + u8" music joyfully."s,
                                    u8"\"Bravo!\""s,
                                    u8"\"Nice song.\""s,
                                    u8"\"Scut!\""s,
                                    name(tc) + u8" "s + is(tc)
                                        + u8" excited!"s);
                            }
                        }
                        cdata[cc].quality_of_performance += cdata[tc].level + 5;
                        if (cc == 0)
                        {
                            if (tc >= 16)
                            {
                                if (rnd(performtips * 2 + 2) == 0)
                                {
                                    x = clamp(
                                        cdata[cc].position.x - 1 + rnd(3),
                                        0,
                                        mdata(0) - 1);
                                    y = clamp(
                                        cdata[cc].position.y - 1 + rnd(3),
                                        0,
                                        mdata(1) - 1);
                                    cell_check(x, y);
                                    if (cellaccess == 0)
                                    {
                                        continue;
                                    }
                                    if (fov_los(
                                            cdata[tc].position.x,
                                            cdata[tc].position.y,
                                            x,
                                            y)
                                        == 0)
                                    {
                                        continue;
                                    }
                                    if (encfindspec(ci, 49))
                                    {
                                        flt(calcobjlv(
                                                cdata[cc].quality_of_performance
                                                / 8),
                                            calcfixlv(3 + (rnd(4) == 0)));
                                    }
                                    else
                                    {
                                        flt(calcobjlv(
                                                cdata[cc].quality_of_performance
                                                / 10),
                                            calcfixlv(3));
                                    }
                                    flttypemajor =
                                        fsetperform(rnd(length(fsetperform)));
                                    dbid = 0;
                                    if (gdata_executing_immediate_quest_type
                                        == 1009)
                                    {
                                        if (rnd(150) == 0)
                                        {
                                            dbid = 241;
                                        }
                                        if (rnd(150) == 0)
                                        {
                                            dbid = 622;
                                        }
                                        if (cdata[tc].level > 15)
                                        {
                                            if (rnd(1000) == 0)
                                            {
                                                dbid = 725;
                                            }
                                        }
                                        if (cdata[tc].level > 10)
                                        {
                                            if (rnd(800) == 0)
                                            {
                                                dbid = 726;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        if (rnd(10) == 0)
                                        {
                                            dbid = 724;
                                        }
                                        if (rnd(250) == 0)
                                        {
                                            dbid = 55;
                                        }
                                    }
                                    int stat = itemcreate(-1, dbid, x, y, 1);
                                    if (stat != 0)
                                    {
                                        --inv[ci].number;
                                        cell_refresh(
                                            inv[ci].position.x,
                                            inv[ci].position.y);
                                        ccbk = cc;
                                        cc = tc;
                                        aniref(0) = inv[ci].image;
                                        aniref(1) = inv[ci].color;
                                        anix = inv[ci].position.x;
                                        aniy = inv[ci].position.y;
                                        play_animation(15);
                                        cc = ccbk;
                                        ++inv[ci].number;
                                        cell_refresh(
                                            inv[ci].position.x,
                                            inv[ci].position.y);
                                        ++performtips;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (gold != 0)
            {
                cdata[cc].tip_gold += gold;
                if (fov_player_sees(cc))
                {
                    snd(11);
                }
            }
        }
        return;
    }
    for (int cnt = 0; cnt < 1; ++cnt)
    {
        if (cc != 0)
        {
            break;
        }
        if (cdata[cc].quality_of_performance < 0)
        {
            txt(lang(u8"まるで駄目だ…"s, u8"It is a waste of time."s));
            break;
        }
        if (cdata[cc].quality_of_performance < 20)
        {
            txt(lang(u8"不評だった…"s, u8"Almost everyone ignores you."s));
            break;
        }
        if (cdata[cc].quality_of_performance < 40)
        {
            txt(lang(
                u8"もっと練習しなくては…"s,
                u8"You need to practice lot more."s));
            break;
        }
        if (cdata[cc].quality_of_performance == 40)
        {
            txt(lang(u8"演奏を終えた。"s, u8"You finish your performance."s));
            break;
        }
        if (cdata[cc].quality_of_performance < 60)
        {
            txt(lang(u8"いまいちだ。"s, u8"Not good."s));
            break;
        }
        if (cdata[cc].quality_of_performance < 80)
        {
            txt(lang(
                u8"手ごたえがあった。"s,
                u8"People seem to like your performance."s));
            break;
        }
        if (cdata[cc].quality_of_performance < 100)
        {
            txt(lang(
                u8"納得できる演奏ができた。"s,
                u8"Your performance is successful."s));
            break;
        }
        if (cdata[cc].quality_of_performance < 120)
        {
            txt(lang(u8"大盛況だ！"s, u8"Wonderful!"s));
            break;
        }
        if (cdata[cc].quality_of_performance < 150)
        {
            txt(lang(
                u8"素晴らしい！"s,
                u8"Great performance. Everyone cheers you."s));
            break;
        }
        txt(lang(u8"歴史に残る名演だ！"s, u8"A Legendary stage!"s));
        break;
    }
    if (cdata[cc].quality_of_performance > 40)
    {
        cdata[cc].quality_of_performance =
            cdata[cc].quality_of_performance * (100 + inv[ci].param1 / 5) / 100;
    }
    if (cdata[cc].tip_gold != 0)
    {
        if (fov_player_sees(cc))
        {
            txt(lang(
                name(cc) + u8"は合計 "s + cdata[cc].tip_gold
                    + i18n::_(u8"ui", u8"gold") + u8"のおひねりを貰った。"s,
                u8"The audience gives "s + name(cc) + u8" total of "s
                    + cdata[cc].tip_gold + u8" gold pieces."s));
        }
    }
    rowactend(cc);
    int experience = cdata[cc].quality_of_performance - sdata(183, cc) + 50;
    if (experience > 0)
    {
        skillexp(183, cc, experience, 0, 0);
    }
    return;
}



void do_disarm_trap()
{
    cell_featset(movx, movy, 0);
    if (cdata[cc].god_id == core_god::mani)
    {
        txt(lang(u8"あなたは罠を解体した。"s, u8"You dismantle the trap."s));
        for (int cnt = 0, cnt_end = (rnd(3) + 1); cnt < cnt_end; ++cnt)
        {
            atxspot = 19;
            matgetmain(
                random_material(
                    gdata_current_dungeon_level,
                    gdata_current_dungeon_level / 5),
                1);
        }
    }
    else
    {
        txt(lang(u8"罠を解除した。"s, u8"You disarm the trap."s));
    }
}



void continuous_action_others()
{
    if (cc != 0)
    {
        rowactend(cc);
        return;
    }
    if (cdata[cc].continuous_action_id == 0)
    {
        cdata[cc].continuous_action_id = 10;
        cdata[cc].continuous_action_item = ci;
        cdata[cc].continuous_action_target = tc;
        if (gdata(91) == 105)
        {
            txt(lang(
                itemname(ci, 1) + u8"に目星をつけた。"s,
                u8"You target "s + itemname(ci, 1) + u8"."s));
            cdata[cc].continuous_action_turn =
                2 + clamp(inv[ci].weight / 500, 0, 50);
        }
        if (gdata(91) == 100)
        {
            if (mdata(6) == 5 || mdata(6) == 3 || mdata(6) == 2)
            {
                txt(lang(u8"寝る仕度を始めた。"s, u8"You lie down."s));
                cdata[cc].continuous_action_turn = 5;
            }
            else
            {
                txt(lang(u8"露営の準備を始めた。"s, u8"You start to camp."s));
                cdata[cc].continuous_action_turn = 20;
            }
        }
        if (gdata(91) == 101)
        {
            txt(lang(
                itemname(ci, 1) + u8"の建設を始めた。"s,
                u8"You start to construct "s + itemname(ci, 1) + u8"."s));
            cdata[cc].continuous_action_turn = 25;
        }
        if (gdata(91) == 102)
        {
            txt(lang(
                itemname(ci, 1) + u8"のハッチを回し始めた。"s,
                u8"You start to pull the hatch."s));
            cdata[cc].continuous_action_turn = 10;
        }
        if (gdata(91) == 103)
        {
            txt(lang(
                itemname(ci, 1) + u8"を掘り始めた。"s,
                u8"You start to pick "s + itemname(ci, 1) + u8"."s));
            cdata[cc].continuous_action_turn = 10
                + clamp(inv[ci].weight
                            / (1 + sdata(10, 0) * 10 + sdata(180, 0) * 40),
                        1,
                        100);
        }
        if (gdata(91) == 104)
        {
            if (gdata_weather == 0 || gdata_weather == 3)
            {
                if (gdata_time_when_textbook_becomes_available > gdata_hour
                        + gdata_day * 24 + gdata_month * 24 * 30
                        + gdata_year * 24 * 30 * 12)
                {
                    txt(lang(u8"もう飽きた。"s, u8"You are bored."s));
                    rowactend(cc);
                    return;
                }
            }
            gdata_time_when_textbook_becomes_available = gdata_hour
                + gdata_day * 24 + gdata_month * 24 * 30
                + gdata_year * 24 * 30 * 12 + 48;
            if (inv[ci].id == 563)
            {
                txt(lang(
                    i18n::_(
                        u8"ability", std::to_string(inv[ci].param1), u8"name")
                        + u8"の学習を始めた。"s,
                    u8"You begin to study "s
                        + i18n::_(
                              u8"ability",
                              std::to_string(inv[ci].param1),
                              u8"name")
                        + u8"."s));
            }
            else
            {
                txt(lang(
                    u8"トレーニングを始めた。"s, u8"You start training."s));
            }
            if (gdata_weather != 0 && gdata_weather != 3)
            {
                if (gdata_current_map == 30
                    || (mdata(14) == 1
                        && (mdata(6) == 5 || mdata(6) == 3 || mdata(6) == 2)))
                {
                    txt(lang(
                        u8"外が悪天候なので、じっくりと取り組むことにした。"s,
                        u8"The weather's bad outside, you have plenty of time to waste."s));
                }
            }
            cdata[cc].continuous_action_turn = 50;
        }
        update_screen();
        return;
    }
    tc = cdata[cc].continuous_action_target;
    if (cdata[cc].continuous_action_turn > 0)
    {
        if (gdata(91) == 103)
        {
            if (rnd(5) == 0)
            {
                skillexp(180, 0, 20, 4);
            }
            if (rnd(6) == 0)
            {
                if (rnd(55) > sdata.get(10, cc).original_level + 25)
                {
                    skillexp(10, cc, 50);
                }
            }
            if (rnd(8) == 0)
            {
                if (rnd(55) > sdata.get(11, cc).original_level + 28)
                {
                    skillexp(11, cc, 50);
                }
            }
            if (rnd(10) == 0)
            {
                if (rnd(55) > sdata.get(15, cc).original_level + 30)
                {
                    skillexp(15, cc, 50);
                }
            }
            if (rnd(4) == 0)
            {
                txtef(9);
                if (jp)
                {
                    txt(u8" *ザクッ* "s,
                        u8" *♪* "s,
                        u8" *ズシュ* "s,
                        u8" *ジャリ* "s);
                }
                if (en)
                {
                    txt(u8"*sing*"s,
                        u8"*pull*"s,
                        u8"*thud*"s,
                        u8"*rumble*"s,
                        u8"*gasp*"s);
                }
            }
        }
        if (gdata(91) == 104)
        {
            p = 25;
            if (gdata_weather != 0 && gdata_weather != 3)
            {
                if (gdata_current_map == 30)
                {
                    p = 5;
                }
                if (gdata_current_map != 30 && mdata(14) == 1)
                {
                    if (mdata(6) == 5 || mdata(6) == 3 || mdata(6) == 2)
                    {
                        p = 5;
                        gdata_minute += 30;
                    }
                }
            }
            if (inv[ci].id == 563)
            {
                if (rnd(p) == 0)
                {
                    skillexp(inv[ci].param1, cc, 25);
                }
            }
            else if (rnd(p) == 0)
            {
                skillexp(randattb(), cc, 25);
            }
        }
        if (gdata(91) == 105)
        {
            if (inv[ci].id == 688)
            {
                if (rnd(15) == 0)
                {
                    rowactend(cc);
                    txt(lang(
                        u8"突然ふたが閉まった！"s,
                        u8"Suddenly, the iron maiden falls forward."s));
                    dmghp(cc, 9999, -18);
                    return;
                }
            }
            if (inv[ci].id == 689)
            {
                if (rnd(15) == 0)
                {
                    rowactend(cc);
                    txt(lang(
                        u8"突然ギロチンが落ちてきた！"s,
                        u8"Suddenly, the guillotine is activated."s));
                    dmghp(cc, 9999, -19);
                    return;
                }
            }
            f = 0;
            f2 = 0;
            tg = inv_getowner(ci);
            if (tg != -1)
            {
                if (cdata[tg].original_relationship == -3)
                {
                    f2 = 1;
                }
            }
            i = sdata(300, 0) * 5 + sdata(12, 0) + 25;
            if (gdata_hour >= 19 || gdata_hour < 7)
            {
                i = i * 15 / 10;
            }
            if (inv[ci].quality == 3)
            {
                i = i * 8 / 10;
            }
            if (inv[ci].quality >= 4)
            {
                i = i * 5 / 10;
            }
            make_sound(cdata[0].position.x, cdata[0].position.y, 5, 8);
            for (int cnt = 16; cnt < ELONA_MAX_CHARACTERS; ++cnt)
            {
                if (cdata[cnt].state != 1)
                {
                    continue;
                }
                if (cdata[cnt].sleep != 0)
                {
                    continue;
                }
                if (dist(
                        cdata[cnt].position.x,
                        cdata[cnt].position.y,
                        cdata[0].position.x,
                        cdata[0].position.y)
                    > 5)
                {
                    continue;
                }
                if (f2 == 1)
                {
                    if (cnt != tg)
                    {
                        continue;
                    }
                }
                p = rnd((i + 1))
                    * (80 + (fov_player_sees(cnt) == 0) * 50
                       + dist(
                             cdata[cnt].position.x,
                             cdata[cnt].position.y,
                             cdata[0].position.x,
                             cdata[0].position.y)
                           * 20)
                    / 100;
                if (cnt < 57)
                {
                    p = p * 2 / 3;
                }
                if (rnd(sdata(13, cnt) + 1) > p)
                {
                    if (fov_player_sees(cnt))
                    {
                        txt(lang(
                            name(cnt) + u8"は窃盗を見咎めた。"s,
                            name(cnt) + u8" notice"s + _s(cnt) + u8" you,"s));
                    }
                    else
                    {
                        txt(lang(
                            name(cnt) + u8"は不振な物音に気づいた。"s,
                            name(cnt) + u8" hear"s + _s(cnt)
                                + u8" loud noise,"s));
                    }
                    if (cdata[cnt].character_role == 14)
                    {
                        txt(lang(
                            u8"「貴様、何をしている！」"s,
                            u8"\"You there, stop!\""s));
                        chara_mod_impression(cnt, -5);
                    }
                    else
                    {
                        txt(lang(
                            u8"「ガード！ガード！」"s,
                            u8"\"Guards! Guards!\""s));
                        chara_mod_impression(cnt, -5);
                    }
                    cdata[cnt].emotion_icon = 520;
                    f = 1;
                }
            }
            if (f)
            {
                txt(lang(
                    u8"盗みを見咎められた！"s, u8"You are found stealing."s));
                modify_karma(0, -5);
                p = inv_getowner(ci);
                if (tg != -1)
                {
                    if (cdata[p].id != 202)
                    {
                        if (cdata[tg].sleep == 0)
                        {
                            cdata[tg].relationship = -2;
                            hostileaction(0, tg);
                            chara_mod_impression(tg, -20);
                        }
                    }
                }
                go_hostile();
            }
            if (tg != -1)
            {
                if (cdata[tg].state != 1)
                {
                    if (f != 1)
                    {
                        txt(lang(
                            u8"対象は死んでいる"s, u8"The target is dead."s));
                        f = 1;
                    }
                }
                if (cdata[tg].character_role == 20)
                {
                    if (f != 1)
                    {
                        txt(lang(
                            u8"それは盗めない。"s, u8"It can't be stolen."s));
                        f = 1;
                    }
                }
                if (dist(
                        cdata[cc].position.x,
                        cdata[cc].position.y,
                        cdata[tg].position.x,
                        cdata[tg].position.y)
                    >= 3)
                {
                    if (f != 1)
                    {
                        txt(lang(
                            u8"対象が見当たらない。"s,
                            u8"You lose the target."s));
                        f = 0;
                    }
                }
            }
            if (inv[ci].number <= 0)
            {
                f = 1;
            }
            if (ibit(5, ci) == 1)
            {
                if (f != 1)
                {
                    txt(lang(u8"それは盗めない。"s, u8"It can't be stolen."s));
                    f = 1;
                }
            }
            if (inv[ci].weight >= sdata(10, 0) * 500)
            {
                if (f != 1)
                {
                    txt(lang(
                        u8"重すぎて手に負えない。"s, u8"It's too heavy."s));
                    f = 1;
                }
            }
            if (itemusingfind(ci, true) != -1)
            {
                if (f != 1)
                {
                    txt(lang(
                        u8"そのアイテムは他の誰かが使用中だ。"s,
                        u8"Someone else is using the item."s));
                    f = 1;
                }
            }
            if (f)
            {
                txt(lang(u8"行動を中断した。"s, u8"You abort stealing."s));
                rowactend(cc);
            }
        }
        return;
    }
    if (gdata(91) == 105)
    {
        tg = inv_getowner(ci);
        if (tg != -1)
        {
            if (cdata[tg].state != 1)
            {
                txt(lang(u8"行動を中断した。"s, u8"You abort stealing."s));
                rowactend(cc);
                return;
            }
        }
        in = 1;
        if (inv[ci].id == 54)
        {
            in = inv[ci].number;
        }
        ti = inv_getfreeid(0);
        if (ti == -1)
        {
            txt(lang(
                u8"バックパックには、もうアイテムを入れる余裕がない。"s,
                u8"Your inventory is full."s));
            return;
        }
        ibitmod(12, ci, 0);
        if (inv[ci].body_part != 0)
        {
            tc = inv_getowner(ci);
            if (tc != -1)
            {
                p = inv[ci].body_part;
                cdata_body_part(tc, p) = cdata_body_part(tc, p) / 10000 * 10000;
            }
            inv[ci].body_part = 0;
            chara_refresh(tc);
        }
        item_copy(ci, ti);
        inv[ti].number = in;
        ibitmod(9, ti, 1);
        inv[ti].own_state = 0;
        inv[ci].number -= in;
        if (inv[ci].number <= 0)
        {
            cell_refresh(inv[ci].position.x, inv[ci].position.y);
        }
        txt(lang(
            itemname(ti) + u8"を盗んだ。"s,
            u8"You successfully steal "s + itemname(ti) + u8"."s));
        if (inv[ci].id == 54)
        {
            snd(11);
            inv[ti].number = 0;
            cdata[0].gold += in;
        }
        else
        {
            item_stack(0, ti, 1);
            snd(14 + rnd(2));
        }
        refresh_burden_state();
        skillexp(300, 0, clamp(inv[ti].weight / 25, 0, 450) + 50);
        if (cdata[0].karma >= -30)
        {
            if (rnd(3) == 0)
            {
                txt(lang(
                    u8"あなたは良心の呵責を感じた。"s,
                    u8"You feel the stings of conscience."s));
                modify_karma(0, -1);
            }
        }
    }
    if (gdata(91) == 100)
    {
        txt(lang(u8"あなたは眠り込んだ。"s, u8"You fall asleep."s));
        label_2151();
    }
    if (gdata(91) == 101)
    {
        snd(58);
        txt(lang(
            itemname(ci, 1) + u8"を作り終えた。"s,
            u8"You finish constructing"s + itemname(ci, 1) + u8"."s));
        in = 1;
        dropval = 1;
        drop_item();
    }
    if (gdata(91) == 102)
    {
        txt(lang(
            itemname(ci, 1) + u8"のハッチを回し終えた。"s,
            u8"You finish pulling the hatch of "s + itemname(ci, 1) + u8"."s));
        chatteleport = 1;
        gdata_previous_map2 = gdata_current_map;
        gdata_previous_dungeon_level = gdata_current_dungeon_level;
        gdata_previous_x = cdata[0].position.x;
        gdata_previous_y = cdata[0].position.y;
        gdata_destination_map = 30;
        gdata_destination_dungeon_level = inv[ci].count;
        levelexitby = 2;
        snd(49);
    }
    if (gdata(91) == 103)
    {
        txt(lang(
            itemname(ci, 1) + u8"を収穫した("s + cnvweight(inv[ci].weight)
                + u8")"s,
            u8"You harvest "s + itemname(ci, 1) + u8". ("s
                + cnvweight(inv[ci].weight) + u8")"s));
        in = inv[ci].number;
        pick_up_item();
    }
    if (gdata(91) == 104)
    {
        if (inv[ci].id == 563)
        {
            txt(lang(
                i18n::_(u8"ability", std::to_string(inv[ci].param1), u8"name")
                    + u8"の学習を終えた。"s,
                u8"You finish studying "s
                    + i18n::_(
                          u8"ability", std::to_string(inv[ci].param1), u8"name")
                    + u8"."s));
        }
        else
        {
            txt(lang(u8"トレーニングを終えた。"s, u8"You finish training."s));
        }
    }
    rowactend(cc);
    return;
}

turn_result_t show_house_board()
{
    txtnew();
    if (mdata(6) != 5)
    {
        ++msgdup;
        txt(lang(
            u8"ここはあなたの家ではない。"s,
            u8"You can only use it in your home."s));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    p(0) = 0;
    p(1) = 0;
    p(2) = 0;
    for (const auto& cnt : items(-1))
    {
        ++p(2);
        if (inv[cnt].number != 0)
        {
            if (the_item_db[inv[cnt].id]->category != 60000)
            {
                ++p;
            }
            else
            {
                ++p(1);
            }
        }
    }
    if (mdata(18) != 0)
    {
        p(2) = mdata(18);
    }
    txt(lang(
        mapname(gdata_current_map) + u8"には"s + p + u8"個のアイテムと"s + p(1)
            + u8"個の家具がある(アイテム最大"s + p(2) + u8"個) "s,
        u8"There are "s + p + u8" items and "s + p(1) + u8" furniture in "s
            + mapname(gdata_current_map) + u8".(Max: "s + p(2) + u8") "s));
    if (adata(16, gdata_current_map) == 102)
    {
        if (getworker(gdata_current_map) != -1)
        {
            txt(lang(
                u8"現在の店主は"s + cdatan(0, getworker(gdata_current_map))
                    + u8"だ。"s,
                u8"Current shopkeeper is "s
                    + cdatan(0, getworker(gdata_current_map)) + u8"."s));
        }
        else
        {
            txt(lang(
                u8"現在店主はいない。"s,
                u8"You haven't assigned a shopkeeper yet."s));
        }
    }
    if (adata(16, gdata_current_map) == 31)
    {
        if (getworker(gdata_current_map) != -1)
        {
            txt(lang(
                u8"現在のブリーダーは"s
                    + cdatan(0, getworker(gdata_current_map)) + u8"だ。"s,
                u8"Current breeder is "s
                    + cdatan(0, getworker(gdata_current_map)) + u8"."s));
        }
        else
        {
            txt(lang(
                u8"現在ブリーダーはいない。"s,
                u8"You haven't assigned a breeder yet."s));
        }
    }
    if (gdata_current_map == 7)
    {
        p = 0;
        for (int cnt = ELONA_MAX_PARTY_CHARACTERS; cnt < ELONA_MAX_CHARACTERS;
             ++cnt)
        {
            if (cdata[cnt].state == 1 || cdata[cnt].state == 2)
            {
                if (cdata[cnt].character_role != 0)
                {
                    ++p;
                }
            }
        }
        txt(lang(
            u8"現在"s + p + u8"人の滞在者がいる(最大"s + (gdata_home_scale + 2)
                + u8"人) "s,
            ""s + p + u8" members are staying at your home. (Max: "s
                + (gdata_home_scale + 2) + u8")."s));
    }
    txtnew();
    txt(lang(u8"何をする？"s, u8"What do you want to do?"s));
    p = 0;
    if (adata(16, gdata_current_map) == 102)
    {
        ELONA_APPEND_PROMPT(
            lang(u8"仲間に店主を頼む"s, u8"Assign a shopkeeper"s),
            u8"null"s,
            ""s + 4);
        if (mdata(18) < 400)
        {
            ELONA_APPEND_PROMPT(
                lang(u8"店を拡張"s, u8"Extend"s) + u8"("s + calcshopreform()
                    + u8" GP)"s,
                u8"null"s,
                ""s + 5);
        }
    }
    if (adata(16, gdata_current_map) == 31)
    {
        ELONA_APPEND_PROMPT(
            lang(u8"ブリーダーを任命する"s, u8"Assign a breeder"s),
            u8"null"s,
            ""s + 4);
    }
    ELONA_APPEND_PROMPT(
        lang(u8"家の模様替え"s, u8"Design"s), u8"null"s, ""s + 0);
    if (gdata_current_map == 7)
    {
        ELONA_APPEND_PROMPT(
            lang(u8"家の情報"s, u8"Home rank"s), u8"null"s, ""s + 2);
        ELONA_APPEND_PROMPT(
            lang(u8"仲間の滞在"s, u8"Allies in your home"s),
            u8"null"s,
            ""s + 4);
        if (gdata_current_dungeon_level == 1)
        {
            ELONA_APPEND_PROMPT(
                lang(u8"使用人を募集する"s, u8"Recruit a servant"s),
                u8"null"s,
                ""s + 6);
        }
        ELONA_APPEND_PROMPT(
            lang(u8"滞在者の移動"s, u8"Move a stayer"s), u8"null"s, ""s + 3);
    }
    int stat = show_prompt(promptx, prompty, 240);
    if (stat == -1)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    rtval = stat;
    switch (rtval)
    {
    case 0:
        start_home_map_mode();
        break;
    case 2:
        show_home_value();
        break;
    case 3:
        prompt_move_ally();
        break;
    case 4:
        prompt_ally_staying();
        break;
    case 5:
        try_extend_shop();
        break;
    case 6:
        prompt_hiring();
        break;
    }
label_1722_internal:
    tlocinitx = 0;
    tlocinity = 0;
    screenupdate = -1;
    update_screen();
    return turn_result_t::show_house_board;
}

int try_to_cast_spell()
{
    int r4 = 0;
    f = 1;
    tc = cc;
    if (cdata[cc].blind != 0)
    {
        f = 0;
    }
    if (cdata[cc].confused != 0 || cdata[cc].dimmed != 0)
    {
        if (rnd(4) != 0)
        {
            f = 0;
        }
    }
    else
    {
        if (r3 == 0)
        {
            r4 = sdata(16, cc);
        }
        else
        {
            r4 = sdata(the_ability_db[r3]->related_basic_attribute, cc);
        }
        if (rnd(sdata(150, cc) * r4 * 4 + 250) < rnd(r2 + 1))
        {
            if (rnd(7) == 0)
            {
                f = 0;
            }
            if (r4 * 10 < r2)
            {
                if (rnd(r4 * 10 + 1) < rnd(r2 + 1))
                {
                    f = 0;
                }
            }
            if (r4 * 20 < r2)
            {
                if (rnd(r4 * 20 + 1) < rnd(r2 + 1))
                {
                    f = 0;
                }
            }
            if (r4 * 30 < r2)
            {
                if (rnd(r4 * 30 + 1) < rnd(r2 + 1))
                {
                    f = 0;
                }
            }
        }
    }
    if (f == 1)
    {
        return 1;
    }
    if (rnd(4) == 0)
    {
        if (fov_player_sees(cc))
        {
            txt(lang(
                name(cc) + u8"はマナを吸い取られた！"s,
                name(cc) + your(cc) + u8" mana is absorbed."s));
        }
        if (cc == 0)
        {
            dmgmp(cc, cdata[cc].max_mp);
        }
        else
        {
            dmgmp(cc, cdata[cc].max_mp / 3);
        }
        return 0;
    }
    if (rnd(4) == 0)
    {
        if (fov_player_sees(cc))
        {
            if (cdata[cc].confused != 0)
            {
                txt(lang(
                    name(cc) + u8"は余計に混乱した。"s,
                    name(cc) + u8" "s + is(cc) + u8" confused more."s));
            }
            else
            {
                txt(lang(u8"難解だ！"s, u8"It's too difficult!"s));
            }
        }
        dmgcon(cc, 5, 100);
        return 0;
    }
    if (rnd(4) == 0)
    {
        if (fov_player_sees(cc))
        {
            txt(lang(
                u8"魔力の渦が何かを召喚した！"s,
                u8"Several creatures are summoned from a vortex of magic."s));
        }
        for (int cnt = 0, cnt_end = (2 + rnd(3)); cnt < cnt_end; ++cnt)
        {
            flt(calcobjlv(cdata[0].level * 3 / 2 + 3), calcfixlv(2));
            int stat =
                chara_create(-1, 0, cdata[cc].position.x, cdata[cc].position.y);
            if (stat != 0)
            {
                if (cdata[cc].relationship <= -3)
                {
                    cdata[rc].relationship = -1;
                    cdata[rc].original_relationship = -1;
                }
            }
        }
        return 0;
    }
    if (fov_player_sees(cc))
    {
        txt(lang(
            name(cc) + u8"は奇妙な力に捻じ曲げられた！"s,
            u8"A dimension door opens in front of you."s));
    }
    tc = cc;
    efid = 408;
    magic();
    return 0;
}



int try_to_reveal()
{
    if (rnd(sdata(159, cc) * 15 + 20 + sdata(13, cc))
        > rnd(gdata_current_dungeon_level * 8 + 60))
    {
        gain_skill_experience_detection(cc);
        return 1;
    }
    return 0;
}






int try_to_disarm_trap()
{
    if (rnd(sdata(175, cc) * 15 + 20 + sdata(12, cc))
        > rnd(gdata_current_dungeon_level * 12 + 100))
    {
        gain_disarm_trap_experience();
        return 1;
    }
    return 0;
}



int try_to_perceive_npc(int cc)
{
    int cv = 0;
    cv = 8;
    if (cdata[cc].position.x > cdata[r2].position.x - cv
        && cdata[cc].position.x < cdata[r2].position.x + cv)
    {
        if (cdata[cc].position.y > cdata[r2].position.y - cv
            && cdata[cc].position.y < cdata[r2].position.y + cv)
        {
            if (cdata[r2].hate > 0)
            {
                return 1;
            }
            p = dist(
                    cdata[cc].position.x,
                    cdata[cc].position.y,
                    cdata[r2].position.x,
                    cdata[r2].position.y)
                    * 150
                + (sdata(157, cc) * 100 + 150) + 1;
            if (rnd(p) < rnd(sdata(13, r2) * 60 + 150))
            {
                return 1;
            }
        }
    }
    if (pcnoise != 0)
    {
        if (rnd(150) < pcnoise)
        {
            return 1;
        }
    }
    return 0;
}



void start_stealing()
{
    gdata(91) = 105;
    continuous_action_others();
    return;
}



turn_result_t try_interact_with_npc()
{
    if (cdata[tc].continuous_action_turn != 0)
    {
        txt(lang(
            name(tc) + u8"は忙しい。"s,
            name(tc) + u8" "s + is(tc) + u8" busy now."s));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    menucycle = 1;
    invally = 1;
    invctrl = 10;
    snd(100);
    menu_result mr = ctrl_inventory();
    assert(mr.turn_result != turn_result_t::none);
    return mr.turn_result;
}



turn_result_t call_npc()
{
    txt(lang(
        name(tc) + u8"を何と呼ぶ？ "s,
        u8"What do you want to call "s + him(tc) + u8"? "s));
    inputlog = "";
    input_mode = 1;
    input_text_dialog((windoww - 220) / 2 + inf_screenx, winposy(90), 12);
    if (inputlog == ""s)
    {
        txt(lang(u8"名前をつけるのはやめた。"s, u8"You changed your mind."s));
    }
    else
    {
        cdatan(0, tc) = ""s + inputlog;
        cdata[tc].has_own_name() = true;
        txt(lang(
            ""s + cdatan(0, tc) + u8"という名前で呼ぶことにした。"s,
            u8"You named "s + him(tc) + u8" "s + cdatan(0, tc) + u8"."s));
    }
    label_1416();
    return turn_result_t::pc_turn_user_error;
}


turn_result_t do_short_cut()
{
    menucycle = 0;
    if (gdata(40 + sc) == 0)
    {
        ++msgdup;
        txt(lang(
            u8"そのキーにはショートカットが割り当てられていない。"s,
            u8"The key is unassigned."s));
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    if (gdata(40 + sc) >= 10000)
    {
        invsc = gdata((40 + sc)) % 10000;
        invctrl(0) = gdata((40 + sc)) / 10000;
        invctrl(1) = 0;
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    efid = gdata(40 + sc);
    if (efid >= 300 && efid < 400)
    {
        return do_use_magic();
    }
    if (efid >= 600)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            return turn_result_t::pc_turn_user_error;
        }
        if (efid < 661)
        {
            if (spact(efid - 600) == 0)
            {
                txt(lang(
                    u8"もうその行動はできない。"s,
                    u8"You can't use this shortcut any more."s));
                update_screen();
                return turn_result_t::pc_turn_user_error;
            }
        }
        return do_use_magic();
    }
    if (efid >= 400)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            return turn_result_t::pc_turn_user_error;
        }
        if (spell(efid - 400) <= 0)
        {
            ++msgdup;
            txt(lang(
                u8"その魔法はもう使えない。"s,
                u8"You can't use that spell anymore."s));
            update_screen();
            return turn_result_t::pc_turn_user_error;
        }
        return do_cast_command();
    }
    return turn_result_t::pc_turn_user_error;
}



turn_result_t do_use_magic()
{
    int stat = label_2174();
    if (stat == 0)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    return turn_result_t::turn_end;
}







} // namespace elona
