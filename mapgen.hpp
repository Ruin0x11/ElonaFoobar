#pragma once
#include <string>

namespace elona
{

void map_initialize();
void map_initcustom(const std::string&);
void map_tileset(int = 0);
void map_converttile();
void map_createroomdoor();
void map_initcustom(const std::string&);
void map_makedoor();
void map_nextdir1(int = 0, int = 0);
void map_nextdir2(int = 0, int = 0);
void map_placearena(int = 0, int = 0);
void map_placecharaonentrance(int = 0, int = 0, int = 0);
int dist_town();
void map_placeplayer();
void map_randomtile(int = 0, int = 0);
void map_randsite(int = 0, int = 0);
void map_setfog(int = 0, int = 0);

void initialize_cell_object_data();
int initialize_quest_map_crop();
int initialize_quest_map_party();
int initialize_random_nefia_rdtype1();
int initialize_random_nefia_rdtype2();
int initialize_random_nefia_rdtype3();
int initialize_random_nefia_rdtype4();
int initialize_random_nefia_rdtype5();
void generate_random_nefia();
void initialize_home_mdata();
void initialize_quest_map_town();
void initialize_random_nefia_rdtype10();
void initialize_random_nefia_rdtype6();
void initialize_random_nefia_rdtype8();
void initialize_random_nefia_rdtype9();
void label_1709();

}
