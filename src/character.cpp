#include "character.hpp"
#include <cassert>
#include <type_traits>
#include "ability.hpp"
#include "calc.hpp"
#include "cat.hpp"
#include "character_status.hpp"
#include "class.hpp"
#include "ctrl_file.hpp"
#include "db_item.hpp"
#include "elona.hpp"
#include "equipment.hpp"
#include "fov.hpp"
#include "i18n.hpp"
#include "item.hpp"
#include "lua_env/lua_env.hpp"
#include "map_cell.hpp"
#include "quest.hpp"
#include "random.hpp"
#include "range.hpp"
#include "trait.hpp"
#include "variables.hpp"

using namespace elona;



namespace
{



int chara_create_internal()
{
    if (rc == -1)
    {
        rc = chara_get_free_slot();
        if (rc == -1)
        {
            rc = 56;
            return 0;
        }
    }
    chara_delete(rc);
    cequipment = 0;
    if (rc == 0)
    {
        p = 10;
    }
    else
    {
        p = 4;
    }
    if (dbid == -1)
    {
        if (fltselect == 0 && filtermax == 0 && fltnrace(0).empty())
        {
            if (fixlv == 3)
            {
                if (rnd(20) == 0)
                {
                    fltselect = 2;
                }
            }
            if (fixlv == 4)
            {
                if (rnd(10) == 0)
                {
                    fltselect = 2;
                }
            }
        }
        dbmode = 1;
        get_random_npc_id();
        if (dbid == 0)
        {
            if (fltselect == 2 || fixlv == 6)
            {
                fixlv = 4;
            }
            flt(objlv + 10, fixlv);
            dbmode = 1;
            get_random_npc_id();
        }
    }
    else if (dbid == 343)
    {
        if (usernpcmax > 0)
        {
            cdata[rc].cnpc_id = rnd(usernpcmax);
        }
    }

    cm = 1;
    cmshade = 0;
    ++npcmemory(1, dbid);
    if (dbid == 323)
    {
        if (rnd(5))
        {
            objlv *= 2;
            if (fixlv >= 4)
            {
                fixlv = 3;
            }
            cmshade = 1;
            flt(objlv, fixlv);
            dbmode = 1;
            get_random_npc_id();
        }
    }
    if (gdata_current_map == mdata_t::map_id_t::the_void)
    {
        if (!novoidlv)
        {
            voidlv = gdata_current_dungeon_level / 50 * 50;
        }
    }
    novoidlv = 0;
    if (dbid == 343)
    {
        create_cnpc();
    }
    else
    {
        dbmode = 3;
        access_character_info();
    }
    if (cmshade)
    {
        cdatan(0, rc) = i18n::s.get("core.locale.chara.job.shade");
        cdata[rc].image = 280;
    }
    cdata[rc].quality = fixlv;
    cdata[rc].index = rc;
    initialize_character();

    lua::lua->get_handle_manager().create_chara_handle_run_callbacks(cdata[rc]);

    rtval = rc;
    return 1;
}



bool is_in_map(const position_t& position)
{
    const auto x = position.x;
    const auto y = position.y;
    return 0 <= x && x < mdata_map_width && 0 <= y && y < mdata_map_height;
}



position_t get_random_position(const position_t& base_position, int n)
{
    return {base_position.x - rnd(n) + rnd(n),
            base_position.y - rnd(n) + rnd(n)};
}



optional<position_t> get_free_space(
    const position_t& base_position,
    int number_of_trials)
{
    for (int i = 0; i < number_of_trials; ++i)
    {
        const auto pos = get_random_position(base_position, 2);
        const auto x = pos.x;
        const auto y = pos.y;
        if (!is_in_map(pos))
        {
            continue;
        }
        if (map(x, y, 6) != 0)
        {
            if (chipm(7, map(x, y, 6) % 1000) & 4)
            {
                continue;
            }
        }
        if (map(x, y, 1) == 0)
        {
            if (!(chipm(7, map(x, y, 0)) & 4))
            {
                return pos;
            }
        }
    }

    return none;
}



int chara_get_free_slot_force()
{
    int ret = chara_get_free_slot();
    if (ret != -1)
    {
        return ret;
    }

    std::vector<int> slots;
    for (auto&& cc : cdata.others())
    {
        if (cc.state() == character::state_t::alive && cc.character_role == 0)
        {
            slots.push_back(cc.index);
        }
    }

    if (slots.empty())
    {
        // FIXME: do not throw exception.
        throw std::runtime_error("chara_get_free_slot_force() failed");
    }

    ret = choice(slots);
    // Force to destroy the character in `ret`.
    chara_vanquish(ret);
    return ret;
}



} // namespace



namespace
{



bool can_place_character_at(const position_t& position, bool allow_stairs)
{
    // Out of range
    if (position.x < 0 || mdata_map_width <= position.x || position.y < 0
        || mdata_map_height <= position.y)
        return false;

    // Wall
    if (chipm(7, map(position.x, position.y, 0)) & 4)
        return false;

    // There is someone.
    if (map(position.x, position.y, 1) != 0)
        return false;

    if (map(position.x, position.y, 6) != 0)
    {
        // There is an object which prevents from walking through.
        if (chipm(7, map(position.x, position.y, 6) % 1000) & 4)
            return false;

        cell_featread(position.x, position.y);
        // Upstairs/downstairs.
        if (feat(1) == 11 || feat(1) == 10)
        {
            if (!allow_stairs)
            {
                return false;
            }
        }
    }

    return true;
}



bool chara_place_internal(
    character& cc,
    optional<position_t> position,
    bool enemy_respawn)
{
    int x;
    int y;

    for (int i = 0;; ++i)
    {
        if (i == 99)
        {
            if (cc.index >= 57)
            {
                // Give up.
                return false;
            }
        }
        if (i > 99)
        {
            if (mdata_map_width == 0)
            {
                return false;
            }
            y = (i - 100) / mdata_map_width;
            x = (i - 100) % mdata_map_width;
            if (y >= mdata_map_height)
            {
                if (cc.index != 0)
                {
                    return false;
                }
                else
                {
                    // Make the cell placable.
                    x = rnd(mdata_map_width);
                    y = rnd(mdata_map_height);
                    // FIXME: I refered to oor, but I think it is not perfect.
                    // Break wall.
                    if (chipm(7, map(x, y, 0)) & 4)
                    {
                        map(x, y, 0) = tile_tunnel;
                    }
                    // Delete someone there.
                    // TODO: Work around. Need delete him/her *completely*.
                    if (map(x, y, 1) != 0)
                    {
                        map(x, y, 1) = 0;
                    }
                    if (map(x, y, 6) != 0)
                    {
                        cell_featread(x, y);
                        if (feat(1) == 21)
                        {
                            // Open closed doors.
                            cell_featset(x, y, tile_dooropen, 20, 0, -1);
                        }
                        else if (feat(1) == 22)
                        {
                            // Reveal hidden path.
                            map(x, y, 6) = 0;
                        }
                    }
                    assert(can_place_character_at({x, y}, true));
                }
            }
        }
        else
        {
            if (position)
            {
                x = position->x + rnd(i + 1) - rnd(i + 1);
                y = position->y + rnd(i + 1) - rnd(i + 1);
            }
            else
            {
                x = rnd(mdata_map_width - 4) + 2;
                y = rnd(mdata_map_height - 4) + 2;
            }
            if (enemy_respawn && i < 20)
            {
                const auto threshold = cdata.player().vision_distance / 2;
                if (std::abs(cdata.player().position.x - x) <= threshold
                    && std::abs(cdata.player().position.y - x) <= threshold)
                {
                    // Too close
                    continue;
                }
            }
        }

        if (can_place_character_at({x, y}, cc.index == 0 || position))
        {
            break;
        }
    }

    // Do place character.
    cc.initial_position = cc.position = {x, y};
    map(x, y, 1) = cc.index + 1;

    return true; // placed successfully.
}



void failed_to_place_character(character& cc)
{
    if (cc.index < 16)
    {
        cc.set_state(character::state_t::pet_in_other_map);
        txt(i18n::s.get("core.locale.chara.place_failure.ally", cc));
    }
    else
    {
        txt(i18n::s.get("core.locale.chara.place_failure.other", cc));
        cc.set_state(character::state_t::empty);
        chara_killed(cc);
        // Exclude town residents because they occupy character slots even
        // if they are dead.
        modify_crowd_density(cc.index, -1);
    }
    if (cc.character_role != 0)
    {
        cc.set_state(character::state_t::villager_dead);
        chara_killed(cc);
    }
    if (cc.character_role == 13)
    {
        cc.set_state(character::state_t::adventurer_dead);
        cc.time_to_revive = gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
            + gdata_year * 24 * 30 * 12 + 24 + rnd(12);
        chara_killed(cdata[cc], 4);
    }
}



} // namespace



