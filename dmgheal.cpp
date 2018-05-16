#include "dmgheal.hpp"
#include "ability.hpp"
#include "animation.hpp"
#include "audio.hpp"
#include "buff.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "config.hpp"
#include "debug.hpp"
#include "draw.hpp"
#include "elona.hpp"
#include "event.hpp"
#include "fov.hpp"
#include "item.hpp"
#include "map_cell.hpp"
#include "mef.hpp"
#include "status_ailment.hpp"
#include "quest.hpp"
#include "variables.hpp"



namespace elona
{

enum class element_t;

int target;
int dmg_at_m141 = 0;



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



int dmghp(int target, int amount, int attacker, element_t element, int element_power)
{
    int ele_at_m141 = static_cast<int>(element);
    int c3_at_m141 = 0;
    int r_at_m141 = 0;
    int dmglevel_at_m141 = 0;
    int f_at_m141 = 0;
    int se_at_m141 = 0;
    elona_vector1<int> p_at_m141;
    int exp_at_m141 = 0;
    elona::target = target;
    if (txt3rd == 0)
    {
        c3_at_m141 = attacker;
    }
    else
    {
        c3_at_m141 = -1;
    }
    if (cdata[target].state != 1)
    {
        end_dmghp();
        return 0;
    }
    dmg_at_m141 = amount * (1 + (cdata[target].furious > 0));
    if (attacker >= 0)
    {
        if (cdata[attacker].furious > 0)
        {
            dmg_at_m141 *= 2;
        }
    }
    if (ele_at_m141 != 0 && ele_at_m141 < 61)
    {
        r_at_m141 = sdata(ele_at_m141, target) / 50;
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
        dmg_at_m141 = dmg_at_m141 * 100 / (sdata(60, target) / 2 + 50);
    }
    if (attacker == 0)
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
    if (cdata[target].wet > 0)
    {
        if (ele_at_m141 == 50 || attacker == -9)
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
            if (cdata[target].is_immune_to_elemental_damage())
            {
                dmg_at_m141 = 0;
            }
        }
    }
    if (cdata[target].is_metal())
    {
        dmg_at_m141 = rnd(dmg_at_m141 / 10 + 2);
    }
    if (cdata[target].is_contracting_with_reaper())
    {
        if (cdata[target].hp - dmg_at_m141 <= 0)
        {
            if (clamp(
                    25 + cdata[target].buffs[buff_find(target, 18)].power / 17,
                    25,
                    80)
                >= rnd(100))
            {
                dmg_at_m141 *= -1;
            }
        }
    }
    if (cdata[target].nullify_damage > 0)
    {
        if (cdata[target].nullify_damage > rnd(100))
        {
            dmg_at_m141 = 0;
        }
    }
    if (ele_at_m141 == 658)
    {
        dmg_at_m141 = amount;
    }
    rtdmg = dmg_at_m141;

    if (target == 0 && cdata[0].god_id == core_god::opatos)
    {
        dmg_at_m141 = dmg_at_m141 * 90 / 100;
    }

    if (debug::voldemort && target == 0)
    {
        dmg_at_m141 = 0;
    }
    cdata[target].hp -= dmg_at_m141;


    if (is_in_fov(target))
    {
        add_damage_popup(std::to_string(dmg_at_m141), target, {0, 0, 0});
    }


