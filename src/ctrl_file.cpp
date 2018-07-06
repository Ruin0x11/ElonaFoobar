#include "ctrl_file.hpp"
#include "ability.hpp"
#include "character.hpp"
#include "elona.hpp"
#include "filesystem.hpp"
#include "item.hpp"
#include "log.hpp"
#include "lua_env/lua_env.hpp"
#include "mef.hpp"
#include "putit.hpp"
#include "variables.hpp"

using namespace elona;


namespace
{



void arrayfile_read(const std::string& fmode_str, const fs::path& filepath)
{
    std::vector<std::string> lines;
    if (fs::exists(filepath))
    {
        range::copy(
            fileutil::read_by_line{filepath}, std::back_inserter(lines));
    }

    if (fmode_str == u8"qname"s)
    {
        lines.resize(500);
        auto itr = std::begin(lines);
        for (int i = 0; i < 500; ++i)
        {
            qname(i) = *itr;
            ++itr;
        }
    }
    else if (fmode_str == u8"gdatan"s)
    {
        lines.resize(50);
        auto itr = std::begin(lines);
        for (int i = 0; i < 50; ++i)
        {
            gdatan(i) = *itr;
            ++itr;
        }
    }
    else if (fmode_str == u8"mdatan"s)
    {
        lines.resize(2);
        auto itr = std::begin(lines);
        for (int i = 0; i < 2; ++i)
        {
            mdatan(i) = *itr;
            ++itr;
        }
    }
    else if (fmode_str == u8"cdatan1"s)
    {
        if (lines.size() <= 57 * 10 / 2)
            lines.resize(57 * 10 / 2);
        else
            lines.resize(57 * 10);
        auto itr = std::begin(lines);
        for (int i = 0; i < 57; ++i)
        {
            for (int j = 0; j < 10; ++j)
            {
                if (lines.size() <= 57 * 10 / 2 && j >= 10 / 2)
                    break;
                cdatan(j, i) = *itr;
                ++itr;
            }
        }
    }
    else if (fmode_str == u8"cdatan2"s)
    {
        if (lines.size() <= 188 * 10 / 2)
            lines.resize(188 * 10 / 2);
        else
            lines.resize(188 * 10);
        auto itr = std::begin(lines);
        for (int i = ELONA_MAX_PARTY_CHARACTERS; i < ELONA_MAX_CHARACTERS; ++i)
        {
            for (int j = 0; j < 10; ++j)
            {
                if (lines.size() <= 188 * 10 / 2 && j >= 10 / 2)
                    break;
                cdatan(j, i) = *itr;
                ++itr;
            }
        }
    }
    else if (fmode_str == u8"cdatan3"s)
    {
        if (lines.size() <= 10 / 2)
            lines.resize(10 / 2);
        else
            lines.resize(10);
        auto itr = std::begin(lines);
        for (int j = 0; j < 10; ++j)
        {
            if (lines.size() < 10 / 2 && j >= 10 / 2)
                break;
            cdatan(j, tg) = *itr;
            ++itr;
        }
    }
}



void arrayfile_write(const std::string& fmode_str, const fs::path& filepath)
{
    std::ofstream out{filepath.native(), std::ios::binary};
    if (!out)
    {
        throw std::runtime_error(
            u8"Error: fail to write "
            + filesystem::make_preferred_path_in_utf8(filepath));
    }

    if (fmode_str == u8"qname"s)
    {
        for (int i = 0; i < 500; ++i)
        {
            out << qname(i) << std::endl;
        }
    }
    else if (fmode_str == u8"gdatan"s)
    {
        for (int i = 0; i < 50; ++i)
        {
            out << gdatan(i) << std::endl;
        }
    }
    else if (fmode_str == u8"mdatan"s)
    {
        for (int i = 0; i < 2; ++i)
        {
            out << mdatan(i) << std::endl;
        }
    }
    else if (fmode_str == u8"cdatan1"s)
    {
        for (int i = 0; i < 57; ++i)
        {
            for (int j = 0; j < 10; ++j)
            {
                out << cdatan(j, i) << std::endl;
            }
        }
    }
    else if (fmode_str == u8"cdatan2"s)
    {
        for (int i = ELONA_MAX_PARTY_CHARACTERS; i < ELONA_MAX_CHARACTERS; ++i)
        {
            for (int j = 0; j < 10; ++j)
            {
                out << cdatan(j, i) << std::endl;
            }
        }
    }
    else if (fmode_str == u8"cdatan3"s)
    {
        for (int j = 0; j < 10; ++j)
        {
            out << cdatan(j, tg) << std::endl;
        }
    }

    if (elona_export == 0)
    {
        fileadd(filepath);
    }
}



void arrayfile(
    bool fread,
    const std::string& fmode_str,
    const fs::path& filepath)
{
    if (!fread)
    {
        arrayfile_write(fmode_str, filepath);
    }
    else
    {
        arrayfile_read(fmode_str, filepath);
    }

    elona_export = 0;
}


template <typename T>
void load_v1(
    const fs::path& filepath,
    elona_vector1<T>& data,
    size_t begin,
    size_t end,
    bool gzip = false)
{
    std::ifstream in{filepath.native(), std::ios::binary};
    if (in.fail())
    {
        throw std::runtime_error(
            u8"Could not open file at "s + filepath.string());
    }
    putit::binary_iarchive ar(in, gzip);
    for (size_t i = begin; i < end; ++i)
    {
        ar.load(data(i));
    }
}


template <typename T>
void save_v1(
    const fs::path& filepath,
    elona_vector1<T>& data,
    size_t begin,
    size_t end,
    bool gzip = false)
{
    std::ofstream out{filepath.native(), std::ios::binary};
    if (out.fail())
    {
        throw std::runtime_error(
            u8"Could not open file at "s + filepath.string());
    }
    putit::binary_oarchive ar(out, gzip);
    for (size_t i = begin; i < end; ++i)
    {
        ar.save(data(i));
    }
}


template <typename T>
void load_v2(
    const fs::path& filepath,
    elona_vector2<T>& data,
    size_t i_begin,
    size_t i_end,
    size_t j_begin,
    size_t j_end,
    bool gzip = false)
{
    std::ifstream in{filepath.native(), std::ios::binary};
    if (in.fail())
    {
        throw std::runtime_error(
            u8"Could not open file at "s + filepath.string());
    }
    putit::binary_iarchive ar{in, gzip};
    for (size_t j = j_begin; j < j_end; ++j)
    {
        for (size_t i = i_begin; i < i_end; ++i)
        {
            ar.load(data(i, j));
        }
    }
}


template <typename T>
void save_v2(
    const fs::path& filepath,
    elona_vector2<T>& data,
    size_t i_begin,
    size_t i_end,
    size_t j_begin,
    size_t j_end,
    bool gzip = false)
{
    std::ofstream out{filepath.native(), std::ios::binary};
    if (out.fail())
    {
        throw std::runtime_error(
            u8"Could not open file at "s + filepath.string());
    }
    putit::binary_oarchive ar{out, gzip};
    for (size_t j = j_begin; j < j_end; ++j)
    {
        for (size_t i = i_begin; i < i_end; ++i)
        {
            ar.save(data(i, j));
        }
    }
}


template <typename T>
void load_v3(
    const fs::path& filepath,
    elona_vector3<T>& data,
    size_t i_begin,
    size_t i_end,
    size_t j_begin,
    size_t j_end,
    size_t k_begin,
    size_t k_end,
    bool gzip = false)
{
    std::ifstream in{filepath.native(), std::ios::binary};
    if (in.fail())
    {
        throw std::runtime_error(
            u8"Could not open file at "s + filepath.string());
    }
    putit::binary_iarchive ar{in, gzip};
    for (size_t k = k_begin; k < k_end; ++k)
    {
        for (size_t j = j_begin; j < j_end; ++j)
        {
            for (size_t i = i_begin; i < i_end; ++i)
            {
                ar.load(data(i, j, k));
            }
        }
    }
}


template <typename T>
void save_v3(
    const fs::path& filepath,
    elona_vector3<T>& data,
    size_t i_begin,
    size_t i_end,
    size_t j_begin,
    size_t j_end,
    size_t k_begin,
    size_t k_end,
    bool gzip = false)
{
    std::ofstream out{filepath.native(), std::ios::binary};
    if (out.fail())
    {
        throw std::runtime_error(
            u8"Could not open file at "s + filepath.string());
    }
    putit::binary_oarchive ar{out, gzip};
    for (size_t k = k_begin; k < k_end; ++k)
    {
        for (size_t j = j_begin; j < j_end; ++j)
        {
            for (size_t i = i_begin; i < i_end; ++i)
            {
                ar.save(data(i, j, k));
            }
        }
    }
}


template <typename T>
void load(const fs::path& filepath, T& data, size_t begin, size_t end)
{
    std::ifstream in{filepath.native(), std::ios::binary};
    if (in.fail())
    {
        throw std::runtime_error(
            u8"Could not open file at "s + filepath.string());
    }
    putit::binary_iarchive ar{in};
    for (size_t i = begin; i < end; ++i)
    {
        ar.load(data[i]);
    }
}


template <typename T>
void save(const fs::path& filepath, T& data, size_t begin, size_t end)
{
    std::ofstream out{filepath.native(), std::ios::binary};
    if (out.fail())
    {
        throw std::runtime_error(
            u8"Could not open file at "s + filepath.string());
    }
    putit::binary_oarchive ar{out};
    for (size_t i = begin; i < end; ++i)
    {
        ar.save(data[i]);
    }
}


// reads or writes global save data:
// - save game header file (on write)
// - character/skill/inventory data for the player and characters in their party
// - global variables
// - NPC memory
// - Item identification memory
// - Foobar save data
// - quest data
// - list of pending events
// - lots of things relevant to the player only, like traits/material
// counts/spell data/recipes...
// - trading cards
// - appearance of the character (PCC)
// - adventurer news
// - artifact discovery information
// - other things...
void fmode_7_8(bool read, const fs::path& dir)
{
    if (!fs::exists(dir))
    {
        fs::create_directory(dir);
    }

    if (!read)
    {
        playerheader =
            cdatan(0, 0) + u8" Lv:" + cdata[0].level + u8" " + mdatan(0);
        bsave(dir / u8"header.txt", playerheader);
    }

    {
        const auto filepath = dir / u8"gdata.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v1(filepath, gdata, 0, 1000);
            }
        }
        else
        {
            save_v1(filepath, gdata, 0, 1000);
        }
    }

    {
        const auto filepath = dir / u8"foobar_data.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                std::ifstream in{filepath.native(), std::ios::binary};
                putit::binary_iarchive ar{in};
                ar.load(foobar_data);
            }
        }
        else
        {
            std::ofstream out{filepath.native(), std::ios::binary};
            putit::binary_oarchive ar{out};
            ar.save(foobar_data);
        }
    }

    {
        const auto filepath = dir / u8"cdata.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                for (int index = 0; index < ELONA_MAX_PARTY_CHARACTERS; index++)
                {
                    lua::lua.on_chara_unloaded(cdata[index]);
                }
                load(filepath, cdata, 0, ELONA_MAX_PARTY_CHARACTERS);
                for (int index = 0; index < ELONA_MAX_PARTY_CHARACTERS; index++)
                {
                    cdata[index].index = index;
                    lua::lua.on_chara_loaded(cdata[index]);
                }
            }
        }
        else
        {
            save(filepath, cdata, 0, ELONA_MAX_PARTY_CHARACTERS);
        }
    }

    {
        const auto filepath = dir / u8"sdata.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                std::ifstream in{filepath.native(), std::ios::binary};
                putit::binary_iarchive ar{in};
                for (int cc = 0; cc < ELONA_MAX_PARTY_CHARACTERS; ++cc)
                {
                    for (int i = 0; i < 600; ++i)
                    {
                        ar.load(sdata.get(i, cc));
                    }
                }
            }
        }
        else
        {
            std::ofstream out{filepath.native(), std::ios::binary};
            putit::binary_oarchive ar{out};
            for (int cc = 0; cc < ELONA_MAX_PARTY_CHARACTERS; ++cc)
            {
                for (int i = 0; i < 600; ++i)
                {
                    ar.save(sdata.get(i, cc));
                }
            }
        }
    }

    {
        const auto filepath = dir / u8"spell.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v1(filepath, spell, 0, 200);
            }
        }
        else
        {
            save_v1(filepath, spell, 0, 200);
        }
    }

    {
        const auto filepath = dir / u8"inv.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                for (int index = 0; index < 1320; index++)
                {
                    lua::lua.on_item_unloaded(inv[index]);
                }
                load(filepath, inv, 0, 1320);
                for (int index = 0; index < 1320; index++)
                {
                    inv[index].index = index;
                    lua::lua.on_item_loaded(inv[index]);
                }
            }
        }
        else
        {
            save(filepath, inv, 0, 1320);
        }
    }

    {
        const auto filepath = dir / u8"kitem.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v2(filepath, itemmemory, 0, 3, 0, 800);
            }
        }
        else
        {
            save_v2(filepath, itemmemory, 0, 3, 0, 800);
        }
    }

    {
        const auto filepath = dir / u8"knpc.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v2(filepath, npcmemory, 0, 2, 0, 800);
            }
        }
        else
        {
            save_v2(filepath, npcmemory, 0, 2, 0, 800);
        }
    }

    {
        const auto filepath = dir / u8"adata.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v2(filepath, adata, 0, 40, 0, 500);
            }
        }
        else
        {
            save_v2(filepath, adata, 0, 40, 0, 500);
        }
    }

    {
        const auto filepath = dir / u8"spact.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v1(filepath, spact, 0, 500);
            }
        }
        else
        {
            save_v1(filepath, spact, 0, 500);
        }
    }

    {
        const auto filepath = dir / u8"qdata.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v2(filepath, qdata, 0, 20, 0, 500);
            }
        }
        else
        {
            save_v2(filepath, qdata, 0, 20, 0, 500);
        }
    }

    {
        const auto filepath = dir / u8"mat.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v1(filepath, mat, 0, 400);
            }
        }
        else
        {
            save_v1(filepath, mat, 0, 400);
        }
    }

    {
        const auto filepath = dir / u8"trait.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v1(filepath, trait, 0, 500);
            }
        }
        else
        {
            save_v1(filepath, trait, 0, 500);
        }
    }

    {
        const auto filepath = dir / u8"pcc.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v2(filepath, pcc, 0, 30, 0, 20);
            }
        }
        else
        {
            save_v2(filepath, pcc, 0, 30, 0, 20);
        }
    }

    {
        const auto filepath = dir / u8"card.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v2(filepath, card, 0, 100, 0, 40);
            }
        }
        else
        {
            save_v2(filepath, card, 0, 100, 0, 40);
        }
    }

    {
        const auto filepath = dir / u8"krecipe.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                if (gdata_version >= 1200)
                {
                    load_v1(filepath, recipememory, 0, 1200);
                }
            }
        }
        else
        {
            save_v1(filepath, recipememory, 0, 1200);
        }
    }

    {
        const auto filepath = dir / u8"art.log";
        if (!read)
        {
            std::ofstream out{filepath.native(), std::ios::binary};
            range::for_each(artifactlocation, [&](const auto& line) {
                out << line << std::endl;
            });
        }
        if (read)
        {
            artifactlocation.clear();
            range::copy(
                fileutil::read_by_line{filepath},
                std::back_inserter(artifactlocation));
        }
    }

    {
        const auto filepath = dir / u8"news.log";
        notesel(newsbuff);
        if (!read)
        {
            std::ofstream out{filepath.native(), std::ios::binary};
            out << newsbuff(0) << std::endl;
        }
        if (read)
        {
            newsbuff(0).clear();
            std::ifstream in{filepath.native(), std::ios::binary};
            std::string tmp;
            while (std::getline(in, tmp))
            {
                newsbuff(0) += tmp + '\n';
            }
        }
    }

    arrayfile(read, u8"cdatan1", dir / u8"cdatan.s1");
    arrayfile(read, u8"qname", dir / u8"qname.s1");
    arrayfile(read, u8"gdatan", dir / u8"gdatan.s1");
    if (!read)
    {
        bsave(dir / u8"evnum.s1", evnum);
        bsave(dir / u8"evdata1.s1", evdata1);
        bsave(dir / u8"evdata2.s1", evdata2);
        bsave(dir / u8"evlist.s1", evlist);
    }
    else
    {
        if (fs::exists(dir / u8"evnum.s1"))
        {
            bload(dir / u8"evnum.s1", evnum);
        }
        if (fs::exists(dir / u8"evdata1.s1"))
        {
            bload(dir / u8"evdata1.s1", evdata1);
        }
        if (fs::exists(dir / u8"evdata2.s1"))
        {
            bload(dir / u8"evdata2.s1", evdata2);
        }
        if (fs::exists(dir / u8"evlist.s1"))
        {
            bload(dir / u8"evlist.s1", evlist);
        }
    }
}


