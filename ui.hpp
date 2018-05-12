#pragma once

namespace elona
{

struct ui_state
{
    int clockw;
    int clockh;
    int clockx;
    int clocky;
    int clockarrowx;
    int clockarrowy;
    int barh;
    int msgh;
    int ver;
    int verh;
    int very;
    int msgline;
    int raderx;
    int radery;
    int raderw;
    int raderh;
    int screenx;
    int screeny;
    int screenw;
    int screenh;
    int tiles;
    int bary;
    int msgy;
    int maxmsglen;
    int maxlog;
    int scposy;
    int hpx;
    int hpy;
    int mpx;
    int mpy;
    int windowx;
    int windowy;
    int windoww;
    int windowh;
    int msgx;
    int mesfont;
    int msgspace;
    int screenmsgy;
    int prompty;
    int promptx;
    int vertype;
};

extern ui_state ui;

void ui_init();

}
