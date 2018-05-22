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

    void on_chara_creation(character& chara);
    void on_item_creation(item& item);
    void on_chara_removal(character& chara);
    void on_item_removal(item& item);

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
