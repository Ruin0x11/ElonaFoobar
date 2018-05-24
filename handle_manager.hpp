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

    void clear_all_handles();
    void clear_map_local_handles();
private:
    std::set<int> chara_handles;
    std::set<int> item_handles;
    sol::environment handle_env;
    lua_env* lua;
};

}
}
