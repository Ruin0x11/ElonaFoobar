#include "config.hpp"
#include <cassert>
#include <fstream>
#include <functional>
#include <string>
#include <stdexcept>
#include "elona.hpp"
#include "range.hpp"
#include "snail/application.hpp"
#include "snail/window.hpp"
#include "variables.hpp"
#include "thirdparty/microhcl/hcl.hpp"


namespace
{


struct config_loading_error : public std::runtime_error
{
    config_loading_error(const std::string& message)
        : std::runtime_error(message)
    {
    }
};



template <typename Iterator, typename Function>
void for_each_with_index(Iterator first, Iterator last, Function f)
{
    for (size_t index = 0; first != last; ++first, ++index)
    {
        (void)f(index, *first);
    }
}



/***
 * Initializes the list of available display modes. To be called after
 * the application has been initialized by calling title().
 */
static void inject_display_modes(config& conf)
{
    const auto display_modes =
        snail::application::instance().get_display_modes();
    std::string default_display_mode =
        snail::application::instance().get_default_display_mode();
    std::vector<std::string> display_mode_names;

    for (const auto pair : display_modes)
    {
        // First pair member contains identifier string, second is SDL
        // display mode struct.
        display_mode_names.emplace_back(pair.first);
    }

    // If the default display_mode is unknown, we're probably in
    // headless mode, so don't try to set any config options (or
    // "invalid enum variant" will be generated).
    if (default_display_mode != "")
    {
        conf.inject_enum("core.config.screen.display_mode", display_mode_names, default_display_mode);
    }
}

/***
 * Initializes the list of save files that can be chosen at startup.
 */
static void inject_save_files(config& conf)
{
    std::vector<std::string> saves;
    saves.push_back("");

    for (const auto& entry : filesystem::dir_entries{
            filesystem::dir::save(), filesystem::dir_entries::type::dir})
    {
        std::string folder = filesystem::to_utf8_path(entry.path().filename());
        saves.push_back(folder);
    }

    conf.inject_enum("core.config.game.default_save", saves, "");
}

/***
 * Initializes the list of languages by adding the names of folders in
 * the locale/ directory.
 *
 * TODO: Support mods which add their own languages.
 */
static void inject_languages(config& conf)
{
    std::vector<std::string> locales;
    bool has_jp = false;
    bool has_en = false;

    for (const auto& entry : filesystem::dir_entries{
            filesystem::dir::locale(), filesystem::dir_entries::type::dir})
    {
        std::string identifier = filesystem::to_utf8_path(entry.path().filename());
        locales.push_back(identifier);

        if (identifier == "en")
        {
            has_en = true;
        }
        if (identifier == "jp")
        {
            has_jp = true;
        }
    }

    // Not having English or Japanese loaded will cause weird things
    // to happen, since many parts of the code depend on one or the
    // other being loaded. This can be removed after those parts of
    // the code are refactored.
    if (!has_en || !has_jp)
    {
        throw config_loading_error("Locale for English or Japanese is missing in locale/ folder.");
    }

    conf.inject_enum("core.config.language.language", locales, spec::unknown_enum_variant);
}

} // namespace



