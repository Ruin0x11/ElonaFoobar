#pragma once

#include "filesystem.hpp"

namespace elona
{

void initialize_config(const fs::path&);
void initialize_cat_db();
void initialize_elona();
void initialize_game();
void initialize_debug_globals();
int run();

}