namespace elona
{


character_db_ex the_character_db;
cdata_t cdata;


elona_vector1<std::string> usertxt;

static std::unordered_map<int, int> convert_resistances(
    const sol::table& data,
    const std::string& id)
{
    std::unordered_map<int, int> resistances;
    sol::optional<sol::table> value = data[id];

    if (value)
    {
        for (const auto& kvp : *value)
        {
            int k = std::stoi(kvp.first.as<std::string>().substr(1));
            int v = kvp.second.as<int>();
            resistances.emplace(k, v);
        }
    }


    return resistances;
}

static std::vector<int> convert_chara_flags(
    const sol::table& data,
    const std::string& id)
{
    std::vector<int> flag_types;
    sol::optional<sol::table> value = data[id];

    if (value)
    {
        for (const auto& kvp : *value)
        {
            std::string variant_name = kvp.second.as<std::string>();
            int variant_value = lua::lua->get_api_manager().get_enum_value(
                "CharaFlag", variant_name);
            flag_types.push_back(variant_value);
        }
    }

    return flag_types;
}

character_data character_db_ex::convert(
    const std::string& id_,
    const sol::table& data,
    lua::lua_env& lua)
{
    ELONA_LION_DB_FIELD_REQUIRED(id, int);
    ELONA_LION_DB_FIELD(ai_act_sub_freq, int, 0);
    ELONA_LION_DB_FIELD(ai_calm, int, 0);
    ELONA_LION_DB_FIELD(ai_dist, int, 0);
    ELONA_LION_DB_FIELD(ai_heal, int, 0);
    ELONA_LION_DB_FIELD(ai_move, int, 0);
    ELONA_LION_DB_FIELD(can_talk, int, 0);
    ELONA_LION_DB_FIELD_ENUM(color, "Color", "None");
    ELONA_LION_DB_FIELD(creaturepack, int, 0);
    ELONA_LION_DB_FIELD(cspecialeq, int, 0);
    ELONA_LION_DB_FIELD(damage_reaction_info, int, 0);
    ELONA_LION_DB_FIELD(item_type, int, 0);
    ELONA_LION_DB_FIELD(element_of_unarmed_attack, int, 0);
    ELONA_LION_DB_FIELD(eqammo_0, int, 0);
    ELONA_LION_DB_FIELD(eqammo_1, int, 0);
    ELONA_LION_DB_FIELD(eqmultiweapon, int, 0);
    ELONA_LION_DB_FIELD(eqrange_0, int, 0);
    ELONA_LION_DB_FIELD(eqrange_1, int, 0);
    ELONA_LION_DB_FIELD(eqring1, int, 0);
    ELONA_LION_DB_FIELD(eqtwohand, int, 0);
    ELONA_LION_DB_FIELD(eqweapon1, int, 0);
    ELONA_LION_DB_FIELD(female_image, int, 0);
    ELONA_LION_DB_FIELD(fixlv, int, 0);
    ELONA_LION_DB_FIELD(has_random_name, bool, false);
    ELONA_LION_DB_FIELD(image, int, 0);
    ELONA_LION_DB_FIELD(level, int, 0);
    ELONA_LION_DB_FIELD(male_image, int, 0);
    ELONA_LION_DB_FIELD_ENUM(original_relationship, "Relation", "Neutral");
    ELONA_LION_DB_FIELD(portrait, int, 0);
    ELONA_LION_DB_FIELD(race, std::string, "");
    ELONA_LION_DB_FIELD(class_, std::string, "");
    ELONA_LION_DB_FIELD_ENUM(sex, "Gender", "Random");
    ELONA_LION_DB_FIELD(fltselect, int, 0);
    ELONA_LION_DB_FIELD(category, int, 0);
    ELONA_LION_DB_FIELD(rarity, int, 10000);
    ELONA_LION_DB_FIELD(coefficient, int, 400);
    ELONA_LION_DB_FIELD_CALLBACK(corpse_eating_callback);

    std::unordered_map<int, int> resistances =
        convert_resistances(data, "resistances");

    std::vector<int> normal_actions;
    if (auto result = lion::convert_vector<int>(data, "normal_actions"))
    {
        normal_actions = *result;
    }
    else
    {
        normal_actions = {-1};
    }

    std::vector<int> special_actions;
    if (auto result = lion::convert_vector<int>(data, "special_actions"))
    {
        special_actions = *result;
    }

    std::vector<int> flag_types = convert_chara_flags(data, "flags");

    // TODO: cannot set bit flags off.
    decltype(character_data::_flags) flags;
    for (const auto& type : flag_types)
    {
        flags[type] = true;
    }

    // TODO: validate by regex/alphanum-only
    std::string filter = lion::convert_tags(data, "tags");

    return character_data{
        id,
        normal_actions,
        special_actions,
        ai_act_sub_freq,
        ai_calm,
        ai_dist,
        ai_heal,
        ai_move,
        can_talk,
        class_,
        static_cast<color_index_t>(color),
        creaturepack,
        cspecialeq,
        damage_reaction_info,
        item_type,
        element_of_unarmed_attack,
        eqammo_0,
        eqammo_1,
        eqmultiweapon,
        eqrange_0,
        eqrange_1,
        eqring1,
        eqtwohand,
        eqweapon1,
        female_image,
        filter,
        fixlv,
        has_random_name,
        image,
        level,
        male_image,
        original_relationship,
        portrait,
        race,
        sex,
        resistances,
        fltselect,
        category,
        rarity,
        coefficient,
        corpse_eating_callback,
        flags,
    };
}

character::character()
    : growth_buffs(10)
    , body_parts(30)
    , buffs(16)
    , attr_adjs(10)
{
}


void character::set_state(character::state_t new_state)
{
    this->state_ = new_state;
}


void character::clear()
{
    copy({}, *this);
}


void character::clear_flags()
{
    for (size_t i = 0; i < 32 * 30; ++i)
    {
        _flags[i] = false;
    }
}


cdata_t::cdata_t()
    : storage(ELONA_MAX_CHARACTERS)
{
    for (size_t i = 0; i < storage.size(); ++i)
    {
        storage[i].index = static_cast<int>(i);
    }
}



void initialize_character_filters()
{
    filter_creature(0) = u8"";
    filter_creature(1) = u8"/man/";
    filter_creature(2) = u8"/man/";
    filter_creature(3) = u8"/slime/";
    filter_creature(4) = u8"/fire/slime/";
    filter_creature(5) = u8"/wild/";
    filter_creature(6) = u8"/wild/";
    filter_creature(7) = u8"/man/";
    filter_creature(8) = u8"/man/";
    filter_creature(9) = u8"/man/";
    filter_creature(10) = u8"/wild/";
    filter_creature(11) = u8"/wild/";
    filter_creature(12) = u8"/man/";
    filter_creature(13) = u8"";
    filter_creature(14) = u8"";
    filter_creature(15) = u8"";
    filter_creature(16) = u8"/man/";
    filter_creature(17) = u8"";
    filter_creature(18) = u8"/man/";
    filter_creature(19) = u8"";
    filter_creature(20) = u8"";
    filter_creature(21) = u8"/undead/";
    filter_creature(22) = u8"";
    filter_creature(23) = u8"/man/god/";
    filter_creature(24) = u8"/man/";
    filter_creature(25) = u8"";
    filter_creature(26) = u8"/man/";
    filter_creature(27) = u8"/man/god/";
    filter_creature(28) = u8"/man/";
    filter_creature(29) = u8"/man/";
    filter_creature(30) = u8"/man/";
    filter_creature(31) = u8"/man/";
    filter_creature(32) = u8"";
    filter_creature(33) = u8"/man/";
    filter_creature(34) = u8"/man/";
    filter_creature(35) = u8"/man/";
    filter_creature(36) = u8"/man/";
    filter_creature(37) = u8"/man/";
    filter_creature(38) = u8"/man/";
    filter_creature(39) = u8"/man/";
    filter_creature(40) = u8"/man/";
    filter_creature(41) = u8"/man/";
    filter_creature(42) = u8"/wild/";
    filter_creature(43) = u8"/wild/";
    filter_creature(44) = u8"/wild/fire/dragon/";
    filter_creature(45) = u8"/fire/";
    filter_creature(46) = u8"";
    filter_creature(47) = u8"/undead/";
    filter_creature(48) = u8"/undead/";
    filter_creature(49) = u8"/undead/";
    filter_creature(50) = u8"/wild/";
    filter_creature(51) = u8"/wild/fire/";
    filter_creature(52) = u8"/wild/";
    filter_creature(53) = u8"/wild/";
    filter_creature(54) = u8"/wild/";
    filter_creature(55) = u8"/wild/";
    filter_creature(56) = u8"/wild/";
    filter_creature(57) = u8"/wild/";
    filter_creature(58) = u8"/wild/";
    filter_creature(59) = u8"/wild/";
    filter_creature(60) = u8"/wild/";
    filter_creature(61) = u8"/wild/";
    filter_creature(62) = u8"/wild/";
    filter_creature(63) = u8"/undead/";
    filter_creature(64) = u8"/undead/";
    filter_creature(65) = u8"";
    filter_creature(66) = u8"";
    filter_creature(67) = u8"";
    filter_creature(68) = u8"/dragon/";
    filter_creature(69) = u8"/man/";
    filter_creature(70) = u8"/man/";
    filter_creature(71) = u8"/man/";
    filter_creature(72) = u8"/man/";
    filter_creature(73) = u8"/man/";
    filter_creature(74) = u8"/man/";
    filter_creature(75) = u8"/man/";
    filter_creature(76) = u8"/man/";
    filter_creature(77) = u8"/man/";
    filter_creature(78) = u8"";
    filter_creature(79) = u8"/man/";
    filter_creature(80) = u8"/man/";
    filter_creature(81) = u8"";
    filter_creature(82) = u8"/fire/";
    filter_creature(83) = u8"";
    filter_creature(84) = u8"";
    filter_creature(85) = u8"/fire/";
    filter_creature(86) = u8"/god/";
    filter_creature(87) = u8"";
    filter_creature(88) = u8"/undead/";
    filter_creature(89) = u8"/undead/";
    filter_creature(90) = u8"/undead/";
    filter_creature(91) = u8"/undead/";
    filter_creature(92) = u8"/undead/";
    filter_creature(93) = u8"";
    filter_creature(94) = u8"";
    filter_creature(95) = u8"/wild/";
    filter_creature(96) = u8"/wild/";
    filter_creature(97) = u8"/fire/dragon/";
    filter_creature(98) = u8"/dragon/";
    filter_creature(99) = u8"/undead/";
    filter_creature(100) = u8"/undead/";
    filter_creature(101) = u8"/undead/";
    filter_creature(102) = u8"";
    filter_creature(103) = u8"";
    filter_creature(104) = u8"";
    filter_creature(105) = u8"";
    filter_creature(106) = u8"/undead/fire/";
    filter_creature(107) = u8"/undead/";
    filter_creature(108) = u8"/wild/";
    filter_creature(109) = u8"/wild/";
    filter_creature(110) = u8"/undead/";
    filter_creature(111) = u8"/undead/";
    filter_creature(112) = u8"/undead/";
    filter_creature(113) = u8"";
    filter_creature(114) = u8"/god/";
    filter_creature(115) = u8"/god/";
    filter_creature(116) = u8"/god/";
    filter_creature(117) = u8"/god/";
    filter_creature(118) = u8"/undead/";
    filter_creature(119) = u8"";
    filter_creature(120) = u8"/dragon/";
    filter_creature(121) = u8"/fire/dragon/";
    filter_creature(122) = u8"/dragon/";
    filter_creature(123) = u8"/dragon/";
    filter_creature(124) = u8"/undead/dragon/";
    filter_creature(125) = u8"/dragon/";
    filter_creature(126) = u8"/fire/";
    filter_creature(127) = u8"/wild/";
    filter_creature(128) = u8"/wild/";
    filter_creature(129) = u8"/wild/";
    filter_creature(130) = u8"/wild/";
    filter_creature(131) = u8"/undead/";
    filter_creature(132) = u8"";
    filter_creature(133) = u8"/fire/";
    filter_creature(134) = u8"/fire/";
    filter_creature(135) = u8"";
    filter_creature(136) = u8"";
    filter_creature(137) = u8"";
    filter_creature(138) = u8"";
    filter_creature(139) = u8"/man/";
    filter_creature(140) = u8"/dragon/";
    filter_creature(141) = u8"/undead/god/";
    filter_creature(142) = u8"/man/";
    filter_creature(143) = u8"";
    filter_creature(144) = u8"";
    filter_creature(145) = u8"";
    filter_creature(146) = u8"/man/";
    filter_creature(147) = u8"/fire/";
    filter_creature(148) = u8"/fire/";
    filter_creature(149) = u8"/fire/";
    filter_creature(150) = u8"/undead/";
    filter_creature(151) = u8"/undead/";
    filter_creature(152) = u8"/man/";
    filter_creature(153) = u8"/pawn/";
    filter_creature(154) = u8"/pawn/";
    filter_creature(155) = u8"/pawn/";
    filter_creature(156) = u8"/pawn/";
    filter_creature(157) = u8"";
    filter_creature(158) = u8"";
    filter_creature(159) = u8"/man/shopguard/";
    filter_creature(160) = u8"/man/shopguard/";
    filter_creature(161) = u8"/man/shopguard/";
    filter_creature(162) = u8"/man/";
    filter_creature(163) = u8"/man/";
    filter_creature(164) = u8"/wild/cat/";
    filter_creature(165) = u8"/wild/";
    filter_creature(166) = u8"/man/";
    filter_creature(167) = u8"/wild/";
    filter_creature(168) = u8"/wild/";
    filter_creature(169) = u8"/slime/";
    filter_creature(170) = u8"/man/";
    filter_creature(171) = u8"/man/";
    filter_creature(172) = u8"/man/";
    filter_creature(173) = u8"/man/";
    filter_creature(174) = u8"/man/sf/";
    filter_creature(175) = u8"/god/";
    filter_creature(176) = u8"/man/";
    filter_creature(177) = u8"/god/";
    filter_creature(178) = u8"/undead/god/";
    filter_creature(179) = u8"/man/";
    filter_creature(180) = u8"";
    filter_creature(181) = u8"/god/";
    filter_creature(182) = u8"/man/";
    filter_creature(183) = u8"/man/";
    filter_creature(184) = u8"/man/";
    filter_creature(185) = u8"/man/";
    filter_creature(186) = u8"";
    filter_creature(187) = u8"";
    filter_creature(188) = u8"";
    filter_creature(189) = u8"/undead/";
    filter_creature(190) = u8"/undead/";
    filter_creature(191) = u8"/undead/ether/";
    filter_creature(192) = u8"/wild/";
    filter_creature(193) = u8"/wild/ether/";
    filter_creature(194) = u8"/slime/";
    filter_creature(195) = u8"/wild/";
    filter_creature(196) = u8"/undead/";
    filter_creature(197) = u8"/undead/";
    filter_creature(198) = u8"/undead/";
    filter_creature(199) = u8"/undead/";
    filter_creature(200) = u8"/undead/";
    filter_creature(201) = u8"/undead/";
    filter_creature(202) = u8"";
    filter_creature(203) = u8"/man/";
    filter_creature(204) = u8"/man/";
    filter_creature(205) = u8"/man/";
    filter_creature(206) = u8"/man/";
    filter_creature(207) = u8"/god/";
    filter_creature(208) = u8"/man/";
    filter_creature(209) = u8"/man/";
    filter_creature(210) = u8"/man/";
    filter_creature(211) = u8"/man/";
    filter_creature(212) = u8"";
    filter_creature(213) = u8"/man/";
    filter_creature(214) = u8"/man/";
    filter_creature(215) = u8"/man/";
    filter_creature(216) = u8"";
    filter_creature(217) = u8"/man/";
    filter_creature(218) = u8"";
    filter_creature(219) = u8"";
    filter_creature(220) = u8"/god/";
    filter_creature(221) = u8"/man/";
    filter_creature(222) = u8"/man/";
    filter_creature(223) = u8"/man/";
    filter_creature(224) = u8"/man/";
    filter_creature(225) = u8"/wild/";
    filter_creature(226) = u8"/man/";
    filter_creature(227) = u8"/man/";
    filter_creature(228) = u8"/wild/cat/";
    filter_creature(229) = u8"/wild/cat/";
    filter_creature(230) = u8"/wild/cat/";
    filter_creature(231) = u8"/man/";
    filter_creature(232) = u8"";
    filter_creature(233) = u8"/man/";
    filter_creature(234) = u8"/man/";
    filter_creature(235) = u8"/man/";
    filter_creature(236) = u8"/yeek/";
    filter_creature(237) = u8"/yeek/";
    filter_creature(238) = u8"/yeek/";
    filter_creature(239) = u8"/man/";
    filter_creature(240) = u8"/yeek/";
    filter_creature(241) = u8"/yeek/";
    filter_creature(242) = u8"/yeek/";
    filter_creature(243) = u8"/man/";
    filter_creature(244) = u8"";
    filter_creature(245) = u8"";
    filter_creature(246) = u8"/wild/cat/";
    filter_creature(247) = u8"/man/";
    filter_creature(248) = u8"/dragon/";
    filter_creature(249) = u8"/man/";
    filter_creature(250) = u8"";
    filter_creature(251) = u8"";
    filter_creature(252) = u8"/man/";
    filter_creature(253) = u8"/man/";
    filter_creature(254) = u8"/undead/";
    filter_creature(255) = u8"/wild/";
    filter_creature(256) = u8"/wild/";
    filter_creature(257) = u8"/undead/";
    filter_creature(258) = u8"";
    filter_creature(259) = u8"/man/";
    filter_creature(260) = u8"/god/";
    filter_creature(261) = u8"/god/";
    filter_creature(262) = u8"/god/";
    filter_creature(263) = u8"/god/";
    filter_creature(264) = u8"/god/";
    filter_creature(265) = u8"/god/";
    filter_creature(266) = u8"/god/";
    filter_creature(267) = u8"/wild/horse/";
    filter_creature(268) = u8"/wild/horse/";
    filter_creature(269) = u8"/man/";
    filter_creature(270) = u8"/man/";
    filter_creature(271) = u8"/man/";
    filter_creature(272) = u8"/man/";
    filter_creature(273) = u8"/man/";
    filter_creature(274) = u8"/man/";
    filter_creature(275) = u8"/wild/horse/";
    filter_creature(276) = u8"/wild/horse/";
    filter_creature(277) = u8"/wild/horse/";
    filter_creature(278) = u8"";
    filter_creature(279) = u8"/man/";
    filter_creature(280) = u8"/man/";
    filter_creature(281) = u8"/dragon/";
    filter_creature(282) = u8"/mino/";
    filter_creature(283) = u8"";
    filter_creature(284) = u8"";
    filter_creature(285) = u8"/slime/";
    filter_creature(286) = u8"/slime/";
    filter_creature(287) = u8"";
    filter_creature(288) = u8"/man/";
    filter_creature(289) = u8"/man/";
    filter_creature(290) = u8"/man/";
    filter_creature(291) = u8"/man/";
    filter_creature(292) = u8"/man/";
    filter_creature(293) = u8"/man/";
    filter_creature(294) = u8"/man/";
    filter_creature(295) = u8"/man/";
    filter_creature(296) = u8"/mino/";
    filter_creature(297) = u8"/man/";
    filter_creature(298) = u8"/mino/";
    filter_creature(299) = u8"/mino/";
    filter_creature(300) = u8"/mino/";
    filter_creature(301) = u8"/man/";
    filter_creature(302) = u8"/man/rogue/";
    filter_creature(303) = u8"/man/rogue/";
    filter_creature(304) = u8"/man/rogue/";
    filter_creature(305) = u8"/man/rogue/";
    filter_creature(306) = u8"/god/";
    filter_creature(307) = u8"/undead/";
    filter_creature(308) = u8"/undead/";
    filter_creature(309) = u8"/god/";
    filter_creature(310) = u8"/god/";
    filter_creature(311) = u8"/god/";
    filter_creature(312) = u8"";
    filter_creature(313) = u8"";
    filter_creature(314) = u8"";
    filter_creature(315) = u8"";
    filter_creature(316) = u8"";
    filter_creature(317) = u8"";
    filter_creature(318) = u8"";
    filter_creature(319) = u8"";
    filter_creature(320) = u8"/man/";
    filter_creature(321) = u8"";
    filter_creature(322) = u8"/man/";
    filter_creature(323) = u8"/undead/";
    filter_creature(324) = u8"";
    filter_creature(325) = u8"";
    filter_creature(326) = u8"/man/";
    filter_creature(327) = u8"";
    filter_creature(328) = u8"";
    filter_creature(329) = u8"";
    filter_creature(330) = u8"/dragon/";
    filter_creature(331) = u8"/god/";
    filter_creature(332) = u8"/wild/cat/";
    filter_creature(333) = u8"/man/";
    filter_creature(334) = u8"/man/";
    filter_creature(335) = u8"/man/";
    filter_creature(336) = u8"/god/";
    filter_creature(337) = u8"/man/";
    filter_creature(338) = u8"/god/";
    filter_creature(339) = u8"/god/";
    filter_creature(340) = u8"";
    filter_creature(341) = u8"";
    filter_creature(342) = u8"/god/";
    filter_creature(343) = u8"";
    filter_creature(344) = u8"/wild/";
    filter_creature(345) = u8"";
    filter_creature(346) = u8"";
    filter_creature(347) = u8"/man/";
    filter_creature(348) = u8"/man/";
    filter_creature(349) = u8"/man/god/";
    filter_creature(350) = u8"/man/";
    filter_creature(351) = u8"/man/";
    filter_creature(352) = u8"/man/";
    filter_creature(353) = u8"/man/";
}

void chara_set_generation_filter()
{
    dbid = 0;
    if (gdata_current_map == mdata_t::map_id_t::cyber_dome)
    {
        flt(calcobjlv(10), calcfixlv(2));
        fltn(u8"sf"s);
        return;
    }
    if (mdata_map_type == mdata_t::map_type_t::town
        || mdata_map_type == mdata_t::map_type_t::guild)
    {
        flt(calcobjlv(10), calcfixlv(2));
        fltselect = 5;
        if (gdata_current_dungeon_level == 1)
        {
            if (gdata_current_map == mdata_t::map_id_t::yowyn)
            {
                if (rnd(2))
                {
                    dbid = 269;
                    return;
                }
            }
            if (gdata_current_map == mdata_t::map_id_t::noyel)
            {
                if (rnd(3) == 0)
                {
                    dbid = 270;
                    return;
                }
            }
            if (gdata_current_map == mdata_t::map_id_t::derphy)
            {
                if (rnd(3) == 0)
                {
                    dbid = 271;
                    return;
                }
                else if (rnd(2) == 0)
                {
                    dbid = 335;
                    return;
                }
            }
            if (gdata_current_map == mdata_t::map_id_t::lumiest)
            {
                if (rnd(3) == 0)
                {
                    dbid = 272;
                    return;
                }
            }
            if (gdata_current_map == mdata_t::map_id_t::vernis)
            {
                if (rnd(2))
                {
                    dbid = 273;
                    return;
                }
            }
            if (gdata_current_map == mdata_t::map_id_t::palmia)
            {
                if (rnd(3) == 0)
                {
                    dbid = 274;
                    return;
                }
            }
        }
        if (gdata_current_map == mdata_t::map_id_t::lumiest)
        {
            if (gdata_current_dungeon_level == 3)
            {
                dbid = 289;
            }
        }
        if (gdata_current_map == mdata_t::map_id_t::derphy)
        {
            if (gdata_current_dungeon_level == 3)
            {
                dbid = 293;
            }
        }
        if (gdata_current_map == mdata_t::map_id_t::port_kapul)
        {
            if (gdata_current_dungeon_level == 3)
            {
                dbid = 295;
            }
        }
        return;
    }
    if (gdata_current_map == mdata_t::map_id_t::lesimas)
    {
        flt(calcobjlv(gdata_current_dungeon_level), calcfixlv(2));
        if (gdata_current_dungeon_level < 4)
        {
            if (objlv > 5)
            {
                objlv = 5;
            }
        }
        return;
    }
    if (gdata_current_map == mdata_t::map_id_t::the_void)
    {
        flt(calcobjlv(gdata_current_dungeon_level % 50 + 5), calcfixlv(2));
        return;
    }
    if (gdata_current_map == mdata_t::map_id_t::dragons_nest)
    {
        flt(calcobjlv(gdata_current_dungeon_level), calcfixlv(2));
        return;
    }
    if (gdata_current_map == mdata_t::map_id_t::crypt_of_the_damned)
    {
        flt(calcobjlv(gdata_current_dungeon_level), calcfixlv(2));
        fltn(u8"undead"s);
        return;
    }
    if (gdata_current_map == mdata_t::map_id_t::tower_of_fire)
    {
        flt(calcobjlv(gdata_current_dungeon_level), calcfixlv(2));
        fltn(u8"fire"s);
        return;
    }
    if (gdata_current_map == mdata_t::map_id_t::ancient_castle)
    {
        flt(calcobjlv(gdata_current_dungeon_level), calcfixlv(2));
        if (rnd(2) == 0)
        {
            fltn(u8"man"s);
        }
        return;
    }
    if (gdata_current_map == mdata_t::map_id_t::pyramid)
    {
        flt(calcobjlv(gdata_current_dungeon_level), calcfixlv(2));
        flttypemajor = 13;
        return;
    }
    if (gdata_current_map == mdata_t::map_id_t::lumiest_graveyard
        || gdata_current_map == mdata_t::map_id_t::truce_ground)
    {
        flt(calcobjlv(20), calcfixlv(2));
        fltselect = 4;
        return;
    }
    if (gdata_current_map == mdata_t::map_id_t::quest)
    {
        if (gdata_executing_immediate_quest_type >= 1000)
        {
            flt(calcobjlv(qdata(5, gdata_executing_immediate_quest) + 1),
                calcfixlv(2));
        }
        if (gdata_executing_immediate_quest_type == 1006)
        {
            fltn(u8"wild"s);
            objlv = clamp(objlv / 4, 1, 8);
        }
        return;
    }
    if (adata(16, gdata_current_map) == mdata_t::map_id_t::yeeks_nest)
    {
        flt(calcobjlv(gdata_current_dungeon_level), calcfixlv(2));
        if (rnd(2))
        {
            fltn(u8"yeek"s);
        }
        return;
    }
    if (adata(16, gdata_current_map) == mdata_t::map_id_t::minotaurs_nest)
    {
        flt(calcobjlv(gdata_current_dungeon_level), calcfixlv(2));
        if (rnd(2))
        {
            fltn(u8"mino"s);
        }
        return;
    }
    if (mdata_map_type >= mdata_t::map_type_t::dungeon)
    {
        flt(calcobjlv(gdata_current_dungeon_level), calcfixlv(2));
        return;
    }
    if (adata(16, gdata_current_map) == mdata_t::map_id_t::museum
        || adata(16, gdata_current_map) == mdata_t::map_id_t::shop)
    {
        flt(calcobjlv(100), calcfixlv(2));
        if (rnd(1))
        {
            fltselect = 5;
        }
        else
        {
            fltselect = 7;
        }
        return;
    }
    flt(calcobjlv(cdata.player().level), calcfixlv(2));
    return;
}



bool chara_place()
{
    if (rc == -1)
        return false;

    if (rc == 56)
    {
        cdata[rc].set_state(character::state_t::empty);
        return false;
    }

    if (gdata_mount != 0 && gdata_mount == rc)
    {
        cdata[rc].position = cdata.player().position;
        return true;
    }

    const auto success = chara_place_internal(
        cdata[rc],
        cxinit >= 0 ? optional<position_t>({cxinit, cyinit}) : none,
        cxinit == -2);
    if (!success)
    {
        failed_to_place_character(cdata[rc]);
    }

    return success;
}



void initialize_character()
{
    if (mode != 1)
    {
        chara_add_quality_parens();
    }
    cdata[rc].set_state(character::state_t::alive);
    cdata[rc].interest = 100;
    cdata[rc].impression = 50;
    cdata[rc].vision_distance = 14;
    if (cdata[rc].id == 205)
    {
        cdata[rc].image = rnd(33) * 2 + cdata[rc].sex + 1;
    }
    if (rc == 0)
    {
        cdata[rc].nutrition = 9000;
    }
    else
    {
        cdata[rc].nutrition = 5000 + rnd(4000);
    }
    cdata[rc].height = cdata[rc].height + rnd((cdata[rc].height / 5 + 1))
        - rnd((cdata[rc].height / 5 + 1));
    cdata[rc].weight =
        cdata[rc].height * cdata[rc].height * (rnd(6) + 18) / 10000;
    update_required_experience(cdata[rc]);
    chara_init_common_skills(cdata[rc]);
    if (cdata[rc].portrait == 0)
    {
        cdata[rc].portrait = rnd(32);
    }
    cdata[rc].personality = rnd(4);
    cdata[rc].talk_type = rnd(7);
    supply_initial_equipments();
    chara_refresh(rc);
    modify_crowd_density(rc, 1);
    cdata[rc].hp = cdata[rc].max_hp;
    cdata[rc].mp = cdata[rc].max_mp;
    cdata[rc].sp = cdata[rc].max_sp;
    if (rc == 0)
    {
        gdata_initial_cart_limit = 80000;
        gdata_current_cart_limit = gdata_initial_cart_limit;
    }
    if (cdata[rc].has_lay_hand())
    {
        cdata[rc].is_lay_hand_available() = true;
    }
    cm = 0;
    return;
}



int chara_create(int prm_756, int prm_757, int prm_758, int prm_759)
{
    bool success = false;

    if (prm_758 == -3)
    {
        cxinit = -1;
        cyinit = -1;
    }
    else
    {
        cxinit = prm_758;
        cyinit = prm_759;
    }
    if (prm_757 == 0)
    {
        dbid = -1;
    }
    else
    {
        dbid = prm_757;
    }
    rc = prm_756;
    int stat = chara_create_internal();
    initlv = 0;
    voidlv = 0;
    if (stat == 1)
    {
        if (rc == 56)
        {
            cdata[rc].set_state(character::state_t::empty);
            --npcmemory(1, cdata[rc].id);
            return 1;
        }
        if (rc != 0)
        {
            if (prm_758 != -1)
            {
                success = chara_place();
            }
        }
    }
    else
    {
        rc = 56;
        return 0;
    }

    return success ? 1 : 0;
}



void chara_refresh(int cc)
{
    int rp = 0;
    int rp2 = 0;
    int rp3 = 0;
    if (cc == 0)
    {
        gdata_seven_league_boot_effect = 0;
        gdata_ether_disease_speed = 0;
        gdata_protects_from_etherwind = 0;
        gdata_protects_from_bad_weather = 0;
        gdata(89) = 70;
        gdata_catches_god_signal = 0;
        gdata_reveals_religion = 0;
    }
    for (int cnt = 0; cnt < 600; ++cnt)
    {
        sdata(cnt, cc) = sdata.get(cnt, cc).original_level;
    }
    if (cc == 0)
    {
        cdata[cc].clear_flags();
        if (trait(161) != 0)
        {
            for (int i = 0; i < 30; ++i)
            {
                if (cdata[cc].body_parts[i] % 10000 == 0)
                {
                    continue;
                }
                rp = cdata[cc].body_parts[i] % 10000 - 1;
                if (inv[rp].weight >= 1000)
                {
                    cdata[cc].body_parts[i] =
                        cdata[cc].body_parts[i] / 10000 * 10000;
                    inv[rp].body_part = 0;
                }
            }
        }
    }
    else if (cdata[cc].id == 343)
    {
        for (size_t i = 0; i < 32 * 30; ++i)
        {
            cdata[cc]._flags[i] =
                userdata(40 + i / (8 * sizeof(int)), cdata[cc].cnpc_id)
                & (1 << (i % (8 * sizeof(int))));
        }
    }
    else
    {
        for (size_t i = 0; i < 32 * 30; ++i)
        {
            cdata[cc]._flags[i] = the_character_db[cdata[cc].id]->_flags[i];
        }
    }
    for (auto&& growth_buff : cdata[cc].growth_buffs)
    {
        growth_buff = 0;
    }
    cdata[cc].dv = 0;
    cdata[cc].pv = 0;
    cdata[cc].hit_bonus = 0;
    cdata[cc].damage_bonus = 0;
    cdata[cc].equipment_type = 0;
    attacknum = 0;
    cdata[cc].rate_to_pierce = 0;
    cdata[cc].rate_of_critical_hit = 0;
    cdata[cc].curse_power = 0;
    cdata[cc].extra_attack = 0;
    cdata[cc].extra_shot = 0;
    cdata[cc].sum_of_equipment_weight = 0;
    cdata[cc].decrease_physical_damage = 0;
    cdata[cc].nullify_damage = 0;
    cdata[cc].cut_counterattack = 0;
    for (int i = 0; i < 30; ++i)
    {
        if (cdata[cc].body_parts[i] % 10000 == 0)
        {
            continue;
        }
        rp = cdata[cc].body_parts[i] % 10000 - 1;
        cdata[cc].sum_of_equipment_weight += inv[rp].weight;
        if (inv[rp].skill == 168)
        {
            if (!(cdata[cc].equipment_type & 1))
            {
                cdata[cc].equipment_type += 1;
            }
        }
        cdata[cc].dv += inv[rp].dv;
        cdata[cc].pv += inv[rp].pv;
        if (inv[rp].dice_x == 0)
        {
            cdata[cc].hit_bonus += inv[rp].hit_bonus;
            cdata[cc].damage_bonus += inv[rp].damage_bonus;
            cdata[cc].pv += inv[rp].enhancement * 2
                + (inv[rp].curse_state == curse_state_t::blessed) * 2;
        }
        else if (cdata[cc].body_parts[i] / 10000 == 5)
        {
            ++attacknum;
        }
        if (inv[rp].curse_state == curse_state_t::cursed)
        {
            cdata[cc].curse_power += 20;
        }
        if (inv[rp].curse_state == curse_state_t::doomed)
        {
            cdata[cc].curse_power += 100;
        }
        if (inv[rp].material == 8)
        {
            if (cc == 0)
            {
                gdata_ether_disease_speed += 5;
            }
        }
        for (int cnt = 0; cnt < 15; ++cnt)
        {
            if (inv[rp].enchantments[cnt].id == 0)
            {
                break;
            }
            rp2 = inv[rp].enchantments[cnt].id;
            if (rp2 >= 10000)
            {
                rp3 = rp2 % 10000;
                rp2 = rp2 / 10000;
                if (rp2 == 1)
                {
                    sdata(rp3, cc) += inv[rp].enchantments[cnt].power / 50 + 1;
                    continue;
                }
                if (rp2 == 2)
                {
                    sdata(rp3, cc) += inv[rp].enchantments[cnt].power / 2;
                    if (sdata(rp3, cc) < 0)
                    {
                        sdata(rp3, cc) = 1;
                    }
                    continue;
                }
                if (rp2 == 3)
                {
                    if (sdata.get(rp3, cc).original_level != 0)
                    {
                        sdata(rp3, cc) +=
                            inv[rp].enchantments[cnt].power / 50 + 1;
                        if (sdata(rp3, cc) < 1)
                        {
                            sdata(rp3, cc) = 1;
                        }
                    }
                    continue;
                }
            }
            else
            {
                if (rp2 == 56)
                {
                    if (cc == 0)
                    {
                        gdata_catches_god_signal = 1;
                        continue;
                    }
                }
                if (rp2 == 59)
                {
                    if (cc == 0)
                    {
                        gdata_reveals_religion = 1;
                        continue;
                    }
                }
                if (rp2 == 29)
                {
                    sdata(18, cc) += inv[rp].enchantments[cnt].power / 50 + 1;
                    if (cc == 0)
                    {
                        gdata_seven_league_boot_effect +=
                            inv[rp].enchantments[cnt].power / 8;
                        continue;
                    }
                }
                if (rp2 == 32)
                {
                    cdata[cc].is_floating() = true;
                    continue;
                }
                if (rp2 == 35)
                {
                    cdata[cc].can_see_invisible() = true;
                    continue;
                }
                if (rp2 == 23)
                {
                    cdata[cc].is_immune_to_blindness() = true;
                    continue;
                }
                if (rp2 == 24)
                {
                    cdata[cc].is_immune_to_paralyzation() = true;
                    continue;
                }
                if (rp2 == 25)
                {
                    cdata[cc].is_immune_to_confusion() = true;
                    continue;
                }
                if (rp2 == 26)
                {
                    cdata[cc].is_immune_to_fear() = true;
                    continue;
                }
                if (rp2 == 27)
                {
                    cdata[cc].is_immune_to_sleep() = true;
                    continue;
                }
                if (rp2 == 28)
                {
                    cdata[cc].is_immune_to_poison() = true;
                    continue;
                }
                if (rp2 == 42)
                {
                    cdata[cc].can_digest_rotten_food() = true;
                    continue;
                }
                if (rp2 == 41)
                {
                    cdata[cc].is_protected_from_thieves() = true;
                    continue;
                }
                if (rp2 == 55)
                {
                    cdata[cc].cures_bleeding_quickly() = true;
                    continue;
                }
                if (rp2 == 52)
                {
                    cdata[cc].decrease_physical_damage +=
                        inv[rp].enchantments[cnt].power / 40 + 5;
                    continue;
                }
                if (rp2 == 53)
                {
                    cdata[cc].nullify_damage +=
                        inv[rp].enchantments[cnt].power / 60 + 3;
                    continue;
                }
                if (rp2 == 54)
                {
                    cdata[cc].cut_counterattack +=
                        inv[rp].enchantments[cnt].power / 5;
                    continue;
                }
                if (rp2 == 44)
                {
                    cdata[cc].rate_of_critical_hit +=
                        inv[rp].enchantments[cnt].power / 50;
                    continue;
                }
                if (rp2 == 39)
                {
                    cdata[cc].rate_to_pierce +=
                        inv[rp].enchantments[cnt].power / 50;
                    continue;
                }
                if (rp2 == 50)
                {
                    cdata[cc].extra_attack +=
                        inv[rp].enchantments[cnt].power / 15;
                    continue;
                }
                if (rp2 == 51)
                {
                    cdata[cc].extra_shot +=
                        inv[rp].enchantments[cnt].power / 15;
                    cdata[cc].extra_shot = 100;
                    continue;
                }
                if (rp2 == 21 || rp2 == 45 || rp2 == 46 || rp2 == 47)
                {
                    cdata[cc].has_cursed_equipments() = true;
                    continue;
                }
                if (cc == 0)
                {
                    if (rp2 == 30)
                    {
                        gdata_protects_from_etherwind = 1;
                        continue;
                    }
                    if (rp2 == 31)
                    {
                        gdata_protects_from_bad_weather = 1;
                        continue;
                    }
                }
            }
        }
    }
    if (refreshmode == 1)
    {
        buff += u8"\n"s;
        buff += u8"<title1>◆ 装備による能力の修正<def>\n"s;
        for (int cnt = 0; cnt < 600; ++cnt)
        {
            sdata(cnt, 56) = sdata.get(cnt, cc).original_level;
            if (sdata(cnt, 56) != sdata(cnt, cc))
            {
                rp = sdata(cnt, cc) - sdata(cnt, 56);
                cnvbonus(cnt, rp);
            }
        }
    }
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        if (cdata[cc].attr_adjs[cnt] != 0)
        {
            if (cdata[cc].quality >= 4)
            {
                if (cdata[cc].attr_adjs[cnt]
                    < sdata.get(10 + cnt, cc).original_level / 5)
                {
                    cdata[cc].attr_adjs[cnt] =
                        sdata.get(10 + cnt, cc).original_level / 5;
                }
            }
            sdata(10 + cnt, cc) += cdata[cc].attr_adjs[cnt];
        }
        if (sdata(10 + cnt, cc) < 1)
        {
            sdata(10 + cnt, cc) = 1;
        }
    }
    if (cc == 0)
    {
        apply_god_blessing(cc);
        for (int cnt = 0; cnt < 217; ++cnt)
        {
            if (trait(cnt) != 0)
            {
                trait_get_info(1, cnt);
            }
        }
    }
    if (cdata[cc].equipment_type & 1)
    {
        if (cdata[cc].pv > 0)
        {
            cdata[cc].pv =
                cdata[cc].pv * (120 + int(std::sqrt(sdata(168, cc))) * 2) / 100;
        }
    }
    else if (attacknum == 1)
    {
        cdata[cc].equipment_type += 2;
    }
    else if (attacknum != 0)
    {
        cdata[cc].equipment_type += 4;
    }
    cdata[cc].max_mp =
        clamp(
            ((sdata(16, cc) * 2 + sdata(15, cc) + sdata(14, cc) / 3)
                 * cdata[cc].level / 25
             + sdata(16, cc)),
            1,
            1000000)
        * sdata(3, cc) / 100;
    cdata[cc].max_hp =
        clamp(
            ((sdata(11, cc) * 2 + sdata(10, cc) + sdata(15, cc) / 3)
                 * cdata[cc].level / 25
             + sdata(11, cc)),
            1,
            1000000)
            * sdata(2, cc) / 100
        + 5;
    cdata[cc].max_sp =
        100 + (sdata(15, cc) + sdata(11, cc)) / 5 + trait(24) * 8;
    if (cdata[cc].max_mp < 1)
    {
        cdata[cc].max_mp = 1;
    }
    if (cdata[cc].max_hp < 1)
    {
        cdata[cc].max_hp = 1;
    }
    if (cc >= ELONA_MAX_PARTY_CHARACTERS || false)
    {
        cdata[cc].dv = cdata[cc].level / 2
            + cdata[cc].dv * cdata[cc].dv_correction_value / 100
            + cdata[cc].dv_correction_value - 100;
        cdata[cc].pv = cdata[cc].level
            + (cdata[cc].pv + cdata[cc].level / 2
               + cdata[cc].pv_correction_value / 25)
                * cdata[cc].pv_correction_value / 100;
        if (cdata[cc].quality == 3)
        {
            cdata[cc].max_hp = cdata[cc].max_hp * 3 / 2;
        }
        if (cdata[cc].quality >= 4)
        {
            cdata[cc].max_hp = cdata[cc].max_hp * 5;
        }
    }
    else
    {
        cdata[cc].max_hp += 10;
    }
    for (int cnt = 0; cnt < 16; ++cnt)
    {
        rp = cdata[cc].buffs[cnt].id;
        if (rp == 0)
        {
            break;
        }
        apply_buff(cc, rp, cdata[cc].buffs[cnt].power);
    }
    if (cdata[cc].equipment_type & 4)
    {
        cdata[cc].extra_attack += int(std::sqrt(sdata(166, cc))) * 3 / 2 + 4;
    }
    if (sdata(186, cc))
    {
        cdata[cc].rate_of_critical_hit += int(std::sqrt(sdata(186, cc))) + 2;
    }
    if (cdata[cc].rate_of_critical_hit > 30)
    {
        cdata[cc].hit_bonus += (cdata[cc].rate_of_critical_hit - 30) * 2;
        cdata[cc].rate_of_critical_hit = 30;
    }
    refresh_burden_state();
    refresh_speed(cdata[cc]);
    cdata[cc].needs_refreshing_status() = false;

