#pragma once

#include "filesystem.hpp"
#include "lib/noncopyable.hpp"
#include "config_def.hpp"
#include "elona.hpp"
#include "log.hpp"
#include "snail/window.hpp"
#include <map>
#include <string>
#include <iostream>



namespace elona
{

class config_def;

class config
{
public:
    static config& instance();

    template <typename T>
    static T get(const std::string& key) {
        return config::instance().get_option<T>("core.config." + key);
    }

    static void set(const std::string& key, const hcl::Value value)
    {
        return config::instance().set_option("core.config." + key, value);
    }

    config() {}
    ~config() = default;

    void init(const fs::path&);
    void init(const config_def);
    void load(std::istream&, const std::string&, bool);
    void write();

    void clear()
    {
        def.clear();
        storage.clear();
        getters.clear();
        setters.clear();
    }

    int alert;
    bool alwayscenter;
    int animewait;
    bool attackanime;
    int attackwait;
    bool autonumlock;
    bool autosave;
    std::string autoturn;
    bool damage_popup;
    std::string display_mode;
    bool env;
    bool extraclass;
    bool extrahelp;
    bool extrarace;
    std::string font1;
    std::string font2;
    std::string fullscreen;
    bool heart;
    bool hideautoidentify;
    bool hideshopresult;
    std::string hp_bar;
    bool attack_neutral_npcs;
    bool joypad;
    int keywait;
    std::string language;
    bool leash_icon;
    bool msgaddtime;
    int msgtrans;
    std::string music;
    bool net;
    bool netchat;
    bool netwish;
    bool noadebug;
    bool objectshadow;
    int restock_interval;
    bool runscroll;
    int runwait;
    bool scroll;
    int scrsync;
    int select_fast_start;
    int select_wait;
    int select_fast_wait;
    bool serverlist;
    bool shadow;
    bool skiprandevents;
    bool sound;
    int startrun;
    std::string startup_script;
    bool story;
    bool titleanime;
    int wait1;
    int walkwait;
    bool windowanime;
    bool wizard;
    bool xkey;
    bool zkey;

    bool use_autopick;
    bool use_autopick_in_home;
    bool use_autopick_in_dungeon;
    bool use_autopick_in_shop;
    bool use_autopick_in_ranch;
    bool use_autopick_in_crop;
    bool use_autopick_in_storage_house;
    bool use_autopick_in_shelter;
    bool use_autopick_in_town;
    bool use_autopick_in_conquered_nefia;
    bool use_autopick_in_conquered_quest_map;

    bool is_test = false; // testing use only

    void bind_getter(const std::string& key,
                     std::function<hcl::Value(void)> getter)
    {
        if (!def.exists(key))
        {
            throw std::runtime_error("No such config value " + key);
        }
        getters[key] = getter;
    }

    template <typename T>
    void bind_setter(const std::string& key,
                  std::function<void(const T&)> setter)
    {
        if (!def.exists(key))
        {
            throw std::runtime_error("No such config value " + key);
        }
        setters[key] = [setter](const hcl::Value& value){ setter(value.as<T>()); };
    }

    void inject_enum(const std::string& key,
                     std::vector<std::string> variants,
                     std::string default_variant)
    {
        def.inject_enum(key, variants, default_variant);

        auto enum_def = def.get<spec::enum_def>(key);
        if (storage.find(key) != storage.end())
        {
            // Check if this enum has an invalid value. If so, set it to the default.
            std::string current = get_option<std::string>(key);
            if (!enum_def.get_index_of(current))
            {
                ELONA_LOG("Config key "s << key << " had invalid variant "s << current << ". "s <<
                          "("s << def.type_to_string(key) << ")"s <<
                          "Setting to "s << enum_def.get_default() << "."s);
                set_option(key, enum_def.get_default());
            }
        }
    }

    bool has_option(const std::string& key)
    {
        return storage.find(key) != storage.end() || getters.find(key) != getters.end();
    }

    template <typename T>
    T get_option(const std::string& key) const
    {
        try
        {
            if (getters.find(key) != getters.end())
            {
                return getters.at(key)().as<T>();
            }
            else
            {
                return storage.at(key).as<T>();
            }
        }
        catch (std::exception& e)
        {
            throw std::runtime_error("Error on getting config value " + key + ": " + e.what());
        }
    }

    void set_option(const std::string& key, const hcl::Value value)
    {
        ELONA_LOG("Set config option: " << key << " to " << value);

        if (!def.exists(key))
        {
            throw std::runtime_error("No such config key " + key);
        }
        if (verify_types(value, key))
        {
            if (value.is<int>())
            {
                int temp = value.as<int>();
                temp = clamp(temp, def.get_min(key), def.get_max(key));
                storage[key] = temp;
            }
            else
            {
                storage[key] = std::move(value);
            }

            if (setters.find(key) != setters.end())
            {
                setters[key](storage.at(key));
            }
        }
        else
        {
            std::stringstream ss;
            ss << "Wrong config item type for key " << key << ": ";
            ss << def.type_to_string(key) << " expected, got ";
            ss << value;
            throw std::runtime_error(ss.str());
        }
    }

    const config_def& get_def() const { return def; }

private:
    void load_defaults(bool);

    void visit(const hcl::Value&, const std::string&, const std::string&, bool);
    void visit_object(const hcl::Object&, const std::string&, const std::string&, bool);
    bool verify_types(const hcl::Value&, const std::string&);

    config_def def;
    std::map<std::string, hcl::Value> storage;
    std::map<std::string, std::function<hcl::Value(void)>> getters;
    std::map<std::string, std::function<void(const hcl::Value&)>> setters;
};



void load_config2(const fs::path& hcl_file);
void load_config(const fs::path& hcl_file);

void set_config(const std::string& key, int value);
void set_config(const std::string& key, const std::string& value);
void set_config(const std::string& key, const std::string& value1, int value2);

snail::window::fullscreen_mode_t config_get_fullscreen_mode();


} // namespace elona
