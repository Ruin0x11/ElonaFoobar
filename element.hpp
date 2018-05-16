#pragma once

#include <string>



namespace elona
{


// TODO: they shouldn't be linked to specific magic numbers.
enum class element_t
{
    none = 0,
    fire = 50,
    cold = 51,
    lightning = 52,
    darkness = 53,
    mind = 54,
    poison = 55,
    nether = 56,
    sound = 57,
    nerve = 58,
    chaos = 59,
    magic = 60,
    cut = 61,
    ether = 62,
    acid = 63,
};


int randomele();
std::string elename(int effect_id, element_t element); // TODO make arg into std::variant
int eleinfo(int prm_754, int prm_755);
void resistmod(int cc, int element, int delta);
void txteledmg(int type, int attacker, int target, int element);



} // namespace elona
