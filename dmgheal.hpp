#pragma once

#include "element.hpp"


namespace elona
{

void healhp(int cc, int delta);
void healmp(int cc, int delta);
void healsp(int cc, int delta);
int dmghp(int target, int amount, int attacker, element_t element = element_t::none, int element_power = 0);
void end_dmghp();
void dmgmp(int cc, int delta);
void dmgsp(int cc, int delta);
void healsan(int prm_862, int prm_863);
void damage_insanity(int cc, int delta);
bool actionsp(int cc, int sp);

}
