#pragma once
#include "../lua_env/mod_manifest.hpp"
#include "../mod_download/mod_downloader_impl.hpp"
#include "../mod_download/network_mod_description.hpp"
#include "ui_menu.hpp"
namespace elona
{
namespace ui
{
struct ModDescription
{
    lua::ModManifest manifest;
    bool enabled;
};

class UIMenuMods : public UIMenu<DummyResult>
{
public:
    UIMenuMods()
        : _downloader("http://localhost:3000")
    {
    }

protected:
    virtual bool init();
    virtual void update();
    virtual void draw();
    virtual optional<UIMenuMods::ResultType> on_key(const std::string& key);

    void _load_mods();
    optional<ModDescription> _find_enabled_mod(const std::string& name);
    void _draw_background();
    void _draw_key(int cnt);
    void _draw_installed_mod(int cnt, int index);
    void _draw_network_mod(int cnt, int index);
    void _draw_mod_list();
    void _draw_window();

private:
    std::vector<ModDescription> _mod_descriptions;
    std::vector<std::string> _description_pages;

    std::vector<NetworkModDescription> _network_mod_descriptions;

    ModDownloaderImpl _downloader;

    bool _redraw;
    bool _is_download;
    optional<std::string> _caption;
};
} // namespace ui
} // namespace elona
