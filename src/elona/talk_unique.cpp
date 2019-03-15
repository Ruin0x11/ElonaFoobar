#include "../util/strutil.hpp"
#include "area.hpp"
#include "audio.hpp"
#include "calc.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "crafting.hpp"
#include "data/types/type_item.hpp"
#include "dialog.hpp"
#include "elona.hpp"
#include "event.hpp"
#include "i18n.hpp"
#include "item.hpp"
#include "itemgen.hpp"
#include "macro.hpp"
#include "map.hpp"
#include "map_cell.hpp"
#include "menu.hpp"
#include "message.hpp"
#include "quest.hpp"
#include "random.hpp"
#include "shop.hpp"
#include "status_ailment.hpp"
#include "talk.hpp"
#include "ui.hpp"
#include "variables.hpp"



namespace elona
{

namespace
{

int chatval_ = 0;

TalkResult talk_unique_zeome()
{
    listmax = 0;
    buff = i18n::s.get("core.locale.talk.unique.zeome");
    tc = tc * 1 + 0;
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
    chatesc = 1;
    ELONA_TALK_SCENE_CUT();

    game_data.quest_flags.main_quest = 170;

    return TalkResult::talk_end;
}


TalkResult talk_unique_orphe()
{
    if (event_id() == 1)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.orphe.dialog",
            0,
            i18n::s.get_enum("core.locale.ui.sex", cdata.player().sex));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.orphe.dialog", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.orphe.dialog", 2);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.orphe.dialog", 3);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.orphe.dialog", 4);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.orphe.dialog", 5);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _loyter_goto_map()
{
    map_data.stair_down_pos =
        cdata.player().position.y * 1000 + cdata.player().position.x;
    map_prepare_for_travel(static_cast<int>(mdata_t::MapId::vernis), 5);
    chatteleport = 1;
}

void _loyter_receive_reward()
{
    flt();
    nostack = 1;
    itemcreate(
        -1, 630, cdata.player().position.x, cdata.player().position.y, 0);
    fixmaterial = 15;
    change_item_material();
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 100000);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 5);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
}

TalkResult talk_unique_loyter()
{
    if (cdata.player().fame < 20000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.loyter.fame_too_low");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.nightmare == 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.loyter.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.nightmare == 0)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.loyter.quest.dialog", 0, cdatan(1, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        ELONA_APPEND_RESPONSE(
            1, i18n::s.get("core.locale.talk.unique.loyter.quest.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0, i18n::s.get("core.locale.talk.unique.loyter.quest.choices.no"));
        buff =
            i18n::s.get_enum("core.locale.talk.unique.loyter.quest.dialog", 1);
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.loyter.quest.no");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.loyter.quest.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.nightmare = 1;

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.nightmare == 1 ||
        game_data.quest_flags.nightmare == 2)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.loyter.quest.begin.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0,
            i18n::s.get(
                "core.locale.talk.unique.loyter.quest.begin.choices.no"));
        buff = i18n::s.get("core.locale.talk.unique.loyter.quest.begin.dialog");
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.loyter.quest.begin.no");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.loyter.quest.begin.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.nightmare = 2;
        _loyter_goto_map();

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.nightmare == 3)
    {
        _loyter_receive_reward();

        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.loyter.quest.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.nightmare = 1000;

        return TalkResult::talk_end;
    }
    return TalkResult::talk_end;
}


void _miches_receive_reward()
{
    flt(calcobjlv(10), calcfixlv(Quality::good));
    itemcreate(
        -1, 449, cdata.player().position.x, cdata.player().position.y, 0);
    flt(calcobjlv(10), calcfixlv(Quality::good));
    itemcreate(-1, 66, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 3000);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 2);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
}

TalkResult talk_unique_miches()
{
    if (game_data.quest_flags.putit_attacks == 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.miches.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.putit_attacks == 0)
    {
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.miches.quest.dialog", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        ELONA_APPEND_RESPONSE(
            1, i18n::s.get("core.locale.talk.unique.miches.quest.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0, i18n::s.get("core.locale.talk.unique.miches.quest.choices.no"));
        buff =
            i18n::s.get_enum("core.locale.talk.unique.miches.quest.dialog", 1);
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.miches.quest.no");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.miches.quest.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.putit_attacks = 1;

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.putit_attacks == 1)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.miches.quest.waiting");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.putit_attacks == 2)
    {
        _miches_receive_reward();

        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.miches.quest.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.putit_attacks = 1000;

        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _shena_receive_reward()
{
    flt();
    itemcreate(-1, 18, cdata.player().position.x, cdata.player().position.y, 0);
    inv[ci].count = 12;
    flt();
    nostack = 1;
    itemcreate(
        -1, 685, cdata.player().position.x, cdata.player().position.y, 0);
    inv[ci].param2 = 5;
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 1500);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 2);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
}

TalkResult talk_unique_shena()
{
    if (game_data.quest_flags.thieves_hideout == 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.shena.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.thieves_hideout == 0)
    {
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.shena.quest.dialog", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        ELONA_APPEND_RESPONSE(
            1, i18n::s.get("core.locale.talk.unique.shena.quest.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0, i18n::s.get("core.locale.talk.unique.shena.quest.choices.no"));
        buff =
            i18n::s.get_enum("core.locale.talk.unique.shena.quest.dialog", 1);
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.shena.quest.no");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.shena.quest.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.thieves_hideout = 1;
        cell_featset(48, 5, tile_downstairs, 11, 4);

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.thieves_hideout == 1)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.shena.quest.waiting");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.thieves_hideout == 2)
    {
        _shena_receive_reward();

        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.shena.quest.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.thieves_hideout = 1000;

        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


TalkResult talk_unique_larnneire()
{
    ELONA_APPEND_RESPONSE(
        3, i18n::s.get("core.locale.talk.unique.larnneire.choices.mission"));
    ELONA_APPEND_RESPONSE(
        2,
        i18n::s.get("core.locale.talk.unique.larnneire.choices.north_tyris"));
    ELONA_APPEND_RESPONSE(
        1, i18n::s.get("core.locale.talk.unique.larnneire.choices.fairy_tale"));
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
    buff = i18n::s.get("core.locale.talk.unique.larnneire.dialog");
    chatval_ = talk_window_query();
    if (chatval_ == 1)
    {
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.larnneire.fairy_tale", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.larnneire.fairy_tale", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.larnneire.fairy_tale", 2);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.larnneire.fairy_tale", 3);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.larnneire.fairy_tale", 4);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.larnneire.fairy_tale", 5);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_unique;
    }
    if (chatval_ == 2)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.larnneire.north_tyris", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.larnneire.north_tyris", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_unique;
    }
    if (chatval_ == 3)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.larnneire.mission");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_unique;
    }
    return TalkResult::talk_end;
}


void _lomias_create_scroll_of_identify()
{
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    flt();
    itemcreate(-1, 14, cdata.player().position.x, cdata.player().position.y, 0);
    inv[ci].identification_state = IdentifyState::completely_identified;
}

void _lomias_create_chest()
{

    flt();
    itemcreate(
        -1, 240, cdata.player().position.x, cdata.player().position.y, 0);
    inv[ci].param1 = 35;
    inv[ci].param2 = 25;
    flt();
    itemcreate(
        -1, 636, cdata.player().position.x, cdata.player().position.y, 2);
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
}

void _lomias_create_corpse()
{
    flt();
    itemcreate(
        -1, 204, cdata.player().position.x, cdata.player().position.y, 0);
    inv[ci].subname = 9;
    inv[ci].identification_state = IdentifyState::completely_identified;
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
}

void _lomias_tutorial_finish()
{
    chara_vanquish(chara_find(33));
    chara_vanquish(chara_find(34));
    txt(i18n::s.get("core.locale.talk.unique.lomias.after.get_out.leave"));
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    for (int cnt = 0; cnt < 3; ++cnt)
    {
        flt();
        flttypemajor = 60000;
        itemcreate(
            -1, 0, cdata.player().position.x, cdata.player().position.y, 0);
    }
}

void _lomias_create_equipment()
{
    flt();
    itemcreate(-1, 58, cdata.player().position.x, cdata.player().position.y, 0);
    inv[ci].curse_state = CurseState::cursed;
    flt();
    itemcreate(-1, 61, cdata.player().position.x, cdata.player().position.y, 0);
    inv[ci].curse_state = CurseState::none;
    flt();
    itemcreate(
        -1, 363, cdata.player().position.x, cdata.player().position.y, 0);
    inv[ci].identification_state = IdentifyState::completely_identified;
    inv[ci].curse_state = CurseState::blessed;
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
}

void _lomias_release_putits()
{
    txt(i18n::s.get("core.locale.talk.unique.lomias.tutorial.equip.done."
                    "lomias_releases"),
        Message::color{ColorIndex::cyan});
    for (int cnt = 0; cnt < 3; ++cnt)
    {
        flt();
        chara_create(
            -1, 3, cdata.player().position.x, cdata.player().position.y);
        cdata[rc].does_not_search_enemy() = true;
    }
    flt();
    itemcreate(-1, 68, cdata.player().position.x, cdata.player().position.y, 0);
    inv[ci].identification_state = IdentifyState::completely_identified;
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
}

