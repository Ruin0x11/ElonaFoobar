#pragma once
#include "lua_api_common.hpp"

namespace elona
{
enum class TileKind;

namespace lua
{

namespace Map
{
int width();

int height();

int id();

bool is_overworld();

bool valid(const Position&);
bool valid_xy(int, int);

bool is_solid(const Position&);
bool is_solid_xy(int, int);

bool is_blocked(const Position&);
bool is_blocked_xy(int, int);

Position bound_within(const Position&);

Position random_pos();

int generate_tile(const EnumString&);

int get_tile(const Position&);
int get_tile_xy(int, int);

int get_memory(const Position&);
int get_memory_xy(int, int);

int get_feat(const Position&);
int get_feat_xy(int, int);

int get_mef(const Position&);
int get_mef_xy(int, int);

sol::optional<LuaCharacterHandle> get_chara(const Position&);
sol::optional<LuaCharacterHandle> get_chara_xy(int, int);

void set_tile(const Position&, int);
void set_tile_xy(int, int, int);

void set_memory(const Position&, int);
void set_memory_xy(int, int, int);


void bind(sol::table&);
} // namespace Map

} // namespace lua
} // namespace elona
