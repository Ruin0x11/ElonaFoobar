#include "status_ailment.hpp"
#include "ability.hpp"
#include "buff.hpp"
#include "character.hpp"
#include "element.hpp"
#include "elona.hpp"
#include "fov.hpp"
#include "map.hpp"
#include "variables.hpp"



namespace
{


int calc_power_decreased_by_resistance(int cc, int power, element_t element)
{
    const auto resistance_level = sdata(int(element), cc) / 50;
    power =
        (rnd(power / 2 + 1) + power / 2) * 100 / (50 + resistance_level * 50);

    if (resistance_level >= 3 && power < 40)
    {
        return 0;
    }
    else
    {
        return power;
    }
}


} // namespace



namespace elona
{



void dmgcon(int cc, status_ailment_t status_ailment, int power)
{
    if (power <= 0)
        return;

    int turn{};

    switch (status_ailment)
    {
    case status_ailment_t::blinded:
        if (cdata[cc].is_immune_to_blindness())
            return;
        if (cdata[cc].quality > 3 && rnd(cdata[cc].level / 2 + 1))
            return;
        power =
            calc_power_decreased_by_resistance(cc, power, element_t::darkness);
        turn = power / 6;
        if (turn <= 0)
            return;
        if (cdata[cc].blind == 0)
        {
            cdata[cc].blind = turn;
            if (is_in_fov(cc))
            {
                txtef(8);
                txt(lang(
                    name(cc) + u8"は盲目になった。"s,
                    name(cc) + u8" "s + is(cc) + u8" blinded."s));
            }
        }
        else
        {
            cdata[cc].blind += turn / 3 + 1;
        }
        rowactend(cc);
        return;
    case status_ailment_t::confused:
        if (cdata[cc].is_immune_to_confusion())
            return;
        if (buff_find(cc, 7) != -1)
            return;
        if (cdata[cc].quality > 3 && rnd(cdata[cc].level / 2 + 1))
            return;
        power = calc_power_decreased_by_resistance(cc, power, element_t::mind);
        turn = power / 7;
        if (turn <= 0)
            return;
        if (cdata[cc].confused == 0)
        {
            cdata[cc].confused = turn;
            if (is_in_fov(cc))
            {
                txtef(8);
                txt(lang(
                    name(cc) + u8"は混乱した。"s,
                    name(cc) + u8" "s + is(cc) + u8" confused."s));
            }
        }
        else
        {
            cdata[cc].confused += turn / 3 + 1;
        }
        rowactend(cc);
        return;
    case status_ailment_t::paralyzed:
        if (cdata[cc].is_immune_to_paralyzation())
            return;
        if (cdata[cc].quality > 3 && rnd(cdata[cc].level + 1))
            return;
        power = calc_power_decreased_by_resistance(cc, power, element_t::nerve);
        turn = power / 10;
        if (turn <= 0)
            return;
        if (cdata[cc].paralyzed == 0)
        {
            cdata[cc].paralyzed = turn;
            if (is_in_fov(cc))
            {
                txtef(8);
                txt(lang(
                    name(cc) + u8"は麻痺した。"s,
                    name(cc) + u8" "s + is(cc) + u8" paralyzed."s));
            }
        }
        else
        {
            cdata[cc].paralyzed += turn / 3 + 1;
        }
        rowactend(cc);
        return;
    case status_ailment_t::poisoned:
        if (cdata[cc].is_immune_to_poison())
            return;
        if (cdata[cc].quality > 3 && rnd(cdata[cc].level / 3 + 1))
            return;
        power =
            calc_power_decreased_by_resistance(cc, power, element_t::poison);
        turn = power / 5;
        if (turn <= 0)
            return;
        if (cdata[cc].poisoned == 0)
        {
            cdata[cc].poisoned = turn;
            if (is_in_fov(cc))
            {
                txtef(8);
                txt(lang(
                    name(cc) + u8"は毒におかされた。"s,
                    name(cc) + u8" "s + is(cc) + u8" poisoned."s));
            }
        }
        else
        {
            cdata[cc].poisoned += turn / 3 + 3;
        }
        rowactend(cc);
        return;
    case status_ailment_t::sleep:
        if (cdata[cc].is_immune_to_sleep())
            return;
        if (cdata[cc].quality > 3 && rnd(cdata[cc].level / 5 + 1))
            return;
        power = calc_power_decreased_by_resistance(cc, power, element_t::nerve);
        turn = power / 4;
        if (turn <= 0)
            return;
        if (cdata[cc].sleep == 0)
        {
            cdata[cc].sleep = turn;
            if (is_in_fov(cc))
            {
                txtef(8);
                txt(lang(
                    name(cc) + u8"は眠りにおちた。"s,
                    name(cc) + u8" fall"s + _s(cc) + u8" asleep."s));
            }
        }
        else
        {
            cdata[cc].sleep += turn / 3 + 1;
        }
        rowactend(cc);
        return;
    case status_ailment_t::fear:
        if (cdata[cc].is_immune_to_fear())
            return;
        if (buff_find(cc, 1) != -1)
            return;
        if (buff_find(cc, 7) != -1)
            return;
        if (cdata[cc].quality > 3 && rnd(cdata[cc].level / 5 + 1))
            return;
        power = calc_power_decreased_by_resistance(cc, power, element_t::mind);
        turn = power / 7;
        if (turn <= 0)
            return;
        if (cdata[cc].fear == 0)
        {
            cdata[cc].fear = turn;
        }
        if (is_in_fov(cc))
        {
            txt(lang(
                name(cc) + u8"は恐怖に侵された。"s,
                name(cc) + u8" "s + is(cc) + u8" frightened."s));
        }
        return;
    case status_ailment_t::dimmed:
        if (cdata[cc].quality > 3 && rnd(cdata[cc].level / 3 + 1))
            return;
        if (cdatan(2, cc) == u8"golem"s)
            return;
        power = calc_power_decreased_by_resistance(cc, power, element_t::sound);
        turn = power / 8;
        if (turn <= 0)
            return;
        if (cdata[cc].dimmed == 0)
        {
            cdata[cc].dimmed = turn;
            if (is_in_fov(cc))
            {
                txtef(8);
                txt(lang(
                    name(cc) + u8"は朦朧とした。"s,
                    name(cc) + u8" "s + is(cc) + u8" dimmed."s));
            }
        }
        else
        {
            cdata[cc].dimmed += turn / 3 + 1;
        }
        rowactend(cc);
        return;
    case status_ailment_t::bleeding:
        if (cdata[cc].quality > 3)
        {
            power /= 2;
        }
        turn = power / 25;
        if (turn <= 0)
            return;
        if (cdata[cc].bleeding == 0)
        {
            cdata[cc].bleeding = turn;
            if (is_in_fov(cc))
            {
                txtef(8);
                txt(lang(
                    name(cc) + u8"は血を流し始めた。"s,
                    name(cc) + u8" begin"s + _s(cc) + u8" to bleed."s));
            }
        }
        else
        {
            cdata[cc].bleeding += turn;
        }
        rowactend(cc);
        return;
    case status_ailment_t::drunk:
        turn = power / 10;
        if (turn <= 0)
            return;
        if (cdata[cc].drunk == 0)
        {
            cdata[cc].drunk = turn;
            if (is_in_fov(cc))
            {
                txt(lang(
                    name(cc) + u8"は酔っ払った。"s,
                    name(cc) + u8" get"s + _s(cc) + u8" drunk."s));
            }
        }
        else
        {
            cdata[cc].drunk += turn;
        }
        return;
    case status_ailment_t::insane:
        turn = power / 8;
        if (turn <= 0)
            return;
        if (cdata[cc].insane == 0)
        {
            cdata[cc].insane = turn;
            if (is_in_fov(cc))
            {
                txtef(8);
                txt(lang(
                    name(cc) + u8"は気が狂った。"s,
                    name(cc) + u8" become"s + _s(cc) + u8" insane."s));
            }
        }
        else
        {
            cdata[cc].insane += turn / 3 + 1;
        }
        rowactend(cc);
        return;
    case status_ailment_t::sick:
        turn = power / 10;
        if (turn <= 0)
            return;
        if (cdata[cc].sick == 0)
        {
            cdata[cc].sick = turn;
            if (is_in_fov(cc))
            {
                txtef(8);
                txt(lang(
                    name(cc) + u8"は病気になった。"s,
                    name(cc) + u8" get"s + _s(cc) + u8" sick."s));
            }
        }
        else
        {
            cdata[cc].sick += turn / 10 + 1;
        }
        return;
    default:
        throw std::runtime_error{u8"Unknown status ailment: "s
                                 + int(status_ailment)};
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
                if (is_in_fov(prm_827))
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
                if (is_in_fov(prm_827))
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
                if (is_in_fov(prm_827))
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
                if (is_in_fov(prm_827))
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
                if (is_in_fov(prm_827))
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
                if (is_in_fov(prm_827))
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
                if (is_in_fov(prm_827))
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
                if (is_in_fov(prm_827))
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
                if (is_in_fov(prm_827))
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
                if (is_in_fov(prm_827))
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
                if (is_in_fov(prm_827))
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



} // namespace elona
