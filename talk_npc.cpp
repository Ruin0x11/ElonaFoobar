#include "ability.hpp"
#include "adventurer.hpp"
#include "audio.hpp"
#include "calc.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "elona.hpp"
#include "event.hpp"
#include "food.hpp"
#include "i18n.hpp"
#include "item.hpp"
#include "item_db.hpp"
#include "macro.hpp"
#include "map_cell.hpp"
#include "menu.hpp"
#include "quest.hpp"
#include "shop.hpp"
#include "talk.hpp"
#include "ui.hpp"
#include "variables.hpp"



namespace elona
{

void talk_wrapper(talk_result_t initial)
{
    talk_result_t result = initial;
    bool finished = false;
    while (!finished)
    {
        switch (result)
        {
        case talk_result_t::talk_npc: result = talk_npc(); break;
        case talk_result_t::talk_unique: result = talk_unique(); break;
        case talk_result_t::talk_quest_giver:
            result = talk_quest_giver();
            break;
        case talk_result_t::talk_house_visitor:
            result = talk_house_visitor();
            break;
        case talk_result_t::talk_sleeping: result = talk_sleeping(); break;
        case talk_result_t::talk_busy: result = talk_busy(); break;
        case talk_result_t::talk_finish_escort:
            result = talk_finish_escort();
            break;
        case talk_result_t::talk_game_begin: result = talk_game_begin(); break;
        case talk_result_t::talk_more: result = talk_more(); break;
        case talk_result_t::talk_end:
            talk_end();
            finished = true;
            break;
        default: assert(0); break;
        }
    }
}


talk_result_t talk_npc()
{
    int tcchat = 0;
    listmax = 0;
    if (buff == ""s)
    {
        get_npc_talk();
        int stat = chara_custom_talk(tc, 106);
        if (stat)
        {
            text_replace_tags_in_quest_board();
        }
        if (cdata[tc].interest > 0)
        {
            if (cdata[tc].relationship != 10)
            {
                if (tc >= 16)
                {
                    if (rnd(3) == 0)
                    {
                        if (cdata[tc].impression < 100)
                        {
                            if (rnd(sdata(17, 0) + 1) > 10)
                            {
                                chara_mod_impression(tc, rnd(3));
                            }
                            else
                            {
                                chara_mod_impression(tc, rnd(3) * -1);
                            }
                        }
                    }
                    cdata[tc].interest -= rnd(30);
                    cdata[tc].time_interest_revive = gdata_hour + gdata_day * 24
                        + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12 + 8;
                }
            }
        }
    }
    if (cdata[tc].character_role == 18)
    {
        if (gdata_number_of_waiting_guests > 0)
        {
            ELONA_APPEND_RESPONSE(
                58, lang(u8"客に会う"s, u8"Yes, I'll meet the guest now."s));
            ELONA_APPEND_RESPONSE(
                59, lang(u8"追い返す"s, u8"I don't want to meet anyone."s));
        }
    }
    if (cdata[tc].interest > 0 && chatval(1) == 0)
    {
        ELONA_APPEND_RESPONSE(1, lang(u8"話がしたい"s, u8"Let's Talk."s));
    }
    if ((cdata[tc].character_role >= 1000 && cdata[tc].character_role < 2000)
        || cdata[tc].character_role == 2003)
    {
        ELONA_APPEND_RESPONSE(
            10, lang(u8"買いたい"s, u8"I want to buy something."s));
        ELONA_APPEND_RESPONSE(
            11, lang(u8"売りたい"s, u8"I want to sell something."s));
        if (cdata[tc].character_role == 1010)
        {
            ELONA_APPEND_RESPONSE(
                31, lang(u8"襲撃するよ"s, u8"Prepare to die!"s));
        }
        if (cdata[tc].character_role != 1010
            && cdata[tc].character_role != 1009)
        {
            ELONA_APPEND_RESPONSE(
                12,
                lang(
                    u8"投資したい"s,
                    u8"Need someone to invest in your shop?"s));
        }
    }
    if (cdata[tc].character_role == 9)
    {
        ELONA_APPEND_RESPONSE(
            33, lang(u8"仲間を呼び戻す"s, u8"Call back my allies."s));
    }
    if (cdata[tc].character_role == 17)
    {
        if (chara_get_free_slot_ally() != 0)
        {
            ELONA_APPEND_RESPONSE(
                36, lang(u8"奴隷を買う"s, u8"I want to buy a slave."s));
        }
        ELONA_APPEND_RESPONSE(
            37, lang(u8"奴隷を売る"s, u8"I want to sell a slave."s));
    }
    if (cdata[tc].character_role == 22)
    {
        if (chara_get_free_slot_ally() != 0)
        {
            ELONA_APPEND_RESPONSE(
                57, lang(u8"馬を買う"s, u8"I want to buy a horse."s));
        }
    }
    if (tc < 16)
    {
        if (cdata[tc].is_escorted() == 0)
        {
            if (cdata[tc].is_escorted_in_sub_quest() == 0)
            {
                ELONA_APPEND_RESPONSE(
                    34, lang(u8"街で待機しろ"s, u8"Wait at the town."s));
                if (cdata[tc].is_married() == 0)
                {
                    ELONA_APPEND_RESPONSE(
                        38,
                        lang(
                            u8"婚約を申し込む"s,
                            u8"May I ask for your hand?"s));
                }
                else if (gdata_continuous_active_hours >= 15)
                {
                    ELONA_APPEND_RESPONSE(
                        39, lang(u8"遺伝子を残す"s, u8"Let's make a gene."s));
                }
                if (cdata[tc].can_talk != 0)
                {
                    if (cdata[tc].is_silent() == 0)
                    {
                        ELONA_APPEND_RESPONSE(
                            48, lang(u8"黙らせる"s, u8"Shut up."s));
                    }
                    else
                    {
                        ELONA_APPEND_RESPONSE(
                            48, lang(u8"喋らせる"s, u8"You can speak now."s));
                    }
                }
                ELONA_APPEND_RESPONSE(
                    35, lang(u8"縁を切る"s, u8"I'm going to abandon you."s));
            }
        }
    }
    if (cdata[tc].character_role == 1000 || cdata[tc].character_role == 1001)
    {
        ELONA_APPEND_RESPONSE(
            54, lang(u8"矢弾の充填"s, u8"I need ammos for my weapon."s));
    }
    if (cdata[tc].character_role == 1005)
    {
        ELONA_APPEND_RESPONSE(
            13,
            lang(u8"食事をとる"s, u8"Bring me something to eat."s) + u8" ("s
                + calcmealvalue() + i18n::_(u8"ui", u8"gold") + u8")"s);
        if (gdata_weather == 1 || gdata_weather == 4 || gdata_weather == 2)
        {
            ELONA_APPEND_RESPONSE(
                43, lang(u8"シェルターに入る"s, u8"Take me to the shelter."s));
        }
    }
    if (cdata[tc].character_role == 5)
    {
        ELONA_APPEND_RESPONSE(
            14,
            lang(u8"鑑定したい"s, u8"I need you to identify an item."s)
                + u8" ("s + calcidentifyvalue(0) + i18n::_(u8"ui", u8"gold")
                + u8")"s);
        ELONA_APPEND_RESPONSE(
            15,
            lang(u8"全て鑑定してほしい"s, u8"Identify all of my stuff."s)
                + u8" ("s + calcidentifyvalue(1) + i18n::_(u8"ui", u8"gold")
                + u8")"s);
        ELONA_APPEND_RESPONSE(
            16,
            lang(u8"調査したい"s, u8"Investigate an item."s) + u8" ("s
                + calcidentifyvalue(2) + i18n::_(u8"ui", u8"gold") + u8")"s);
    }
    if (cdata[tc].character_role == 7)
    {
        ELONA_APPEND_RESPONSE(17, lang(u8"訓練したい"s, u8"Train me."s));
        ELONA_APPEND_RESPONSE(
            30,
            lang(
                u8"新しい能力を覚えたい"s, u8"What skills can you teach me?"s));
    }
    if (cdata[tc].character_role == 8)
    {
        ELONA_APPEND_RESPONSE(
            18, lang(u8"冒険者の情報"s, u8"Show me the list of adventurers."s));
        ELONA_APPEND_RESPONSE(
            47,
            lang(
                u8"仲間の調査"s,
                u8"I want you to investigate one of my allies."s));
    }
    if (cdata[tc].character_role == 12)
    {
        ELONA_APPEND_RESPONSE(
            19,
            lang(u8"能力の復元"s, u8"Restore my attributes."s) + u8"("s
                + calcrestorecost() + i18n::_(u8"ui", u8"gold") + u8")"s);
    }
    if (cdata[tc].character_role == 13)
    {
        ELONA_APPEND_RESPONSE(
            20, lang(u8"アイテム交換"s, u8"Are you interested in trade?"s));
        if (cdata[tc].is_contracting() == 0)
        {
            ELONA_APPEND_RESPONSE(
                50, lang(u8"護衛を依頼する"s, u8"I want to hire you."s));
            ELONA_APPEND_RESPONSE(51, lang(u8"仲間に誘う"s, u8"Join me!"s));
        }
    }
    if (cdata[tc].character_role == 10)
    {
        ELONA_APPEND_RESPONSE(
            21,
            lang(u8"試合に出る[決闘]"s, u8"I'm entering the arena. [Duel]"s));
        ELONA_APPEND_RESPONSE(
            22,
            lang(
                u8"試合に出る[ランブル]"s,
                u8"I'm entering the arena. [Rumble]"s));
        ELONA_APPEND_RESPONSE(
            23, lang(u8"成績を聞く"s, u8"Tell me my scores."s));
    }
    if (cdata[tc].character_role == 11)
    {
        ELONA_APPEND_RESPONSE(
            40,
            lang(u8"ペットデュエル"s, u8"I want to register my pet. [Duel]"s));
        ELONA_APPEND_RESPONSE(
            41,
            lang(u8"チームバトル"s, u8"I want to register my team. [Team]"s));
        ELONA_APPEND_RESPONSE(
            42, lang(u8"成績を聞く"s, u8"Tell me my scores."s));
    }
    if (cdata[tc].character_role == 18)
    {
        ELONA_APPEND_RESPONSE(
            45,
            lang(
                u8"家の名前を考えてくれ"s,
                u8"Think of a nice name for my house."s));
    }
    if (cdata[tc].character_role == 19)
    {
        ELONA_APPEND_RESPONSE(
            46, lang(u8"免罪符を買いたい"s, u8"I want to buy an indulgence."s));
    }
    int stat = talk_check_trade(tc);
    if (stat)
    {
        ELONA_APPEND_RESPONSE(
            20, lang(u8"アイテム交換"s, u8"Are you interested in trade?"s));
    }
    if (cdata[tc].character_role == 14)
    {
        int stat = talk_guide_quest_client();
        if (stat != 0)
        {
            for (int cnt = 0, cnt_end = (stat); cnt < cnt_end; ++cnt)
            {
                ELONA_APPEND_RESPONSE(
                    10000 + cnt,
                    lang(
                        cdatan(0, rtval(cnt)) + u8"の居場所を聞く"s,
                        u8"Where is "s + cdatan(0, rtval(cnt)) + u8"?"s));
            }
        }
        if (itemfind(0, 284) != -1)
        {
            ELONA_APPEND_RESPONSE(
                32,
                lang(
                    u8"落し物の財布を届ける"s,
                    u8"Here is a lost wallet I found."s));
        }
        else if (itemfind(0, 283) != -1)
        {
            ELONA_APPEND_RESPONSE(
                32,
                lang(
                    u8"落し物のカバンを届ける。"s,
                    u8"Here is a lost suitcase I found."s));
        }
    }
    if (cdata[tc].character_role == 21)
    {
        ELONA_APPEND_RESPONSE(
            53, lang(u8"帰還したい"s, u8"I want to return."s));
    }
    if (cdata[tc].character_role == 1020)
    {
        if (gdata_belongs_to_mages_guild != 0)
        {
            ELONA_APPEND_RESPONSE(
                55,
                lang(u8"魔法書の予約"s, u8"I want to reserve some books."s));
        }
    }
    if (cdata[tc].drunk != 0 || 0)
    {
        if (gdata_current_map != 35)
        {
            if (tc >= 16)
            {
                if (event_id() == -1)
                {
                    ELONA_APPEND_RESPONSE(
                        56,
                        lang(
                            u8"気持ちいいことしない？"s,
                            u8"Interested in a little tail t'night?"s));
                }
            }
        }
    }
    if (cdata[tc].id == 335)
    {
        if (event_id() == -1)
        {
            ELONA_APPEND_RESPONSE(
                60, lang(u8"暗い場所に移ろう"s, u8"I'll buy you."s));
        }
    }
    if (cdata[tc].character_role == 23)
    {
        ELONA_APPEND_RESPONSE(
            61, lang(u8"キャラバンを雇う"s, u8"Hire caravan."s));
    }
    f = 0;
    deliver(0) = -1;
    deliver(1) = -1;
    if (gdata_current_dungeon_level == 1)
    {
        for (int cnt = 0, cnt_end = (gdata_number_of_existing_quests);
             cnt < cnt_end;
             ++cnt)
        {
            if (qdata(1, cnt) == gdata_current_map)
            {
                if (qdata(0, cnt) == tc)
                {
                    rq = cnt;
                    f = 1;
                    break;
                }
            }
        }
    }
    if (f == 1)
    {
        for (int cnt = 0, cnt_end = (gdata_number_of_existing_quests);
             cnt < cnt_end;
             ++cnt)
        {
            if (qdata(3, cnt) == 0)
            {
                continue;
            }
            if (qdata(8, cnt) != 1)
            {
                continue;
            }
            if (qdata(14, cnt) == 2)
            {
                if (qdata(10, cnt) == rq)
                {
                    p = qdata(11, cnt);
                    deliver = cnt;
                    for (const auto& cnt : items(0))
                    {
                        if (inv[cnt].number == 0)
                        {
                            continue;
                        }
                        if (inv[cnt].id == p)
                        {
                            deliver(1) = cnt;
                            break;
                        }
                    }
                }
            }
        }
        if (qdata(8, rq) == 3)
        {
            quest_set_data(3);
            quest_complete();
        }
        else if (qdata(14, rq) == 3 && qdata(8, rq) == 1)
        {
            supply = -1;
            for (const auto& cnt : items(0))
            {
                if (inv[cnt].number == 0)
                {
                    continue;
                }
                if (ibit(13, cnt))
                {
                    continue;
                }
                if (qdata(3, rq) == 1003)
                {
                    if (the_item_db[inv[cnt].id]->category == 57000)
                    {
                        if (inv[cnt].param1 / 1000 == qdata(12, rq))
                        {
                            if (inv[cnt].param2 == qdata(13, rq))
                            {
                                supply = cnt;
                                break;
                            }
                        }
                    }
                }
                if (qdata(3, rq) == 1004 || qdata(3, rq) == 1011)
                {
                    if (inv[cnt].id == qdata(11, rq))
                    {
                        supply = cnt;
                        break;
                    }
                }
            }
            if (supply != -1)
            {
                ELONA_APPEND_RESPONSE(
                    26,
                    lang(
                        itemname(supply, 1) + u8"を納入する"s,
                        u8"Here is "s + itemname(supply, 1)
                            + u8" you asked."s));
            }
            else
            {
                ELONA_APPEND_RESPONSE(
                    24, lang(u8"依頼について"s, u8"About the work."s));
            }
        }
        else if (qdata(3, rq) != 0)
        {
            ELONA_APPEND_RESPONSE(
                24, lang(u8"依頼について"s, u8"About the work."s));
        }
    }
    if (deliver != -1 && deliver(1) != -1)
    {
        ELONA_APPEND_RESPONSE(
            25, lang(u8"配達物を渡す"s, u8"Here's your delivery."s));
    }
    if (gdata_current_map == 7)
    {
        if (tc >= 57)
        {
            if (cdata[tc].character_role != 0)
            {
                if ((cdata[tc].character_role < 2000
                     || cdata[tc].character_role >= 3000)
                    && event_id() == -1)
                {
                    ELONA_APPEND_RESPONSE(
                        44, lang(u8"解雇する"s, u8"You are fired."s));
                }
            }
        }
    }
    if (cdata[tc].character_role == 1015)
    {
        if (gdata_pael_and_her_mom == 1000)
        {
            rc = chara_find(222);
            if (rc != 0)
            {
                if (cdata[rc].state == 1)
                {
                    ELONA_APPEND_RESPONSE(
                        52,
                        lang(
                            u8"パエルの母を売る"s,
                            u8"I want to sell Pael's mom."s));
                }
            }
        }
    }
    ELONA_APPEND_RESPONSE(0, i18n::_(u8"ui", u8"bye"));
    chatesc = 1;
    talk_window_query();
    if (chatval == 10 || chatval == 11)
    {
        if ((cdata[tc].character_role >= 1000
             && cdata[tc].character_role < 2000)
            || cdata[tc].character_role == 2003)
        {
            if (cdata[0].karma < -30)
            {
                if (gdata_current_map != 14)
                {
                    if (gdata_current_map != 7)
                    {
                        if (cdata[0].is_incognito() == 0)
                        {
                            listmax = 0;
                            buff = lang(
                                u8"犯罪者に売る物はない"s + _yo(),
                                u8"I don't have business with criminals."s);
                            tc = tc * 1 + 0;
                            ELONA_APPEND_RESPONSE(0, i18n::_(u8"ui", u8"more"));
                            chatesc = 1;
                            ELONA_TALK_SCENE_CUT();
                            return talk_result_t::talk_npc;
                        }
                    }
                }
            }
        }
    }
    if (chatval == 1)
    {
        buff = "";
        return talk_result_t::talk_npc;
    }
    if (chatval == 10)
    {
        invctrl = 11;
        invfile = cdata[tc].shop_store_id;
        shop_sell_item();
        screenupdate = -1;
        update_screen();
        cs = 0;
        buff = "";
        return talk_result_t::talk_npc;
    }
    if (chatval == 11)
    {
        invctrl = 12;
        invfile = cdata[tc].shop_store_id;
        shop_sell_item();
        cc = 0;
        screenupdate = -1;
        update_screen();
        cs = 0;
        buff = "";
        return talk_result_t::talk_npc;
    }
    if (chatval == 12)
    {
        return talk_invest();
    }
    if (chatval == 13)
    {
        if (cdata[0].gold < calcmealvalue())
        {
            buff = i18n::_(u8"ui", u8"no_gold");
            return talk_result_t::talk_npc;
        }
        if (cdata[0].nutrition >= 15000)
        {
            buff = lang(
                u8"腹が減っているようにはみえない"s + _yo(),
                u8"You don't seem that hungry."s);
            return talk_result_t::talk_npc;
        }
        snd(12);
        cdata[0].gold -= calcmealvalue();
        snd(18);
        cdata[0].nutrition = 15000;
        buff = lang(_dozo(), u8"Here you are."s);
        txt(lang(u8"なかなか美味しかった。"s, u8"It was tasty."s),
            lang(u8"悪くない。"s, u8"Not bad at all."s),
            lang(u8"あなたは舌鼓をうった。"s, u8"You smack your lips."s));
        show_eating_message();
        chara_anorexia(0);
        return talk_result_t::talk_npc;
    }
    if (chatval >= 14 && chatval < 17)
    {
        if (cdata[0].gold < calcidentifyvalue(chatval - 14))
        {
            buff = i18n::_(u8"ui", u8"no_gold");
            return talk_result_t::talk_npc;
        }
        p = 0;
        for (const auto& cnt : items(0))
        {
            if (inv[cnt].number == 0)
            {
                continue;
            }
            if (inv[cnt].identification_state
                != identification_state_t::completely_identified)
            {
                ++p;
            }
        }
        if (p == 0)
        {
            buff = lang(
                u8"鑑定するアイテムはないみたい"s + _da(),
                u8"Your items have already been identified."s);
            return talk_result_t::talk_npc;
        }
        if (chatval == 15)
        {
            cdata[0].gold -= calcidentifyvalue(1);
            p(0) = 0;
            p(1) = 0;
            p(0) = 0;
            p(1) = 0;
            for (const auto& cnt : items(0))
            {
                if (inv[cnt].number == 0)
                {
                    continue;
                }
                if (inv[cnt].identification_state
                    != identification_state_t::completely_identified)
                {
                    const auto result = item_identify(inv[cnt], 250);
                    item_stack(0, cnt, 1);
                    ++p(1);
                    if (result >= identification_state_t::completely_identified)
                    {
                        ++p;
                    }
                }
            }
            txt(lang(
                ""s + p(1) + u8"個の未判明のアイテムのうち、"s + p
                    + u8"個のアイテムが完全に判明した。"s,
                ""s + p + u8" out of "s + p(1)
                    + u8" unknown items are fully identified."s));
            buff = lang(
                u8"鑑定結果はこの通り"s + _da(),
                u8"Here, I have finished identifing your stuff."s);
        }
        else
        {
            if (chatval == 14)
            {
                efp = 250;
            }
            else
            {
                efp = 1000;
            }
            efid = 411;
            magic();
            if (efcancel == 1)
            {
                buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
                return talk_result_t::talk_npc;
            }
            if (idtresult == identification_state_t::completely_identified)
            {
                buff = lang(
                    u8"鑑定結果はこの通り"s + _da(),
                    u8"Here, I have finished identifing your stuff."s);
            }
            else
            {
                buff = lang(
                    u8"さらなる知識を求めるのなら、調査する必要が"s + _aru(),
                    u8"You need to investigate it to gain more knowledge."s);
            }
            cdata[0].gold -= calcidentifyvalue(chatval - 14);
        }
        snd(12);
        return talk_result_t::talk_npc;
    }
    if (chatval == 17)
    {
        csctrl = 2;
        return talk_trainer();
    }
    if (chatval == 18)
    {
        list_adventurers();
        buff = lang(u8"お目当ての情報は見つかった"s + _kana(), u8"Done?"s);
        return talk_result_t::talk_npc;
    }
    if (chatval == 19)
    {
        if (cdata[0].gold < calcrestorecost())
        {
            buff = i18n::_(u8"ui", u8"no_gold");
            return talk_result_t::talk_npc;
        }
        snd(12);
        cdata[0].gold -= calcrestorecost();
        tcbk = tc;
        for (int cnt = 0; cnt < 16; ++cnt)
        {
            if (cdata[cnt].state != 1)
            {
                continue;
            }
            tc = cnt;
            efid = 439;
            efp = 100;
            magic();
            efid = 440;
            efp = 100;
            magic();
        }
        tc = tcbk;
        talk_start();
        buff = lang(u8"治療が完了し"s + _ta(), u8"Done treatment. Take care!"s);
        return talk_result_t::talk_npc;
    }
    if (chatval == 20)
    {
        invsubroutine = 1;
        for (const auto& cnt : items(tc))
        {
            if (inv[cnt].number != 0)
            {
                inv[cnt].identification_state =
                    identification_state_t::completely_identified;
            }
        }
        invctrl(0) = 20;
        invctrl(1) = 0;
        menu_result result = ctrl_inventory();
        if (!result.succeeded)
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
            return talk_result_t::talk_npc;
        }
        buff = lang(_thanks(2), u8"Thanks!"s);
        return talk_result_t::talk_npc;
    }
    if (chatval == 21 || chatval == 22)
    {
        if (gdata_mount != 0)
        {
            int stat =
                cell_findspace(cdata[0].position.x, cdata[0].position.y, 1);
            if (stat == 0)
            {
                txt(lang(
                    u8"降りるスペースがない。"s,
                    u8"There's no place to get off."s));
                return talk_result_t::talk_end;
            }
            cell_setchara(gdata_mount, rtval, rtval(1));
            txt(lang(
                name(gdata_mount) + u8"から降りた。"s,
                u8"You dismount from "s + name(gdata_mount) + u8"."s));
            ride_end();
        }
        gdata(74) = calcfame(
            0,
            (220 - gdata(120) / 50)
                    * (100 + clamp(adata(22, gdata_current_map), 0, 50)) / 100
                + 2);
        listmax = 0;
        randomize(adata(26, gdata_current_map));
        if (chatval == 21)
        {
            if (adata(26, gdata_current_map) > gdata_hour + gdata_day * 24
                    + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12)
            {
                buff = lang(
                    u8"残念だが、今日の試合はもう終了し"s + _ta(),
                    u8"The game is over today. Come again tomorrow."s);
                return talk_result_t::talk_npc;
            }
            randomize(adata(24, gdata_current_map));
            exrand_randomize(adata(24, gdata_current_map));
            for (int cnt = 0; cnt < 50; ++cnt)
            {
                arenaop(0) = 0;
                arenaop(1) = (100 - gdata(120) / 100) / 3 + 1;
                arenaop(2) = 3;
                if (gdata(120) / 100 < 30)
                {
                    arenaop(2) = 4;
                }
                if (gdata(120) / 100 < 10)
                {
                    arenaop(2) = 5;
                }
                minlevel = arenaop(1) / 3 * 2;
                flt(arenaop(1));
                fixlv = arenaop(2);
                chara_create(56, 0, -3, 0);
                if (cmshade)
                {
                    continue;
                }
                if (cdata[rc].level < minlevel)
                {
                    continue;
                }
                if (cdata[rc].original_relationship != -3)
                {
                    continue;
                }
                break;
            }
            arenaop(1) = cdata[rc].id;
            buff = lang(
                u8"今日の対戦相手は"s + chara_refstr(cdata[rc].id, 2) + _da()
                    + u8"挑戦する"s + _noka(1),
                u8"You got "s + cdatan(0, rc) + u8" today. What'ya say?"s);
        }
        else
        {
            if (adata(27, gdata_current_map) > gdata_hour + gdata_day * 24
                    + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12)
            {
                buff = lang(
                    u8"残念だが、今日の試合はもう終了し"s + _ta(),
                    u8"The game is over today. Come again tomorrow."s);
                return talk_result_t::talk_npc;
            }
            arenaop(0) = 1;
            arenaop(1) = (100 - gdata(120) / 100) / 2 + 1;
            buff = lang(
                u8"対戦相手はレベル"s + arenaop(1) + u8"以下の相手複数"s + _da()
                    + u8"挑戦する"s + _noka(1),
                u8"Your play is a group of monster around level "s + arenaop(1)
                    + u8". Sounds easy huh?"s);
        }
        ELONA_APPEND_RESPONSE(1, lang(u8"挑戦する"s, u8"Alright."s));
        ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"I'll pass."s));
        chatesc = 1;
        talk_window_query();
        if (chatval != 1)
        {
            buff = lang(
                u8"用があるときは声をかけて"s + _kure(),
                u8"Alright. Call me if you changed your mind."s);
            return talk_result_t::talk_npc;
        }
        if (arenaop == 0)
        {
            adata(26, gdata_current_map) = gdata_hour + gdata_day * 24
                + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12 + 24;
        }
        if (arenaop == 1)
        {
            adata(27, gdata_current_map) = gdata_hour + gdata_day * 24
                + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12 + 24;
        }
        gdata_executing_immediate_quest_type = 1;
        gdata(71) = 1;
        gdata_executing_immediate_quest = 0;
        gdata(73) = 1;
        gdata_previous_map2 = gdata_current_map;
        gdata_previous_dungeon_level = gdata_current_dungeon_level;
        gdata_previous_x = cdata[0].position.x;
        gdata_previous_y = cdata[0].position.y;
        gdata_destination_map = 6;
        gdata_destination_dungeon_level = 1;
        levelexitby = 2;
        chatteleport = 1;
        return talk_result_t::talk_end;
    }
    if (chatval == 40 || chatval == 41)
    {
        gdata(74) = calcfame(
            0,
            (220 - gdata(121) / 50)
                    * (50 + clamp(adata(23, gdata_current_map), 0, 50)) / 100
                + 2);
        listmax = 0;
        if (chatval == 40)
        {
            arenaop(0) = 0;
            arenaop(1) = 1;
            arenaop(2) = (100 - gdata(121) / 100) / 3 * 2 + 3;
            buff = lang(
                u8"一対一の戦いで、対戦相手はレベル"s + arenaop(2)
                    + u8"ぐらいの相手"s + _da() + u8"挑戦する"s + _noka(1),
                u8"The opponent is around level "s + arenaop(2)
                    + u8". Want to give it a try? "s);
        }
        if (chatval == 41)
        {
            arenaop(0) = 1;
            arenaop(1) = 2;
            arenaop(2) = (100 - gdata(121) / 100) / 2 + 1;
            arenaop(1) = rnd(7) + 2;
            buff = lang(""s + arenaop(1) +
                    u8"人同士のチームバトルで、対戦相手はレベル"s + arenaop(2) +
                    u8"以下の相手複数"s + _da() + u8"挑戦する"s + _noka(1),
                u8"It's a "s + arenaop(1) + u8" vs "s + arenaop(1) +
                    u8" match. The opponent's group is formed by the pets less than "s +
                    arenaop(2) + u8" levels. What you say?"s);
        }
        ELONA_APPEND_RESPONSE(1, lang(u8"挑戦する"s, u8"I'll send my pet."s));
        ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"I'll pass."s));
        chatesc = 1;
        talk_window_query();
        if (chatval != 1)
        {
            buff = lang(
                u8"用があるときは声をかけて"s + _kure(),
                u8"Alright. Call me if you changed your mind."s);
            return talk_result_t::talk_npc;
        }
        DIM2(followerexist, 16);
        for (int cnt = 0; cnt < 16; ++cnt)
        {
            followerexist(cnt) = cdata[cnt].state;
        }
        allyctrl = 2;
        int stat = ctrl_ally();
        if (stat == -1)
        {
            buff = lang(
                u8"用があるときは声をかけて"s + _kure(),
                u8"Alright. Call me if you changed your mind."s);
            return talk_result_t::talk_npc;
        }
        gdata_executing_immediate_quest_type = 2;
        gdata(71) = 0;
        gdata_executing_immediate_quest = 0;
        gdata(73) = 1;
        gdata_previous_map2 = gdata_current_map;
        gdata_previous_dungeon_level = gdata_current_dungeon_level;
        gdata_previous_x = cdata[0].position.x;
        gdata_previous_y = cdata[0].position.y;
        gdata_destination_map = 40;
        gdata_destination_dungeon_level = 1;
        levelexitby = 2;
        chatteleport = 1;
        return talk_result_t::talk_end;
    }
    if (chatval == 42)
    {
        buff = lang(u8"現在は"s + adata(23, gdata_current_map) + u8"連勝中"s + _da() +
                u8"5連勝,20連勝毎にボーナスを与え"s + _ru(),
            u8"Your winning streak has reached "s + adata(23, gdata_current_map) +
                u8" matches now. Keep the audience excited. You get nice bonus at every 5th and 20th wins in a row."s);
        return talk_result_t::talk_npc;
    }
    if (chatval == 23)
    {
        buff = lang(u8"現在は"s + adata(22, gdata_current_map) + u8"連勝中"s + _da() +
                u8"5連勝,20連勝毎にボーナスを与え"s + _ru(),
            u8"Your winning streak has reached "s + adata(22, gdata_current_map) +
                u8" matches now. Keep the audience excited. You get nice bonus at every 5th and 20th wins in a row."s);
        return talk_result_t::talk_npc;
    }
    if (chatval == 24)
    {
        return talk_result_t::talk_quest_giver;
    }
    if (chatval == 25)
    {
        int stat = inv_getfreeid_force();
        ti = stat;
        item_copy(deliver(1), ti);
        inv[ti].number = 1;
        ci = ti;
        rc = tc;
        chara_set_item_which_will_be_used();
        rq = deliver;
        --inv[deliver(1)].number;
        txt(lang(
            itemname(deliver(1), 1) + u8"を手渡した。"s,
            u8"You hand over "s + itemname(deliver(1), 1) + u8"."s));
        quest_set_data(3);
        quest_complete();
        refresh_burden_state();
        return talk_result_t::talk_npc;
    }
    if (chatval == 26)
    {
        int stat = inv_getfreeid_force();
        ti = stat;
        item_copy(supply, ti);
        inv[ti].number = 1;
        cdata[tc].was_passed_item_by_you_just_now() = true;
        ci = ti;
        rc = tc;
        chara_set_item_which_will_be_used();
        --inv[supply].number;
        txt(lang(
            itemname(supply, 1) + u8"を手渡した。"s,
            u8"You hand over "s + itemname(supply, 1) + u8"."s));
        quest_set_data(3);
        quest_complete();
        refresh_burden_state();
        return talk_result_t::talk_npc;
    }
    if (chatval == 30)
    {
        csctrl = 3;
        return talk_trainer();
    }
    if (chatval == 31)
    {
        listmax = 0;
        buff = lang(
            _rob(2),
            u8"Oh crap. Another bandit trying to spoil my business! Form up, mercenaries."s);
        ELONA_APPEND_RESPONSE(1, lang(u8"神に祈れ"s, u8"Pray to your God."s));
        ELONA_APPEND_RESPONSE(
            0, lang(u8"いや、冗談です"s, u8"W-Wait! I was just kidding."s));
        chatesc = 1;
        talk_window_query();
        if (chatval != 1)
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
            return talk_result_t::talk_npc;
        }
        go_hostile();
        return talk_result_t::talk_end;
    }
    if (chatval == 32)
    {
        listmax = 0;
        p = itemfind(0, 284);
        if (p == -1)
        {
            p = itemfind(0, 283);
        }
        --inv[p].number;
        if (inv[p].param1 == 0)
        {
            buff = lang(u8"む…中身が空っぽ"s + _dana(2), u8"Hmm! It's empty!"s);
            ELONA_APPEND_RESPONSE(0, lang(u8"しまった…"s, u8"Oops...!"s));
            chatesc = 1;
            talk_window_query();
            modify_karma(0, -5);
        }
        else
        {
            buff = lang(
                u8"わざわざ落し物を届けてくれた"s + _noka() + _kimi(3)
                    + u8"は市民の模範"s + _da() + _thanks(),
                u8"How nice of you to take the trouble to bring it. You're a model citizen indeed!"s);
            ELONA_APPEND_RESPONSE(
                0, lang(u8"当然のことだ"s, u8"It's nothing."s));
            chatesc = 1;
            talk_window_query();
            modify_karma(0, 5);
            ++gdata_lost_wallet_count;
            if (gdata_lost_wallet_count >= 4)
            {
                listmax = 0;
                buff = lang(
                    u8"む、また"s + _kimi(3) + _ka()
                        + u8"随分と頻繁に財布を見つけられるもの"s + _dana(),
                    u8"Oh, it's you again? How come you find the wallets so often?"s);
                tc = tc * 1 + 0;
                ELONA_APPEND_RESPONSE(0, i18n::_(u8"ui", u8"more"));
                chatesc = 1;
                ELONA_TALK_SCENE_CUT();
                buff = lang(u8"（…あやしい）"s, u8"(...suspicious)"s);
                ELONA_APPEND_RESPONSE(
                    0,
                    lang(u8"ぎくっ"s, u8"I really found it on the street!"s));
                chatesc = 1;
                talk_window_query();
                modify_karma(0, -10);
            }
        }
        refresh_burden_state();
        buff = "";
        return talk_result_t::talk_npc;
    }
    if (chatval == 33)
    {
        allyctrl = 0;
        int stat = ctrl_ally();
        if (stat != -1)
        {
            rc = stat;
            if (cdata[rc].state == 1)
            {
                buff = lang(
                    u8"そいつは呼び戻す必要はないよう"s + _da(),
                    u8"Huh? You don't need to do that."s);
                return talk_result_t::talk_npc;
            }
            listmax = 0;
            buff = lang(
                u8"そいつを呼び戻すには、"s + calcresurrectvalue(rc)
                    + i18n::_(u8"ui", u8"gold") + u8"必要"s + _da(),
                u8"Alright. We had taken good care of your pet. It will cost you "s
                    + calcresurrectvalue(rc) + u8" gold pieces."s);
            if (cdata[0].gold >= calcresurrectvalue(rc))
            {
                ELONA_APPEND_RESPONSE(1, lang(u8"呼び戻す"s, u8"I'll pay."s));
            }
            ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"Never mind."s));
            chatesc = 1;
            talk_window_query();
            if (chatval == 1)
            {
                snd(12);
                cdata[0].gold -= calcresurrectvalue(rc);
                buff = lang(
                    u8"(バーテンが店の奥から"s + name(rc) + u8"を連れてきた)"s
                        + _dozo(),
                    u8"("s + name(tc) + u8" brings "s + name(rc)
                        + u8" from the stable.) There you go."s);
                revive_character();
            }
            else
            {
                buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
            }
        }
        else
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
        }
        return talk_result_t::talk_npc;
    }
    if (chatval == 34)
    {
        listmax = 0;
        buff = lang(
            u8"(あなたは、"s + name(tc) + u8"に街で待っているように指示した)"s,
            u8"(You order "s + name(tc) + u8" to wait at the town.)"s);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::_(u8"ui", u8"more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        map(cdata[tc].position.x, cdata[tc].position.y, 1) = 0;
        cdata[tc].state = 7;
        cdata[tc].current_map = 0;
        return talk_result_t::talk_end;
    }
    if (chatval == 35)
    {
        listmax = 0;
        buff = lang(
            u8"("s + name(tc)
                + u8"は悲しそうな目であなたを見ている。本当に縁を切る？)"s,
            u8"("s + name(tc) + u8" looks at you sadly. Really abandon "s
                + him(tc) + u8"? )"s);
        ELONA_APPEND_RESPONSE(1, lang(u8"切る"s, u8"Yes."s));
        ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"No."s));
        chatesc = 1;
        talk_window_query();
        if (chatval == 1)
        {
            txt(lang(
                ""s + name(tc) + u8"と別れた…"s,
                u8"You abandoned "s + name(tc) + u8"..."s));
            map(cdata[tc].position.x, cdata[tc].position.y, 1) = 0;
            chara_delete(tc);
            return talk_result_t::talk_end;
        }
        buff = "";
        return talk_result_t::talk_npc;
    }
    if (chatval == 36 || chatval == 57)
    {
        for (int cnt = 0; cnt < 10; ++cnt)
        {
            flt(cdata[0].level / 2 + 5);
            fixlv = 2;
            if (chatval == 36)
            {
                fltn(u8"man"s);
            }
            else
            {
                fltn(u8"horse"s);
            }
            chara_create(56, 0, -3, 0);
            if (cdata[56].level == 0)
            {
                chara_vanquish(56);
                continue;
            }
            break;
        }
        listmax = 0;
        buff = lang(
            u8"そう"s + _dana() + ""s + cdatan(0, 56) + u8"を"s
                + calcslavevalue(56) + i18n::_(u8"ui", u8"gold") + u8"でどう"s
                + _da(1),
            u8"Okay. Let me check the stable....How about "s
                + cnven(cdatan(0, 56)) + u8" for "s + calcslavevalue(56)
                + u8" gold pieces. I'd say it's quite a bargain!"s);
        if (cdata[0].gold >= calcslavevalue(56))
        {
            ELONA_APPEND_RESPONSE(1, lang(u8"買い取る"s, u8"I'll pay."s));
        }
        ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"Never mind."s));
        chatesc = 1;
        talk_window_query();
        if (chatval == 1)
        {
            txt(lang(
                cdatan(0, 56) + u8"を買い取った。"s,
                u8"You buy "s + cnven(cdatan(0, 56)) + u8"."s));
            snd(12);
            cdata[0].gold -= calcslavevalue(56);
            rc = 56;
            new_ally_joins();
            buff = lang(_thanks(2), u8"Thanks!"s);
        }
        else
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
        }
        return talk_result_t::talk_npc;
    }
    if (chatval == 37)
    {
        allyctrl = 1;
        int stat = ctrl_ally();
        if (stat != -1)
        {
            rc = stat;
            listmax = 0;
            buff = lang(
                u8"なかなかの身体つき"s + _dana() + calcslavevalue(rc) * 2 / 3
                    + i18n::_(u8"ui", u8"gold") + u8"でどう"s + _da(1),
                u8"Let me see....Hmmm, this one got a nice figure. I'll give you "s
                    + calcslavevalue(rc) * 2 / 3 + u8" gold pieces."s);
            ELONA_APPEND_RESPONSE(1, lang(u8"売る"s, u8"Deal."s));
            ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"No way."s));
            chatesc = 1;
            talk_window_query();
            if (chatval == 1)
            {
                txt(lang(
                    cdatan(0, rc) + u8"を売り飛ばした。"s,
                    u8"You sell off "s + cnven(cdatan(0, rc)) + u8"."s));
                snd(11);
                cdata[0].gold += calcslavevalue(rc) * 2 / 3;
                if (cdata[rc].state == 1)
                {
                    map(cdata[rc].position.x, cdata[rc].position.y, 1) = 0;
                }
                if (cdata[rc].is_escorted() == 1)
                {
                    event_add(15, cdata[rc].id);
                }
                chara_delete(rc);
                buff = lang(_thanks(2), u8"Thanks!"s);
            }
            else
            {
                buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
            }
        }
        else
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
        }
        return talk_result_t::talk_npc;
    }
    if (chatval == 38)
    {
        if (cdata[tc].impression < 200)
        {
            buff = lang(
                u8"("s + name(tc) + u8"はやんわりと断った)"s,
                u8"("s + name(tc) + u8" gently refuses your proposal. )"s);
            return talk_result_t::talk_npc;
        }
        cdata[tc].is_married() = true;
        listmax = 0;
        buff = lang(u8"はい…喜んで。"s, u8"With preasure."s);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::_(u8"ui", u8"more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        marry = tc;
        event_add(13);
        return talk_result_t::talk_end;
    }
    if (chatval == 39)
    {
        if (gdata_current_map == 30)
        {
            listmax = 0;
            buff = lang(u8"こんな場所では嫌よ"s, u8"Not here!"s);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::_(u8"ui", u8"more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            return talk_result_t::talk_end;
        }
        listmax = 0;
        buff = lang(u8"いやん、あなたったら…"s, u8"*blush*"s);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::_(u8"ui", u8"more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        cdata[tc].has_made_gene() = true;
        if (gdata_wizard == 0)
        {
            gdata(98) = tc;
        }
        return talk_result_t::talk_end;
    }
    if (chatval == 43)
    {
        listmax = 0;
        buff = lang(
            u8"悪天候時はシェルターを無料で開放している"s + _nda()
                + u8"すみやかに避難して"s + _kure(),
            u8"The shelter is free to use for anyone. Here, come in."s);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::_(u8"ui", u8"more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        gdata_previous_map2 = gdata_current_map;
        gdata_previous_dungeon_level = gdata_current_dungeon_level;
        gdata_previous_x = cdata[0].position.x;
        gdata_previous_y = cdata[0].position.y;
        gdata_destination_map = 30;
        gdata_destination_dungeon_level = 1;
        levelexitby = 2;
        chatteleport = 1;
        snd(49);
        return talk_result_t::talk_end;
    }
    if (chatval == 44)
    {
        listmax = 0;
        buff = lang(
            u8"("s + name(tc)
                + u8"は悲しそうな目であなたを見ている。本当に縁を切る？)"s,
            u8"("s + name(tc) + u8" looks at you sadly. Really dismiss "s
                + him(tc) + u8"? )"s);
        ELONA_APPEND_RESPONSE(1, lang(u8"切る"s, u8"Yes."s));
        ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"No."s));
        chatesc = 1;
        talk_window_query();
        if (chatval == 1)
        {
            txt(lang(
                ""s + name(tc) + u8"を解雇した… "s,
                u8"You dismiss "s + name(tc) + u8"."s));
            chara_vanquish(tc);
            calccosthire();
            return talk_result_t::talk_end;
        }
        buff = "";
        return talk_result_t::talk_npc;
    }
    if (chatval == 45)
    {
        mdatan(0) = random_title();
        if (jp)
        {
            s(0) = u8"の家"s;
            s(1) = u8"邸"s;
            s(2) = u8"城"s;
            s(3) = u8"ハーレム"s;
            s(4) = u8"の巣窟"s;
            s(5) = u8"ハウス"s;
            s(6) = u8"ホーム"s;
            s(7) = u8"の住処"s;
            s(8) = u8"宅"s;
            s(9) = u8"の隠れ家"s;
            s(10) = u8"ドーム"s;
            if (rnd(5))
            {
                mdatan(0) += s(rnd(10));
            }
        }
        else
        {
            s(0) = u8"Home"s;
            s(1) = u8"Mansion"s;
            s(2) = u8"Shack"s;
            s(3) = u8"Nest"s;
            s(4) = u8"Base"s;
            s(5) = u8"Hideout"s;
            s(6) = u8"Dome"s;
            s(7) = u8"Hut"s;
            s(8) = u8"Cabin"s;
            s(9) = u8"Hovel"s;
            s(10) = u8"Shed"s;
            if (rnd(3))
            {
                mdatan(0) += u8" "s + s(rnd(10));
            }
        }
        screenupdate = -1;
        update_entire_screen();
        buff = lang(
            u8"そう"s + _dana() + u8"これからこの家の名前は"s + mdatan(0)
                + _da(),
            u8"Hey, I've come up a good idea! \""s + mdatan(0)
                + u8"\", doesn't it sound so charming?"s);
        return talk_result_t::talk_npc;
    }
    if (chatval == 46)
    {
        if (cdata[0].karma >= -30)
        {
            buff = lang(
                u8"その程度の罪なら自分でなんとかしなさい。"s,
                u8"You karma isn't that low. Come back after you have committed more crimes!"s);
            return talk_result_t::talk_npc;
        }
        listmax = 0;
        buff = lang(
            u8"免罪符を希望する"s + _noka(1) + calcguiltvalue()
                + i18n::_(u8"ui", u8"gold") + u8"かかるけどいいの"s + _kana(1),
            u8"In the authority of all the saints, I will grant you an indulgence, for money of course. The price is "s
                + calcguiltvalue() + u8" gold pieces."s);
        if (cdata[0].gold >= calcguiltvalue())
        {
            ELONA_APPEND_RESPONSE(1, lang(u8"買う"s, u8"Deal."s));
        }
        ELONA_APPEND_RESPONSE(
            0, lang(u8"やめる"s, u8"The price is too high."s));
        chatesc = 1;
        talk_window_query();
        if (chatval == 1)
        {
            snd(12);
            cdata[0].gold -= calcguiltvalue();
            modify_karma(0, (cdata[0].karma - -30) * -1 + 1);
            buff = lang(_thanks(2), u8"Thanks!"s);
        }
        else
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
        }
        return talk_result_t::talk_npc;
    }
    if (chatval == 47)
    {
        tcchat = tc;
        allyctrl = 4;
        int stat = ctrl_ally();
        if (stat != -1)
        {
            rc = stat;
            listmax = 0;
            buff = lang(
                u8"10000 goldかかるけどいい"s + _ka(1),
                u8"10000 gold pieces."s);
            if (cdata[0].gold >= 10000)
            {
                ELONA_APPEND_RESPONSE(
                    1, lang(u8"調査する"s, u8"Too expensive, but okay."s));
            }
            ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"No way!"s));
            chatesc = 1;
            talk_window_query();
            if (chatval == 1)
            {
                snd(12);
                cdata[0].gold -= 10000;
                cc = rc;
                csctrl = 4;
                snd(26);
                menu_character_sheet();
                cc = 0;
                talk_start();
                buff = "";
            }
            else
            {
                buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
            }
        }
        else
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
        }
        tc = tcchat;
        return talk_result_t::talk_npc;
    }
    if (chatval == 48)
    {
        if (cdata[tc].is_silent() == 0)
        {
            cdata[tc].is_silent() = true;
            buff = u8"("s
                + lang((name(tc) + u8"はしゅんとなった…"s),
                       (name(tc) + u8" stops talking..."s))
                + u8")"s;
        }
        else
        {
            buff = u8"("s
                + lang((name(tc) + u8"はあなたに抱きついた"s),
                       (name(tc) + u8" hugs you."s))
                + u8")"s;
            cdata[tc].is_silent() = false;
        }
        return talk_result_t::talk_npc;
    }
    if (chatval == 50)
    {
        buff = lang(
            _ore(3) + u8"の剣が必要な"s + _noka(1) + u8"そう"s + _dana(3)
                + u8"、"s + calchireadv(tc) + i18n::_(u8"ui", u8"gold")
                + u8"払うならば、7日間護衛を引き受け"s + _ru(),
            u8"I will take the job for "s + calchireadv(tc)
                + u8" gold pieces, for seven day."s);
        if (cdata[0].gold >= calchireadv(tc))
        {
            ELONA_APPEND_RESPONSE(1, lang(u8"頼む"s, u8"Sounds fair enough."s));
        }
        ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"Some other time."s));
        chatesc = 1;
        talk_window_query();
        if (chatval == 1)
        {
            snd(12);
            cdata[0].gold -= calchireadv(tc);
            cdata[tc].relationship = 10;
            cdata[tc].is_contracting() = true;
            cdata[tc].period_of_contract = gdata_hour + gdata_day * 24
                + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12 + 168;
            ++cdata[tc].hire_count;
            snd(64);
            txtef(5);
            txt(lang(
                name(tc) + u8"を雇った。"s,
                u8"You hired "s + name(tc) + u8"."s));
            buff = lang(_thanks(2), u8"Thanks!"s);
        }
        else
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
        }
        return talk_result_t::talk_npc;
    }
    if (chatval == 51)
    {
        if (cdata[0].level * 3 / 2 + 10 < cdata[tc].level)
        {
            buff = lang(
                _kimi(3) + u8"の仲間になれと？あまりにも力の差がありすぎる"s
                    + _na(),
                u8"Huh? You are no match for me."s);
            return talk_result_t::talk_npc;
        }
        if (cdata[tc].impression >= 200 && cdata[tc].hire_count > 2)
        {
            listmax = 0;
            buff = lang(
                _kimi(3) + u8"となら上手くやっていけそう"s + _da() + _yoro(2),
                u8"Sure, I guess you and I can make a good team."s);
            tc = tc * 1 + 0;
            ELONA_APPEND_RESPONSE(0, i18n::_(u8"ui", u8"more"));
            chatesc = 1;
            ELONA_TALK_SCENE_CUT();
            f = chara_get_free_slot_ally();
            if (f == 0)
            {
                buff = lang(
                    u8"これ以上仲間を連れて行けないよう"s + _da()
                        + u8"人数を調整してまた来て"s + _kure(),
                    u8"It seems your party is already full. Come see me again when you're ready."s);
                return talk_result_t::talk_npc;
            }
            rc = tc;
            new_ally_joins();
            cdata[rc].character_role = 0;
            cdata[rc].current_map = 0;
            cdata[tc].impression = 100;
            rc = oc;
            create_adventurer();
            return talk_result_t::talk_end;
        }
        buff = lang(
            _kimi(3) + u8"の仲間になれと？悪い"s + _ga(3) + u8"お断り"s + _da(),
            u8"Huh? What made you think I'd want to join you? I don't even know you well."s);
        return talk_result_t::talk_npc;
    }
    if (chatval == 52)
    {
        listmax = 0;
        buff = lang(
            u8"ほほう、モンスターの顔をした人間か。見世物としてなかなかいけそうだ。金貨50000枚で買い取ろう。"s,
            u8"Look what we have! A woman who got a monster's face. It'll be a good show. Wanna sell me for 50000 gold coins?"s);
        ELONA_APPEND_RESPONSE(1, lang(u8"売る"s, u8"Sure, take her."s));
        ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"You cold bastard."s));
        chatesc = 1;
        talk_window_query();
        if (chatval == 1)
        {
            txt(lang(u8"パエルの母親を売った…"s, u8"You sell Pael's mom..."s));
            modify_karma(0, -20);
            snd(11);
            cdata[0].gold += 50000;
            gdata_pael_and_her_mom = 1002;
            rc = chara_find(222);
            cdata[rc].ai_calm = 3;
            cdata[rc].relationship = 0;
            cdata[rc].initial_position.x = 48;
            cdata[rc].initial_position.y = 18;
            cell_movechara(rc, 48, 18);
            buff = lang(_thanks(2), u8"Thanks!"s);
        }
        else
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
        }
        return talk_result_t::talk_npc;
    }
    if (chatval == 53)
    {
        listmax = 0;
        buff = lang(
            u8"ここからふもとに下りるのは不便だから、ボランティアで帰還サービスをやってい"s
                + _ru() + _kimi(3) + u8"も帰還サービスを希望"s + _kana(),
            u8"I'm practicing a spell of return. Would you like to take my service?"s);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::_(u8"ui", u8"more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        try_to_return();
        return talk_result_t::talk_end;
    }
    if (chatval == 54)
    {
        if (calccostreload(0) == 0)
        {
            buff = lang(
                u8"充填する必要はないみたい"s + _da(),
                u8"Reload what? You don't have any ammo in your inventory."s);
            return talk_result_t::talk_npc;
        }
        buff = lang(
            u8"そう"s + _dana(3) + u8"、全ての矢弾を補充すると"s
                + calccostreload(0) + i18n::_(u8"ui", u8"gold") + _da(),
            u8"Sure, let me check what type of ammos you need....Okay, reloading all of your ammos will cost "s
                + calccostreload(0) + u8" gold pieces."s);
        if (cdata[0].gold >= calccostreload(0))
        {
            ELONA_APPEND_RESPONSE(1, lang(u8"頼む"s, u8"Alright."s));
        }
        ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"Another time."s));
        chatesc = 1;
        talk_window_query();
        if (chatval == 1)
        {
            snd(12);
            cdata[0].gold -= calccostreload(0);
            p = calccostreload(0, true);
            buff = lang(_thanks(2), u8"Thanks!"s);
        }
        else
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
        }
        return talk_result_t::talk_npc;
    }
    if (chatval == 55)
    {
        screenupdate = -1;
        update_screen();
        invctrl = 0;
        show_spell_writer_menu();
        buff = "";
        return talk_result_t::talk_npc;
    }
    if (chatval == 56)
    {
        ELONA_APPEND_RESPONSE(1, lang(u8"はじめる"s, u8"Let's do it."s));
        ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"W-wai.t.."s));
        buff = lang(
            u8"なかなかの体つき"s + _dana() + u8"よし、買"s + _u(2),
            u8"You are...quite attractive. I'll buy you."s);
        talk_window_query();
        if (chatval != 1)
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
            return talk_result_t::talk_npc;
        }
        listmax = 0;
        buff = lang(u8"いく"s + _yo(2), u8"Okay, no turning back now!"s);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, lang(u8"うふふ"s, u8"Come on!"s));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        continuous_action_sex();
        return talk_result_t::talk_end;
    }
    if (chatval == 58)
    {
        if (gdata_left_turns_of_timestop == 0)
        {
            event_add(25);
        }
        return talk_result_t::talk_end;
    }
    if (chatval == 59)
    {
        --gdata_number_of_waiting_guests;
        listmax = 0;
        buff = lang(u8"追い返す"s + _yo(), u8"Alright."s);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, i18n::_(u8"ui", u8"more"));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        buff = "";
        return talk_result_t::talk_npc;
    }
    if (chatval == 60)
    {
        sexvalue = sdata(17, tc) * 25 + 100 + cdata[0].fame / 10;
        if (cdata[0].gold >= sexvalue)
        {
            ELONA_APPEND_RESPONSE(1, lang(u8"はじめる"s, u8"Let's do it."s));
        }
        ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"W-wai.t.."s));
        buff = lang(
            u8"そう"s + _dana() + u8"金貨"s + sexvalue + u8"枚を前払いで"s
                + _kure(),
            u8"Okay sweetie, I need "s + sexvalue
                + u8" gold pieces in front."s);
        talk_window_query();
        if (chatval != 1)
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
            return talk_result_t::talk_npc;
        }
        snd(12);
        cdata[cc].gold -= sexvalue;
        cdata[tc].gold += sexvalue;
        listmax = 0;
        buff = lang(u8"いく"s + _yo(2), u8"Okay, no turning back now!"s);
        tc = tc * 1 + 0;
        ELONA_APPEND_RESPONSE(0, lang(u8"うふふ"s, u8"Come on!"s));
        chatesc = 1;
        ELONA_TALK_SCENE_CUT();
        cc = tc;
        tc = 0;
        continuous_action_sex();
        cc = 0;
        return talk_result_t::talk_end;
    }
    if (chatval == 61)
    {
        if (gdata_current_map == 43)
        {
            p(0) = 45;
            p(1) = 48;
            p(2) = 0;
        }
        if (gdata_current_map == 45)
        {
            p(0) = 43;
            p(1) = 48;
            p(2) = 0;
        }
        if (gdata_current_map == 48)
        {
            p(0) = 43;
            p(1) = 45;
            p(2) = 0;
        }
        for (int cnt = 0; cnt < 10; ++cnt)
        {
            if (p(cnt) == 0)
            {
                ELONA_APPEND_RESPONSE(0, lang(u8"やめる"s, u8"Never mind!"s));
                break;
            }
            ELONA_APPEND_RESPONSE(p(cnt), mapname(p(cnt)));
        }
        buff = lang(u8"つぇｔ"s, u8"tset"s);
        talk_window_query();
        if (chatval <= 0)
        {
            buff = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
            return talk_result_t::talk_npc;
        }
        gdata_destination_map = adata(30, chatval);
        gdata_destination_dungeon_level = 1;
        levelexitby = 4;
        gdata(79) = 1;
        gdata(850) = adata(30, chatval);
        gdata_pc_home_x = adata(1, chatval);
        gdata_pc_home_y = adata(2, chatval);
        fixtransfermap = 1;
        chatteleport = 1;
        return talk_result_t::talk_end;
    }
    if (chatval >= 10000)
    {
        talk_guide_quest_client();
        rc = rtval(chatval - 10000);
        p = direction(
            cdata[0].position.x,
            cdata[0].position.y,
            cdata[rc].position.x,
            cdata[rc].position.y);
        if (p == 1)
        {
            s = lang(u8"西"s, u8"west"s);
        }
        else if (p == 2)
        {
            s = lang(u8"東"s, u8"east"s);
        }
        else if (p == 3)
        {
            s = lang(u8"北"s, u8"north"s);
        }
        else
        {
            s = lang(u8"南"s, u8"south"s);
        }
        p = dist(
            cdata[0].position.x,
            cdata[0].position.y,
            cdata[rc].position.x,
            cdata[rc].position.y);
        for (int cnt = 0; cnt < 1; ++cnt)
        {
            if (rc == tc)
            {
                s = lang(u8"冷やかし"s + _ka(1), u8"You kidding? "s);
                break;
            }
            if (cdata[rc].state != 1)
            {
                s = lang(
                    u8"奴なら今は死んでいる"s + _yo(2),
                    u8"Oh forget it, dead for now."s);
                break;
            }
            if (p < 6)
            {
                s = lang(
                    cdatan(0, rc) + u8"ならすぐ近くにいる"s + _yo() + s
                        + u8"の方を向いてごらん。"s,
                    u8"Oh look carefully before asking, just turn "s + s
                        + u8"."s);
                break;
            }
            if (p < 12)
            {
                s = lang(
                    u8"ちょっと前に"s + s + u8"の方で見かけた"s + _yo(),
                    u8"I saw "s + cdatan(0, rc) + u8" just a minute ago. Try "s
                        + s + u8"."s);
                break;
            }
            if (p < 20)
            {
                s = lang(
                    cdatan(0, rc) + u8"なら"s + s + u8"の方角を探してごらん。"s,
                    u8"Walk to "s + s + u8" for a while, you'll find "s
                        + cdatan(0, rc) + u8"."s);
                break;
            }
            if (p < 35)
            {
                s = lang(
                    cdatan(0, rc) + u8"に会いたいのなら、"s + s
                        + u8"にかなり歩く必要があ"s + _ru(),
                    u8"If you want to meet "s + cdatan(0, rc)
                        + u8", you have to considerably walk to "s + s
                        + u8"."s);
                break;
            }
            s = lang(
                cdatan(0, rc) + _ka(3) + u8"、ここから"s + s
                    + u8"の物凄く離れた場所にいるはず"s + _da(),
                u8"You need to walk long way to "s + s + u8" to meet "s
                    + cdatan(0, rc) + u8"."s);
            break;
        }
        buff = s;
        return talk_result_t::talk_npc;
    }
    if (event_id() == 11)
    {
        levelexitby = 4;
        chatteleport = 1;
        snd(49);
    }
    return talk_result_t::talk_end;
}



} // namespace elona