TalkResult talk_unique_lomias()
{
    if (chatflag == 0)
    {
        if (game_data.quest_flags.tutorial == 0)
        {
            buff = i18n::s.get(
                "core.locale.talk.unique.lomias.tutorial.before.dialog");
            ELONA_APPEND_RESPONSE(
                1,
                i18n::s.get("core.locale.talk.unique.lomias.tutorial.before."
                            "choices.yes"));
            ELONA_APPEND_RESPONSE(
                0,
                i18n::s.get("core.locale.talk.unique.lomias.tutorial.before."
                            "choices.no"));
        }
        if (game_data.quest_flags.tutorial == 1)
        {
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.movement.dialog", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.movement.dialog", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.movement.dialog", 2);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.movement.dialog", 3);
            ELONA_APPEND_RESPONSE(
                0,
                i18n::s.get("core.locale.talk.unique.lomias.tutorial.movement."
                            "choices.alright"));
            ELONA_APPEND_RESPONSE(
                3,
                i18n::s.get("core.locale.talk.unique.lomias.tutorial.movement."
                            "choices.ate"));
        }
        if (game_data.quest_flags.tutorial == 2)
        {
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.skills.dialog", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.skills.dialog", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.skills.dialog", 2);
            ELONA_APPEND_RESPONSE(
                0,
                i18n::s.get(
                    "core.locale.talk.unique.lomias.tutorial.skills.response"));
        }
        if (game_data.quest_flags.tutorial == 3)
        {
            _lomias_create_scroll_of_identify();

            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.lomias.tutorial.after_dig.dialog");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            game_data.quest_flags.tutorial = 4;
        }
        if (game_data.quest_flags.tutorial == 4)
        {
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.identify.dialog", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.identify.dialog", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.identify.dialog", 2);
            ELONA_APPEND_RESPONSE(
                0,
                i18n::s.get("core.locale.talk.unique.lomias.tutorial.identify."
                            "choices.alright"));
            ELONA_APPEND_RESPONSE(
                4,
                i18n::s.get("core.locale.talk.unique.lomias.tutorial.identify."
                            "choices.done"));
        }
        if (game_data.quest_flags.tutorial == 5)
        {
            buff = i18n::s.get(
                "core.locale.talk.unique.lomias.tutorial.equip.dialog");
            ELONA_APPEND_RESPONSE(
                0,
                i18n::s.get("core.locale.talk.unique.lomias.tutorial.equip."
                            "choices.alright"));
            ELONA_APPEND_RESPONSE(
                5,
                i18n::s.get("core.locale.talk.unique.lomias.tutorial.equip."
                            "choices.done"));
        }
        if (game_data.quest_flags.tutorial == 6)
        {
            p = chara_find(3);
            if (p != 0)
            {
                listmax = 0;
                buff = i18n::s.get(
                    "core.locale.talk.unique.lomias.tutorial.combat.not_"
                    "finished");
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                return TalkResult::talk_end;
            }
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.lomias.tutorial.combat.finished");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            game_data.quest_flags.tutorial = 7;
        }
        if (game_data.quest_flags.tutorial == 7)
        {
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.chests.dialog", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.chests.dialog", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            _lomias_create_chest();

            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.chests.dialog", 2);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(
                0,
                i18n::s.get(
                    "core.locale.talk.unique.lomias.tutorial.chests.response"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            game_data.quest_flags.tutorial = 8;

            return TalkResult::talk_end;
        }
        if (game_data.quest_flags.tutorial == 8)
        {
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.house.dialog", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.house.dialog", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.house.dialog", 2);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.house.dialog", 3);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            game_data.quest_flags.tutorial = 99;
        }
        if (game_data.quest_flags.tutorial == 99)
        {
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.end.dialog", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lomias.tutorial.end.dialog", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            game_data.quest_flags.tutorial = -1;

            return TalkResult::talk_end;
        }
        if (game_data.quest_flags.tutorial == -1)
        {
            buff = i18n::s.get("core.locale.talk.unique.lomias.after.dialog");
            ELONA_APPEND_RESPONSE(
                0,
                i18n::s.get(
                    "core.locale.talk.unique.lomias.after.choices.nothing"));
        }
        if (game_data.quest_flags.tutorial == 0 ||
            game_data.quest_flags.tutorial == -1)
        {
            ELONA_APPEND_RESPONSE(
                2,
                i18n::s.get(
                    "core.locale.talk.unique.lomias.after.choices.get_out"));
        }
        chatval_ = talk_window_query();
    }
    if (chatval_ == 1)
    {
        listmax = 0;
        buff =
            i18n::s.get("core.locale.talk.unique.lomias.tutorial.before.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _lomias_create_corpse();
        game_data.quest_flags.tutorial = 1;

        return TalkResult::talk_unique;
    }
    if (chatval_ == 2)
    {
        tc = chara_find(33);
        if (tc == 0 || cdata[tc].state() != Character::State::alive)
        {
            tc = chara_find(34);
            hostileaction(0, tc);

            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.lomias.after.get_out.larnneire_died");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.after.get_out.dialog",
            0,
            cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.after.get_out.dialog", 1);
        tc =
            tc * (chara_find(34) == 0) + (chara_find(34) != 0) * chara_find(34);
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.after.get_out.dialog",
            2,
            cdatan(1, 0));
        tc =
            tc * (chara_find(33) == 0) + (chara_find(33) != 0) * chara_find(33);
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _lomias_tutorial_finish();
    }
    if (chatval_ == 3)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.tutorial.movement.ate.dialog", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(
            0,
            i18n::s.get("core.locale.talk.unique.lomias.tutorial.movement.ate."
                        "response"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.tutorial.movement.ate.dialog", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.tutorial.movement.ate.dialog", 2);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.tutorial = 2;

        return TalkResult::talk_unique;
    }
    if (chatval_ == 4)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.tutorial.identify.done.dialog", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.tutorial.identify.done.dialog", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _lomias_create_equipment();
        game_data.quest_flags.tutorial = 5;

        return TalkResult::talk_unique;
    }
    if (chatval_ == 5)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.tutorial.equip.done.dialog", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.tutorial.equip.done.dialog", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.lomias.tutorial.equip.done.dialog", 2);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _lomias_release_putits();
        game_data.quest_flags.tutorial = 6;
    }
    return TalkResult::talk_end;
}


TalkResult talk_unique_stersha()
{
    if (game_data.quest_flags.main_quest == 200)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.stersha.late");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.main_quest < 90)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.stersha.early");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
    }
    else
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.stersha.mid");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
    }
    return TalkResult::talk_end;
}


void _xabi_receive_reward()
{
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 2500);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 3);
    flt();
    itemcreate(
        -1, 559, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1, 621, cdata.player().position.x, cdata.player().position.y, 0);
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
}

TalkResult talk_unique_xabi()
{
    if (game_data.quest_flags.main_quest >= 60)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.xabi.late");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.main_quest == 50)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.xabi.mid");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.main_quest == 40)
    {
        game_data.quest_flags.main_quest = 50;

        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.xabi.early", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        update_screen();
        fade_out();
        gsel(0);

        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.xabi.early", 1, cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _xabi_receive_reward();

        return TalkResult::talk_unique;
    }
    return TalkResult::talk_ignored;
}


void _slan_receive_reward()
{
    for (int cnt = 0; cnt < 4; ++cnt)
    {
        flt(calcobjlv(game_data.current_dungeon_level),
            calcfixlv(Quality::bad));
        flttypemajor = fltsetdungeon();
        itemcreate(-1, 0, cdata[tc].position.x, cdata[tc].position.y, 0);
    }
    flt();
    itemcreate(
        -1, 54, cdata[tc].position.x, cdata[tc].position.y, 1000 + rnd(200));
    flt();
    itemcreate(-1, 55, cdata[tc].position.x, cdata[tc].position.y, 3);
    flt();
    itemcreate(-1, 239, cdata[tc].position.x, cdata[tc].position.y, 0);
    inv[ci].param2 = 0;
    snd("core.write1");
    txt(i18n::s.get("core.locale.talk.unique.slan.you_receive"),
        Message::color{ColorIndex::green});
    txt(i18n::s.get("core.locale.talk.unique.slan.dies", cdata[tc]));
    chara_vanquish(tc);
}

