#include "damage.hpp"
#include "ability.hpp"
#include "animation.hpp"
#include "audio.hpp"
#include "buff.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "config.hpp"
#include "debug.hpp"
#include "draw.hpp"
#include "enchantment.hpp"
#include "event.hpp"
#include "fov.hpp"
#include "map_cell.hpp"
#include "mef.hpp"
#include "i18n.hpp"
#include "item.hpp"
#include "quest.hpp"
#include "variables.hpp"


namespace elona
{

int dmg_at_m141 = 0;
int prm_853;
int prm_818;
int prm_820;
int f_at_con = 0;
int p_at_con = 0;
int res_at_con = 0;

int dmghp(int prm_853, int prm_854, int prm_855, int prm_856, int prm_857)
{
    int ele_at_m141 = 0;
    int c3_at_m141 = 0;
    int r_at_m141 = 0;
    int dmglevel_at_m141 = 0;
    int f_at_m141 = 0;
    int se_at_m141 = 0;
    elona_vector1<int> p_at_m141;
    int exp_at_m141 = 0;
    elona::prm_853 = prm_853;
    ele_at_m141 = prm_856;
    if (txt3rd == 0)
    {
        c3_at_m141 = prm_855;
    }
    else
    {
        c3_at_m141 = -1;
    }
    if (cdata[prm_853].state != 1)
    {
        end_dmghp();
        return 0;
    }
    dmg_at_m141 = prm_854 * (1 + (cdata[prm_853].furious > 0));
    if (prm_855 >= 0)
    {
        if (cdata[prm_855].furious > 0)
        {
            dmg_at_m141 *= 2;
        }
    }
    if (ele_at_m141 != 0 && ele_at_m141 < 61)
    {
        r_at_m141 = sdata(ele_at_m141, prm_853) / 50;
        if (r_at_m141 < 3)
        {
            dmg_at_m141 =
                dmg_at_m141 * 150 / clamp((r_at_m141 * 50 + 50), 40, 150);
        }
        else if (r_at_m141 < 10)
        {
            dmg_at_m141 = dmg_at_m141 * 100 / (r_at_m141 * 50 + 50);
        }
        else
        {
            dmg_at_m141 = 0;
        }
        dmg_at_m141 = dmg_at_m141 * 100 / (sdata(60, prm_853) / 2 + 50);
    }
    if (prm_855 == 0)
    {
        if (critical)
        {
            snd(3);
        }
        else
        {
            snd(2);
        }
    }
    if (cdata[prm_853].wet > 0)
    {
        if (ele_at_m141 == 50 || prm_855 == -9)
        {
            dmg_at_m141 = dmg_at_m141 / 3;
        }
        if (ele_at_m141 == 52)
        {
            dmg_at_m141 = dmg_at_m141 * 3 / 2;
        }
    }
    if (ele_at_m141)
    {
        if (ele_at_m141 != 60)
        {
            if (cdata[prm_853].is_immune_to_elemental_damage())
            {
                dmg_at_m141 = 0;
            }
        }
    }
    if (cdata[prm_853].is_metal())
    {
        dmg_at_m141 = rnd(dmg_at_m141 / 10 + 2);
    }
    if (cdata[prm_853].is_contracting_with_reaper())
    {
        if (cdata[prm_853].hp - dmg_at_m141 <= 0)
        {
            if (clamp(
                    25 + cdata[prm_853].buffs[findbuff(prm_853, 18)].power / 17,
                    25,
                    80)
                >= rnd(100))
            {
                dmg_at_m141 *= -1;
            }
        }
    }
    if (cdata[prm_853].nullify_damage > 0)
    {
        if (cdata[prm_853].nullify_damage > rnd(100))
        {
            dmg_at_m141 = 0;
        }
    }
    if (ele_at_m141 == 658)
    {
        dmg_at_m141 = prm_854;
    }
    rtdmg = dmg_at_m141;

    if (prm_853 == 0 && cdata[0].god_id == core_god::opatos)
    {
        dmg_at_m141 = dmg_at_m141 * 90 / 100;
    }

    if (debug::voldemort && prm_853 == 0)
    {
        dmg_at_m141 = 0;
    }
    cdata[prm_853].hp -= dmg_at_m141;


    if (fov_player_sees(prm_853))
    {
        add_damage_popup(std::to_string(dmg_at_m141), prm_853, {0, 0, 0});
    }


    if (ele_at_m141 == 56)
    {
        if (prm_855 >= 0)
        {
            if (dmg_at_m141 > 0)
            {
                healhp(
                    prm_855,
                    clamp(
                        rnd(dmg_at_m141 * (150 + prm_857 * 2) / 1000 + 10),
                        1,
                        cdata[prm_855].max_hp / 10 + rnd(5)));
            }
        }
    }
    if (prm_853 == 0)
    {
        gdata(30) = 0;
        if (cdata[prm_853].hp < 0)
        {
            if (event_id() != -1)
            {
                if (event_id() != 21)
                {
                    cdata[prm_853].hp = 1;
                }
            }
            if (gdata_current_map == 40)
            {
                cdata[prm_853].hp = 1;
            }
        }
    }
    if (dmg_at_m141 <= 0)
    {
        dmglevel_at_m141 = -1;
    }
    else
    {
        dmglevel_at_m141 = dmg_at_m141 * 6 / cdata[prm_853].max_hp;
    }
    if (cdata[prm_853].hp < 0)
    {
        if (prm_853 < 16)
        {
            for (int cnt = 0; cnt < 16; ++cnt)
            {
                if (prm_853 == cnt)
                {
                    continue;
                }
                if (cdata[cnt].state != 1)
                {
                    continue;
                }
                if (cdata[cnt].has_lay_hand() == 0)
                {
                    continue;
                }
                if (cdata[cnt].is_lay_hand_available() == 0)
                {
                    continue;
                }
                cdata[cnt].is_lay_hand_available() = false;
                txtef(9);
                txt(lang(
                    name(cnt)
                        + u8"は叫んだ。「この者にジュアの加護を。レイハンド！」"s,
                    name(cnt) + u8" shout"s + _s(cnt) + u8", "s
                        + u8"\"Lay hand!\""s));
                txt(lang(
                    name(prm_853) + u8"は回復した。"s,
                    name(prm_853) + u8" "s + is(prm_853) + u8" healed."s));
                cdata[prm_853].hp = cdata[prm_853].max_hp / 2;
                animode = 100 + prm_853;
                play_animation(19);
                snd(120);
                break;
            }
        }
        else if (cdata[prm_853].is_hung_on_sand_bag())
        {
            cdata[prm_853].hp = cdata[prm_853].max_hp;
        }
    }
    if (cdata[prm_853].hp >= 0)
    {
        if (dmglevel_at_m141 > 1)
        {
            spillblood(
                cdata[prm_853].position.x,
                cdata[prm_853].position.y,
                1 + rnd(2));
        }
        if (gdata(809) == 1)
        {
            txteledmg(0, c3_at_m141, prm_853, ele_at_m141);
            goto label_1369_internal;
        }
        if (dmglevel_at_m141 > 0)
        {
            if (cdata[prm_853].max_hp / 2 > cdata[prm_853].hp)
            {
                ++dmglevel_at_m141;
                if (cdata[prm_853].max_hp / 4 > cdata[prm_853].hp)
                {
                    ++dmglevel_at_m141;
                    if (cdata[prm_853].max_hp / 10 > cdata[prm_853].hp)
                    {
                        ++dmglevel_at_m141;
                    }
                }
            }
        }
        if (gdata(809) == 2)
        {
            txtcontinue();
            if (dmglevel_at_m141 == -1)
            {
                txt(lang(
                    u8"かすり傷をつけた。"s,
                    u8"make"s + _s(c3_at_m141) + u8" a scratch."s));
            }
            if (dmglevel_at_m141 == 0)
            {
                txtef(5);
                txt(lang(
                    u8"軽い傷を負わせた。"s,
                    u8"slightly wound"s + _s(c3_at_m141) + u8" "s + him(prm_853)
                        + u8"."s));
            }
            if (dmglevel_at_m141 == 1)
            {
                txtef(11);
                txt(lang(
                    u8"傷つけた。"s,
                    u8"moderately wound"s + _s(c3_at_m141) + u8" "s
                        + him(prm_853) + u8"."s));
            }
            if (dmglevel_at_m141 == 2)
            {
                txtef(10);
                txt(lang(
                    u8"深い傷を負わせた。"s,
                    u8"severely wound"s + _s(c3_at_m141) + u8" "s + him(prm_853)
                        + u8"."s));
            }
            if (dmglevel_at_m141 >= 3)
            {
                txtef(3);
                txt(lang(
                    u8"致命傷を与えた。"s,
                    u8"critically wound"s + _s(c3_at_m141) + u8" "s
                        + him(prm_853) + u8"!"s));
            }
            rowact_check(prm_853);
            goto label_1369_internal;
        }
        if (dmglevel_at_m141 == 1)
        {
            if (fov_player_sees(prm_853))
            {
                txtef(11);
                txt(lang(
                    name(prm_853) + u8"は痛手を負った。"s,
                    name(prm_853) + u8" scream"s + _s(prm_853) + u8"."s));
            }
        }
        if (dmglevel_at_m141 == 2)
        {
            if (fov_player_sees(prm_853))
            {
                txtef(10);
                txt(lang(
                    name(prm_853) + u8"は苦痛にもだえた。"s,
                    name(prm_853) + u8" writhe"s + _s(prm_853)
                        + u8" in pain."s));
            }
        }
        if (dmglevel_at_m141 >= 3)
        {
            if (fov_player_sees(prm_853))
            {
                txtef(3);
                txt(lang(
                    name(prm_853) + u8"は悲痛な叫び声をあげた。"s,
                    name(prm_853) + u8" "s + is(prm_853)
                        + u8" severely hurt!"s));
            }
        }
        if (dmg_at_m141 < 0)
        {
            if (cdata[prm_853].hp > cdata[prm_853].max_hp)
            {
                cdata[prm_853].hp = cdata[prm_853].max_hp;
            }
            if (fov_player_sees(prm_853))
            {
                txtef(4);
                txt(lang(
                    name(prm_853) + u8"は回復した。"s,
                    name(prm_853) + u8" "s + is(prm_853) + u8" healed."s));
            }
        }
    label_1369_internal:
        rowact_check(prm_853);
        if (cdata[prm_853].hp < cdata[prm_853].max_hp / 5)
        {
            if (prm_853 != 0)
            {
                if (cdata[prm_853].fear == 0)
                {
                    if (cdata[prm_853].is_immune_to_fear() == 0)
                    {
                        if (dmg_at_m141 * 100 / cdata[prm_853].max_hp + 10
                            > rnd(200))
                        {
                            f_at_m141 = 1;
                        }
                        else
                        {
                            f_at_m141 = 0;
                        }
                        if (prm_855 == 0)
                        {
                            if (trait(44))
                            {
                                f_at_m141 = 0;
                            }
                        }
                        if (f_at_m141)
                        {
                            cdata[prm_853].fear = rnd(20) + 5;
                            if (fov_player_sees(prm_853))
                            {
                                txtef(4);
                                txt(lang(
                                    name(prm_853) + u8"は恐怖して逃げ出した。"s,
                                    name(prm_853) + u8" run"s + _s(prm_853)
                                        + u8" away in terror."s));
                            }
                        }
                    }
                }
            }
        }
        if (ele_at_m141)
        {
            if (ele_at_m141 == 59)
            {
                if (rnd(10) < prm_857 / 75 + 4)
                {
                    dmgcon(prm_853, 4, rnd(prm_857 / 3 * 2 + 1));
                }
                if (rnd(20) < prm_857 / 50 + 4)
                {
                    dmgcon(prm_853, 3, rnd(prm_857 / 3 * 2 + 1));
                }
                if (rnd(20) < prm_857 / 50 + 4)
                {
                    dmgcon(prm_853, 5, rnd(prm_857 / 3 * 2 + 1));
                }
                if (rnd(20) < prm_857 / 50 + 4)
                {
                    dmgcon(prm_853, 1, rnd(prm_857 / 3 * 2 + 1));
                }
                if (rnd(20) < prm_857 / 50 + 4)
                {
                    dmgcon(prm_853, 2, rnd(prm_857 / 3 * 2 + 1));
                }
            }
            if (ele_at_m141 == 52)
            {
                if (rnd(3 + (cdata[prm_853].quality >= 4) * 3) == 0)
                {
                    ++cdata[prm_853].paralyzed;
                }
            }
            if (ele_at_m141 == 53)
            {
                dmgcon(prm_853, 4, rnd(prm_857 + 1));
            }
            if (ele_at_m141 == 58)
            {
                dmgcon(prm_853, 3, rnd(prm_857 + 1));
            }
            if (ele_at_m141 == 54)
            {
                dmgcon(prm_853, 5, rnd(prm_857 + 1));
            }
            if (ele_at_m141 == 57)
            {
                dmgcon(prm_853, 5, rnd(prm_857 + 1));
            }
            if (ele_at_m141 == 55)
            {
                dmgcon(prm_853, 1, rnd(prm_857 + 1));
            }
            if (ele_at_m141 == 61)
            {
                dmgcon(prm_853, 9, rnd(prm_857 + 1));
            }
            if (ele_at_m141 == 62)
            {
                if (prm_853 == 0)
                {
                    modcorrupt(rnd(prm_857 + 1));
                }
            }
            if (ele_at_m141 == 63)
            {
                if (prm_853 == 0 || rnd(3) == 0)
                {
                    item_acid(prm_853, -1);
                }
            }
        }
        if ((ele_at_m141 == 50 || prm_855 == -9) && cdata[prm_853].wet == 0)
        {
            item_fire(prm_853, -1);
        }
        if (ele_at_m141 == 51)
        {
            item_cold(prm_853, -1);
        }
        if (cdata[prm_853].sleep != 0)
        {
            if (ele_at_m141 != 54 && ele_at_m141 != 58 && ele_at_m141 != 59)
            {
                cdata[prm_853].sleep = 0;
                txt(lang(
                    name(prm_853) + u8"は眠りを妨げられた。"s,
                    name(prm_853) + your(prm_853) + u8" sleep "s + is(prm_853)
                        + u8" disturbed."s));
            }
        }
        if (prm_855 == 0)
        {
            hostileaction(0, prm_853);
            gdata(94) = prm_853;
        }
        if (prm_853 == 0)
        {
            if (cdata[prm_853].max_hp / 4 > cdata[prm_853].hp)
            {
                if (config::instance().sound)
                {
                    if (config::instance().heart == 1)
                    {
                        if (!CHECKPLAY(32))
                        {
                            snd(32);
                        }
                    }
                }
            }
        }
        if (cdata[prm_853].explodes())
        {
            if (rnd(3) == 0)
            {
                cdata[prm_853].will_explode_soon() = true;
                txtef(9);
                txt(lang(u8" *カチッ* "s, u8"*click*"s));
            }
        }
        if (cdata[prm_853].splits())
        {
            if (gdata(809) != 1)
            {
                if (dmg_at_m141 > cdata[prm_853].max_hp / 20 || rnd(10) == 0)
                {
                    if (mdata(6) != 1)
                    {
                        int stat = chara_copy(prm_853);
                        if (stat == 1)
                        {
                            txt(lang(
                                name(prm_853) + u8"は分裂した！"s,
                                name(prm_853) + u8" split"s + _s(prm_853)
                                    + u8"!"s));
                        }
                    }
                }
            }
        }
        if (cdata[prm_853].splits2())
        {
            if (gdata(809) != 1)
            {
                if (rnd(3) == 0)
                {
                    if (cdata[prm_853].confused == 0
                        && cdata[prm_853].dimmed == 0
                        && cdata[prm_853].poisoned == 0
                        && cdata[prm_853].paralyzed == 0
                        && cdata[prm_853].blind == 0)
                    {
                        if (mdata(6) != 1)
                        {
                            int stat = chara_copy(prm_853);
                            if (stat == 1)
                            {
                                txt(lang(
                                    name(prm_853) + u8"は分裂した！"s,
                                    name(prm_853) + u8" split"s + _s(prm_853)
                                        + u8"!"s));
                            }
                        }
                    }
                }
            }
        }
        if (cdata[prm_853].is_quick_tempered())
        {
            if (gdata(809) != 1)
            {
                if (cdata[prm_853].furious == 0)
                {
                    if (rnd(20) == 0)
                    {
                        if (fov_player_sees(prm_853))
                        {
                            txtef(4);
                            txt(lang(
                                name(prm_853) + u8"は怒りに体を奮わせた！"s,
                                name(prm_853) + u8" "s + is(prm_853)
                                    + u8" engulfed in fury!"s));
                        }
                        cdata[prm_853].furious += rnd(30) + 15;
                    }
                }
            }
        }
        if (prm_855 >= 0)
        {
            f_at_m141 = 0;
            if (cdata[prm_853].relationship <= -3)
            {
                if (cdata[prm_855].original_relationship > -3)
                {
                    if (cdata[prm_853].hate == 0 || rnd(4) == 0)
                    {
                        f_at_m141 = 1;
                    }
                }
            }
            else if (cdata[prm_855].original_relationship <= -3)
            {
                if (cdata[prm_853].hate == 0 || rnd(4) == 0)
                {
                    f_at_m141 = 1;
                }
            }
            if (prm_855 != 0)
            {
                if (cdata[prm_855].enemy_id == prm_853)
                {
                    if (rnd(3) == 0)
                    {
                        f_at_m141 = 1;
                    }
                }
            }
            if (f_at_m141)
            {
                if (prm_853 != 0)
                {
                    cdata[prm_853].enemy_id = prm_855;
                    if (cdata[prm_853].hate == 0)
                    {
                        cdata[prm_853].emotion_icon = 218;
                        cdata[prm_853].hate = 20;
                    }
                    else
                    {
                        cdata[prm_853].hate += 2;
                    }
                }
            }
        }
    }
    if (cdata[prm_853].hp < 0)
    {
        se_at_m141 = eleinfo(ele_at_m141, 1);
        if (se_at_m141)
        {
            snd(se_at_m141, false, false);
        }
        txtef(3);
        if (prm_855 >= 0)
        {
            if (ele_at_m141)
            {
                if (prm_853 >= 16 && gdata(809) == 2)
                {
                    txtcontinue();
                    txteledmg(1, c3_at_m141, prm_853, ele_at_m141);
                }
                else
                {
                    txteledmg(2, c3_at_m141, prm_853, ele_at_m141);
                }
            }
            else
            {
                p_at_m141 = rnd(4);
                if (p_at_m141 == 0)
                {
                    if (prm_853 >= 16 && gdata(809) == 2)
                    {
                        txtcontinue();
                        txt(lang(
                            u8"粉々の肉片に変えた。"s,
                            u8"transform"s + _s(c3_at_m141) + u8" "s
                                + him(prm_853)
                                + u8" into several pieces of meat."s));
                    }
                    else
                    {
                        txt(lang(
                            name(prm_853) + u8"は粉々の肉片に変えられた。"s,
                            name(prm_853) + u8" "s + is(prm_853)
                                + u8" transformed into several pieces of meat."s));
                    }
                }
                if (p_at_m141 == 1)
                {
                    if (prm_853 >= 16 && gdata(809) == 2)
                    {
                        txtcontinue();
                        txt(lang(
                            u8"破壊した。"s,
                            u8"destroy"s + _s(c3_at_m141) + u8" "s
                                + him(prm_853) + u8"."s));
                    }
                    else
                    {
                        txt(lang(
                            name(prm_853) + u8"は破壊された。"s,
                            name(prm_853) + u8" "s + is(prm_853)
                                + u8" killed."s));
                    }
                }
                if (p_at_m141 == 2)
                {
                    if (prm_853 >= 16 && gdata(809) == 2)
                    {
                        txtcontinue();
                        txt(lang(
                            u8"ミンチにした。"s,
                            u8"mince"s + _s(c3_at_m141) + u8" "s + him(prm_853)
                                + u8"."s));
                    }
                    else
                    {
                        txt(lang(
                            name(prm_853) + u8"はミンチにされた。"s,
                            name(prm_853) + u8" "s + is(prm_853)
                                + u8" minced."s));
                    }
                }
                if (p_at_m141 == 3)
                {
                    if (prm_853 >= 16 && gdata(809) == 2)
                    {
                        txtcontinue();
                        txt(lang(
                            u8"殺した。"s,
                            u8"kill"s + _s(c3_at_m141) + u8" "s + him(prm_853)
                                + u8"."s));
                    }
                    else
                    {
                        txt(lang(
                            name(prm_853) + u8"は殺された。"s,
                            name(prm_853) + u8" "s + is(prm_853)
                                + u8" slain."s));
                    }
                }
            }
            ndeathcause = lang(
                cdatan(0, cc) + u8"に殺された。"s,
                u8"was killed by "s + cdatan(0, cc));
        }
        else
        {
            if (prm_855 == -11)
            {
                txt(lang(
                    name(prm_853) + u8"は見えざる手に葬られた。"s,
                    name(prm_853) + u8" "s + is(prm_853)
                        + u8" assassinated by the unseen hand."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"見えざる手に葬られた。"s,
                        u8"got assassinated by the unseen hand"s);
                }
            }
            if (prm_855 == -1)
            {
                txt(lang(
                    name(prm_853) + u8"は罠にかかって死んだ。"s,
                    name(prm_853) + u8" "s + is(prm_853)
                        + u8" caught in a trap and die"s + _s(prm_853)
                        + u8"."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"罠にかかって死んだ。"s,
                        u8"got caught in a trap and died"s);
                }
            }
            if (prm_855 == -2)
            {
                txt(lang(
                    name(prm_853) + u8"はマナの反動で死んだ。"s,
                    name(prm_853) + u8" die"s + _s(prm_853)
                        + u8" from over-casting."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"マナの反動で消滅した。"s,
                        u8"was completely wiped by magic reaction"s);
                }
            }
            if (prm_855 == -3)
            {
                txt(lang(
                    name(prm_853) + u8"は餓死した。"s,
                    name(prm_853) + u8" "s + is(prm_853)
                        + u8" starved to death."s));
                if (prm_853 == 0)
                {
                    ndeathcause =
                        lang(u8"飢え死にした。"s, u8"was starved to death"s);
                }
            }
            if (prm_855 == -4)
            {
                txt(lang(
                    name(prm_853) + u8"は毒に蝕まれ死んだ。"s,
                    name(prm_853) + u8" "s + is(prm_853)
                        + u8" killed with poison."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"毒にもがき苦しみながら死んだ。"s,
                        u8"miserably died from poison"s);
                }
            }
            if (prm_855 == -13)
            {
                txt(lang(
                    name(prm_853) + u8"は出血多量で死んだ。"s,
                    name(prm_853) + u8" die"s + _s(prm_853)
                        + u8" from loss of blood"s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"血を流しすぎて死んだ。"s,
                        u8"died from loss of blood"s);
                }
            }
            if (prm_855 == -5)
            {
                txt(lang(
                    name(prm_853) + u8"は呪いの力で死んだ。"s,
                    name(prm_853) + u8" die"s + _s(prm_853)
                        + u8" from curse."s));
                if (prm_853 == 0)
                {
                    ndeathcause =
                        lang(u8"呪い殺された。"s, u8"died from curse"s);
                }
            }
            if (prm_855 == -7)
            {
                txt(lang(
                    name(prm_853) + u8"は階段から転げ落ちて死んだ。"s,
                    name(prm_853) + u8" tumble"s + _s(prm_853)
                        + u8" from stairs and die"s + _s(prm_853) + u8"."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"階段から転げ落ちて亡くなった。"s,
                        u8"tumbled from stairs and died"s);
                }
            }
            if (prm_855 == -8)
            {
                txt(lang(
                    name(prm_853) + u8"は聴衆に殺された。"s,
                    name(prm_853) + u8" "s + is(prm_853)
                        + u8" killed by an audience."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"演奏中に激怒した聴衆に殺された。"s,
                        u8"was killed by an audience"s);
                }
            }
            if (prm_855 == -9)
            {
                txt(lang(
                    name(prm_853) + u8"は焼け死んだ。"s,
                    name(prm_853) + u8" "s + is(prm_853)
                        + u8" burnt and turned into ash."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"焼けて消滅した。"s,
                        u8"was burnt and turned into ash"s);
                }
            }
            if (prm_855 == -12)
            {
                txt(lang(
                    name(prm_853) + u8"は食中毒で死んだ。"s,
                    name(prm_853) + u8" "s + is(prm_853)
                        + u8" killed by food poisoning."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"食中毒で倒れた。"s,
                        u8"got killed by food poisoning"s);
                }
            }
            if (prm_855 == -14)
            {
                txt(lang(
                    name(prm_853) + u8"はエーテルに侵食され死んだ。"s,
                    name(prm_853) + u8" die"s + _s(prm_853)
                        + u8" of the Ether disease."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"エーテルの病に倒れた。"s,
                        u8"died of the Ether disease"s);
                }
            }
            if (prm_855 == -15)
            {
                txt(lang(
                    name(prm_853) + u8"は溶けて液体になった。"s,
                    name(prm_853) + u8" melt"s + _s(prm_853) + u8" down."s));
                if (prm_853 == 0)
                {
                    ndeathcause =
                        lang(u8"溶けて液体になった。"s, u8"melted down"s);
                }
            }
            if (prm_855 == -16)
            {
                txt(lang(
                    name(prm_853) + u8"はバラバラになった。"s,
                    name(prm_853) + u8" shatter"s + _s(prm_853) + u8"."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(u8"自殺した。"s, u8"committed suicide"s);
                }
            }
            if (prm_855 == -17)
            {
                txt(lang(
                    name(prm_853) + u8"は核爆発に巻き込まれて塵となった。"s,
                    name(prm_853) + u8" "s + is(prm_853)
                        + u8" turned into atoms."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"核爆発に巻き込まれて死んだ。"s,
                        u8"was killed by an atomic bomb"s);
                }
            }
            if (prm_855 == -18)
            {
                txt(lang(
                    name(prm_853)
                        + u8"はアイアンメイデンの中で串刺しになって果てた。"s,
                    name(prm_853) + u8" step"s + _s(prm_853)
                        + u8" in an iron maiden and die"s + _s(prm_853)
                        + u8"."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"アイアンメイデンにはさまれて死んだ。"s,
                        u8"stepped in an iron maiden and died"s);
                }
            }
            if (prm_855 == -19)
            {
                txt(lang(
                    name(prm_853) + u8"はギロチンで首をちょんぎられて死んだ。"s,
                    name(prm_853) + u8" "s + is(prm_853)
                        + u8" guillotined and die"s + _s(prm_853) + u8"."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"ギロチンで首を落とされて死んだ。"s,
                        u8"was guillotined"s);
                }
            }
            if (prm_855 == -20)
            {
                txt(lang(
                    name(prm_853) + u8"は首を吊った。"s,
                    name(prm_853) + u8" hang"s + _s(prm_853) + u8" "s
                        + his(prm_853) + u8"self."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"首を吊った。"s, u8"commited suicide by hanging"s);
                }
            }
            if (prm_855 == -21)
            {
                txt(lang(
                    name(prm_853) + u8"はもちを喉に詰まらせて死んだ。"s,
                    name(prm_853) + u8" choke"s + _s(prm_853)
                        + u8" on mochi and die."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        u8"もちを喉に詰まらせて死んだ。"s,
                        u8"ate mochi and died"s);
                }
            }
            if (prm_855 == -6)
            {
                p_at_m141(0) = -1;
                p_at_m141(1) = 0;
                for (const auto& cnt : items(0))
                {
                    if (inv[cnt].number == 0)
                    {
                        continue;
                    }
                    if (inv[cnt].weight > p_at_m141(1))
                    {
                        p_at_m141(0) = cnt;
                        p_at_m141(1) = inv[cnt].weight;
                    }
                }
                if (p_at_m141 == -1)
                {
                    rtvaln = lang(u8"荷物"s, u8"backpack"s);
                }
                else
                {
                    rtvaln = itemname(p_at_m141);
                }
                txt(lang(
                    name(prm_853) + u8"は"s + rtvaln
                        + u8"の重さに耐え切れず死んだ。"s,
                    name(prm_853) + u8" "s + is(prm_853) + u8" squashed by "s
                        + rtvaln + u8"."s));
                if (prm_853 == 0)
                {
                    ndeathcause = lang(
                        rtvaln + u8"の重さに耐え切れず潰れた。"s,
                        u8"was squashed by "s + rtvaln);
                }
            }
        }
        if (prm_855 == -9 || ele_at_m141 == 50)
        {
            mef_add(
                cdata[prm_853].position.x,
                cdata[prm_853].position.y,
                5,
                24,
                rnd(10) + 5,
                100,
                prm_855);
        }
        if (ele_at_m141 == 56)
        {
            if (prm_855 >= 0)
            {
                if (dmg_at_m141 > 0)
                {
                    healhp(cc, rnd(dmg_at_m141 * (200 + prm_857) / 1000 + 5));
                }
            }
        }
        if (gdata_mount != prm_853 || prm_853 == 0)
        {
            cell_removechara(
                cdata[prm_853].position.x, cdata[prm_853].position.y);
        }
        if (cdata[prm_853].breaks_into_debris())
        {
            if (fov_player_sees(prm_853))
            {
                x = cdata[prm_853].position.x;
                y = cdata[prm_853].position.y;
                snd(45, false, false);
                animeblood(prm_853, 1, ele_at_m141);
            }
            spillfrag(cdata[prm_853].position.x, cdata[prm_853].position.y, 3);
        }
        else
        {
            snd(8 + rnd(2), false, false);
            animeblood(prm_853, 0, ele_at_m141);
            spillblood(cdata[prm_853].position.x, cdata[prm_853].position.y, 4);
        }
        if (cdata[prm_853].character_role == 0)
        {
            cdata[prm_853].state = 0;
        }
        else if (cdata[prm_853].character_role == 13)
        {
            cdata[prm_853].state = 4;
            cdata[prm_853].time_to_revive = gdata_hour + gdata_day * 24
                + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12 + 24
                + rnd(12);
        }
        else
        {
            cdata[prm_853].state = 2;
            cdata[prm_853].time_to_revive = gdata_hour + gdata_day * 24
                + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12 + 48;
        }
        if (prm_853 != 0)
        {
            if (prm_853 < 16)
            {
                chara_mod_impression(prm_853, -10);
                cdata[prm_853].state = 6;
                cdata[prm_853].current_map = 0;
                if (cdata[prm_853].is_escorted() == 1)
                {
                    event_add(15, cdata[prm_853].id);
                    cdata[prm_853].state = 0;
                }
                if (cdata[prm_853].is_escorted_in_sub_quest() == 1)
                {
                    cdata[prm_853].state = 0;
                }
            }
        }
        if (prm_853 == 0)
        {
            ++gdata_death_count;
        }
        if (prm_853 == gdata(94))
        {
            gdata(94) = 0;
        }
        if (prm_855 >= 0)
        {
            if (prm_855 != 0)
            {
                chara_custom_talk(prm_855, 103);
            }
            exp_at_m141 = clamp(cdata[prm_853].level, 1, 200)
                    * clamp((cdata[prm_853].level + 1), 1, 200)
                    * clamp((cdata[prm_853].level + 2), 1, 200) / 20
                + 8;
            if (cdata[prm_853].level > cdata[prm_855].level)
            {
                exp_at_m141 /= 4;
            }
            if (cdata[prm_853].splits() || cdata[prm_853].splits2())
            {
                exp_at_m141 /= 20;
            }
            cdata[prm_855].experience += exp_at_m141;
            if (prm_855 == 0)
            {
                gdata_sleep_experience += exp_at_m141;
            }
            cdata[prm_855].hate = 0;
            if (prm_855 < 16)
            {
                cdata[prm_855].enemy_id = 0;
                cdata[0].enemy_id = 0;
                gdata(94) = 0;
            }
        }
        if (prm_853 != 0)
        {
            if (gdata_current_map != 35)
            {
                if (gdata_current_map != 42)
                {
                    if (cdata[prm_853].id == 2)
                    {
                        event_add(1);
                    }
                    if (cdata[prm_853].id == 141)
                    {
                        txtef(2);
                        txt(lang(
                            u8"愚者の魔石を手に入れた！"s,
                            u8"You obtain the fool's magic stone!"s));
                        snd(51);
                        gdata_magic_stone_of_fool = 1;
                    }
                    if (cdata[prm_853].id == 143)
                    {
                        txtef(2);
                        txt(lang(
                            u8"覇者の魔石を手に入れた！"s,
                            u8"You obtain the king's magic stone!"s));
                        snd(51);
                        gdata_magic_stone_of_king = 1;
                    }
                    if (cdata[prm_853].id == 144)
                    {
                        txtef(2);
                        txt(lang(
                            u8"賢者の魔石を手に入れた！"s,
                            u8"You obtain the sage's magic stone!"s));
                        snd(51);
                        gdata_magic_stone_of_sage = 1;
                    }
                    if (cdata[prm_853].id == 242)
                    {
                        if (gdata_novice_knight < 1000)
                        {
                            gdata_novice_knight = 2;
                            snd(44);
                            txtef(2);
                            txt(lang(
                                u8"ジャーナルが更新された。"s,
                                u8"Your journal has been updated."s));
                        }
                    }
                    if (cdata[prm_853].id == 257)
                    {
                        if (gdata_pyramid_trial < 1000)
                        {
                            gdata_pyramid_trial = 1000;
                            snd(44);
                            txtef(2);
                            txt(lang(
                                u8"ジャーナルが更新された。"s,
                                u8"Your journal has been updated."s));
                            txt(lang(
                                u8"クエストを達成した！"s,
                                u8"You have completed the quest!"s));
                            snd(51);
                        }
                    }
                    if (cdata[prm_853].id == 300)
                    {
                        if (gdata_minotaur_king < 1000)
                        {
                            gdata_minotaur_king = 2;
                            snd(44);
                            txtef(2);
                            txt(lang(
                                u8"ジャーナルが更新された。"s,
                                u8"Your journal has been updated."s));
                        }
                    }
                    if (cdata[prm_853].id == 318)
                    {
                        event_add(
                            27,
                            cdata[prm_853].position.x,
                            cdata[prm_853].position.y);
                    }
                    if (cdata[prm_853].id == 319)
                    {
                        ++gdata_kill_count_of_little_sister;
                        txtef(3);
                        txt(lang(
                            u8"リトルシスター殺害"s
                                + gdata_kill_count_of_little_sister
                                + u8"回、救出"s
                                + gdata_save_count_of_little_sister + u8"回。"s,
                            u8"You have saved little sister "s
                                + gdata_save_count_of_little_sister
                                + u8" times and killed "s
                                + gdata_kill_count_of_little_sister
                                + u8" times."s));
                    }
                    if (gdata_current_dungeon_level
                            == adata(10, gdata_current_map)
                        || gdata_current_map == 42)
                    {
                        if (adata(20, gdata_current_map) == prm_853
                            && cdata[prm_853].is_lord_of_dungeon() == 1)
                        {
                            event_add(5);
                        }
                    }
                    if (cdata[prm_853].id == 331)
                    {
                        if (rnd(4) == 0)
                        {
                            event_add(
                                28,
                                cdata[prm_853].position.x,
                                cdata[prm_853].position.y);
                        }
                    }
                    quest_check();
                }
                else if (gdata_current_map == 42)
                {
                    if (adata(20, gdata_current_map) == prm_853
                        && cdata[prm_853].is_lord_of_dungeon() == 1)
                    {
                        event_add(5);
                    }
                }
            }
        }
        if (prm_853 != 0)
        {
            ++npcmemory(0, cdata[prm_853].id);
            chara_custom_talk(prm_853, 102);
            if (prm_853 < 16)
            {
                txt(lang(
                    u8"あなたは悲しくなった。"s,
                    u8"You feel sad for a moment."s));
            }
        }
        --gdata_other_character_count;
        if (gdata_mount)
        {
            if (prm_853 == gdata_mount)
            {
                txt(lang(
                    name(0) + u8"は"s + name(prm_853) + u8"の死体から降りた。"s,
                    name(0) + u8" get off the corpse of "s + name(prm_853)
                        + u8"."s));
                ride_end();
            }
        }
        check_kill(prm_855, prm_853);
        catitem = 0;
        rollanatomy = 0;
        if (rnd(60) == 0)
        {
            rollanatomy = 1;
        }
        if (prm_855 >= 0)
        {
            if (cdata[prm_855].id == 260)
            {
                catitem = prm_855;
            }
            if (int(std::sqrt(sdata(161, prm_855))) > rnd(150))
            {
                rollanatomy = 1;
            }
            skillexp(161, prm_855, 10 + rollanatomy * 4);
        }
        rc = prm_853;
        label_1573();
        if (gdata_current_map == 40)
        {
            if (rnd(5) == 0)
            {
                snd(69);
            }
        }
        if (cdata[prm_853].is_death_master() == 1)
        {
            txt(lang(
                u8"死の宣告は無効になった。"s, u8"The death word breaks."s));
            for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
            {
                if (cdata[cnt].state != 1)
                {
                    continue;
                }
                p_at_m141 = cnt;
                for (int cnt = 0; cnt < 16; ++cnt)
                {
                    if (cdata[p_at_m141].buffs[cnt].id == 0)
                    {
                        break;
                    }
                    if (cdata[p_at_m141].buffs[cnt].id == 16)
                    {
                        delbuff(p_at_m141, cnt);
                        --cnt;
                        continue;
                    }
                }
            }
        }
        if (prm_855 == 0)
        {
            if (gdata_catches_god_signal)
            {
                if (rnd(20) == 0)
                {
                    txtgod(cdata[0].god_id, 9);
                }
            }
        }
        end_dmghp();
        return 0;
    }
    end_dmghp();
    return 1;
}



void end_dmghp()
{
    if (cdata[prm_853].is_hung_on_sand_bag())
    {
        if (fov_player_sees(prm_853))
        {
            txt(u8"("s + dmg_at_m141 + u8")"s + lang(u8" "s, ""s));
            if (rnd(20) == 0)
            {
                txt(lang(u8"「くっ！」"s, u8"\"Kill me already!\""s),
                    lang(u8"「まだまだ！」"s, u8"\"No..not yet...!\""s),
                    lang(
                        u8"「もう限界…」"s,
                        u8"\"I can't take it anymore...\""s),
                    lang(u8"「うぐぐ」"s, u8"\"Argh!\""s),
                    lang(u8"「あう」"s, u8"\"Uhhh\""s),
                    lang(u8"「ああんっ」"s, u8"\"Ugggg\""s));
            }
        }
    }
    gdata(809) = 0;
    txt3rd = 0;
    return;
}



void dmgmp(int cc, int delta)
{
    cdata[cc].mp -= delta;
    if (cdata[cc].mp < -999999)
    {
        cdata[cc].mp = -999999;
    }
    if (cdata[cc].mp < 0)
    {
        gain_skill_experience_mana_capacity(cc);
        int damage = -cdata[cc].mp * 400 / (100 + sdata(164, cc) * 10);
        if (cc == 0)
        {
            if (trait(156) == 1)
            {
                damage /= 2;
            }
        }
        else
        {
            damage /= 5;
            if (damage < 10)
                return;
        }
        txt(lang(
            u8"マナの反動が"s + name(cc) + u8"の精神を蝕んだ！"s,
            u8"Magic reaction hurts "s + name(cc) + u8"!"s));
        dmghp(cc, damage, -2);
    }
}



void dmgsp(int cc, int delta)
{
    if (cc != 0 || debug::voldemort)
        return;

    if (cdata[cc].sp >= -100)
    {
        cdata[cc].sp -= delta;
    }
}



void healsan(int prm_862, int prm_863)
{
    cdata[prm_862].insanity -= prm_863;
    if (cdata[prm_862].insanity < 0)
    {
        cdata[prm_862].insanity = 0;
    }
    return;
}



void damage_insanity(int cc, int delta)
{
    if (cdata[cc].quality >= 4)
        return;

    int resistance = std::max(sdata(54, cc) / 50, 1);
    if (resistance > 10)
        return;

    delta /= resistance;
    if (cc < 16)
    {
        if (trait(166))
        {
            delta -= rnd(4);
        }
    }
    delta = std::max(delta, 0);
    cdata[cc].insanity += delta;
    if (rnd(10) == 0 || rnd(delta + 1) > 5 || rnd(cdata[cc].insanity + 1) > 50)
    {
        dmgcon(cc, 11, 100);
    }
}



bool actionsp(int cc, int sp)
{
    if (cc != 0 || debug::voldemort)
        return true;

    if (cdata[cc].sp < 50 && cdata[cc].sp < rnd(75))
    {
        dmgsp(cc, sp);
        return false;
    }
    dmgsp(cc, sp);
    return true;
}


int dmgcon(int prm_818, int prm_819, int prm_820)
{
    elona::prm_818 = prm_818;
    elona::prm_820 = prm_820;
    if (prm_820 < 1)
    {
        f_at_con = 0;
    }
    else
    {
        f_at_con = 1;
    }
    if (prm_819 == 4)
    {
        if (cdata[prm_818].is_immune_to_blindness() == 1)
        {
            f_at_con = 0;
        }
        if (cdata[prm_818].quality > 3)
        {
            if (rnd(cdata[prm_818].level / 2 + 1))
            {
                f_at_con = 0;
            }
        }
        res_at_con = 53;
        label_1346();
        if (f_at_con == 0)
        {
            return 0;
        }
        p_at_con = p_at_con / 6;
        if (p_at_con > 0)
        {
            if (cdata[prm_818].blind == 0)
            {
                cdata[prm_818].blind = p_at_con;
                if (fov_player_sees(prm_818))
                {
                    txtef(8);
                    txt(lang(
                        name(prm_818) + u8"は盲目になった。"s,
                        name(prm_818) + u8" "s + is(prm_818) + u8" blinded."s));
                }
            }
            else
            {
                cdata[prm_818].blind += p_at_con / 3 + 1;
            }
            rowactend(prm_818);
        }
        return 1;
    }
    if (prm_819 == 5)
    {
        if (cdata[prm_818].is_immune_to_confusion() == 1)
        {
            f_at_con = 0;
        }
        if (findbuff(prm_818, 7) != -1)
        {
            f_at_con = 0;
        }
        if (cdata[prm_818].quality > 3)
        {
            if (rnd(cdata[prm_818].level / 2 + 1))
            {
                f_at_con = 0;
            }
        }
        res_at_con = 54;
        label_1346();
        if (f_at_con == 0)
        {
            return 0;
        }
        p_at_con = p_at_con / 7;
        if (p_at_con > 0)
        {
            if (cdata[prm_818].confused == 0)
            {
                cdata[prm_818].confused = p_at_con;
                if (fov_player_sees(prm_818))
                {
                    txtef(8);
                    txt(lang(
                        name(prm_818) + u8"は混乱した。"s,
                        name(prm_818) + u8" "s + is(prm_818)
                            + u8" confused."s));
                }
            }
            else
            {
                cdata[prm_818].confused += p_at_con / 3 + 1;
            }
            rowactend(prm_818);
        }
        return 1;
    }
    if (prm_819 == 3)
    {
        if (cdata[prm_818].is_immune_to_paralyzation() == 1)
        {
            f_at_con = 0;
        }
        if (cdata[prm_818].quality > 3)
        {
            if (rnd(cdata[prm_818].level + 1))
            {
                f_at_con = 0;
            }
        }
        res_at_con = 58;
        label_1346();
        if (f_at_con == 0)
        {
            return 0;
        }
        p_at_con = p_at_con / 10;
        if (p_at_con > 0)
        {
            if (cdata[prm_818].paralyzed == 0)
            {
                cdata[prm_818].paralyzed = p_at_con;
                if (fov_player_sees(prm_818))
                {
                    txtef(8);
                    txt(lang(
                        name(prm_818) + u8"は麻痺した。"s,
                        name(prm_818) + u8" "s + is(prm_818)
                            + u8" paralyzed."s));
                }
            }
            else
            {
                cdata[prm_818].paralyzed += p_at_con / 3 + 1;
            }
            rowactend(prm_818);
        }
        return 1;
    }
    if (prm_819 == 1)
    {
        if (cdata[prm_818].is_immune_to_poison() == 1)
        {
            f_at_con = 0;
        }
        if (cdata[prm_818].quality > 3)
        {
            if (rnd(cdata[prm_818].level / 3 + 1))
            {
                f_at_con = 0;
            }
        }
        res_at_con = 55;
        label_1346();
        if (f_at_con == 0)
        {
            return 0;
        }
        p_at_con = p_at_con / 5;
        if (p_at_con > 0)
        {
            if (cdata[prm_818].poisoned == 0)
            {
                cdata[prm_818].poisoned = p_at_con;
                if (fov_player_sees(prm_818))
                {
                    txtef(8);
                    txt(lang(
                        name(prm_818) + u8"は毒におかされた。"s,
                        name(prm_818) + u8" "s + is(prm_818)
                            + u8" poisoned."s));
                }
            }
            else
            {
                cdata[prm_818].poisoned += p_at_con / 3 + 3;
            }
            rowactend(prm_818);
        }
        return 1;
    }
    if (prm_819 == 2)
    {
        if (cdata[prm_818].is_immune_to_sleep() == 1)
        {
            f_at_con = 0;
        }
        if (cdata[prm_818].quality > 3)
        {
            if (rnd(cdata[prm_818].level / 5 + 1))
            {
                f_at_con = 0;
            }
        }
        res_at_con = 58;
        label_1346();
        if (f_at_con == 0)
        {
            return 0;
        }
        p_at_con = p_at_con / 4;
        if (p_at_con > 0)
        {
            if (cdata[prm_818].sleep == 0)
            {
                cdata[prm_818].sleep = p_at_con;
                if (fov_player_sees(prm_818))
                {
                    txtef(8);
                    txt(lang(
                        name(prm_818) + u8"は眠りにおちた。"s,
                        name(prm_818) + u8" fall"s + _s(prm_818)
                            + u8" asleep."s));
                }
            }
            else
            {
                cdata[prm_818].sleep += p_at_con / 3 + 1;
            }
            rowactend(prm_818);
        }
        return 1;
    }
    if (prm_819 == 6)
    {
        if (cdata[prm_818].is_immune_to_fear() == 1)
        {
            f_at_con = 0;
        }
        if (findbuff(prm_818, 1) != -1)
        {
            f_at_con = 0;
        }
        if (findbuff(prm_818, 7) != -1)
        {
            f_at_con = 0;
        }
        if (cdata[prm_818].quality > 3)
        {
            if (rnd(cdata[prm_818].level / 5 + 1))
            {
                f_at_con = 0;
            }
        }
        res_at_con = 54;
        label_1346();
        if (f_at_con == 0)
        {
            return 0;
        }
        p_at_con = p_at_con / 7;
        if (p_at_con > 0)
        {
            if (cdata[prm_818].fear == 0)
            {
                cdata[prm_818].fear = p_at_con;
            }
            if (fov_player_sees(prm_818))
            {
                txt(lang(
                    name(prm_818) + u8"は恐怖に侵された。"s,
                    name(prm_818) + u8" "s + is(prm_818) + u8" frightened."s));
            }
        }
        return 1;
    }
    if (prm_819 == 7)
    {
        if (cdata[prm_818].quality > 3)
        {
            if (rnd(cdata[prm_818].level / 3 + 1))
            {
                f_at_con = 0;
            }
        }
        if (cdatan(2, prm_818) == u8"golem"s)
        {
            f_at_con = 0;
        }
        res_at_con = 57;
        label_1346();
        if (f_at_con == 0)
        {
            return 0;
        }
        p_at_con = p_at_con / 8;
        if (p_at_con > 0)
        {
            if (cdata[prm_818].dimmed == 0)
            {
                cdata[prm_818].dimmed = p_at_con;
                if (fov_player_sees(prm_818))
                {
                    txtef(8);
                    txt(lang(
                        name(prm_818) + u8"は朦朧とした。"s,
                        name(prm_818) + u8" "s + is(prm_818) + u8" dimmed."s));
                }
            }
            else
            {
                cdata[prm_818].dimmed += p_at_con / 3 + 1;
            }
            rowactend(prm_818);
        }
        return 1;
    }
    if (prm_819 == 9)
    {
        p_at_con = prm_820 / 25;
        if (cdata[prm_818].quality > 3)
        {
            p_at_con /= 2;
        }
        if (p_at_con > 0)
        {
            if (cdata[prm_818].bleeding == 0)
            {
                cdata[prm_818].bleeding = p_at_con;
                if (fov_player_sees(prm_818))
                {
                    txtef(8);
                    txt(lang(
                        name(prm_818) + u8"は血を流し始めた。"s,
                        name(prm_818) + u8" begin"s + _s(prm_818)
                            + u8" to bleed."s));
                }
            }
            else
            {
                cdata[prm_818].bleeding += p_at_con;
            }
            rowactend(prm_818);
        }
        return 1;
    }
    if (prm_819 == 8)
    {
        p_at_con = prm_820 / 10;
        if (p_at_con > 0)
        {
            if (cdata[prm_818].drunk == 0)
            {
                cdata[prm_818].drunk = p_at_con;
                if (fov_player_sees(prm_818))
                {
                    txt(lang(
                        name(prm_818) + u8"は酔っ払った。"s,
                        name(prm_818) + u8" get"s + _s(prm_818)
                            + u8" drunk."s));
                }
            }
            else
            {
                cdata[prm_818].drunk += p_at_con;
            }
        }
        return 1;
    }
    if (prm_819 == 11)
    {
        p_at_con = prm_820 / 8;
        if (p_at_con > 0)
        {
            if (cdata[prm_818].insane == 0)
            {
                cdata[prm_818].insane = p_at_con;
                if (fov_player_sees(prm_818))
                {
                    txtef(8);
                    txt(lang(
                        name(prm_818) + u8"は気が狂った。"s,
                        name(prm_818) + u8" become"s + _s(prm_818)
                            + u8" insane."s));
                }
            }
            else
            {
                cdata[prm_818].insane += p_at_con / 3 + 1;
            }
            rowactend(prm_818);
        }
        return 1;
    }
    if (prm_819 == 12)
    {
        p_at_con = prm_820 / 10;
        if (p_at_con > 0)
        {
            if (cdata[prm_818].sick == 0)
            {
                cdata[prm_818].sick = p_at_con;
                if (fov_player_sees(prm_818))
                {
                    txtef(8);
                    txt(lang(
                        name(prm_818) + u8"は病気になった。"s,
                        name(prm_818) + u8" get"s + _s(prm_818) + u8" sick."s));
                }
            }
            else
            {
                cdata[prm_818].sick += p_at_con / 10 + 1;
            }
        }
        return 1;
    }
    return 0;
}

void label_1346()
{
    int r_at_con = 0;
    p_at_con = rnd(prm_820 / 2 + 1) + prm_820 / 2;
    r_at_con = sdata(res_at_con, prm_818) / 50;
    p_at_con = p_at_con * 100 / (50 + r_at_con * 50);
    if (r_at_con >= 3)
    {
        if (p_at_con < 40)
        {
            f_at_con = 0;
        }
    }
    return;
}



void healhp(int cc, int delta)
{
    cdata[cc].hp += delta;
    if (cdata[cc].hp > cdata[cc].max_hp)
    {
        cdata[cc].hp = cdata[cc].max_hp;
    }
}



void healmp(int cc, int delta)
{
    cdata[cc].mp += delta;
    if (cdata[cc].mp > cdata[cc].max_mp)
    {
        cdata[cc].mp = cdata[cc].max_mp;
    }
}



void healsp(int cc, int delta)
{
    cdata[cc].sp += delta;
    if (cdata[cc].sp > cdata[cc].max_sp)
    {
        cdata[cc].sp = cdata[cc].max_sp;
    }
}



void healcon(int prm_827, int prm_828, int prm_829)
{
    if (prm_828 == 6)
    {
        if (cdata[prm_827].fear > 0)
        {
            if (prm_829 == 0)
            {
                cdata[prm_827].fear = 0;
            }
            else
            {
                cdata[prm_827].fear -= prm_829;
            }
            if (cdata[prm_827].fear <= 0)
            {
                cdata[prm_827].fear = 0;
                if (fov_player_sees(prm_827))
                {
                    txt(lang(
                        name(prm_827) + u8"は恐怖から立ち直った。"s,
                        name(prm_827) + u8" shake"s + _s(prm_827) + u8" off "s
                            + his(prm_827) + u8" fear."s));
                }
            }
            return;
        }
    }
    if (prm_828 == 4)
    {
        if (cdata[prm_827].blind > 0)
        {
            if (prm_829 == 0)
            {
                cdata[prm_827].blind = 0;
            }
            else
            {
                cdata[prm_827].blind -= prm_829;
            }
            if (cdata[prm_827].blind <= 0)
            {
                cdata[prm_827].blind = 0;
                if (fov_player_sees(prm_827))
                {
                    txt(lang(
                        name(prm_827) + u8"は盲目から回復した。"s,
                        name(prm_827) + u8" can see again."s));
                }
            }
            return;
        }
    }
    if (prm_828 == 5)
    {
        if (cdata[prm_827].confused > 0)
        {
            if (prm_829 == 0)
            {
                cdata[prm_827].confused = 0;
            }
            else
            {
                cdata[prm_827].confused -= prm_829;
            }
            if (cdata[prm_827].confused <= 0)
            {
                cdata[prm_827].confused = 0;
                if (fov_player_sees(prm_827))
                {
                    txt(lang(
                        name(prm_827) + u8"は混乱から回復した。"s,
                        name(prm_827) + u8" recover"s + _s(prm_827)
                            + u8" from confusion."s));
                }
            }
            return;
        }
    }
    if (prm_828 == 3)
    {
        if (cdata[prm_827].paralyzed > 0)
        {
            if (prm_829 == 0)
            {
                cdata[prm_827].paralyzed = 0;
            }
            else
            {
                cdata[prm_827].paralyzed -= prm_829;
            }
            if (cdata[prm_827].paralyzed <= 0)
            {
                cdata[prm_827].paralyzed = 0;
                if (fov_player_sees(prm_827))
                {
                    txt(lang(
                        name(prm_827) + u8"は麻痺から回復した。"s,
                        name(prm_827) + u8" recover"s + _s(prm_827)
                            + u8" from paralysis."s));
                }
            }
            return;
        }
    }
    if (prm_828 == 1)
    {
        if (cdata[prm_827].poisoned > 0)
        {
            if (prm_829 == 0)
            {
                cdata[prm_827].poisoned = 0;
            }
            else
            {
                cdata[prm_827].poisoned -= prm_829;
            }
            if (cdata[prm_827].poisoned <= 0)
            {
                cdata[prm_827].poisoned = 0;
                if (fov_player_sees(prm_827))
                {
                    txt(lang(
                        name(prm_827) + u8"は毒から回復した。"s,
                        name(prm_827) + u8" recover"s + _s(prm_827)
                            + u8" from poison."s));
                }
            }
            return;
        }
    }
    if (prm_828 == 2)
    {
        if (cdata[prm_827].sleep > 0)
        {
            if (prm_829 == 0)
            {
                cdata[prm_827].sleep = 0;
            }
            else
            {
                cdata[prm_827].sleep -= prm_829;
            }
            if (cdata[prm_827].sleep <= 0)
            {
                cdata[prm_827].sleep = 0;
                if (fov_player_sees(prm_827))
                {
                    txt(lang(
                        name(prm_827) + u8"は心地よい眠りから覚めた。"s,
                        name(prm_827) + u8" awake"s + _s(prm_827) + u8" from "s
                            + his(prm_827) + u8" sleep."s));
                }
            }
            return;
        }
    }
    if (prm_828 == 7)
    {
        if (cdata[prm_827].dimmed > 0)
        {
            if (prm_829 == 0)
            {
                cdata[prm_827].dimmed = 0;
            }
            else
            {
                cdata[prm_827].dimmed -= prm_829;
            }
            if (cdata[prm_827].dimmed <= 0)
            {
                cdata[prm_827].dimmed = 0;
                if (fov_player_sees(prm_827))
                {
                    txt(lang(
                        name(prm_827) + u8"の意識ははっきりした。"s,
                        name(prm_827) + u8" recover"s + _s(prm_827)
                            + u8" from poison."s));
                }
            }
        }
    }
    if (prm_828 == 9)
    {
        if (cdata[prm_827].bleeding > 0)
        {
            if (prm_829 == 0)
            {
                cdata[prm_827].bleeding = 0;
            }
            else
            {
                cdata[prm_827].bleeding -= prm_829;
            }
            if (cdata[prm_827].bleeding <= 0)
            {
                cdata[prm_827].bleeding = 0;
                if (fov_player_sees(prm_827))
                {
                    txt(lang(
                        name(prm_827) + u8"の出血は止まった。"s,
                        name(prm_827) + your(prm_827) + u8" bleeding stops."s));
                }
            }
        }
    }
    if (prm_828 == 8)
    {
        if (cdata[prm_827].drunk > 0)
        {
            if (prm_829 == 0)
            {
                cdata[prm_827].drunk = 0;
            }
            else
            {
                cdata[prm_827].drunk -= prm_829;
            }
            if (cdata[prm_827].drunk <= 0)
            {
                cdata[prm_827].drunk = 0;
                if (fov_player_sees(prm_827))
                {
                    txt(lang(
                        name(prm_827) + u8"の酔いは覚めた。"s,
                        name(prm_827) + u8" get"s + _s(prm_827)
                            + u8" sober."s));
                }
            }
        }
    }
    if (prm_828 == 11)
    {
        if (cdata[prm_827].insane > 0)
        {
            if (prm_829 == 0)
            {
                cdata[prm_827].insane = 0;
            }
            else
            {
                cdata[prm_827].insane -= prm_829;
            }
            if (cdata[prm_827].insane <= 0)
            {
                cdata[prm_827].insane = 0;
                if (fov_player_sees(prm_827))
                {
                    txt(lang(
                        name(prm_827) + u8"は正気に戻った。"s,
                        name(prm_827) + u8" come"s + _s(prm_827) + u8" to "s
                            + yourself(prm_827) + u8" again."s));
                }
            }
        }
    }
    if (prm_828 == 12)
    {
        if (cdata[prm_827].sick > 0)
        {
            if (prm_829 == 0)
            {
                cdata[prm_827].sick = 0;
            }
            else
            {
                cdata[prm_827].sick -= prm_829;
            }
            if (cdata[prm_827].sick <= 0)
            {
                cdata[prm_827].sick = 0;
                if (fov_player_sees(prm_827))
                {
                    txt(lang(
                        name(prm_827) + u8"の病気は治った。"s,
                        name(prm_827) + u8" recover"s + _s(prm_827)
                            + u8" from "s + his(prm_827) + u8" illness."s));
                }
            }
        }
    }
    return;
}

void txteledmg(int type, int attacker, int target, int element)
{
    switch (element)
    {
    case 50:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"は燃え上がった。"s,
                    name(target) + u8" "s + is(target) + u8" burnt."s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"燃やし尽くした。"s,
                u8"burn"s + _s(attacker) + u8" "s + him(target)
                    + u8" to death."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は燃え尽きて灰になった。"s,
                name(target) + u8" "s + is(target) + u8" burnt to ashes."s));
        }
        break;
    case 51:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"は凍えた。"s,
                    name(target) + u8" "s + is(target) + u8" frozen."s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"氷の塊に変えた。"s,
                u8"transform"s + _s(attacker) + u8" "s + him(target)
                    + u8" to an ice sculpture."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は氷の彫像になった。"s,
                name(target) + u8" "s + is(target) + u8" frozen and turn"s
                    + _s(target) + u8" into an ice sculpture."s));
        }
        break;
    case 52:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"に電流が走った。"s,
                    name(target) + u8" "s + is(target) + u8" shocked."s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"焦げカスにした。"s,
                u8"electrocute"s + _s(attacker) + u8" "s + him(target)
                    + u8" to death."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は雷に打たれ死んだ。"s,
                name(target) + u8" "s + is(target)
                    + u8" struck by lightning and die"s + _s(target) + u8"."s));
        }
        break;
    case 53:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"は闇の力で傷ついた。"s,
                    name(target) + u8" "s + is(target)
                        + u8" struck by dark force."s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"闇に飲み込んだ。"s,
                u8"let"s + _s(attacker) + u8" the depths swallow "s
                    + him(target) + u8"."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は闇に蝕まれて死んだ。"s,
                name(target) + u8" "s + is(target)
                    + u8" consumed by darkness."s));
        }
        break;
    case 58:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"の神経は傷ついた。"s,
                    name(target) + your(target) + u8" nerves are hurt."s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"神経を破壊した。"s,
                u8"destroy"s + _s(attacker) + u8" "s + his(target)
                    + u8" nerves."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は神経を蝕まれて死んだ。"s,
                name(target) + u8" die"s + _s(target)
                    + u8" from neurofibroma."s));
        }
        break;
    case 54:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"は狂気に襲われた。"s,
                    name(target) + u8" suffer"s + _s(target)
                        + u8" a splitting headache."s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"再起不能にした。"s,
                u8"completely disable"s + _s(attacker) + u8" "s + him(target)
                    + u8"."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は発狂して死んだ。"s,
                name(target) + u8" lose"s + _s(target) + u8" "s + his(target)
                    + u8" mind and commit"s + _s(target) + u8" a suicide."s));
        }
        break;
    case 59:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"は混沌の渦で傷ついた。"s,
                    name(target) + u8" "s + is(target)
                        + u8" hurt by chaotic force."s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"混沌の渦に吸い込んだ。"s,
                u8"let"s + _s(attacker) + u8" the chaos consume "s + him(target)
                    + u8"."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は混沌の渦に吸収された。"s,
                name(target) + u8" "s + is(target)
                    + u8" drawn into a chaotic vortex."s));
        }
        break;
    case 56:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"は冥界の冷気で傷ついた。"s,
                    name(target) + u8" "s + is(target)
                        + u8" chilled by infernal squall."s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"冥界に墜とした。"s,
                u8"entrap"s + _s(attacker) + u8" "s + him(target)
                    + u8" into the inferno."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は冥界に墜ちた。"s,
                name(target) + u8" go"s + _s(target, true) + u8" to hell."s));
        }
        break;
    case 55:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"は吐き気を催した。"s,
                    name(target) + u8" suffer"s + _s(target)
                        + u8" from venom."s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"毒殺した。"s,
                u8"kill"s + _s(attacker) + u8" "s + him(target)
                    + u8" with poison."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は毒に蝕まれて死んだ。"s,
                name(target) + u8" "s + is(target) + u8" poisoned to death."s));
        }
        break;
    case 57:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"は轟音の衝撃を受けた。"s,
                    name(target) + u8" "s + is(target)
                        + u8" shocked by a shrill sound"s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"聴覚を破壊し殺した。"s,
                u8"shatter"s + _s(attacker) + u8" "s + him(target)
                    + u8" to atoms."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は朦朧となって死んだ。"s,
                name(target) + u8" resonate"s + _s(target)
                    + u8" and break up."s));
        }
        break;
    case 63:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"は酸に焼かれた。"s,
                    name(target) + u8" "s + is(target) + u8" burnt by acid."s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"ドロドロに溶かした。"s,
                u8"melt"s + _s(attacker) + u8" "s + him(target) + u8" away."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は酸に焼かれ溶けた。"s,
                name(target) + u8" melt"s + _s(target) + u8"."s));
        }
        break;
    case 61:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"は切り傷を負った。"s,
                    name(target) + u8" get"s + _s(target) + u8" a cut."s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"千切りにした。"s,
                u8"cut"s + _s(attacker) + u8" "s + him(target)
                    + u8" into thin strips."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は千切りになった。"s,
                name(target) + u8" "s + is(target)
                    + u8" cut into thin strips."s));
        }
        break;
    default:
        if (type == 0)
        {
            if (fov_player_sees(target))
            {
                txt(lang(
                    name(target) + u8"は傷ついた。"s,
                    name(target) + u8" "s + is(target) + u8" wounded."s));
            }
        }
        else if (type == 1)
        {
            txt(lang(
                u8"殺した。"s,
                u8"kill"s + _s(attacker) + u8" "s + him(target) + u8"."s));
        }
        else if (type == 2)
        {
            txt(lang(
                name(target) + u8"は死んだ。"s,
                name(target) + u8" "s + is(target) + u8" killed."s));
        }
        break;
    }
}

