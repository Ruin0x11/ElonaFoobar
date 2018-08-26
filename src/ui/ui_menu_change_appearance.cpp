#include "ui_menu_change_appearance.hpp"
#include "../audio.hpp"
#include "../character.hpp"
#include "../draw.hpp"
#include "../i18n.hpp"

namespace elona
{
namespace ui
{

static void _set_pcc_info(int list_index, pcc_info& info)
{
    info.item = pcc_info::item_type::pcc_part;
    info.use_external_image = true;

    if (page == 0)
    {
        switch (list_index)
        {
        case 0: info.item = pcc_info::item_type::confirm; break;
        case 1:
            info.index = 100;
            info.current_value = cdata[cc].portrait;
            break;
        case 2:
            info.index = 1;
            info.current_value = pcc(1, cc) % 1000;
            info.name = u8"hair"s;
            break;
        case 3:
            info.index = 10;
            info.current_value = pcc(10, cc) % 1000;
            info.name = u8"subhair"s;
            break;
        case 4:
            info.index = 1;
            info.use_external_image = false;
            info.current_value = pcc(1, cc) / 1000;
            break;
        case 5:
            info.index = 15;
            info.current_value = pcc(15, cc) % 1000;
            info.name = u8"body"s;
            break;
        case 6:
            info.index = 9;
            info.current_value = pcc(9, cc) % 1000;
            info.name = u8"cloth"s;
            break;
        case 7:
            info.index = 7;
            info.current_value = pcc(7, cc) % 1000;
            info.name = u8"pants"s;
            break;
        case 8:
            info.item = pcc_info::item_type::change_page;
            info.name = "";
            break;
        case 9:
            if (cc != 0)
            {
                info.index = 101;
                info.current_value = cdata[cc].has_own_sprite();
            }
            else
            {
                info.index = 16;
                info.current_value = pcc(16, cc) % 1000;
                info.name = u8"ride"s;
            }
            break;
        }
    }
    else
    {
        switch (list_index)
        {
        case 0:
            info.index = 15;
            info.use_external_image = false;
            info.current_value = pcc(15, cc) / 1000;
            break;
        case 1:
            info.index = 9;
            info.use_external_image = false;
            info.current_value = pcc(9, cc) / 1000;
            break;
        case 2:
            info.index = 7;
            info.use_external_image = false;
            info.current_value = pcc(7, cc) / 1000;
            break;
        case 3:
            info.index = 11;
            info.current_value = pcc(11, cc) % 1000;
            info.name = u8"etc"s;
            break;
        case 4:
            info.index = 12;
            info.current_value = pcc(12, cc) % 1000;
            info.name = u8"etc"s;
            break;
        case 5:
            info.index = 13;
            info.current_value = pcc(13, cc) % 1000;
            info.name = u8"etc"s;
            break;
        case 6:
            info.index = 14;
            info.current_value = pcc(14, cc) % 1000;
            info.name = u8"eye"s;
            break;
        case 7:
            info.item = pcc_info::item_type::change_page;
            info.name = "";
            break;
        }
    }
} // namespace ui

static void _load_portraits()
{
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"face1.bmp", 1);
    buffer(7, 800, 112);
    boxf();
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        const auto filepath = filesystem::dir::user() / u8"graphic"
            / (u8"face"s + (cnt + 1) + u8".bmp");
        if (fs::exists(filepath))
        {
            pos(cnt * 80, 0);
            picload(filepath, 1);
        }
    }
}

static void _draw_deco()
{
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_mirror.bmp", 1);
}

bool ui_menu_change_appearance::init()
{
    create_pcpic(cc, false);
    page = 0;
    pagesize = 19;
    cs = 0;
    cs_bk = -1;
    ww = 380;
    wh = 340;
    wx = (windoww - ww) / 2 + inf_screenx;
    wy = winposy(wh);
    snd(97);
    window_animation(wx, wy, ww, wh, 9, 7);

    _load_portraits();
    _draw_deco();

    gsel(0);
    windowshadow = 1;

    return true;
}

