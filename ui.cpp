#include "ui.hpp"

namespace elona
{

ui_state ui{};

void ui_init()
{
    ui.clockarrowx = ui.clockx + 62;
    ui.clockarrowy = ui.clocky + 48;
    ui.barh = 16;
    ui.msgh = 72;
    ui.verh = ui.barh + ui.msgh;
    ui.msgline = 4;
    ui.raderx = 1;
    ui.raderw = 136;
    ui.screenw = ui.windoww / ui.tiles + (ui.windoww % ui.tiles != 0);
    if (ui.windowh > 768)
    {
        ui.windowh = 768;
    }
    ui.screenh = (ui.windowh - ui.verh) / ui.tiles;
    if ((ui.windowh - ui.verh) % ui.tiles != 0)
    {
        ++ui.screenh;
    }
    if (ui.vertype == 0)
    {
        ui.ver = 0;
        ui.bary = 0;
        ui.msgy = ui.ver + ui.barh;
        ui.screeny = ui.verh;
        ui.clocky = ui.windowh - ui.clockh;
        ui.radery = 1;
    }
    else
    {
        ui.screeny = 0;
        if ((ui.windowh - ui.verh) % ui.tiles != 0)
        {
            ui.screeny = 0 - ui.tiles + (ui.windowh - ui.verh) % ui.tiles;
        }
        ui.ver = ui.windowh - ui.verh;
        ui.bary = ui.windowh - ui.barh;
        ui.msgy = ui.ver;
        ui.clocky = 0;
        ui.radery = ui.windowh - 86;
    }
    ui.scposy = ui.screenh / 2 - 1;
    ui.hpx = (ui.windoww - 84) / 2 - 100;
    ui.hpy = ui.ver - 12;
    ui.mpx = (ui.windoww - 84) / 2 + 40;
    ui.mpy = ui.ver - 12;
    ui.msgx = ui.raderw;
    ui.msgspace = 15;
    ui.maxmsglen = (ui.windoww - ui.msgx - 28) / ui.mesfont * 2 - 1;
    ui.maxlog = (ui.msgy - 100) / ui.msgspace + 3;
    ui.very = ui.windowh - ui.verh;
    ui.screenmsgy = ui.screeny + ui.tiles * 2;
    ui.prompty = (ui.windowh - ui.verh - 30) / 2 - 4;
    ui.promptx = (ui.windoww - 10) / 2 + 3;
    return;
}

}
