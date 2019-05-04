#pragma once
#include "../lua_env/mod_manifest.hpp"
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
    {
    }

protected:
    virtual bool init();
    virtual void update();
    virtual void draw();
    virtual optional<UIMenuMods::ResultType> on_key(const std::string& key);

    optional<ModDescription> _find_enabled_mod(const std::string& name);
    void _draw_key(int cnt, int index);
    void _draw_mod_list();
    void _draw_window();

private:
    std::vector<ModDescription> _mod_descriptions;
    std::vector<std::string> _description_pages;

    bool _redraw;
};
} // namespace ui
} // namespace elona
