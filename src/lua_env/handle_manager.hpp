#pragma once

#include <set>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "../character.hpp"
#include "../item.hpp"
#include "../lib/noncopyable.hpp"
#include "lua_env.hpp"

namespace elona
{
namespace lua
{

class lua_env;

/***
 * Provides and manages references to C++ objects as Lua tables
 * (handles) that can be checked for validity on access. Works around
 * the problem of users storing references to C++ things somewhere and
 * forgetting to check they're still valid.
 *
 * The handles are represented as Lua tables stored in an isolated Lua
 * environment. The handle manager acts as the interface for providing
 * references to the handles to other Lua environments.
 *
 * See mods/core/handle.lua for more information.
 */
class handle_manager : public lib::noncopyable
{
public:
    explicit handle_manager(lua_env*);

    /***
     * Creates a new handle in the isolated handle environment.
     *
     * If the handle already exists, handle_set is instead checked for
     * validity.
     */
    void create_chara_handle(character& chara);
    void create_item_handle(item& item);

    /***
     * Removes an existing handle in the isolated handle environment.
     *
     * If the handle doesn't exist in this manager's handle list, handle_set
     * is checked that the handle is invalid.
     */
    void remove_chara_handle(character& chara);
    void remove_item_handle(item& item);


    /***
     * Methods that will create/remove handles as well as run
     * creation/removal event callbacks using the event manager
     * instance.
     */
    void create_chara_handle_run_callbacks(character&);
    void create_item_handle_run_callbacks(item&);
    void remove_chara_handle_run_callbacks(character&);
    void remove_item_handle_run_callbacks(item&);


    /***
     * Looks up the C++ reference that a handle points to. Will throw
     * if the handle is invalid or of the wrong type.
     */
    template <typename T>
    T& get_ref(sol::table handle)
    {
        sol::object obj =
            handle_env["Handle"]["get_ref"](handle, T::lua_type());
        return obj.as<T&>();
    }

    template <typename T>
    bool handle_is(sol::table handle)
    {
        return handle["kind"] == T::lua_type();
    }

    bool handle_is_valid(sol::table handle)
    {
        return handle_env["Handle"]["is_valid"](handle);
    }


    /***
     * Provides a Lua reference to a handle from the isolated handle
     * environment.
     */
    template <typename T>
    sol::table get_handle(T& obj)
    {
        if (obj.index == -1)
        {
            return sol::lua_nil;
        }

        if (handles.find(T::lua_type()) == handles.end())
        {
            return sol::lua_nil;
        }

        auto& handle_set = handles.at(T::lua_type());
        if (handle_set.find(obj.index) == handle_set.end())
        {
            // std::cout << "Handle " << obj.index << " not found." <<
            // std::endl;
            return sol::lua_nil;
        }

        // NOTE: currently indexes by the object's integer ID, but
        // this may be phased out in the future.
        sol::table handle =
            handle_env["Handle"]["get_handle"](obj, T::lua_type());
        return handle;
    }

    void clear_all_handles();

    /***
     * Clears handles that are local to the current map without
     * running removal callbacks. Player and party character/item
     * handles are not cleared.
     */
    void clear_map_local_handles();

private:
    template <typename T>
    void create_handle(T& obj)
    {
        if (handles.find(T::lua_type()) == handles.end())
        {
            handles.emplace(T::lua_type(), std::set<int>());
        }

        auto& handle_set = handles.at(T::lua_type());

        if (handle_set.find(obj.index) != handle_set.end())
        {
            handle_env["Handle"]["assert_valid"](obj, T::lua_type());
            return;
        }
        handle_set.emplace(obj.index);

        std::string uuid = boost::lexical_cast<std::string>(uuid_generator());
        handle_env["Handle"]["create_handle"](obj, T::lua_type(), uuid);
    }

    template <typename T>
    void remove_handle(T& obj)
    {
        if (handles.find(T::lua_type()) == handles.end())
        {
            handles.emplace(T::lua_type(), std::set<int>());
        }

        auto& handle_set = handles.at(T::lua_type());

        if (handle_set.find(obj.index) == handle_set.end())
        {
            handle_env["Handle"]["assert_invalid"](obj, T::lua_type());
            return;
        }
        handle_set.erase(obj.index);
        handle_env["Handle"]["remove_handle"](obj, T::lua_type());
    }

    void bind(lua_env&);

    std::unordered_map<std::string, std::set<int>> handles;
    boost::uuids::random_generator uuid_generator;

    /***
     * The isolated Lua environment where the handles are stored and
     * managed.
     */
    sol::environment handle_env;

    lua_env* lua;
};

} // namespace lua
} // namespace elona