// reads or writes gene data.
void fmode_14_15(bool read)
{
    const auto dir =
        read ? filesystem::dir::save(geneuse) : filesystem::dir::tmp();
    if (!read)
    {
        playerheader = cdatan(0, 0) + u8"(Lv" + cdata[0].level + u8")の遺伝子";
        const auto filepath = dir / u8"gene_header.txt";
        bsave(filepath, playerheader);
        fileadd(filepath);
    }

    {
        const auto filepath = dir / u8"g_cdata.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                for (int index = 0; index < ELONA_MAX_PARTY_CHARACTERS; index++)
                {
                    lua::lua.on_chara_unloaded(cdata[index]);
                }
                load(filepath, cdata, 0, ELONA_MAX_PARTY_CHARACTERS);
                for (int index = 0; index < ELONA_MAX_PARTY_CHARACTERS; index++)
                {
                    cdata[index].index = index;
                    lua::lua.on_chara_loaded(cdata[index]);
                }
            }
        }
        else
        {
            fileadd(filepath);
            save(filepath, cdata, 0, ELONA_MAX_PARTY_CHARACTERS);
        }
    }

    {
        const auto filepath = dir / u8"g_sdata.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                std::ifstream in{filepath.native(), std::ios::binary};
                putit::binary_iarchive ar{in};
                for (int cc = 0; cc < ELONA_MAX_PARTY_CHARACTERS; ++cc)
                {
                    for (int i = 0; i < 600; ++i)
                    {
                        ar.load(sdata.get(i, cc));
                    }
                }
            }
        }
        else
        {
            fileadd(filepath);
            std::ofstream out{filepath.native(), std::ios::binary};
            putit::binary_oarchive ar{out};
            for (int cc = 0; cc < ELONA_MAX_PARTY_CHARACTERS; ++cc)
            {
                for (int i = 0; i < 600; ++i)
                {
                    ar.save(sdata.get(i, cc));
                }
            }
        }
    }

    {
        const auto filepath = dir / u8"g_spell.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v1(filepath, spell, 0, 200);
            }
        }
        else
        {
            fileadd(filepath);
            save_v1(filepath, spell, 0, 200);
        }
    }

    {
        const auto filepath = dir / u8"g_inv.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                for (int index = 0; index < 1320; index++)
                {
                    lua::lua.on_item_unloaded(inv[index]);
                }
                load(filepath, inv, 0, 1320);
                for (int index = 0; index < 1320; index++)
                {
                    inv[index].index = index;
                    lua::lua.on_item_loaded(inv[index]);
                }
            }
        }
        else
        {
            fileadd(filepath);
            save(filepath, inv, 0, 1320);
        }
    }

    {
        const auto filepath = dir / u8"g_spact.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v1(filepath, spact, 0, 500);
            }
        }
        else
        {
            fileadd(filepath);
            save_v1(filepath, spact, 0, 500);
        }
    }

    {
        const auto filepath = dir / u8"g_mat.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v1(filepath, mat, 0, 400);
            }
        }
        else
        {
            fileadd(filepath);
            save_v1(filepath, mat, 0, 400);
        }
    }

    {
        const auto filepath = dir / u8"g_card.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v2(filepath, card, 0, 100, 0, 40);
            }
        }
        else
        {
            fileadd(filepath);
            save_v2(filepath, card, 0, 100, 0, 40);
        }
    }

    {
        const auto filepath = dir / u8"g_genetemp.s1";
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v1(filepath, genetemp, 0, 1000);
            }
        }
        else
        {
            fileadd(filepath);
            save_v1(filepath, genetemp, 0, 1000);
        }
    }
}