TalkResult talk_unique_slan()
{
    if (game_data.quest_flags.main_quest == 20)
    {
        game_data.quest_flags.main_quest = 30;

        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.slan.dialog", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.slan.dialog", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.slan.dialog", 2);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _slan_receive_reward();

        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _erystia_receive_reward()
{
    snd("core.write1");
    txt(i18n::s.get("core.locale.talk.unique.erystia.all_stones.you_receive"),
        Message::color{ColorIndex::green});
    flt();
    itemcreate(
        -1, 360, cdata.player().position.x, cdata.player().position.y, 0);
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    game_data.quest_flags.main_quest = 125;
}

void _erystia_receive_key_17th_level()
{
    snd("core.write1");
    txt(i18n::s.get("core.locale.talk.unique.erystia.stones.you_receive"),
        Message::color{ColorIndex::green});
    game_data.quest_flags.main_quest = 110;
}

void _erystia_receive_key_4th_level()
{
    snd("core.write1");
    txt(i18n::s.get("core.locale.talk.unique.erystia.introduction.pledge_"
                    "strength.you_receive"),
        Message::color{ColorIndex::green});
    game_data.quest_flags.main_quest = 60;
}

TalkResult talk_unique_erystia()
{
    if (game_data.quest_flags.main_quest == 200)
    {
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.erystia.late", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.erystia.late", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.late",
            2,
            cdatan(1, 0),
            cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.main_quest == 120)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.all_stones.dialog",
            0,
            cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.all_stones.dialog", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.all_stones.dialog", 2);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.all_stones.dialog",
            3,
            cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _erystia_receive_reward();

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.main_quest == 105)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.stones.dialog", 0, cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        update_screen();
        fade_out();
        gsel(0);

        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.stones.dialog", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.stones.dialog", 2);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.stones.dialog", 3);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.stones.dialog", 4);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _erystia_receive_key_17th_level();

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.main_quest >= 60)
    {
        buff = i18n::s.get(
            "core.locale.talk.unique.erystia.investigation.dialog",
            cdatan(0, 0));
        ELONA_APPEND_RESPONSE(
            6,
            i18n::s.get("core.locale.talk.unique.erystia.investigation.choices."
                        "lesimas"));
        ELONA_APPEND_RESPONSE(
            5,
            i18n::s.get("core.locale.talk.unique.erystia.investigation.choices."
                        "mission"));
        if (game_data.quest_flags.main_quest >= 100 &&
            game_data.quest_flags.main_quest <= 120)
        {
            ELONA_APPEND_RESPONSE(
                3,
                i18n::s.get("core.locale.talk.unique.erystia.investigation."
                            "choices.stones.castle"));
            ELONA_APPEND_RESPONSE(
                2,
                i18n::s.get("core.locale.talk.unique.erystia.investigation."
                            "choices.stones.inferno"));
            ELONA_APPEND_RESPONSE(
                1,
                i18n::s.get("core.locale.talk.unique.erystia.investigation."
                            "choices.stones.crypt"));
        }
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        chatval_ = talk_window_query();
        if (chatval_ == 6)
        {
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.lesmias", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.lesmias", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.lesmias", 2);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.lesmias", 3);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.lesmias", 4);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_unique;
        }
        if (chatval_ == 5)
        {
            if (game_data.quest_flags.main_quest >= 125)
            {
                listmax = 0;
                buff = i18n::s.get_enum(
                    "core.locale.talk.unique.erystia.investigation.mission."
                    "excavation",
                    0);
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                listmax = 0;
                buff = i18n::s.get_enum(
                    "core.locale.talk.unique.erystia.investigation.mission."
                    "excavation",
                    1);
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                return TalkResult::talk_unique;
            }
            if (game_data.quest_flags.main_quest >= 110)
            {
                listmax = 0;
                buff = i18n::s.get_enum(
                    "core.locale.talk.unique.erystia.investigation.mission."
                    "stones",
                    0);
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                return TalkResult::talk_unique;
            }
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.mission.stones",
                1,
                cdatan(0, 0));
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_unique;
        }
        if (chatval_ == 3)
        {
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.castle", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.castle", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.castle", 2);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_unique;
        }
        if (chatval_ == 2)
        {
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.inferno", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.inferno", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.inferno", 2);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_unique;
        }
        if (chatval_ == 1)
        {
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.crypt", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.crypt", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.erystia.investigation.crypt", 2);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_unique;
        }
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.main_quest == 50)
    {
        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.erystia.introduction.dialog",
            cdatan(0, 0));
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get("core.locale.talk.unique.erystia.introduction.choices."
                        "pledge_strength"));
        ELONA_APPEND_RESPONSE(
            0,
            i18n::s.get("core.locale.talk.unique.erystia.introduction.choices."
                        "not_interested"));
        chatesc = 1;
        chatval_ = talk_window_query();
        if (chatval_ == 0)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.erystia.introduction.not_interested");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.introduction.pledge_strength."
            "dialog",
            0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.introduction.pledge_strength."
            "dialog",
            1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.introduction.pledge_strength."
            "dialog",
            2);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.erystia.introduction.pledge_strength."
            "dialog",
            3,
            cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _erystia_receive_key_4th_level();

        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _karam_receive_reward()
{
    for (int cnt = 0; cnt < 4; ++cnt)
    {
        flt(calcobjlv(game_data.current_dungeon_level),
            calcfixlv(Quality::bad));
        flttypemajor = fltsetdungeon();
        itemcreate(-1, 0, cdata[tc].position.x, cdata[tc].position.y, 0);
    }
    flt();
    itemcreate(
        -1, 54, cdata[tc].position.x, cdata[tc].position.y, 1000 + rnd(200));
    flt();
    itemcreate(-1, 55, cdata[tc].position.x, cdata[tc].position.y, 3);
    flt();
    itemcreate(-1, 239, cdata[tc].position.x, cdata[tc].position.y, 0);
    inv[ci].param2 = 0;
    quest_update_journal_msg();
    txt(i18n::s.get("core.locale.talk.unique.karam.dies", cdata[tc]));
    chara_vanquish(tc);
}

TalkResult talk_unique_karam()
{
    if (game_data.quest_flags.main_quest == 90)
    {
        game_data.quest_flags.main_quest = 100;

        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.karam.dialog", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.karam.dialog", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.karam.dialog", 2);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.karam.dialog", 3);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.karam.dialog", 4);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.karam.dialog", 5);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _karam_receive_reward();

        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


TalkResult talk_unique_garokk()
{
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
    buff = i18n::s.get("core.locale.talk.unique.garokk");
    chatval_ = talk_window_query();
    if (chatval_ == 1)
    {
        invctrl = 1;
        snd("core.pop2");
        crafting_menu();
        return TalkResult::talk_unique;
    }
    return TalkResult::talk_end;
}


void _miral_trade_small_medals()
{
    invctrl = 28;
    invfile = cdata[tc].shop_store_id;
    shop_sell_item();
    screenupdate = -1;
    update_screen();
    cs = 0;
}

void _miral_upgrade_cargo_limit()
{
    Message::instance().linebreak();
    txt(i18n::s.get(
        "core.locale.talk.unique.miral.upgrade_cart.give.limit_increased",
        cnvweight(calccargoupdate())));
    snd("core.build1");
    inv[ci].modify_number((-calccargoupdatecost()));
    game_data.current_cart_limit += calccargoupdate();
    refresh_burden_state();
}

TalkResult talk_unique_miral()
{
    ELONA_APPEND_RESPONSE(
        2, i18n::s.get("core.locale.talk.unique.miral.choices.small_medals"));
    ELONA_APPEND_RESPONSE(
        3, i18n::s.get("core.locale.talk.unique.miral.choices.upgrade_cart"));
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
    buff = i18n::s.get("core.locale.talk.unique.miral.dialog");
    chatval_ = talk_window_query();
    if (chatval_ == 2)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.miral.small_medals");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _miral_trade_small_medals();

        return TalkResult::talk_unique;
    }
    if (chatval_ == 3)
    {
        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.miral.upgrade_cart.dialog",
            calccargoupdatecost());
        int stat = item_find(622, 3, 1);
        if (stat != -1)
        {
            ci = stat;
            if (inv[ci].number() >= calccargoupdatecost())
            {
                ELONA_APPEND_RESPONSE(
                    1,
                    i18n::s.get("core.locale.talk.unique.miral.upgrade_cart."
                                "choices.give"));
            }
        }
        ELONA_APPEND_RESPONSE(
            0,
            i18n::s.get(
                "core.locale.talk.unique.miral.upgrade_cart.choices.go_back"));
        chatesc = 1;
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.miral.upgrade_cart.go_back");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_unique;
        }

        _miral_upgrade_cargo_limit();

        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.miral.upgrade_cart.give.dialog");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_unique;
    }
    return TalkResult::talk_end;
}


TalkResult talk_unique_pael()
{
    if (game_data.quest_flags.pael_and_her_mom == 1000)
    {
        if (game_data.current_map == mdata_t::MapId::noyel &&
            area_data[game_data.current_map].christmas_festival)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.pael.festival");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.pael.after_face");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom == 1001)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.pael.after_death");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom == 1002)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.pael.after_sold");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom == 0)
    {
        buff = i18n::s.get("core.locale.talk.unique.pael.before");
        bool stat = talk_give_potion_of_cure_corruption();
        if (!stat)
        {
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        game_data.quest_flags.pael_and_her_mom = 1;
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom == 1 ||
        game_data.quest_flags.pael_and_her_mom == 3)
    {
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.pael.progress", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom == 5 ||
        game_data.quest_flags.pael_and_her_mom == 7)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.pael.progress", 1, cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom == 2 ||
        game_data.quest_flags.pael_and_her_mom == 4)
    {
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.pael.progress", 2, cdatan(0, 0));
        bool stat = talk_give_potion_of_cure_corruption();
        if (!stat)
        {
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        ++game_data.quest_flags.pael_and_her_mom;
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom == 6)
    {
        buff = i18n::s.get_enum("core.locale.talk.unique.pael.progress", 3);

        bool stat = talk_give_potion_of_cure_corruption();
        if (!stat)
        {
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        ++game_data.quest_flags.pael_and_her_mom;
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom == 8)
    {
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.pael.progress", 4, cdatan(0, 0));
        bool stat = talk_give_potion_of_cure_corruption();
        if (!stat)
        {
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        ++game_data.quest_flags.pael_and_her_mom;
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom == 9)
    {
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.pael.progress", 5);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom == 10)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.pael.progress", 6, cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _paels_mom_receive_reward()
{
    flt();
    itemcreate(
        -1, 639, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 20000);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 4);
    snd("core.complete1");
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
}

void _paels_mom_give_potion()
{
    int stat = inv_find(559, 0);
    inv[stat].modify_number(-1);
    txt(
        i18n::s.get("core.locale.talk.unique.paels_mom.progress.end_"
                    "life.give.you_hand_her"));
    snd("core.equip1");
    modify_karma(cdata.player(), 20);
}

TalkResult talk_unique_paels_mom()
{
    if (game_data.quest_flags.pael_and_her_mom == 1002)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.paels_mom.after_sold");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom == 10)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.paels_mom.progress.last", 0, cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.paels_mom.progress.last", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _paels_mom_receive_reward();

        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.paels_mom.progress.last", 2);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.pael_and_her_mom = 1000;
        cdata[tc].is_silent() = true;
    }
    if (game_data.quest_flags.pael_and_her_mom == 1000)
    {
        if (game_data.current_map == mdata_t::MapId::noyel &&
            area_data[game_data.current_map].christmas_festival)
        {
            buff = i18n::s.get(
                "core.locale.talk.unique.paels_mom.progress.festival.dialog");
            int stat = inv_find(559, 0);
            if (stat != -1)
            {
                ELONA_APPEND_RESPONSE(
                    2,
                    i18n::s.get("core.locale.talk.unique.paels_mom.progress."
                                "festival.choices.give"));
            }
            ELONA_APPEND_RESPONSE(
                0,
                i18n::s.get("core.locale.talk.unique.paels_mom.progress."
                            "festival.choices.take_care"));
            chatesc = 1;
            chatval_ = talk_window_query();
            if (chatval_ == 2)
            {
                _paels_mom_give_potion();

                listmax = 0;
                buff = i18n::s.get(
                    "core.locale.talk.unique.paels_mom.progress.festival.give."
                    "dialog");
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                return TalkResult::talk_end;
            }
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.paels_mom.progress.festival.take_"
                "care");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
    }
    if (game_data.quest_flags.pael_and_her_mom == 1000)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.paels_mom.progress.end_life.dialog",
            0,
            cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.paels_mom.progress.end_life.dialog", 1);
        int stat = inv_find(559, 0);
        if (stat != -1)
        {
            ELONA_APPEND_RESPONSE(
                2,
                i18n::s.get("core.locale.talk.unique.paels_mom.progress.end_"
                            "life.choices.give"));
        }
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get("core.locale.talk.unique.paels_mom.progress.end_life."
                        "choices.end"));
        ELONA_APPEND_RESPONSE(
            0,
            i18n::s.get("core.locale.talk.unique.paels_mom.progress.end_life."
                        "choices.leave"));
        chatesc = 1;
        chatval_ = talk_window_query();
        if (chatval_ == 1)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.paels_mom.progress.end_life.end");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            event_add(20, tc);

            return TalkResult::talk_end;
        }
        if (chatval_ == 2)
        {
            _paels_mom_give_potion();

            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.paels_mom.progress.end_life.give."
                "dialog");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.paels_mom.progress.end_life.leave");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom > 7)
    {
        listmax = 0;
        buff =
            i18n::s.get("core.locale.talk.unique.paels_mom.progress.very_late");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom > 5)
    {
        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.paels_mom.progress.late", cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pael_and_her_mom > 3)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.paels_mom.progress.mid");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    listmax = 0;
    buff = i18n::s.get("core.locale.talk.unique.paels_mom.progress.early");
    tc = tc * 1 + 0;
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
    chatesc = 1;
    ELONA_TALK_SCENE_CUT();
    return TalkResult::talk_end;
}


