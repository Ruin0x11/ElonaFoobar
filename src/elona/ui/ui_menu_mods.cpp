#include "ui_menu_mods.hpp"

#include <iomanip>

#include "../../util/fileutil.hpp"
#include "../../util/strutil.hpp"
#include "../audio.hpp"
#include "../draw.hpp"
#include "../i18n.hpp"
#include "../lua_env/mod_manager.hpp"
#include "simple_prompt.hpp"
#include "ui_menu_mod_info.hpp"



namespace elona
{
namespace ui
{

bool UIMenuMods::init()
{
    snd("core.pop2");

    _is_download = false;
    _load_mods();

    update();

    return true;
}


class DownloadModsInitPrompt
    : public SimplePrompt<NetworkResult<NetworkModList>>
{
public:
    DownloadModsInitPrompt(ModDownloaderImpl& downloader)
        : SimplePrompt("Downloading")
        , _downloader(downloader)
    {
    }

protected:
    optional<NetworkResult<NetworkModList>> update() override
    {
        await(Config::instance().general_wait);

        auto result = _downloader.list(1);

        if (result.is_right())
        {
            _message = "Failed. " + result.right();

            draw();

            auto action = key_check();

            if (action != ""s)
            {
                return result;
            }

            return none;
        }

        return result;
    }

private:
    ModDownloaderImpl& _downloader;
};


void UIMenuMods::_load_mods()
{
    // Redraw background as topic size can change, leaving marks
    _draw_background();

    _mod_descriptions.clear();
    _network_mod_descriptions.clear();

    listmax = 0;
    page = 0;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    page_bk = -1;
    cs_bk2 = 0;
    pagesize = 18;

    if (_is_download)
    {
        draw();
        auto result = DownloadModsInitPrompt(_downloader).query();

        if (result.is_left())
        {
            _network_mod_descriptions = result.left();
            listmax = _network_mod_descriptions.size();
        }
        else
        {
            // Recurse.
            _is_download = false;
            _load_mods();
        }
    }
    else
    {
        for (const auto& mod : lua::lua->get_mod_manager().all_mods())
        {
            const auto& id = mod->second->manifest.id;

            if (lua::ModManager::mod_id_is_reserved(id))
                continue;

            ModDescription mod_desc{
                mod->second->manifest,
                static_cast<bool>(
                    lua::lua->get_mod_manager().get_enabled_version(id))};

            _mod_descriptions.emplace_back(mod_desc);
            listmax++;
        }
    }
}


optional<ModDescription> UIMenuMods::_find_enabled_mod(const std::string& name)
{
    for (const auto& desc : _mod_descriptions)
    {
        if (desc.enabled && desc.manifest.id == name)
            return desc;
    }
    return none;
}



void UIMenuMods::update()
{
    pagemax = (listmax - 1) / pagesize;
    if (page < 0)
        page = pagemax;
    else if (page > pagemax)
        page = 0;

    cs_bk = -1;
    page_bk = page;

    _redraw = true;
}



void UIMenuMods::_draw_background()
{
    asset_load("void");
    ::draw("void", 0, 0, windoww, windowh);
    gsel(0);
    gmode(0);
    gcopy(4, 0, 0, windoww, windowh, 0, 0);
    gmode(2);

    windowshadow = 1;
}



void UIMenuMods::_draw_key(int cnt)
{
    if (cnt % 2 == 0)
    {
        boxf(wx + 57, wy + 66 + cnt * 19, 640, 18, {12, 14, 16, 16});
    }

    display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
}



void UIMenuMods::_draw_window()
{
    std::string hint;
    std::string title;
    std::string caption;

    if (_is_download)
    {
        hint = ""s + key_enter + " " +
            i18n::s.get("core.locale.main_menu.mods.hint.do_download") + "  " +
            key_mode + " " +
            i18n::s.get("core.locale.main_menu.mods.hint.installed");
        title = i18n::s.get("core.locale.main_menu.mods.title.download");
        caption = i18n::s.get("core.locale.main_menu.mods.caption.download");
    }
    else
    {
        hint = ""s + key_enter + " " +
            i18n::s.get("core.locale.main_menu.mods.hint.toggle") + "  " +
            key_identify + " " +
            i18n::s.get("core.locale.main_menu.mods.hint.info") + "  " +
            key_mode + " " +
            i18n::s.get("core.locale.main_menu.mods.hint.download");
        title = i18n::s.get("core.locale.main_menu.mods.title.installed");
        caption = i18n::s.get("core.locale.main_menu.mods.caption.installed");
    }

    if (_caption)
    {
        caption = *_caption;
        _caption = none;
    }

    ui_draw_caption(caption);

    ui_display_window(
        title,
        strhint2 + strhint3b + hint,
        (windoww - 730) / 2 + inf_screenx,
        winposy(460, 1),
        730,
        480);

    keyrange = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        int index = pagesize * page + cnt;
        if (index >= listmax)
            break;
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        _draw_key(cnt);
    }