    if (ele_at_m141 == 56)
    {
        if (attacker >= 0)
        {
            if (dmg_at_m141 > 0)
            {
                healhp(
                    attacker,
                    clamp(
                        rnd(dmg_at_m141 * (150 + element_power * 2) / 1000 + 10),
                        1,
                        cdata[attacker].max_hp / 10 + rnd(5)));
            }
        }
    }
    if (target == 0)
    {
        gdata(30) = 0;
        if (cdata[target].hp < 0)
        {
            if (event_id() != -1)
            {
                if (event_id() != 21)
                {
                    cdata[target].hp = 1;
                }
            }
            if (gdata_current_map == 40)
            {
                cdata[target].hp = 1;
            }
        }
    }
    if (dmg_at_m141 <= 0)
    {
        dmglevel_at_m141 = -1;
    }
    else
    {
        dmglevel_at_m141 = dmg_at_m141 * 6 / cdata[target].max_hp;
    }
    if (cdata[target].hp < 0)
    {
        if (target < 16)
        {
            for (int cnt = 0; cnt < 16; ++cnt)
            {
                if (target == cnt)
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
                    name(target) + u8"は回復した。"s,
                    name(target) + u8" "s + is(target) + u8" healed."s));
                cdata[target].hp = cdata[target].max_hp / 2;
                animode = 100 + target;
                play_animation(19);
                snd(120);
                break;
            }
        }
        else if (cdata[target].is_hung_on_sand_bag())
        {
            cdata[target].hp = cdata[target].max_hp;
        }
    }
    if (cdata[target].hp >= 0)
    {
        if (dmglevel_at_m141 > 1)
        {
            spillblood(
                cdata[target].position.x,
                cdata[target].position.y,
                1 + rnd(2));
        }
        if (gdata(809) == 1)
        {
            txteledmg(0, c3_at_m141, target, ele_at_m141);
            goto label_1369_internal;
        }
        if (dmglevel_at_m141 > 0)
        {
            if (cdata[target].max_hp / 2 > cdata[target].hp)
            {
                ++dmglevel_at_m141;
                if (cdata[target].max_hp / 4 > cdata[target].hp)
                {
                    ++dmglevel_at_m141;
                    if (cdata[target].max_hp / 10 > cdata[target].hp)
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
                    u8"slightly wound"s + _s(c3_at_m141) + u8" "s + him(target)
                        + u8"."s));
            }
            if (dmglevel_at_m141 == 1)
            {
                txtef(11);
                txt(lang(
                    u8"傷つけた。"s,
                    u8"moderately wound"s + _s(c3_at_m141) + u8" "s
                        + him(target) + u8"."s));
            }
            if (dmglevel_at_m141 == 2)
            {
                txtef(10);
                txt(lang(
                    u8"深い傷を負わせた。"s,
                    u8"severely wound"s + _s(c3_at_m141) + u8" "s + him(target)
                        + u8"."s));
            }
            if (dmglevel_at_m141 >= 3)
            {
                txtef(3);
                txt(lang(
                    u8"致命傷を与えた。"s,
                    u8"critically wound"s + _s(c3_at_m141) + u8" "s
                        + him(target) + u8"!"s));
            }
            rowact_check(target);
            goto label_1369_internal;
        }
        if (dmglevel_at_m141 == 1)
        {
            if (is_in_fov(target))
            {
                txtef(11);
                txt(lang(
                    name(target) + u8"は痛手を負った。"s,
                    name(target) + u8" scream"s + _s(target) + u8"."s));
            }
        }
        if (dmglevel_at_m141 == 2)
        {
            if (is_in_fov(target))
            {
                txtef(10);
                txt(lang(
                    name(target) + u8"は苦痛にもだえた。"s,
                    name(target) + u8" writhe"s + _s(target)
                        + u8" in pain."s));
            }
        }
        if (dmglevel_at_m141 >= 3)
        {
            if (is_in_fov(target))
            {
                txtef(3);
                txt(lang(
                    name(target) + u8"は悲痛な叫び声をあげた。"s,
                    name(target) + u8" "s + is(target)
                        + u8" severely hurt!"s));
            }
        }
        if (dmg_at_m141 < 0)
        {
            if (cdata[target].hp > cdata[target].max_hp)
            {
                cdata[target].hp = cdata[target].max_hp;
            }
            if (is_in_fov(target))
            {
                txtef(4);
                txt(lang(
                    name(target) + u8"は回復した。"s,
                    name(target) + u8" "s + is(target) + u8" healed."s));
            }
        }
    label_1369_internal:
        rowact_check(target);
        if (cdata[target].hp < cdata[target].max_hp / 5)
        {
            if (target != 0)
            {
                if (cdata[target].fear == 0)
                {
                    if (cdata[target].is_immune_to_fear() == 0)
                    {
                        if (dmg_at_m141 * 100 / cdata[target].max_hp + 10
                            > rnd(200))
                        {
                            f_at_m141 = 1;
                        }
                        else
                        {
                            f_at_m141 = 0;
                        }
                        if (attacker == 0)
                        {
                            if (trait(44))
                            {
                                f_at_m141 = 0;
                            }
                        }
                        if (f_at_m141)
                        {
                            cdata[target].fear = rnd(20) + 5;
                            if (is_in_fov(target))
                            {
                                txtef(4);
                                txt(lang(
                                    name(target) + u8"は恐怖して逃げ出した。"s,
                                    name(target) + u8" run"s + _s(target)
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
                if (rnd(10) < element_power / 75 + 4)
                {
                    dmgcon(target, status_ailment_t::blinded, rnd(element_power / 3 * 2 + 1));
                }
                if (rnd(20) < element_power / 50 + 4)
                {
                    dmgcon(target, status_ailment_t::paralyzed, rnd(element_power / 3 * 2 + 1));
                }
                if (rnd(20) < element_power / 50 + 4)
                {
                    dmgcon(target, status_ailment_t::confused, rnd(element_power / 3 * 2 + 1));
                }
                if (rnd(20) < element_power / 50 + 4)
                {
                    dmgcon(target, status_ailment_t::poisoned, rnd(element_power / 3 * 2 + 1));
                }
                if (rnd(20) < element_power / 50 + 4)
                {
                    dmgcon(target, status_ailment_t::sleep, rnd(element_power / 3 * 2 + 1));
                }
            }
            if (ele_at_m141 == 52)
            {
                if (rnd(3 + (cdata[target].quality >= 4) * 3) == 0)
                {
                    ++cdata[target].paralyzed;
                }
            }
            if (ele_at_m141 == 53)
            {
                dmgcon(target, status_ailment_t::blinded, rnd(element_power + 1));
            }
            if (ele_at_m141 == 58)
            {
                dmgcon(target, status_ailment_t::paralyzed, rnd(element_power + 1));
            }
            if (ele_at_m141 == 54)
            {
                dmgcon(target, status_ailment_t::confused, rnd(element_power + 1));
            }
            if (ele_at_m141 == 57)
            {
                dmgcon(target, status_ailment_t::confused, rnd(element_power + 1));
            }
            if (ele_at_m141 == 55)
            {
                dmgcon(target, status_ailment_t::poisoned, rnd(element_power + 1));
            }
            if (ele_at_m141 == 61)
            {
                dmgcon(target, status_ailment_t::bleeding, rnd(element_power + 1));
            }
            if (ele_at_m141 == 62)
            {
                if (target == 0)
                {
                    modcorrupt(rnd(element_power + 1));
                }
            }
            if (ele_at_m141 == 63)
            {
                if (target == 0 || rnd(3) == 0)
                {
                    item_acid(target, -1);
                }
            }
        }
        if ((ele_at_m141 == 50 || attacker == -9) && cdata[target].wet == 0)
        {
            item_fire(target, -1);
        }
        if (ele_at_m141 == 51)
        {
            item_cold(target, -1);
        }
        if (cdata[target].sleep != 0)
        {
            if (ele_at_m141 != 54 && ele_at_m141 != 58 && ele_at_m141 != 59)
            {
                cdata[target].sleep = 0;
                txt(lang(
                    name(target) + u8"は眠りを妨げられた。"s,
                    name(target) + your(target) + u8" sleep "s + is(target)
                        + u8" disturbed."s));
            }
        }
        if (attacker == 0)
        {
            hostileaction(0, target);
            gdata(94) = target;
        }
        if (target == 0)
        {
            if (cdata[target].max_hp / 4 > cdata[target].hp)
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
        if (cdata[target].explodes())
        {
            if (rnd(3) == 0)
            {
                cdata[target].will_explode_soon() = true;
                txtef(9);
                txt(lang(u8" *カチッ* "s, u8"*click*"s));
            }
        }
        if (cdata[target].splits())
        {
            if (gdata(809) != 1)
            {
                if (dmg_at_m141 > cdata[target].max_hp / 20 || rnd(10) == 0)
                {
                    if (mdata(6) != 1)
                    {
                        int stat = chara_copy(target);
                        if (stat == 1)
                        {
                            txt(lang(
                                name(target) + u8"は分裂した！"s,
                                name(target) + u8" split"s + _s(target)
                                    + u8"!"s));
                        }
                    }
                }
            }
        }
        if (cdata[target].splits2())
        {
            if (gdata(809) != 1)
            {
                if (rnd(3) == 0)
                {
                    if (cdata[target].confused == 0
                        && cdata[target].dimmed == 0
                        && cdata[target].poisoned == 0
                        && cdata[target].paralyzed == 0
                        && cdata[target].blind == 0)
                    {
                        if (mdata(6) != 1)
                        {
                            int stat = chara_copy(target);
                            if (stat == 1)
                            {
                                txt(lang(
                                    name(target) + u8"は分裂した！"s,
                                    name(target) + u8" split"s + _s(target)
                                        + u8"!"s));
                            }
                        }
                    }
                }
            }
        }
        if (cdata[target].is_quick_tempered())
        {
            if (gdata(809) != 1)
            {
                if (cdata[target].furious == 0)
                {
                    if (rnd(20) == 0)
                    {
                        if (is_in_fov(target))
                        {
                            txtef(4);
                            txt(lang(
                                name(target) + u8"は怒りに体を奮わせた！"s,
                                name(target) + u8" "s + is(target)
                                    + u8" engulfed in fury!"s));
                        }
                        cdata[target].furious += rnd(30) + 15;
                    }
                }
            }
        }
        if (attacker >= 0)
        {
            f_at_m141 = 0;
            if (cdata[target].relationship <= -3)
            {
                if (cdata[attacker].original_relationship > -3)
                {
                    if (cdata[target].hate == 0 || rnd(4) == 0)
                    {
                        f_at_m141 = 1;
                    }
                }
            }
            else if (cdata[attacker].original_relationship <= -3)
            {
                if (cdata[target].hate == 0 || rnd(4) == 0)
                {
                    f_at_m141 = 1;
                }
            }
            if (attacker != 0)
            {
                if (cdata[attacker].enemy_id == target)
                {
                    if (rnd(3) == 0)
                    {
                        f_at_m141 = 1;
                    }
                }
            }
            if (f_at_m141)
            {
                if (target != 0)
                {
                    cdata[target].enemy_id = attacker;
                    if (cdata[target].hate == 0)
                    {
                        cdata[target].emotion_icon = 218;
                        cdata[target].hate = 20;
                    }
                    else
                    {
                        cdata[target].hate += 2;
                    }
                }
            }
        }
    }
    if (cdata[target].hp < 0)
    {
        se_at_m141 = eleinfo(ele_at_m141, 1);
        if (se_at_m141)
        {
            snd(se_at_m141, false, false);
        }
        txtef(3);
        if (attacker >= 0)
        {
            if (ele_at_m141)
            {
                if (target >= 16 && gdata(809) == 2)
                {
                    txtcontinue();
                    txteledmg(1, c3_at_m141, target, ele_at_m141);
                }
                else
                {
                    txteledmg(2, c3_at_m141, target, ele_at_m141);
                }
            }
            else
            {
                p_at_m141 = rnd(4);
                if (p_at_m141 == 0)
                {
                    if (target >= 16 && gdata(809) == 2)
                    {
                        txtcontinue();
                        txt(lang(
                            u8"粉々の肉片に変えた。"s,
                            u8"transform"s + _s(c3_at_m141) + u8" "s
                                + him(target)
                                + u8" into several pieces of meat."s));
                    }
                    else
                    {
                        txt(lang(
                            name(target) + u8"は粉々の肉片に変えられた。"s,
                            name(target) + u8" "s + is(target)
                                + u8" transformed into several pieces of meat."s));
                    }
                }
                if (p_at_m141 == 1)
                {
                    if (target >= 16 && gdata(809) == 2)
                    {
                        txtcontinue();
                        txt(lang(
                            u8"破壊した。"s,
                            u8"destroy"s + _s(c3_at_m141) + u8" "s
                                + him(target) + u8"."s));
                    }
                    else
                    {
                        txt(lang(
                            name(target) + u8"は破壊された。"s,
                            name(target) + u8" "s + is(target)
                                + u8" killed."s));
                    }
                }
                if (p_at_m141 == 2)
                {
                    if (target >= 16 && gdata(809) == 2)
                    {
                        txtcontinue();
                        txt(lang(
                            u8"ミンチにした。"s,
                            u8"mince"s + _s(c3_at_m141) + u8" "s + him(target)
                                + u8"."s));
                    }
                    else
                    {
                        txt(lang(
                            name(target) + u8"はミンチにされた。"s,
                            name(target) + u8" "s + is(target)
                                + u8" minced."s));
                    }
                }
                if (p_at_m141 == 3)
                {
                    if (target >= 16 && gdata(809) == 2)
                    {
                        txtcontinue();
                        txt(lang(
                            u8"殺した。"s,
                            u8"kill"s + _s(c3_at_m141) + u8" "s + him(target)
                                + u8"."s));
                    }
                    else
                    {
                        txt(lang(
                            name(target) + u8"は殺された。"s,
                            name(target) + u8" "s + is(target)
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
            if (attacker == -11)
            {
                txt(lang(
                    name(target) + u8"は見えざる手に葬られた。"s,
                    name(target) + u8" "s + is(target)
                        + u8" assassinated by the unseen hand."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"見えざる手に葬られた。"s,
                        u8"got assassinated by the unseen hand"s);
                }
            }
            if (attacker == -1)
            {
                txt(lang(
                    name(target) + u8"は罠にかかって死んだ。"s,
                    name(target) + u8" "s + is(target)
                        + u8" caught in a trap and die"s + _s(target)
                        + u8"."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"罠にかかって死んだ。"s,
                        u8"got caught in a trap and died"s);
                }
            }
            if (attacker == -2)
            {
                txt(lang(
                    name(target) + u8"はマナの反動で死んだ。"s,
                    name(target) + u8" die"s + _s(target)
                        + u8" from over-casting."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"マナの反動で消滅した。"s,
                        u8"was completely wiped by magic reaction"s);
                }
            }
            if (attacker == -3)
            {
                txt(lang(
                    name(target) + u8"は餓死した。"s,
                    name(target) + u8" "s + is(target)
                        + u8" starved to death."s));
                if (target == 0)
                {
                    ndeathcause =
                        lang(u8"飢え死にした。"s, u8"was starved to death"s);
                }
            }
            if (attacker == -4)
            {
                txt(lang(
                    name(target) + u8"は毒に蝕まれ死んだ。"s,
                    name(target) + u8" "s + is(target)
                        + u8" killed with poison."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"毒にもがき苦しみながら死んだ。"s,
                        u8"miserably died from poison"s);
                }
            }
            if (attacker == -13)
            {
                txt(lang(
                    name(target) + u8"は出血多量で死んだ。"s,
                    name(target) + u8" die"s + _s(target)
                        + u8" from loss of blood"s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"血を流しすぎて死んだ。"s,
                        u8"died from loss of blood"s);
                }
            }
            if (attacker == -5)
            {
                txt(lang(
                    name(target) + u8"は呪いの力で死んだ。"s,
                    name(target) + u8" die"s + _s(target)
                        + u8" from curse."s));
                if (target == 0)
                {
                    ndeathcause =
                        lang(u8"呪い殺された。"s, u8"died from curse"s);
                }
            }
            if (attacker == -7)
            {
                txt(lang(
                    name(target) + u8"は階段から転げ落ちて死んだ。"s,
                    name(target) + u8" tumble"s + _s(target)
                        + u8" from stairs and die"s + _s(target) + u8"."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"階段から転げ落ちて亡くなった。"s,
                        u8"tumbled from stairs and died"s);
                }
            }
            if (attacker == -8)
            {
                txt(lang(
                    name(target) + u8"は聴衆に殺された。"s,
                    name(target) + u8" "s + is(target)
                        + u8" killed by an audience."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"演奏中に激怒した聴衆に殺された。"s,
                        u8"was killed by an audience"s);
                }
            }
            if (attacker == -9)
            {
                txt(lang(
                    name(target) + u8"は焼け死んだ。"s,
                    name(target) + u8" "s + is(target)
                        + u8" burnt and turned into ash."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"焼けて消滅した。"s,
                        u8"was burnt and turned into ash"s);
                }
            }
            if (attacker == -12)
            {
                txt(lang(
                    name(target) + u8"は食中毒で死んだ。"s,
                    name(target) + u8" "s + is(target)
                        + u8" killed by food poisoning."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"食中毒で倒れた。"s,
                        u8"got killed by food poisoning"s);
                }
            }
            if (attacker == -14)
            {
                txt(lang(
                    name(target) + u8"はエーテルに侵食され死んだ。"s,
                    name(target) + u8" die"s + _s(target)
                        + u8" of the Ether disease."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"エーテルの病に倒れた。"s,
                        u8"died of the Ether disease"s);
                }
            }
            if (attacker == -15)
            {
                txt(lang(
                    name(target) + u8"は溶けて液体になった。"s,
                    name(target) + u8" melt"s + _s(target) + u8" down."s));
                if (target == 0)
                {
                    ndeathcause =
                        lang(u8"溶けて液体になった。"s, u8"melted down"s);
                }
            }
            if (attacker == -16)
            {
                txt(lang(
                    name(target) + u8"はバラバラになった。"s,
                    name(target) + u8" shatter"s + _s(target) + u8"."s));
                if (target == 0)
                {
                    ndeathcause = lang(u8"自殺した。"s, u8"committed suicide"s);
                }
            }
            if (attacker == -17)
            {
                txt(lang(
                    name(target) + u8"は核爆発に巻き込まれて塵となった。"s,
                    name(target) + u8" "s + is(target)
                        + u8" turned into atoms."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"核爆発に巻き込まれて死んだ。"s,
                        u8"was killed by an atomic bomb"s);
                }
            }
            if (attacker == -18)
            {
                txt(lang(
                    name(target)
                        + u8"はアイアンメイデンの中で串刺しになって果てた。"s,
                    name(target) + u8" step"s + _s(target)
                        + u8" in an iron maiden and die"s + _s(target)
                        + u8"."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"アイアンメイデンにはさまれて死んだ。"s,
                        u8"stepped in an iron maiden and died"s);
                }
            }
            if (attacker == -19)
            {
                txt(lang(
                    name(target) + u8"はギロチンで首をちょんぎられて死んだ。"s,
                    name(target) + u8" "s + is(target)
                        + u8" guillotined and die"s + _s(target) + u8"."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"ギロチンで首を落とされて死んだ。"s,
                        u8"was guillotined"s);
                }
            }
            if (attacker == -20)
            {
                txt(lang(
                    name(target) + u8"は首を吊った。"s,
                    name(target) + u8" hang"s + _s(target) + u8" "s
                        + his(target) + u8"self."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"首を吊った。"s, u8"commited suicide by hanging"s);
                }
            }
            if (attacker == -21)
            {
                txt(lang(
                    name(target) + u8"はもちを喉に詰まらせて死んだ。"s,
                    name(target) + u8" choke"s + _s(target)
                        + u8" on mochi and die."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        u8"もちを喉に詰まらせて死んだ。"s,
                        u8"ate mochi and died"s);
                }
            }
            if (attacker == -6)
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
                    name(target) + u8"は"s + rtvaln
                        + u8"の重さに耐え切れず死んだ。"s,
                    name(target) + u8" "s + is(target) + u8" squashed by "s
                        + rtvaln + u8"."s));
                if (target == 0)
                {
                    ndeathcause = lang(
                        rtvaln + u8"の重さに耐え切れず潰れた。"s,
                        u8"was squashed by "s + rtvaln);
                }
            }
        }
        if (attacker == -9 || ele_at_m141 == 50)
        {
            mef_add(
                cdata[target].position.x,
                cdata[target].position.y,
                5,
                24,
                rnd(10) + 5,
                100,
                attacker);
        }
        if (ele_at_m141 == 56)
        {
            if (attacker >= 0)
            {
                if (dmg_at_m141 > 0)
                {
                    healhp(cc, rnd(dmg_at_m141 * (200 + element_power) / 1000 + 5));
                }
            }
        }
        if (gdata_mount != target || target == 0)
        {
            cell_removechara(
                cdata[target].position.x, cdata[target].position.y);
        }
        if (cdata[target].breaks_into_debris())
        {
            if (is_in_fov(target))
            {
                x = cdata[target].position.x;
                y = cdata[target].position.y;
                snd(45, false, false);
                animeblood(target, 1, ele_at_m141);
            }
            spillfrag(cdata[target].position.x, cdata[target].position.y, 3);
        }
        else
        {
            snd(8 + rnd(2), false, false);
            animeblood(target, 0, ele_at_m141);
            spillblood(cdata[target].position.x, cdata[target].position.y, 4);
        }
        if (cdata[target].character_role == 0)
        {
            cdata[target].state = 0;
        }
        else if (cdata[target].character_role == 13)
        {
            cdata[target].state = 4;
            cdata[target].time_to_revive = gdata_hour + gdata_day * 24
                + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12 + 24
                + rnd(12);
        }
        else
        {
            cdata[target].state = 2;
            cdata[target].time_to_revive = gdata_hour + gdata_day * 24
                + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12 + 48;
        }
        if (target != 0)
        {
            if (target < 16)
            {
                chara_mod_impression(target, -10);
                cdata[target].state = 6;
                cdata[target].current_map = 0;
                if (cdata[target].is_escorted() == 1)
                {
                    event_add(15, cdata[target].id);
                    cdata[target].state = 0;
                }
                if (cdata[target].is_escorted_in_sub_quest() == 1)
                {
                    cdata[target].state = 0;
                }
            }
        }
        if (target == 0)
        {
            ++gdata_death_count;
        }
        if (target == gdata(94))
        {
            gdata(94) = 0;
        }
        if (attacker >= 0)
        {
            if (attacker != 0)
            {
                chara_custom_talk(attacker, 103);
            }
            exp_at_m141 = clamp(cdata[target].level, 1, 200)
                    * clamp((cdata[target].level + 1), 1, 200)
                    * clamp((cdata[target].level + 2), 1, 200) / 20
                + 8;
            if (cdata[target].level > cdata[attacker].level)
            {
                exp_at_m141 /= 4;
            }
            if (cdata[target].splits() || cdata[target].splits2())
            {
                exp_at_m141 /= 20;
            }
            cdata[attacker].experience += exp_at_m141;
            if (attacker == 0)
            {
                gdata_sleep_experience += exp_at_m141;
            }
            cdata[attacker].hate = 0;
            if (attacker < 16)
            {
                cdata[attacker].enemy_id = 0;
                cdata[0].enemy_id = 0;
                gdata(94) = 0;
            }
        }
        if (target != 0)
        {
            if (gdata_current_map != 35)
            {
                if (gdata_current_map != 42)
                {
                    if (cdata[target].id == 2)
                    {
                        event_add(1);
                    }
                    if (cdata[target].id == 141)
                    {
                        txtef(2);
                        txt(lang(
                            u8"愚者の魔石を手に入れた！"s,
                            u8"You obtain the fool's magic stone!"s));
                        snd(51);
                        gdata_magic_stone_of_fool = 1;
                    }
                    if (cdata[target].id == 143)
                    {
                        txtef(2);
                        txt(lang(
                            u8"覇者の魔石を手に入れた！"s,
                            u8"You obtain the king's magic stone!"s));
                        snd(51);
                        gdata_magic_stone_of_king = 1;
                    }
                    if (cdata[target].id == 144)
                    {
                        txtef(2);
                        txt(lang(
                            u8"賢者の魔石を手に入れた！"s,
                            u8"You obtain the sage's magic stone!"s));
                        snd(51);
                        gdata_magic_stone_of_sage = 1;
                    }
                    if (cdata[target].id == 242)
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
                    if (cdata[target].id == 257)
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
                    if (cdata[target].id == 300)
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
                    if (cdata[target].id == 318)
                    {
                        event_add(
                            27,
                            cdata[target].position.x,
                            cdata[target].position.y);
                    }
                    if (cdata[target].id == 319)
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
                        if (adata(20, gdata_current_map) == target
                            && cdata[target].is_lord_of_dungeon() == 1)
                        {
                            event_add(5);
                        }
                    }
                    if (cdata[target].id == 331)
                    {
                        if (rnd(4) == 0)
                        {
                            event_add(
                                28,
                                cdata[target].position.x,
                                cdata[target].position.y);
                        }
                    }
                    quest_check();
                }
                else if (gdata_current_map == 42)
                {
                    if (adata(20, gdata_current_map) == target
                        && cdata[target].is_lord_of_dungeon() == 1)
                    {
                        event_add(5);
                    }
                }
            }
        }
        if (target != 0)
        {
            ++npcmemory(0, cdata[target].id);
            chara_custom_talk(target, 102);
            if (target < 16)
            {
                txt(lang(
                    u8"あなたは悲しくなった。"s,
                    u8"You feel sad for a moment."s));
            }
        }
        --gdata_other_character_count;
        if (gdata_mount)
        {
            if (target == gdata_mount)
            {
                txt(lang(
                    name(0) + u8"は"s + name(target) + u8"の死体から降りた。"s,
                    name(0) + u8" get off the corpse of "s + name(target)
                        + u8"."s));
                ride_end();
            }
        }
        check_kill(attacker, target);
        catitem = 0;
        rollanatomy = 0;
        if (rnd(60) == 0)
        {
            rollanatomy = 1;
        }
        if (attacker >= 0)
        {
            if (cdata[attacker].id == 260)
            {
                catitem = attacker;
            }
            if (int(std::sqrt(sdata(161, attacker))) > rnd(150))
            {
                rollanatomy = 1;
            }
            skillexp(161, attacker, 10 + rollanatomy * 4);
        }
        rc = target;
        label_1573();
        if (gdata_current_map == 40)
        {
            if (rnd(5) == 0)
            {
                snd(69);
            }
        }
        if (cdata[target].is_death_master() == 1)
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
                        buff_delete(p_at_m141, cnt);
                        --cnt;
                        continue;
                    }
                }
            }
        }
        if (attacker == 0)
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
    if (cdata[target].is_hung_on_sand_bag())
    {
        if (is_in_fov(target))
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
        gain_mana_capacity_experience(cc);
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
        dmgcon(cc, status_ailment_t::insane, 100);
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



}
