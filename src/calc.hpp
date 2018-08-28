#pragma once

#include <string>
#include "enums.hpp"
#include "optional.hpp"


namespace elona
{

struct character;

int calc_buff_duration(int id, int power);
std::string get_buff_description(int id, int power);
void apply_buff(int cc, int id, int power);

struct skill_damage
{
    int dice_x;
    int dice_y;
    int damage_bonus;
    int element;
    int element_power;
};
optional<skill_damage> calc_skill_damage(int, int, int);
int calcobjlv(int = 0);
int calcfixlv(int = 0);
int calcfame(int = 0, int = 0);
int decfame(int = 0, int = 0);
int calcshopreform();
int calc_rate_to_pierce(int);
std::string calcage(int);
int calcexpalive(int = 0);
int calc_evasion(int cc);
int calc_accuracy(bool consider_distance);
int calcattackhit();
int calcattackdmg(int = 0);
int calcmedalvalue(int = 0);
int calcitemvalue(int = 0, int = 0);
int calcinvestvalue();
int calcguiltvalue();
int calchireadv(int = 0);
int calchirecost(int = 0);
void generatemoney(int = 0);
void calccosthire();
int calccostbuilding();
int calccosttax();
int calcmealvalue();
int calccostreload(int, bool = false);
int calccargoupdate();
int calccargoupdatecost();
int calcidentifyvalue(int);
int calctraincost(int, int, bool = false);
int calclearncost(int, int, bool = false);
int calcresurrectvalue(int = 0);
int calcslavevalue(int = 0);
int calcrestorecost();
int calcinitgold(int);
int calcspellpower(int = 0, int = 0);
int calcspellfail(int = 0, int = 0);
int calcspellcostmp(int = 0, int = 0);
int calcspellcoststock(int = 0, int = 0);
int calcscore();
void calcpartyscore();
void calcpartyscore2();

int generate_color(color_index_t index, int id);


/**
 * Calculates the amount of gold earned from performing. It can be
 * greater than the listener's held gold, but in that case it will be
 * clamped later.
 *
 * @param performer The performing character
 * @param listener The listening character
 *
 * @return The gold amount earned
 */
int calc_performance_gold_earned(
    const character& performer,
    const character& listener);

/**
 * Returns true if a performance's quality changes based on the listener.
 */
bool calc_performance_quality_chance(
    const character& performer,
    const character& listener);

/**
 * Calculates the change in performance quality.
 */
int calc_performance_quality_amount(const character& listener);

/**
 * Returns true if a performance interests a listener.
 */
bool calc_performance_interest_chance(
    const character& performer,
    const character& listener);

/**
 * Returns true if a performance listener throws an item.
 */
bool calc_performance_item_chance(int performtips);

/**
 * Calculates the quality of performance after finishing.
 */
int calc_performance_extra_quality(int base_quality, int instrument_quality);

int calc_skill_exp_gain_performance(
    const character& chara,
    int performance_quality);

} // namespace elona