    display_topic(
        i18n::s.get("core.locale.main_menu.mods.info.name"), wx + 46, wy + 36);
    display_topic(
        i18n::s.get("core.locale.main_menu.mods.info.version"),
        wx + 225,
        wy + 36);

    if (_is_download)
    {
        display_topic(
            i18n::s.get("core.locale.main_menu.mods.info.updated"),
            wx + 335,
            wy + 36);
        display_topic(
            i18n::s.get("core.locale.main_menu.mods.info.downloads"),
            wx + 525,
            wy + 36);
    }
}



void UIMenuMods::_draw_installed_mod(int cnt, int index)
{
    const auto& desc = _mod_descriptions.at(index);
    snail::Color text_color = {0, 0, 0};
    if (desc.enabled)
    {
        text_color = {0, 0, 255};
    }

    int y = wy + 66 + cnt * 19 - 1 + vfix + 3;

    cs_list(
        cs == cnt,
        desc.manifest.name,
        wx + 84,
        wy + 66 + cnt * 19 - 1,
        0,
        text_color);
    mes(wx + 240, y, desc.manifest.version.to_string(), {0, 0, 0});
}



static std::string _date_int(int i, int w)
{
    std::ostringstream oss;
    oss << std::setw(w) << std::setfill('0') << i;
    return oss.str();
}



void UIMenuMods::_draw_network_mod(int cnt, int index)
{
    const auto& desc = _network_mod_descriptions.at(index);
    snail::Color text_color = {0, 0, 0};
    const auto& date = desc.updated_at;
    auto date_str = i18n::s.get(
        "core.locale.main_menu.mods.date",
        _date_int(date.year, 4),
        _date_int(date.month, 2),
        _date_int(date.day, 2),
        _date_int(date.hour, 2),
        _date_int(date.minute, 2),
        _date_int(static_cast<int>(date.second), 2));

    int y = wy + 66 + cnt * 19 - 1 + vfix + 3;

    cs_list(
        cs == cnt, desc.name, wx + 84, wy + 66 + cnt * 19 - 1, 0, text_color);
    mes(wx + 240, y, desc.version.to_string(), {0, 0, 0});
    mes(wx + 350, y, date_str, {0, 0, 0});
    mes(wx + 540, y, std::to_string(desc.downloads), {0, 0, 0});
}



void UIMenuMods::_draw_mod_list()
{
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        int index = pagesize * page + cnt;
        if (index >= listmax)
        {
            break;
        }

        if (_is_download)
        {
            _draw_network_mod(cnt, index);
        }
        else
        {
            _draw_installed_mod(cnt, index);
        }
    }

    if (keyrange != 0)
    {
        cs_bk = cs;
    }
}



void UIMenuMods::draw()
{
    if (_redraw)
    {
        _draw_window();
        _draw_mod_list();
        _redraw = false;
    }
}



optional<UIMenuMods::ResultType> UIMenuMods::on_key(const std::string& action)
{
    if (auto selected = get_selected_index_this_page())
    {
        cs = *selected;
        if (_is_download)
        {
            _redraw = true;
            _caption =
                i18n::s.get("core.locale.main_menu.mods.caption.confirm");
            _draw_background();
            draw();

            yes_no();
        }
        else
        {
            auto& desc = _mod_descriptions.at(pagesize * page + cs);
            if (cs_bk == cs)
            {
                snd("core.ok1");
                desc.enabled = !desc.enabled;
            }
            else
            {
                snd("core.pop1");
            }
        }
        set_reupdate();
    }

    if (cs != cs_bk)
    {
        set_reupdate();
    }

    if (action == "next_page")
    {
        if (pagemax != 0)
        {
            snd("core.pop1");
            ++page;
            set_reupdate();
        }
    }
    if (action == "previous_page")
    {
        if (pagemax != 0)
        {
            snd("core.pop1");
            --page;
            set_reupdate();
        }
    }
    if (action == "identify")
    {
        if (!_is_download)
        {
            const auto& desc = _mod_descriptions.at(pagesize * page + cs);

            int cs_prev = cs;
            int page_prev = page;
            int listmax_prev = listmax;
            int pagesize_prev = pagesize;

            lib::scope_guard restore([&]() {
                cs = cs_prev;
                page_prev = page;
                listmax = listmax_prev;
                pagesize = pagesize_prev;
            });

            UIMenuModInfo(desc).show();

            _draw_background();

            set_reupdate();
        }
    }
    if (action == "switch_mode")
    {
        snd("core.ok1");
        cs = 0;
        _is_download = !_is_download;
        _load_mods();
        set_reupdate();
    }

    if (action == "cancel")
    {
        return UIMenuMods::ResultType::finish();
    }
    return none;
}

} // namespace ui
} // namespace elona