    auto handle = lua::lua->get_handle_manager().get_handle(cdata[cc]);
    if (handle != sol::lua_nil)
    {
        lua::lua->get_event_manager()
            .run_callbacks<lua::event_kind_t::character_refreshed>(handle);
    }
}



int relationbetween(int c1, int c2)
{
    if (cdata[c1].relationship >= -2)
    {
        if (cdata[c2].relationship <= -3)
        {
            return -3;
        }
    }
    else if (cdata[c2].relationship >= -2)
    {
        return -3;
    }
    return 0;
}



int chara_find(int id)
{
    for (auto&& i : cdata.others())
    {
        if (i.state() != character::state_t::villager_dead)
        {
            if (i.state() != character::state_t::alive)
            {
                continue;
            }
        }
        if (i.id == id)
        {
            return i.index;
        }
    }
    return 0;
}



int chara_find_ally(int id)
{
    for (int i = 0; i < 16; ++i)
    {
        if (cdata[i].state() != character::state_t::alive)
        {
            continue;
        }
        if (cdata[i].id == id)
        {
            return i;
        }
    }
    return -1;
}



int chara_get_free_slot()
{
    for (auto&& i : cdata.others())
    {
        if (i.state() == character::state_t::empty)
        {
            return i.index;
        }
    }
    return -1;
}



