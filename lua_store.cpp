#include "lua_store.hpp"

#include "character.hpp"
#include "item.hpp"
#include "optional.hpp"
#include "position.hpp"
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

void store::init(sol::state& state)
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

void store::init(sol::state& state, sol::environment& env)
{
    if(env["Store"].valid()) // already exists
        return;

    sol::table Store = state.create_table("Store");

    Store.set_function("set", [this](std::string key, const sol::object &val){
        set(key, val);
    });

    Store.set_function("get", [this](std::string key, sol::this_state tstate){
        return get(key, tstate);
    });

    env["Store"] = Store;
}


store::object store::serialize_userdata(const sol::object &val)
{
    store::object obj = sol::nil;
    if(val.is<character&>())
    {
        character& chara = val.as<character&>();
        if(chara.idx == -1 || chara.state == 0)
        {
            obj = sol::nil;
        }
        else
        {
            obj = character_ref(chara.idx);
        }
    }
    else if(val.is<item&>())
    {
        item& i = val.as<item&>();
        if(i.idx == -1 || i.number == 0)
        {
            obj = sol::nil;
        }
        else
        {
            obj = item_ref(i.idx);
        }
    }
    else if(val.is<position_t&>())
    {
        position_t pos = val.as<position_t>();
        obj = pos;
    }
    else
    {
        assert(0);
    }
    return obj;
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
    case sol::type::userdata:
        obj = serialize_userdata(val);
        break;
    case sol::type::lightuserdata:break;
    case sol::type::poly:break;
    case sol::type::table:
        obj = val;
        break;
    }
    store[key.data()] = {type, obj};
}

sol::object store::deserialize_character(const store::object& obj, sol::state_view& view)
{
    character_ref idx = boost::get<character_ref>(obj);
    assert(idx != -1);

    // Check to make sure this reference is still valid.
    if(elona::cdata(idx).state == 0)
    {
        return sol::nil;
    }

    return sol::make_reference(view, elona::cdata(static_cast<int>(idx)));
}

sol::object store::deserialize_item(const store::object& obj, sol::state_view& view)
{
    item_ref idx = boost::get<item_ref>(obj);
    assert(idx != -1);

    // Check to make sure this reference is still valid.
    if(elona::inv(idx).number == 0)
    {
        return sol::nil;
    }

    return sol::make_reference(view, elona::inv(static_cast<int>(idx)));
}

sol::object store::deserialize_position(const store::object& obj, sol::state_view& view)
{
    position_t pos = boost::get<position_t>(obj);
    return sol::make_object(view, pos);
}

sol::object store::deserialize_userdata(const store::object& obj, sol::state_view& view)
{
    if (obj.type() == typeid(character_ref)) {
        return deserialize_character(obj, view);
    }
    else if (obj.type() == typeid(store::item_ref))
    {
        return deserialize_item(obj, view);
    }
    else if (obj.type() == typeid(position_t))
    {
        return deserialize_position(obj, view);
    }
    else {
        assert(0);
        return sol::nil;
    }
}

sol::object store::get(std::string key, sol::this_state tstate)
{
    sol::state_view view(tstate);
    auto val = store.find(key.data());
    if (val == store.end())
        return sol::nil;

    const auto& pair = val->second;
    const auto& type = pair.first;
    const auto& obj = pair.second;

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
    case sol::type::userdata:
        return deserialize_userdata(obj, view);
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
