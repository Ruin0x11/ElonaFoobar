#include "ui_menu_change_appearance_equip.hpp"
#include "../audio.hpp"
#include "../draw.hpp"
#include "../i18n.hpp"

namespace elona
{
namespace ui
{

static void _load_option_list()
{
    s(0) = i18n::s.get("core.locale.ui.appearance.equipment.done");
    s(1) = i18n::s.get("core.locale.ui.appearance.equipment.chest");
    s(2) = i18n::s.get("core.locale.ui.appearance.equipment.leg");
    s(3) = i18n::s.get("core.locale.ui.appearance.equipment.belt");
    s(4) = i18n::s.get("core.locale.ui.appearance.equipment.glove");
    s(5) = i18n::s.get("core.locale.ui.appearance.equipment.mantle");
    p = 6;
    for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
    {
        list(0, cnt) = cnt;
        listn(0, cnt) = s(cnt);
        ++listmax;
    }
    keyrange = 0;
    for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
    {
        p = cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_enter;
        ++keyrange;
    }
}

bool ui_menu_change_appearance_equip::init()
{
    create_pcpic(cc, true);
    snd(26);
    page = 0;
    pagesize = 18;
    cs = 0;
    cs_bk = -1;
    windowshadow = 1;
    listmax = 0;

    _load_option_list();

    return true;
}

void ui_menu_change_appearance_equip::update()
{
}

static void _draw_window()
{
    pagesize = 0;
    s(0) = i18n::s.get("core.locale.ui.appearance.equipment.title");
    s(1) = i18n::s.get("core.locale.ui.appearance.hint");
    display_window(
        (windoww - 360) / 2 + inf_screenx, winposy(289) - 12, 360, 289);
    s = i18n::s.get("core.locale.ui.appearance.equipment.part");
    pagesize = listmax;
    display_topic(s, wx + 34, wy + 36);
}

static void _draw_own_sprite(int facing_direction)
{
    window2(wx + 234, wy + 60, 88, 120, 1, 1);
    pos(wx + 280, wy + 120);
    gmode(2);
    gcopy_c(
        20 + cc,
        facing_direction / 4 % 4 * 32,
        facing_direction / 16 % 4 * 48,
        32,
        48,
        48,
        80);
}

static void _draw_option_arrows(int cnt)
{
    pos(wx + 30, wy + 66 + cnt * 21 - 5);
    gcopy(3, 312, 336, 24, 24);
    pos(wx + 175, wy + 66 + cnt * 21 - 5);
    gcopy(3, 336, 336, 24, 24);
}

static void _draw_option(int cnt, const std::string& text)
{
    std::string option_text = text;
    if (cnt != 0)
    {
        int part_index = 20 + cnt - 1;
        if (pcc(part_index, cc) == 0)
        {
            option_text += u8"On"s;
        }
        else
        {
            option_text += u8"Off"s;
        }
        _draw_option_arrows(cnt);
    }
    cs_list(cs == cnt, option_text, wx + 60, wy + 66 + cnt * 21 - 1, 0);
}

static void _draw_options()
{
    gmode(2);
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        if (cnt >= listmax)
        {
            break;
        }

        const std::string& text = listn(0, cnt);
        _draw_option(cnt, text);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
}

void ui_menu_change_appearance_equip::draw()
{
    _draw_window();

    ++_frame;
    if (_frame % 100 < 45)
    {
        _pcc_facing_direction = _frame % 16;
    }
    else
    {
        ++_pcc_facing_direction;
    }

    _draw_own_sprite(_pcc_facing_direction);
    _draw_options();
}

static void _toggle_equip_display(int index)
{
    int part_index = 20 + index - 1;
    if (pcc(part_index, cc) == 0)
    {
        pcc(part_index, cc) = 1;
    }
    else
    {
        pcc(part_index, cc) = 0;
    }
    create_pcpic(cc, true);
    snd(5);
}

optional<ui_menu_change_appearance_equip::result_type>
ui_menu_change_appearance_equip::on_key(const std::string& key)
{
    if (cs != 0)
    {
        if (key == key_enter || key == key_pageup || key == key_pagedown)
        {
            _toggle_equip_display(cs);
        }
    }
    if ((cs == 0 && key == key_enter) || key == key_cancel)
    {
        snd(20);
        create_pcpic(cc, true);

        return ui_menu_change_appearance_equip::result::finish();
    }

    return none;
}

} // namespace ui
} // namespace elona
