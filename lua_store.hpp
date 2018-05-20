#pragma once

#include "character.hpp"
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

enum class serial_t
{
    position,
    character,
    item,
};

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
                           character_ref,
                           item_ref> object;

    void init(sol::state &state);
private:
    std::unordered_map<std::string, std::pair<sol::type, object>> store;
};

}
}
