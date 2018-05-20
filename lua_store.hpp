#pragma once

#include "thirdparty/sol2/sol.hpp"
#include <vector>
#include <any>
#include <unordered_map>
#include <boost/variant.hpp>

namespace elona
{
namespace lua
{

class store
{
public:
    void set(std::string key, const sol::object &);
    sol::object get(std::string key, sol::this_state tstate);

    typedef boost::variant<int, bool, std::string, sol::table> object;

    void init(sol::state &state);
private:
    std::unordered_map<std::string, std::pair<sol::type, object>> store;
};

}
}
