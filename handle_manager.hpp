#pragma once

#include "lua.hpp"
#include "character.hpp"
#include "item.hpp"
#include <set>

namespace elona
{
namespace lua
{

class lua_env;

class handle_manager
{
public:
    explicit handle_manager(lua_env*);

    void create_chara_handle(character& chara);
    void create_item_handle(item& item);
    void remove_chara_handle(character& chara);
    void remove_item_handle(item& item);

    sol::object get_chara_handle(character& chara);
    sol::object get_item_handle(item& item);

    void clear();
private:
    std::set<int> chara_handles; // TODO make into explicit strong type
    std::set<int> item_handles; // TODO make into explicit strong type
    sol::environment handle_env;
    lua_env* lua;
};

}
}