int chara_get_free_slot_ally()
{
    const auto max_allies = clamp(sdata(17, 0) / 5 + 1, 2, 15);
    for (int i = 1; i < max_allies + 1; ++i)
    {
        if (cdata[i].state() != character::state_t::empty)
        {
            continue;
        }
        return i;
    }
    return 0;
}


int chara_custom_talk(int cc, int talk_type)
{
    std::vector<std::string> talk_file_buffer;

    bool use_external_file = false;

    if (cdata[cc].has_custom_talk())
    {
        const auto filepath =
            filesystem::dir::user() / u8"talk" / cdatan(4, cc);
        if (!fs::exists(filepath))
            return 0;
        range::copy(
            fileutil::read_by_line{filepath},
            std::back_inserter(talk_file_buffer));
        use_external_file = true;
    }
    else if (cdata[cc].id == 343)
    {
        talk_file_buffer = strutil::split_lines(usertxt(cdata[cc].cnpc_id));
        use_external_file = true;
    }

    if (use_external_file)
    {
        std::string tag;
        switch (talk_type)
        {
        case 101: tag = u8"%txtAggro"s; break;
        case 103: tag = u8"%txtKilled"s; break;
        case 102: tag = u8"%txtDead"s; break;
        case 104: tag = u8"%txtWelcome"s; break;
        case 100: tag = u8"%txtCalm"s; break;
        case 106: tag = u8"%txtDialog"s; break;
        default: assert(0);
        }

        tag += u8"," + lang(u8"JP"s, u8"EN"s);
        const auto start_text = range::find_if(
            talk_file_buffer,
            [&](const auto& line) { return strutil::contains(line, tag); });
        if (start_text == std::end(talk_file_buffer))
        {
            throw std::runtime_error(u8"Error: fail to load custom talk");
        }
        const auto end_text = std::find_if(
            std::next(start_text),
            std::cend(talk_file_buffer),
            [](const auto& line) { return strutil::contains(line, u8"%"); });
        if (end_text == std::end(talk_file_buffer))
        {
            throw std::runtime_error(u8"Error: fail to load custom talk");
        }

        // Example:
        // %txtDialog,EN                              <- start_text
        // Elona foobar is one of the Elona variants.
        // Elona is one of the rogue-like games.
        // %END                                       <- end_text
        const auto number_of_lines_plus_1 = std::distance(start_text, end_text);
        if (number_of_lines_plus_1 == 1)
        {
            throw std::runtime_error(u8"Error: fail to load custom talk");
        }
        else
        {
            std::vector<std::string> lines;
            std::copy(
                std::next(start_text), end_text, std::back_inserter(lines));
            const auto line = lines[rnd(int(lines.size()))];
            if (!line.empty())
            {
                if (talk_type == 106)
                {
                    buff = line;
                }
                else
                {
                    txtef(9);
                    txt(line);
                }
            }
        }
        return 1;
    }

    if (talk_type == 106)
        return 0;

    if (cdata[cc].can_talk != 0)
    {
        dbid = cdata[cc].id;
        dbmode = talk_type;
        access_character_info();
        return 1;
    }
    return 0;
}