void _raphael_give_wife()
{
    s = chara_refstr(cdata[tc].id, 8);
    if (!strutil::contains(s(0), u8"/man/"))
    {
        dmgcon(tcbk, StatusAilment::insane, 1000);
        f = 1;
        modify_karma(cdata.player(), 2);
    }
    else
    {
        f = 0;
        modify_karma(cdata.player(), -15);
    }
}

void _raphael_receive_reward()
{
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 5000);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    quest_update_journal_msg();
    game_data.quest_flags.wife_collector = 1000;
}

void _raphael_receive_wife_reward()
{
    chara_vanquish(rc);
    snd("core.complete1");
    flt();
    itemcreate(
        -1, 640, cdata.player().position.x, cdata.player().position.y, 2);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 2);
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
}

TalkResult talk_unique_raphael()
{
    if (game_data.quest_flags.wife_collector == 0)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get("core.locale.talk.unique.raphael.quest.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0, i18n::s.get("core.locale.talk.unique.raphael.quest.choices.no"));
        buff = i18n::s.get("core.locale.talk.unique.raphael.quest.dialog");
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.raphael.quest.no");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.raphael.quest.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.wife_collector = 1;

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.wife_collector == 1 ||
        game_data.quest_flags.wife_collector == 1000)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.raphael.bring_wife.choices.this_one"));
        ELONA_APPEND_RESPONSE(
            0,
            i18n::s.get(
                "core.locale.talk.unique.raphael.bring_wife.choices.go_back"));
        buff = i18n::s.get("core.locale.talk.unique.raphael.bring_wife.dialog");
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.raphael.bring_wife.go_back");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        int stat = ctrl_ally(ControlAllyOperation::sell);
        if (stat == -1)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.raphael.bring_wife.this_one.no_such_"
                "wife");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        rc = stat;
        if (cdata[rc].is_married() == 0)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.raphael.bring_wife.this_one.not_"
                "married");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.raphael.bring_wife.this_one.come_along",
            cdatan(0, rc));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        tcbk = tc;
        tc = rc;
        _raphael_give_wife();

        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.raphael.bring_wife.this_one.leaving");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        tc = tcbk;

        if (f == 1)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.raphael.bring_wife.this_one.not_"
                "human");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
        }
        if (game_data.quest_flags.wife_collector == 1)
        {
            _raphael_receive_reward();
        }
        _raphael_receive_wife_reward();

        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.raphael.bring_wife.this_one.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _ainc_receive_reward()
{
    flt();
    itemcreate(
        -1, 661, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 5000);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 3);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
    game_data.quest_flags.novice_knight = 1000;
}

