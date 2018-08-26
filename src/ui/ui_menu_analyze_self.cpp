#include "ui_menu_analyze_self.hpp"
#include "../ability.hpp"
#include "../audio.hpp"
#include "../character.hpp"
#include "../god.hpp"
#include "../i18n.hpp"

namespace elona
{
namespace ui
{

static void _load_player_info()
{
    buff = "";
    notesel(buff);

    chara_delete(56);

    cdata.tmp().piety_point = cdata.player().piety_point;
    cdata.tmp().god_id = cdata.player().god_id;

    for (int cnt = 0; cnt < 600; ++cnt)
    {
        sdata(cnt, rc) = 1;
    }

    apply_god_blessing(56);

    if (!cdata.player().god_id.empty())
    {
        buff += u8"<title1>◆ "s
            + i18n::_(u8"god", cdata.player().god_id, u8"name")
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
}

bool ui_menu_analyze_self::init()
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

    _load_player_info();

    return true;
}

void ui_menu_analyze_self::update()
{
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
}

static void _draw_window()
{
    s(0) = lang(u8"自己の分析", u8"Analyze Self");
    s(1) = strhint2 + strhint3b;
    display_window((windoww - 400) / 2 + inf_screenx, winposy(448), 400, 448);

    std::string topic = lang(u8"分析結果", u8"Analysis Result");
    display_topic(topic, wx + 28, wy + 36);
    font(14 - en * 2);
}

static void _draw_single_desc(int cnt, const std::string& text)
{
    pos(wx + 54, wy + 66 + cnt * 19 + 2);
    gmes(text, wx, wy + 66 + cnt * 19 + 2, 600, {30, 30, 30}, false);
}

static void _draw_desc()
{
    std::string text;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        int index = pagesize * page + cnt;
        if (index >= listmax)
        {
            break;
        }

        noteget(text, index);
        _draw_single_desc(cnt, text);
    }
}

void ui_menu_analyze_self::draw()
{
    _draw_window();
    _draw_desc();
}

optional<ui_menu_analyze_self::result_type> ui_menu_analyze_self::on_key(
    const std::string& key)
{
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            set_reupdate();
        }
    }
    else if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            set_reupdate();
        }
    }
    else if (key == key_cancel)
    {
        return ui_menu_analyze_self::result::cancel();
    }

    return none;
}

} // namespace ui
} // namespace elona