// reads or writes map-local data for the map with id "mid" (map data,
// tiles, characters, skill status, map effects, character names)
// does not read/write cdata or sdata for player or party characters.
void fmode_1_2(bool read)
{
    const auto dir = filesystem::dir::tmp();

    {
        const auto filepath = dir / (u8"mdata_"s + mid + u8".s2");
        if (read)
        {
            load_v1(filepath, mdata, 0, 100);
        }
        else
        {
            fileadd(filepath);
            save_v1(filepath, mdata, 0, 100);
        }
    }

    {
        const auto filepath = dir / (u8"map_"s + mid + u8".s2");
        if (read)
        {
            DIM4(map, mdata(0), mdata(1), 10);
            DIM3(mapsync, mdata(0), mdata(1));
            DIM3(mef, 9, MEF_MAX);
            load_v3(filepath, map, 0, mdata(0), 0, mdata(1), 0, 10);
        }
        else
        {
            fileadd(filepath);
            save_v3(filepath, map, 0, mdata(0), 0, mdata(1), 0, 10);
        }
    }

    {
        const auto filepath = dir / (u8"cdata_"s + mid + u8".s2");
        if (read)
        {
            for (int index = ELONA_MAX_PARTY_CHARACTERS;
                 index < ELONA_MAX_CHARACTERS;
                 index++)
            {
                lua::lua.on_chara_unloaded(cdata[index]);
            }
            load(
                filepath,
                cdata,
                ELONA_MAX_PARTY_CHARACTERS,
                ELONA_MAX_CHARACTERS);
            for (int index = ELONA_MAX_PARTY_CHARACTERS;
                 index < ELONA_MAX_CHARACTERS;
                 index++)
            {
                cdata[index].index = index;
                lua::lua.on_chara_loaded(cdata[index]);
            }
        }
        else
        {
            fileadd(filepath);
            save(
                filepath,
                cdata,
                ELONA_MAX_PARTY_CHARACTERS,
                ELONA_MAX_CHARACTERS);
        }
    }

    {
        const auto filepath = dir / (u8"sdata_"s + mid + u8".s2");
        if (read)
        {
            std::ifstream in{filepath.native(), std::ios::binary};
            putit::binary_iarchive ar{in};
            for (int cc = ELONA_MAX_PARTY_CHARACTERS; cc < ELONA_MAX_CHARACTERS;
                 ++cc)
            {
                for (int i = 0; i < 600; ++i)
                {
                    ar.load(sdata.get(i, cc));
                }
            }
        }
        else
        {
            fileadd(filepath);
            std::ofstream out{filepath.native(), std::ios::binary};
            putit::binary_oarchive ar{out};
            for (int cc = ELONA_MAX_PARTY_CHARACTERS; cc < ELONA_MAX_CHARACTERS;
                 ++cc)
            {
                for (int i = 0; i < 600; ++i)
                {
                    ar.save(sdata.get(i, cc));
                }
            }
        }
    }

    {
        const auto filepath = dir / (u8"mef_"s + mid + u8".s2");
        if (read)
        {
            if (mdata(21) == 0)
            {
                for (int y = 0; y < mdata(1); ++y)
                {
                    for (int x = 0; x < mdata(0); ++x)
                    {
                        map(x, y, 8) = 0;
                    }
                }
                mdata(21) = 1;
            }
            else
            {
                load_v2(filepath, mef, 0, 9, 0, MEF_MAX);
            }
        }
        else
        {
            fileadd(filepath);
            save_v2(filepath, mef, 0, 9, 0, MEF_MAX);
        }
    }

    arrayfile(read, u8"cdatan2", dir / (u8"cdatan_"s + mid + u8".s2"));
    arrayfile(read, u8"mdatan", dir / (u8"mdatan_"s + mid + u8".s2"));
}