void damage_apply_status_effects(int cc)
{
    int regen = 0;
    regen = 1;
    if (cdata[cc].sleep > 0)
    {
        healcon(cc, 2, 1);
        if (cdata[cc].sleep > 0)
        {
            cdata[cc].emotion_icon = 114;
        }
        healhp(cc, 1);
        healmp(cc, 1);
    }
    if (cdata[cc].poisoned > 0)
    {
        dmghp(cc, rnd(2 + sdata(11, cc) / 10), -4);
        healcon(cc, 1, 1);
        if (cdata[cc].poisoned > 0)
        {
            cdata[cc].emotion_icon = 108;
        }
        regen = 0;
    }
    if (cdata[cc].choked > 0)
    {
        if (cdata[cc].choked % 3 == 0)
        {
            if (fov_player_sees(cc))
            {
                txt(lang(u8"「うぐぐ…！」"s, u8"\"Ughh...!\""s));
            }
        }
        ++cdata[cc].choked;
        if (cdata[cc].choked > 15)
        {
            dmghp(cc, 500, -21);
        }
        regen = 0;
    }
    if (cdata[cc].gravity > 0)
    {
        --cdata[cc].gravity;
        if (cdata[cc].gravity == 0)
        {
            if (fov_player_sees(cc))
            {
                txt(lang(
                    name(cc) + u8"は重力から抜け出した。"s,
                    name(cc) + u8" break"s + _s(cc)
                        + u8" away from gravity."s));
            }
        }
    }
    if (cdata[cc].furious > 0)
    {
        --cdata[cc].furious;
        if (cdata[cc].furious == 0)
        {
            if (fov_player_sees(cc))
            {
                txt(lang(
                    name(cc) + u8"はやや落ち着いた。"s,
                    name(cc) + u8" calm"s + _s(cc) + u8" down."s));
            }
        }
    }
    if (cdata[cc].sick > 0)
    {
        if (rnd(80) == 0)
        {
            p = rnd(10);
            if (encfind(cc, 60010 + p) == -1)
            {
                cdata[cc].attr_adjs[p] -=
                    sdata.get(10 + p, cc).original_level / 25 + 1;
                chara_refresh(cc);
            }
        }
        if (rnd(5))
        {
            regen = 0;
        }
        if (cc >= 16)
        {
            if (cdata[cc].quality >= 4)
            {
                if (rnd(200) == 0)
                {
                    healcon(cc, 12);
                }
            }
        }
    }
    if (cdata[cc].blind > 0)
    {
        healcon(cc, 4, 1);
        if (cdata[cc].blind > 0)
        {
            cdata[cc].emotion_icon = 110;
        }
    }
    if (cdata[cc].paralyzed > 0)
    {
        regen = 0;
        healcon(cc, 3, 1);
        if (cdata[cc].paralyzed > 0)
        {
            cdata[cc].emotion_icon = 115;
        }
    }
    if (cdata[cc].confused > 0)
    {
        healcon(cc, 5, 1);
        if (cdata[cc].confused > 0)
        {
            cdata[cc].emotion_icon = 111;
        }
    }
    if (cdata[cc].fear > 0)
    {
        healcon(cc, 6, 1);
        if (cdata[cc].fear > 0)
        {
            cdata[cc].emotion_icon = 113;
        }
    }
    if (cdata[cc].dimmed > 0)
    {
        healcon(cc, 7, 1);
        if (cdata[cc].dimmed > 0)
        {
            cdata[cc].emotion_icon = 112;
        }
    }
    if (cdata[cc].drunk > 0)
    {
        healcon(cc, 8, 1);
        if (cdata[cc].drunk > 0)
        {
            cdata[cc].emotion_icon = 106;
        }
    }
    if (cdata[cc].bleeding > 0)
    {
        dmghp(
            cc,
            rnd(cdata[cc].hp * (1 + cdata[cc].bleeding / 4) / 100 + 3) + 1,
            -13);
        healcon(cc, 9, 1 + cdata[cc].cures_bleeding_quickly() * 3);
        if (cdata[cc].bleeding > 0)
        {
            cdata[cc].emotion_icon = 109;
        }
        regen = 0;
        spillblood(cdata[cc].position.x, cdata[cc].position.y);
    }
    if (cdata[cc].wet > 0)
    {
        --cdata[cc].wet;
    }
    if (cdata[cc].insane > 0)
    {
        if (fov_player_sees(cc))
        {
            if (rnd(3) == 0)
            {
                txtef(9);
                if (jp)
                {
                    r2 = rnd(5);
                    if (r2 == 0)
                    {
                        txt(name(cc) + u8"「キョキョキョ」"s,
                            name(cc) + u8"「クワッ」"s,
                            name(cc) + u8"「シャアァァ」"s,
                            name(cc) + u8"「ばぶっふ！」"s,
                            name(cc) + u8"「煮殺せ！」"s,
                            name(cc) + u8"「許しなさい許しなさい！！」"s);
                    }
                    if (r2 == 1)
                    {
                        txt(name(cc) + u8"「フゥハハハー！」"s,
                            name(cc) + u8"「あ、あ、あ、あ」"s,
                            name(cc) + u8"「ぴ…ぴ…ぴか…」"s,
                            name(cc) + u8"「お兄ちゃん！」"s,
                            name(cc) + u8"「うみみやぁ」"s);
                    }
                    if (r2 == 2)
                    {
                        txt(name(cc) + u8"は突然踊りだした。"s,
                            name(cc) + u8"は着ていたものを脱ぎだした。"s,
                            name(cc) + u8"はぐるぐる回りだした。"s,
                            name(cc) + u8"は奇声を発した。"s,
                            name(cc) + u8"「ねうねう♪ねうねう♪」"s);
                    }
                    if (r2 == 3)
                    {
                        txt(name(cc) + u8"「ウージッムシ♪ウージッムシ♪」"s,
                            name(cc) + u8"「じゃあ殺さなきゃ。うん♪」"s,
                            name(cc) + u8"「このナメクジがっ」"s,
                            name(cc) + u8"「おすわり！」"s,
                            name(cc) +
                                u8"「フーーーーン フーーーーン･･･ フーーーンフ」"s);
                    }
                    if (r2 == 4)
                    {
                        txt(name(cc) + u8"「このかたつむり野郎がっ」"s,
                            name(cc) + u8"「うにゅみゅあ！」"s,
                            name(cc) + u8"「ごめんなさいごめんなさい！」"s,
                            name(cc) + u8"「もうすぐ生まれるよ♪」"s,
                            name(cc) + u8"「フーーーーン フー…クワッ！」"s);
                    }
                }
                else
                {
                    r2 = rnd(2);
                    if (r2 == 0)
                    {
                        txt(name(cc) + u8" start"s + _s(cc) + u8" to take "s
                                + his(cc) + u8" cloths off."s,
                            name(cc) + u8" shout"s + _s(cc) + u8"."s,
                            name(cc) + u8" dance"s + _s(cc) + u8"."s);
                    }
                    if (r2 == 1)
                    {
                        txt(u8"\"Weeeeeee!\""s,
                            u8"\"Forgive me! Forgive me!\""s,
                            u8"\"P-P-Pika!\""s,
                            u8"\"Shhhhhh!\""s,
                            u8"\"So I have to kill.\""s,
                            u8"\"You snail!\""s);
                    }
                }
            }
        }
        if (rnd(5) == 0)
        {
            cdata[cc].confused += rnd(10);
        }
        if (rnd(5) == 0)
        {
            cdata[cc].dimmed += rnd(10);
        }
        if (rnd(5) == 0)
        {
            cdata[cc].sleep += rnd(5);
        }
        if (rnd(5) == 0)
        {
            cdata[cc].fear += rnd(10);
        }
        healcon(cc, 11, 1);
        if (cdata[cc].insane > 0)
        {
            cdata[cc].emotion_icon = 124;
        }
    }
    if (cc == 0)
    {
        if (cdata[cc].nutrition < 2000)
        {
            if (cdata[cc].nutrition < 1000)
            {
                if (cdata[cc].continuous_action_id != 1)
                {
                    dmghp(cc, rnd(2) + cdata[0].max_hp / 50, -3);
                    if (gdata_play_turns % 10 == 0)
                    {
                        rowact_check(cc);
                        if (rnd(50) == 0)
                        {
                            modweight(cc, -1);
                        }
                    }
                }
            }
            regen = 0;
        }
        if (gdata_continuous_active_hours >= 30)
        {
            if (debug::voldemort)
            {
                gdata_continuous_active_hours = 0;
            }
            if (gdata_play_turns % 100 == 0)
            {
                txt(lang(
                    u8"あなたは眠りを必要としている。"s,
                    u8"You need to sleep."s));
            }
            if (rnd(2))
            {
                regen = 0;
            }
            if (gdata_continuous_active_hours >= 50)
            {
                regen = 0;
                dmgsp(cc, 1);
            }
        }
    }
    else if (cdata[cc].related_quest_id != 0)
    {
        p = cdata[cc].related_quest_id - 1;
        if (qdata(15, p) > 0)
        {
            cdata[cc].emotion_icon = 122;
        }
        if (qdata(8, p) != 0)
        {
            if (cdata[cc].turn % 2 == 1)
            {
                cdata[cc].emotion_icon = 123;
            }
        }
    }
    if (gdata_executing_immediate_quest_type == 1009)
    {
        if (cc >= 57)
        {
            if (cdata[cc].impression >= 53)
            {
                cdata[cc].emotion_icon = 225;
            }
        }
    }
    if (regen == 1)
    {
        if (rnd(6) == 0)
        {
            healhp(cc, rnd(sdata(154, cc) / 3 + 1) + 1);
        }
        if (rnd(5) == 0)
        {
            healmp(cc, rnd(sdata(155, cc) / 2 + 1) + 1);
        }
    }
}


