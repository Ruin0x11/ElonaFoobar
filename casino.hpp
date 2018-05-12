#pragma once
#include "elona.hpp"

namespace elona
{

// TODO nothing here can be const since elona_vectorX auto resizes
// But just std::vector could be used anyway since the array sizes are known
struct card_state
{
    elona_vector2<int> card_player;
    int card_player_max; // TODO unnecessary, use vector.size()
    int card_holder_max;
    int pilex;
    int piley;
    elona_vector2<int> card;
    int card_max;
    bool no_joker;
    int dx;
    int dy;
};



void casino_dealer();
void casino_choose_card();
void casino_something_a();
void casino_something_b();
void casino_something_c();
void casino_acquire_items();
void casino_random_site();
void casino_search();
void casino_wrapper();
bool casino_start();
bool casino_blackjack();

void card_player_init(card_state&);
void card_player_add(card_state&, int, int, int);
card_state card_init(int, int, int = 0);
void card_show2(card_state&, int, int = 0);
void card_show_pile(card_state&);
void card_show(card_state&);
int card_serve(card_state&, int);
void card_showholder(card_state&);
int card_open2(card_state&, int, int = 0);
int trashcard(card_state&, int);
int card_cp_score(card_state&, int);
int card_last_index(card_state&, int);
int card_cp_black_card(card_state&, int);
int card_cp_num(card_state&, int);
int card_pile_remain(card_state&);


} // namespace elona