void fmode_16()
{
    DIM3(cmapdata, 5, 400);

    load_v3(fmapfile + u8".map", map, 0, mdata(0), 0, mdata(1), 0, 3, true);

    const auto filepath = fmapfile + u8".obj"s;
    if (!fs::exists(filepath))
    {
        return;
    }
    load_v2(filepath, cmapdata, 0, 5, 0, 400, true);
}


// reads or writes a custom map.
// this is currently never called to write anything.
void fmode_5_6(bool read)
{
    if (read)
    {
        DIM3(cmapdata, 5, 400);
        DIM3(mef, 9, MEF_MAX);
    }

    {
        const auto filepath = fmapfile + u8".idx"s;
        if (read)
        {
            load_v1(filepath, mdatatmp, 0, 100, true);
            for (int j = 0; j < 5; ++j)
            {
                mdata(j) = mdatatmp(j);
            }
        }
        else
        {
            save_v1(filepath, mdata, 0, 100, true);
        }
    }

    {
        const auto filepath = fmapfile + u8".map"s;
        if (read)
        {
            DIM4(map, mdata(0), mdata(1), 10);
            DIM3(mapsync, mdata(0), mdata(1)); // TODO length_exception
            load_v3(filepath, map, 0, mdata(0), 0, mdata(1), 0, 10, true);
        }
        else
        {
            save_v3(filepath, map, 0, mdata(0), 0, mdata(1), 0, 10, true);
        }
    }

    {
        const auto filepath = fmapfile + u8".obj"s;
        if (read)
        {
            if (fs::exists(filepath))
            {
                load_v2(filepath, cmapdata, 0, 5, 0, 400, true);
            }
        }
        else
        {
            save_v2(filepath, cmapdata, 0, 5, 0, 400, true);
        }
    }
}


