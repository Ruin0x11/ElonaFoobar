#pragma once
#include <string>
#include <unordered_map>
#include "data/types/type_map_tileset.hpp"

namespace elona
{

void map_initialize();
void map_initcustom(const std::string&);
void map_tileset(int = 0);
void map_converttile();
void map_createroomdoor();
void map_makedoor();
void map_nextdir1(int = 0, int = 0);
void map_nextdir2(int = 0, int = 0);
void map_placearena(int chara_index, bool is_enemy);
void map_placecharaonentrance(int chara_index, int entrance_type);
int dist_town();
void map_placeplayer();
void map_randomtile(int = 0, int = 0);
void map_setfog(int = 0, int = 0);

void generate_debug_map();
void generate_random_nefia();
int initialize_quest_map_crop();
int initialize_random_nefia_rdtype1();
int initialize_random_nefia_rdtype4();
int initialize_random_nefia_rdtype5();
int initialize_random_nefia_rdtype2();
int initialize_random_nefia_rdtype3();
int initialize_quest_map_party();
void initialize_home_mdata();

struct Tileset
{
    int tile(const std::string& name);
    int random(const std::string& name);
    void set_chance(const std::string& name, int tile, int count);

    SharedId current_id;
    std::unordered_map<std::string, MapTile> raw_tiles;
};

extern Tileset tileset;

} // namespace elona
