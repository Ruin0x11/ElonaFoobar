#include "atx.hpp"
#include "variables.hpp"
#include "elona.hpp"
#include "ui.hpp"

namespace elona
{

atx_state atx{};

void atx_init()
{
    if (atx.through == 0)
    {
        atx.buff = "";
        notesel(atx.buff);
    }
    atx.through = 0;
    listmax = 0;
    cs = 0;
    page = 0;
    cc = 0;
    cs_bk = -1;
    pagesize = 16;
    chatesc = -1;
    if (atx.bg != atx.bgbk)
    {
        mode = 0;
        screenupdate = -1;
        update_screen();
        mode = 9;
        atx.bgbk = atx.bg;
        gsel(4);
        gmode(0);
        pos(0, 0);
        picload(filesystem::dir::graphic() / (atx.bg + u8".bmp"s), 1);
        pos(0, ui.msgh);
        gzoom(4, 0, 0, 240, 160, ui.windoww, ui.windowh - ui.verh - ui.msgh);
        gmode(2);
        p = ui.windoww / 192;
        for (int cnt = 0, cnt_end = (p + 1); cnt < cnt_end; ++cnt)
        {
            if (cnt == p)
            {
                sx = ui.windoww % 192;
            }
            else
            {
                sx = 192;
            }
            pos(cnt * 192, 0);
            gcopy(3, 496, 528, sx, ui.msgh);
        }
        window2(ui.windoww - 208, 0, 208, 98, 0, 0);
        pos(ui.windoww - 204, 4);
        gzoom(0, 120, 88, ui.windoww - 120, ui.windowh - ui.verh - 112, 200, 90);
        gsel(0);
    }
    return;
}


}
