#include "damage.hpp"
#include "ability.hpp"
#include "animation.hpp"
#include "audio.hpp"
#include "buff.hpp"
#include "calc.hpp"
#include "character.hpp"
#include "class.hpp"
#include "config.hpp"
#include "debug.hpp"
#include "draw.hpp"
#include "elona.hpp"
#include "item.hpp"
#include "optional.hpp"
#include "item_db.hpp"
#include "map.hpp"
#include "trait.hpp"
#include "variables.hpp"

namespace elona
{

int damage_hp(int defender, int amount, int attacker, int element, int element_power)
{
    int local_element = element;
    int local_attacker = 0;
    int resistance = 0;
    int damage_level = 0;
    int total_damage = 0;
    int rollanatomy = 0;
    int catitem = 0;

    if (txt3rd == 0)
    {
        local_attacker = attacker;
    }
    else
    {
        local_attacker = -1;
    }
    if (cdata[defender].state != 1)
    {
        end_damage_hp(defender, total_damage);
        return 0;
    }
    total_damage = amount * (1 + (cdata[defender].furious > 0));
    if (attacker >= 0)
    {
        if (cdata[attacker].furious > 0)
        {
            total_damage *= 2;
        }
    }
    if (local_element != 0 && local_element < 61)
    {
        resistance = sdata(local_element, defender) / 50;
        if (resistance < 3)
        {
            total_damage =
                total_damage * 150 / clamp((resistance * 50 + 50), 40, 150);
        }
        else if (resistance < 10)
        {
            total_damage = total_damage * 100 / (resistance * 50 + 50);
        }
        else
        {
            total_damage = 0;
        }
        total_damage = total_damage * 100 / (sdata(60, defender) / 2 + 50);
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
    if (cdata[defender].wet > 0)
    {
        if (local_element == 50 || attacker == -9)
        {
            total_damage = total_damage / 3;
        }
        if (local_element == 52)
        {
            total_damage = total_damage * 3 / 2;
        }
    }
    if (local_element)
    {
        if (local_element != 60)
        {
            if (cdata[defender].is_immune_to_elemental_damage())
            {
                total_damage = 0;
            }
        }
    }
    if (cdata[defender].is_metal())
    {
        total_damage = rnd(total_damage / 10 + 2);
    }
    if (cdata[defender].is_contracting_with_reaper())
    {
        if (cdata[defender].hp - total_damage <= 0)
        {
            if (clamp(
                    25 + cdata[defender].buffs[findbuff(defender, 18)].power / 17,
                    25,
                    80)
                >= rnd(100))
            {
                total_damage *= -1;
            }
        }
    }
    if (cdata[defender].nullify_damage > 0)
    {
        if (cdata[defender].nullify_damage > rnd(100))
        {
            total_damage = 0;
        }
    }
    if (local_element == 658)
    {
        total_damage = amount;
    }
    elona::rtdmg = total_damage; // TODO

    if (defender == 0 && cdata[0].god_id == core_god::opatos)
    {
        total_damage = total_damage * 90 / 100;
    }

    if (debug::voldemort && defender == 0)
    {
        total_damage = 0;
    }
    cdata[defender].hp -= total_damage;


    if (is_in_fov(defender))
    {
        add_damage_popup(std::to_string(total_damage), defender, {0, 0, 0});
    }


    if (local_element == 56)
    {
        if (attacker >= 0)
        {
            if (total_damage > 0)
            {
                heal_hp(
                    attacker,
                    clamp(
                        rnd(total_damage * (150 + element_power * 2) / 1000 + 10),
                        1,
                        cdata[attacker].max_hp / 10 + rnd(5)));
            }
        }
    }
    if (defender == 0)
    {
        gdata(30) = 0;
        if (cdata[defender].hp < 0)
        {
            if (evid() != -1)
            {
                if (evid() != 21)
                {
                    cdata[defender].hp = 1;
                }
            }
            if (gdata_current_map == 40)
            {
                cdata[defender].hp = 1;
            }
        }
    }
    if (total_damage <= 0)
    {
        damage_level = -1;
    }
    else
    {
        damage_level = total_damage * 6 / cdata[defender].max_hp;
    }
    if (cdata[defender].hp < 0)
    {
        if (defender < 16)
        {
            for (int cnt = 0; cnt < 16; ++cnt)
            {
                if (defender == cnt)
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
                    name(defender) + u8"は回復した。"s,
                    name(defender) + u8" "s + is(defender) + u8" healed."s));
                cdata[defender].hp = cdata[defender].max_hp / 2;
                animode = 100 + defender;
                play_animation(19);
                snd(120);
                break;
            }
        }
        else if (cdata[defender].is_hung_on_sand_bag())
        {
            cdata[defender].hp = cdata[defender].max_hp;
        }
    }
    if (cdata[defender].hp >= 0)
    {
        if (damage_level > 1)
        {
            spillblood(
                cdata[defender].position.x,
                cdata[defender].position.y,
                1 + rnd(2));
        }
        if (gdata(809) == 1)
        {
            txteledmg(0, local_attacker, defender, local_element);
            goto label_1369_internal;
        }
        if (damage_level > 0)
        {
            if (cdata[defender].max_hp / 2 > cdata[defender].hp)
            {
                ++damage_level;
                if (cdata[defender].max_hp / 4 > cdata[defender].hp)
                {
                    ++damage_level;
                    if (cdata[defender].max_hp / 10 > cdata[defender].hp)
                    {
                        ++damage_level;
                    }
                }
            }
        }
        if (gdata(809) == 2)
        {
            txtcontinue();
            if (damage_level == -1)
            {
                txt(lang(
                    u8"かすり傷をつけた。"s,
                    u8"make"s + _s(local_attacker) + u8" a scratch."s));
            }
            if (damage_level == 0)
            {
                txtef(5);
                txt(lang(
                    u8"軽い傷を負わせた。"s,
                    u8"slightly wound"s + _s(local_attacker) + u8" "s + him(defender)
                        + u8"."s));
            }
            if (damage_level == 1)
            {
                txtef(11);
                txt(lang(
                    u8"傷つけた。"s,
                    u8"moderately wound"s + _s(local_attacker) + u8" "s
                        + him(defender) + u8"."s));
            }
            if (damage_level == 2)
            {
                txtef(10);
                txt(lang(
                    u8"深い傷を負わせた。"s,
                    u8"severely wound"s + _s(local_attacker) + u8" "s + him(defender)
                        + u8"."s));
            }
            if (damage_level >= 3)
            {
                txtef(3);
                txt(lang(
                    u8"致命傷を与えた。"s,
                    u8"critically wound"s + _s(local_attacker) + u8" "s
                        + him(defender) + u8"!"s));
            }
            rowact_check(defender);
            goto label_1369_internal;
        }
        if (damage_level == 1)
        {
            if (is_in_fov(defender))
            {
                txtef(11);
                txt(lang(
                    name(defender) + u8"は痛手を負った。"s,
                    name(defender) + u8" scream"s + _s(defender) + u8"."s));
            }
        }
        if (damage_level == 2)
        {
            if (is_in_fov(defender))
            {
                txtef(10);
                txt(lang(
                    name(defender) + u8"は苦痛にもだえた。"s,
                    name(defender) + u8" writhe"s + _s(defender)
                        + u8" in pain."s));
            }
        }
        if (damage_level >= 3)
        {
            if (is_in_fov(defender))
            {
                txtef(3);
                txt(lang(
                    name(defender) + u8"は悲痛な叫び声をあげた。"s,
                    name(defender) + u8" "s + is(defender)
                        + u8" severely hurt!"s));
            }
        }
        if (total_damage < 0)
        {
            if (cdata[defender].hp > cdata[defender].max_hp)
            {
                cdata[defender].hp = cdata[defender].max_hp;
            }
            if (is_in_fov(defender))
            {
                txtef(4);
                txt(lang(
                    name(defender) + u8"は回復した。"s,
                    name(defender) + u8" "s + is(defender) + u8" healed."s));
            }
        }
    label_1369_internal:
        rowact_check(defender);
        if (cdata[defender].hp < cdata[defender].max_hp / 5)
        {
            if (defender != 0)
            {
                if (cdata[defender].fear == 0)
                {
                    if (cdata[defender].is_immune_to_fear() == 0)
                    {
                        bool afraid = total_damage * 100 / cdata[defender].max_hp + 10 > rnd(200);
                        if (attacker == 0)
                        {
                            if (trait(44))
                            {
                                afraid = false;
                            }
                        }
                        if (afraid)
                        {
                            cdata[defender].fear = rnd(20) + 5;
                            if (is_in_fov(defender))
                            {
                                txtef(4);
                                txt(lang(
                                    name(defender) + u8"は恐怖して逃げ出した。"s,
                                    name(defender) + u8" run"s + _s(defender)
                                        + u8" away in terror."s));
                            }
                        }
                    }
                }
            }
        }
        if (local_element)
        {
            if (local_element == 59)
            {
                if (rnd(10) < element_power / 75 + 4)
                {
                    damage_status_effect(defender, 4, rnd(element_power / 3 * 2 + 1));
                }
                if (rnd(20) < element_power / 50 + 4)
                {
                    damage_status_effect(defender, 3, rnd(element_power / 3 * 2 + 1));
                }
                if (rnd(20) < element_power / 50 + 4)
                {
                    damage_status_effect(defender, 5, rnd(element_power / 3 * 2 + 1));
                }
                if (rnd(20) < element_power / 50 + 4)
                {
                    damage_status_effect(defender, 1, rnd(element_power / 3 * 2 + 1));
                }
                if (rnd(20) < element_power / 50 + 4)
                {
                    damage_status_effect(defender, 2, rnd(element_power / 3 * 2 + 1));
                }
            }
            if (local_element == 52)
            {
                if (rnd(3 + (cdata[defender].quality >= 4) * 3) == 0)
                {
                    ++cdata[defender].paralyzed;
                }
            }
            if (local_element == 53)
            {
                damage_status_effect(defender, 4, rnd(element_power + 1));
            }
            if (local_element == 58)
            {
                damage_status_effect(defender, 3, rnd(element_power + 1));
            }
            if (local_element == 54)
            {
                damage_status_effect(defender, 5, rnd(element_power + 1));
            }
            if (local_element == 57)
            {
                damage_status_effect(defender, 5, rnd(element_power + 1));
            }
            if (local_element == 55)
            {
                damage_status_effect(defender, 1, rnd(element_power + 1));
            }
            if (local_element == 61)
            {
                damage_status_effect(defender, 9, rnd(element_power + 1));
            }
            if (local_element == 62)
            {
                if (defender == 0)
                {
                    modify_corruption(rnd(element_power + 1));
                }
            }
            if (local_element == 63)
            {
                if (defender == 0 || rnd(3) == 0)
                {
                    item_acid(defender, -1);
                }
            }
        }
        if ((local_element == 50 || attacker == -9) && cdata[defender].wet == 0)
        {
            item_fire(defender, -1);
        }
        if (local_element == 51)
        {
            item_cold(defender, -1);
        }
        if (cdata[defender].sleep != 0)
        {
            if (local_element != 54 && local_element != 58 && local_element != 59)
            {
                cdata[defender].sleep = 0;
                txt(lang(
                    name(defender) + u8"は眠りを妨げられた。"s,
                    name(defender) + your(defender) + u8" sleep "s + is(defender)
                        + u8" disturbed."s));
            }
        }
        if (attacker == 0)
        {
            hostileaction(0, defender);
            gdata(94) = defender;
        }
        if (defender == 0)
        {
            if (cdata[defender].max_hp / 4 > cdata[defender].hp)
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
        if (cdata[defender].explodes())
        {
            if (rnd(3) == 0)
            {
                cdata[defender].will_explode_soon() = true;
                txtef(9);
                txt(lang(u8" *カチッ* "s, u8"*click*"s));
            }
        }
        if (cdata[defender].splits())
        {
            if (gdata(809) != 1)
            {
                if (total_damage > cdata[defender].max_hp / 20 || rnd(10) == 0)
                {
                    if (mdata(6) != 1)
                    {
                        int stat = copy_chara(defender);
                        if (stat == 1)
                        {
                            txt(lang(
                                name(defender) + u8"は分裂した！"s,
                                name(defender) + u8" split"s + _s(defender)
                                    + u8"!"s));
                        }
                    }
                }
            }
        }
        if (cdata[defender].splits2())
        {
            if (gdata(809) != 1)
            {
                if (rnd(3) == 0)
                {
                    if (cdata[defender].confused == 0
                        && cdata[defender].dimmed == 0
                        && cdata[defender].poisoned == 0
                        && cdata[defender].paralyzed == 0
                        && cdata[defender].blind == 0)
                    {
                        if (mdata(6) != 1)
                        {
                            int stat = copy_chara(defender);
                            if (stat == 1)
                            {
                                txt(lang(
                                    name(defender) + u8"は分裂した！"s,
                                    name(defender) + u8" split"s + _s(defender)
                                        + u8"!"s));
                            }
                        }
                    }
                }
            }
        }
        if (cdata[defender].is_quick_tempered())
        {
            if (gdata(809) != 1)
            {
                if (cdata[defender].furious == 0)
                {
                    if (rnd(20) == 0)
                    {
                        if (is_in_fov(defender))
                        {
                            txtef(4);
                            txt(lang(
                                name(defender) + u8"は怒りに体を奮わせた！"s,
                                name(defender) + u8" "s + is(defender)
                                    + u8" engulfed in fury!"s));
                        }
                        cdata[defender].furious += rnd(30) + 15;
                    }
                }
            }
        }
        if (attacker >= 0)
        {
            bool aggroed = false;
            if (cdata[defender].relationship <= -3)
            {
                if (cdata[attacker].original_relationship > -3)
                {
                    if (cdata[defender].hate == 0 || rnd(4) == 0)
                    {
                        aggroed = true;
                    }
                }
            }
            else if (cdata[attacker].original_relationship <= -3)
            {
                if (cdata[defender].hate == 0 || rnd(4) == 0)
                {
                    aggroed = true;
                }
            }
            if (attacker != 0)
            {
                if (cdata[attacker].enemy_id == defender)
                {
                    if (rnd(3) == 0)
                    {
                        aggroed = true;
                    }
                }
            }
            if (aggroed)
            {
                if (defender != 0)
                {
                    cdata[defender].enemy_id = attacker;
                    if (cdata[defender].hate == 0)
                    {
                        cdata[defender].emotion_icon = 218;
                        cdata[defender].hate = 20;
                    }
                    else
                    {
                        cdata[defender].hate += 2;
                    }
                }
            }
        }
    }
    if (cdata[defender].hp < 0)
    {
        int sound_id = eleinfo(local_element, 1);
        if (sound_id > 0)
        {
            snd(sound_id, false, false);
        }
        txtef(3);
        if (attacker >= 0)
        {
            if (local_element)
            {
                if (defender >= 16 && gdata(809) == 2)
                {
                    txtcontinue();
                    txteledmg(1, local_attacker, defender, local_element);
                }
                else
                {
                    txteledmg(2, local_attacker, defender, local_element);
                }
            }
            else
            {
                int death_kind = rnd(4);
                if (death_kind == 0)
                {
                    if (defender >= 16 && gdata(809) == 2)
                    {
                        txtcontinue();
                        txt(lang(
                            u8"粉々の肉片に変えた。"s,
                            u8"transform"s + _s(local_attacker) + u8" "s
                                + him(defender)
                                + u8" into several pieces of meat."s));
                    }
                    else
                    {
                        txt(lang(
                            name(defender) + u8"は粉々の肉片に変えられた。"s,
                            name(defender) + u8" "s + is(defender)
                                + u8" transformed into several pieces of meat."s));
                    }
                }
                else if (death_kind == 1)
                {
                    if (defender >= 16 && gdata(809) == 2)
                    {
                        txtcontinue();
                        txt(lang(
                            u8"破壊した。"s,
                            u8"destroy"s + _s(local_attacker) + u8" "s
                                + him(defender) + u8"."s));
                    }
                    else
                    {
                        txt(lang(
                            name(defender) + u8"は破壊された。"s,
                            name(defender) + u8" "s + is(defender)
                                + u8" killed."s));
                    }
                }
                else if (death_kind == 2)
                {
                    if (defender >= 16 && gdata(809) == 2)
                    {
                        txtcontinue();
                        txt(lang(
                            u8"ミンチにした。"s,
                            u8"mince"s + _s(local_attacker) + u8" "s + him(defender)
                                + u8"."s));
                    }
                    else
                    {
                        txt(lang(
                            name(defender) + u8"はミンチにされた。"s,
                            name(defender) + u8" "s + is(defender)
                                + u8" minced."s));
                    }
                }
                else if (death_kind == 3)
                {
                    if (defender >= 16 && gdata(809) == 2)
                    {
                        txtcontinue();
                        txt(lang(
                            u8"殺した。"s,
                            u8"kill"s + _s(local_attacker) + u8" "s + him(defender)
                                + u8"."s));
                    }
                    else
                    {
                        txt(lang(
                            name(defender) + u8"は殺された。"s,
                            name(defender) + u8" "s + is(defender)
                                + u8" slain."s));
                    }
                }
            }
            elona::ndeathcause = lang(
                cdatan(0, attacker) + u8"に殺された。"s,
                u8"was killed by "s + cdatan(0, attacker));
        }
        else
        {
            if (attacker == -11)
            {
                txt(lang(
                    name(defender) + u8"は見えざる手に葬られた。"s,
                    name(defender) + u8" "s + is(defender)
                        + u8" assassinated by the unseen hand."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"見えざる手に葬られた。"s,
                        u8"got assassinated by the unseen hand"s);
                }
            }
            if (attacker == -1)
            {
                txt(lang(
                    name(defender) + u8"は罠にかかって死んだ。"s,
                    name(defender) + u8" "s + is(defender)
                        + u8" caught in a trap and die"s + _s(defender)
                        + u8"."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"罠にかかって死んだ。"s,
                        u8"got caught in a trap and died"s);
                }
            }
            if (attacker == -2)
            {
                txt(lang(
                    name(defender) + u8"はマナの反動で死んだ。"s,
                    name(defender) + u8" die"s + _s(defender)
                        + u8" from over-casting."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"マナの反動で消滅した。"s,
                        u8"was completely wiped by magic reaction"s);
                }
            }
            if (attacker == -3)
            {
                txt(lang(
                    name(defender) + u8"は餓死した。"s,
                    name(defender) + u8" "s + is(defender)
                        + u8" starved to death."s));
                if (defender == 0)
                {
                    elona::ndeathcause =
                        lang(u8"飢え死にした。"s, u8"was starved to death"s);
                }
            }
            if (attacker == -4)
            {
                txt(lang(
                    name(defender) + u8"は毒に蝕まれ死んだ。"s,
                    name(defender) + u8" "s + is(defender)
                        + u8" killed with poison."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"毒にもがき苦しみながら死んだ。"s,
                        u8"miserably died from poison"s);
                }
            }
            if (attacker == -13)
            {
                txt(lang(
                    name(defender) + u8"は出血多量で死んだ。"s,
                    name(defender) + u8" die"s + _s(defender)
                        + u8" from loss of blood"s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"血を流しすぎて死んだ。"s,
                        u8"died from loss of blood"s);
                }
            }
            if (attacker == -5)
            {
                txt(lang(
                    name(defender) + u8"は呪いの力で死んだ。"s,
                    name(defender) + u8" die"s + _s(defender)
                        + u8" from curse."s));
                if (defender == 0)
                {
                    elona::ndeathcause =
                        lang(u8"呪い殺された。"s, u8"died from curse"s);
                }
            }
            if (attacker == -7)
            {
                txt(lang(
                    name(defender) + u8"は階段から転げ落ちて死んだ。"s,
                    name(defender) + u8" tumble"s + _s(defender)
                        + u8" from stairs and die"s + _s(defender) + u8"."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"階段から転げ落ちて亡くなった。"s,
                        u8"tumbled from stairs and died"s);
                }
            }
            if (attacker == -8)
            {
                txt(lang(
                    name(defender) + u8"は聴衆に殺された。"s,
                    name(defender) + u8" "s + is(defender)
                        + u8" killed by an audience."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"演奏中に激怒した聴衆に殺された。"s,
                        u8"was killed by an audience"s);
                }
            }
            if (attacker == -9)
            {
                txt(lang(
                    name(defender) + u8"は焼け死んだ。"s,
                    name(defender) + u8" "s + is(defender)
                        + u8" burnt and turned into ash."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"焼けて消滅した。"s,
                        u8"was burnt and turned into ash"s);
                }
            }
            if (attacker == -12)
            {
                txt(lang(
                    name(defender) + u8"は食中毒で死んだ。"s,
                    name(defender) + u8" "s + is(defender)
                        + u8" killed by food poisoning."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"食中毒で倒れた。"s,
                        u8"got killed by food poisoning"s);
                }
            }
            if (attacker == -14)
            {
                txt(lang(
                    name(defender) + u8"はエーテルに侵食され死んだ。"s,
                    name(defender) + u8" die"s + _s(defender)
                        + u8" of the Ether disease."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"エーテルの病に倒れた。"s,
                        u8"died of the Ether disease"s);
                }
            }
            if (attacker == -15)
            {
                txt(lang(
                    name(defender) + u8"は溶けて液体になった。"s,
                    name(defender) + u8" melt"s + _s(defender) + u8" down."s));
                if (defender == 0)
                {
                    elona::ndeathcause =
                        lang(u8"溶けて液体になった。"s, u8"melted down"s);
                }
            }
            if (attacker == -16)
            {
                txt(lang(
                    name(defender) + u8"はバラバラになった。"s,
                    name(defender) + u8" shatter"s + _s(defender) + u8"."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(u8"自殺した。"s, u8"committed suicide"s);
                }
            }
            if (attacker == -17)
            {
                txt(lang(
                    name(defender) + u8"は核爆発に巻き込まれて塵となった。"s,
                    name(defender) + u8" "s + is(defender)
                        + u8" turned into atoms."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"核爆発に巻き込まれて死んだ。"s,
                        u8"was killed by an atomic bomb"s);
                }
            }
            if (attacker == -18)
            {
                txt(lang(
                    name(defender)
                        + u8"はアイアンメイデンの中で串刺しになって果てた。"s,
                    name(defender) + u8" step"s + _s(defender)
                        + u8" in an iron maiden and die"s + _s(defender)
                        + u8"."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"アイアンメイデンにはさまれて死んだ。"s,
                        u8"stepped in an iron maiden and died"s);
                }
            }
            if (attacker == -19)
            {
                txt(lang(
                    name(defender) + u8"はギロチンで首をちょんぎられて死んだ。"s,
                    name(defender) + u8" "s + is(defender)
                        + u8" guillotined and die"s + _s(defender) + u8"."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"ギロチンで首を落とされて死んだ。"s,
                        u8"was guillotined"s);
                }
            }
            if (attacker == -20)
            {
                txt(lang(
                    name(defender) + u8"は首を吊った。"s,
                    name(defender) + u8" hang"s + _s(defender) + u8" "s
                        + his(defender) + u8"self."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"首を吊った。"s, u8"commited suicide by hanging"s);
                }
            }
            if (attacker == -21)
            {
                txt(lang(
                    name(defender) + u8"はもちを喉に詰まらせて死んだ。"s,
                    name(defender) + u8" choke"s + _s(defender)
                        + u8" on mochi and die."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang(
                        u8"もちを喉に詰まらせて死んだ。"s,
                        u8"ate mochi and died"s);
                }
            }
            if (attacker == -6)
            {
                int largest_weight = 0;
                int item_squashed_by = -1;
                for (const auto& cnt : items(0))
                {
                    if (inv[cnt].number == 0)
                    {
                        continue;
                    }
                    if (inv[cnt].weight > largest_weight)
                    {
                        item_squashed_by = cnt;
                        largest_weight = inv[cnt].weight;
                    }
                }
                if (item_squashed_by == -1)
                {
                    rtvaln = lang(u8"荷物"s, u8"backpack"s);
                }
                else
                {
                    rtvaln = itemname(item_squashed_by);
                }
                txt(lang(
                    name(defender) + u8"は"s + rtvaln
                        + u8"の重さに耐え切れず死んだ。"s,
                    name(defender) + u8" "s + is(defender) + u8" squashed by "s
                        + rtvaln + u8"."s));
                if (defender == 0)
                {
                    elona::ndeathcause = lang( // TODO
                        rtvaln + u8"の重さに耐え切れず潰れた。"s,
                        u8"was squashed by "s + rtvaln);
                }
            }
        }
        if (attacker == -9 || local_element == 50)
        {
            addmef(
                cdata[defender].position.x,
                cdata[defender].position.y,
                5,
                24,
                rnd(10) + 5,
                100,
                attacker);
        }
        if (local_element == 56)
        {
            if (attacker >= 0)
            {
                if (total_damage > 0)
                {
                    heal_hp(attacker, rnd(total_damage * (200 + element_power) / 1000 + 5));
                }
            }
        }
        if (gdata_mount != defender || defender == 0)
        {
            cell_removechara(
                cdata[defender].position.x, cdata[defender].position.y);
        }
        if (cdata[defender].breaks_into_debris())
        {
            if (is_in_fov(defender))
            {
                x = cdata[defender].position.x;
                y = cdata[defender].position.y;
                snd(45, false, false);
                animeblood(defender, 1, local_element);
            }
            spillfrag(cdata[defender].position.x, cdata[defender].position.y, 3);
        }
        else
        {
            snd(8 + rnd(2), false, false);
            animeblood(defender, 0, local_element);
            spillblood(cdata[defender].position.x, cdata[defender].position.y, 4);
        }
        if (cdata[defender].character_role == 0)
        {
            cdata[defender].state = 0;
        }
        else if (cdata[defender].character_role == 13)
        {
            cdata[defender].state = 4;
            cdata[defender].time_to_revive = gdata_hour + gdata_day * 24
                + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12 + 24
                + rnd(12);
        }
        else
        {
            cdata[defender].state = 2;
            cdata[defender].time_to_revive = gdata_hour + gdata_day * 24
                + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12 + 48;
        }
        if (defender != 0)
        {
            if (defender < 16)
            {
                modimp(defender, -10);
                cdata[defender].state = 6;
                cdata[defender].current_map = 0;
                if (cdata[defender].is_escorted() == 1)
                {
                    evadd(15, cdata[defender].id);
                    cdata[defender].state = 0;
                }
                if (cdata[defender].is_escorted_in_sub_quest() == 1)
                {
                    cdata[defender].state = 0;
                }
            }
        }
        if (defender == 0)
        {
            ++gdata_death_count;
        }
        if (defender == gdata(94))
        {
            gdata(94) = 0;
        }
        if (attacker >= 0)
        {
            if (attacker != 0)
            {
                customtalk(attacker, 103);
            }
            int exp_gained = clamp(cdata[defender].level, 1, 200)
                    * clamp((cdata[defender].level + 1), 1, 200)
                    * clamp((cdata[defender].level + 2), 1, 200) / 20
                + 8;
            if (cdata[defender].level > cdata[attacker].level)
            {
                exp_gained /= 4;
            }
            if (cdata[defender].splits() || cdata[defender].splits2())
            {
                exp_gained /= 20;
            }
            cdata[attacker].experience += exp_gained;
            if (attacker == 0)
            {
                gdata_sleep_experience += exp_gained;
            }
            cdata[attacker].hate = 0;
            if (attacker < 16)
            {
                cdata[attacker].enemy_id = 0;
                cdata[0].enemy_id = 0;
                gdata(94) = 0;
            }
        }
        if (defender != 0)
        {
            if (gdata_current_map != 35)
            {
                if (gdata_current_map != 42)
                {
                    if (cdata[defender].id == 2)
                    {
                        evadd(1);
                    }
                    if (cdata[defender].id == 141)
                    {
                        txtef(2);
                        txt(lang(
                            u8"愚者の魔石を手に入れた！"s,
                            u8"You obtain the fool's magic stone!"s));
                        snd(51);
                        gdata_magic_stone_of_fool = 1;
                    }
                    if (cdata[defender].id == 143)
                    {
                        txtef(2);
                        txt(lang(
                            u8"覇者の魔石を手に入れた！"s,
                            u8"You obtain the king's magic stone!"s));
                        snd(51);
                        gdata_magic_stone_of_king = 1;
                    }
                    if (cdata[defender].id == 144)
                    {
                        txtef(2);
                        txt(lang(
                            u8"賢者の魔石を手に入れた！"s,
                            u8"You obtain the sage's magic stone!"s));
                        snd(51);
                        gdata_magic_stone_of_sage = 1;
                    }
                    if (cdata[defender].id == 242)
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
                    if (cdata[defender].id == 257)
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
                    if (cdata[defender].id == 300)
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
                    if (cdata[defender].id == 318)
                    {
                        evadd(
                            27,
                            cdata[defender].position.x,
                            cdata[defender].position.y);
                    }
                    if (cdata[defender].id == 319)
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
                        if (adata(20, gdata_current_map) == defender
                            && cdata[defender].is_lord_of_dungeon() == 1)
                        {
                            evadd(5);
                        }
                    }
                    if (cdata[defender].id == 331)
                    {
                        if (rnd(4) == 0)
                        {
                            evadd(
                                28,
                                cdata[defender].position.x,
                                cdata[defender].position.y);
                        }
                    }
                    check_quest();
                }
                else if (gdata_current_map == 42)
                {
                    if (adata(20, gdata_current_map) == defender
                        && cdata[defender].is_lord_of_dungeon() == 1)
                    {
                        evadd(5);
                    }
                }
            }
        }
        if (defender != 0)
        {
            ++npcmemory(0, cdata[defender].id);
            customtalk(defender, 102);
            if (defender < 16)
            {
                txt(lang(
                    u8"あなたは悲しくなった。"s,
                    u8"You feel sad for a moment."s));
            }
        }
        --gdata_other_character_count;
        if (gdata_mount)
        {
            if (defender == gdata_mount)
            {
                txt(lang(
                    name(0) + u8"は"s + name(defender) + u8"の死体から降りた。"s,
                    name(0) + u8" get off the corpse of "s + name(defender)
                        + u8"."s));
                ride_end();
            }
        }
        check_kill(attacker, defender);
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
        rc = defender; // TODO see if used elsewhere globally
        drop_loot(defender, rollanatomy, catitem);
        if (gdata_current_map == 40)
        {
            if (rnd(5) == 0)
            {
                snd(69);
            }
        }
        if (cdata[defender].is_death_master() == 1)
        {
            txt(lang(
                u8"死の宣告は無効になった。"s, u8"The death word breaks."s));
            for (int chara = 0; chara < ELONA_MAX_CHARACTERS; ++chara)
            {
                if (cdata[chara].state != 1)
                {
                    continue;
                }
                for (int cnt = 0; cnt < 16; ++cnt)
                {
                    if (cdata[chara].buffs[cnt].id == 0)
                    {
                        break;
                    }
                    if (cdata[chara].buffs[cnt].id == 16)
                    {
                        delbuff(chara, cnt);
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
        end_damage_hp(defender, total_damage);
        return 0;
    }
    end_damage_hp(defender, total_damage);
    return 1;
}

void drop_loot(int rc, int rollanatomy, int catitem)
{
    int lootrich = 0;
    if (rc == 0)
    {
        if (gdata_executing_immediate_quest_type != 0)
        {
            return;
        }
        for (const auto& cnt : items(rc))
        {
            ci = cnt;
            if (inv[cnt].number == 0)
            {
                continue;
            }
            if (mdata(7) == 0)
            {
                if (inv[cnt].body_part != 0)
                {
                    continue;
                }
                if (ibit(5, ci))
                {
                    continue;
                }
                if (rnd(3))
                {
                    continue;
                }
            }
            else if (rnd(5))
            {
                continue;
            }
            if (the_item_db[inv[ci].id]->is_cargo == 1)
            {
                if (mdata(6) != 1 && mdata(6) != 5 && mdata(6) != 3
                    && mdata(6) != 4 && mdata(6) != 6 && mdata(6) != 2)
                {
                    continue;
                }
                else if (rnd(2))
                {
                    continue;
                }
            }
            f = 0;
            if (inv[ci].body_part != 0)
            {
                if (rnd(10))
                {
                    f = 1;
                }
                if (inv[ci].curse_state == curse_state_t::blessed)
                {
                    if (rnd(2))
                    {
                        f = 1;
                    }
                }
                if (is_cursed(inv[ci].curse_state))
                {
                    if (rnd(2))
                    {
                        f = 0;
                    }
                }
                if (inv[ci].curse_state == curse_state_t::doomed)
                {
                    if (rnd(2))
                    {
                        f = 0;
                    }
                }
            }
            else if (
                inv[ci].identification_state
                == identification_state_t::completely_identified)
            {
                if (rnd(4))
                {
                    f = 1;
                }
            }
            if (f)
            {
                continue;
            }
            if (inv[ci].body_part != 0)
            {
                cdata_body_part(rc, inv[ci].body_part) =
                    cdata_body_part(rc, inv[ci].body_part) / 10000 * 10000;
                inv[ci].body_part = 0;
            }
            f = 0;
            if (ibit(5, ci) == 0)
            {
                if (rnd(4) == 0)
                {
                    f = 1;
                }
                if (inv[ci].curse_state == curse_state_t::blessed)
                {
                    if (rnd(3) == 0)
                    {
                        f = 0;
                    }
                }
                if (is_cursed(inv[ci].curse_state))
                {
                    if (rnd(3) == 0)
                    {
                        f = 1;
                    }
                }
                if (inv[ci].curse_state == curse_state_t::doomed)
                {
                    if (rnd(3) == 0)
                    {
                        f = 1;
                    }
                }
            }
            if (f)
            {
                inv[ci].number = 0;
                continue;
            }
            inv[ci].position.x = cdata[rc].position.x;
            inv[ci].position.y = cdata[rc].position.y;
            int stat = item_stack(-1, ci);
            if (stat == 0)
            {
                ti = inv_getfreeid(-1);
                if (ti == -1)
                {
                    break;
                }
                item_copy(ci, ti);
                inv[ti].own_state = -2;
            }
            inv[ci].number = 0;
        }
        cell_refresh(cdata[rc].position.x, cdata[rc].position.y);
        create_pcpic(0, true);
        return;
    }
    else
    {
        if (rc < 16)
        {
            if (cdata[rc].has_own_sprite() == 1)
            {
                create_pcpic(rc, true);
            }
        }
        if (cdata[rc].relationship == 10)
        {
            return;
        }
    }
    if (gdata_current_map == 33)
    {
        if (cdata[rc].id == 185)
        {
            return;
        }
        if (cdata[rc].id == 204)
        {
            return;
        }
    }
    if (cdata[rc].is_contracting())
    {
        return;
    }
    if (cdata[rc].splits() || cdata[rc].splits2())
    {
        if (rnd(6))
        {
            return;
        }
    }
    for (const auto& cnt : items(rc))
    {
        if (inv[cnt].number == 0)
        {
            continue;
        }
        ci = cnt;
        f = 0;
        if (cdata[rc].character_role == 20)
        {
            break;
        }
        if (inv[ci].quality > 4 || inv[ci].id == 55)
        {
            f = 1;
        }
        if (rnd(30) == 0)
        {
            f = 1;
        }
        if (cdata[rc].quality >= 4)
        {
            if (rnd(2) == 0)
            {
                f = 1;
            }
        }
        if (cdata[rc].character_role == 13)
        {
            if (rnd(5))
            {
                f = 0;
            }
        }
        if (gdata_current_map == 6)
        {
            if (rnd(4))
            {
                f = 0;
            }
        }
        if (inv[ci].quality == 6)
        {
            f = 1;
        }
        if (ibit(12, ci))
        {
            f = 1;
        }
        if (f == 0)
        {
            continue;
        }
        if (catitem != 0)
        {
            if (ibit(8, ci) == 0)
            {
                if (the_item_db[inv[ci].id]->category < 50000)
                {
                    if (inv[ci].quality >= 3)
                    {
                        if (rnd(3))
                        {
                            txtef(9);
                            txt(lang(
                                name(catitem) + u8"は"s + itemname(ci)
                                    + u8"をぺろぺろと舐めた。"s,
                                name(catitem) + " licks " + itemname(ci)
                                    + "."));
                            ibitmod(8, ci, 1);
                            reftype = the_item_db[inv[ci].id]->category;
                            encadd(
                                ci,
                                randomenc(randomenclv(rnd(4))),
                                randomencp());
                            animeload(8, rc);
                        }
                    }
                }
            }
        }
        if (inv[ci].body_part != 0)
        {
            cdata_body_part(rc, inv[ci].body_part) =
                cdata_body_part(rc, inv[ci].body_part) / 10000 * 10000;
            inv[ci].body_part = 0;
        }
        inv[ci].position.x = cdata[rc].position.x;
        inv[ci].position.y = cdata[rc].position.y;
        itemturn(ci);
        int stat = item_stack(-1, ci);
        if (stat == 0)
        {
            ti = inv_getfreeid(-1);
            if (ti == -1)
            {
                break;
            }
            item_copy(ci, ti);
        }
        inv[ci].number = 0;
    }
    if (cdata[rc].quality >= 4 || rnd(20) == 0 || cdata[rc].drops_gold() == 1
        || rc < 16)
    {
        if (cdata[rc].gold > 0)
        {
            flt();
            itemcreate(
                -1,
                54,
                cdata[rc].position.x,
                cdata[rc].position.y,
                cdata[rc].gold / (1 + 3 * (cdata[rc].drops_gold() == 0)));
            cdata[rc].gold -=
                cdata[rc].gold / (1 + 3 * (cdata[rc].drops_gold() == 0));
        }
    }

    switch (access_class_info(16, cdatan(3, rc)))
    {
    case 1:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 7:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 3:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 2:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 53000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(40) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 54000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 4:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 5:
        if (rnd(50) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 54000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    }

    dbid = cdata[rc].id;
    dbmode = 16;
    dbspec = 3;
    switch (access_character_info())
    {
    case 3:
        if (rnd(40) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(40) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 53000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(40) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = fsetwear(rnd(length(fsetwear)));
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(40) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = fsetweapon(rnd(length(fsetweapon)));
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 68000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 1:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 62000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
            remain_make(ci, rc);
        }
        break;
    case 2:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 62000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
            remain_make(ci, rc);
        }
        break;
    case 6:
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 32000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 34000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 54000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 53000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 72000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 68000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 77000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 4:
        if (rnd(5) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = fsetwear(rnd(length(fsetwear)));
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(5) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = fsetweapon(rnd(length(fsetweapon)));
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 72000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(4) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 68000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 5:
        if (rnd(5) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = fsetwear(rnd(length(fsetwear)));
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(5) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = fsetweapon(rnd(length(fsetweapon)));
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(15) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 54000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(5) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(5) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 53000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 72000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(4) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 68000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(4) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 77000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    }

    if (rnd(40) == 0)
    {
        p = 0;
        flt(calcobjlv(cdata[tc].level), calcfixlv(2));
        flttypemajor = 62000;
        flttypeminor = 0;
        itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        remain_make(ci, rc);
    }
    if (gdata_current_map == 35)
    {
        cell_refresh(cdata[rc].position.x, cdata[rc].position.y);
        return;
    }
    if (gdata_current_map != 6 && cdata[rc].character_role != 20)
    {
        if (rnd(175) == 0 || cdata[rc].quality == 6 || 0
            || (cdata[rc].quality == 4 && rnd(2) == 0)
            || (cdata[rc].quality == 5 && rnd(3) == 0))
        {
            flt();
            itemcreate(-1, 504, cdata[rc].position.x, cdata[rc].position.y, 0);
            inv[ci].param1 = cdata[rc].image;
            inv[ci].subname = cdata[rc].id;
            cell_refresh(inv[ci].position.x, inv[ci].position.y);
        }
        if (rnd(175) == 0 || cdata[rc].quality == 6 || 0
            || (cdata[rc].quality == 4 && rnd(2) == 0)
            || (cdata[rc].quality == 5 && rnd(3) == 0))
        {
            flt();
            itemcreate(-1, 503, cdata[rc].position.x, cdata[rc].position.y, 0);
            inv[ci].param1 = cdata[rc].image;
            inv[ci].subname = cdata[rc].id;
            cell_refresh(inv[ci].position.x, inv[ci].position.y);
        }
    }
    if (cdata[rc].character_role == 1010)
    {
        flt();
        itemcreate(-1, 361, cdata[rc].position.x, cdata[rc].position.y, 0);
        inv[ci].param1 = cdata[rc].shop_store_id;
        inv[ci].own_state = 2;
    }
    if (rollanatomy == 1 || cdata[rc].quality >= 4 || 0
        || cdata[rc].is_livestock() == 1 || 0)
    {
        flt();
        int stat =
            itemcreate(-1, 204, cdata[rc].position.x, cdata[rc].position.y, 0);
        if (stat != 0)
        {
            remain_make(ci, rc);
            if (cdata[rc].is_livestock() == 1)
            {
                if (sdata(161, 0) != 0)
                {
                    inv[ci].number +=
                        rnd(1 + (sdata(161, 0) > cdata[rc].level));
                }
            }
        }
    }
    lootrich = -1;
    if (cdata[rc].id == 183)
    {
        lootrich = 8;
    }
    if (cdata[rc].id == 184)
    {
        lootrich = 4;
    }
    if (cdata[rc].id == 185)
    {
        lootrich = 2;
    }
    if (lootrich != -1)
    {
        for (int cnt = 0, cnt_end = (lootrich); cnt < cnt_end; ++cnt)
        {
            flt(calcobjlv(cdata[rc].level));
            flttypeminor = 77001;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(3) == 0)
        {
            flt();
            itemcreate(-1, 284, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    drop(0) = -1;
    drop(1) = -1;
    drop(2) = -1;
    drop(3) = -1;
    drop(4) = -1;
    drop(5) = -1;
    drop(6) = -1;
    drop(7) = -1;
    drop(8) = -1;
    drop(9) = -1;
    drop(10) = -1;
    if (cdata[rc].id == 140)
    {
        drop(0) = 355;
        drop(1) = -1;
    }
    if (cdata[rc].id == 175 || cdata[rc].id == 177 || cdata[rc].id == 178)
    {
        if (npcmemory(0, cdata[rc].id) == 0)
        {
            drop(0) = 290;
            drop(1) = 421;
            drop(2) = -1;
        }
        else
        {
            drop(0) = 421;
            drop(1) = 421;
            drop(2) = -1;
        }
    }
    if (gdata_current_map == 37)
    {
        if (cdata[rc].id == 257)
        {
            drop(0) = 665;
            drop(1) = 55;
            drop(2) = 55;
            drop(3) = 55;
            drop(4) = 55;
            drop(5) = 621;
            drop(6) = -1;
        }
    }
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        if (drop(cnt) == -1)
        {
            break;
        }
        flt();
        itemcreate(
            -1, drop(cnt), cdata[rc].position.x, cdata[rc].position.y, 0);
    }
    if (cdata[rc].id == 5)
    {
        if (rnd(200) == 0)
        {
            flt();
            itemcreate(-1, 702, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 21)
    {
        if (rnd(100) == 0)
        {
            flt();
            itemcreate(-1, 706, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 34)
    {
        if (rnd(4) == 0)
        {
            flt();
            itemcreate(-1, 717, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 99 || cdata[rc].id == 100 || cdata[rc].id == 101)
    {
        if (rnd(300) == 0)
        {
            flt();
            itemcreate(-1, 708, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 117)
    {
        if (rnd(140) == 0)
        {
            flt();
            itemcreate(-1, 720, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 168)
    {
        if (rnd(100) == 0)
        {
            flt();
            itemcreate(-1, 740, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id >= 302 && cdata[rc].id <= 305)
    {
        if (rnd(90) == 0)
        {
            flt();
            itemcreate(-1, 771, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 307 || cdata[rc].id == 308)
    {
        if (rnd(150) == 0)
        {
            flt();
            itemcreate(-1, 709, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 189 || cdata[rc].id == 190)
    {
        if (rnd(150) == 0)
        {
            flt();
            itemcreate(-1, 711, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 213)
    {
        if (rnd(50) == 0)
        {
            flt();
            int stat = itemcreate(
                -1, 672, cdata[rc].position.x, cdata[rc].position.y, 0);
            if (stat != 0)
            {
                inv[ci].param1 = 169;
            }
        }
    }
    if (cdata[rc].id == 250)
    {
        if (rnd(200) == 0)
        {
            flt();
            itemcreate(-1, 715, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 341)
    {
        if (rnd(3) == 0)
        {
            flt();
            itemcreate(-1, 635, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 85 || cdata[rc].id == 86 || cdata[rc].id == 87)
    {
        if (rnd(600) == 0)
        {
            flt();
            itemcreate(-1, 737, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 302)
    {
        for (int cnt = 0, cnt_end = (2 + rnd(4)); cnt < cnt_end; ++cnt)
        {
            flt(cdata[rc].level, 2);
            flttypemajor = 92000;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
            if (inv[ci].value < 800)
            {
                inv[ci].number = rnd(5) + 1;
            }
        }
    }
    if (cdata[rc].id == 328)
    {
        flt();
        itemcreate(
            -1, 55, cdata[rc].position.x, cdata[rc].position.y, 1 + rnd(3));
        if (rnd(2))
        {
            flt();
            itemcreate(-1, 622, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 329)
    {
        flt();
        itemcreate(
            -1,
            54,
            cdata[rc].position.x,
            cdata[rc].position.y,
            2500 + rnd((cdata[0].fame + 1000)));
    }
    if (cdata[rc].id == 351 || cdata[rc].id == 352)
    {
        for (int cnt = 0; cnt < 12; ++cnt)
        {
            flt();
            nostack = 1;
            int stat = itemcreate(
                -1, 772, cdata[rc].position.x, cdata[rc].position.y, 0);
            if (rnd(2))
            {
                if (stat != 0)
                {
                    inv[ci].param3 = -1;
                    inv[ci].image = 336;
                }
            }
        }
    }
    cell_refresh(cdata[rc].position.x, cdata[rc].position.y);
    if (cdata[rc].character_role == 13)
    {
        supply_new_equipment();
    }
    return;
}


void end_damage_hp(int defender, int total_damage)
{
    if (cdata[defender].is_hung_on_sand_bag())
    {
        if (is_in_fov(defender))
        {
            txt(u8"("s + total_damage + u8")"s + lang(u8" "s, ""s));
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

void damage_mp(int chara, int delta)
{
    cdata[chara].mp -= delta;
    if (cdata[chara].mp < -999999)
    {
        cdata[chara].mp = -999999;
    }
    if (cdata[chara].mp < 0)
    {
        gain_skill_experience_mana_capacity(chara);
        int damage = -cdata[chara].mp * 400 / (100 + sdata(164, chara) * 10);
        if (chara == 0)
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
            u8"マナの反動が"s + name(chara) + u8"の精神を蝕んだ！"s,
            u8"Magic reaction hurts "s + name(chara) + u8"!"s));
        damage_hp(chara, damage, -2);
    }
}

void damage_stamina(int chara, int delta)
{
    if (chara != 0 || debug::voldemort)
        return;

    if (cdata[chara].sp >= -100)
    {
        cdata[chara].sp -= delta;
    }
}

void heal_sanity(int chara, int delta)
{
    cdata[chara].insanity -= delta;
    if (cdata[chara].insanity < 0)
    {
        cdata[chara].insanity = 0;
    }
    return;
}

void damage_sanity(int chara, int delta)
{
    if (cdata[chara].quality >= 4)
        return;

    int resistance = std::max(sdata(54, chara) / 50, 1);
    if (resistance > 10)
        return;

    delta /= resistance;
    if (chara < 16)
    {
        if (trait(166))
        {
            delta -= rnd(4);
        }
    }
    delta = std::max(delta, 0);
    cdata[chara].insanity += delta;
    if (rnd(10) == 0 || rnd(delta + 1) > 5 || rnd(cdata[chara].insanity + 1) > 50)
    {
        damage_status_effect(chara, 11, 100);
    }
}

bool do_stamina_action(int chara, int sp)
{
    if (chara != 0 || debug::voldemort)
        return true;

    if (cdata[chara].sp < 50 && cdata[chara].sp < rnd(75))
    {
        damage_stamina(chara, sp);
        return false;
    }
    damage_stamina(chara, sp);
    return true;
}

void add_resistance(int chara, int element, int delta)
{
    if (delta >= 50)
    {
        txtef(2);
        switch (element)
        {
        case 50:
            txt(lang(
                name(chara) + u8"の身体は急に火照りだした。"s,
                u8"Suddenly, "s + name(chara) + u8" feel"s + _s(chara)
                    + u8" very hot."s));
            break;
        case 51:
            txt(lang(
                name(chara) + u8"の身体は急に冷たくなった。"s,
                u8"Suddenly, "s + name(chara) + u8" feel"s + _s(chara)
                    + u8" cool."s));
            break;
        case 52:
            txt(lang(
                name(chara) + u8"の身体に電気が走った。"s,
                name(chara) + u8" "s + is(chara)
                    + u8" struck by an electric shock."s));
            break;
        case 54:
            txt(lang(
                name(chara) + u8"は急に明晰になった。"s,
                u8"Suddenly, "s + name(chara) + your(chara)
                    + u8" mind becomes very clear."s));
            break;
        case 58:
            txt(lang(
                name(chara) + u8"は急に神経が図太くなった。"s,
                name(chara) + your(chara) + u8" nerve is sharpened."s));
            break;
        case 53:
            txt(lang(
                name(chara) + u8"は急に暗闇が怖くなくなった。"s,
                name(chara) + u8" no longer fear"s + _s(chara) + u8" darkness."s));
            break;
        case 57:
            txt(lang(
                name(chara) + u8"は騒音を気にしなくなった。"s,
                name(chara) + your(chara) + u8" eardrums get thick."s));
            break;
        case 59:
            txt(lang(
                name(chara) + u8"は急にカオスを理解した。"s,
                u8"Suddenly, "s + name(chara) + u8" understand"s + _s(chara)
                    + u8" chaos."s));
            break;
        case 55:
            txt(lang(
                name(chara) + u8"の毒への耐性は強くなった。"s,
                name(chara) + u8" now "s + have(chara)
                    + u8" antibodies to poisons."s));
            break;
        case 56:
            txt(lang(
                name(chara) + u8"の魂は地獄に近づいた。"s,
                name(chara) + u8" "s + is(chara) + u8" no longer afraid"s + _s(chara)
                    + u8" of hell."s));
            break;
        case 60:
            txt(lang(
                name(chara) + u8"の皮膚は魔力のオーラに包まれた。"s,
                name(chara) + your(chara)
                    + u8" body is covered by a magical aura."s));
            break;
        default: assert(0);
        }
    }
    else if (delta <= 50 * -1)
    {
        txtef(8);
        switch (element)
        {
        case 50:
            txt(lang(
                name(chara) + u8"は急に汗をかきだした。"s,
                name(chara) + u8" sweat"s + _s(chara) + u8"."s));
            break;
        case 51:
            txt(lang(
                name(chara) + u8"は急に寒気を感じた。"s,
                name(chara) + u8" shivers"s + _s(chara) + u8"."s));
            break;
        case 52:
            txt(lang(
                name(chara) + u8"は急に電気に敏感になった。"s,
                name(chara) + u8" "s + is(chara) + u8" shocked."s));
            break;
        case 54:
            txt(lang(
                name(chara) + u8"は以前ほど明晰ではなくなった。"s,
                name(chara) + your(chara) + u8" mind becomes slippery."s));
            break;
        case 58:
            txt(lang(
                name(chara) + u8"の神経は急に萎縮した。"s,
                name(chara) + u8" become"s + _s(chara) + u8" dull."s));
            break;
        case 53:
            txt(lang(
                name(chara) + u8"は急に暗闇が怖くなった。"s,
                u8"Suddenly, "s + name(chara) + u8" fear"s + _s(chara)
                    + u8" darkness."s));
            break;
        case 57:
            txt(lang(
                name(chara) + u8"は急に辺りをうるさく感じた。"s,
                name(chara) + u8" become"s + _s(chara)
                    + u8" very sensitive to noises."s));
            break;
        case 59:
            txt(lang(
                name(chara) + u8"はカオスへの理解を失った。"s,
                name(chara) + u8" no longer understand"s + _s(chara) + u8" chaos."s));
            break;
        case 55:
            txt(lang(
                name(chara) + u8"の毒への耐性は薄れた。"s,
                name(chara) + u8" lose"s + _s(chara, true)
                    + u8" antibodies to poisons."s));
            break;
        case 56:
            txt(lang(
                name(chara) + u8"の魂は地獄から遠ざかった。"s,
                name(chara) + u8" "s + is(chara) + u8" afraid of hell."s));
            break;
        case 60:
            txt(lang(
                name(chara) + u8"の皮膚から魔力のオーラが消えた。"s,
                u8"The magical aura disappears from "s + name(chara) + your(chara)
                    + u8" body."s));
            break;
        }
    }

    sdata.get(element, chara).original_level =
        clamp(sdata.get(element, chara).original_level + delta, 50, 200);
    snd(107);
    animeload(15, chara);

    refresh_character(chara);
}

void modify_corruption(int delta)
{
    int org_at_m134 = 0;
    int p_at_m134 = 0;
    int mod_at_m134 = 0;
    int cnt2_at_m134 = 0;
    int i_at_m134 = 0;
    org_at_m134 = gdata_ether_disease_stage / 1000;
    p_at_m134 = delta + (delta > 0) * gdata_ether_disease_speed;
    if (trait(168))
    {
        if (delta > 0)
        {
            p_at_m134 = p_at_m134 * 100 / 150;
        }
    }
    gdata_ether_disease_stage += p_at_m134;
    if (gdata_ether_disease_stage < 0)
    {
        gdata_ether_disease_stage = 0;
    }
    mod_at_m134 = gdata_ether_disease_stage / 1000 - org_at_m134;
    if (mod_at_m134 > 0)
    {
        if (org_at_m134 == 0)
        {
            txtef(8);
            txt(lang(
                u8"エーテルの病が発症した。"s,
                u8"The symptom of the Ether disease is shown up on you."s));
            if (config::instance().extrahelp)
            {
                if (gdata(215) == 0)
                {
                    if (mode == 0)
                    {
                        if (cdata[0].continuous_action_turn == 0)
                        {
                            gdata(215) = 1;
                            ghelp = 15;
                            show_ex_help();
                        }
                    }
                }
            }
        }
        if (org_at_m134 + mod_at_m134 >= 20)
        {
            p_at_m134 = 20 - org_at_m134;
        }
        else
        {
            p_at_m134 = mod_at_m134;
        }
        for (int cnt = 0, cnt_end = (p_at_m134); cnt < cnt_end; ++cnt)
        {
            cnt2_at_m134 = cnt;
            if (org_at_m134 + cnt2_at_m134 > 20)
            {
                break;
            }
            for (int cnt = 0; cnt < 100000; ++cnt)
            {
                await();
                int tid = rnd(17) + 200;
                int stat = get_trait_info(0, tid);
                if (stat == 0 || traitref != 3)
                {
                    continue;
                }
                if (trait(tid) <= traitref(1))
                {
                    continue;
                }
                --trait(tid);
                i_at_m134 = 700 + org_at_m134 + cnt2_at_m134;
                gdata(i_at_m134) = tid;
                txtef(8);
                txt(lang(
                    u8"あなたはエーテルに侵食された。"s,
                    u8"Your disease is getting worse."s));
                txtef(3);
                txt(traitrefn(1));
                if (tid == 203)
                {
                    body = 9;
                    label_2196(0);
                }
                if (tid == 205)
                {
                    body = 3;
                    label_2196(0);
                }
                if (tid == 206)
                {
                    body = 2;
                    label_2196(0);
                }
                break;
            }
        }
        animeload(8, 0);
        refresh_character(0);
        return;
    }
    if (mod_at_m134 < 0)
    {
        if (org_at_m134 + mod_at_m134 < 0)
        {
            p_at_m134 = org_at_m134;
        }
        else
        {
            p_at_m134 = std::abs(mod_at_m134);
        }
        if (p_at_m134 < 0)
        {
            p_at_m134 = 0;
        }
        for (int cnt = 0, cnt_end = (p_at_m134); cnt < cnt_end; ++cnt)
        {
            cnt2_at_m134 = cnt;
            for (int cnt = 0; cnt < 100000; ++cnt)
            {
                await();
                int tid = rnd(17) + 200;
                if (cnt == 0)
                {
                    i_at_m134 = 700 + org_at_m134 - cnt2_at_m134 - 1;
                    if (gdata(i_at_m134) != 0)
                    {
                        tid = gdata(i_at_m134);
                    }
                }
                int stat = get_trait_info(0, tid);
                if (stat == 0 || traitref != 3)
                {
                    continue;
                }
                if (trait(tid) >= 0)
                {
                    continue;
                }
                ++trait(tid);
                txtef(2);
                txt(lang(
                    u8"あなたのエーテルの侵食はやわらいだ。"s,
                    u8"The symptoms of the Ether disease seem to calm down."s));
                txtef(2);
                txt(traitrefn(0));
                break;
            }
        }
        animeload(10, 0);
        refresh_character(0);
        return;
    }
    return;
}

void wet(int chara, int turns)
{
    cdata[chara].wet += turns;
    if (is_in_fov(chara))
    {
        txt(lang(
            name(chara) + u8"は濡れた。"s,
            name(chara) + u8" get"s + _s(chara) + u8" wet."s));
        if (cdata[chara].is_invisible())
        {
            txt(lang(
                name(chara) + u8"の姿があらわになった。"s,
                name(chara) + u8" "s + is(chara) + u8" revealed "s + his(chara)
                    + u8" shape."s));
        }
    }
}

// TODO: in desperate need of DRY
int damage_status_effect(int chara, int status_effect, int effect_power)
{
    elona::cc = chara; // TODO is this needed?
    int resist_type = 0;
    bool succeeded = true;
    int adjusted_power = effect_power;
    optional<int> result = none;
    if (effect_power < 1)
    {
        return 0;
    }
    if (status_effect == 4)
    {
        if (cdata[chara].is_immune_to_blindness() == 1)
        {
            succeeded = false;
        }
        if (cdata[chara].quality > 3)
        {
            if (rnd(cdata[chara].level / 2 + 1))
            {
                succeeded = false;
            }
        }
        result = damage_status_effect_resist(chara, effect_power, 53, succeeded);
        if (!succeeded || !result)
        {
            return 0;
        }
        adjusted_power = *result / 6;
        if (adjusted_power > 0)
        {
            if (cdata[chara].blind == 0)
            {
                cdata[chara].blind = adjusted_power;
                if (is_in_fov(chara))
                {
                    txtef(8);
                    txt(lang(
                        name(chara) + u8"は盲目になった。"s,
                        name(chara) + u8" "s + is(chara) + u8" blinded."s));
                }
            }
            else
            {
                cdata[chara].blind += adjusted_power / 3 + 1;
            }
            rowactend(chara);
        }
        return 1;
    }
    if (status_effect == 5)
    {
        if (cdata[chara].is_immune_to_confusion() == 1)
        {
            succeeded = false;
        }
        if (findbuff(chara, 7) != -1)
        {
            succeeded = false;
        }
        if (cdata[chara].quality > 3)
        {
            if (rnd(cdata[chara].level / 2 + 1))
            {
                succeeded = false;
            }
        }
        result = damage_status_effect_resist(chara, effect_power, 54, succeeded);
        if (!succeeded || !result)
        {
            return 0;
        }
        adjusted_power = *result / 7;
        if (adjusted_power > 0)
        {
            if (cdata[chara].confused == 0)
            {
                cdata[chara].confused = adjusted_power;
                if (is_in_fov(chara))
                {
                    txtef(8);
                    txt(lang(
                        name(chara) + u8"は混乱した。"s,
                        name(chara) + u8" "s + is(chara)
                            + u8" confused."s));
                }
            }
            else
            {
                cdata[chara].confused += adjusted_power / 3 + 1;
            }
            rowactend(chara);
        }
        return 1;
    }
    if (status_effect == 3)
    {
        if (cdata[chara].is_immune_to_paralyzation() == 1)
        {
            succeeded = false;
        }
        if (cdata[chara].quality > 3)
        {
            if (rnd(cdata[chara].level + 1))
            {
                succeeded = false;
            }
        }
        result = damage_status_effect_resist(chara, effect_power, 58, succeeded);
        if (!succeeded || !result)
        {
            return 0;
        }
        adjusted_power = *result / 10;
        if (adjusted_power > 0)
        {
            if (cdata[chara].paralyzed == 0)
            {
                cdata[chara].paralyzed = adjusted_power;
                if (is_in_fov(chara))
                {
                    txtef(8);
                    txt(lang(
                        name(chara) + u8"は麻痺した。"s,
                        name(chara) + u8" "s + is(chara)
                            + u8" paralyzed."s));
                }
            }
            else
            {
                cdata[chara].paralyzed += adjusted_power / 3 + 1;
            }
            rowactend(chara);
        }
        return 1;
    }
    if (status_effect == 1)
    {
        if (cdata[chara].is_immune_to_poison() == 1)
        {
            succeeded = false;
        }
        if (cdata[chara].quality > 3)
        {
            if (rnd(cdata[chara].level / 3 + 1))
            {
                succeeded = false;
            }
        }
        result = damage_status_effect_resist(chara, effect_power, 55, succeeded);
        if (!succeeded || !result)
        {
            return 0;
        }
        adjusted_power = *result / 5;
        if (adjusted_power > 0)
        {
            if (cdata[chara].poisoned == 0)
            {
                cdata[chara].poisoned = adjusted_power;
                if (is_in_fov(chara))
                {
                    txtef(8);
                    txt(lang(
                        name(chara) + u8"は毒におかされた。"s,
                        name(chara) + u8" "s + is(chara)
                            + u8" poisoned."s));
                }
            }
            else
            {
                cdata[chara].poisoned += adjusted_power / 3 + 3;
            }
            rowactend(chara);
        }
        return 1;
    }
    if (status_effect == 2)
    {
        if (cdata[chara].is_immune_to_sleep() == 1)
        {
            succeeded = false;
        }
        if (cdata[chara].quality > 3)
        {
            if (rnd(cdata[chara].level / 5 + 1))
            {
                succeeded = false;
            }
        }
        result = damage_status_effect_resist(chara, effect_power, 58, succeeded);
        if (!succeeded || !result)
        {
            return 0;
        }
        adjusted_power = *result / 4;
        if (adjusted_power > 0)
        {
            if (cdata[chara].sleep == 0)
            {
                cdata[chara].sleep = adjusted_power;
                if (is_in_fov(chara))
                {
                    txtef(8);
                    txt(lang(
                        name(chara) + u8"は眠りにおちた。"s,
                        name(chara) + u8" fall"s + _s(chara)
                            + u8" asleep."s));
                }
            }
            else
            {
                cdata[chara].sleep += adjusted_power / 3 + 1;
            }
            rowactend(chara);
        }
        return 1;
    }
    if (status_effect == 6)
    {
        if (cdata[chara].is_immune_to_fear() == 1)
        {
            succeeded = false;
        }
        if (findbuff(chara, 1) != -1)
        {
            succeeded = false;
        }
        if (findbuff(chara, 7) != -1)
        {
            succeeded = false;
        }
        if (cdata[chara].quality > 3)
        {
            if (rnd(cdata[chara].level / 5 + 1))
            {
                succeeded = false;
            }
        }
        resist_type = 54;
        result = damage_status_effect_resist(chara, effect_power, 54, succeeded);
        if (!succeeded || !result)
        {
            return 0;
        }
        adjusted_power = *result / 7;
        if (adjusted_power > 0)
        {
            if (cdata[chara].fear == 0)
            {
                cdata[chara].fear = adjusted_power;
            }
            if (is_in_fov(chara))
            {
                txt(lang(
                    name(chara) + u8"は恐怖に侵された。"s,
                    name(chara) + u8" "s + is(chara) + u8" frightened."s));
            }
        }
        return 1;
    }
    if (status_effect == 7)
    {
        if (cdata[chara].quality > 3)
        {
            if (rnd(cdata[chara].level / 3 + 1))
            {
                succeeded = false;
            }
        }
        if (cdatan(2, chara) == u8"golem"s)
        {
            succeeded = false;
        }
        result = damage_status_effect_resist(chara, effect_power, 57, succeeded);
        if (!succeeded || !result)
        {
            return 0;
        }
        adjusted_power = *result / 8;
        if (adjusted_power > 0)
        {
            if (cdata[chara].dimmed == 0)
            {
                cdata[chara].dimmed = adjusted_power;
                if (is_in_fov(chara))
                {
                    txtef(8);
                    txt(lang(
                        name(chara) + u8"は朦朧とした。"s,
                        name(chara) + u8" "s + is(chara) + u8" dimmed."s));
                }
            }
            else
            {
                cdata[chara].dimmed += adjusted_power / 3 + 1;
            }
            rowactend(chara);
        }
        return 1;
    }
    if (status_effect == 9)
    {
        adjusted_power = effect_power / 25;
        if (cdata[chara].quality > 3)
        {
            adjusted_power /= 2;
        }
        if (adjusted_power > 0)
        {
            if (cdata[chara].bleeding == 0)
            {
                cdata[chara].bleeding = adjusted_power;
                if (is_in_fov(chara))
                {
                    txtef(8);
                    txt(lang(
                        name(chara) + u8"は血を流し始めた。"s,
                        name(chara) + u8" begin"s + _s(chara)
                            + u8" to bleed."s));
                }
            }
            else
            {
                cdata[chara].bleeding += adjusted_power;
            }
            rowactend(chara);
        }
        return 1;
    }
    if (status_effect == 8)
    {
        adjusted_power = effect_power / 10;
        if (adjusted_power > 0)
        {
            if (cdata[chara].drunk == 0)
            {
                cdata[chara].drunk = adjusted_power;
                if (is_in_fov(chara))
                {
                    txt(lang(
                        name(chara) + u8"は酔っ払った。"s,
                        name(chara) + u8" get"s + _s(chara)
                            + u8" drunk."s));
                }
            }
            else
            {
                cdata[chara].drunk += adjusted_power;
            }
        }
        return 1;
    }
    if (status_effect == 11)
    {
        adjusted_power = effect_power / 8;
        if (adjusted_power > 0)
        {
            if (cdata[chara].insane == 0)
            {
                cdata[chara].insane = adjusted_power;
                if (is_in_fov(chara))
                {
                    txtef(8);
                    txt(lang(
                        name(chara) + u8"は気が狂った。"s,
                        name(chara) + u8" become"s + _s(chara)
                            + u8" insane."s));
                }
            }
            else
            {
                cdata[chara].insane += adjusted_power / 3 + 1;
            }
            rowactend(chara);
        }
        return 1;
    }
    if (status_effect == 12)
    {
        adjusted_power = effect_power / 10;
        if (adjusted_power > 0)
        {
            if (cdata[chara].sick == 0)
            {
                cdata[chara].sick = adjusted_power;
                if (is_in_fov(chara))
                {
                    txtef(8);
                    txt(lang(
                        name(chara) + u8"は病気になった。"s,
                        name(chara) + u8" get"s + _s(chara) + u8" sick."s));
                }
            }
            else
            {
                cdata[chara].sick += adjusted_power / 10 + 1;
            }
        }
        return 1;
    }
    return 0;
}

optional<int> damage_status_effect_resist(int chara, int effect_power, int resistance, bool succeeded)
{
    int adjusted_resistance = 0;
    int adjusted_power = rnd(effect_power / 2 + 1) + effect_power / 2;
    adjusted_resistance = sdata(resistance, chara) / 50;
    adjusted_power = adjusted_power * 100 / (50 + adjusted_resistance * 50);
    if (adjusted_resistance >= 3)
    {
        if (adjusted_power < 40)
        {
            succeeded = false;
        }
    }
    if(succeeded)
    {
        return adjusted_power;
    }
    else {
        return none;
    }
}

void heal_hp(int chara, int delta)
{
    cdata[chara].hp += delta;
    if (cdata[chara].hp > cdata[chara].max_hp)
    {
        cdata[chara].hp = cdata[chara].max_hp;
    }
}


void heal_mp(int chara, int delta)
{
    cdata[chara].mp += delta;
    if (cdata[chara].mp > cdata[chara].max_mp)
    {
        cdata[chara].mp = cdata[chara].max_mp;
    }
}

void heal_stamina(int chara, int delta)
{
    cdata[chara].sp += delta;
    if (cdata[chara].sp > cdata[chara].max_sp)
    {
        cdata[chara].sp = cdata[chara].max_sp;
    }
}

void heal_status_effect(int target, int status_effect, int delta)
{
    if (status_effect == 6)
    {
        if (cdata[target].fear > 0)
        {
            if (delta == 0)
            {
                cdata[target].fear = 0;
            }
            else
            {
                cdata[target].fear -= delta;
            }
            if (cdata[target].fear <= 0)
            {
                cdata[target].fear = 0;
                if (is_in_fov(target))
                {
                    txt(lang(
                        name(target) + u8"は恐怖から立ち直った。"s,
                        name(target) + u8" shake"s + _s(target) + u8" off "s
                            + his(target) + u8" fear."s));
                }
            }
            return;
        }
    }
    if (status_effect == 4)
    {
        if (cdata[target].blind > 0)
        {
            if (delta == 0)
            {
                cdata[target].blind = 0;
            }
            else
            {
                cdata[target].blind -= delta;
            }
            if (cdata[target].blind <= 0)
            {
                cdata[target].blind = 0;
                if (is_in_fov(target))
                {
                    txt(lang(
                        name(target) + u8"は盲目から回復した。"s,
                        name(target) + u8" can see again."s));
                }
            }
            return;
        }
    }
    if (status_effect == 5)
    {
        if (cdata[target].confused > 0)
        {
            if (delta == 0)
            {
                cdata[target].confused = 0;
            }
            else
            {
                cdata[target].confused -= delta;
            }
            if (cdata[target].confused <= 0)
            {
                cdata[target].confused = 0;
                if (is_in_fov(target))
                {
                    txt(lang(
                        name(target) + u8"は混乱から回復した。"s,
                        name(target) + u8" recover"s + _s(target)
                            + u8" from confusion."s));
                }
            }
            return;
        }
    }
    if (status_effect == 3)
    {
        if (cdata[target].paralyzed > 0)
        {
            if (delta == 0)
            {
                cdata[target].paralyzed = 0;
            }
            else
            {
                cdata[target].paralyzed -= delta;
            }
            if (cdata[target].paralyzed <= 0)
            {
                cdata[target].paralyzed = 0;
                if (is_in_fov(target))
                {
                    txt(lang(
                        name(target) + u8"は麻痺から回復した。"s,
                        name(target) + u8" recover"s + _s(target)
                            + u8" from paralysis."s));
                }
            }
            return;
        }
    }
    if (status_effect == 1)
    {
        if (cdata[target].poisoned > 0)
        {
            if (delta == 0)
            {
                cdata[target].poisoned = 0;
            }
            else
            {
                cdata[target].poisoned -= delta;
            }
            if (cdata[target].poisoned <= 0)
            {
                cdata[target].poisoned = 0;
                if (is_in_fov(target))
                {
                    txt(lang(
                        name(target) + u8"は毒から回復した。"s,
                        name(target) + u8" recover"s + _s(target)
                            + u8" from poison."s));
                }
            }
            return;
        }
    }
    if (status_effect == 2)
    {
        if (cdata[target].sleep > 0)
        {
            if (delta == 0)
            {
                cdata[target].sleep = 0;
            }
            else
            {
                cdata[target].sleep -= delta;
            }
            if (cdata[target].sleep <= 0)
            {
                cdata[target].sleep = 0;
                if (is_in_fov(target))
                {
                    txt(lang(
                        name(target) + u8"は心地よい眠りから覚めた。"s,
                        name(target) + u8" awake"s + _s(target) + u8" from "s
                            + his(target) + u8" sleep."s));
                }
            }
            return;
        }
    }
    if (status_effect == 7)
    {
        if (cdata[target].dimmed > 0)
        {
            if (delta == 0)
            {
                cdata[target].dimmed = 0;
            }
            else
            {
                cdata[target].dimmed -= delta;
            }
            if (cdata[target].dimmed <= 0)
            {
                cdata[target].dimmed = 0;
                if (is_in_fov(target))
                {
                    txt(lang(
                        name(target) + u8"の意識ははっきりした。"s,
                        name(target) + u8" recover"s + _s(target)
                            + u8" from poison."s));
                }
            }
        }
    }
    if (status_effect == 9)
    {
        if (cdata[target].bleeding > 0)
        {
            if (delta == 0)
            {
                cdata[target].bleeding = 0;
            }
            else
            {
                cdata[target].bleeding -= delta;
            }
            if (cdata[target].bleeding <= 0)
            {
                cdata[target].bleeding = 0;
                if (is_in_fov(target))
                {
                    txt(lang(
                        name(target) + u8"の出血は止まった。"s,
                        name(target) + your(target) + u8" bleeding stops."s));
                }
            }
        }
    }
    if (status_effect == 8)
    {
        if (cdata[target].drunk > 0)
        {
            if (delta == 0)
            {
                cdata[target].drunk = 0;
            }
            else
            {
                cdata[target].drunk -= delta;
            }
            if (cdata[target].drunk <= 0)
            {
                cdata[target].drunk = 0;
                if (is_in_fov(target))
                {
                    txt(lang(
                        name(target) + u8"の酔いは覚めた。"s,
                        name(target) + u8" get"s + _s(target)
                            + u8" sober."s));
                }
            }
        }
    }
    if (status_effect == 11)
    {
        if (cdata[target].insane > 0)
        {
            if (delta == 0)
            {
                cdata[target].insane = 0;
            }
            else
            {
                cdata[target].insane -= delta;
            }
            if (cdata[target].insane <= 0)
            {
                cdata[target].insane = 0;
                if (is_in_fov(target))
                {
                    txt(lang(
                        name(target) + u8"は正気に戻った。"s,
                        name(target) + u8" come"s + _s(target) + u8" to "s
                            + yourself(target) + u8" again."s));
                }
            }
        }
    }
    if (status_effect == 12)
    {
        if (cdata[target].sick > 0)
        {
            if (delta == 0)
            {
                cdata[target].sick = 0;
            }
            else
            {
                cdata[target].sick -= delta;
            }
            if (cdata[target].sick <= 0)
            {
                cdata[target].sick = 0;
                if (is_in_fov(target))
                {
                    txt(lang(
                        name(target) + u8"の病気は治った。"s,
                        name(target) + u8" recover"s + _s(target)
                            + u8" from "s + his(target) + u8" illness."s));
                }
            }
        }
    }
    return;
}

} // namespace elona