std::string chara_refstr(int prm_0258, int prm_0259)
{
    dbmode = 16;
    dbid = prm_0258;
    dbspec = prm_0259;
    access_character_info();
    return refstr;
}



int chara_impression_level(int impression)
{
    if (impression < 10)
        return 0;
    if (impression < 25)
        return 1;
    if (impression < 40)
        return 2;
    if (impression < 75)
        return 3;
    if (impression < 100)
        return 4;
    if (impression < 150)
        return 5;
    if (impression < 200)
        return 6;
    if (impression < 300)
        return 7;
    else
        return 8;
}



void chara_modify_impression(character& cc, int delta)
{
    int level1 = chara_impression_level(cc.impression);
    if (delta >= 0)
    {
        delta = delta * 100 / (50 + level1 * level1 * level1);
        if (delta == 0)
        {
            if (level1 < rnd(10))
            {
                delta = 1;
            }
        }
    }
    cc.impression += delta;
    int level2 = chara_impression_level(cc.impression);
    if (level1 > level2)
    {
        txtef(8);
        txt(i18n::s.get("core.locale.chara.impression.lose", cc, level2));
    }
    else if (level2 > level1)
    {
        if (cc.relationship != -3)
        {
            txtef(2);
            txt(i18n::s.get("core.locale.chara.impression.gain", cc, level2));
        }
    }
}



