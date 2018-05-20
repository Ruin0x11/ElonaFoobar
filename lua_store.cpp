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


store::object serialize_userdata(const sol::object &val)
{
    store::object obj;
    if(val.is<character&>())
    {
        character& chara = val.as<character&>();
        assert(chara.idx != -1);
        assert(chara.state != 0);
        obj = store::character_ref(chara.idx);
    }
    else if(val.is<item&>())
    {
        item& i = val.as<item&>();
        assert(i.idx != -1);
        assert(i.number != 0);
        obj = store::item_ref(i.idx);
    }
    else if(val.is<position_t>())
    {
        assert(0);
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
        if (obj.type() == typeid(character_ref)) {
            character_ref idx = boost::get<character_ref>(obj);
            assert(idx != -1);

            // Check to make sure this reference is still valid.
            if(elona::cdata(idx).state == 0)
            {
                return sol::nil;
            }

            return sol::make_reference(view, elona::cdata(static_cast<int>(idx)));
        }
        else if (obj.type() == typeid(item_ref))
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
        else {
            assert(0);
        }
        break;
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
