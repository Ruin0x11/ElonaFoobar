#pragma once

#include "filesystem.hpp"



namespace elona
{


enum class file_operation_t
{
    map_read,
    map_write,
    custom_map_read,
    custom_map_write,
    save_game_delete,
    temp_dir_delete,
    map_delete,
    map_delete_preserve_items,
    temp_dir_delete_area,
    gene_write,
    gene_read,
    map_load_map_obj_files,
    map_home_upgrade,
};



enum class file_operation2_t
{
    map_items_read,
    map_items_write,
    global_read,
    global_write,
    deck_write,
    deck_read,
};


void ctrl_file(file_operation_t file_operation);
void ctrl_file(file_operation2_t file_operation, const fs::path& filepath);
void fileadd(const fs::path& filepath, int prm_693 = 0);


} // namespace elona