void chara_vanquish(int cc)
{
    if (cc == 0)
        return;

    if (cc == gdata_mount)
    {
        ride_end();
    }
    else if (
        cdata[cc].state() == character::state_t::alive
        || cdata[cc].state() == character::state_t::servant_being_selected)
    {
        map(cdata[cc].position.x, cdata[cc].position.y, 1) = 0;
    }
    cdata[cc].set_state(character::state_t::empty);
    cdata[cc].character_role = 0;
    if (cdata[cc].shop_store_id != 0)
    {
        const auto storage_filename = filesystem::u8path(
            "shop"s + std::to_string(cdata[cc].shop_store_id) + ".s2");
        const auto storage_filepath = filesystem::dir::tmp() / storage_filename;
        tmpload(storage_filename);
        if (fs::exists(storage_filepath))
        {
            fs::remove(storage_filepath);
            save_t::instance().remove(storage_filepath.filename());
        }
        cdata[cc].shop_store_id = 0;
    }
    quest_check();
    modify_crowd_density(cc, 1);
}



bool chara_copy(const character& source)
{
    // Find empty slot.
    const auto slot = chara_get_free_slot();
    if (slot == -1)
    {
        return false;
    }
    auto& destination = cdata[slot];

    // Find free space.
    const auto pos = get_free_space(source.position, 4);
    if (!pos)
    {
        return false;
    }
    const auto x = pos->x;
    const auto y = pos->y;

    // Delete completely the previous character in `slot`.
    chara_delete(slot);

    // Copy from `source` to `destination`.
    character::copy(source, destination);
    sdata.copy(slot, source.index);
    for (int i = 0; i < 10; ++i)
    {
        cdatan(i, slot) = cdatan(i, source.index);
    }

    // Place `destination` to the found free space.
    map(x, y, 1) = slot + 1;
    destination.position = *pos;

    // Reset some fields which should not be copied.
    destination.impression = 0;
    destination.gold = 0;
    destination.original_relationship = -3;
    destination.has_own_sprite() = false;
    destination.is_livestock() = false;
    destination.is_married() = false;
    destination.is_ridden() = false;
    destination.needs_refreshing_status() = true;
    destination.is_hung_on_sand_bag() = false;

    // Unequip all gears.
    for (size_t i = 0; i < destination.body_parts.size(); ++i)
    {
        destination.body_parts[i] = destination.body_parts[i] / 10000 * 10000;
    }

    // Increase crowd density.
    modify_crowd_density(slot, 1);
    // Increase the generation counter.
    ++npcmemory(1, destination.id);

    return true;
}