namespace elona
{


void config_query_language()
{
    buffer(4);
    picload(filesystem::dir::graphic() / u8"lang.bmp");
    gsel(0);
    gmode(0);
    p = 0;

    while (1)
    {
        boxf();
        pos(160, 170);
        gcopy(4, 0, 0, 340, 100);
        pos(180, 220 + p * 20);
        gcopy(4, 360, 6, 20, 18);
        redraw();
        await(30);
        if (getkey(snail::key::down))
        {
            p = 1;
        }
        if (getkey(snail::key::keypad_2))
        {
            p = 1;
        }
        if (getkey(snail::key::up))
        {
            p = 0;
        }
        if (getkey(snail::key::keypad_8))
        {
            p = 0;
        }
        if (getkey(snail::key::enter))
        {
            break;
        }
        if (getkey(snail::key::keypad_enter))
        {
            break;
        }
        if (getkey(snail::key::space))
        {
            break;
        }
    }

    std::string locale = spec::unknown_enum_variant;
    if (p == 0)
    {
        locale = "jp";
    }
    else
    {
        locale = "en";
    }
    config::set("language.language", locale);
}

#define CONFIG_OPTION(confkey, type, getter) \
    conf.bind_getter("core.config."s + confkey, [&]() { return (getter); } );             \
    conf.bind_setter<type>("core.config."s + confkey, [&](auto value) { getter = value; } )

#define CONFIG_KEY(confkey, keyname) \
    CONFIG_OPTION((confkey), std::string, keyname)

void load_config(const fs::path& hcl_file)
{
    auto& conf = config::instance();

    inject_display_modes(conf);
    inject_save_files(conf);

    // CONFIG_OPTION("anime.scroll"s,                    bool,        config::instance().scroll); // TODO TEST
    CONFIG_OPTION("font.size_adjustment"s,            int,         sizefix);
    CONFIG_OPTION("font.vertical_offset"s,            int,         vfix);

    CONFIG_KEY("key.north"s,            key_north);
    CONFIG_KEY("key.south"s,            key_south);
    CONFIG_KEY("key.west"s,             key_west);
    CONFIG_KEY("key.east"s,             key_east);
    CONFIG_KEY("key.northwest"s,        key_northwest);
    CONFIG_KEY("key.northeast"s,        key_northeast);
    CONFIG_KEY("key.southwest"s,        key_southwest);
    CONFIG_KEY("key.southeast"s,        key_southeast);
    CONFIG_KEY("key.wait"s,             key_wait);
    CONFIG_KEY("key.cancel"s,           key_cancel);
    CONFIG_KEY("key.esc"s,              key_esc);
    CONFIG_KEY("key.alter"s,            key_alter);
    CONFIG_KEY("key.pageup"s,           key_pageup);
    CONFIG_KEY("key.pagedown"s,         key_pagedown);
    CONFIG_KEY("key.mode"s,             key_mode);
    CONFIG_KEY("key.mode2"s,            key_mode2);
    CONFIG_KEY("key.quick_menu"s,       key_quick);
    CONFIG_KEY("key.zap"s,              key_zap);
    CONFIG_KEY("key.inventory"s,        key_inventory);
    CONFIG_KEY("key.quick_inventory"s,  key_quickinv);
    CONFIG_KEY("key.get"s,              key_get);
    CONFIG_KEY("key.get2"s,             key_get2);
    CONFIG_KEY("key.drop"s,             key_drop);
    CONFIG_KEY("key.chara_info"s,       key_charainfo);
    CONFIG_KEY("key.enter"s,            key_enter);
    CONFIG_KEY("key.eat"s,              key_eat);
    CONFIG_KEY("key.wear"s,             key_wear);
    CONFIG_KEY("key.cast"s,             key_cast);
    CONFIG_KEY("key.drink"s,            key_drink);
    CONFIG_KEY("key.read"s,             key_read);
    CONFIG_KEY("key.fire"s,             key_fire);
    CONFIG_KEY("key.go_down"s,          key_godown);
    CONFIG_KEY("key.go_up"s,            key_goup);
    CONFIG_KEY("key.save"s,             key_save);
    CONFIG_KEY("key.search"s,           key_search);
    CONFIG_KEY("key.interact"s,         key_interact);
    CONFIG_KEY("key.identify"s,         key_identify);
    CONFIG_KEY("key.skill"s,            key_skill);
    CONFIG_KEY("key.close"s,            key_close);
    CONFIG_KEY("key.rest"s,             key_rest);
    CONFIG_KEY("key.target"s,           key_target);
    CONFIG_KEY("key.dig"s,              key_dig);
    CONFIG_KEY("key.use"s,              key_use);
    CONFIG_KEY("key.bash"s,             key_bash);
    CONFIG_KEY("key.open"s,             key_open);
    CONFIG_KEY("key.dip"s,              key_dip);
    CONFIG_KEY("key.pray"s,             key_pray);
    CONFIG_KEY("key.offer"s,            key_offer);
    CONFIG_KEY("key.journal"s,          key_journal);
    CONFIG_KEY("key.material"s,         key_material);
    CONFIG_KEY("key.trait"s,            key_trait);
    CONFIG_KEY("key.look"s,             key_look);
    CONFIG_KEY("key.give"s,             key_give);
    CONFIG_KEY("key.throw"s,            key_throw);
    CONFIG_KEY("key.ammo"s,             key_ammo);
    CONFIG_KEY("key.autodig"s,          key_autodig);
    CONFIG_KEY("key.quicksave"s,        key_quicksave);
    CONFIG_KEY("key.quickload"s,        key_quickload);
    CONFIG_KEY("key.help"s,             key_help);
    CONFIG_KEY("key.message_log"s,      key_msglog);

    conf.bind_setter<hcl::List>("core.config.key.key_set",
                    [&](auto values)
                        {
                            for_each_with_index(
                                std::begin(values),
                                std::end(values),
                                [&](auto index, hcl::Value value) {
                                    std::string s = value.as<std::string>();
                                    key_select(index) = s;
                                });
                        });

    std::ifstream ifs{filesystem::make_preferred_path_in_utf8(hcl_file.native())};
    conf.load(ifs, hcl_file.string(), false);

    key_prev = key_northwest;
    key_next = key_northeast;

    if (config::instance().zkey == 0)
    {
        key_quick = u8"z"s;
        key_zap = u8"Z"s;
    }
    else if (config::instance().zkey == 1)
    {
        key_zap = u8"z"s;
        key_quick = u8"Z"s;
    }
    if (config::instance().xkey == 0)
    {
        key_quickinv = u8"x"s;
        key_inventory = u8"X"s;
    }
    else if (config::instance().xkey == 1)
    {
        key_inventory = u8"x"s;
        key_quickinv = u8"X"s;
    }
    if (config::get<std::string>("language.language") == spec::unknown_enum_variant)
    {
        config_query_language();
    }
    if (config::get<std::string>("language.language") == "jp")
    {
        jp = 1;
        vfix = 0;
        sizefix = 0;
    }
    else
    {
        en = 1;
    }
    if (key_mode == ""s)
    {
        key_mode = u8"z"s;
        conf.set_option("core.config.key.mode", key_mode);
    }
    if (key_mode2 == ""s)
    {
        key_mode2 = u8"*"s;
        conf.set_option("core.config.key.mode2", key_mode2);
    }
    if (key_ammo == ""s)
    {
        key_ammo = u8"A"s;
        conf.set_option("core.config.key.ammo", key_ammo);
    }
}

void load_config2(const fs::path& hcl_file)
{
    auto& conf = config::instance();

    inject_languages(conf);

    CONFIG_OPTION("ui.msg_line"s,         int,         inf_msgline);
    CONFIG_OPTION("ui.tile_size"s,        int,         inf_tiles);
    CONFIG_OPTION("ui.font_size"s,        int,         inf_mesfont);
    CONFIG_OPTION("ui.inf_ver_type"s,     int,         inf_vertype);
    CONFIG_OPTION("ui.window_x"s,         int,         windowx);
    CONFIG_OPTION("ui.window_y"s,         int,         windowy);
    CONFIG_OPTION("ui.clock_x"s,          int,         inf_clockx);
    CONFIG_OPTION("ui.clock_w"s,          int,         inf_clockw);
    CONFIG_OPTION("ui.clock_h"s,          int,         inf_clockh);
    CONFIG_OPTION("game.default_save"s,   std::string, defload);

    std::ifstream ifs{filesystem::make_preferred_path_in_utf8(hcl_file.native())};
    conf.load(ifs, hcl_file.string(), true);
}

#undef CONFIG_OPTION
#undef CONFIG_KEY

snail::window::fullscreen_mode_t config_get_fullscreen_mode()
{
    if (config::get<std::string>("screen.fullscreen") == "fullscreen")
    {
        return snail::window::fullscreen_mode_t::fullscreen;
    }
    else if (config::get<std::string>("screen.fullscreen") == "desktop_fullscreen")
    {
        return snail::window::fullscreen_mode_t::fullscreen_desktop;
    }
    else
    {
        return snail::window::fullscreen_mode_t::windowed;
    }
}

config& config::instance()
{
    static config the_instance;
    return the_instance;
}

void config::init(const fs::path& config_def_file)
{
    clear();
    def.init(config_def_file);
}

void config::init(const config_def def_)
{
    clear();
    def = def_;
}

void config::load_defaults(bool preload)
{
    for (auto& pair : def)
    {
        const std::string& key = pair.first;

        // Sections don't have defaults, so trying to set them would
        // cause an error.
        if (!def.is<spec::section_def>(key))
        {
            if (preload == def.get_metadata(key).preload)
            {
                set_option(key, def.get_default(key));
            }
        }
    }
}

void config::load(std::istream& is, const std::string& hcl_file, bool preload)
{
    load_defaults(preload);

    hcl::ParseResult parseResult = hcl::parse(is);

    if (!parseResult.valid()) {
        std::cerr << parseResult.errorReason << std::endl;
        throw config_loading_error(u8"Failed to read " + hcl_file + u8": "
                                   + parseResult.errorReason);
    }

    // TODO: This pattern seems to be shared in various places in the
    // code.
    const hcl::Value& value = parseResult.value;

    if (!value.is<hcl::Object>() || !value.has("config"))
    {
        throw config_loading_error(hcl_file + ": \"config\" object not found at top level");
    }

    const hcl::Value conf = value["config"];

    // TODO mod support
    if (!conf.is<hcl::Object>() || !conf.has("core"))
    {
        throw config_loading_error(hcl_file + ": \"core\" object not found after \"config\"");
    }

    const hcl::Value core = conf["core"];
    visit_object(core.as<hcl::Object>(), "core.config", hcl_file, preload);
}

void config::visit_object(const hcl::Object& object,
                          const std::string& current_key,
                          const std::string& hcl_file,
                          bool preload)
{
    for (const auto& pair : object)
    {
        visit(pair.second, current_key + "." + pair.first, hcl_file, preload);
    }
}

void config::visit(const hcl::Value& value,
                   const std::string& current_key,
                   const std::string& hcl_file,
                   bool preload)
{
    if (value.is<hcl::Object>())
    {
        if (!def.is<spec::section_def>(current_key))
        {
            throw config_loading_error(hcl_file + ": No such config section \"" + current_key + "\".");
        }
        visit_object(value.as<hcl::Object>(), current_key, hcl_file, preload);
    }
    else
    {
        if (!def.exists(current_key))
        {
            throw config_loading_error(hcl_file + ": No such config value \"" + current_key + "\".");
        }
        if (preload == def.get_metadata(current_key).preload)
        {
            set_option(current_key, value);
        }
    }
}

bool config::verify_types(const hcl::Value& value, const std::string& current_key)
{
    if (def.is<spec::section_def>(current_key))
    {
        // It doesn't make sense to set a section as a value.
        return false;
    }
    if (value.is<bool>())
    {
        return def.is<spec::bool_def>(current_key);
    }
    if (value.is<int>())
    {
        return def.is<spec::int_def>(current_key);
    }
    if (value.is<hcl::List>())
    {
        return def.is<spec::list_def>(current_key);
    }
    if (value.is<std::string>())
    {
        if (def.is<spec::enum_def>(current_key))
        {
            auto enum_def = def.get<spec::enum_def>(current_key);
            if (enum_def.pending)
            {
                // The key could be anything because the values are
                // not known yet, so don't attempt to check anything.
                return true;
            }
            else
            {
                return static_cast<bool>(enum_def.get_index_of(value.as<std::string>()));
            }
        }
        else
        {
            return def.is<spec::string_def>(current_key);
        }
    }

    return false;
}

void config::write()
{
    std::ofstream file{(filesystem::dir::exe() / u8"config.hcl").native(),
                       std::ios::binary};
    if (!file)
    {
        throw config_loading_error{
            u8"Failed to open: "s
                + filesystem::make_preferred_path_in_utf8(
                    filesystem::dir::exe() / u8"config.hcl")};
    }

    // Create a top level "config" section.
    hcl::Value out = hcl::Value(hcl::Object());
    out.set("config", hcl::Object());
    hcl::Value* parent = out.find("config");
    assert(parent);

    // Create sections under the top-level "config" section for each
    // mod that has config options (for now, only "core"), then write
    // their individual config sections.
    for (auto&& pair : storage)
    {
        std::string key = pair.first;
        hcl::Value value = pair.second;
        hcl::Value* current = parent;

        // Don't save injected enum values that are still unknown
        // (though this should never happen)
        if (def.is<spec::enum_def>(key)
            && value.as<std::string>() == spec::unknown_enum_variant)
        {
            ELONA_LOG("Injected enum value was unknown on save: " << key);
            continue;
        }

        size_t pos = 0;
        std::string token;

        // Function to split the flat key ("core.config.some.option")
        // on the next period and set the token to the split section
        // name ("some" or "option").
        auto advance = [&pos, &key, &token]()
                           {
                               pos = key.find(".");
                               if (pos == std::string::npos)
                               {
                                   return false;
                               }
                               token = key.substr(0, pos);
                               key.erase(0, pos + 1);
                               return true;
                           };

        // Function that either creates a new object for holding the
        // nested config value or finds an existing one.
        auto set = [&current](std::string key)
                       {
                           hcl::Value* existing = current->find(key);
                           if (existing)
                           {
                               current = existing;
                           }
                           else
                           {
                               current->set(key, hcl::Object());
                               current = current->find(key);
                               assert(current);
                           }
                       };

        // Get the mod-level scope ("core").
        assert(advance());
        std::string scope = token;
        set(token);

        // Skip the "config" section name.
        assert(advance());
        assert(token == "config");

        while (advance()) {
            set(token);
        }

        current->set(key, value);
    }

    file << out;

}

} // namespace elona