void get_pregnant()
{
    if (encfind(tc, 48) != -1)
    {
        if (fov_player_sees(tc))
        {
            txt(lang(
                u8"しかしすぐに吐き出した。"s,
                u8"But "s + he(tc) + u8" puke"s + _s(tc)
                    + u8" it out quickly."s));
        }
        return;
    }
    if (cdata[tc].is_pregnant() == 0)
    {
        txtef(5);
        txt(lang(
            name(tc) + u8"は寄生された。"s,
            name(tc) + u8" get"s + _s(tc) + u8" pregnant."s));
        animeload(8, tc);
        cdata[tc].is_pregnant() = true;
    }
    return;
}


void wet(int cc, int turns)
{
    cdata[cc].wet += turns;
    if (fov_player_sees(cc))
    {
        txt(lang(
            name(cc) + u8"は濡れた。"s,
            name(cc) + u8" get"s + _s(cc) + u8" wet."s));
        if (cdata[cc].is_invisible())
        {
            txt(lang(
                name(cc) + u8"の姿があらわになった。"s,
                name(cc) + u8" "s + is(cc) + u8" revealed "s + his(cc)
                    + u8" shape."s));
        }
    }
}

void get_hungry(int cc)
{
    if ((trait(158) && rnd(3) == 0) || debug::voldemort)
        return;

    int p = cdata[cc].nutrition / 1000;
    cdata[cc].nutrition -= 8;
    if (cdata[cc].nutrition / 1000 != p)
    {
        if (p == 1)
        {
            msgalert = 1;
            txt(lang(u8"このままだと餓死してしまう！"s, u8"You are starving!"s),
                lang(
                    u8"腹が減ってほとんど死にかけている。"s,
                    u8"You are almost dead from hunger."s));
        }
        else if (p == 2)
        {
            msgalert = 1;
            txt(lang(
                    u8"空腹で目が回りだした…"s,
                    u8"Your hunger makes you dizzy."s),
                lang(
                    u8"すぐに何かを食べなくては…"s,
                    u8"You have to eat something NOW."s));
        }
        else if (p == 5)
        {
            msgalert = 1;
            txt(lang(u8"腹がすいてきた。"s, u8"You are getting hungry."s),
                lang(u8"空腹になった。"s, u8"You feel hungry."s),
                lang(u8"さて何を食べようか。"s, u8"Now what shall I eat?"s));
        }
        refreshspeed(cc);
    }
}


} // namespace elona
