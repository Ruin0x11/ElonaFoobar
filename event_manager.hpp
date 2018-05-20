#pragma once

#include "lua.hpp"
#include <map>

namespace elona
{
namespace lua
{

class lua_env;

enum class event_kind_t
{
    chara_moved,
    all_turns_finished,
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
        callback_t(sol::environment _env, sol::function _function)
        {
            env = _env;
            function = _function;
        }
    };

    template<typename> struct retval_type {};

    typedef std::vector<callback_t> callback_container;
    typedef callback_container::iterator iterator;
    typedef callback_container::const_iterator const_iterator;

    template<typename... Args>
    void run_all(retval_type<void>, Args&&... args)
    {
        for (const auto it : functions)
        {
            it.function.call(std::forward<Args>(args)...);
        }
    }

    template<typename R, typename... Args>
    void run_all(retval_type<R>, Args&&... args)
    {
        R retval;

        for (const auto& it : functions)
        {
            retval = it.function.call(std::forward<Args>(args)...);
        }

        return retval;
    }
private:
    callback_container functions;
};

class event_manager
{

public:
    static void init(lua_env& lua);
public:
    explicit event_manager(lua_env *lua_env);
    /***
     * Registers a new event handler from a mod's environment.
     */
    void register_event(event_kind_t, sol::environment&, sol::function&);

    /***
     * Runs all callbacks for this event in the order they were registered.
     *
     * Returns true if the current character's turn needs to end.
     */
    bool trigger_event(event_kind_t, sol::table, const std::string);

    template<event_kind_t event, typename R = void, typename... Args>
    R run_all(Args&&... args)
    {
        return events.at(event).run_all(callbacks::retval_type<R>{},
                                        std::forward<Args>(args)...);
    }

    template<event_kind_t event, typename R = void>
    R run_all()
    {
        return events.at(event).run_all(callbacks::retval_type<R>{});
    }

    typedef std::unordered_map<event_kind_t, callbacks> container;
private:
    container events;
    std::weak_ptr<lua_env> lua_p;
};

} // namespace lua
} // namespace elona