// reads or writes map-local item data (inv_xx.s2)
// does not read/write player or party character inventories.
void fmode_3_4(bool read, const fs::path& filename)
{
    const auto filepath = filesystem::dir::tmp() / filename;
    if (read)
    {
        for (int index = 1320; index < 5480; index++)
        {
            lua::lua.on_item_unloaded(inv[index]);
        }
        load(filepath, inv, 1320, 5480);
        for (int index = 1320; index < 5480; index++)
        {
            inv[index].index = index;
            lua::lua.on_item_loaded(inv[index]);
        }
    }
    else
    {
        fileadd(filepath);
        save(filepath, inv, 1320, 5480);
    }
}



void fmode_23_24(bool read, const fs::path& filepath)
{
    if (read)
    {
        fileadd(filepath);
        save_v1(filepath, deck, 0, 1000);
    }
    else
    {
        load_v1(filepath, deck, 0, 1000);
    }
}


// reads character and skill data when upgrading the character's home.
void fmode_17()
{
    const auto dir = filesystem::dir::tmp();

    if (!fs::exists(dir / (u8"cdata_"s + mid + u8".s2")))
        return;

    {
        const auto filepath = dir / (u8"cdata_"s + mid + u8".s2");
        if (true)
        {
            for (int index = ELONA_MAX_PARTY_CHARACTERS;
                 index < ELONA_MAX_CHARACTERS;
                 index++)
            {
                lua::lua.on_chara_unloaded(cdata[index]);
            }
            load(
                filepath,
                cdata,
                ELONA_MAX_PARTY_CHARACTERS,
                ELONA_MAX_CHARACTERS);
            for (int index = ELONA_MAX_PARTY_CHARACTERS;
                 index < ELONA_MAX_CHARACTERS;
                 index++)
            {
                cdata[index].index = index;
                lua::lua.on_chara_loaded(cdata[index]);
            }
        }
        else
        {
            fileadd(filepath);
            save(
                filepath,
                cdata,
                ELONA_MAX_PARTY_CHARACTERS,
                ELONA_MAX_CHARACTERS);
        }
    }

    {
        const auto filepath = dir / (u8"sdata_"s + mid + u8".s2");
        if (true)
        {
            std::ifstream in{filepath.native(), std::ios::binary};
            putit::binary_iarchive ar{in};
            for (int cc = ELONA_MAX_PARTY_CHARACTERS; cc < ELONA_MAX_CHARACTERS;
                 ++cc)
            {
                for (int i = 0; i < 600; ++i)
                {
                    ar.load(sdata.get(i, cc));
                }
            }
        }
        else
        {
            fileadd(filepath);
            std::ofstream out{filepath.native(), std::ios::binary};
            putit::binary_oarchive ar{out};
            for (int cc = ELONA_MAX_PARTY_CHARACTERS; cc < ELONA_MAX_CHARACTERS;
                 ++cc)
            {
                for (int i = 0; i < 600; ++i)
                {
                    ar.save(sdata.get(i, cc));
                }
            }
        }
    }

    arrayfile(true, u8"cdatan2", dir / (u8"cdatan_"s + mid + u8".s2"));
}