void chara_remove(character& chara)
{
    chara.state = 0;
    lua::lua->get_handle_manager().remove_chara_handle_run_callbacks(chara);
}

void chara_killed(character& chara, int new_state)
{
    // Run the character killed lua callback if the character was not dead to
    // begin with.
    if (chara.state != 0 && chara.state != 2 && chara.state != 4
        && chara.state != 6)
    {
        auto handle = lua::lua->get_handle_manager().get_handle(chara);
        lua::lua->get_event_manager()
            .run_callbacks<lua::event_kind_t::character_killed>(handle);
    }

    // The provided state must be 0, 2, 4 or 6.
    chara.state = new_state;

    if (chara.state() == character::state_t::empty)
    {
        // This character slot is invalid, and can be overwritten by
        // newly created characters at any time. Run any Lua callbacks
        // to clean up character things, if there is a valid Lua
        // handle for it.
        lua::lua->get_handle_manager().remove_chara_handle_run_callbacks(chara);
    }
    else if (
        chara.state() == character::state_t::villager_dead
        || chara.state() == character::state_t::adventurer_dead
        || chara.state() == character::state_t::pet_dead)
    {
        // This character revives.
    }
    else
    {
        assert(false);
    }
}



void chara_delete(int cc)
{
    if (cc != -1)
    {
        chara_remove(cdata[cc]);
    }

    for (const auto& cnt : items(cc))
    {
        inv[cnt].remove();
    }
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        cdatan(cnt, cc) = "";
    }
    sdata.clear(cc);
    cdata[cc].clear();
}



