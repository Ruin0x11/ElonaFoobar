#include "plant.hpp"

namespace elona
{



turn_result_t do_plant()
{
    screenupdate = -1;
    update_screen();
    if (mdata(6) == 1 || mdata(6) == 3 || mdata(6) == 2)
    {
        txt(lang(
            u8"この場所には埋められない。"s, u8"You can't plant it here."s));
        update_screen();
                        return turn_result_t::pc_turn_user_error;
    }
    if (map(cdata[cc].position.x, cdata[cc].position.y, 6) != 0)
    {
        txt(lang(
            u8"この場所には埋められない。"s, u8"You can't plant it here."s));
        update_screen();
                        return turn_result_t::pc_turn_user_error;
    }
    int val0;
    if (chipm(0, map(cdata[0].position.x, cdata[0].position.y, 0)) == 2)
    {
        val0 = 1;
    }
    else
    {
        val0 = 0;
    }
    feat(0) = tile_plant;
    feat(1) = 29;
    feat(2) = inv[ci].material;
    label_2234(val0);
    if (val0)
    {
        s = u8"畑に"s;
    }
    else
    {
        s = "";
    }
    s += lang(
        ""s + itemname(ci, 1) + u8"を埋めた。"s,
        u8"You plant "s + itemname(ci, 1));
    txt(s);
    snd(55);
    removeitem(ci, 1);
    cell_featset(
        cdata[cc].position.x,
        cdata[cc].position.y,
        tile_plant,
        29,
        inv[ci].material,
        feat(3));
    skillexp(180, 0, 300);
    return turn_result_t::turn_end;
}



void label_2233(int val0)
{
    --feat(3);
    if (feat(3) % 50 == 0)
    {
        if (feat(3) >= 50)
        {
            feat = tile_plant + 3;
        }
        else
        {
            ++feat;
            label_2234(val0);
        }
    }
    return;
}



void label_2234(int val0)
{
    feat(3) = 4 + rnd(5);
    p = 10;
    if (feat(2) == 41)
    {
        p = 15;
    }
    if (feat(2) == 42)
    {
        p = 25;
    }
    if (feat(2) == 38)
    {
        p = 30;
    }
    if (feat(2) == 39)
    {
        p = 25;
    }
    if (feat(2) == 40)
    {
        p = 40;
    }
    if (val0 == 0)
    {
        p = p * 3 / 2;
    }
    if (feat == tile_plant)
    {
        if (gdata_weather < 3)
        {
            p = p * 2;
        }
    }
    if (sdata(180, 0) < rnd(p + 1) || rnd(20) == 0)
    {
        feat(3) += 50;
    }
    return;
}



void label_2235(int val)
{
    p = 15;
    if (feat(2) == 41)
    {
        p = 25;
    }
    if (feat(2) == 42)
    {
        p = 30;
    }
    if (feat(2) == 38)
    {
        p = 40;
    }
    if (feat(2) == 39)
    {
        p = 35;
    }
    if (val == 0)
    {
        p = p * 2;
    }
    if (gdata_weather < 3)
    {
        p = p * 4 / 3;
    }
    if (sdata(180, 0) < rnd(p + 1) || rnd(5) == 0 || feat(2) == 40)
    {
        map(cdata[0].position.x, cdata[0].position.y, 6) = 0;
        return;
    }
    feat = tile_plant;
    label_2234();
    cell_featset(
        cdata[cc].position.x,
        cdata[cc].position.y,
        feat,
        feat(1),
        feat(2),
        feat(3));
    txtef(2);
    txt(lang(u8"新しい芽が息吹いている！"s, u8"A new plant grows!"s));
    return;
}

void label_2236()
{
    skillexp(180, 0, 75);
    snd(55);
    flt(sdata(180, 0) / 2 + 15, 2);
    dbid = 0;
    if (feat(2) == 39)
    {
        flttypemajor = fsetplantunknown(rnd(length(fsetplantunknown)));
        if (rnd(100) == 0)
        {
            dbid = 559;
        }
    }
    if (feat(2) == 40 || (feat(2) == 39 && rnd(50) == 0))
    {
        flttypemajor = fsetplantartifact(rnd(length(fsetplantartifact)));
        fixlv = 4;
        autosave = 1 * (gdata_current_map != 35);
    }
    if (feat(2) == 36)
    {
        flttypeminor = 57003;
    }
    if (feat(2) == 37)
    {
        flttypeminor = 57004;
    }
    if (feat(2) == 38)
    {
        flttypeminor = 58005;
    }
    if (feat(2) == 41)
    {
        flttypeminor = 77001;
    }
    if (feat(2) == 42)
    {
        flttypemajor = 56000;
    }
    itemcreate(0, dbid, -1, -1, 0);
    txt(lang(
        itemname(ci, 1) + u8"を収穫した。"s,
        u8"You harvest "s + itemname(ci, 1) + u8"."s));
    item_stack(0, ci, 1);
    return;
}




} // namespace elona
