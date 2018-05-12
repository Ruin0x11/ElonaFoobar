#pragma once
#include "elona.hpp"


namespace elona
{

struct atx_state
{

    int through; // GUI, casino only
    std::string bg; // GUI, casino only
    std::string bgbk; // GUI, casino only
    elona_vector1<int> pic; // GUI, casino only
    elona_vector1<std::string> infon; // GUI
    elona_vector1<int> id; // GUI
    int msgfix; // GUI
    elona_vector1<int> mattile; // GUI
    int ap;
    int lv; // TODO move to material?
    int screenupdate;
    std::string buff;
    int spot; // TODO move to material?

};

extern atx_state atx;

void atx_init();

}
