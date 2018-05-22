#pragma once

#include "lua.hpp"
#include <map>

namespace elona
{
namespace lua
{

class lua_env;

enum class event_kind_t : unsigned
{
    chara_moved,
    player_turn,
    all_turns_finished,

    map_initialized,
    character_initialized,
    item_initialized,
    game_initialized,

    map_created,
    character_created,
    item_created,

    character_removed,
    item_removed,

    COUNT // for iterating over all event kinds
};

enum class event_control_t
{
    next,
    no_process_rest,
    end_turn,
};

class callbacks
{
public:
    struct callback_t
    {
        sol::function function;
        sol::environment env;
        std::string mod_name;
        callback_t(sol::environment _env, sol::function _function, std::string _mod_name)
        {
            env = _env;
            function = _function;
            mod_name = _mod_name;
        }
    };

    template<typename> struct retval_type {};

    typedef std::vector<callback_t> callback_container;
    typedef callback_container::iterator iterator;
    typedef callback_container::const_iterator const_iterator;

    const_iterator begin() const { return functions.begin(); }
    const_iterator end() const { return functions.end(); }

    void push(sol::environment &env, sol::function &function)
    {
        std::string mod_name = env["Global"]["MOD_NAME"];
        functions.emplace_back(env, function, mod_name);
    }

    template<typename... Args>
    void run(retval_type<void>, Args&&... args)
    {
        for (const auto iter : functions)
        {
            iter.function.call(std::forward<Args>(args)...);
        }
    }

    template<typename R, typename... Args>
    void run(retval_type<R>, Args&&... args)
    {
        R retval;

        for (const auto& iter : functions)
        {
            retval = iter.function.call(std::forward<Args>(args)...);
        }

        return retval;
    }
private:
    callback_container functions;
};

class event_manager
{

public:
    static void init(lua_env&);
public:
    explicit event_manager(lua_env*);
    /***
     * Registers a new event handler from a mod's environment.
     */
    void register_event(event_kind_t, sol::environment&, sol::function&);

    /***
     * Runs all callbacks for this event in the order they were registered.
     */
    void trigger_event(event_kind_t, sol::table);

    const callbacks& get_callbacks(event_kind_t event) const
    {
        return events.at(event);
    }

    template<event_kind_t event, typename R = void, typename... Args>
    R run_callbacks(Args&&... args)
    {
        return events.at(event).run(callbacks::retval_type<R>{},
                                        std::forward<Args>(args)...);
    }

    template<event_kind_t event, typename R = void>
    R run_callbacks()
    {
        return events.at(event).run(callbacks::retval_type<R>{});
    }

    void clear();

    typedef std::unordered_map<event_kind_t, callbacks> container;
private:
    void init_events();
    container events;
    lua_env* lua;
};

} // namespace lua
} // namespace elona