TalkResult talk_unique_ainc()
{
    if (game_data.quest_flags.novice_knight == 1000)
    {
        listmax = 0;
        buff =
            i18n::s.get("core.locale.talk.unique.ainc.complete", cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.novice_knight == 0)
    {
        ELONA_APPEND_RESPONSE(
            1, i18n::s.get("core.locale.talk.unique.ainc.quest.choices.do_it"));
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        buff = i18n::s.get("core.locale.talk.unique.ainc.quest.dialog");
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.ainc.quest.bye");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.ainc.quest.do_it");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.novice_knight = 1;

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.novice_knight == 1)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.ainc.quest.waiting");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.novice_knight == 2)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.ainc.quest.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _ainc_receive_reward();

        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _poppy_add_to_party()
{
    txt(i18n::s.get(
        "core.locale.talk.unique.poppy.find.you_must_return", cdata[tc]));
    rc = tc;
    new_ally_joins();
    cdata[rc].is_escorted_in_sub_quest() = true;
    chara_refresh(rc);
}

TalkResult talk_unique_poppy()
{
    if (game_data.quest_flags.puppys_cave == 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.poppy.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (chara_get_free_slot_ally() != 0)
    {
        ELONA_APPEND_RESPONSE(
            1, i18n::s.get("core.locale.talk.unique.poppy.find.choices.take"));
    }
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
    buff = i18n::s.get("core.locale.talk.unique.poppy.find.dialog");
    chatval_ = talk_window_query();
    if (chatval_ == 1)
    {
        _poppy_add_to_party();
        return TalkResult::talk_end;
    }
    else
    {
        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _rilian_receive_reward()
{
    flt();
    itemcreate(
        -1, 641, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 2500);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 2);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
    game_data.quest_flags.puppys_cave = 1000;
    chara_vanquish(chara_find_ally(225));
    flt();
    chara_create(-1, 225, 31, 4);
    cdata[rc].character_role = 3;
}

TalkResult talk_unique_rilian()
{
    if (game_data.quest_flags.puppys_cave == 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.rilian.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.puppys_cave == 0)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get("core.locale.talk.unique.rilian.quest.choices.do_it"));
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        buff = i18n::s.get("core.locale.talk.unique.rilian.quest.dialog");
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.rilian.quest.bye");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.rilian.quest.do_it");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.puppys_cave = 1;

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.puppys_cave == 1)
    {
        if (chara_find_ally(225) == -1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.rilian.quest.waiting");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.rilian.quest.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _rilian_receive_reward();

        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _tam_receive_reward()
{
    flt();
    nostack = 1;
    itemcreate(
        -1, 630, cdata.player().position.x, cdata.player().position.y, 0);
    fixmaterial = 24;
    change_item_material();
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 25500);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 4);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
}

TalkResult talk_unique_tam()
{
    if (game_data.quest_flags.cat_house == 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.tam.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.cat_house == 0)
    {
        ELONA_APPEND_RESPONSE(
            1, i18n::s.get("core.locale.talk.unique.tam.quest.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0, i18n::s.get("core.locale.talk.unique.tam.quest.choices.no"));
        buff = i18n::s.get("core.locale.talk.unique.tam.quest.dialog");
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.tam.quest.no");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.tam.quest.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.cat_house = 1;
        cell_featset(23, 22, tile_downstairs, 11, 3);

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.cat_house == 1)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.tam.quest.waiting");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.cat_house == 2)
    {
        _tam_receive_reward();

        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.tam.quest.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        game_data.quest_flags.cat_house = 1000;
        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _gilbert_goto_map()
{
    game_data.quest_flags.defense_line = 2;
    map_data.stair_down_pos =
        cdata.player().position.y * 1000 + cdata.player().position.x;
    map_prepare_for_travel(static_cast<int>(mdata_t::MapId::yowyn), 4);
    chatteleport = 1;
}

void _gilbert_receive_reward()
{
    flt();
    itemcreate(
        -1, 655, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 10000);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 3);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
}

TalkResult talk_unique_gilbert()
{
    if (game_data.quest_flags.defense_line == 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.gilbert.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (cdata.player().fame < 5000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.gilbert.fame_too_low");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.defense_line == 0)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.gilbert.quest.dialog", 0, cdatan(1, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get("core.locale.talk.unique.gilbert.quest.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0, i18n::s.get("core.locale.talk.unique.gilbert.quest.choices.no"));
        buff =
            i18n::s.get_enum("core.locale.talk.unique.gilbert.quest.dialog", 1);
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.gilbert.quest.no");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.gilbert.quest.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.defense_line = 1;

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.defense_line == 1 ||
        game_data.quest_flags.defense_line == 2)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.gilbert.quest.begin.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0,
            i18n::s.get(
                "core.locale.talk.unique.gilbert.quest.begin.choices.no"));
        buff =
            i18n::s.get("core.locale.talk.unique.gilbert.quest.begin.dialog");
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff =
                i18n::s.get("core.locale.talk.unique.gilbert.quest.begin.no");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.gilbert.quest.begin.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _gilbert_goto_map();

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.defense_line == 3)
    {
        _gilbert_receive_reward();

        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.gilbert.quest.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.defense_line = 1000;

        return TalkResult::talk_end;
    }
    return TalkResult::talk_end;
}


void _arnord_goto_map()
{
    game_data.quest_flags.kamikaze_attack = 2;
    map_data.stair_down_pos =
        cdata.player().position.y * 1000 + cdata.player().position.x;
    map_prepare_for_travel(static_cast<int>(mdata_t::MapId::port_kapul), 25);
    chatteleport = 1;
}

void _arnord_receive_reward()
{
    flt();
    itemcreate(
        -1, 662, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 7500);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 3);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
}

TalkResult talk_unique_arnord()
{
    if (game_data.quest_flags.kamikaze_attack == 1000)
    {
        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.arnord.complete", cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.kamikaze_attack == 0)
    {
        buff = i18n::s.get("core.locale.talk.unique.arnord.quest.dialog");
        ELONA_APPEND_RESPONSE(
            1, i18n::s.get("core.locale.talk.unique.arnord.quest.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0, i18n::s.get("core.locale.talk.unique.arnord.quest.choices.no"));
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.arnord.quest.no");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.arnord.quest.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.kamikaze_attack = 1;

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.kamikaze_attack == 1 ||
        game_data.quest_flags.kamikaze_attack == 2)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.arnord.quest.begin.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0,
            i18n::s.get(
                "core.locale.talk.unique.arnord.quest.begin.choices.no"));
        buff = i18n::s.get("core.locale.talk.unique.arnord.quest.begin.dialog");
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.arnord.quest.begin.no");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.arnord.quest.begin.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _arnord_goto_map();

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.kamikaze_attack == 3)
    {
        _arnord_receive_reward();

        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.arnord.quest.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.kamikaze_attack = 1000;

        return TalkResult::talk_end;
    }
    return TalkResult::talk_end;
}


void _mia_receive_reward()
{
    flt();
    itemcreate(
        -1, 663, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 5000);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 3);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
    game_data.quest_flags.mias_dream = 1000;
    chara_relocate(cdata[chara_find_ally(246)], none);
    cdata[rc].relationship = -1;
    cdata[rc].original_relationship = -1;
    cdata[rc].character_role = 3;
}

TalkResult talk_unique_mia()
{
    if (game_data.quest_flags.mias_dream == 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.mia.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.mias_dream == 0)
    {
        ELONA_APPEND_RESPONSE(
            1, i18n::s.get("core.locale.talk.unique.mia.quest.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0, i18n::s.get("core.locale.talk.unique.mia.quest.choices.no"));
        buff = i18n::s.get("core.locale.talk.unique.mia.quest.dialog");
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.mia.quest.no");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.mia.quest.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.mias_dream = 1;

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.mias_dream == 1)
    {
        if (chara_find_ally(246) == -1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.mia.quest.waiting");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.mia.quest.end", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.mia.quest.end", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _mia_receive_reward();

        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _renton_take_books()
{
    p(0) = 0;
    p(1) = 0;
    p(2) = 0;
    p(3) = 0;
    p(4) = 0;
    p(5) = 0;
    for (const auto& cnt : items(0))
    {
        if (inv[cnt].number() == 0)
        {
            continue;
        }
        if (inv[cnt].id == 668)
        {
            if (p(inv[cnt].param2) == 0)
            {
                inv[cnt].modify_number(-1);
                p(inv[cnt].param2) = 1;
            }
        }
    }
}

void _renton_receive_reward()
{
    flt();
    itemcreate(
        -1, 666, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1, 655, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 20000);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 5);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
    game_data.quest_flags.rare_books = 1000;
}

TalkResult talk_unique_renton()
{
    if (game_data.quest_flags.rare_books == 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.renton.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.rare_books == 0 ||
        game_data.quest_flags.rare_books == 1)
    {
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.renton.quest.dialog", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.renton.quest.dialog", 1);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.renton.quest.dialog", 2);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.renton.quest.dialog", 3);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        if (game_data.quest_flags.rare_books == 0)
        {
            quest_update_journal_msg();
            game_data.quest_flags.rare_books = 1;
            return TalkResult::talk_end;
        }

        p(0) = 0;
        p(1) = 0;
        p(2) = 0;
        p(3) = 0;
        p(4) = 0;
        p(5) = 0;
        for (const auto& cnt : items(0))
        {
            if (inv[cnt].number() == 0)
            {
                continue;
            }
            if (inv[cnt].id == 668)
            {
                p(inv[cnt].param2) = 1;
            }
        }
        for (int cnt = 1; cnt < 5; ++cnt)
        {
            p += p(cnt);
        }

        if (p != 0)
        {
            if (p != 4)
            {
                listmax = 0;
                buff = i18n::s.get(
                    "core.locale.talk.unique.renton.quest.brought", p(0));
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                return TalkResult::talk_end;
            }

            _renton_take_books();

            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.renton.quest.brought_all.dialog", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            if (jp)
            {
                buff = u8"…"s;
            }
            else
            {
                buff = u8"..."s;
            }
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            if (jp)
            {
                buff = u8"… …"s;
            }
            else
            {
                buff = u8"... ..."s;
            }
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            if (jp)
            {
                buff = u8"… … …"s;
            }
            else
            {
                buff = u8"... ... ..."s;
            }
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.renton.quest.brought_all.dialog", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            txt(i18n::s.get(
                    "core.locale.talk.unique.renton.quest.brought_all.ehekatl"),
                Message::color{ColorIndex::orange});
            txt(i18n::s.get("core.locale.quest.completed"));
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.renton.quest.brought_all.dialog", 2);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            _renton_receive_reward();
        }
        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _marks_receive_invitation()
{
    cdata.player().gold -= 20000;
    snd("core.paygold1");
    flt();
    itemcreate(-1, 24, cdata.player().position.x, cdata.player().position.y, 0);
    inv[ci].param1 = 13;
}

TalkResult talk_unique_marks()
{
    if (game_data.quest_flags.pyramid_trial != 0)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.marks.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (cdata.player().fame < 3000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.marks.fame_too_low");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.pyramid_trial == 0)
    {
        listmax = 0;
        buff = i18n::s.get_enum(
            "core.locale.talk.unique.marks.quest.dialog", 0, cdatan(1, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        if (cdata.player().gold >= 20000)
        {
            ELONA_APPEND_RESPONSE(
                1,
                i18n::s.get("core.locale.talk.unique.marks.quest.choices.pay"));
        }
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        buff =
            i18n::s.get_enum("core.locale.talk.unique.marks.quest.dialog", 1);
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.marks.quest.bye");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));

        _marks_receive_invitation();

        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.marks.quest.pay");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.pyramid_trial = 1;

        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _noel_receive_reward()
{
    flt();
    nostack = 1;
    itemcreate(
        -1, 672, cdata.player().position.x, cdata.player().position.y, 0);
    inv[ci].param1 = 162;
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 6);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
    game_data.quest_flags.red_blossom_in_palmia = 1000;
}

void _noel_buy_nuke()
{
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    cdata.player().gold -= 12000;
    snd("core.paygold1");
    flt();
    itemcreate(
        -1, 671, cdata.player().position.x, cdata.player().position.y, 0);
}

TalkResult talk_unique_noel()
{
    if (game_data.quest_flags.red_blossom_in_palmia == 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.noel.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
    }
    if (game_data.quest_flags.red_blossom_in_palmia == 1)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.noel.quest.waiting");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
    }
    if (game_data.quest_flags.red_blossom_in_palmia == 2)
    {
        _noel_receive_reward();

        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.noel.quest.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
    }
    if (game_data.quest_flags.red_blossom_in_palmia == 0)
    {
        listmax = 0;
        buff = i18n::s.get_enum("core.locale.talk.unique.noel.quest.dialog", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.noel.quest.choices.of_course"));
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        buff = i18n::s.get_enum("core.locale.talk.unique.noel.quest.dialog", 1);
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.noel.quest.bye");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }

        quest_update_journal_msg();
        game_data.quest_flags.red_blossom_in_palmia = 1;

        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.noel.quest.of_course");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
    }
    if (cdata.player().gold >= 12000)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.noel.quest.buy_nuke.choices.buy"));
    }
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
    buff = i18n::s.get("core.locale.talk.unique.noel.quest.buy_nuke.dialog");
    chatval_ = talk_window_query();
    if (chatval_ != 1)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.noel.quest.buy_nuke.bye");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }

    _noel_buy_nuke();

    listmax = 0;
    buff = i18n::s.get("core.locale.talk.unique.noel.quest.buy_nuke.buy");
    tc = tc * 1 + 0;
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
    chatesc = 1;
    ELONA_TALK_SCENE_CUT();
    return TalkResult::talk_end;
}


void _icolle_receive_monster_balls()
{
    flt();
    nostack = 1;
    itemcreate(
        -1, 685, cdata.player().position.x, cdata.player().position.y, 6);
    inv[ci].param2 = 5;
    quest_update_journal_msg();
}

int _icolle_check_monster_balls()
{
    int f_ = 0;
    const auto tmp = inv_getheader(0);
    const auto invhead = tmp.first;
    const auto invrange = tmp.second;
    for (int cnt = invhead, cnt_end = invhead + invrange; cnt < cnt_end; ++cnt)
    {
        if (game_data.quest_flags.ambitious_scientist >= 6)
        {
            break;
        }
        if (inv[cnt].number() > 0)
        {
            if (inv[cnt].id == 685)
            {
                if (inv[cnt].subname != 0)
                {
                    inv[cnt].modify_number(-1);
                    ++game_data.quest_flags.ambitious_scientist;
                    f_ = 1;
                    txt(i18n::s.get(
                        "core.locale.talk.unique.icolle.quest.give.deliver",
                        inv[cnt]));
                    --cnt;
                    continue;
                }
            }
        }
    }

    return f_;
}

void _icolle_receive_reward()
{
    flt();
    itemcreate(
        -1, 684, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 2500);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 2);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
    game_data.quest_flags.ambitious_scientist = 1000;
}

TalkResult talk_unique_icolle()
{
    if (game_data.quest_flags.ambitious_scientist >= 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.icolle.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.ambitious_scientist == 0)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.icolle.quest.choices.alright"));
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        buff = i18n::s.get("core.locale.talk.unique.icolle.quest.dialog");
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.icolle.quest.bye");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }

        _icolle_receive_monster_balls();

        txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.icolle.quest.alright");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.ambitious_scientist = 1;

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.ambitious_scientist >= 1 &&
        game_data.quest_flags.ambitious_scientist <= 5)
    {
        f = _icolle_check_monster_balls();
        if (f)
        {
            listmax = 0;
            buff =
                i18n::s.get("core.locale.talk.unique.icolle.quest.give.have");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
        }
        if (f == 0)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.icolle.quest.give.do_not_have");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
        }
        refresh_burden_state();
    }
    if (game_data.quest_flags.ambitious_scientist >= 6)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.icolle.quest.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _icolle_receive_reward();
    }
    return TalkResult::talk_end;
}


void _balzak_receive_reward()
{
    flt();
    itemcreate(
        -1, 686, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    nostack = 1;
    itemcreate(
        -1, 685, cdata.player().position.x, cdata.player().position.y, 0);
    inv[ci].param2 = 30;
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 15000);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 4);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
}

TalkResult talk_unique_balzak()
{
    if (game_data.quest_flags.sewer_sweeping == 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.balzak.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.sewer_sweeping == 0)
    {
        listmax = 0;
        buff =
            i18n::s.get_enum("core.locale.talk.unique.balzak.quest.dialog", 0);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        ELONA_APPEND_RESPONSE(
            1, i18n::s.get("core.locale.talk.unique.balzak.quest.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0, i18n::s.get("core.locale.talk.unique.balzak.quest.choices.no"));
        buff =
            i18n::s.get_enum("core.locale.talk.unique.balzak.quest.dialog", 1);
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.balzak.quest.no");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.balzak.quest.yes");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.sewer_sweeping = 1;
        cell_featset(18, 45, tile_downstairs, 11, 20);

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.sewer_sweeping == 1)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.balzak.quest.waiting");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.sewer_sweeping == 2)
    {
        _balzak_receive_reward();

        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.balzak.quest.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        game_data.quest_flags.sewer_sweeping = 1000;
        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _lexus_join_mages_guild()
{
    game_data.ranks.at(8) = 10000;
    game_data.guild.belongs_to_thieves_guild = 0;
    game_data.guild.belongs_to_fighters_guild = 0;
    game_data.guild.belongs_to_mages_guild = 1;
    game_data.guild.mages_guild_quota = 0;
    game_data.guild.thieves_guild_quota = 0;
    game_data.guild.joining_mages_guild = 1000;
    game_data.guild.joining_thieves_guild = 0;
    game_data.guild.joining_fighters_guild = 0;
    game_data.guild.fighters_guild_quota_recurring = 0;
    game_data.guild.mages_guild_quota_recurring = 0;
    game_data.guild.thieves_guild_quota_recurring = 0;
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    quest_update_journal_msg();
    txt(i18n::s.get("core.locale.talk.unique.lexus.nonmember.joined"),
        Message::color{ColorIndex::orange});
}

void _lexus_start_trial()
{
    game_data.guild.joining_mages_guild = 1;
    game_data.guild.mages_guild_quota = 30;
    quest_update_journal_msg();
}

void _lexus_update_quota()
{
    game_data.guild.mages_guild_quota_recurring = 1;
    game_data.guild.mages_guild_quota = 75 - game_data.ranks.at(8) / 200;
    quest_update_journal_msg();
}

void _lexus_move_self()
{
    cell_movechara(tc, 4, 20);
    cdata[tc].initial_position.x = 4;
    cdata[tc].initial_position.y = 20;
}

void _lexus_receive_reward()
{
    game_data.guild.mages_guild_quota_recurring = 0;
    flt(51 - game_data.ranks.at(8) / 200);
    flttypemajor = 54000;
    itemcreate(-1, 0, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1,
        54,
        cdata.player().position.x,
        cdata.player().position.y,
        10000 - game_data.ranks.at(8) + 1000);
    flt();
    itemcreate(
        -1,
        55,
        cdata.player().position.x,
        cdata.player().position.y,
        clamp(4 - game_data.ranks.at(8) / 2500, 1, 4));
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
    modrank(8, 500, 8);
}

TalkResult talk_unique_lexus()
{
    if (game_data.guild.belongs_to_mages_guild == 0)
    {
        ELONA_APPEND_RESPONSE(
            2,
            i18n::s.get("core.locale.talk.unique.lexus.nonmember.choices.tell_"
                        "me_about"));
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get("core.locale.talk.unique.lexus.nonmember.choices.want_"
                        "to_join"));
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        buff = i18n::s.get("core.locale.talk.unique.lexus.nonmember.dialog");
        chatval_ = talk_window_query();
        if (chatval_ == 2)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.lexus.nonmember.tell_me_about");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_unique;
        }
        if (chatval_ == 1)
        {
            if (game_data.guild.joining_mages_guild == 0)
            {
                listmax = 0;
                buff = i18n::s.get_enum(
                    "core.locale.talk.unique.lexus.nonmember.want_to_join", 0);
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                listmax = 0;
                buff = i18n::s.get_enum(
                    "core.locale.talk.unique.lexus.nonmember.want_to_join", 1);
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();

                _lexus_start_trial();

                listmax = 0;
                buff = i18n::s.get_enum(
                    "core.locale.talk.unique.lexus.nonmember.want_to_join", 2);
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                return TalkResult::talk_unique;
            }
            if (game_data.guild.mages_guild_quota > 0)
            {
                listmax = 0;
                buff = i18n::s.get(
                    "core.locale.talk.unique.lexus.nonmember.quota");
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                return TalkResult::talk_unique;
            }
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lexus.nonmember.end", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            _lexus_join_mages_guild();

            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lexus.nonmember.end", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            _lexus_move_self();
        }
        return TalkResult::talk_end;
    }
    _lexus_move_self();

    if (game_data.guild.mages_guild_quota_recurring == 0)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.lexus.member.choices.new_quota"));
    }
    else
    {
        ELONA_APPEND_RESPONSE(
            2,
            i18n::s.get(
                "core.locale.talk.unique.lexus.member.choices.report_quota"));
    }
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
    buff = i18n::s.get(
        "core.locale.talk.unique.lexus.member.dialog",
        ranktitle(8),
        cdatan(0, 0));
    chatval_ = talk_window_query();
    if (chatval_ == 1)
    {
        _lexus_update_quota();

        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.lexus.member.new_quota",
            game_data.guild.mages_guild_quota);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_unique;
    }
    if (chatval_ == 2)
    {
        if (game_data.guild.mages_guild_quota > 0)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.lexus.member.report_quota.waiting");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_unique;
        }

        _lexus_receive_reward();

        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.lexus.member.report_quota.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    return TalkResult::talk_end;
}


void _abyss_start_trial()
{
    game_data.guild.joining_thieves_guild = 1;
    quest_update_journal_msg();
}

void _abyss_join_thieves_guild()
{
    game_data.ranks.at(8) = 10000;
    game_data.guild.belongs_to_thieves_guild = 1;
    game_data.guild.belongs_to_fighters_guild = 0;
    game_data.guild.belongs_to_mages_guild = 0;
    game_data.guild.mages_guild_quota = 0;
    game_data.guild.thieves_guild_quota = 0;
    game_data.guild.joining_mages_guild = 0;
    game_data.guild.joining_thieves_guild = 1000;
    game_data.guild.joining_fighters_guild = 0;
    game_data.guild.fighters_guild_quota_recurring = 0;
    game_data.guild.mages_guild_quota_recurring = 0;
    game_data.guild.thieves_guild_quota_recurring = 0;
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    quest_update_journal_msg();
    txt(i18n::s.get("core.locale.talk.unique.abyss.nonmember.joined"),
        Message::color{ColorIndex::orange});
}

void _abyss_move_self()
{
    cell_movechara(tc, 17, 17);
    cdata[tc].initial_position.x = 17;
    cdata[tc].initial_position.y = 17;
}

void _abyss_update_quota()
{
    game_data.guild.thieves_guild_quota_recurring = 1;
    game_data.guild.thieves_guild_quota =
        (10000 - game_data.ranks.at(8)) * 6 + 1000;
    quest_update_journal_msg();
}

void _abyss_receive_reward()
{
    game_data.guild.thieves_guild_quota_recurring = 0;
    flt(51 - game_data.ranks.at(8) / 200);
    flttypemajor = 60000;
    itemcreate(-1, 0, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1,
        54,
        cdata.player().position.x,
        cdata.player().position.y,
        10000 - game_data.ranks.at(8) + 1000);
    flt();
    itemcreate(
        -1,
        55,
        cdata.player().position.x,
        cdata.player().position.y,
        clamp(3 - game_data.ranks.at(8) / 3000, 1, 3));
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
    modrank(8, 500, 8);
}

TalkResult talk_unique_abyss()
{
    if (game_data.guild.belongs_to_thieves_guild == 0)
    {
        ELONA_APPEND_RESPONSE(
            2,
            i18n::s.get("core.locale.talk.unique.abyss.nonmember.choices.tell_"
                        "me_about"));
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get("core.locale.talk.unique.abyss.nonmember.choices.want_"
                        "to_join"));
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        buff = i18n::s.get("core.locale.talk.unique.abyss.nonmember.dialog");
        chatval_ = talk_window_query();
        if (chatval_ == 2)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.abyss.nonmember.tell_me_about");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_unique;
        }
        if (chatval_ == 1)
        {
            if (game_data.guild.joining_thieves_guild == 0)
            {
                listmax = 0;
                buff = i18n::s.get_enum(
                    "core.locale.talk.unique.abyss.nonmember.want_to_join", 0);
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();

                _abyss_start_trial();

                listmax = 0;
                buff = i18n::s.get_enum(
                    "core.locale.talk.unique.abyss.nonmember.want_to_join", 1);
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                return TalkResult::talk_unique;
            }
            if (game_data.left_bill <= 4)
            {
                listmax = 0;
                buff = i18n::s.get(
                    "core.locale.talk.unique.abyss.nonmember.quota");
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                return TalkResult::talk_unique;
            }
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.abyss.nonmember.end", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            _abyss_join_thieves_guild();

            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.abyss.nonmember.end", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            _abyss_move_self();
        }
        return TalkResult::talk_end;
    }
    _abyss_move_self();

    if (game_data.guild.thieves_guild_quota_recurring == 0)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.lexus.member.choices.new_quota"));
    }
    else
    {
        ELONA_APPEND_RESPONSE(
            2,
            i18n::s.get(
                "core.locale.talk.unique.lexus.member.choices.report_quota"));
    }
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
    buff = i18n::s.get(
        "core.locale.talk.unique.abyss.member.dialog",
        ranktitle(8),
        cdatan(0, 0));
    chatval_ = talk_window_query();
    if (chatval_ == 1)
    {
        _abyss_update_quota();

        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.abyss.member.new_quota",
            game_data.guild.thieves_guild_quota);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_unique;
    }
    if (chatval_ == 2)
    {
        if (game_data.guild.thieves_guild_quota > 0)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.lexus.member.report_quota.waiting");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_unique;
        }

        _abyss_receive_reward();

        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.lexus.member.report_quota.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    return TalkResult::talk_end;
}