void ui_menu_change_appearance::update()
{
    listmax = 0;
    if (page == 0)
    {
        s(0) = i18n::s.get("core.locale.ui.appearance.basic.done");
        s(1) = i18n::s.get("core.locale.ui.appearance.basic.portrait");
        s(2) = i18n::s.get("core.locale.ui.appearance.basic.hair");
        s(3) = i18n::s.get("core.locale.ui.appearance.basic.sub_hair");
        s(4) = i18n::s.get("core.locale.ui.appearance.basic.hair_color");
        s(5) = i18n::s.get("core.locale.ui.appearance.basic.body");
        s(6) = i18n::s.get("core.locale.ui.appearance.basic.cloth");
        s(7) = i18n::s.get("core.locale.ui.appearance.basic.pants");
        s(8) = i18n::s.get("core.locale.ui.appearance.basic.set_detail");
        if (cc != 0)
        {
            s(9) = i18n::s.get("core.locale.ui.appearance.basic.custom");
        }
        else
        {
            s(9) = i18n::s.get("core.locale.ui.appearance.basic.riding");
        }
        p = 9 + (cc != 0) + (cc == 0) * (gdata_mount != 0);
    }
    else
    {
        s(0) = i18n::s.get("core.locale.ui.appearance.detail.body_color");
        s(1) = i18n::s.get("core.locale.ui.appearance.detail.cloth_color");
        s(2) = i18n::s.get("core.locale.ui.appearance.detail.pants_color");
        s(3) = i18n::s.get("core.locale.ui.appearance.detail.etc_1");
        s(4) = i18n::s.get("core.locale.ui.appearance.detail.etc_2");
        s(5) = i18n::s.get("core.locale.ui.appearance.detail.etc_3");
        s(6) = i18n::s.get("core.locale.ui.appearance.detail.eyes");
        s(7) = i18n::s.get("core.locale.ui.appearance.detail.set_basic");
        p = 8;
    }
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

static void _draw_window()
{
    pagesize = 0;
    s(0) = i18n::s.get("core.locale.ui.appearance.basic.title");
    s(1) = i18n::s.get("core.locale.ui.appearance.hint");
    display_window(
        (windoww - 380) / 2 + inf_screenx, winposy(340) - 12, 380, 340);
    pagesize = listmax;
    display_topic(
        i18n::s.get("core.locale.ui.appearance.basic.category"),
        wx + 34,
        wy + 36);
    pos(wx + ww - 40, wy);
    gcopy(3, 960, 96, 48, 120);
}

static void _draw_portrait()
{
    if (cdata[cc].portrait >= 0)
    {
        int portrait_id = cdata[cc].sex * 64 + cdata[cc].portrait;
        pos(wx + 238, wy + 75);
        gcopy(4, portrait_id % 16 * 48, portrait_id / 16 * 72, 48, 72, 80, 112);
    }
    else if (cdata[cc].portrait != -1)
    {
        pos(wx + 238, wy + 75);
        gcopy(7, std::abs((cdata[cc].portrait + 2)) * 80, 0, 80, 112, 80, 112);
    }
}

static void _draw_own_sprite(int facing_direction)
{
    pos(wx + 280, wy + 130);
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

static void _draw_appearance(int facing_direction)
{
    window2(wx + 234, wy + 71, 88, 120, 1, 1);
    if (cs == 1 && page == 0)
    {
        _draw_portrait();
    }
    else if (cdata[cc].has_own_sprite() == 1)
    {
        _draw_own_sprite(facing_direction);
    }
    else
    {
        draw_chara(cdata[cc], wx + 280, wy + 130);
    }
}

static void _draw_option_arrows(int cnt)
{
    pos(wx + 30, wy + 66 + cnt * 21 - 5);
    gcopy(3, 312, 336, 24, 24);
    pos(wx + 175, wy + 66 + cnt * 21 - 5);
    gcopy(3, 336, 336, 24, 24);
}

static void _draw_option(int cnt, const pcc_info& info, const std::string& text)
{
    std::string option_text = text;
    if (info.item == pcc_info::item_type::pcc_part)
    {
        if (info.current_value >= 0)
        {
            option_text += u8" "s + info.current_value;
        }
        else if (info.current_value == -1)
        {
            option_text += u8" N/A"s;
        }
        else
        {
            option_text += u8" u"s + (std::abs(info.current_value) - 1);
        }
    }
    cs_list(cs == cnt, option_text, wx + 60, wy + 66 + cnt * 21 - 1, 0);
    if (info.item != pcc_info::item_type::confirm)
    {
        _draw_option_arrows(cnt);
    }
}

static void _draw_options()
{
    gmode(2);
    font(14 - en * 2);
    cs_listbk();

    pcc_info info;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        int index = cnt;
        if (index >= listmax)
        {
            break;
        }
        _set_pcc_info(cnt, info);
        const std::string& text = listn(0, index);

        _draw_option(cnt, info, text);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
}

void ui_menu_change_appearance::draw()
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

    _draw_appearance(_pcc_facing_direction);
    _draw_options();
}

static bool _increase_option(const pcc_info& info)
{
    snd(5);
    if (info.index == 100)
    {
        if (cdata[cc].portrait < 31)
        {
            ++cdata[cc].portrait;
        }
        return true;
    }
    if (info.index == 101)
    {
        cdata[cc].has_own_sprite() = true;
        return true;
    }
    if (info.use_external_image)
    {
        if (fs::exists(
                filesystem::dir::graphic()
                / (u8"pcc_"s + info.name + u8"_"
                   + (pcc(info.index, cc) % 1000 + 1) + u8".bmp")))
        {
            ++pcc(info.index, cc);
            p = 1;
        }
    }
    else if (pcc(info.index, cc) / 1000 < 21)
    {
        pcc(info.index, cc) += 1000;
        p = 1;
    }

    return false;
}

static bool _decrease_option(const pcc_info& info)
{
    snd(5);
    if (info.index == 100)
    {
        if (cdata[cc].portrait > -10)
        {
            --cdata[cc].portrait;
        }
        return true;
    }
    if (info.index == 101)
    {
        cdata[cc].has_own_sprite() = false;
        return true;
    }
    if (info.use_external_image)
    {
        if ((pcc(info.index, cc) % 1000 == 1 && info.index != 15)
            || fs::exists(
                   filesystem::dir::graphic()
                   / (u8"pcc_"s + info.name + u8"_"s
                      + (pcc(info.index, cc) % 1000 - 1) + u8".bmp"s)))
        {
            --pcc(info.index, cc);
            p = 1;
        }
    }
    else if (pcc(info.index, cc) / 1000 > 0)
    {
        pcc(info.index, cc) -= 1000;
        p = 1;
    }

    return false;
}

optional<ui_menu_change_appearance::result_type>
ui_menu_change_appearance::on_key(const std::string& key)
{
    pcc_info info;
    _set_pcc_info(cs, info);
    p = 0;

    if (info.item == pcc_info::item_type::confirm)
    {
        if (key == key_enter)
        {
            create_pcpic(cc, true);
            return ui_menu_change_appearance::result::finish(true);
        }
        if (key == key_pageup || key == key_pagedown)
        {
            return none;
        }
    }
    else if (info.item == pcc_info::item_type::change_page)
    {
        if (key == key_pageup || key == key_enter || key == key_pagedown)
        {
            snd(5);
            if (page == 0)
            {
                page = 1;
            }
            else
            {
                page = 0;
                cs = 8;
            }
            set_reupdate();
            return none;
        }
    }
    else
    {
        if (key == key_pageup || key == key_enter)
        {
            if (_increase_option(info))
            {
                return none;
            }
        }
        else if (key == key_pagedown)
        {
            if (_decrease_option(info))
            {
                return none;
            }
        }

        create_pcpic(cc, false);

        if (key == key_cancel)
        {
            create_pcpic(cc, true);
            return ui_menu_change_appearance::result::cancel();
        }
        else if (
            getkey(snail::key::f1) && _operation == operation::character_making)
        {
            return ui_menu_change_appearance::result::finish();
        }
    }

    return none;
}

} // namespace ui
} // namespace elona
