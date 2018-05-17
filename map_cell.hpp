#pragma once

#include "elona.hpp"
#include <unordered_map>

namespace elona
{

struct position_t;

// Maps from an enumeration to an ID in the current tileset.
enum class tile_type_t
{
    normal,
    wall,
    tunnel,
    room,
    fog
};

std::pair<int, int> cell_itemoncell(const position_t& pos);
int cell_featread(int = 0, int = 0, int = 0);
int cell_findspace(int = 0, int = 0, int = 0);
int cell_itemlist(int = 0, int = 0);
void cell_check(int = 0, int = 0);
void cell_featclear(int = 0, int = 0);
void cell_featset(int = 0, int = 0, int = 0, int = 0, int = 0, int = 0);
void cell_movechara(int = 0, int = 0, int = 0);
void cell_refresh(int = 0, int = 0);
void cell_removechara(int = 0, int = 0);
void cell_setchara(int = 0, int = 0, int = 0);
void cell_swap(int = 0, int = 0, int = 0, int = 0);

/**
 * \param type tile type to generate
 * \return a random tile of the provided type
 *
 * Generates a random tile ID to be placed in the current map.
 * The ID is only valid until the tileset is changed.
 * Tilesets can have multiple variations for the same tile type.
 */
int cell_get_type(tile_type_t type);

} // namespace elona
