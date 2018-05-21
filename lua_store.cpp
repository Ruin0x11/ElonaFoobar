#include "lua_store.hpp"

#include "character.hpp"
#include "item.hpp"
#include "optional.hpp"
#include "position.hpp"
#include "thirdparty/sol2/sol.hpp"
#include <vector>
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
    bind(state, Store);
    state["Store"] = this;
}

void store::init(sol::state& state, sol::environment& env)
{
    sol::table Store = state.create_table_with();
    bind(state, Store);
    // This doesn't point to the store correctly unless it is set inside mod_info.
    // I don't know why.
    //env["Store"] = this;
}

void store::bind(sol::state& state, sol::table& Store)
{
    //sol::table metatable = state.create_table_with();

    state.new_usertype<lua::store>("LuaStore",
                                   sol::meta_function::new_index , [](lua::store& s, std::string key, const sol::object val, sol::this_state tstate){
        sol::state_view view(tstate);
        s.set(key, val, view);
                                   },
                                   sol::meta_function::index , [](lua::store& s, std::string key, sol::this_state tstate) {
        sol::state_view view(tstate);
        return s.get(key, view);
    });

    // metatable[sol::meta_function::new_index] = [this](sol::table table, std::string key, const sol::object val, sol::this_state tstate){
    //     sol::state_view view(tstate);
    //     set(key, val, view);
    // };

    // metatable[sol::meta_function::index] = [this](sol::table table, std::string key, sol::this_state tstate) {
    //     sol::state_view view(tstate);
    //     return get(key, view);
    // };

    state.new_usertype<character_ref>( "LuaCharacterRef" );
    state.new_usertype<item_ref>( "LuaItemRef" );

    //Store[sol::metatable_key] = metatable;
}

void store::set(std::string key, const sol::object &val, sol::state_view& view)
{
    std::cout << "Set: " << key << std::endl;
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
        obj = val.as<sol::table>();
        break;
    }
    store[key.data()] = {type, obj};
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

void store::convert_table_value(sol::object& value, sol::state_view& view)
{
    sol::type valuetype = value.get_type();
    switch (valuetype) {
    case sol::type::number:
    case sol::type::string:
    case sol::type::boolean:
    case sol::type::lua_nil:
        break;
    case sol::type::userdata:
    {
        store::object obj = serialize_userdata(value);
        if(obj.type() == typeid(character_ref))
        {
            value = sol::make_object(view, boost::get<character_ref>(obj));
        }
        else if(obj.type() == typeid(item_ref))
        {
            value = sol::make_object(view, boost::get<item_ref>(obj));
        }
        else if(obj.type() == typeid(position_t))
        {
            // do nothing
        }
        else
        {
            assert(0);
        }
    }
    break;
    case sol::type::table:
    {
        sol::table t = value;
        value = serialize_table(view, t);
    }
    break;
    default:
        // The user tried storing an unsupported value.
        assert(0);
        break;
    }
}

sol::table store::serialize_table(sol::state_view& view, sol::table& table)
{
    auto fx = [&](sol::object key, sol::object value) {
        convert_table_value(key, view);
        convert_table_value(value, view);
    };
    table.for_each(fx);
    return table;
}

sol::object store::get(std::string key, sol::state_view& view)
{
    std::cout << "Get: " << key << std::endl;
    std::cout << key.data() << std::endl;
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
        // set metatable here
        return boost::get<sol::table>(obj);
    }
    return sol::nil;
}

sol::object deserialize_character(store::character_ref idx, sol::state_view& view)
{
    assert(idx != -1);

    // Check to make sure this reference is still valid.
    if(elona::cdata(idx).state == 0)
    {
        return sol::nil;
    }

    return sol::make_reference(view, elona::cdata(static_cast<int>(idx)));
}

sol::object deserialize_item(store::item_ref idx, sol::state_view& view)
{
    assert(idx != -1);

    // Check to make sure this reference is still valid.
    if(elona::inv(idx).number == 0)
    {
        return sol::nil;
    }

    return sol::make_reference(view, elona::inv(static_cast<int>(idx)));
}

sol::object deserialize_position(const position_t pos, sol::state_view& view)
{
    return sol::make_object(view, pos);
}

sol::object store::deserialize_userdata(const store::object& obj, sol::state_view& view)
{
    if (obj.type() == typeid(character_ref)) {
        return deserialize_character(boost::get<character_ref>(obj), view);
    }
    else if (obj.type() == typeid(item_ref))
    {
        return deserialize_item(boost::get<item_ref>(obj), view);
    }
    else if (obj.type() == typeid(position_t))
    {
        return deserialize_position(boost::get<position_t>(obj), view);
    }
    else {
        assert(0);
        return sol::nil;
    }
}

} // namespace lua
} // namespace elona
