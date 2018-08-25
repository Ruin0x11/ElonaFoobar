#include "ui_menu_change_appearance_equip.hpp"
#include "../audio.hpp"
#include "../draw.hpp"
#include "../i18n.hpp"

namespace elona
{
namespace ui
{

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

    return true;
}

void ui_menu_change_appearance_equip::update()
{
}

void ui_menu_change_appearance_equip::draw()
{
    pagesize = 0;
    s(0) = i18n::s.get("core.locale.ui.appearance.equipment.title");
    s(1) = i18n::s.get("core.locale.ui.appearance.hint");
    display_window(
        (windoww - 360) / 2 + inf_screenx, winposy(289) - 12, 360, 289);
    s = i18n::s.get("core.locale.ui.appearance.equipment.part");
    pagesize = listmax;
    display_topic(s, wx + 34, wy + 36);
    ++i;
    if (i % 100 < 45)
    {
        f = i % 16;
    }
    else
    {
        ++f;
    }
    window2(wx + 234, wy + 60, 88, 120, 1, 1);
    pos(wx + 280, wy + 120);
    gmode(2);
    gcopy_c(20 + cc, f / 4 % 4 * 32, f / 16 % 4 * 48, 32, 48, 48, 80);
    gmode(2);
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = cnt;
        if (p >= listmax)
        {
            break;
        }
        s = listn(0, p);
        if (cnt != 0)
        {
            if (pcc(20 + cnt - 1, cc) == 0)
            {
                s += u8"On"s;
            }
            else
            {
                s += u8"Off"s;
            }
            pos(wx + 30, wy + 66 + cnt * 21 - 5);
            gcopy(3, 312, 336, 24, 24);
            pos(wx + 175, wy + 66 + cnt * 21 - 5);
            gcopy(3, 336, 336, 24, 24);
        }
        cs_list(cs == cnt, s, wx + 60, wy + 66 + cnt * 21 - 1, 0);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
}

optional<ui_menu_change_appearance_equip::result_type>
ui_menu_change_appearance_equip::on_key(const std::string& key)
{
    if (cs != 0)
    {
        if (key == key_enter || key == key_pageup || key == key_pagedown)
        {
            if (pcc(20 + cs - 1, cc) == 0)
            {
                pcc(20 + cs - 1, cc) = 1;
            }
            else
            {
                pcc(20 + cs - 1, cc) = 0;
            }
            create_pcpic(cc, true);
            snd(5);
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