void chara_relocate(
    character& source,
    optional<int> destination_slot,
    chara_relocate_mode mode)
{
    if (source.index == gdata_mount)
    {
        ride_end();
        source.position = cdata.player().position;
    }

    const auto slot =
        destination_slot ? *destination_slot : chara_get_free_slot_force();
    auto& destination = cdata[slot];

    // Backups for changing
    position_t position;
    position_t initial_position;
    int relationship;
    int original_relationship;
    int hate;
    int enemy_id;
    int hp;
    if (mode == chara_relocate_mode::change)
    {
        position = destination.position;
        initial_position = destination.initial_position;
        relationship = destination.relationship;
        original_relationship = destination.original_relationship;
        hate = destination.hate;
        enemy_id = destination.enemy_id;
        hp = destination.hp;
    }

    // Copy `source`'s inventory to `destination`.
    const auto tmp = inv_getheader(source.index);
    const auto invhead = tmp.first;
    const auto invrange = tmp.second;
    int p = invhead;
    for (const auto& cnt : items(slot))
    {
        if (cnt == invrange)
        {
            break;
        }
        if (cc == source.index)
        {
            if (ci == p)
            {
                ci = cnt;
            }
        }
        item::copy(inv[p], inv[cnt]);
        inv[p].clear();
        inv[cnt].body_part = 0;
        ++p;
    }

    // Clear some fields which should not be copied.
    source.item_which_will_be_used = 0;
    source.is_livestock() = false;

    lua::lua->get_handle_manager().relocate_handle<character>(
        cdata[prm_784], tc_at_m125);

    // Copy from `source` to `destination` and clear `source`
    sdata.copy(slot, source.index);
    sdata.clear(source.index);

    character::copy(source, destination);
    source.clear();

    for (int cnt = 0; cnt < 10; ++cnt)
    {
        cdatan(cnt, slot) = cdatan(cnt, source.index);
        cdatan(cnt, source.index) = "";
    }

    // Unequip all gears.
    for (size_t i = 0; i < destination.body_parts.size(); ++i)
    {
        destination.body_parts[i] = destination.body_parts[i] / 10000 * 10000;
    }

    if (mode == chara_relocate_mode::change)
    {
        destination.set_state(character::state_t::alive);
        destination.position = position;
        destination.initial_position = initial_position;
        destination.relationship = relationship;
        destination.original_relationship = original_relationship;
        destination.hate = hate;
        destination.enemy_id = enemy_id;
        destination.hp = hp;
        map(destination.position.x, destination.position.y, 1) = slot + 1;
    }
    else
    {
        if (source.index != 56)
        {
            map(destination.position.x, destination.position.y, 1) = slot + 1;
        }
        else
        {
            rc = slot;
            destination.set_state(character::state_t::alive);
            cxinit = cdata.player().position.x;
            cyinit = cdata.player().position.y;
            chara_place();
        }
        destination.enemy_id = 0;
        destination.hate = 0;
    }

    // Lose resistance.
    if (slot < 16)
    {
        for (int element = 50; element < 61; ++element)
        {
            auto resistance = 100;
            if (sdata.get(element, slot).original_level >= 500
                || sdata.get(element, slot).original_level <= 100)
            {
                resistance = sdata.get(element, slot).original_level;
            }
            if (resistance > 500)
            {
                resistance = 500;
            }
            sdata.get(element, slot).original_level = resistance;
            sdata.get(element, slot).experience = 0;
            sdata.get(element, slot).potential = 0;
        }
    }

    rc = slot;
    wear_most_valuable_equipment_for_all_body_parts();
    chara_refresh(slot);

    // TODO handle transferring through Lua robustly
    // lua::lua->create_chara_handle_run_callbacks(destination);

    if (slot < 57)
    {
        modify_crowd_density(source.index, -1);
    }
    if (source.index < 57)
    {
        modify_crowd_density(slot, 1);
    }
}



void chara_set_item_which_will_be_used(character& cc)
{
    int category = the_item_db[inv[ci].id]->category;
    if (category == 57000 || category == 52000 || category == 53000)
    {
        cc.item_which_will_be_used = ci;
    }
}



int chara_armor_class(const character& cc)
{
    if (cc.sum_of_equipment_weight >= 35000)
    {
        return 169;
    }
    else if (cc.sum_of_equipment_weight >= 15000)
    {
        return 170;
    }
    else
    {
        return 171;
    }
}



bool belong_to_same_team(const character& c1, const character& c2)
{
    return (c1.relationship >= 0 && c2.relationship >= 0)
        || (c1.relationship == -1 && c2.relationship == -1)
        || (c1.relationship <= -2 && c2.relationship <= -2);
}



void chara_add_quality_parens()
{
    if (fixlv == 4)
    {
        cdatan(0, rc) = i18n::_(u8"ui", u8"bracket_left") + cdatan(0, rc)
            + i18n::_(u8"ui", u8"bracket_right");
        cdata[rc].level = cdata[rc].level * 10 / 8;
    }
    else if (fixlv == 5)
    {
        cdatan(0, rc) =
            i18n::s.get("core.locale.chara.name_with_title", cdata[rc]);
        cdata[rc].level = cdata[rc].level * 10 / 6;
    }
}

} // namespace elona
