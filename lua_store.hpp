#pragma once

#include "character.hpp"
#include "position.hpp"
#include "thirdparty/sol2/sol.hpp"
#include <vector>
#include <unordered_map>
#include <boost/variant.hpp>
#include <boost/serialization/strong_typedef.hpp>

namespace elona
{
namespace lua
{

class store
{


public:
    void clear();
    void set(std::string key, const sol::object&, sol::state_view& view);
    sol::object get(std::string key, sol::state_view& view);

    BOOST_STRONG_TYPEDEF(int, character_ref);
    BOOST_STRONG_TYPEDEF(int, item_ref);

    typedef boost::variant<int,
                           bool,
                           std::string,
                           sol::table,
                           position_t,
                           character_ref,
                           item_ref> object;

    void init(sol::state &state);
    void init(sol::state &state, sol::environment& env);
private:
    void bind(sol::state&);

    /***
     * Serializes a compatible userdata object or reference (character, item or position).
     */
    object serialize_userdata(const sol::object&);
    sol::table serialize_table(sol::state_view&, sol::table&);
    void convert_table_value(sol::object&, sol::state_view&);

    /***
     * Deserializes a compatible userdata object or reference (character, item or position).
     */
    sol::object deserialize_userdata(const store::object&, sol::state_view&);
private:
    std::unordered_map<std::string, std::pair<sol::type, object>> store_inner;
};

sol::object deserialize_character(store::character_ref, sol::state_view&);
sol::object deserialize_item(store::item_ref, sol::state_view&);
sol::object deserialize_position(position_t, sol::state_view&);
sol::object deserialize_table(sol::table&, sol::state_view&);
sol::object deserialize_table_nested_value(const sol::object&, sol::state_view&);
sol::object serialize_table_nested_value(const sol::object&, sol::state_view&);

// These are for handling nested assignments of references to custom usertypes inside tables.
void table_meta_new_index(sol::table table, sol::object key, sol::object value, sol::this_state ts);
sol::object table_meta_index(sol::table table, sol::object key, sol::this_state ts);

}
}
