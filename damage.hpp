#pragma once

namespace elona
{

int damage_hp(int, int, int = 0, int = 0, int = 0);
void damage_status_effect_resist();
void end_damage_hp(int, int);
void damage_mp(int, int);
void damage_stamina(int, int);
int damage_status_effect(int = 0, int = 0, int = 0);
void wet(int = 0, int = 0);
void damage_sanity(int, int);

void heal_status_effect(int = 0, int = 0, int = 0);
void heal_hp(int = 0, int = 0);
void heal_mp(int = 0, int = 0);
void heal_sanity(int = 0, int = 0);
void heal_stamina(int = 0, int = 0);

void modify_corruption(int = 0);

void add_resistance(int = 0, int = 0, int = 0);
}
