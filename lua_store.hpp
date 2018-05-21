#pragma once

#include "character.hpp"
#include "position.hpp"
#include "thirdparty/sol2/sol.hpp"
#include <vector>
#include <any>
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
    void set(std::string key, const sol::object &);
    sol::object get(std::string key, sol::this_state tstate);

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
private:
    /***
     * Serializes a compatible userdata object or reference (character, item or position).
     */
    object serialize_userdata(const sol::object&);

    sol::object deserialize_character(const store::object&, sol::state_view&);
    sol::object deserialize_item(const store::object&, sol::state_view&);
    sol::object deserialize_position(const store::object&, sol::state_view&);

    /***
     * Deserializes a compatible userdata object or reference (character, item or position).
     */
    sol::object deserialize_userdata(const store::object&, sol::state_view&);
private:
    std::unordered_map<std::string, std::pair<sol::type, object>> store;
};

}
}
