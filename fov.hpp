#pragma once

namespace elona
{

struct position_t;

void initialize_fovmap_and_fovlist();

// Returns wheather the PC can see  the position or the character.
bool fov_player_sees(const position_t&);

bool fov_player_sees(int cc);
int fov_los(int = 0, int = 0, int = 0, int = 0);
int get_route(int = 0, int = 0, int = 0, int = 0);

}
