#include "lua_store.hpp"

#include "thirdparty/sol2/sol.hpp"
#include <vector>
#include <any>
#include <unordered_map>
#include <boost/variant.hpp>
#include <cassert>

namespace elona
{
namespace lua
{

void store::init(sol::state &state)
{
    if(state["Store"].valid()) // already exists
        return;

    sol::table Store = state.create_table("Store");

    Store.set_function("set", [this](std::string key, const sol::object &val){
        set(key, val);
    });

    Store.set_function("get", [this](std::string key, sol::this_state tstate){
        return get(key, tstate);
    });
}


void store::set(std::string key, const sol::object &val)
{
    store::object obj;
    auto type = val.get_type();
    switch(type)
    {
        case sol::type::none:break;
        case sol::type::lua_nil:break;
        case sol::type::string:
            obj = val.as<std::string>();
            break;
        case sol::type::number:
            obj = val.as<int>();
            break;
        case sol::type::thread:break;
        case sol::type::boolean:
            obj = val.as<bool>();
            break;
        case sol::type::function:break;
        case sol::type::userdata:break;
        case sol::type::lightuserdata:break;
        case sol::type::poly:break;
        case sol::type::table:
            obj = val;
            break;
    }
    store[key.data()] = {type, obj};
}

sol::object store::get(std::string key, sol::this_state tstate)
{
    sol::state_view view(tstate);
    auto val = store.find(key.data());
    if (val == store.end())
        return sol::nil;

    auto &[type, obj] = val->second;

    switch(type)
    {
        case sol::type::none:break;
        case sol::type::lua_nil:break;
        case sol::type::string:
            assert(obj.type() == typeid(std::string));
            return sol::object(view, sol::in_place, boost::get<std::string>(obj));
        case sol::type::number:
            assert(obj.type() == typeid(int));
            return sol::make_object(view, boost::get<int>(obj));
        case sol::type::thread:break;
        case sol::type::boolean:
            assert(obj.type() == typeid(bool));
            return sol::make_object(view, boost::get<bool>(obj));
        case sol::type::function:break;
        case sol::type::userdata:break;
        case sol::type::lightuserdata:break;
        case sol::type::poly:break;
        case sol::type::table:
            assert(obj.type() == typeid(sol::table));
            return boost::get<sol::table>(obj);
    }
    return sol::nil;
}

}
}