// deletes all files inside the temporary directory (tmp/)
void fmode_10()
{
    for (const auto& entry : filesystem::dir_entries(
             filesystem::dir::tmp(),
             filesystem::dir_entries::type::file,
             std::regex{u8R"(.*\..*)"}))
    {
        fs::remove_all(entry.path());
    }
}


// deletes a saved game.
void fmode_9()
{
    fs::remove_all(filesystem::dir::save(playerid));
}


// deletes a map and optionally deletes characters/skills/items in it.
// the optional case is so the characters/skills/items can be
// preserved in the case of upgrading the player's home.
void fmode_11_12(bool preserve_items)
{
    if (preserve_items)
    {
        if (!fs::exists(filesystem::dir::tmp() / (u8"mdata_"s + mid + u8".s2")))
        {
            // We tried preserving the characters/items, but the home
            // map to transfer them from didn't exist.
            return;
        }
    }
    auto filepath = filesystem::dir::tmp() / (u8"map_"s + mid + u8".s2");
    if (!fs::exists(filepath))
        return;

    fs::remove_all(filepath);
    fileadd(filepath, 1);
    if (!preserve_items)
    {
        filepath = filesystem::dir::tmp() / (u8"cdata_"s + mid + u8".s2");
        fs::remove_all(filepath);
        fileadd(filepath, 1);
        filepath = filesystem::dir::tmp() / (u8"sdata_"s + mid + u8".s2");
        fs::remove_all(filepath);
        fileadd(filepath, 1);
        filepath = filesystem::dir::tmp() / (u8"cdatan_"s + mid + u8".s2");
        fs::remove_all(filepath);
        fileadd(filepath, 1);
        filepath = filesystem::dir::tmp() / (u8"inv_"s + mid + u8".s2");
        fs::remove_all(filepath);
        fileadd(filepath, 1);
    }
    filepath = filesystem::dir::tmp() / (u8"mdata_"s + mid + u8".s2");
    fs::remove_all(filepath);
    fileadd(filepath, 1);
    filepath = filesystem::dir::tmp() / (u8"mdatan_"s + mid + u8".s2");
    fs::remove_all(filepath);
    fileadd(filepath, 1);
    filepath = filesystem::dir::tmp() / (u8"mef_"s + mid + u8".s2");
    fs::remove_all(filepath);
    fileadd(filepath, 1);
}