void _doria_start_trial()
{
    game_data.guild.joining_fighters_guild = 1;
    quest_update_journal_msg();
    while (1)
    {
        flt(10);
        chara_create(56, 0, -3, 0);
        if (the_character_db[cdata.tmp().id]->rarity / 1000 < 70)
        {
            continue;
        }
        if (cdata.tmp().relationship != -3)
        {
            continue;
        }
        if (cdata.tmp().quality >= Quality::miracle)
        {
            continue;
        }
        break;
    }
    game_data.guild.fighters_guild_quota = 15;
    game_data.guild.fighters_guild_target = cdata.tmp().id;
    chara_vanquish(56);
}

void _doria_join_fighters_guild()
{
    game_data.ranks.at(8) = 10000;
    game_data.guild.belongs_to_thieves_guild = 0;
    game_data.guild.belongs_to_fighters_guild = 1;
    game_data.guild.belongs_to_mages_guild = 0;
    game_data.guild.mages_guild_quota = 0;
    game_data.guild.thieves_guild_quota = 0;
    game_data.guild.joining_mages_guild = 0;
    game_data.guild.joining_thieves_guild = 0;
    game_data.guild.joining_fighters_guild = 1000;
    game_data.guild.fighters_guild_quota_recurring = 0;
    game_data.guild.mages_guild_quota_recurring = 0;
    game_data.guild.thieves_guild_quota_recurring = 0;
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    quest_update_journal_msg();
    txt(i18n::s.get("core.locale.talk.unique.doria.nonmember.joined"),
        Message::color{ColorIndex::orange});
}

