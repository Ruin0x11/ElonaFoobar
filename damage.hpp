#pragma once

namespace elona
{

int dmgcon(int = 0, int = 0, int = 0);
int dmghp(int = 0, int = 0, int = 0, int = 0, int = 0);
void end_dmghp();
void dmgmp(int, int);
void damage_insanity(int, int);
void dmgsp(int, int);
void dmgplayer(int = 0, int = 0);
bool actionsp(int, int);
void damage_apply_status_effects(int);

void healcon(int = 0, int = 0, int = 0);
void healhp(int = 0, int = 0);
void healmp(int = 0, int = 0);
void healsan(int = 0, int = 0);
void healsp(int = 0, int = 0);

void get_hungry(int);
void get_pregnant();
void wet(int = 0, int = 0);

void txteledmg(int = 0, int = 0, int = 0, int = 0);

} // namespace elona
