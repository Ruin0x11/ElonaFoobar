#include "map_cell.hpp"
#include "character.hpp"
#include "elona.hpp"
#include "item.hpp"
#include "map.hpp"
#include "variables.hpp"

namespace elona
{

int tc_at_m81 = 0;

void cell_featset(
    int x,
    int y,
    int id,
    int extra_info_1,
    int extra_info_2,
    int extra_info_3)
{
    elona_vector1<int> feat_at_m80;
    if (id != -1)
    {
        feat_at_m80 = id;
    }
    else
    {
        feat_at_m80 = cell_data.at(x, y).feats % 1000;
    }
    if (extra_info_1 != -1)
    {
        feat_at_m80(1) = extra_info_1;
    }
    else
    {
        feat_at_m80(1) = cell_data.at(x, y).feats / 1000 % 100;
    }
    if (extra_info_2 != -1)
    {
        feat_at_m80(2) = extra_info_2;
    }
    else
    {
        feat_at_m80(2) = cell_data.at(x, y).feats / 100000 % 100;
    }
    if (extra_info_3 != -1)
    {
        feat_at_m80(3) = extra_info_3;
    }
    else
    {
        feat_at_m80(3) = cell_data.at(x, y).feats / 10000000;
    }
    cell_data.at(x, y).feats = feat_at_m80 + feat_at_m80(1) * 1000 +
        feat_at_m80(2) * 100000 + feat_at_m80(3) * 10000000;
}



int cell_featread(int prm_598, int prm_599, int)
{
    feat(0) = cell_data.at(prm_598, prm_599).feats % 1000;
    feat(1) = cell_data.at(prm_598, prm_599).feats / 1000 % 100;
    feat(2) = cell_data.at(prm_598, prm_599).feats / 100000 % 100;
    feat(3) = cell_data.at(prm_598, prm_599).feats / 10000000;
    return 0;
}



void cell_featclear(int prm_601, int prm_602)
{
    cell_data.at(prm_601, prm_602).feats = 0;
}



void cell_check(int prm_603, int prm_604)
{
    cellaccess = 1;
    cellchara = -1;
    cellfeat = -1;
    if (prm_603 < 0 || prm_603 >= map_data.width || prm_604 < 0 ||
        prm_604 >= map_data.height)
    {
        cellaccess = 0;
        return;
    }
    if (cell_data.at(prm_603, prm_604).chara_index_plus_one != 0)
    {
        cellchara = cell_data.at(prm_603, prm_604).chara_index_plus_one - 1;
        cellaccess = 0;
    }
    if (cell_data.at(prm_603, prm_604).feats != 0)
    {
        cellfeat = cell_data.at(prm_603, prm_604).feats / 1000 % 100;
        if (chipm(7, cell_data.at(prm_603, prm_604).feats % 1000) & 4)
        {
            cellaccess = 0;
        }
    }
    if (chipm(7, cell_data.at(prm_603, prm_604).chip_id_actual) & 4)
    {
        cellaccess = 0;
    }
}



bool cell_swap(int prm_605, int prm_606, int prm_607, int prm_608)
{
    int x2_at_m81 = 0;
    int y2_at_m81 = 0;
    if (game_data.mount != 0)
    {
        if (game_data.mount == prm_605 || game_data.mount == prm_606)
        {
            return false;
        }
    }
    tc_at_m81 = prm_606;
    if (tc_at_m81 == -1)
    {
        if (cell_data.at(prm_607, prm_608).chara_index_plus_one != 0)
        {
            tc_at_m81 = cell_data.at(prm_607, prm_608).chara_index_plus_one - 1;
        }
    }
    if (tc_at_m81 != -1)
    {
        cell_data.at(cdata[prm_605].position.x, cdata[prm_605].position.y)
            .chara_index_plus_one = tc_at_m81 + 1;
        x2_at_m81 = cdata[tc_at_m81].position.x;
        y2_at_m81 = cdata[tc_at_m81].position.y;
        cdata[tc_at_m81].position.x = cdata[prm_605].position.x;
        cdata[tc_at_m81].position.y = cdata[prm_605].position.y;
    }
    else
    {
        cell_data.at(cdata[prm_605].position.x, cdata[prm_605].position.y)
            .chara_index_plus_one = 0;
        x2_at_m81 = prm_607;
        y2_at_m81 = prm_608;
    }
    cell_data.at(x2_at_m81, y2_at_m81).chara_index_plus_one = prm_605 + 1;
    cdata[prm_605].position.x = x2_at_m81;
    cdata[prm_605].position.y = y2_at_m81;
    if (prm_605 == 0 || tc_at_m81 == 0)
    {
        if (game_data.mount)
        {
            cdata[game_data.mount].position.x = cdata.player().position.x;
            cdata[game_data.mount].position.y = cdata.player().position.y;
        }
    }
    return true;
}



void cell_movechara(int cc, int x, int y)
{
    if (cell_data.at(x, y).chara_index_plus_one != 0)
    {
        if (cell_data.at(x, y).chara_index_plus_one - 1 == cc)
        {
            return;
        }
        cell_swap(cc, tc_at_m81);
    }
    else
    {
        cell_data.at(cdata[cc].position.x, cdata[cc].position.y)
            .chara_index_plus_one = 0;
        cdata[cc].position = {x, y};
        cell_data.at(x, y).chara_index_plus_one = cc + 1;
    }
}

int cell_itemlist(int prm_625, int prm_626)
{
    listmax = 0;
    for (const auto& cnt : items(-1))
    {
        if (inv[cnt].number() > 0)
        {
            if (inv[cnt].position.x == prm_625 &&
                inv[cnt].position.y == prm_626)
            {
                list(0, listmax) = cnt;
                ++listmax;
            }
        }
    }
    return rtval;
}

// Returns pair of number of items and the last item on the cell.
std::pair<int, int> cell_itemoncell(const Position& pos)
{
    int number{};
    int item{};

    for (const auto& ci : items(-1))
    {
        if (inv[ci].number() > 0 && inv[ci].position == pos)
        {
            ++number;
            item = ci;
        }
    }

    return std::make_pair(number, item);
}

void cell_setchara(int cc, int x, int y)
{
    cell_data.at(x, y).chara_index_plus_one = cc + 1;
    cdata[cc].position = Position{x, y};
}



void cell_removechara(int x, int y)
{
    cell_data.at(x, y).chara_index_plus_one = 0;
}



int cell_findspace(int prm_796, int prm_797, int prm_798)
{
    int f_at_m130 = 0;
    int dy_at_m130 = 0;
    int dx_at_m130 = 0;
    f_at_m130 = 0;
    for (int cnt = 0, cnt_end = (prm_798 * 2 + 1); cnt < cnt_end; ++cnt)
    {
        dy_at_m130 = prm_797 + cnt - 1;
        if (dy_at_m130 < 0 || dy_at_m130 >= map_data.height)
        {
            continue;
        }
        for (int cnt = 0, cnt_end = (prm_798 * 2 + 1); cnt < cnt_end; ++cnt)
        {
            dx_at_m130 = prm_796 + cnt - 1;
            if (dx_at_m130 < 0 || dx_at_m130 >= map_data.width)
            {
                continue;
            }
            if (cell_data.at(dx_at_m130, dy_at_m130).chara_index_plus_one != 0)
            {
                continue;
            }
            if (chipm(7, cell_data.at(dx_at_m130, dy_at_m130).chip_id_actual) &
                4)
            {
                continue;
            }
            if (chipm(7, cell_data.at(dx_at_m130, dy_at_m130).feats % 1000) & 4)
            {
                continue;
            }
            rtval(0) = dx_at_m130;
            rtval(1) = dy_at_m130;
            f_at_m130 = 1;
        }
        if (f_at_m130)
        {
            break;
        }
    }
    return f_at_m130;
}

static int _random_tile(elona_vector1<int> tile)
{
    if (tile(1) == 0 || tile(2) == 0)
    {
        return tile(0);
    }
    return tile(0) + (rnd(tile(2)) == 0) * rnd(tile(1));
}

int cell_get_type(TileKind type)
{
    // TODO dedup from map_converttile?
    elona_vector1<int> tile;
    switch (type)
    {
    case TileKind::normal: tile = tile_default; break;
    case TileKind::wall: tile = tile_wall; break;
    case TileKind::tunnel: tile = tile_tunnel; break;
    case TileKind::room: tile = tile_room; break;
    case TileKind::fog: tile = tile_fog; break;
    default: assert(0);
    }

    return _random_tile(tile);
}

} // namespace elona