// deletes files inside the temporary directory (tmp/) for an area
void fmode_13()
{
    for (int i = 0; i < 40; ++i)
    {
        adata(i, area) = 0;
    }
    for (const auto& entry : filesystem::dir_entries(
             filesystem::dir::tmp(),
             filesystem::dir_entries::type::file,
             std::regex{u8R"(.*_)"s + area + u8R"(_.*\..*)"}))
    {
        fs::remove_all(entry.path());
        fileadd(entry.path(), 1);
    }
}



} // namespace


namespace elona
{


void ctrl_file(file_operation_t file_operation)
{
    bool is_read;
    notesel(filemod);
    gdata_play_time = gdata_play_time + timeGetTime() / 1000 - time_begin;
    time_begin = timeGetTime() / 1000;

    switch (file_operation)
    {
    case file_operation_t::map_read:
    case file_operation_t::map_write:
        is_read = file_operation == file_operation_t::map_read;
        fmode_1_2(is_read);
        break;
    case file_operation_t::custom_map_read:
    case file_operation_t::custom_map_write:
        is_read = file_operation == file_operation_t::custom_map_read;
        fmode_5_6(is_read);
        break;
    case file_operation_t::save_game_delete: fmode_9(); break;
    case file_operation_t::temp_dir_delete: fmode_10(); break;
    case file_operation_t::map_delete:
    case file_operation_t::map_delete_preserve_items:
        fmode_11_12(file_operation == file_operation_t::map_delete_preserve_items);
        break;
    case file_operation_t::temp_dir_delete_area: fmode_13(); break;
    case file_operation_t::gene_write:
    case file_operation_t::gene_read:
        is_read = file_operation == file_operation_t::gene_read;
        fmode_14_15(is_read);
        break;
    case file_operation_t::map_load_map_obj_files: fmode_16(); break;
    case file_operation_t::map_home_upgrade: fmode_17(); break;
    default: assert(0);
    }
}


void ctrl_file(file_operation2_t file_operation, const fs::path& filepath)
{
    bool is_read;
    notesel(filemod);
    gdata_play_time = gdata_play_time + timeGetTime() / 1000 - time_begin;
    time_begin = timeGetTime() / 1000;

    switch (file_operation)
    {
    case file_operation2_t::map_items_read:
    case file_operation2_t::map_items_write:
        is_read = file_operation == file_operation2_t::map_items_read;
        fmode_3_4(is_read, filepath);
        break;
    case file_operation2_t::global_read:
    case file_operation2_t::global_write:
        is_read = file_operation == file_operation2_t::global_read;
        fmode_7_8(is_read, filepath);
        break;
    case file_operation2_t::deck_write:
    case file_operation2_t::deck_read:
        is_read = file_operation == file_operation2_t::deck_read;
        fmode_23_24(is_read, filepath);
        break;
    default: assert(0);
    }
}



void fileadd(const fs::path& filepath, int prm_693)
{
    const auto mark_a = prm_693 ? '#' : '*';
    const auto mark_b = prm_693 ? '*' : '#';

    const auto filename = filesystem::to_utf8_path(filepath.filename());
    notesel(filemod);
    const auto pos = filemod(0).find(filename);
    if (pos != std::string::npos)
    {
        if (filemod(0)[pos - 1] == mark_b)
        {
            filemod(0)[pos - 1] = mark_a;
        }
        noteunsel();
        return;
    }
    noteadd(mark_a + filename);
    noteunsel();
}



} // namespace elona