void _doria_move_self()
{
    cell_movechara(tc, 29, 2);
    cdata[tc].initial_position.x = 29;
    cdata[tc].initial_position.y = 2;
}

void _doria_update_quota()
{
    while (1)
    {
        flt(cdata.player().level + 10);
        chara_create(56, 0, -3, 0);
        if (the_character_db[cdata.tmp().id]->rarity / 1000 < 70)
        {
            continue;
        }
        if (cdata.tmp().relationship != -3)
        {
            continue;
        }
        if (cdata.tmp().quality >= Quality::miracle)
        {
            continue;
        }
        break;
    }
    game_data.guild.fighters_guild_quota = 2 + rnd(3);
    game_data.guild.fighters_guild_target = cdata.tmp().id;
    chara_vanquish(56);
    game_data.guild.fighters_guild_quota_recurring = 1;
    quest_update_journal_msg();
}

void _doria_receive_reward()
{
    game_data.guild.fighters_guild_quota_recurring = 0;
    flt(51 - game_data.ranks.at(8) / 200, calcfixlv(Quality::good));
    flttypemajor = 10000;
    itemcreate(-1, 0, cdata.player().position.x, cdata.player().position.y, 0);
    flt();
    itemcreate(
        -1,
        54,
        cdata.player().position.x,
        cdata.player().position.y,
        10000 - game_data.ranks.at(8) + 1000);
    flt();
    itemcreate(
        -1,
        55,
        cdata.player().position.x,
        cdata.player().position.y,
        clamp(4 - game_data.ranks.at(8) / 2500, 1, 4));
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
    modrank(8, 500, 8);
}

TalkResult talk_unique_doria()
{
    if (game_data.guild.belongs_to_fighters_guild == 0)
    {
        ELONA_APPEND_RESPONSE(
            2,
            i18n::s.get("core.locale.talk.unique.doria.nonmember.choices.tell_"
                        "me_about"));
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get("core.locale.talk.unique.doria.nonmember.choices.want_"
                        "to_join"));
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        buff = i18n::s.get("core.locale.talk.unique.doria.nonmember.dialog");
        chatval_ = talk_window_query();
        if (chatval_ == 2)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.doria.nonmember.tell_me_about");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_unique;
        }
        if (chatval_ == 1)
        {
            if (game_data.guild.joining_fighters_guild == 0)
            {
                listmax = 0;
                buff = i18n::s.get_enum(
                    "core.locale.talk.unique.doria.nonmember.want_to_join", 0);
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                listmax = 0;

                _doria_start_trial();

                buff = i18n::s.get_enum(
                    "core.locale.talk.unique.doria.nonmember.want_to_join",
                    1,
                    game_data.guild.fighters_guild_quota,
                    chara_refstr(game_data.guild.fighters_guild_target, 2));
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                return TalkResult::talk_unique;
            }
            if (game_data.guild.fighters_guild_quota > 0)
            {
                listmax = 0;
                buff = i18n::s.get(
                    "core.locale.talk.unique.doria.nonmember.quota",
                    game_data.guild.fighters_guild_quota,
                    chara_refstr(game_data.guild.fighters_guild_target, 2));
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                return TalkResult::talk_unique;
            }
            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.lexus.nonmember.end", 0);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            _doria_join_fighters_guild();

            listmax = 0;
            buff = i18n::s.get_enum(
                "core.locale.talk.unique.doria.nonmember.end", 1);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            _doria_move_self();
        }
        return TalkResult::talk_end;
    }
    _doria_move_self();

    if (game_data.guild.fighters_guild_quota_recurring == 0)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.lexus.member.choices.new_quota"));
    }
    else
    {
        ELONA_APPEND_RESPONSE(
            2,
            i18n::s.get(
                "core.locale.talk.unique.lexus.member.choices.report_quota"));
    }
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
    buff = i18n::s.get(
        "core.locale.talk.unique.doria.member.dialog",
        ranktitle(8),
        cdatan(0, 0));
    chatval_ = talk_window_query();
    if (chatval_ == 1)
    {
        _doria_update_quota();

        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.doria.member.new_quota",
            game_data.guild.fighters_guild_quota,
            chara_refstr(game_data.guild.fighters_guild_target, 2));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_unique;
    }
    if (chatval_ == 2)
    {
        if (game_data.guild.fighters_guild_quota > 0)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.lexus.member.report_quota.waiting");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_unique;
        }

        _doria_receive_reward();

        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.lexus.member.report_quota.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    return TalkResult::talk_end;
}


void _conery_receive_reward()
{
    flt();
    nostack = 1;
    itemcreate(
        -1, 630, cdata.player().position.x, cdata.player().position.y, 0);
    change_item_material(inv[ci], 31);
    flt();
    itemcreate(
        -1, 54, cdata.player().position.x, cdata.player().position.y, 50000);
    flt();
    itemcreate(-1, 55, cdata.player().position.x, cdata.player().position.y, 4);
    txt(i18n::s.get("core.locale.quest.completed"));
    snd("core.complete1");
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
    game_data.quest_flags.minotaur_king = 1000;
}

TalkResult talk_unique_conery()
{
    if (game_data.quest_flags.minotaur_king == 1000)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.conery.complete");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.minotaur_king == 0)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get("core.locale.talk.unique.conery.quest.choices.do_it"));
        ELONA_APPEND_RESPONSE(
            0, i18n::s.get("core.locale.talk.unique.conery.quest.choices.bye"));
        buff = i18n::s.get("core.locale.talk.unique.conery.quest.dialog");
        chatval_ = talk_window_query();
        if (chatval_ != 1)
        {
            listmax = 0;
            buff = i18n::s.get("core.locale.talk.unique.conery.quest.bye");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return TalkResult::talk_end;
        }
        quest_update_journal_msg();
        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.conery.quest.do_it", cdatan(0, 0));
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        game_data.quest_flags.minotaur_king = 1;

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.minotaur_king == 1)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.conery.quest.waiting");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.minotaur_king == 2)
    {
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.conery.quest.end");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _conery_receive_reward();

        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}

void _rogue_boss_surrender()
{
    snd("core.paygold1");
    cdata.player().gold -= cdata.player().gold / 5;
    for (const auto& cnt : items(0))
    {
        if (inv[cnt].number() > 0)
        {
            if (the_item_db[inv[cnt].id]->is_cargo)
            {
                txt(i18n::s.get(
                    "core.locale.talk.npc.common.hand_over", inv[cnt]));
                inv[cnt].remove();
            }
        }
    }
    refresh_burden_state();
}

void _rogue_boss_leave_map()
{
    levelexitby = 4;
    chatteleport = 1;
    snd("core.exitmap1");
}

TalkResult talk_unique_rogue_boss()
{
    if (cdata.player().gold <= 10)
    {
        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.rogue_boss.too_poor", cdata[tc]);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    ELONA_APPEND_RESPONSE(
        1,
        i18n::s.get(
            "core.locale.talk.unique.rogue_boss.ambush.choices.try_me"));
    ELONA_APPEND_RESPONSE(
        0,
        i18n::s.get(
            "core.locale.talk.unique.rogue_boss.ambush.choices.surrender"));
    buff = i18n::s.get(
        "core.locale.talk.unique.rogue_boss.ambush.dialog",
        gdatan(1),
        (cdata.player().gold / 5),
        cdata[tc]);
    chatesc = 0;
    chatval_ = talk_window_query();
    if (chatval_ == 0)
    {
        _rogue_boss_surrender();

        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.rogue_boss.ambush.surrender", cdata[tc]);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _rogue_boss_leave_map();

        return TalkResult::talk_end;
    }
    listmax = 0;
    buff = i18n::s.get(
        "core.locale.talk.unique.rogue_boss.ambush.try_me", cdata[tc]);
    tc = tc * 1 + 0;
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
    chatesc = 1;
    ELONA_TALK_SCENE_CUT();
    return TalkResult::talk_end;
}


void _strange_scientist_receive_little_balls()
{
    game_data.quest_flags.little_sister = 0;
    flt();
    itemcreate(
        -1, 699, cdata.player().position.x, cdata.player().position.y, 0);
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
    quest_update_journal_msg();
    game_data.quest_flags.little_sister = 1;
}

bool _strange_scientist_can_receive_reward()
{
    p = game_data.quest_flags.kill_count_of_little_sister;
    for (int cnt = 1,
             cnt_end =
                 cnt + (game_data.quest_flags.gift_count_of_little_sister + 1);
         cnt < cnt_end;
         ++cnt)
    {
        p += cnt;
    }

    return game_data.quest_flags.save_count_of_little_sister >= p || 0;
}

void _strange_scientist_pick_reward()
{
    begintempinv();
    mode = 6;
    flt();
    itemcreate(-1, 283, -1, -1, 0);
    flt();
    itemcreate(-1, 284, -1, -1, 0);
    for (int cnt = 0; cnt < 800; ++cnt)
    {
        if (cnt == 672)
        {
            continue;
        }
        randomize(game_data.date.day + cnt);
        f = 0;
        if (itemmemory(0, cnt))
        {
            f = 1;
        }
        if (cnt == 662)
        {
            if (game_data.quest_flags.kamikaze_attack >= 1000)
            {
                f = 1;
            }
        }
        if (cnt == 655)
        {
            if (game_data.quest_flags.rare_books >= 1000)
            {
                f = 1;
            }
        }
        if (cnt == 639)
        {
            if (game_data.quest_flags.pael_and_her_mom >= 1000)
            {
                f = 1;
            }
        }
        if (f)
        {
            flt(cdata.player().level * 3 / 2, calcfixlv(Quality::good));
            int stat = itemcreate(-1, cnt, -1, -1, 0);
            if (stat == 1)
            {
                if (inv[ci].quality < Quality::miracle)
                {
                    inv[ci].remove();
                }
            }
        }
        randomize();
    }
    txt(
        i18n::s.get("core.locale.talk.unique.strange_scientist."
                    "reward.choose_one"));
    invsubroutine = 1;
    invctrl(0) = 22;
    invctrl(1) = 4;
    ctrl_inventory();
    exittempinv();
    mode = 0;
}

void _strange_scientist_replenish_little_balls()
{
    flt();
    itemcreate(
        -1, 699, cdata.player().position.x, cdata.player().position.y, 0);
    txt(i18n::s.get("core.locale.common.something_is_put_on_the_ground"));
}

void _strange_scientist_turn_over()
{
    txt(i18n::s.get(
        "core.locale.talk.unique.strange_scientist.turn_over.text"));
    ++game_data.quest_flags.save_count_of_little_sister;
    txt(i18n::s.get(
            "core.locale.talk.unique.strange_scientist.saved_count",
            game_data.quest_flags.save_count_of_little_sister,
            game_data.quest_flags.kill_count_of_little_sister),
        Message::color{ColorIndex::green});
    chara_vanquish(chara_find_ally(319));
    snd("core.complete1");
}

TalkResult talk_unique_strange_scientist()
{
    if (game_data.quest_flags.little_sister == 0)
    {
        listmax = 0;
        i18n::s.get("core.locale.talk.unique.strange_scientist.first");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();

        _strange_scientist_receive_little_balls();

        return TalkResult::talk_end;
    }
    if (game_data.quest_flags.little_sister > 0)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.strange_scientist.choices.reward"));
        ELONA_APPEND_RESPONSE(
            2,
            i18n::s.get(
                "core.locale.talk.unique.strange_scientist.choices.replenish"));
        if (chara_find_ally(319) != -1)
        {
            ELONA_APPEND_RESPONSE(
                3,
                i18n::s.get("core.locale.talk.unique.strange_scientist.choices."
                            "turn_over"));
        }
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        i18n::s.get("core.locale.talk.unique.strange_scientist.dialog");
        chatval_ = talk_window_query();
        if (chatval_ == 1)
        {
            if (_strange_scientist_can_receive_reward())
            {
                listmax = 0;
                i18n::s.get(
                    "core.locale.talk.unique.strange_scientist.reward.dialog");
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();

                _strange_scientist_pick_reward();

                listmax = 0;
                buff = i18n::s.get(
                    "core.locale.talk.unique.strange_scientist.reward.find");
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
            }
            else
            {
                listmax = 0;
                buff = i18n::s.get(
                    "core.locale.talk.unique.strange_scientist.reward.not_"
                    "enough");
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
            }
            return TalkResult::talk_end;
        }
        if (chatval_ == 2)
        {
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.strange_scientist.replenish");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            _strange_scientist_replenish_little_balls();

            return TalkResult::talk_end;
        }
        if (chatval_ == 3)
        {
            _strange_scientist_turn_over();

            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.strange_scientist.turn_over.dialog");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
        }
        return TalkResult::talk_end;
    }
    return TalkResult::talk_ignored;
}


void _kaneda_bike_join_party()
{
    rc = tc;
    new_ally_joins();
    game_data.quest_flags.blue_capsule_drug = 0;
}

void _kaneda_bike_give_drug()
{
    int stat = inv_find(771, 0);
    inv[stat].modify_number(-1);
    txt(
        i18n::s.get("core.locale.talk.unique.kaneda_bike.before_"
                    "drug.yes.you_hand_him"));
    snd("core.equip1");
}

TalkResult talk_unique_kaneda_bike()
{
    if (game_data.quest_flags.blue_capsule_drug >= 3)
    {
        buff = i18n::s.get(
            "core.locale.talk.unique.kaneda_bike.after_drug.dialog");
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.kaneda_bike.after_drug.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0,
            i18n::s.get(
                "core.locale.talk.unique.kaneda_bike.after_drug.choices.no"));
        chatesc = 1;
        chatval_ = talk_window_query();
        if (chatval_ == 1)
        {
            f = chara_get_free_slot_ally();
            if (f == 0)
            {
                listmax = 0;
                buff = i18n::s.get(
                    "core.locale.talk.unique.kaneda_bike.after_drug.yes.party_"
                    "full");
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
            }
            listmax = 0;
            buff = i18n::s.get(
                "core.locale.talk.unique.kaneda_bike.after_drug.yes.dialog");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();

            _kaneda_bike_join_party();

            return TalkResult::talk_end;
        }
        listmax = 0;
        buff = i18n::s.get("core.locale.talk.unique.kaneda_bike.after_drug.no");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    {
        int stat = inv_find(771, 0);
        if (stat != -1)
        {
            buff = i18n::s.get(
                "core.locale.talk.unique.kaneda_bike.before_drug.dialog");
            ELONA_APPEND_RESPONSE(
                1,
                i18n::s.get("core.locale.talk.unique.kaneda_bike.before_drug."
                            "choices.yes"));
            ELONA_APPEND_RESPONSE(
                0,
                i18n::s.get("core.locale.talk.unique.kaneda_bike.before_drug."
                            "choices.no"));
            chatesc = 1;
            chatval_ = talk_window_query();
            if (chatval_ == 1)
            {
                _kaneda_bike_give_drug();

                listmax = 0;
                buff = i18n::s.get(
                    "core.locale.talk.unique.kaneda_bike.before_drug.yes."
                    "dialog");
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();

                ++game_data.quest_flags.blue_capsule_drug;

                return TalkResult::talk_end;
            }
        }
    }
    listmax = 0;
    buff = i18n::s.get("core.locale.talk.unique.kaneda_bike.do_not_have_drug");
    tc = tc * 1 + 0;
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
    chatesc = 1;
    ELONA_TALK_SCENE_CUT();
    return TalkResult::talk_end;
}


void _part_time_worker_switch_religion()
{
    flt();
    itemcreate(
        -1, 767, cdata.player().position.x, cdata.player().position.y, 0);
    cdata.player().god_id = core_god::int2godid(6);
    switch_religion();
}

TalkResult talk_unique_part_time_worker()
{
    if (game_data.current_map != mdata_t::MapId::noyel ||
        !area_data[game_data.current_map].christmas_festival)
    {
        return TalkResult::talk_end;
    }
    if (cdata.player().god_id == core_god::jure)
    {
        listmax = 0;
        buff = i18n::s.get(
            "core.locale.talk.unique.part_time_worker.already_believe_in_jure");
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        return TalkResult::talk_end;
    }
    ELONA_APPEND_RESPONSE(
        1, i18n::s.get("core.locale.talk.unique.part_time_worker.choices.yes"));
    ELONA_APPEND_RESPONSE(
        0, i18n::s.get("core.locale.talk.unique.part_time_worker.choices.no"));
    buff = i18n::s.get("core.locale.talk.unique.part_time_worker.dialog");
    chatesc = 1;
    chatval_ = talk_window_query();
    if (chatval_ == 1)
    {
        ELONA_APPEND_RESPONSE(
            1,
            i18n::s.get(
                "core.locale.talk.unique.part_time_worker.yes.choices.yes"));
        ELONA_APPEND_RESPONSE(
            0,
            i18n::s.get(
                "core.locale.talk.unique.part_time_worker.yes.choices.no"));
        buff =
            i18n::s.get("core.locale.talk.unique.part_time_worker.yes.dialog");
        chatesc = 1;
        chatval_ = talk_window_query();
        if (chatval_ == 1)
        {
            listmax = 0;
            buff =
                i18n::s.get("core.locale.talk.unique.part_time_worker.yes.yes");
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            txt(i18n::s.get(
                "core.locale.common.something_is_put_on_the_ground"));

            _part_time_worker_switch_religion();

            return TalkResult::talk_end;
        }
    }
    listmax = 0;
    buff = i18n::s.get("core.locale.talk.unique.part_time_worker.no");
    tc = tc * 1 + 0;
    ELONA_APPEND_RESPONSE(0, i18n::s.get("core.locale.ui.bye"));
    chatesc = 1;
    ELONA_TALK_SCENE_CUT();
    return TalkResult::talk_end;
}

} // namespace

TalkResult talk_unique()
{
    listmax = 0;

    auto& chara = cdata[tc];
    const auto& dialog_id = the_character_db[chara.id]->dialog_id;

    if (dialog_id)
    {
        dialog_start(chara, *dialog_id);
        return TalkResult::talk_end;
    }

    switch (*chatval_unique_chara_id)
    {
    case 243: return talk_unique_arnord();
    case 247: return talk_unique_mia();
    case 252: return talk_unique_renton();
    case 253: return talk_unique_marks();
    case 259: return talk_unique_noel();
    case 279: return talk_unique_icolle();
    case 280: return talk_unique_balzak();
    case 290: return talk_unique_lexus();
    case 294: return talk_unique_abyss();
    case 297: return talk_unique_doria();
    case 301: return talk_unique_conery();
    case 302: return talk_unique_rogue_boss();
    case 322: return talk_unique_strange_scientist();
    case 345: return talk_unique_kaneda_bike();
    case 348: return talk_unique_part_time_worker();
    }
    return TalkResult::talk_ignored;
}

} // namespace elona
