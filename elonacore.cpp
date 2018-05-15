#include "ability.hpp"
#include "action.hpp"
#include "adventurer.hpp"
#include "ai.hpp"
#include "animation.hpp"
#include "audio.hpp"
#include "autopick.hpp"
#include "blending.hpp"
#include "buff.hpp"
#include "building.hpp"
#include "calc.hpp"
#include "card.hpp"
#include "casino.hpp"
#include "casino_card.hpp"
#include "character.hpp"
#include "character_status.hpp"
#include "class.hpp"
#include "command.hpp"
#include "config.hpp"
#include "crafting.hpp"
#include "ctrl_file.hpp"
#include "damage.hpp"
#include "debug.hpp"
#include "draw.hpp"
#include "elona.hpp"
#include "enchantment.hpp"
#include "equipment.hpp"
#include "event.hpp"
#include "filesystem.hpp"
#include "fish.hpp"
#include "foobar_save.hpp"
#include "food.hpp"
#include "fov.hpp"
#include "gathering.hpp"
#include "i18n.hpp"
#include "input.hpp"
#include "item.hpp"
#include "item_db.hpp"
#include "item_material.hpp"
#include "itemgen.hpp"
#include "log.hpp"
#include "macro.hpp"
#include "main.hpp"
#include "map.hpp"
#include "map_cell.hpp"
#include "mapgen.hpp"
#include "mef.hpp"
#include "menu.hpp"
#include "net.hpp"
#include "plant.hpp"
#include "quest.hpp"
#include "race.hpp"
#include "random.hpp"
#include "shop.hpp"
#include "trait.hpp"
#include "ui.hpp"
#include "variables.hpp"
#include "version.hpp"
#include "wish.hpp"

using namespace elona;


namespace
{

int cs_posbk_x{};
int cs_posbk_y{};
int cs_posbk_w{};
int cs_posbk_h{};



std::string atbuff;



} // namespace



namespace elona
{


int prm_518;
int prm_818;
int prm_820;
int prm_853;

elona_vector1<int> inhlist;

double r_at_m133;


elona_vector1<int> ranknorma;
int f_at_m14 = 0;
elona_vector1<std::string> tname;
int lv_at_m77 = 0;
int exp_at_m77 = 0;
int growth_at_m77 = 0;
int lvchange_at_m77 = 0;
int pagebk = 0;
int csprev = 0;
int pagesaved = 0;
int himc_at_ime_control = 0;
int hwnd = 0;
int x2_at_m105 = 0;
int y2_at_m105 = 0;
int dx_at_m133 = 0;
elona_vector1<int> dy_at_m133;
int f_at_con = 0;
int res_at_con = 0;
int p_at_con = 0;
int dx_at_m136 = 0;
int dy_at_m136 = 0;
int dmg_at_m141 = 0;
int rtdmg = 0;
std::string ndeathcause;
int catitem = 0;
int rollanatomy = 0;
std::string url_at_m147;
std::string chatsendurl;
std::string conurl_at_m147;
elona_vector1<std::string> cgiurl3;
std::string votesendurl;
elona_vector1<std::string> cgiurl2;
std::string chatnew;
std::string textreadurl;
std::string chatreadurl;
std::string votereadurl;
int chatturn = 0;
int r3 = 0;
int refdiff = 0;
int pcnoise = 0;
int traveldone = 0;
elona_vector1<int> fsetincome;
elona_vector1<int> fsetperform;
elona_vector1<int> fsetrare;
elona_vector1<int> fsetplantartifact;
elona_vector1<int> fsetplantunknown;
elona_vector1<int> fsetremain;
elona_vector1<int> fsetbarrel;
elona_vector1<int> isetgiftminor;
elona_vector1<int> isetgiftmajor;
elona_vector1<int> isetgiftgrand;
int cibk = 0;
int n_at_m174 = 0;
std::string s_at_m174;
int dbg_skipevent = 0;
int cansee = 0;
int kdx = 0;
int kdy = 0;
int mousex = 0;
int mousey = 0;
int findlocmode = 0;
int tgloc = 0;
int tglocx = 0;
int tglocy = 0;
std::string fileext;
std::string filedsc;
int lenhead = 0;
int lensum = 0;
elona_vector1<std::string> filebuff;
elona_vector1<std::string> filetemp;
int lensum_at_m188 = 0;
elona_vector1<std::string> filebuff_at_m188;
elona_vector1<int> p_at_m188;
int len_at_m188 = 0;
int lenhead_at_m188 = 0;
elona_vector1<std::string> filetemp_at_m188;
elona_vector1<std::string> txtbuff;
int initunid = 0;
int cun = 0;
elona_vector1<int> unaiactsubfreq;
elona_vector1<int> bmpbuff;
std::string usertitle;
std::string dbm;
int dbg_exitshowroom = 0;
int digx = 0;
int digy = 0;
int noeffect = 0;
int inumbk = 0;
int dirsub = 0;
int attackitem = 0;
int extraattack = 0;
int ct = 0;



int zentohan(const std::string& prm_209, std::string& prm_210, int prm_211)
{
    return LCMapStringA(
        GetUserDefaultLCID(), 4194304, prm_209, -1, prm_210, prm_211);
}



std::string lang(const std::string& prm_246, const std::string& prm_247)
{
    if (jp)
    {
        return prm_246;
    }
    else
    {
        return prm_247;
    }
}



std::string ranktitle(int prm_265)
{
    int p_at_m6 = 0;
    p_at_m6 = gdata((prm_265 + 120)) / 100;
    if (p_at_m6 == 1)
    {
        return rankn(0, prm_265);
    }
    if (p_at_m6 <= 5)
    {
        return rankn(1, prm_265);
    }
    if (p_at_m6 <= 10)
    {
        return rankn(2, prm_265);
    }
    if (p_at_m6 <= 80)
    {
        return rankn(p_at_m6 / 15 + 3, prm_265);
    }
    return rankn(9, prm_265);
}



void initialize_rankn()
{
    SDIM4(rankn, 30, 11, 9);
    if (jp)
    {
        rankn(0, 0) = u8"アリーナのチャンピオン"s;
        rankn(1, 0) = u8"アリーナの超エリート闘士"s;
        rankn(2, 0) = u8"アリーナのスター闘士"s;
        rankn(3, 0) = u8"アリーナのエリート闘士"s;
        rankn(4, 0) = u8"アリーナの熟練闘士"s;
        rankn(5, 0) = u8"アリーナの中堅闘士"s;
        rankn(6, 0) = u8"アリーナの期待の星"s;
        rankn(7, 0) = u8"アリーナのダークホース"s;
        rankn(8, 0) = u8"アリーナの下級闘士"s;
        rankn(9, 0) = u8"アリーナの無名の闘士"s;
        rankn(10, 0) = u8"アリーナ"s;
        rankn(0, 1) = u8"全てのペットの王"s;
        rankn(1, 1) = u8"超エリートペット使い"s;
        rankn(2, 1) = u8"ペットの王子"s;
        rankn(3, 1) = u8"ペットの憧れの的"s;
        rankn(4, 1) = u8"ペット使いのエリート"s;
        rankn(5, 1) = u8"名の知れたペット使い"s;
        rankn(6, 1) = u8"ペットの母"s;
        rankn(7, 1) = u8"なかなかのペット使い"s;
        rankn(8, 1) = u8"駆け出しのペット使い"s;
        rankn(9, 1) = u8"無名のペット使い"s;
        rankn(10, 1) = u8"ペットアリーナ"s;
        rankn(0, 2) = u8"ネフィアを統べる者"s;
        rankn(1, 2) = u8"迷宮の覇者"s;
        rankn(2, 2) = u8"ダンジョンの主"s;
        rankn(3, 2) = u8"高名な探検者"s;
        rankn(4, 2) = u8"子供達の憧れの的"s;
        rankn(5, 2) = u8"ダンジョン案内人"s;
        rankn(6, 2) = u8"名の知れた遺跡荒らし"s;
        rankn(7, 2) = u8"探検者"s;
        rankn(8, 2) = u8"ちんけな遺跡荒らし"s;
        rankn(9, 2) = u8"観光客"s;
        rankn(10, 2) = u8"クローラー"s;
        rankn(0, 3) = u8"ティリス最大の博物館"s;
        rankn(1, 3) = u8"超有名な博物館"s;
        rankn(2, 3) = u8"大人気の博物館"s;
        rankn(3, 3) = u8"有名な博物館"s;
        rankn(4, 3) = u8"来客の絶えない博物館"s;
        rankn(5, 3) = u8"ちょっと人気のある博物館"s;
        rankn(6, 3) = u8"名の知れた博物館"s;
        rankn(7, 3) = u8"まあまあの博物館"s;
        rankn(8, 3) = u8"来客の少ない博物館"s;
        rankn(9, 3) = u8"無名の博物館"s;
        rankn(10, 3) = u8"博物館"s;
        rankn(0, 4) = u8"ティリスの楽園"s;
        rankn(1, 4) = u8"超セレブな家"s;
        rankn(2, 4) = u8"ティリス有数の家"s;
        rankn(3, 4) = u8"とても住みたくなる家"s;
        rankn(4, 4) = u8"主婦の憧れの家"s;
        rankn(5, 4) = u8"ちょっと人目をひく家"s;
        rankn(6, 4) = u8"まあまあの家"s;
        rankn(7, 4) = u8"少しみすぼらしい家"s;
        rankn(8, 4) = u8"貧困にあえぐ家"s;
        rankn(9, 4) = u8"乞食の家"s;
        rankn(10, 4) = u8"ホーム"s;
        rankn(0, 5) = u8"ティリス最大の店"s;
        rankn(1, 5) = u8"王様が来る店"s;
        rankn(2, 5) = u8"大繁盛している店"s;
        rankn(3, 5) = u8"セレブが来る店"s;
        rankn(4, 5) = u8"来客の絶えない店"s;
        rankn(5, 5) = u8"マダムの来る店"s;
        rankn(6, 5) = u8"リピーターのいる店"s;
        rankn(7, 5) = u8"人が増えてきた売店"s;
        rankn(8, 5) = u8"ほとんど売れない売店"s;
        rankn(9, 5) = u8"無名の売店"s;
        rankn(10, 5) = u8"店"s;
        rankn(0, 6) = u8"ボス"s;
        rankn(1, 6) = u8"王様のアドバイザー"s;
        rankn(2, 6) = u8"エリートコンサルター"s;
        rankn(3, 6) = u8"ご意見番"s;
        rankn(4, 6) = u8"有権者の鑑"s;
        rankn(5, 6) = u8"マダム好みの有権者"s;
        rankn(6, 6) = u8"名の知れた有権者"s;
        rankn(7, 6) = u8"普通の有権者"s;
        rankn(8, 6) = u8"無関心な有権者"s;
        rankn(9, 6) = u8"ぎりぎり有権者"s;
        rankn(10, 6) = u8"コミュニティ"s;
        rankn(0, 8) = u8"グランドマスター"s;
        rankn(1, 8) = u8"マスター"s;
        rankn(2, 8) = u8"ギルドの重役"s;
        rankn(3, 8) = u8"ギルドの重役候補"s;
        rankn(4, 8) = u8"アダプト"s;
        rankn(5, 8) = u8"エキスパート"s;
        rankn(6, 8) = u8"ジャーニーマン"s;
        rankn(7, 8) = u8"ギルドの正式メンバー"s;
        rankn(8, 8) = u8"ギルドのメンバー候補"s;
        rankn(9, 8) = u8"見習い"s;
        rankn(10, 8) = u8"ギルド"s;
    }
    else
    {
        rankn(0, 0) = u8"Arena champion"s;
        rankn(1, 0) = u8"Super elite gladiator"s;
        rankn(2, 0) = u8"Star gladiator"s;
        rankn(3, 0) = u8"Elite gladiator"s;
        rankn(4, 0) = u8"Veteran gladiator"s;
        rankn(5, 0) = u8"Popular gladiator"s;
        rankn(6, 0) = u8"New hope"s;
        rankn(7, 0) = u8"Darkhorse"s;
        rankn(8, 0) = u8"Low class fighter"s;
        rankn(9, 0) = u8"Unknown fighter"s;
        rankn(10, 0) = u8"Arena"s;
        rankn(0, 1) = u8"King of tamer"s;
        rankn(1, 1) = u8"Super elite tamer"s;
        rankn(2, 1) = u8"Prince of animals"s;
        rankn(3, 1) = u8"Chief of animals"s;
        rankn(4, 1) = u8"Elite tamer"s;
        rankn(5, 1) = u8"Notorious tamer"s;
        rankn(6, 1) = u8"New hope"s;
        rankn(7, 1) = u8"Average tamer"s;
        rankn(8, 1) = u8"Petty tamer"s;
        rankn(9, 1) = u8"Unknown tamer"s;
        rankn(10, 1) = u8"Pet Arena"s;
        rankn(0, 2) = u8"King of Nefia"s;
        rankn(1, 2) = u8"Champion of labyrinth"s;
        rankn(2, 2) = u8"Dungeon master"s;
        rankn(3, 2) = u8"Famous adventurer"s;
        rankn(4, 2) = u8"Children's star"s;
        rankn(5, 2) = u8"Guide of Nefia"s;
        rankn(6, 2) = u8"Notorious tomb robber"s;
        rankn(7, 2) = u8"Tomb robber"s;
        rankn(8, 2) = u8"Famous tourist"s;
        rankn(9, 2) = u8"Tourist"s;
        rankn(10, 2) = u8"Crawler"s;
        rankn(0, 3) = u8"Tyris' greatest museum"s;
        rankn(1, 3) = u8"Royal museum"s;
        rankn(2, 3) = u8"Great museum"s;
        rankn(3, 3) = u8"Top museum"s;
        rankn(4, 3) = u8"Great museum"s;
        rankn(5, 3) = u8"Good museum"s;
        rankn(6, 3) = u8"Average museum"s;
        rankn(7, 3) = u8"Small museum"s;
        rankn(8, 3) = u8"Unknown museum"s;
        rankn(9, 3) = u8"Unknown Ruin"s;
        rankn(10, 3) = u8"Museum"s;
        rankn(0, 4) = u8"Heaven of Tyris"s;
        rankn(1, 4) = u8"Royal mansion"s;
        rankn(2, 4) = u8"Celebrity mansion"s;
        rankn(3, 4) = u8"Dream mansion"s;
        rankn(4, 4) = u8"Cozy mansion"s;
        rankn(5, 4) = u8"Attractive house"s;
        rankn(6, 4) = u8"Average house"s;
        rankn(7, 4) = u8"Poor house"s;
        rankn(8, 4) = u8"Peasant's shack"s;
        rankn(9, 4) = u8"Beggar's shack"s;
        rankn(10, 4) = u8"Home"s;
        rankn(0, 5) = u8"Tyris' greatest mall"s;
        rankn(1, 5) = u8"Royal mall"s;
        rankn(2, 5) = u8"Prosperous mall"s;
        rankn(3, 5) = u8"Celebrity shop"s;
        rankn(4, 5) = u8"Prosperous shop"s;
        rankn(5, 5) = u8"Popular shop"s;
        rankn(6, 5) = u8"Average shop"s;
        rankn(7, 5) = u8"Small shop"s;
        rankn(8, 5) = u8"Souvenir shop "s;
        rankn(9, 5) = u8"Unknown shop"s;
        rankn(10, 5) = u8"Shop"s;
        rankn(0, 6) = u8"Boss"s;
        rankn(1, 6) = u8"King's advisor"s;
        rankn(2, 6) = u8"Elite consultant"s;
        rankn(3, 6) = u8"Famous consultant"s;
        rankn(4, 6) = u8"Model voter"s;
        rankn(5, 6) = u8"Nice voter"s;
        rankn(6, 6) = u8"Infamous voter"s;
        rankn(7, 6) = u8"Average voter"s;
        rankn(8, 6) = u8"Indifferent voter"s;
        rankn(9, 6) = u8"Almost voter"s;
        rankn(10, 6) = u8"Community"s;
        rankn(0, 8) = u8"Grand master"s;
        rankn(1, 8) = u8"Master"s;
        rankn(2, 8) = u8"Adept"s;
        rankn(3, 8) = u8"Expert"s;
        rankn(4, 8) = u8"Journeyman"s;
        rankn(5, 8) = u8"Senior"s;
        rankn(6, 8) = u8"Apprentice"s;
        rankn(7, 8) = u8"Official member"s;
        rankn(8, 8) = u8"Candidate"s;
        rankn(9, 8) = u8"Thug"s;
        rankn(10, 8) = u8"Guild"s;
    }
    DIM2(ranknorma, 9);
    ranknorma(0) = 20;
    ranknorma(1) = 60;
    ranknorma(2) = 45;
    ranknorma(6) = 30;
    return;
}



std::string guildname()
{
    std::string s_at_m7;
    s_at_m7 = lang(u8"なし"s, u8"None"s);
    if (gdata_belongs_to_mages_guild != 0)
    {
        s_at_m7 = lang(u8"魔術士ギルド"s, u8"Mages Guild"s);
    }
    if (gdata_belongs_to_fighters_guild != 0)
    {
        s_at_m7 = lang(u8"戦士ギルド"s, u8"Fighters Guild"s);
    }
    if (gdata_belongs_to_thieves_guild != 0)
    {
        s_at_m7 = lang(u8"盗賊ギルド"s, u8"Thieves Guild"s);
    }
    return s_at_m7;
}



void initialize_post_data()
{
    SDIM3(popostname, 20, 7);
    popostname(1) = lang(u8"市長"s, u8"mayor"s);
    popostname(2) = lang(u8"村長"s, u8"chief"s);
    popostname(3) = lang(u8"司祭"s, u8"priest"s);
    popostname(4) = lang(u8"守衛長"s, u8"guard master"s);
    popostname(5) = lang(u8"徴税官"s, u8"tax master"s);
    popostname(6) = lang(u8"建設主任"s, u8"head architect"s);
    DIM4(pochart, 10, 10, 10);
    pochart(0, 0, 0) = 2;
    pochart(0, 1, 0) = 3;
    pochart(1, 1, 0) = 4;
    pochart(0, 2, 0) = 5;
    pochart(1, 2, 0) = 6;
    DIM3(podata, 300, 20);
    return;
}



void initialize_building_daga()
{
    DIM4(bddata, 40, 20, 10);
    DIM3(bdref, 10, 7);
    SDIM3(bdrefn, 20, 7);
    bdref(0, 1) = 0;
    bdref(1, 1) = 1500;
    bdref(0, 2) = 1;
    bdref(1, 2) = 2500;
    bdref(0, 3) = 2;
    bdref(1, 3) = 2500;
    bdref(0, 4) = 3;
    bdref(1, 4) = 2500;
    bdref(0, 5) = 4;
    bdref(1, 5) = 2500;
    bdref(0, 6) = 5;
    bdref(1, 6) = 2500;
    bdrefn(1) = lang(u8"炭鉱"s, u8"Mine"s);
    bdrefn(2) = lang(u8"畑"s, u8"Crop"s);
    bdrefn(3) = lang(u8"アトリエ"s, u8"Art Atelier"s);
    bdrefn(4) = lang(u8"寺院"s, u8"Temple"s);
    bdrefn(5) = lang(u8"盗賊の隠れ家"s, u8"Smuggler's Hideout"s);
    bdrefn(6) = lang(u8"灯台"s, u8"Light House"s);
    return;
}



void initialize_character()
{
    if (mode != 1)
    {
        add_quality_parentheses();
    }
    cdata[rc].state = 1;
    cdata[rc].interest = 100;
    cdata[rc].impression = 50;
    cdata[rc].vision_distance = 14;
    if (cdata[rc].id == 205)
    {
        cdata[rc].image = rnd(33) * 2 + cdata[rc].sex + 1;
    }
    if (rc == 0)
    {
        cdata[rc].nutrition = 9000;
    }
    else
    {
        cdata[rc].nutrition = 5000 + rnd(4000);
    }
    cdata[rc].height = cdata[rc].height + rnd((cdata[rc].height / 5 + 1))
        - rnd((cdata[rc].height / 5 + 1));
    cdata[rc].weight =
        cdata[rc].height * cdata[rc].height * (rnd(6) + 18) / 10000;
    label_1456(rc);
    label_1512(rc);
    if (cdata[rc].portrait == 0)
    {
        cdata[rc].portrait = rnd(32);
    }
    cdata[rc].personality = rnd(4);
    cdata[rc].talk_type = rnd(7);
    label_1530();
    chara_refresh(rc);
    ++gdata_other_character_count;
    cdata[rc].hp = cdata[rc].max_hp;
    cdata[rc].mp = cdata[rc].max_mp;
    cdata[rc].sp = cdata[rc].max_sp;
    if (rc == 0)
    {
        initialize_fovmap_and_fovlist();
        gdata_initial_cart_limit = 80000;
        gdata_current_cart_limit = gdata_initial_cart_limit;
    }
    if (cdata[rc].has_lay_hand())
    {
        cdata[rc].is_lay_hand_available() = true;
    }
    cm = 0;
    return;
}


void initialize_pc_character()
{
    cdata[0].quality = 2;
    cdata[0].relationship = 10;
    cdata[0].original_relationship = 10;
    cdata[0].has_own_sprite() = true;
    flt();
    itemcreate(0, 333, -1, -1, 0);
    inv[ci].number = 8;
    flt();
    itemcreate(0, 233, -1, -1, 0);
    inv[ci].number = 4;
    flt();
    itemcreate(0, 31, -1, -1, 0);
    inv[ci].number = 2;
    if (sdata(150, 0) == 0)
    {
        flt();
        itemcreate(0, 68, -1, -1, 0);
        inv[ci].number = 3;
    }
    if (cdatan(3, 0) == u8"pianist"s)
    {
        flt();
        itemcreate(0, 88, -1, -1, 0);
    }
    if (cdatan(3, 0) == u8"farmer"s)
    {
        flt();
        itemcreate(0, 256, -1, -1, 0);
    }
    if (cdatan(3, 0) == u8"wizard"s || cdatan(3, 0) == u8"warmage"s)
    {
        flt();
        itemcreate(0, 116, -1, -1, 0);
        flt();
        itemcreate(0, 257, -1, -1, 0);
        inv[ci].number = 3;
    }
    if (cdatan(3, 0) == u8"priest"s)
    {
        flt();
        itemcreate(0, 249, -1, -1, 0);
        inv[ci].number = 3;
        flt();
        itemcreate(0, 378, -1, -1, 0);
    }
    gain_race_feat();
    cdata[0].skill_bonus = 5 + trait(154);
    cdata[0].total_skill_bonus = 5 + trait(154);
    for (const auto& cnt : items(0))
    {
        if (inv[cnt].number == 0)
        {
            continue;
        }
        inv[cnt].identification_state =
            identification_state_t::completely_identified;
    }
    chara_refresh(0);
    return;
}



int randskill()
{
    return rnd(40) + 150;
}



int randattb()
{
    return rnd(8) + 10;
}



void clear_trait_data()
{
    DIM2(traitref, 10);
    SDIM3(traitrefn, 80, 9);
    SDIM3(traitrefn2, 20, 6);
    return;
}




int iequiploc(int ci)
{
    switch (the_item_db[inv[ci].id]->category)
    {
    case 12000: return 1;
    case 34000: return 2;
    case 20000: return 3;
    case 16000: return 4;
    case 10000: return 5;
    case 14000: return 5;
    case 32000: return 6;
    case 22000: return 7;
    case 18000: return 9;
    case 24000: return 10;
    case 25000: return 11;
    case 19000: return 8;
    default: return 0;
    }
}



void setunid(int prm_282, int prm_283)
{
    cdata[prm_282].cnpc_id = prm_283;
    cdata[prm_282].image = 825 + prm_283;
    cdata[prm_282].image += 0;
    return;
}



int getunid(int prm_284)
{
    f_at_m14 = 0;
    for (int cnt = 0, cnt_end = (usernpcmax); cnt < cnt_end; ++cnt)
    {
        if (cdatan(5, prm_284) == userdatan(0, cnt))
        {
            setunid(prm_284, cnt);
            f_at_m14 = 1;
            break;
        }
    }
    if (f_at_m14 == 0)
    {
        cdata[prm_284].image = 1;
        cdata[prm_284].cnpc_id = usernpcmax;
        cdatan(0, prm_284) = lang(u8"残りカス"s, u8"a garbage"s);
    }
    return f_at_m14;
}



int findunid(const std::string& prm_285)
{
    f_at_m14 = 0;
    for (int cnt = 0, cnt_end = (usernpcmax); cnt < cnt_end; ++cnt)
    {
        if (prm_285 == userdatan(0, cnt))
        {
            f_at_m14 = 1 + cnt;
            break;
        }
    }
    return f_at_m14;
}



int roundmargin(int x, int y)
{
    if (x > y)
        return x - rnd(x - y);
    else if (x < y)
        return x + rnd(y - x);
    else
        return x;
}



void auto_turn(int delay)
{
    if (cc != 0)
        return;

    autoturn = 1;
    if (config::instance().autoturn == 0)
    {
        await(delay);
        ++scrturn;
    }
    if (config::instance().autoturn != 2 || firstautoturn == 1)
    {
        screenupdate = -1;
        update_screen();
    }
    if (config::instance().autoturn == 0)
    {
        redraw();
    }
}



std::string elename(int prm_348)
{
    if (efid == 614)
    {
        return lang(u8"飢えた"s, u8"starving"s);
    }
    if (efid == 613)
    {
        return lang(u8"腐った"s, u8"rotten"s);
    }
    if (efid == 617)
    {
        return lang(u8"恐ろしい"s, u8"fearful"s);
    }
    if (efid == 618)
    {
        return lang(u8"柔らかい"s, u8"silky"s);
    }
    if (prm_348 == 50)
    {
        return lang(u8"燃える"s, u8"burning"s);
    }
    if (prm_348 == 51)
    {
        return lang(u8"冷たい"s, u8"icy"s);
    }
    if (prm_348 == 52)
    {
        return lang(u8"放電する"s, u8"electric"s);
    }
    if (prm_348 == 54)
    {
        return lang(u8"霊的な"s, u8"psychic"s);
    }
    if (prm_348 == 58)
    {
        return lang(u8"痺れる"s, u8"numb"s);
    }
    if (prm_348 == 57)
    {
        return lang(u8"震える"s, u8"shivering"s);
    }
    if (prm_348 == 55)
    {
        return lang(u8"毒の"s, u8"poisonous"s);
    }
    if (prm_348 == 56)
    {
        return lang(u8"地獄の"s, u8"infernal"s);
    }
    if (prm_348 == 59)
    {
        return lang(u8"混沌の"s, u8"chaotic"s);
    }
    if (prm_348 == 53)
    {
        return lang(u8"暗黒の"s, u8"gloomy"s);
    }
    if (prm_348 == 61)
    {
        return lang(u8"出血の"s, u8"cut"s);
    }
    if (prm_348 == 62)
    {
        return lang(u8"エーテルの"s, u8"ether"s);
    }
    return u8"?"s;
}



std::string txttargetlevel(int cc, int tc)
{
    int x = cdata[cc].level;
    int y = cdata[tc].level;
    if (x * 20 < y)
    {
        return lang(
            u8"相手が巨人だとすれば、あなたは蟻のフン以下だ。",
            u8"If "s + he(tc)
                + u8" is a giant, you are less than a dropping of an ant.");
    }
    else if (x * 10 < y)
    {
        return lang(
            u8"絶対に勝てない相手だ。",
            cnven(he(tc)) + u8" can mince you with " + his(tc)
                + u8" eyes closed.");
    }
    else if (x * 5 < y)
    {
        return lang(
            u8"確実に殺されるだろう。",
            u8"You will get killed, a hundred percent sure.");
    }
    else if (x * 3 < y)
    {
        return lang(
            u8"奇跡が起きなければ殺されるだろう。",
            u8"You will get killed unless miracles happen.");
    }
    else if (x * 2 < y)
    {
        return lang(
            u8"少なくとも、あなたの倍は強そうだ。",
            cnven(he(tc)) + u8" " + is(tc)
                + u8" at least twice stronger than you.");
    }
    else if (x * 3 / 2 < y)
    {
        return lang(
            u8"相手はかなり強そうだ。", u8"The opponent looks quite strong.");
    }
    else if (x < y)
    {
        return lang(u8"勝てない相手ではない。", u8"Won't be an easy fight.");
    }
    else if (x / 3 * 2 < y)
    {
        return lang(u8"たぶん勝てそうだ。", u8"You will probably win.");
    }
    else if (x / 2 < y)
    {
        return lang(
            u8"負ける気はしない。",
            cnven(he(tc)) + u8" " + is(tc) + u8" an easy opponent.");
    }
    else if (x / 3 < y)
    {
        return lang(
            u8"目隠ししていても勝てそうだ。",
            u8"You bet you can beat "s + him(tc) + u8" with your eyes closed.");
    }
    else
    {
        return lang(
            u8"目隠しして座っていても勝てる。",
            u8"You can absolutely beat "s + him(tc)
                + u8" with your eyes closed and arms crossed.");
    }
}



std::string mapfile(int prm_365)
{
    if (adata(16, prm_365) == 5)
    {
        return u8"vernis"s;
    }
    if (adata(16, prm_365) == 11)
    {
        return u8"kapul"s;
    }
    if (adata(16, prm_365) == 12)
    {
        return u8"yowyn"s;
    }
    if (adata(16, prm_365) == 14)
    {
        return u8"rogueden"s;
    }
    if (adata(16, prm_365) == 15)
    {
        return u8"palmia"s;
    }
    if (adata(16, prm_365) == 33)
    {
        return u8"noyel"s;
    }
    if (adata(16, prm_365) == 36)
    {
        return u8"lumiest"s;
    }
    return u8"vernis"s;
}



void initialize_picfood()
{
    DIM3(picfood, 10, 9);
    picfood(0, 8) = 230;
    picfood(1, 8) = 230;
    picfood(2, 8) = 230;
    picfood(3, 8) = 190;
    picfood(4, 8) = 229;
    picfood(5, 8) = 190;
    picfood(6, 8) = 342;
    picfood(7, 8) = 168;
    picfood(8, 8) = 347;
    picfood(9, 8) = 194;
    picfood(0, 1) = 230;
    picfood(1, 1) = 230;
    picfood(2, 1) = 230;
    picfood(3, 1) = 195;
    picfood(4, 1) = 227;
    picfood(5, 1) = 167;
    picfood(6, 1) = 167;
    picfood(7, 1) = 194;
    picfood(8, 1) = 229;
    picfood(9, 1) = 227;
    picfood(0, 2) = 230;
    picfood(1, 2) = 230;
    picfood(2, 2) = 230;
    picfood(3, 2) = 229;
    picfood(4, 2) = 342;
    picfood(5, 2) = 194;
    picfood(6, 2) = 229;
    picfood(7, 2) = 342;
    picfood(8, 2) = 229;
    picfood(9, 2) = 194;
    picfood(0, 3) = 230;
    picfood(1, 3) = 230;
    picfood(2, 3) = 230;
    picfood(3, 3) = 229;
    picfood(4, 3) = 346;
    picfood(5, 3) = 346;
    picfood(6, 3) = 345;
    picfood(7, 3) = 345;
    picfood(8, 3) = 347;
    picfood(9, 3) = 346;
    picfood(0, 4) = 230;
    picfood(1, 4) = 230;
    picfood(2, 4) = 230;
    picfood(3, 4) = 108;
    picfood(4, 4) = 346;
    picfood(5, 4) = 110;
    picfood(6, 4) = 346;
    picfood(7, 4) = 347;
    picfood(8, 4) = 347;
    picfood(9, 4) = 347;
    picfood(0, 5) = 230;
    picfood(1, 5) = 230;
    picfood(2, 5) = 229;
    picfood(3, 5) = 343;
    picfood(4, 5) = 344;
    picfood(5, 5) = 344;
    picfood(6, 5) = 343;
    picfood(7, 5) = 343;
    picfood(8, 5) = 344;
    picfood(9, 5) = 343;
    picfood(0, 6) = 230;
    picfood(1, 6) = 230;
    picfood(2, 6) = 230;
    picfood(3, 6) = 228;
    picfood(4, 6) = 342;
    picfood(5, 6) = 342;
    picfood(6, 6) = 228;
    picfood(7, 6) = 341;
    picfood(8, 6) = 228;
    picfood(9, 6) = 228;
    picfood(0, 7) = 230;
    picfood(1, 7) = 230;
    picfood(2, 7) = 110;
    picfood(3, 7) = 108;
    picfood(4, 7) = 110;
    picfood(5, 7) = 112;
    picfood(6, 7) = 114;
    picfood(7, 7) = 113;
    picfood(8, 7) = 115;
    picfood(9, 7) = 111;
    return;
}



void finish_elona()
{
    int ieopen = 0;
    int ie_event = 0;
    int ie = 0;
    if (ieopen)
    {
        delcom(ie_event);
        delcom(ie);
    }
    if (config::instance().autonumlock)
    {
        keybd_event(144);
        keybd_event(144, 0, 2);
    }
    if (mutex_handle != 0)
    {
        CloseHandle(mutex_handle);
    }
    end();
    return;
}



void load_pcc_part(int cc, int body_part, const char* body_part_str)
{
    const auto filepath = filesystem::dir::graphic()
        / (u8"pcc_"s + body_part_str + (pcc(body_part, cc) % 1000) + u8".bmp");
    if (!fs::exists(filepath))
        return;

    pos(128, 0);
    picload(filepath, 1);
    boxf(256, 0, 384, 198);
    gmode(4, -1, -1, 256);
    pget(128, 0);
    pos(256, 0);
    gcopy(10 + cc, 128, 0, 128, 198);
    pos(256, 0);
    gfini(128, 198);
    gfdec2(
        c_col(0, pcc(body_part, cc) / 1000),
        c_col(1, pcc(body_part, cc) / 1000),
        c_col(2, pcc(body_part, cc) / 1000));
    gmode(2);
    pos(0, 0);
    gcopy(10 + cc, 256, 0, 128, 198);
}



void set_pcc_depending_on_equipments(int cc, int ci)
{
    int item_appearance = the_item_db[inv[ci].id]->appearance;
    if (item_appearance == 0)
    {
        item_appearance = 1;
    }
    switch (iequiploc(ci))
    {
    case 3: pcc(4, cc) = item_appearance + inv[ci].color * 1000; break;
    case 4: pcc(2, cc) = item_appearance + inv[ci].color * 1000; break;
    case 7: pcc(8, cc) = item_appearance + inv[ci].color * 1000; break;
    case 8: pcc(5, cc) = item_appearance + inv[ci].color * 1000; break;
    case 9: pcc(3, cc) = item_appearance + inv[ci].color * 1000; break;
    default: break;
    }
}


void chara_preparepic(int prm_618, int prm_619)
{
    int p_at_m83 = 0;
    if (prm_619 == 0)
    {
        p_at_m83 = prm_618 / 1000;
    }
    else
    {
        p_at_m83 = prm_619;
    }
    gsel(5);
    boxf(0, 960, chipc(2, prm_618), chipc(3, prm_618) + 960);
    pos(0, 960);
    set_color_mod(
        255 - c_col(0, p_at_m83),
        255 - c_col(1, p_at_m83),
        255 - c_col(2, p_at_m83));
    gcopy(
        5,
        chipc(0, prm_618),
        chipc(1, prm_618),
        chipc(2, prm_618),
        chipc(3, prm_618));
    set_color_mod(255, 255, 255);
    gfini(chipc(2, prm_618), chipc(3, prm_618));
    gfdec2(c_col(0, p_at_m83), c_col(1, p_at_m83), c_col(2, p_at_m83));
    gsel(0);
    return;
}



void create_pcpic(int cc, bool prm_410)
{
    buffer(10 + cc, 384, 198);
    boxf();

    if (pcc(15, cc) == 0)
    {
        pcc(15, cc) = cdata[cc].sex + 1;
        pcc(14, cc) = cdata[cc].sex + 7;
        pcc(1, cc) = 2 + rnd(21) * 1000;
        pcc(9, cc) = 1 + rnd(21) * 1000;
        pcc(7, cc) = 1 + rnd(21) * 1000;
        pcc(16, cc) = 1;
    }

    if (prm_410)
    {
        pcc(4, cc) = 0;
        pcc(2, cc) = 0;
        pcc(6, cc) = 0;
        pcc(3, cc) = 0;
        pcc(8, cc) = 0;
        pcc(5, cc) = 0;
        for (int i = 0; i < 30; ++i)
        {
            if (cdata_body_part(cc, i) % 10000 != 0)
            {
                set_pcc_depending_on_equipments(
                    cc, cdata_body_part(cc, i) % 10000 - 1);
            }
        }
    }

    pcc(10, cc) = pcc(1, cc) / 1000 * 1000 + pcc(10, cc) % 1000;
    pcc(14, cc) = pcc(15, cc) / 1000 * 1000 + pcc(14, cc) % 1000;
    if (prm_410)
    {
        if (pcc(24, cc) == 0)
        {
            load_pcc_part(cc, 4, u8"mantle_");
        }
    }
    load_pcc_part(cc, 1, u8"hairbk_");
    if (cc == 0 && gdata_mount != 0 && pcc(16, cc) != 0)
    {
        load_pcc_part(cc, 16, u8"ridebk_");
    }
    else
    {
        load_pcc_part(cc, 15, u8"body_");
    }
    load_pcc_part(cc, 14, u8"eye_");
    if (cc != 0 || gdata_mount == 0 || pcc(16, cc) == 0)
    {
        load_pcc_part(cc, 7, u8"pants_");
    }
    load_pcc_part(cc, 9, u8"cloth_");
    if (prm_410)
    {
        if (pcc(20, cc) == 0)
        {
            load_pcc_part(cc, 2, u8"chest_");
        }
        if ((cc != 0 || gdata_mount == 0 || pcc(16, cc) == 0)
            && pcc(21, cc) == 0)
        {
            load_pcc_part(cc, 3, u8"leg_");
        }
        if (pcc(22, cc) == 0)
        {
            load_pcc_part(cc, 5, u8"belt_");
        }
        if (pcc(23, cc) == 0)
        {
            load_pcc_part(cc, 8, u8"glove_");
        }
    }
    if (cc == 0)
    {
        if (gdata_mount != 0)
        {
            load_pcc_part(cc, 16, u8"ride_");
        }
    }
    if (prm_410)
    {
        if (pcc(24, cc) == 0)
        {
            load_pcc_part(cc, 4, u8"mantlebk_");
        }
    }
    load_pcc_part(cc, 1, u8"hair_");
    load_pcc_part(cc, 10, u8"subhair_");
    load_pcc_part(cc, 11, u8"etc_");
    load_pcc_part(cc, 12, u8"etc_");
    load_pcc_part(cc, 13, u8"etc_");

    gsel(0);
}



void addefmap(int x, int y, int efmap0, int efmap1, int efmap2, int efmap3)
{
    efmap(0, x, y) = efmap0;
    efmap(1, x, y) = efmap1;
    efmap(2, x, y) = efmap2;
    efmap(3, x, y) = efmap3;
}



void initialize_map_chip()
{
    DIM3(chipm, 8, 825);
    if (mdata(2) == 0)
    {
        chipm(5, 233) = 0;
        chipm(6, 233) = 0;
        for (int cnt = 26; cnt < 33; ++cnt)
        {
            chipm(0, cnt) = 4;
        }
        chipm(0, 568) = 4;
        chipm(0, 569) = 4;
        chipm(0, 570) = 4;
        for (int cnt = 99; cnt < 132; ++cnt)
        {
            chipm(0, cnt) = 7;
        }
        for (int cnt = 165; cnt < 198; ++cnt)
        {
            chipm(0, cnt) = 8;
        }
        for (int cnt = 198; cnt < 231; ++cnt)
        {
            chipm(0, cnt) = 4;
        }
        for (int cnt = 594; cnt < 599; ++cnt)
        {
            chipm(0, cnt) = 7;
        }
        for (int cnt = 599; cnt < 604; ++cnt)
        {
            chipm(0, cnt) = 8;
        }
        for (int cnt = 107; cnt < 119; ++cnt)
        {
            chipm(1, cnt) = 9;
        }
        for (int cnt = 173; cnt < 185; ++cnt)
        {
            chipm(1, cnt) = 9;
        }
        for (int cnt = 206; cnt < 218; ++cnt)
        {
            chipm(1, cnt) = 9;
        }
        chipm(0, 604) = 10;
        for (int cnt = 605; cnt < 617; ++cnt)
        {
            chipm(0, cnt) = 10;
            chipm(1, cnt) = 9;
        }
        for (int cnt = 396; cnt < 825; ++cnt)
        {
            chipm(7, cnt) = 5;
        }
        for (int cnt = 264; cnt < 297; ++cnt)
        {
            chipm(7, cnt) = 4;
        }
        chipm(5, 135) = 8;
        chipm(5, 137) = 16;
        chipm(5, 140) = 6;
        chipm(5, 145) = 16;
        chipm(5, 149) = 16;
    }
    if (mdata(2) == 1)
    {
        for (int cnt = 396; cnt < 825; ++cnt)
        {
            chipm(7, cnt) = 5;
        }
        chipm(5, 233) = 56;
        chipm(6, 233) = 48;
        chipm(7, 594) = 4;
        chipm(7, 628) = 4;
        chipm(7, 637) = 4;
        chipm(7, 641) = 4;
        chipm(7, 733) = 4;
        for (int cnt = 45; cnt < 61; ++cnt)
        {
            chipm(0, cnt) = 4;
        }
        chipm(0, 82) = 4;
        chipm(0, 83) = 4;
        chipm(0, 84) = 4;
        for (int cnt = 462; cnt < 528; ++cnt)
        {
            chipm(2, cnt) = 1;
            chipm(2, cnt - 66) = 1;
        }
        for (int cnt = 462; cnt < 495; ++cnt)
        {
            chipm(2, cnt) = 2;
            chipm(2, cnt - 66) = 2;
        }
        chipm(0, 29) = 1;
        chipm(0, 30) = 2;
        chipm(0, 31) = 2;
        chipm(0, 464) = 6;
        chipm(3, 550) = 2;
        chipm(2, 550) = 1;
        chipm(0, 165) = 3;
        chipm(3, 165) = 3;
        chipm(0, 168) = 3;
        chipm(3, 168) = 3;
        chipm(0, 171) = 3;
        chipm(1, 171) = 5;
        chipm(3, 171) = 3;
        chipm(0, 594) = 3;
        chipm(3, 594) = 3;
    }
    if (mdata(2) == 2)
    {
        for (int cnt = 0; cnt < 11; ++cnt)
        {
            int cnt2 = cnt;
            for (int cnt = 0; cnt < 13; ++cnt)
            {
                chipm(0, cnt2 * 33 + cnt + 20) = 4;
            }
        }
        for (int cnt = 33; cnt < 66; ++cnt)
        {
            chipm(0, cnt) = 4;
        }
        for (int cnt = 396; cnt < 825; ++cnt)
        {
            chipm(7, cnt) = 5;
        }
        chipm(5, 233) = 56;
        chipm(6, 233) = 48;
        chipm(7, 594) = 4;
        for (int cnt = 462; cnt < 528; ++cnt)
        {
            chipm(2, cnt) = 1;
            chipm(2, cnt - 66) = 1;
        }
        for (int cnt = 462; cnt < 495; ++cnt)
        {
            chipm(2, cnt) = 2;
            chipm(2, cnt - 66) = 2;
        }
        chipm(3, 550) = 2;
        chipm(2, 550) = 1;
        chipm(0, 165) = 3;
        chipm(3, 165) = 3;
        chipm(0, 168) = 3;
        chipm(3, 168) = 3;
        chipm(0, 171) = 3;
        chipm(1, 171) = 5;
        chipm(3, 171) = 3;
        chipm(0, 594) = 3;
        chipm(3, 594) = 3;
        chipm(2, 476) = 0;
        chipm(2, 509) = 0;
    }
    return;
}



void initialize_item_chip()
{
    initialize_mef();
    SDIM3(tname, 16, 11);
    tname(1) = lang(u8"日干し岩"s, u8"a dryrock"s);
    tname(2) = lang(u8"畑"s, u8"a field"s);
    DIM3(chipc, 6, 925);
    DIM3(chipi, 8, 825);
    for (int cnt = 0; cnt < 825; ++cnt)
    {
        chipc(0, cnt) = cnt % 33 * inf_tiles;
        chipc(1, cnt) = cnt / 33 * inf_tiles;
        chipc(2, cnt) = inf_tiles;
        chipc(3, cnt) = inf_tiles;
        chipc(4, cnt) = 16;
        chipi(0, cnt) = cnt % 33 * inf_tiles;
        chipi(1, cnt) = cnt / 33 * inf_tiles;
        chipi(2, cnt) = inf_tiles;
        chipi(3, cnt) = inf_tiles;
        chipi(4, cnt) = 0;
        chipi(5, cnt) = 8;
        chipi(6, cnt) = 40;
    }
    for (int cnt = 825; cnt < 925; ++cnt)
    {
        chipc(0, cnt) = cnt % 33 * inf_tiles;
        chipc(1, cnt) = cnt / 33 * inf_tiles;
        chipc(2, cnt) = inf_tiles;
        chipc(3, cnt) = inf_tiles;
        chipc(4, cnt) = 16;
    }
    chipi(4, 24) = 16;
    chipi(4, 30) = 16;
    chipi(4, 72) = 22;
    chipi(4, 73) = 22;
    chipi(4, 74) = 22;
    chipi(4, 75) = 22;
    chipi(4, 76) = 22;
    chipi(5, 76) = 28;
    chipi(4, 78) = 22;
    chipi(4, 80) = 22;
    chipi(4, 85) = 8;
    chipi(5, 85) = 8;
    chipi(6, 85) = 150;
    chipi(4, 87) = 22;
    chipi(4, 88) = 22;
    chipi(4, 91) = 22;
    chipi(4, 95) = 22;
    chipi(4, 96) = 22;
    chipi(5, 96) = 18;
    chipi(4, 97) = 22;
    chipi(4, 99) = 22;
    chipi(4, 98) = 22;
    chipi(4, 100) = 22;
    chipi(4, 102) = 22;
    chipi(4, 103) = 22;
    chipi(4, 104) = 22;
    chipi(4, 107) = 22;
    chipi(4, 116) = 22;
    chipi(4, 117) = 22;
    chipi(4, 123) = 12;
    chipi(4, 125) = 12;
    chipi(4, 130) = 12;
    chipi(4, 132) = 22;
    chipi(4, 134) = 12;
    chipi(4, 136) = 12;
    chipi(4, 137) = 22;
    chipi(4, 138) = 22;
    chipi(4, 139) = 22;
    chipi(4, 142) = 22;
    chipi(5, 142) = 36;
    chipi(4, 143) = 22;
    chipi(5, 143) = 36;
    chipi(4, 145) = 22;
    chipi(4, 146) = 22;
    chipi(4, 147) = 22;
    chipi(4, 150) = 22;
    chipi(5, 150) = 8;
    chipi(6, 150) = 70;
    chipi(4, 151) = 22;
    chipi(4, 156) = 8;
    chipi(5, 156) = 20;
    chipi(6, 156) = 150;
    chipi(4, 158) = 8;
    chipi(5, 158) = 24;
    chipi(6, 158) = 150;
    chipi(4, 159) = 8;
    chipi(5, 159) = 24;
    chipi(6, 159) = 150;
    chipi(4, 160) = 22;
    chipi(4, 163) = 22;
    chipi(5, 163) = 16;
    chipi(4, 164) = 22;
    chipi(4, 118) = 22;
    chipi(4, 166) = 22;
    chipi(4, 197) = 22;
    chipi(5, 197) = 24;
    chipi(4, 232) = 22;
    chipi(4, 248) = 22;
    chipi(4, 234) = 22;
    chipi(5, 234) = 24;
    chipi(4, 235) = 22;
    chipi(5, 235) = 36;
    chipi(4, 236) = 22;
    chipi(4, 242) = 22;
    chipi(5, 242) = 8;
    chipi(6, 242) = 250;
    chipi(4, 259) = 8;
    chipi(5, 259) = 12;
    chipi(4, 260) = 22;
    chipi(5, 260) = 22;
    chipi(4, 262) = 22;
    chipi(4, 263) = 22;
    chipi(4, 264) = 22;
    chipi(4, 266) = 22;
    chipi(5, 266) = 24;
    chipi(4, 270) = 22;
    chipi(4, 272) = 22;
    chipi(5, 272) = 34;
    chipi(4, 273) = 22;
    chipi(4, 276) = 22;
    chipi(4, 277) = 8;
    chipi(4, 278) = 22;
    chipi(4, 279) = 22;
    chipi(4, 281) = 22;
    chipi(5, 281) = 40;
    chipi(4, 282) = 22;
    chipi(4, 285) = 22;
    chipi(4, 286) = 22;
    chipi(4, 288) = 22;
    chipi(5, 288) = 18;
    chipi(4, 291) = 22;
    chipi(4, 292) = 22;
    chipi(5, 292) = 32;
    chipi(4, 293) = 22;
    chipi(4, 295) = 22;
    chipi(4, 296) = 22;
    chipi(4, 299) = 22;
    chipi(5, 299) = 8;
    chipi(6, 299) = 250;
    chipi(4, 300) = 22;
    chipi(4, 320) = 22;
    chipi(4, 321) = 22;
    chipi(4, 325) = 22;
    chipi(4, 327) = 22;
    chipi(4, 331) = 22;
    chipi(4, 332) = 22;
    chipi(4, 348) = 22;
    chipi(4, 353) = 48;
    chipi(5, 354) = 8;
    chipi(6, 354) = 20;
    chipi(4, 360) = 0;
    chipi(5, 360) = 2;
    chipi(6, 360) = 1;
    chipi(4, 364) = 22;
    chipi(5, 364) = 8;
    chipi(6, 364) = 250;
    chipi(4, 367) = 22;
    chipi(4, 368) = 22;
    chipi(4, 370) = 8;
    chipi(4, 372) = 22;
    chipi(4, 373) = 22;
    chipi(4, 375) = 22;
    chipi(4, 376) = 22;
    chipi(4, 377) = 22;
    chipi(4, 395) = 22;
    chipi(4, 378) = 48;
    chipi(4, 379) = 48;
    chipi(4, 380) = 8;
    chipi(4, 381) = 22;
    chipi(4, 382) = 48;
    chipi(4, 384) = 22;
    chipi(4, 442) = 8;
    chipi(5, 442) = 24;
    chipi(6, 442) = 100;
    chipi(4, 507) = 22;
    chipi(4, 508) = 22;
    chipi(4, 506) = 22;
    chipi(4, 510) = 22;
    chipi(4, 511) = 22;
    chipi(5, 511) = 28;
    chipi(4, 512) = 22;
    chipi(4, 513) = 22;
    chipi(4, 541) = 22;
    chipi(5, 541) = 36;
    chipi(4, 543) = 22;
    chipi(4, 544) = 48;
    chipi(4, 545) = 0;
    chipi(5, 545) = 0;
    chipi(6, 545) = 0;
    chipi(4, 627) = 22;
    chipi(4, 637) = 22;
    chipi(4, 639) = 48;
    chipi(4, 640) = 22;
    chipi(5, 640) = 34;
    chipi(4, 641) = 22;
    chipi(4, 642) = 22;
    chipi(4, 643) = 22;
    chipi(4, 644) = 32;
    chipi(4, 646) = 22;
    chipi(4, 647) = 22;
    chipi(4, 648) = 22;
    chipi(4, 650) = 22;
    chipi(4, 651) = 48;
    chipi(4, 652) = 48;
    chipi(4, 653) = 32;
    chipi(4, 655) = 22;
    chipi(4, 659) = 22;
    chipi(4, 662) = 22;
    chipi(6, 664) = 0;
    chipi(6, 665) = 0;
    chipi(6, 667) = 0;
    chipi(4, 668) = 8;
    chipi(4, 669) = 8;
    chipi(4, 672) = 38;
    chipi(4, 674) = 22;
    chipi(5, 674) = 24;
    chipi(4, 675) = 16;
    chipi(4, 676) = 40;
    chipi(4, 677) = 16;
    chipi(5, 677) = 8;
    chipi(6, 677) = 50;
    chipi(6, 679) = 1;
    chipi(3, 523) = inf_tiles * 2;
    chipi(4, 523) = inf_tiles + 16;
    chipi(5, 523) = 40;
    chipi(6, 523) = 6;
    chipi(3, 524) = inf_tiles * 2;
    chipi(4, 524) = inf_tiles + 16;
    chipi(5, 524) = 65;
    chipi(3, 525) = inf_tiles * 2;
    chipi(4, 525) = inf_tiles + 20;
    chipi(3, 526) = inf_tiles * 2;
    chipi(4, 526) = inf_tiles + 20;
    chipi(3, 527) = inf_tiles * 2;
    chipi(4, 527) = inf_tiles + 20;
    chipi(3, 531) = inf_tiles * 2;
    chipi(4, 531) = inf_tiles + 16;
    chipi(5, 531) = 40;
    chipi(3, 563) = inf_tiles * 2;
    chipi(4, 563) = inf_tiles + 15;
    chipi(3, 566) = inf_tiles * 2;
    chipi(4, 566) = inf_tiles + 12;
    chipi(3, 567) = inf_tiles * 2;
    chipi(4, 567) = inf_tiles + 12;
    chipi(3, 568) = inf_tiles * 2;
    chipi(4, 568) = inf_tiles + 12;
    chipi(3, 569) = inf_tiles * 2;
    chipi(4, 569) = inf_tiles + 12;
    chipi(5, 569) = 70;
    chipi(6, 569) = 6;
    chipi(3, 570) = inf_tiles * 2;
    chipi(4, 570) = inf_tiles + 20;
    chipi(3, 571) = inf_tiles * 2;
    chipi(4, 571) = inf_tiles + 20;
    chipi(5, 571) = 64;
    chipi(3, 572) = inf_tiles * 2;
    chipi(4, 572) = inf_tiles + 20;
    chipi(3, 573) = inf_tiles * 2;
    chipi(4, 573) = inf_tiles + 20;
    chipi(3, 574) = inf_tiles * 2;
    chipi(4, 574) = inf_tiles + 20;
    chipi(3, 575) = inf_tiles * 2;
    chipi(4, 575) = inf_tiles + 20;
    chipi(3, 576) = inf_tiles * 2;
    chipi(4, 576) = inf_tiles + 20;
    chipi(3, 577) = inf_tiles * 2;
    chipi(4, 577) = inf_tiles + 20;
    chipi(5, 577) = 48;
    chipi(6, 577) = 6;
    chipi(3, 578) = inf_tiles * 2;
    chipi(4, 578) = inf_tiles + 20;
    chipi(3, 579) = inf_tiles * 2;
    chipi(4, 579) = inf_tiles + 20;
    chipi(3, 580) = inf_tiles * 2;
    chipi(4, 580) = inf_tiles + 20;
    chipi(5, 580) = 40;
    chipi(6, 580) = 6;
    chipi(3, 581) = inf_tiles * 2;
    chipi(4, 581) = inf_tiles + 20;
    chipi(3, 582) = inf_tiles * 2;
    chipi(4, 582) = inf_tiles + 20;
    chipi(3, 583) = inf_tiles * 2;
    chipi(4, 583) = inf_tiles + 20;
    chipi(5, 583) = 44;
    chipi(6, 583) = 6;
    chipi(3, 584) = inf_tiles * 2;
    chipi(4, 584) = inf_tiles + 20;
    chipi(5, 584) = 40;
    chipi(6, 584) = 6;
    chipi(3, 585) = inf_tiles * 2;
    chipi(4, 585) = inf_tiles + 20;
    chipi(3, 586) = inf_tiles * 2;
    chipi(4, 586) = inf_tiles + 20;
    chipi(5, 586) = 44;
    chipi(6, 586) = 6;
    chipi(3, 587) = inf_tiles * 2;
    chipi(4, 587) = inf_tiles + 20;
    chipi(3, 588) = inf_tiles * 2;
    chipi(4, 588) = inf_tiles + 20;
    chipi(3, 589) = inf_tiles * 2;
    chipi(4, 589) = inf_tiles + 20;
    chipi(3, 590) = inf_tiles * 2;
    chipi(4, 590) = inf_tiles + 20;
    chipi(3, 591) = inf_tiles * 2;
    chipi(4, 591) = inf_tiles + 20;
    chipi(3, 592) = inf_tiles * 2;
    chipi(4, 592) = inf_tiles + 20;
    chipi(3, 593) = inf_tiles * 2;
    chipi(4, 593) = inf_tiles + 20;
    chipi(3, 680) = inf_tiles * 2;
    chipi(4, 680) = inf_tiles + 16;
    chipi(3, 681) = inf_tiles * 2;
    chipi(4, 681) = inf_tiles + 16;
    chipi(3, 682) = inf_tiles * 2;
    chipi(4, 682) = inf_tiles + 16;
    chipi(3, 683) = inf_tiles * 2;
    chipi(4, 683) = inf_tiles + 22;
    chipi(5, 683) = 50;
    chipi(3, 685) = inf_tiles * 2;
    chipi(4, 685) = inf_tiles + 52;
    chipi(5, 685) = 50;
    chipi(6, 685) = 18;
    chipi(3, 684) = inf_tiles * 2;
    chipi(4, 684) = inf_tiles + 52;
    chipi(5, 684) = 50;
    chipi(6, 684) = 18;
    chipi(3, 686) = inf_tiles * 2;
    chipi(4, 686) = inf_tiles + 16;
    chipi(3, 687) = inf_tiles * 2;
    chipi(4, 687) = inf_tiles + 16;
    chipi(3, 688) = inf_tiles * 2;
    chipi(4, 688) = inf_tiles + 16;
    chipi(3, 689) = inf_tiles * 2;
    chipi(4, 689) = inf_tiles + 16;
    chipi(3, 690) = inf_tiles * 2;
    chipi(4, 690) = inf_tiles + 16;
    chipi(3, 691) = inf_tiles * 2;
    chipi(4, 691) = inf_tiles + 16;
    chipi(5, 691) = 48;
    chipi(3, 692) = inf_tiles * 2;
    chipi(4, 692) = inf_tiles + 16;
    chipi(7, 19) = 2;
    chipi(7, 24) = 3;
    chipi(7, 27) = 3;
    chipi(7, 30) = 2;
    chipi(7, 349) = 3;
    chipi(7, 355) = 3;
    chipc(4, 176) = 8;
    chipc(4, 225) = 29;
    chipc(4, 230) = 12;
    chipc(4, 256) = 16;
    chipc(4, 277) = 29;
    chipc(3, 201) = inf_tiles * 2;
    chipc(4, 201) = inf_tiles + 16;
    chipc(3, 228) = inf_tiles * 2;
    chipc(4, 228) = inf_tiles + 8;
    chipc(3, 231) = inf_tiles * 2;
    chipc(4, 231) = inf_tiles + 16;
    chipc(3, 232) = inf_tiles * 2;
    chipc(4, 232) = inf_tiles + 16;
    chipc(3, 233) = inf_tiles * 2;
    chipc(4, 233) = inf_tiles + 8;
    chipc(3, 297) = inf_tiles * 2;
    chipc(4, 297) = inf_tiles + 16;
    chipc(3, 235) = inf_tiles * 2;
    chipc(4, 235) = inf_tiles + 16;
    chipc(3, 280) = inf_tiles * 2;
    chipc(4, 280) = inf_tiles + 32;
    chipc(3, 338) = inf_tiles * 2;
    chipc(4, 338) = inf_tiles + 32;
    chipc(3, 339) = inf_tiles * 2;
    chipc(4, 339) = inf_tiles + 16;
    chipc(3, 341) = inf_tiles * 2;
    chipc(4, 341) = inf_tiles + 16;
    chipc(3, 342) = inf_tiles * 2;
    chipc(4, 342) = inf_tiles + 12;
    chipc(3, 343) = inf_tiles * 2;
    chipc(4, 343) = inf_tiles + 16;
    chipc(3, 349) = inf_tiles * 2;
    chipc(4, 349) = inf_tiles + 8;
    chipc(3, 351) = inf_tiles * 2;
    chipc(4, 351) = inf_tiles + 8;
    chipc(3, 389) = inf_tiles * 2;
    chipc(4, 389) = inf_tiles + 16;
    chipc(3, 391) = inf_tiles * 2;
    chipc(4, 391) = inf_tiles + 16;
    chipc(3, 393) = inf_tiles * 2;
    chipc(4, 393) = inf_tiles + 16;
    chipc(3, 398) = inf_tiles * 2;
    chipc(4, 398) = inf_tiles + 16;
    chipc(3, 404) = inf_tiles * 2;
    chipc(4, 404) = inf_tiles + 16;
    chipc(3, 405) = inf_tiles * 2;
    chipc(4, 405) = inf_tiles + 16;
    chipc(3, 408) = inf_tiles * 2;
    chipc(4, 408) = inf_tiles + 16;
    chipc(3, 413) = inf_tiles * 2;
    chipc(4, 413) = inf_tiles + 16;
    chipc(3, 429) = inf_tiles * 2;
    chipc(4, 429) = inf_tiles + 8;
    chipc(3, 430) = inf_tiles * 2;
    chipc(4, 430) = inf_tiles + 8;
    chipc(3, 432) = inf_tiles * 2;
    chipc(4, 432) = inf_tiles + 8;
    chipc(3, 433) = inf_tiles * 2;
    chipc(4, 433) = inf_tiles + 8;
    chipc(3, 439) = inf_tiles * 2;
    chipc(4, 439) = inf_tiles + 8;
    chipc(3, 442) = inf_tiles * 2;
    chipc(4, 442) = inf_tiles + 8;
    chipc(3, 447) = inf_tiles * 2;
    chipc(4, 447) = inf_tiles + 16;
    DIM3(deco, 3, 300);
    for (int cnt = 0; cnt < 16; ++cnt)
    {
        deco(0, 1 + cnt * 16) = 0;
        deco(1, 1 + cnt * 16) = 1;
        deco(0, 2 + cnt * 16) = 1;
        deco(1, 2 + cnt * 16) = 2;
        deco(0, 4 + cnt * 16) = 1;
        deco(1, 4 + cnt * 16) = 0;
        deco(0, 6 + cnt * 16) = -1;
        deco(1, 6 + cnt * 16) = 21;
        deco(0, 7 + cnt * 16) = -1;
        deco(1, 7 + cnt * 16) = 30;
        deco(0, 8 + cnt * 16) = 2;
        deco(1, 8 + cnt * 16) = 1;
        deco(0, 9 + cnt * 16) = -1;
        deco(1, 9 + cnt * 16) = 20;
        deco(0, 10 + cnt * 16) = 2;
        deco(1, 10 + cnt * 16) = 2;
        deco(0, 11 + cnt * 16) = -1;
        deco(1, 11 + cnt * 16) = 33;
        deco(0, 12 + cnt * 16) = 2;
        deco(1, 12 + cnt * 16) = 0;
        deco(0, 13 + cnt * 16) = -1;
        deco(1, 13 + cnt * 16) = 32;
        deco(0, 14 + cnt * 16) = -1;
        deco(1, 14 + cnt * 16) = 31;
        deco(0, 15 + cnt * 16) = 3;
        deco(1, 15 + cnt * 16) = 1;
        deco(0, 16 + cnt * 16) = -1;
        deco(1, 16 + cnt * 16) = -1;
        deco(0, 19 + cnt * 16) = 0;
        deco(1, 19 + cnt * 16) = 2;
    }
    deco(2, 49) = 2;
    deco(2, 52) = 2;
    deco(2, 53) = 2;
    deco(2, 97) = 2;
    deco(2, 113) = 2;
    deco(2, 117) = 2;
    deco(2, 164) = 2;
    deco(2, 180) = 2;
    deco(2, 181) = 2;
    deco(2, 145) = 4;
    deco(2, 162) = 4;
    deco(2, 194) = 4;
    deco(2, 195) = 4;
    deco(2, 209) = 4;
    deco(2, 226) = 4;
    deco(2, 243) = 4;
    deco(2, 84) = 3;
    deco(2, 104) = 3;
    deco(2, 196) = 3;
    deco(2, 200) = 3;
    deco(2, 204) = 3;
    deco(2, 212) = 3;
    deco(2, 220) = 3;
    deco(2, 232) = 3;
    deco(2, 236) = 3;
    deco(2, 24) = 1;
    deco(2, 50) = 1;
    deco(2, 56) = 1;
    deco(2, 58) = 1;
    deco(2, 82) = 1;
    deco(2, 114) = 1;
    deco(2, 122) = 1;
    deco(2, 152) = 1;
    deco(2, 184) = 1;
    deco(2, 186) = 1;
    deco(2, 178) = 8;
    deco(2, 241) = 10;
    deco(2, 242) = 8;
    deco(2, 244) = 7;
    deco(0, 16) = -1;
    deco(1, 16) = 1;
    deco(0, 32) = -1;
    deco(1, 32) = 2;
    deco(0, 64) = -1;
    deco(1, 64) = 3;
    deco(0, 128) = -1;
    deco(1, 128) = 4;
    deco(0, 48) = -1;
    deco(1, 48) = 5;
    deco(0, 192) = -1;
    deco(1, 192) = 6;
    deco(0, 96) = -1;
    deco(1, 96) = 7;
    deco(0, 144) = -1;
    deco(1, 144) = 8;
    deco(0, 80) = -1;
    deco(1, 80) = 9;
    deco(0, 160) = -1;
    deco(1, 160) = 10;
    deco(0, 91) = 0;
    deco(1, 91) = 1;
    deco(0, 93) = 0;
    deco(1, 93) = 1;
    deco(0, 21) = 0;
    deco(1, 21) = 0;
    deco(0, 85) = 0;
    deco(1, 85) = 0;
    deco(0, 213) = 0;
    deco(1, 213) = 0;
    deco(0, 149) = 0;
    deco(1, 149) = 0;
    initialize_map_chip();
    return;
}



void csvsort(
    elona_vector1<std::string>& prm_435,
    std::string prm_436,
    int prm_437)
{
    elona_vector1<int> p_at_m40;
    p_at_m40(0) = 0;
    for (int cnt = 0; cnt < 40; ++cnt)
    {
        prm_435(cnt) = "";
        getstr(prm_435(cnt), prm_436, p_at_m40(0), prm_437);
        if (strsize == 0)
        {
            break;
        }
        p_at_m40(0) += strsize;
    }
    return;
}



void load_random_name_table()
{
    std::vector<std::string> lines;
    range::copy(
        fileutil::read_by_line{filesystem::dir::data()
                               / lang(u8"ndata.csv", u8"ndata-e.csv")},
        std::back_inserter(lines));

    SDIM3(randn1, 30, 20);
    SDIM4(rnlist, 20, 15, lines.size());

    for (size_t i = 0; i < lines.size(); ++i)
    {
        csvsort(randn1, lines[i], 44);
        for (size_t j = 0; j < 15; ++j)
        {
            rnlist(j, i) = randn1(j);
        }
    }
}



void load_random_title_table()
{
    std::vector<std::string> lines;
    range::copy(
        fileutil::read_by_line{filesystem::dir::data() / u8"name.csv"},
        std::back_inserter(lines));

    SDIM3(rn1, 15, lines.size());
    SDIM3(rn2, 15, lines.size());

    for (size_t i = 0; i < lines.size(); ++i)
    {
        csvsort(randn1, lines[i], 44);
        rn1(i) = lang(randn1(0), randn1(1));
        rn2(i) = lang(randn1(2), randn1(3));
    }
}



void getinheritance(int prm_440, elona_vector1<int>& prm_441, int& prm_442)
{
    int p_at_m42 = 0;
    int f_at_m42 = 0;
    randomize(inv[prm_440].turn + 1);
    DIM2(prm_441, 15);
    prm_442 = 0;
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        p_at_m42 = rnd(15);
        if (inv[prm_440].enchantments[p_at_m42].id == 0)
        {
            continue;
        }
        f_at_m42 = 0;
        for (int cnt = 0, cnt_end = (prm_442 + 1); cnt < cnt_end; ++cnt)
        {
            if (p_at_m42 == prm_441(cnt))
            {
                f_at_m42 = 1;
                break;
            }
        }
        if (f_at_m42 == 1)
        {
            continue;
        }
        if (rnd(4) > prm_442 || inv[prm_440].enchantments[p_at_m42].power < 0)
        {
            prm_441(prm_442) = p_at_m42;
            ++prm_442;
        }
    }
    randomize();
    return;
}



void flt(int level, int quality)
{
    filtermax = 0;
    fltselect = 0;
    flttypemajor = 0;
    flttypeminor = 0;
    fltnrace = "";
    objlv = level == 0 ? calcobjlv(gdata_current_dungeon_level) : level;
    fixlv = quality == 0 ? calcfixlv(2) : quality;
}



void fltn(const std::string& prm_447)
{
    std::string s2_at_m44;
    int p_at_m44 = 0;
    s2_at_m44 = prm_447;
    p_at_m44 = 0;
    for (int cnt = 0; cnt < 5; ++cnt)
    {
        getstr(filtern(cnt), s2_at_m44, p_at_m44, 47);
        if (strsize == 0)
        {
            break;
        }
        filtern(cnt) = u8"/"s + filtern(cnt) + u8"/"s;
        p_at_m44 += strsize;
        ++filtermax;
    }
    return;
}



int discsetmc()
{
    return rnd(40);
}



int fltsetdungeon()
{
    if (rnd(20) == 0)
    {
        return fsetrare(rnd(length(fsetrare)));
    }
    if (rnd(3) == 0)
    {
        return fsetwear(rnd(length(fsetwear)));
    }
    return fsetitem(rnd(length(fsetitem)));
}



int randomele()
{
    int p_at_m45 = 0;
    int i_at_m45 = 0;
    p_at_m45 = rnd(11) + 50;
    if (p_at_m45 != 1)
    {
        for (int cnt = 0, cnt_end = (the_ability_db[p_at_m45]->sdataref4);
             cnt < cnt_end;
             ++cnt)
        {
            i_at_m45 = rnd(11) + 50;
            if (the_ability_db[i_at_m45]->sdataref4
                < the_ability_db[p_at_m45]->sdataref4)
            {
                if (rnd(2) == 0)
                {
                    p_at_m45 = i_at_m45;
                }
            }
        }
    }
    return p_at_m45;
}



void csvstr2(elona_vector1<std::string>& prm_532, const std::string& prm_533)
{
    std::string word_at_m67;
    int p_at_m67 = 0;
    int i_at_m67 = 0;
    word_at_m67 = prm_533;
    p_at_m67 = 0;
    i_at_m67 = word_at_m67.size();
    for (int cnt = 0; cnt < 40; ++cnt)
    {
        prm_532(cnt) = "";
        getstr(prm_532(cnt), word_at_m67, clamp(p_at_m67, 0, i_at_m67), 44);
        if (strsize > i_at_m67)
        {
            break;
        }
        p_at_m67 += strsize;
    }
    return;
}



int cargocheck()
{
    if (the_item_db[inv[ci].id]->is_cargo == 0)
    {
        return 1;
    }
    if (mdata(6) != 1 && mdata(6) != 5 && mdata(6) != 3 && mdata(6) != 4
        && mdata(6) != 6 && mdata(6) != 2)
    {
        ++msgdup;
        txt(lang(
            u8"荷車の荷物は街か野外でしか操作できない。"s,
            u8"You can only use cargo items on the surface."s));
        snd(27);
        return 0;
    }
    else
    {
        return 1;
    }
}



int dist(int x1, int y1, int x2, int y2)
{
    const auto dx = x1 - x2;
    const auto dy = y1 - y2;
    return int(std::sqrt(dx * dx + dy * dy));
}



int winposy(int prm_539, int prm_540)
{
    if (prm_540 == 0)
    {
        return ((inf_screenh + 1) * inf_tiles - prm_539) / 2 + 8;
    }
    return (windowh - prm_539) / 2;
}



void cutname(std::string& prm_541, int prm_542)
{
    int len_at_m71 = 0;
    elona_vector1<std::string> buf_at_m71;
    int p_at_m71 = 0;
    if (strlen_u(prm_541) > size_t(prm_542))
    {
        len_at_m71 = zentohan(prm_541, buf_at_m71, 0);
        SDIM2(buf_at_m71, len_at_m71);
        zentohan(prm_541, prm_541, len_at_m71);
        if (strlen_u(prm_541) > size_t(prm_542))
        {
            len_at_m71 = 0;
            while (1)
            {
                if (len_at_m71 >= prm_542)
                {
                    break;
                }
                p_at_m71 = prm_541[len_at_m71];
                if ((p_at_m71 >= 129 && p_at_m71 <= 159)
                    || (p_at_m71 >= 224 && p_at_m71 <= 252))
                {
                    p_at_m71 = 2;
                }
                else
                {
                    p_at_m71 = 1;
                }
                len_at_m71 += p_at_m71;
            }
            prm_541 = strmid(prm_541, 0, len_at_m71) + u8".."s;
        }
    }
    return;
}



void cs_listbk()
{
    if (cs_bk == -1)
        return;
    pos(cs_posbk_x, cs_posbk_y);
    gcopy(3, 264, 96, cs_posbk_w, cs_posbk_h);
}



void cs_list(
    bool is_selected,
    const std::string& text,
    int x,
    int y,
    int x_offset,
    int color_mode,
    int ci)
{
    if (is_selected)
    {
        const auto width =
            clamp(int(strlen_u(text)) * 7 + 32 + x_offset, 10, 480);

        gsel(3);
        pos(264, 96);
        gcopy(0, x, y, width, 19);
        gsel(0);

        boxf(x, y, x + width, y + 19, {127, 191, 255, 63});
        pos(x + width - 20, y + 4);
        gcopy(3, 48, 360, 16, 16);

        cs_posbk_x = x;
        cs_posbk_y = y;
        cs_posbk_w = width;
        cs_posbk_h = 19;
    }

    switch (color_mode)
    {
    case 0: color(10, 10, 10); break;
    case 1:
        color(0, 0, 0);
        if (inv[ci].identification_state
            == identification_state_t::completely_identified)
        {
            switch (inv[ci].curse_state)
            {
            case curse_state_t::doomed: color(100, 10, 100); break;
            case curse_state_t::cursed: color(150, 10, 10); break;
            case curse_state_t::none: color(10, 40, 120); break;
            case curse_state_t::blessed: color(10, 110, 30); break;
            }
        }
        if (ibit(13, ci))
        {
            color(120, 80, 0);
        }
        break;
    case 2: color(240, 240, 240); break;
    case 3: color(160, 10, 10); break;
    case 4: color(128, 128, 128); break;
    default: break;
    }

    pos(x + 4 + x_offset, y + 3);
    mes(text);
    color(0, 0, 0);
}



void go_hostile()
{
    for (int cc = ELONA_MAX_PARTY_CHARACTERS; cc < ELONA_MAX_CHARACTERS; ++cc)
    {
        if (cdata[cc].character_role == 14 || cdata[cc].character_role == 16
            || cdata[cc].character_role == 1010)
        {
            cdata[cc].relationship = -3;
            cdata[cc].hate = 80;
            cdata[cc].emotion_icon = 218;
        }
    }
}



void modify_karma(int cc, int delta)
{
    if (trait(162) && delta < 0)
    {
        delta = delta * 75 / 100;
        if (delta == 0)
            return;
    }
    if (trait(169) && delta < 0)
    {
        delta = delta * 150 / 100;
    }

    if (delta >= 0)
    {
        txtef(5);
    }
    else
    {
        txtef(8);
    }
    txt(lang(u8"カルマ変動("s + delta + u8") ", u8"Karma("s + delta + u8")"));
    if (delta > 0)
    {
        if (cdata[cc].karma < -30 && cdata[cc].karma + delta >= -30)
        {
            txtef(2);
            txt(lang(
                u8"あなたの罪は軽くなった。",
                u8"You are no longer a criminal"));
        }
    }
    else if (delta < 0)
    {
        if (cdata[cc].karma >= -30 && cdata[cc].karma + delta < -30)
        {
            txtef(8);
            txt(lang(u8"あなたは今や罪人だ。", u8"You are a criminal now."));
            go_hostile();
        }
    }

    cdata[cc].karma += delta;

    int max = 20;
    if (trait(162))
    {
        max -= 20;
    }
    if (trait(169))
    {
        max += 20;
    }
    cdata[cc].karma = clamp(cdata[cc].karma, -100, max);
}



void modrank(int prm_552, int prm_553, int prm_554)
{
    elona_vector1<int> p_at_m75;
    int orgrank_at_m75 = 0;
    int i_at_m75 = 0;
    std::string s_at_m75;
    p_at_m75 = 120 + prm_552;
    p_at_m75(1) = gdata(p_at_m75) / 100;
    orgrank_at_m75 = gdata(p_at_m75);
    i_at_m75 = prm_553;
    if (prm_553 > 0)
    {
        i_at_m75 = prm_553 * (p_at_m75(1) + 20) * (p_at_m75(1) + 20) / 2500;
        gdata(140 + prm_552) = ranknorma(prm_552);
        if (gdata(p_at_m75) == 100)
        {
            return;
        }
        if (prm_554 != 0)
        {
            if (i_at_m75 / 100 > prm_554)
            {
                i_at_m75 = prm_554 * 100;
            }
        }
    }
    gdata(p_at_m75) -= i_at_m75;
    if (gdata(p_at_m75) >= 10000)
    {
        gdata(p_at_m75) = 10000;
    }
    if (gdata(p_at_m75) < 100)
    {
        gdata(p_at_m75) = 100;
    }
    if (orgrank_at_m75 / 100 != gdata(p_at_m75) / 100)
    {
        i_at_m75 = gdata(p_at_m75) / 100 - orgrank_at_m75 / 100;
        if (i_at_m75 < 0)
        {
            txtef(2);
        }
        else
        {
            txtef(8);
        }
        s_at_m75 = u8" "s + orgrank_at_m75 / 100 + lang(u8"位 → "s, u8" -> "s)
            + gdata(p_at_m75) / 100 + lang(u8"位 "s, ""s);
        txt(lang(u8"ランク変動"s, u8"Ranking Change "s) + u8"("s
            + rankn(10, prm_552) + s_at_m75 + u8") "s + lang(u8"《"s, u8"<"s)
            + ranktitle(prm_552) + lang(u8"》"s, u8">"s));
    }
    else if (i_at_m75 > 0)
    {
        txtef(2);
        txt(lang(
            u8"着実に次のランクに近づいている。"s,
            u8"You are one step closer to the next rank."s));
    }
    return;
}





int getworker(int map_id, int prm_579)
{
    int ret = -1;
    for (int i = 1; i < 16; ++i)
    {
        if (prm_579 != 0 && i != prm_579)
            continue;
        if (cdata[i].current_map == map_id)
        {
            ret = i;
            break;
        }
    }
    return ret;
}



int route_info(int& prm_612, int& prm_613, int prm_614)
{
    if (route(0, prm_614 % maxroute) == 1)
    {
        prm_612 += route(1, prm_614 % maxroute);
    }
    else
    {
        prm_613 += route(1, prm_614 % maxroute);
    }
    if (prm_614 % maxroute % 2 == 0)
    {
        if (route(0, (prm_614 + 1) % maxroute) != route(0, prm_614 % maxroute))
        {
            return -1;
        }
    }
    if (prm_614 >= maxroute)
    {
        if (prm_612 < scx || prm_613 < scy || prm_612 >= scx + inf_screenw
            || prm_613 >= scy + inf_screenh)
        {
            return 0;
        }
        if (prm_612 < 0 || prm_613 < 0 || prm_612 >= mdata(0)
            || prm_613 >= mdata(1))
        {
            return 0;
        }
        if (chipm(7, map(prm_612, prm_613, 0)) & 1)
        {
            return 0;
        }
        if (map(prm_612, prm_613, 6) != 0)
        {
            cell_featread(prm_612, prm_613);
            if (chipm(7, feat) & 1)
            {
                return 0;
            }
        }
    }
    if (route(1, prm_614 % maxroute) == 0)
    {
        return -1;
    }
    return 1;
}



int breath_list()
{
    int breathw = 0;
    DIM3(breathlist, 2, 100);
    maxbreath = 0;
    breathw = 1;
    dx = cdata[cc].position.x;
    dy = cdata[cc].position.y;
    for (int cnt = 0,
             cnt_end = cnt + (the_ability_db[efid]->sdataref3 % 1000 + 1);
         cnt < cnt_end;
         ++cnt)
    {
        if (route(0, cnt % maxroute) == 1)
        {
            dx += route(1, cnt % maxroute);
        }
        else
        {
            dy += route(1, cnt % maxroute);
        }
        if (cnt < 6)
        {
            if (cnt % 3 == 1)
            {
                breathw += 2;
            }
        }
        else
        {
            breathw -= 2;
            if (breathw < 3)
            {
                breathw = 3;
            }
        }
        for (int cnt = 0, cnt_end = (breathw); cnt < cnt_end; ++cnt)
        {
            ty = cnt - breathw / 2 + dy;
            for (int cnt = 0, cnt_end = (breathw); cnt < cnt_end; ++cnt)
            {
                tx = cnt - breathw / 2 + dx;
                if (tx < scx || ty < scy || tx >= scx + inf_screenw
                    || ty >= scy + inf_screenh)
                {
                    continue;
                }
                if (tx < 0 || ty < 0 || tx >= mdata(0) || ty >= mdata(1))
                {
                    continue;
                }
                if (chipm(7, map(tx, ty, 0)) & 1)
                {
                    continue;
                }
                if (maxbreath >= 100)
                {
                    break;
                }
                p = 0;
                for (int cnt = 0, cnt_end = (maxbreath); cnt < cnt_end; ++cnt)
                {
                    if (breathlist(0, cnt) == tx)
                    {
                        if (breathlist(1, cnt) == ty)
                        {
                            p = 1;
                            break;
                        }
                    }
                }
                if (p == 1)
                {
                    continue;
                }
                breathlist(0, maxbreath) = tx;
                breathlist(1, maxbreath) = ty;
                ++maxbreath;
            }
        }
    }
    return 1;
}



void cursor_check()
{
    if (key == key_north)
    {
        snd(5);
        --cs;
        if (cs < 0)
        {
            cs = keyrange - 1;
            if (cs < 0)
            {
                cs = 0;
            }
        }
    }
    if (key == key_south)
    {
        snd(5);
        ++cs;
        if (cs >= keyrange)
        {
            cs = 0;
        }
    }
    if (key == key_west)
    {
        key = key_pagedown;
    }
    if (key == key_east)
    {
        key = key_pageup;
    }
    if (cs >= keyrange)
    {
        cs_bk = -1;
        cs = keyrange - 1;
        if (cs < 0)
        {
            cs = 0;
        }
    }
    if (key == key_enter)
    {
        key = key_list(cs);
    }
    return;
}



void lenfix(std::string& prm_644, int prm_645)
{
    int p_at_m89 = 0;
    p_at_m89 = prm_645 - strlen_u(prm_644);
    if (p_at_m89 < 1)
    {
        p_at_m89 = 1;
    }
    for (int cnt = 0, cnt_end = (p_at_m89); cnt < cnt_end; ++cnt)
    {
        prm_644 += u8" "s;
    }
    return;
}



void showscroll(const std::string& title, int x, int y, int width, int height)
{
    if (windowshadow != 0)
    {
        gmode(6, -1, -1, 80);
        draw_scroll(x + 3, y + 3, width, height);
        windowshadow = 0;
        gmode(2);
    }
    draw_scroll(x, y, width, height);

    if (title.empty())
        return;

    pos(x + 40, y + height - 67 - height % 8);
    gcopy(3, 96, 360, 24, 16);
    color(194, 173, 161);
    line(
        x + 60,
        y + height - 68 - height % 8,
        x + width - 40,
        y + height - 68 - height % 8);
    color(224, 213, 191);
    line(
        x + 60,
        y + height - 69 - height % 8,
        x + width - 40,
        y + height - 69 - height % 8);
    font(12 + sizefix - en * 2);
    color(0, 0, 0);
    pos(x + 68, y + height - 63 - height % 8);
    mes(s);
    if (pagesize != 0)
    {
        s = u8"Page."s + (page + 1) + u8"/"s + (pagemax + 1);
        font(12 + sizefix - en * 2, snail::font_t::style_t::bold);
        pos(x + width - strlen_u(s) * 7 - 40, y + height - 63 - height % 8);
        mes(s);
    }
}



void window(
    int prm_650,
    int prm_651,
    int prm_652,
    int prm_653,
    int,
    int prm_655)
{
    int dx_at_m92 = 0;
    int dy_at_m92 = 0;
    int x3_at_m92 = 0;
    int y3_at_m92 = 0;
    int p_at_m92 = 0;
    int cnt2_at_m92 = 0;
    if (prm_655 == -1)
    {
        gmode(6, -1, -1, 80);
    }
    else
    {
        gmode(2);
    }
    dx_at_m92 = 0;
    dy_at_m92 = 48;
    x3_at_m92 = prm_652 + prm_650 - prm_652 % 8 - 64;
    y3_at_m92 = prm_653 + prm_651 - prm_653 % 8 - 64;
    if (y3_at_m92 < prm_651 + 14)
    {
        y3_at_m92 = prm_651 + 14;
    }
    for (int cnt = 0, cnt_end = (prm_652 / 8); cnt < cnt_end; ++cnt)
    {
        if (cnt < 8)
        {
            if (cnt == 0)
            {
                pos(prm_650, prm_651);
                gcopy(3, dx_at_m92, dy_at_m92, 64, 48);
                pos(prm_650, y3_at_m92);
                gcopy(3, dx_at_m92, dy_at_m92 + 144, 64, 48);
            }
            continue;
        }
        if (cnt < prm_652 / 8 - 8)
        {
            pos(cnt * 8 + prm_650, prm_651);
            gcopy(3, (cnt - 8) % 18 * 8 + dx_at_m92 + 36, dy_at_m92, 8, 48);
            pos(cnt * 8 + prm_650, y3_at_m92);
            gcopy(
                3, (cnt - 8) % 18 * 8 + dx_at_m92 + 54, dy_at_m92 + 144, 8, 48);
            continue;
        }
        pos(x3_at_m92, prm_651);
        gcopy(3, dx_at_m92 + 208, dy_at_m92, 56, 48);
        pos(x3_at_m92, y3_at_m92);
        gcopy(3, dx_at_m92 + 208, dy_at_m92 + 144, 56, 48);
        break;
    }
    p_at_m92 = prm_653 / 8 - 14;
    if (p_at_m92 < 0)
    {
        p_at_m92 = 0;
    }
    for (int cnt = 0, cnt_end = (p_at_m92); cnt < cnt_end; ++cnt)
    {
        cnt2_at_m92 = cnt;
        for (int cnt = 0, cnt_end = (prm_652 / 8); cnt < cnt_end; ++cnt)
        {
            if (cnt == 0)
            {
                pos(prm_650, cnt2_at_m92 * 8 + prm_651 + 48);
                gcopy(
                    3, dx_at_m92, cnt2_at_m92 % 12 * 8 + dy_at_m92 + 48, 64, 8);
                continue;
            }
            if (cnt < prm_652 / 8 - 15)
            {
                pos(cnt * 8 + prm_650 + 56, cnt2_at_m92 * 8 + prm_651 + 48);
                gcopy(
                    3,
                    cnt % 18 * 8 + dx_at_m92 + 64,
                    cnt2_at_m92 % 12 * 8 + dy_at_m92 + 48,
                    8,
                    8);
                continue;
            }
            pos(x3_at_m92, cnt2_at_m92 * 8 + prm_651 + 48);
            gcopy(
                3,
                dx_at_m92 + 208,
                cnt2_at_m92 % 12 * 8 + dy_at_m92 + 48,
                56,
                8);
            break;
        }
    }
    gmode(2);
    return;
}



void window2(
    int prm_656,
    int prm_657,
    int prm_658,
    int prm_659,
    int prm_660,
    int prm_661)
{
    int dx_at_m93 = 0;
    int dy_at_m93 = 0;
    int x2_at_m93 = 0;
    int y2_at_m93 = 0;
    int x3_at_m93 = 0;
    int y3_at_m93 = 0;
    int p_at_m93 = 0;
    dx_at_m93 = 0;
    dy_at_m93 = 240;
    x2_at_m93 = prm_658;
    y2_at_m93 = prm_659;
    if (x2_at_m93 < 32)
    {
        x2_at_m93 = 32;
    }
    if (y2_at_m93 < 24)
    {
        y2_at_m93 = 24;
    }
    x3_at_m93 = prm_656 + x2_at_m93 / 16 * 16 - 16;
    y3_at_m93 = prm_657 + y2_at_m93 / 16 * 16 - 16;
    if (prm_661 == 0)
    {
        pos(prm_656 + 4, prm_657 + 4);
        gzoom(3, 24, 72, 228, 144, x2_at_m93 - 6, y2_at_m93 - 8);
    }
    if (prm_661 == 1)
    {
        pos(prm_656 + 4, prm_657 + 4);
        gzoom(3, 24, 72, 228, 144, x2_at_m93 - 6, y2_at_m93 - 8);
        pos(prm_656 + 4, prm_657 + 4);
        gfini(x2_at_m93 - 4, y2_at_m93 - 4);
        gfdec2(195, 205, 195);
    }
    if (prm_661 == 2)
    {
        pos(prm_656 + 4, prm_657 + 4);
        gzoom(3, 24, 72, 228, 144, x2_at_m93 - 6, y2_at_m93 - 8);
        pos(prm_656 + 4, prm_657 + 4);
        gfini(x2_at_m93 - 4, y2_at_m93 - 4);
        gfdec2(210, 215, 205);
    }
    if (prm_661 == 3)
    {
        pos(prm_656 + 4, prm_657 + 4);
        gzoom(3, 24, 72, 228, 144, x2_at_m93 - 6, y2_at_m93 - 8);
        pos(prm_656 + 4, prm_657 + 4);
        gfini(x2_at_m93 - 4, y2_at_m93 - 4);
        gfdec2(10, 13, 16);
    }
    if (prm_661 == 4)
    {
        pos(prm_656 + 4, prm_657 + 4);
        gzoom(3, 24, 72, 228, 144, x2_at_m93 - 6, y2_at_m93 - 8);
        pos(prm_656 + 4, prm_657 + 4);
        gfini(x2_at_m93 - 4, y2_at_m93 - 4);
        gfdec2(195, 205, 195);
    }
    if (prm_661 == 6)
    {
        pos(prm_656 + x2_at_m93 / 2, prm_657 + y2_at_m93 / 2);
        gmode(4, 228, 144, 180);
        grotate(3, 24, 72, 0, x2_at_m93 - 4, y2_at_m93 - 4);
    }
    gmode(2, 16, 16);
    for (int cnt = 0, cnt_end = (x2_at_m93 / 16 - 2); cnt < cnt_end; ++cnt)
    {
        pos(cnt * 16 + prm_656 + 16, prm_657);
        gcopy(3, prm_660 * 48 + dx_at_m93 + 16, dy_at_m93);
        pos(cnt * 16 + prm_656 + 16, prm_657 + y2_at_m93 - 16);
        gcopy(3, prm_660 * 48 + dx_at_m93 + 16, dy_at_m93 + 32);
    }
    pos(x3_at_m93, prm_657);
    gcopy(3, prm_660 * 48 + dx_at_m93 + 16, dy_at_m93, x2_at_m93 % 16, 16);
    pos(x3_at_m93, prm_657 + y2_at_m93 - 16);
    gcopy(3, prm_660 * 48 + dx_at_m93 + 16, dy_at_m93 + 32, x2_at_m93 % 16, 16);
    p_at_m93 = y2_at_m93 / 16 - 2;
    if (p_at_m93 < 0)
    {
        p_at_m93 = 0;
    }
    for (int cnt = 0, cnt_end = (p_at_m93); cnt < cnt_end; ++cnt)
    {
        pos(prm_656, cnt * 16 + prm_657 + 16);
        gcopy(3, prm_660 * 48 + dx_at_m93, dy_at_m93 + 16);
        pos(prm_656 + x2_at_m93 - 16, cnt * 16 + prm_657 + 16);
        gcopy(3, prm_660 * 48 + dx_at_m93 + 32, dy_at_m93 + 16);
    }
    pos(prm_656, y3_at_m93);
    gcopy(3, prm_660 * 48 + dx_at_m93, dy_at_m93 + 16, 16, y2_at_m93 % 16);
    pos(prm_656 + x2_at_m93 - 16, y3_at_m93);
    gcopy(3, prm_660 * 48 + dx_at_m93 + 32, dy_at_m93 + 16, 16, y2_at_m93 % 16);
    pos(prm_656, prm_657);
    gcopy(3, prm_660 * 48 + dx_at_m93, dy_at_m93);
    pos(prm_656, prm_657 + y2_at_m93 - 16);
    gcopy(3, prm_660 * 48 + dx_at_m93, dy_at_m93 + 32);
    pos(prm_656 + x2_at_m93 - 16, prm_657);
    gcopy(3, prm_660 * 48 + dx_at_m93 + 32, dy_at_m93);
    pos(prm_656 + x2_at_m93 - 16, prm_657 + y2_at_m93 - 16);
    gcopy(3, prm_660 * 48 + dx_at_m93 + 32, dy_at_m93 + 32);
    if (prm_661 == 5)
    {
        pos(prm_656 + 2, prm_657 + 2);
        gzoom(3, 24, 72, 228, 144, x2_at_m93 - 4, y2_at_m93 - 5);
        pos(prm_656 + 2, prm_657 + 2);
        gfini(x2_at_m93 - 4, y2_at_m93 - 4);
        gfdec2(195, 205, 195);
    }
    return;
}



int roll(int x, int y, int z)
{
    if (x < 1)
        x = 1;
    if (y < 1)
        y = 1;
    int ret = 0;
    for (int i = 0; i < x; ++i)
    {
        ret += rnd(y) + 1;
    }
    return ret + z;
}



int roll_max(int x, int y, int z)
{
    return x * y + z;
}



void page_save()
{
    pagebk = page;
    csprev = cs;
    pagesaved = 1;
    return;
}



void page_load()
{
    if (pagesaved == 1)
    {
        page = pagebk;
        cs = csprev;
        pagesaved = 0;
    }
    return;
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
    ELONA_LOG("fileadd(" << mark_a << "): " << filename);
    noteadd(mark_a + filename);
    noteunsel();
}



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



std::string cnv_str(
    std::string& prm_696,
    const std::string& prm_697,
    const std::string& prm_698)
{
    std::string s3_at_m100;
    int p_at_m100 = 0;
    while (1)
    {
        s3_at_m100 = strutil::to_lower(prm_696);
        p_at_m100 = instr(s3_at_m100, 0, prm_697);
        if (p_at_m100 == -1)
        {
            break;
        }
        prm_696 = strmid(prm_696, 0, p_at_m100) + lang(prm_698, prm_698)
            + strmid(prm_696, (p_at_m100 + prm_697.size()), 300);
    }
    return prm_696;
}



void fix_input_chat(std::string& str)
{
    cnv_str(str, u8" "s, u8"+"s);
    cnv_str(str, u8"%"s, u8"per"s);
    cnv_str(str, u8"&"s, u8"and"s);
    str = strutil::remove_str(str, u8"<");
    str = strutil::remove_str(str, u8">");
}



void fix_input_chat2(std::string& str)
{
    cnv_str(str, u8"fucking"s, u8"nyoro~n"s);
    cnv_str(str, u8"fuck"s, u8"nyou talk funny"s);
}



void cnv_filestr(std::string& str)
{
    cnv_str(str, u8"\""s, u8"_"s);
    cnv_str(str, u8"\\"s, u8"_"s);
    cnv_str(str, u8"<"s, u8"_"s);
    cnv_str(str, u8">"s, u8"_"s);
    cnv_str(str, u8"/"s, u8"_"s);
    cnv_str(str, u8"?"s, u8"_"s);
    cnv_str(str, u8"|"s, u8"_"s);
    cnv_str(str, u8"*"s, u8"_"s);
}



void imeset(int prm_709)
{
    himc_at_ime_control = ImmGetContext(hwnd);
    ImmSetOpenStatus(himc_at_ime_control, prm_709);
    ImmReleaseContext(hwnd, himc_at_ime_control);
    return;
}



int imeget()
{
    int imesw_at_ime_control = 0;
    himc_at_ime_control = ImmGetContext(hwnd);
    imesw_at_ime_control = ImmGetOpenStatus(himc_at_ime_control);
    ImmReleaseContext(hwnd, himc_at_ime_control);
    return imesw_at_ime_control;
}



void boxl(int x1, int y1, int x2, int y2)
{
    pos(x1, y2);
    line(x1, y1);
    line(x2, y1);
    line(x2, y2);
    line(x1, y2);
}



std::string fixtxt(const std::string& prm_724, int prm_725)
{
    std::string m_at_m104;
    m_at_m104 = ""s + prm_724;
    if (strlen_u(prm_724) < size_t(prm_725))
    {
        while (1)
        {
            if (strlen_u(m_at_m104) >= size_t(prm_725))
            {
                break;
            }
            m_at_m104 += u8" "s;
        }
    }
    else
    {
        m_at_m104 = ""s + strmid(prm_724, 0, prm_725);
    }
    return ""s + m_at_m104;
}



void windowanime(
    int prm_726,
    int prm_727,
    int prm_728,
    int prm_729,
    int prm_730,
    int prm_731)
{
    int cenx_at_m105 = 0;
    int ceny_at_m105 = 0;
    if (nowindowanime)
    {
        nowindowanime = 0;
        return;
    }
    if (config::instance().windowanime == 0)
    {
        return;
    }
    gsel(prm_731);
    gmode(0);
    pos(0, 0);
    gcopy(0, prm_726, prm_727, prm_728, prm_729);
    gsel(0);
    gmode(0);
    x2_at_m105 = prm_728 / 2;
    y2_at_m105 = prm_729 / 2;
    cenx_at_m105 = prm_726 + x2_at_m105;
    ceny_at_m105 = prm_727 + y2_at_m105;
    for (int cnt = 1, cnt_end = cnt + (prm_730 - 1); cnt < cnt_end; ++cnt)
    {
        color(30, 30, 30);
        boxl(
            cenx_at_m105 - x2_at_m105 / prm_730 * cnt,
            ceny_at_m105 - y2_at_m105 / prm_730 * cnt,
            cenx_at_m105 + x2_at_m105 / prm_730 * cnt,
            ceny_at_m105 + y2_at_m105 / prm_730 * cnt);
        color(240, 240, 240);
        boxl(
            cenx_at_m105 - x2_at_m105 / prm_730 * cnt - 1,
            ceny_at_m105 - y2_at_m105 / prm_730 * cnt - 1,
            cenx_at_m105 + x2_at_m105 / prm_730 * cnt - 1,
            ceny_at_m105 + y2_at_m105 / prm_730 * cnt - 1);
        color(0, 0, 0);
        redraw();
        if (cnt != prm_730 - 1)
        {
            await(15);
        }
        pos(prm_726, prm_727);
        gcopy(prm_731, 0, 0, prm_728, prm_729);
    }
    gmode(2);
    return;
}



void windowanimecorner(
    int prm_732,
    int prm_733,
    int prm_734,
    int prm_735,
    int prm_736,
    int prm_737)
{
    if (config::instance().windowanime == 0)
    {
        return;
    }
    gsel(prm_737);
    gmode(0);
    pos(0, 0);
    gcopy(0, prm_732, prm_733, prm_734, prm_735);
    gsel(0);
    gmode(0);
    x2_at_m105 = prm_734 - prm_732;
    y2_at_m105 = prm_735 - prm_733;
    for (int cnt = 1, cnt_end = cnt + (prm_736); cnt < cnt_end; ++cnt)
    {
        color(30, 30, 30);
        boxl(
            prm_732 + prm_734,
            prm_733 + prm_735,
            prm_732 + prm_734 - (prm_734 - prm_732) / prm_736 * cnt,
            prm_733 + prm_735 - (prm_735 - prm_733) / prm_736 * cnt);
        color(240, 240, 240);
        boxl(
            prm_732 + prm_734 - 1,
            prm_733 + prm_735 - 1,
            prm_732 + prm_734 - (prm_734 - prm_732) / prm_736 * cnt - 1,
            prm_733 + prm_735 - (prm_735 - prm_733) / prm_736 * cnt - 1);
        color(0, 0, 0);
        redraw();
        if (cnt != prm_736)
        {
            await(15);
        }
        pos(prm_732, prm_733);
        gcopy(prm_737, 0, 0, prm_734, prm_735);
    }
    gmode(2);
    return;
}



void showtitle(const std::string&, const std::string& prm_739, int prm_740, int)
{
    int x_at_m106 = 0;
    int y_at_m106 = 0;
    font(12 + sizefix - en * 2);
    if (mode != 1)
    {
        x_at_m106 = prm_740 - 10;
        y_at_m106 = 0;
    }
    else
    {
        x_at_m106 = 240;
        y_at_m106 = windowh - 16;
    }
    for (int cnt = 0, cnt_end = ((windoww - x_at_m106 - 8) / 192 + 1);
         cnt < cnt_end;
         ++cnt)
    {
        pos(x_at_m106 + 8 + cnt * 192, y_at_m106);
        gcopy(3, 496, 581, 192, 18);
    }
    gmode(2);
    pos(x_at_m106, y_at_m106 + (mode != 1));
    gcopy(3, 96, 360, 24, 16);
    pos(x_at_m106 + 32, y_at_m106 + 1 + jp);
    bmes(prm_739, 250, 250, 250);
    return;
}



int direction(int x1, int y1, int x2, int y2)
{
    if (std::abs(x1 - x2) > std::abs(y1 - y2))
    {
        return x1 > x2 ? 1 : 2;
    }
    else
    {
        return y1 > y2 ? 3 : 0;
    }
}



void fixaiact(int prm_753)
{
    // FIXME: DRY
    for (auto&& action : cdata[prm_753].normal_actions)
    {
        if (action == 464 || action == 441 || action == 660 || action == 657)
        {
            action = -1;
        }
    }
    for (auto&& action : cdata[prm_753].special_actions)
    {
        if (action == 464 || action == 441 || action == 660 || action == 657)
        {
            action = -1;
        }
    }
    int i_at_m112 = cdata[prm_753].ai_heal;
    if (i_at_m112 == 464 || i_at_m112 == 441 || i_at_m112 == 660
        || i_at_m112 == 657)
    {
        cdata[prm_753].ai_heal = 0;
    }
    return;
}



int eleinfo(int prm_754, int prm_755)
{
    if (prm_754 == 50)
    {
        if (prm_755 == 0)
        {
            return 3;
        }
        else
        {
            return 108;
        }
    }
    if (prm_754 == 51)
    {
        if (prm_755 == 0)
        {
            return 12;
        }
        else
        {
            return 106;
        }
    }
    if (prm_754 == 52)
    {
        if (prm_755 == 0)
        {
            return 5;
        }
        else
        {
            return 107;
        }
    }
    if (prm_754 == 59)
    {
        if (prm_755 == 0)
        {
            return 8;
        }
        else
        {
            return 114;
        }
    }
    if (prm_754 == 53)
    {
        if (prm_755 == 0)
        {
            return 4;
        }
        else
        {
            return 115;
        }
    }
    if (prm_754 == 58)
    {
        if (prm_755 == 0)
        {
            return 9;
        }
        else
        {
            return 111;
        }
    }
    if (prm_754 == 57)
    {
        if (prm_755 == 0)
        {
            return 11;
        }
        else
        {
            return 112;
        }
    }
    if (prm_754 == 54)
    {
        if (prm_755 == 0)
        {
            return 10;
        }
        else
        {
            return 113;
        }
    }
    if (prm_754 == 55)
    {
        if (prm_755 == 0)
        {
            return 2;
        }
        else
        {
            return 110;
        }
    }
    if (prm_754 == 56)
    {
        if (prm_755 == 0)
        {
            return 7;
        }
        else
        {
            return 109;
        }
    }
    if (prm_754 == 63)
    {
        if (prm_755 == 0)
        {
            return 2;
        }
        else
        {
            return 110;
        }
    }
    return 0;
}



void refreshspeed(int cc)
{
    if (cdata[cc].speed_correction_value == 0)
    {
        cdata[cc].current_speed = sdata(18, cc);
    }
    else
    {
        cdata[cc].current_speed = sdata(18, cc)
            * clamp((100 - cdata[cc].speed_correction_value), 0, 100) / 100;
    }
    if (cdata[cc].current_speed < 10)
    {
        cdata[cc].current_speed = 10;
    }
    cdata[cc].speed_percentage_in_next_turn = 0;

    if (cc != 0 && gdata_mount != cc)
        return;

    if (gdata_mount != 0)
    {
        cdata[0].current_speed = sdata(18, gdata_mount) * 100
            / clamp((100 + sdata(18, gdata_mount)
                     - sdata(10, gdata_mount) * 3 / 2 - sdata(301, 0) * 2
                     - (cdata[gdata_mount].is_suitable_for_mount() == 1) * 50),
                    100,
                    1000);
        if (cdata[gdata_mount].is_unsuitable_for_mount())
        {
            cdata[0].current_speed /= 10;
        }
        if (gdata_mount == cc)
        {
            cdata[cc].current_speed =
                clamp(sdata(10, cc) + sdata(301, 0), 10, sdata(18, cc));
            return;
        }
    }

    gspdorg = sdata.get(18, 0).original_level;

    if (gdata_mount == 0)
    {
        int n = cdata[0].nutrition / 1000 * 1000;
        if (n < 1000)
        {
            cdata[0].speed_percentage_in_next_turn -= 30;
        }
        if (n < 2000)
        {
            cdata[0].speed_percentage_in_next_turn -= 10;
        }
        if (cdata[0].sp < 0)
        {
            cdata[0].speed_percentage_in_next_turn -= 30;
        }
        if (cdata[0].sp < 25)
        {
            cdata[0].speed_percentage_in_next_turn -= 20;
        }
        if (cdata[0].sp < 50)
        {
            cdata[0].speed_percentage_in_next_turn -= 10;
        }
    }
    if (cdata[0].inventory_weight_type >= 3)
    {
        cdata[0].speed_percentage_in_next_turn -= 50;
    }
    if (cdata[0].inventory_weight_type == 2)
    {
        cdata[0].speed_percentage_in_next_turn -= 30;
    }
    if (cdata[0].inventory_weight_type == 1)
    {
        cdata[0].speed_percentage_in_next_turn -= 10;
    }
    if (mdata(6) == 1 || mdata(6) == 4)
    {
        if (gdata_cargo_weight > gdata_current_cart_limit)
        {
            cdata[0].speed_percentage_in_next_turn -=
                25 + 25 * (gdata_cargo_weight / (gdata_current_cart_limit + 1));
        }
    }
    gspd = cdata[0].current_speed * (100 + cdata[0].speed_percentage) / 100;
    if (gspd < 10)
    {
        gspd = 10;
    }
}



void ride_begin(int mount)
{
    txt(lang(
        name(mount) + u8"に騎乗した("s + name(mount) + u8"の速度: "s
            + cdata[mount].current_speed + u8"→"s,
        u8"You ride "s + name(mount) + u8". ("s + name(mount) + u8"'s speed: "s
            + cdata[mount].current_speed + u8"->"s));
    cdata[mount].is_ridden() = true;
    map(cdata[mount].position.x, cdata[mount].position.y, 1) = 0;
    gdata_mount = mount;
    create_pcpic(0, true);
    rowactend(gdata_mount);
    refreshspeed(gdata_mount);
    txt(""s + cdata[mount].current_speed + u8") "s);
    if (cdata[gdata_mount].is_suitable_for_mount())
    {
        txt(lang(
            u8"この生物は乗馬用にちょうどいい！"s, u8"You feel comfortable."s));
    }
    else if (cdata[gdata_mount].is_unsuitable_for_mount())
    {
        txt(lang(
            u8"この生物はあなたを乗せるには非力すぎる。"s,
            u8"This creature is too weak to carry you."s));
    }
}



void ride_end()
{
    int mount = gdata_mount;
    cdata[mount].is_ridden() = false;
    rowactend(mount);
    gdata_mount = 0;
    create_pcpic(0, true);
    refreshspeed(mount);
}



void turn_aggro(int cc, int tc, int hate)
{
    if (tc < 16)
    {
        cdata[cc].relationship = -3;
    }
    cdata[cc].hate = hate;
    cdata[cc].emotion_icon = 218;
    cdata[cc].enemy_id = tc;
}



void make_sound(
    int prm_777,
    int prm_778,
    int prm_779,
    int prm_780,
    int prm_781,
    int prm_782)
{
    for (int cnt = 1; cnt < ELONA_MAX_CHARACTERS; ++cnt)
    {
        if (cdata[cnt].state != 1)
        {
            continue;
        }
        if (dist(prm_777, prm_778, cdata[cnt].position.x, cdata[cnt].position.y)
            < prm_779)
        {
            if (rnd(prm_780) == 0)
            {
                if (cdata[cnt].sleep != 0)
                {
                    cdata[cnt].sleep = 0;
                    if (is_in_fov(cnt))
                    {
                        txt(lang(
                            name(cnt) + u8"は物音に気付き目を覚ました。"s,
                            name(cnt) + u8" notice"s + _s(cnt)
                                + u8" the sound and wake"s + _s(cnt)
                                + u8" up."s));
                    }
                    cdata[cnt].emotion_icon = 221;
                    if (prm_781)
                    {
                        if (rnd(500) == 0)
                        {
                            if (is_in_fov(cnt))
                            {
                                txtef(9);
                                txt(lang(
                                    name(cnt) + u8"はキレた。"s,
                                    name(cnt)
                                        + u8" can no longer put up with it."s));
                                txt(lang(
                                    u8"「もう我慢できない」"s,
                                    u8"\"That's it.\""s));
                            }
                            turn_aggro(cnt, prm_782, 80);
                        }
                    }
                }
            }
        }
    }
    return;
}



void hostileaction(int prm_787, int prm_788)
{
    if (prm_787 >= 16 || prm_788 == 0)
    {
        return;
    }
    if (cdata[prm_788].relationship != -3)
    {
        cdata[prm_788].emotion_icon = 418;
    }
    if (cdata[prm_788].relationship == 10)
    {
        txtef(8);
        txt(lang(
            name(prm_788) + u8"は嫌な顔をした。"s,
            name(prm_788) + u8" glares at you."s));
    }
    else
    {
        if (cdata[prm_788].relationship == 0)
        {
            modify_karma(0, -2);
        }
        if (cdata[prm_788].id == 202)
        {
            if (gdata_released_fire_giant == 0)
            {
                txtef(8);
                txt(lang(
                    name(prm_788) + u8"は嫌な顔をした。"s,
                    name(prm_788) + u8" glares at you."s));
                return;
            }
        }
        if (cdata[prm_788].relationship > -2)
        {
            txtef(8);
            txt(lang(
                name(prm_788) + u8"は嫌な顔をした。"s,
                name(prm_788) + u8" glares at you."s));
            cdata[prm_788].relationship = -2;
        }
        else
        {
            if (cdata[prm_788].relationship != -3)
            {
                txtef(8);
                txt(lang(
                    name(prm_788) + u8"は激怒した。"s,
                    name(prm_788) + u8" gets furious!"s));
            }
            cdata[prm_788].relationship = -3;
            cdata[prm_788].hate = 80;
            cdata[prm_788].enemy_id = prm_787;
        }
        chara_custom_talk(prm_788, 101);
    }
    if (cdata[prm_788].is_livestock() == 1)
    {
        if (rnd(50) == 0)
        {
            txtef(3);
            txt(lang(u8"家畜は興奮した！"s, u8"Animals get excited!"s));
            for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
            {
                if (cdata[cnt].is_livestock() == 1)
                {
                    cdata[cnt].enemy_id = 0;
                    cdata[cnt].hate = 20;
                    cdata[cnt].emotion_icon = 318;
                }
            }
        }
    }
    rowactend(prm_788);
    return;
}



void rowact_check(int prm_789)
{
    if (cdata[prm_789].continuous_action_id != 0)
    {
        if (cdata[prm_789].continuous_action_id != 3)
        {
            cdata[prm_789].stops_continuous_action_if_damaged = 1;
        }
    }
    return;
}



void rowact_item(int prm_790)
{
    for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
    {
        if (cdata[cnt].state != 1)
        {
            continue;
        }
        if (cdata[cnt].continuous_action_turn <= 0)
        {
            continue;
        }
        if (cdata[cnt].continuous_action_id == 1
            || cdata[cnt].continuous_action_id == 2)
        {
            if (cdata[cnt].continuous_action_item == prm_790)
            {
                rowactend(cnt);
                txt(lang(
                    name(cnt) + u8"は行動を中断した。"s,
                    name(cnt) + u8" cancel"s + _s(cnt) + u8" "s + his(cnt)
                        + u8" action."s));
            }
        }
    }
    return;
}



void wake_up()
{
    if (gdata_hour >= 7 && gdata_hour <= 22)
    {
        for (int cnt = ELONA_MAX_PARTY_CHARACTERS; cnt < ELONA_MAX_CHARACTERS;
             ++cnt)
        {
            if (cdata[cnt].sleep > 0)
            {
                if (rnd(10))
                {
                    cdata[cnt].sleep = 0;
                }
            }
        }
    }
    return;
}



void incognitobegin()
{
    for (int cnt = 16; cnt < ELONA_MAX_CHARACTERS; ++cnt)
    {
        if (cdata[cnt].state != 1)
        {
            continue;
        }
        if (cdata[cnt].character_role == 1010)
        {
            continue;
        }
        if (cdata[cnt].character_role == 16)
        {
            continue;
        }
        if (cdata[cnt].original_relationship >= -2)
        {
            if (cdata[cnt].relationship <= -2)
            {
                cdata[cnt].hate = 0;
                cdata[cnt].relationship = cdata[cnt].original_relationship;
                cdata[cnt].emotion_icon = 221;
            }
        }
    }
    return;
}



void incognitoend()
{
    for (int cnt = 16; cnt < ELONA_MAX_CHARACTERS; ++cnt)
    {
        if (cdata[cnt].state != 1)
        {
            continue;
        }
        if (cdata[cnt].character_role == 14)
        {
            if (cdata[0].karma < -30)
            {
                cdata[cnt].relationship = -3;
                cdata[cnt].hate = 80;
                cdata[cnt].emotion_icon = 218;
            }
        }
    }
    return;
}


void animeload(int prm_807, int prm_808)
{
    elona_vector1<int> i_at_m133;
    if (mode != 0)
    {
        return;
    }
    if (is_in_fov(prm_808) == 0)
    {
        return;
    }
    if (config::instance().animewait == 0)
    {
        return;
    }
    screenupdate = -1;
    update_screen();
    dx_at_m133 = (cdata[prm_808].position.x - scx) * inf_tiles + inf_screenx;
    dy_at_m133 = (cdata[prm_808].position.y - scy) * inf_tiles + inf_screeny;
    gsel(7);
    picload(filesystem::dir::graphic() / (u8"anime"s + prm_807 + u8".bmp"));
    gsel(4);
    gmode(0);
    pos(0, 0);
    gcopy(0, dx_at_m133 - 24, dy_at_m133 - 40, 96, 96);
    gsel(0);
    gmode(2);
    i_at_m133(0) = 5;
    i_at_m133(1) = 50;
    r_at_m133 = 0;
    if (prm_807 == 8)
    {
        snd(65);
    }
    if (prm_807 == 10)
    {
        i_at_m133(0) = 8;
        i_at_m133(1) = 30;
        r_at_m133 = 0.2;
        snd(119);
    }
    if (prm_807 == 11)
    {
        i_at_m133(0) = 5;
        i_at_m133(1) = 50;
        r_at_m133 = 0;
        snd(118);
    }
    if (prm_807 == 14)
    {
        i_at_m133(0) = 6;
        i_at_m133(1) = 50;
    }
    for (int cnt = 0, cnt_end = (i_at_m133); cnt < cnt_end; ++cnt)
    {
        gmode(2, 96, 96);
        pos(dx_at_m133 + 24, dy_at_m133 + 8);
        grotate(7, cnt * 96, 0, r_at_m133 * cnt, 96, 96);
        gmode(0);
        redraw();
        pos(dx_at_m133 - 24, dy_at_m133 - 40);
        gcopy(4, 0, 0, 96, 96);
        await(config::instance().animewait + i_at_m133(1));
    }
    gmode(2);
    return;
}



void animeblood(int prm_809, int prm_810, int prm_811)
{
    int ele2_at_m133 = 0;
    elona_vector1<int> x_at_m133;
    elona_vector1<int> y_at_m133;
    int cnt2_at_m133 = 0;
    if (is_in_fov(prm_809) == 0)
    {
        return;
    }
    if (config::instance().animewait == 0)
    {
        return;
    }
    screenupdate = -1;
    update_screen();
    if (prm_810 == 0)
    {
        prepare_item_image(16, 0);
    }
    if (prm_810 == 1)
    {
        prepare_item_image(18, 0);
    }
    dx_at_m133 = (cdata[prm_809].position.x - scx) * inf_tiles + inf_screenx;
    dy_at_m133(0) = (cdata[prm_809].position.y - scy) * inf_tiles + inf_screeny;
    dy_at_m133(1) = 0;
    gsel(4);
    gmode(0);
    pos(0, 0);
    gcopy(0, dx_at_m133 - 48, dy_at_m133 - 56, 144, 160);
    ele2_at_m133 = 0;
    if (prm_811 == 52)
    {
        ele2_at_m133 = 1;
        gsel(7);
        picload(filesystem::dir::graphic() / u8"anime18.bmp");
        dy_at_m133(1) = -16;
    }
    if (prm_811 == 51)
    {
        ele2_at_m133 = 1;
        gsel(7);
        picload(filesystem::dir::graphic() / u8"anime19.bmp");
        dy_at_m133(1) = -16;
    }
    if (prm_811 == 50)
    {
        ele2_at_m133 = 1;
        gsel(7);
        picload(filesystem::dir::graphic() / u8"anime20.bmp");
        dy_at_m133(1) = -20;
    }
    if (prm_811 == 56)
    {
        ele2_at_m133 = 1;
        gsel(7);
        picload(filesystem::dir::graphic() / u8"anime22.bmp");
        dy_at_m133(1) = -24;
    }
    if (prm_811 == 53)
    {
        ele2_at_m133 = 1;
        gsel(7);
        picload(filesystem::dir::graphic() / u8"anime21.bmp");
        dy_at_m133(1) = -16;
    }
    if (prm_811 == 54)
    {
        ele2_at_m133 = 1;
        gsel(7);
        picload(filesystem::dir::graphic() / u8"anime23.bmp");
        dy_at_m133(1) = -16;
    }
    if (prm_811 == 57)
    {
        ele2_at_m133 = 1;
        gsel(7);
        picload(filesystem::dir::graphic() / u8"anime24.bmp");
        dy_at_m133(1) = -16;
    }
    if (prm_811 == 59)
    {
        ele2_at_m133 = 1;
        gsel(7);
        picload(filesystem::dir::graphic() / u8"anime25.bmp");
        dy_at_m133(1) = -16;
    }
    if (prm_811 == 58)
    {
        ele2_at_m133 = 1;
        gsel(7);
        picload(filesystem::dir::graphic() / u8"anime26.bmp");
        dy_at_m133(1) = -16;
    }
    if (prm_811 == 55 || prm_811 == 63)
    {
        ele2_at_m133 = 1;
        gsel(7);
        picload(filesystem::dir::graphic() / u8"anime27.bmp");
        dy_at_m133(1) = -16;
    }
    gmode(2);
    gsel(0);
    for (int cnt = 0; cnt < 20; ++cnt)
    {
        x_at_m133(cnt) = rnd(48) - 24;
        y_at_m133(cnt) = rnd(16);
    }
    for (int cnt = 0; cnt < 6; ++cnt)
    {
        cnt2_at_m133 = cnt * 2;
        gmode(2, inf_tiles, inf_tiles);
        if (ele2_at_m133)
        {
            pos(dx_at_m133 - 24, dy_at_m133 - 32 + dy_at_m133(1));
            gcopy(7, cnt * 96, 0, 96, 96);
        }
        for (int cnt = 0; cnt < 20; ++cnt)
        {
            pos(dx_at_m133 + 24 + x_at_m133(cnt)
                    + (x_at_m133(cnt) < 3) * ((1 + (cnt % 2 == 0)) * -1)
                        * cnt2_at_m133
                    + (x_at_m133(cnt) > -4) * (1 + (cnt % 2 == 0))
                        * cnt2_at_m133,
                dy_at_m133 + y_at_m133(cnt) + cnt2_at_m133 * cnt2_at_m133 / 2
                    - 12 + cnt);
            grotate(
                1,
                0,
                960,
                0.2 * cnt,
                24 - cnt2_at_m133 * 2,
                24 - cnt2_at_m133 * 2);
        }
        gmode(0);
        redraw();
        pos(dx_at_m133 - 48, dy_at_m133 - 56);
        gcopy(4, 0, 0, 144, 160);
        await(config::instance().animewait + 15 + (ele2_at_m133 != 0) * 20);
    }
    gmode(2);
    return;
}



void spillblood(int prm_830, int prm_831, int prm_832)
{
    for (int cnt = 0, cnt_end = (prm_832 + 1); cnt < cnt_end; ++cnt)
    {
        if (cnt == 0)
        {
            dx_at_m136 = prm_830;
            dy_at_m136 = prm_831;
        }
        else
        {
            dx_at_m136 = prm_830 + rnd(2) - rnd(2);
            dy_at_m136 = prm_831 + rnd(2) - rnd(2);
        }
        if (dx_at_m136 < 0 || dx_at_m136 >= mdata(0) || dy_at_m136 < 0
            || dy_at_m136 >= mdata(1))
        {
            continue;
        }
        if (chipm(2, map(dx_at_m136, dy_at_m136, 0)))
        {
            continue;
        }
        if (map(dx_at_m136, dy_at_m136, 7) % 10 < 5)
        {
            ++map(dx_at_m136, dy_at_m136, 7);
        }
    }
    return;
}



void spillfrag(int prm_833, int prm_834, int prm_835)
{
    for (int cnt = 0, cnt_end = (prm_835 + 1); cnt < cnt_end; ++cnt)
    {
        if (cnt == 0)
        {
            dx_at_m136 = prm_833;
            dy_at_m136 = prm_834;
        }
        else
        {
            dx_at_m136 = prm_833 + rnd(2) - rnd(2);
            dy_at_m136 = prm_834 + rnd(2) - rnd(2);
        }
        if (dx_at_m136 < 0 || dx_at_m136 >= mdata(0) || dy_at_m136 < 0
            || dy_at_m136 >= mdata(1))
        {
            continue;
        }
        if (chipm(2, map(dx_at_m136, dy_at_m136, 0)))
        {
            continue;
        }
        if (map(dx_at_m136, dy_at_m136, 7) / 10 < 4)
        {
            map(dx_at_m136, dy_at_m136, 7) = map(dx_at_m136, dy_at_m136, 7) % 10
                + (map(dx_at_m136, dy_at_m136, 7) / 10 + 1) * 10;
        }
    }
    return;
}



void check_kill(int prm_836, int prm_837)
{
    int p_at_m137 = 0;
    if (gdata_current_map == 40 || gdata_current_map == 35
        || gdata_current_map == 6)
    {
        return;
    }
    p_at_m137 = 0;
    if (prm_836 >= 0)
    {
        if (prm_836 == 0 || cdata[prm_836].relationship >= 10)
        {
            if (prm_837 > 16)
            {
                ++gdata_kill_count;
                if (cdata[prm_837].id == gdata_fighters_guild_target)
                {
                    if (gdata_fighters_guild_quota > 0)
                    {
                        --gdata_fighters_guild_quota;
                    }
                }
                if (cdata[prm_837].original_relationship >= 0)
                {
                    p_at_m137 = -2;
                }
                if (cdata[prm_837].id == 183)
                {
                    p_at_m137 = -15;
                }
                if (cdata[prm_837].id == 184)
                {
                    p_at_m137 = -10;
                }
                if (cdata[prm_837].id == 185)
                {
                    p_at_m137 = -5;
                }
                if ((cdata[prm_837].character_role >= 1000
                     && cdata[prm_837].character_role < 2000)
                    || cdata[prm_837].character_role == 2003)
                {
                    p_at_m137 = -10;
                }
                if (cdata[prm_837].character_role == 13)
                {
                    chara_mod_impression(prm_837, -25);
                }
            }
        }
        if (cdata[prm_836].relationship >= 10)
        {
            if (prm_836 != 0)
            {
                if (cdata[prm_836].impression < 200)
                {
                    if (rnd(2))
                    {
                        chara_mod_impression(prm_836, 1);
                        cdata[prm_836].emotion_icon = 317;
                    }
                }
                else if (rnd(10) == 0)
                {
                    chara_mod_impression(prm_836, 1);
                    cdata[prm_836].emotion_icon = 317;
                }
            }
        }
    }
    if (p_at_m137 != 0)
    {
        modify_karma(0, p_at_m137);
    }
    return;
}


std::string cnveqweight(int cc)
{
    int id = chara_armor_class(cc);
    if (id == 169)
    {
        return lang(u8"(重装備)"s, u8"(Heavy)"s);
    }
    else if (id == 170)
    {
        return lang(u8"(中装備)"s, u8"(Medium)"s);
    }
    else
    {
        return lang(u8"(軽装備)"s, u8"(Light)"s);
    }
}



void cnvbonus(int prm_895, int prm_896)
{
    if (prm_895 >= 50 && prm_895 < 61)
    {
        if (prm_896 > 0)
        {
            buff += u8"　　"s
                + i18n::_(u8"ability", std::to_string(prm_895), u8"name")
                + u8"耐性に <green>クラス"s + prm_896 / 50 + u8"<col>("s
                + prm_896 + u8") のボーナス\n"s;
        }
        if (prm_896 < 0)
        {
            buff += u8"　　"s
                + i18n::_(u8"ability", std::to_string(prm_895), u8"name")
                + u8"耐性に <red>クラス"s + prm_896 / 50 + u8"<col>("s + prm_896
                + u8") のマイナス修正\n"s;
        }
    }
    else
    {
        if (prm_896 > 0)
        {
            buff += u8"　　"s
                + i18n::_(u8"ability", std::to_string(prm_895), u8"name")
                + u8"に <green>+"s + prm_896 + u8"<col> のボーナス\n"s;
        }
        if (prm_896 < 0)
        {
            buff += u8"　　"s
                + i18n::_(u8"ability", std::to_string(prm_895), u8"name")
                + u8"に <red>"s + prm_896 + u8"<col> のマイナス修正\n"s;
        }
    }
    return;
}



int can_evade_trap()
{
    if (feat(2) == 7)
    {
        return 0;
    }
    if (cc < 16)
    {
        if (rnd(refdiff + 1) < sdata(13, cc) + sdata(159, cc) * 4)
        {
            return 1;
        }
    }
    else if (rnd(4) != 0)
    {
        return 1;
    }
    return 0;
}


void refresh_burden_state()
{
    cdata[0].inventory_weight = clamp(inv_weight(0), 0, 20000000)
        * (100 - trait(201) * 10 + trait(205) * 20) / 100;
    cdata[0].max_inventory_weight =
        sdata(10, 0) * 500 + sdata(11, 0) * 250 + sdata(153, 0) * 2000 + 45000;
    for (int cnt = 0; cnt < 1; ++cnt)
    {
        if (cdata[0].inventory_weight > cdata[0].max_inventory_weight * 2)
        {
            cdata[0].inventory_weight_type = 4;
            break;
        }
        if (cdata[0].inventory_weight > cdata[0].max_inventory_weight)
        {
            cdata[0].inventory_weight_type = 3;
            break;
        }
        if (cdata[0].inventory_weight > cdata[0].max_inventory_weight / 4 * 3)
        {
            cdata[0].inventory_weight_type = 2;
            break;
        }
        if (cdata[0].inventory_weight > cdata[0].max_inventory_weight / 2)
        {
            cdata[0].inventory_weight_type = 1;
            break;
        }
        cdata[0].inventory_weight_type = 0;
    }
    refreshspeed(0);
    return;
}



void label_1537()
{
    label_1538();
    cxinit = cdata[0].position.x;
    cyinit = cdata[0].position.y;
    chara_place();
    cdata[rc].current_map = 0;
    snd(64);
    txtef(5);
    txt(cnven(cdatan(0, rc))
        + lang(
              u8"は復活した！"s,
              (u8" "s + have(rc) + u8" been resurrected!"s)));
    return;
}



void label_1538()
{
    label_15380();
    label_15390();
    return;
}



void label_1539()
{
    label_15390();
    return;
}



void label_15380()
{
    cdata[rc].will_explode_soon() = false;
    cdata[rc].is_sentenced_daeth() = false;
    cdata[rc].is_pregnant() = false;
    cdata[rc].is_contracting_with_reaper() = false;
    cdata[rc].has_anorexia() = false;
    cdata[rc].hp = cdata[rc].max_hp / 3;
    cdata[rc].mp = cdata[rc].max_mp / 3;
    cdata[rc].sp = cdata[rc].max_sp / 3;
    cdata[rc].insanity = 0;
    cdata[rc].state = 1;
    cdata[rc].current_map = 0;
    cdata[rc].relationship = cdata[rc].original_relationship;
    cdata[rc].nutrition = 8000;
    return;
}



void label_15390()
{
    cdata[rc].is_contracting_with_reaper() = false;
    rowactend(rc);
    cdata[rc].poisoned = 0;
    cdata[rc].sleep = 0;
    cdata[rc].confused = 0;
    cdata[rc].blind = 0;
    cdata[rc].paralyzed = 0;
    cdata[rc].choked = 0;
    cdata[rc].furious = 0;
    cdata[rc].dimmed = 0;
    cdata[rc].drunk = 0;
    cdata[rc].bleeding = 0;
    cdata[rc].gravity = 0;
    cdata[rc].item_which_will_be_used = 0;
    cdata[rc].hate = 0;
    cdata[rc].enemy_id = 0;
    cdata[rc].sick = 0;
    cdata[rc].emotion_icon = 0;
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        cdata[rc].attr_adjs[cnt] = 0;
    }
    if (cdata[rc].buffs[0].id != 0)
    {
        for (int cnt = 0; cnt < 16; ++cnt)
        {
            if (cdata[rc].buffs[cnt].id == 0)
            {
                break;
            }
            if (cdata[rc].buffs[cnt].id == 13)
            {
                continue;
            }
            delbuff(rc, cnt);
            --cnt;
            continue;
        }
    }
    chara_refresh(rc);
    return;
}



void label_1540()
{
    label_1538();
    if (rc == 0)
    {
        gdata_is_returning_or_escaping = 0;
        traveldone = 0;
        if (gdata_executing_immediate_quest_type == 0)
        {
            event_add(6);
        }
    }
    if (cdata[rc].character_role == 1)
    {
        cdata[rc].relationship = 0;
    }
    if (cdata[rc].character_role == 14)
    {
        p = rnd(5) + 1;
        for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
        {
            r2 = 1;
            gain_level(rc);
        }
    }
    if (cdata[rc].id == 326)
    {
        skillmod(183, rc, 1000);
    }
    chara_refresh(rc);
    return;
}



int convertartifact(int prm_930, int prm_931)
{
    int f_at_m163 = 0;
    int tc_at_m163 = 0;
    std::string n_at_m163;
    if (the_item_db[inv[prm_930].id]->category >= 50000)
    {
        return prm_930;
    }
    if (inv[prm_930].quality != 6)
    {
        return prm_930;
    }
    if (inv[prm_930].body_part != 0)
    {
        return prm_930;
    }
    f_at_m163 = 0;
    for (int cnt = 0; cnt < 5480; ++cnt)
    {
        if (prm_931)
        {
            if (cnt >= 5080)
            {
                break;
            }
        }
        if (cnt >= 520 && cnt < 5060)
        {
            continue;
        }
        tc_at_m163 = inv_getowner(cnt);
        if (tc_at_m163 != -1)
        {
            if (cdata[tc_at_m163].state == 0
                || cdata[tc_at_m163].character_role == 13)
            {
                continue;
            }
        }
        if (inv[cnt].number > 0)
        {
            if (inv[cnt].id == inv[prm_930].id)
            {
                if (cnt != prm_930)
                {
                    f_at_m163 = 1;
                    break;
                }
            }
        }
    }
    if (f_at_m163 == 0)
    {
        return prm_930;
    }
    n_at_m163 = ""s + itemname(prm_930);

    while (1)
    {
        flt(the_item_db[inv[prm_930].id]->level, 4);
        flttypeminor = the_item_db[inv[prm_930].id]->subcategory;
        inv[prm_930].number = 0;
        itemcreate(
            inv_getowner(prm_930),
            0,
            inv[prm_930].position.x,
            inv[prm_930].position.y,
            0);
        if (inv[prm_930].quality != 6)
        {
            break;
        }
    }

    txt(lang(
        n_at_m163 + u8"は"s + itemname(ci) + u8"に形を変えた。"s,
        n_at_m163 + u8" turns its shape into "s + itemname(ci) + u8"."s));
    return prm_930;
}



void initialize_set_of_random_generation()
{
    fsetincome(0) = 52000;
    fsetincome(1) = 52000;
    fsetincome(2) = 52000;
    fsetincome(3) = 53000;
    fsetincome(4) = 53000;
    fsetincome(5) = 56000;
    fsetincome(6) = 54000;
    fsetincome(7) = 77000;
    fsetincome(8) = 57000;
    fsetincome(9) = 57000;
    fsetchest(0) = 10000;
    fsetchest(1) = 24000;
    fsetchest(2) = 12000;
    fsetchest(3) = 16000;
    fsetchest(4) = 20000;
    fsetchest(5) = 19000;
    fsetchest(6) = 18000;
    fsetchest(7) = 22000;
    fsetchest(8) = 14000;
    fsetchest(9) = 32000;
    fsetchest(10) = 34000;
    fsetchest(11) = 54000;
    fsetchest(12) = 59000;
    fsetwear(0) = 10000;
    fsetwear(1) = 10000;
    fsetwear(2) = 24000;
    fsetwear(3) = 24000;
    fsetwear(4) = 25000;
    fsetwear(5) = 12000;
    fsetwear(6) = 16000;
    fsetwear(7) = 20000;
    fsetwear(8) = 19000;
    fsetwear(9) = 18000;
    fsetwear(10) = 22000;
    fsetwear(11) = 14000;
    fsetwear(12) = 32000;
    fsetwear(13) = 34000;
    fsetitem(0) = 52000;
    fsetitem(1) = 52000;
    fsetitem(2) = 53000;
    fsetitem(3) = 53000;
    fsetitem(4) = 56000;
    fsetitem(5) = 68000;
    fsetitem(6) = 54000;
    fsetitem(7) = 64000;
    fsetitem(8) = 59000;
    fsetitem(9) = 55000;
    fsetrewardsupply(0) = 52000;
    fsetrewardsupply(1) = 53000;
    fsetrewardsupply(2) = 56000;
    fsetrewardsupply(3) = 54000;
    fsetrewardsupply(4) = 57000;
    fsetrare(0) = 60000;
    fsetrare(1) = 72000;
    fsetrare(2) = 77000;
    fsetrare(3) = 55000;
    fsetrare(4) = 57000;
    fsetperform(0) = 77000;
    fsetperform(1) = 57000;
    fsetperform(2) = 57000;
    fsetperform(3) = 57000;
    fsetperform(4) = 60000;
    fsetperform(5) = 18000;
    fsetperform(6) = 20000;
    fsetperform(7) = 32000;
    fsetperform(8) = 34000;
    fsetperform(9) = 52000;
    fsetperform(10) = 64000;
    fsetperform(11) = 64000;
    fsetdeliver(0) = 60000;
    fsetdeliver(1) = 77000;
    fsetdeliver(2) = 54000;
    fsetdeliver(3) = 64000;
    fsetsupply(0) = 60000;
    fsetsupply(1) = 77000;
    fsetsupply(2) = 56000;
    fsetsupply(3) = 54000;
    fsetsupply(4) = 64000;
    fsetmagic(0) = 53000;
    fsetmagic(1) = 56000;
    fsetmagic(2) = 54000;
    fsetarmor(0) = 12000;
    fsetarmor(1) = 16000;
    fsetarmor(2) = 20000;
    fsetarmor(3) = 19000;
    fsetarmor(4) = 18000;
    fsetarmor(5) = 22000;
    fsetarmor(6) = 14000;
    fsetweapon(0) = 10000;
    fsetweapon(1) = 24000;
    fsetweapon(2) = 25000;
    fsetplantartifact(0) = 32000;
    fsetplantartifact(1) = 34000;
    fsetplantunknown(0) = 57000;
    fsetplantunknown(1) = 57000;
    fsetplantunknown(2) = 54000;
    fsetplantunknown(3) = 64000;
    fsetplantunknown(4) = 77000;
    fsetremain(0) = 25000;
    fsetremain(1) = 57000;
    fsetremain(2) = 57000;
    fsetremain(3) = 77000;
    fsetremain(4) = 53000;
    fsetremain(5) = 52000;
    fsetremain(6) = 57000;
    fsetbarrel(0) = 25000;
    fsetbarrel(1) = 57000;
    fsetbarrel(2) = 53000;
    fsetbarrel(3) = 52000;
    fsetbarrel(4) = 77000;
    fsetbarrel(5) = 64000;
    fsetcollect(0) = 64000;
    fsetcollect(1) = 60000;
    fsetcollect(2) = 57000;
    fsetcollect(3) = 77000;
    isetdeed(0) = 344;
    isetdeed(1) = 521;
    isetdeed(2) = 522;
    isetdeed(3) = 542;
    isetdeed(4) = 543;
    isetdeed(5) = 555;
    isetdeed(6) = 572;
    isetfruit(0) = 180;
    isetfruit(1) = 181;
    isetfruit(2) = 196;
    isetfruit(3) = 197;
    isetfruit(4) = 192;
    isetfruit(5) = 183;
    isetthrowpotionminor(0) = 27;
    isetthrowpotionminor(1) = 28;
    isetthrowpotionminor(2) = 376;
    isetthrowpotionminor(3) = 30;
    isetthrowpotionminor(4) = 262;
    isetthrowpotionminor(5) = 253;
    isetthrowpotionminor(6) = 379;
    isetthrowpotionminor(7) = 392;
    isetthrowpotionmajor(0) = 28;
    isetthrowpotionmajor(1) = 376;
    isetthrowpotionmajor(2) = 205;
    isetthrowpotionmajor(3) = 368;
    isetthrowpotionmajor(4) = 433;
    isetthrowpotionmajor(5) = 382;
    isetthrowpotionmajor(6) = 577;
    isetthrowpotionmajor(7) = 577;
    isetthrowpotiongreater(0) = 28;
    isetthrowpotiongreater(1) = 205;
    isetthrowpotiongreater(2) = 368;
    isetthrowpotiongreater(3) = 432;
    isetthrowpotiongreater(4) = 429;
    isetthrowpotiongreater(5) = 29;
    isetthrowpotiongreater(6) = 577;
    isethire(0) = 205;
    isethire(1) = 70;
    isethire(2) = 74;
    isethire(3) = 41;
    isethire(4) = 69;
    isethire(5) = 76;
    isethire(6) = 1;
    isethire(7) = 1;
    isethire(8) = 1;
    isetgiftminor(0) = 753;
    isetgiftminor(1) = 754;
    isetgiftminor(2) = 754;
    isetgiftminor(3) = 756;
    isetgiftminor(4) = 756;
    isetgiftminor(5) = 540;
    isetgiftminor(6) = 541;
    isetgiftminor(7) = 537;
    isetgiftminor(8) = 538;
    isetgiftminor(9) = 526;
    isetgiftminor(10) = 415;
    isetgiftminor(11) = 511;
    isetgiftminor(12) = 343;
    isetgiftminor(13) = 340;
    isetgiftminor(14) = 338;
    isetgiftminor(15) = 328;
    isetgiftminor(16) = 220;
    isetgiftminor(17) = 575;
    isetgiftminor(18) = 622;
    isetgiftminor(19) = 167;
    isetgiftmajor(0) = 755;
    isetgiftmajor(1) = 755;
    isetgiftmajor(2) = 633;
    isetgiftmajor(3) = 626;
    isetgiftmajor(4) = 502;
    isetgiftmajor(5) = 632;
    isetgiftmajor(6) = 667;
    isetgiftmajor(7) = 555;
    isetgiftmajor(8) = 748;
    isetgiftmajor(9) = 640;
    isetgiftgrand(0) = 757;
    isetgiftgrand(1) = 558;
    isetgiftgrand(2) = 664;
    isetgiftgrand(3) = 758;
    isetgiftgrand(4) = 759;
    isetgiftgrand(5) = 761;
    isethirerole(0) = 18;
    isethirerole(1) = 9;
    isethirerole(2) = 12;
    isethirerole(3) = 5;
    isethirerole(4) = 8;
    isethirerole(5) = 14;
    isethirerole(6) = 1008;
    isethirerole(7) = 1008;
    isethirerole(8) = 1008;
    isetcrop(0) = 180;
    isetcrop(1) = 181;
    isetcrop(2) = 197;
    isetcrop(3) = 192;
    isetcrop(4) = 183;
    isetcrop(5) = 188;
    isetcrop(6) = 200;
    isetgod(0) = 0;
    isetgod(1) = 1;
    isetgod(2) = 2;
    isetgod(3) = 3;
    isetgod(4) = 4;
    isetgod(5) = 5;
    isetgod(6) = 6;
    isetgod(7) = 7;
    asettown(0) = 5;
    asettown(1) = 11;
    asettown(2) = 14;
    asettown(3) = 15;
    asettown(4) = 12;
    moneybox(0) = 500;
    moneybox(1) = 2000;
    moneybox(2) = 10000;
    moneybox(3) = 50000;
    moneybox(4) = 500000;
    moneybox(5) = 5000000;
    moneybox(6) = 100000000;
    giftvalue(0) = 10;
    giftvalue(1) = 20;
    giftvalue(2) = 30;
    giftvalue(3) = 50;
    giftvalue(4) = 75;
    giftvalue(5) = 100;
    giftn(0) = lang(u8"つまらない"s, u8"cheap"s);
    giftn(1) = lang(u8"普通の"s, u8"so so"s);
    giftn(2) = lang(u8"ちょっとドキドキする"s, u8"exciting"s);
    giftn(3) = lang(u8"高価な"s, u8"expensive"s);
    giftn(4) = lang(u8"気絶しそうなほど素晴らしい"s, u8"hot and gorgeous"s);
    giftn(5) = lang(u8"王様が羨むほどの"s, u8"crazy epic"s);
    giftn(6) = u8"bug"s;
    moneyboxn(0) = lang(u8"5百金貨"s, u8"500 GP"s);
    moneyboxn(1) = lang(u8"2千金貨"s, u8"2k GP"s);
    moneyboxn(2) = lang(u8"1万金貨"s, u8"10K GP"s);
    moneyboxn(3) = lang(u8"5万金貨"s, u8"50K GP"s);
    moneyboxn(4) = lang(u8"50万金貨"s, u8"500K GP"s);
    moneyboxn(5) = lang(u8"500万金貨"s, u8"5M GP"s);
    moneyboxn(6) = lang(u8"1億金貨"s, u8"500M GP"s);
    biten(0) = lang(u8"ミジンコ"s, u8"water flea"s);
    biten(1) = lang(u8"バッタ"s, u8"grasshopper"s);
    biten(2) = lang(u8"テントウ"s, u8"ladybug"s);
    biten(3) = lang(u8"トンボ"s, u8"dragonfly"s);
    biten(4) = lang(u8"セミ"s, u8"locust"s);
    biten(5) = lang(u8"ヘラクレス"s, u8"beetle"s);
    notesel(buff);
    {
        buff(0).clear();
        std::ifstream in{(filesystem::dir::data() / u8"book.txt").native(),
                         std::ios::binary};
        std::string tmp;
        while (std::getline(in, tmp))
        {
            buff(0) += tmp + '\n';
        }
    }
    p = instr(buff, 0, u8"%DEFINE"s);
    buff = strmid(buff, p, instr(buff, p, u8"%END"s));
    notedel(0);
    SDIM3(booktitle, noteinfo(), 25);
    p = 0;
    for (int cnt = 0, cnt_end = (noteinfo()); cnt < cnt_end; ++cnt)
    {
        noteget(msgtemp, cnt);
        csvsort(s, msgtemp, 44);
        booktitle(elona::stoi(s(0))) = lang(s(1), s(2));
        if (elona::stoi(s(3)) == 1)
        {
            isetbook(p) = elona::stoi(s(0));
            ++p;
        }
    }
    magebookn(0) = lang(u8"ヴォイニッチ写本"s, u8"Voynich Manuscript"s);
    magebookn(1) = lang(u8"ドール賛歌"s, u8"Dhol Chants"s);
    magebookn(2) = lang(u8"ポナペ教教典"s, u8"Ponape Scripture"s);
    magebookn(3) = lang(u8"グラーキ黙示録"s, u8"Revelations of Glaaki"s);
    magebookn(4) = lang(u8"グ＝ハーン断章"s, u8"G'harne Fragments"s);
    magebookn(5) = lang(u8"断罪の書"s, u8"Liber Damnatus"s);
    magebookn(6) = lang(u8"ドジアンの書"s, u8"Book of Dzyan"s);
    magebookn(7) = lang(u8"エイボンの書"s, u8"Book of Eibon"s);
    magebookn(8) = lang(u8"大いなる教書"s, u8"Grand Grimoire"s);
    magebookn(9) = lang(u8"セラエノ断章"s, u8"Celaeno Fragments"s);
    magebookn(10) = lang(u8"ネクロノミコン"s, u8"Necronomicon"s);
    magebookn(11) = lang(u8"ルルイエ異本"s, u8"The R'lyeh Text"s);
    magebookn(12) = lang(u8"エルトダウン・シャールズ"s, u8"Eltdown Shards"s);
    magebookn(13) = lang(u8"金枝篇"s, u8"The Golden Bough"s);
    magebookn(14) = lang(u8"終焉の書"s, u8"Apocalypse"s);
    return;
}



void label_1573()
{
    int lootrich = 0;
    if (rc == 0)
    {
        if (gdata_executing_immediate_quest_type != 0)
        {
            return;
        }
        for (const auto& cnt : items(rc))
        {
            ci = cnt;
            if (inv[cnt].number == 0)
            {
                continue;
            }
            if (mdata(7) == 0)
            {
                if (inv[cnt].body_part != 0)
                {
                    continue;
                }
                if (ibit(5, ci))
                {
                    continue;
                }
                if (rnd(3))
                {
                    continue;
                }
            }
            else if (rnd(5))
            {
                continue;
            }
            if (the_item_db[inv[ci].id]->is_cargo == 1)
            {
                if (mdata(6) != 1 && mdata(6) != 5 && mdata(6) != 3
                    && mdata(6) != 4 && mdata(6) != 6 && mdata(6) != 2)
                {
                    continue;
                }
                else if (rnd(2))
                {
                    continue;
                }
            }
            f = 0;
            if (inv[ci].body_part != 0)
            {
                if (rnd(10))
                {
                    f = 1;
                }
                if (inv[ci].curse_state == curse_state_t::blessed)
                {
                    if (rnd(2))
                    {
                        f = 1;
                    }
                }
                if (is_cursed(inv[ci].curse_state))
                {
                    if (rnd(2))
                    {
                        f = 0;
                    }
                }
                if (inv[ci].curse_state == curse_state_t::doomed)
                {
                    if (rnd(2))
                    {
                        f = 0;
                    }
                }
            }
            else if (
                inv[ci].identification_state
                == identification_state_t::completely_identified)
            {
                if (rnd(4))
                {
                    f = 1;
                }
            }
            if (f)
            {
                continue;
            }
            if (inv[ci].body_part != 0)
            {
                cdata_body_part(rc, inv[ci].body_part) =
                    cdata_body_part(rc, inv[ci].body_part) / 10000 * 10000;
                inv[ci].body_part = 0;
            }
            f = 0;
            if (ibit(5, ci) == 0)
            {
                if (rnd(4) == 0)
                {
                    f = 1;
                }
                if (inv[ci].curse_state == curse_state_t::blessed)
                {
                    if (rnd(3) == 0)
                    {
                        f = 0;
                    }
                }
                if (is_cursed(inv[ci].curse_state))
                {
                    if (rnd(3) == 0)
                    {
                        f = 1;
                    }
                }
                if (inv[ci].curse_state == curse_state_t::doomed)
                {
                    if (rnd(3) == 0)
                    {
                        f = 1;
                    }
                }
            }
            if (f)
            {
                inv[ci].number = 0;
                continue;
            }
            inv[ci].position.x = cdata[rc].position.x;
            inv[ci].position.y = cdata[rc].position.y;
            int stat = item_stack(-1, ci);
            if (stat == 0)
            {
                ti = inv_getfreeid(-1);
                if (ti == -1)
                {
                    break;
                }
                item_copy(ci, ti);
                inv[ti].own_state = -2;
            }
            inv[ci].number = 0;
        }
        cell_refresh(cdata[rc].position.x, cdata[rc].position.y);
        create_pcpic(0, true);
        return;
    }
    else
    {
        if (rc < 16)
        {
            if (cdata[rc].has_own_sprite() == 1)
            {
                create_pcpic(rc, true);
            }
        }
        if (cdata[rc].relationship == 10)
        {
            return;
        }
    }
    if (gdata_current_map == 33)
    {
        if (cdata[rc].id == 185)
        {
            return;
        }
        if (cdata[rc].id == 204)
        {
            return;
        }
    }
    if (cdata[rc].is_contracting())
    {
        return;
    }
    if (cdata[rc].splits() || cdata[rc].splits2())
    {
        if (rnd(6))
        {
            return;
        }
    }
    for (const auto& cnt : items(rc))
    {
        if (inv[cnt].number == 0)
        {
            continue;
        }
        ci = cnt;
        f = 0;
        if (cdata[rc].character_role == 20)
        {
            break;
        }
        if (inv[ci].quality > 4 || inv[ci].id == 55)
        {
            f = 1;
        }
        if (rnd(30) == 0)
        {
            f = 1;
        }
        if (cdata[rc].quality >= 4)
        {
            if (rnd(2) == 0)
            {
                f = 1;
            }
        }
        if (cdata[rc].character_role == 13)
        {
            if (rnd(5))
            {
                f = 0;
            }
        }
        if (gdata_current_map == 6)
        {
            if (rnd(4))
            {
                f = 0;
            }
        }
        if (inv[ci].quality == 6)
        {
            f = 1;
        }
        if (ibit(12, ci))
        {
            f = 1;
        }
        if (f == 0)
        {
            continue;
        }
        if (catitem != 0)
        {
            if (ibit(8, ci) == 0)
            {
                if (the_item_db[inv[ci].id]->category < 50000)
                {
                    if (inv[ci].quality >= 3)
                    {
                        if (rnd(3))
                        {
                            txtef(9);
                            txt(lang(
                                name(catitem) + u8"は"s + itemname(ci)
                                    + u8"をぺろぺろと舐めた。"s,
                                name(catitem) + " licks " + itemname(ci)
                                    + "."));
                            ibitmod(8, ci, 1);
                            reftype = the_item_db[inv[ci].id]->category;
                            enchantment_add(
                                ci,
                                enchantment_generate(enchantment_gen_level(rnd(4))),
                                enchantment_gen_p());
                            animeload(8, rc);
                        }
                    }
                }
            }
        }
        if (inv[ci].body_part != 0)
        {
            cdata_body_part(rc, inv[ci].body_part) =
                cdata_body_part(rc, inv[ci].body_part) / 10000 * 10000;
            inv[ci].body_part = 0;
        }
        inv[ci].position.x = cdata[rc].position.x;
        inv[ci].position.y = cdata[rc].position.y;
        itemturn(ci);
        int stat = item_stack(-1, ci);
        if (stat == 0)
        {
            ti = inv_getfreeid(-1);
            if (ti == -1)
            {
                break;
            }
            item_copy(ci, ti);
        }
        inv[ci].number = 0;
    }
    if (cdata[rc].quality >= 4 || rnd(20) == 0 || cdata[rc].drops_gold() == 1
        || rc < 16)
    {
        if (cdata[rc].gold > 0)
        {
            flt();
            itemcreate(
                -1,
                54,
                cdata[rc].position.x,
                cdata[rc].position.y,
                cdata[rc].gold / (1 + 3 * (cdata[rc].drops_gold() == 0)));
            cdata[rc].gold -=
                cdata[rc].gold / (1 + 3 * (cdata[rc].drops_gold() == 0));
        }
    }

    switch (access_class_info(16, cdatan(3, rc)))
    {
    case 1:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 7:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 3:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 2:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 53000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(40) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 54000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 4:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 5:
        if (rnd(50) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 54000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    }

    dbid = cdata[rc].id;
    dbmode = 16;
    dbspec = 3;
    switch (access_character_info())
    {
    case 3:
        if (rnd(40) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(40) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 53000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(40) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = fsetwear(rnd(length(fsetwear)));
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(40) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = fsetweapon(rnd(length(fsetweapon)));
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 68000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 1:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 62000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
            remain_make(ci, rc);
        }
        break;
    case 2:
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 62000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
            remain_make(ci, rc);
        }
        break;
    case 6:
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 32000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 34000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 54000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 53000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 72000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 68000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 77000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 4:
        if (rnd(5) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = fsetwear(rnd(length(fsetwear)));
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(5) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = fsetweapon(rnd(length(fsetweapon)));
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(20) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 72000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(4) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 68000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    case 5:
        if (rnd(5) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = fsetwear(rnd(length(fsetwear)));
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(5) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = fsetweapon(rnd(length(fsetweapon)));
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(15) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 54000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(5) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 52000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(5) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 53000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(10) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 72000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(4) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 68000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(4) == 0)
        {
            p = 0;
            flt(calcobjlv(cdata[tc].level), calcfixlv(2));
            flttypemajor = 77000;
            flttypeminor = 0;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        break;
    }

    if (rnd(40) == 0)
    {
        p = 0;
        flt(calcobjlv(cdata[tc].level), calcfixlv(2));
        flttypemajor = 62000;
        flttypeminor = 0;
        itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        remain_make(ci, rc);
    }
    if (gdata_current_map == 35)
    {
        cell_refresh(cdata[rc].position.x, cdata[rc].position.y);
        return;
    }
    if (gdata_current_map != 6 && cdata[rc].character_role != 20)
    {
        if (rnd(175) == 0 || cdata[rc].quality == 6 || 0
            || (cdata[rc].quality == 4 && rnd(2) == 0)
            || (cdata[rc].quality == 5 && rnd(3) == 0))
        {
            flt();
            itemcreate(-1, 504, cdata[rc].position.x, cdata[rc].position.y, 0);
            inv[ci].param1 = cdata[rc].image;
            inv[ci].subname = cdata[rc].id;
            cell_refresh(inv[ci].position.x, inv[ci].position.y);
        }
        if (rnd(175) == 0 || cdata[rc].quality == 6 || 0
            || (cdata[rc].quality == 4 && rnd(2) == 0)
            || (cdata[rc].quality == 5 && rnd(3) == 0))
        {
            flt();
            itemcreate(-1, 503, cdata[rc].position.x, cdata[rc].position.y, 0);
            inv[ci].param1 = cdata[rc].image;
            inv[ci].subname = cdata[rc].id;
            cell_refresh(inv[ci].position.x, inv[ci].position.y);
        }
    }
    if (cdata[rc].character_role == 1010)
    {
        flt();
        itemcreate(-1, 361, cdata[rc].position.x, cdata[rc].position.y, 0);
        inv[ci].param1 = cdata[rc].shop_store_id;
        inv[ci].own_state = 2;
    }
    if (rollanatomy == 1 || cdata[rc].quality >= 4 || 0
        || cdata[rc].is_livestock() == 1 || 0)
    {
        flt();
        int stat =
            itemcreate(-1, 204, cdata[rc].position.x, cdata[rc].position.y, 0);
        if (stat != 0)
        {
            remain_make(ci, rc);
            if (cdata[rc].is_livestock() == 1)
            {
                if (sdata(161, 0) != 0)
                {
                    inv[ci].number +=
                        rnd(1 + (sdata(161, 0) > cdata[rc].level));
                }
            }
        }
    }
    lootrich = -1;
    if (cdata[rc].id == 183)
    {
        lootrich = 8;
    }
    if (cdata[rc].id == 184)
    {
        lootrich = 4;
    }
    if (cdata[rc].id == 185)
    {
        lootrich = 2;
    }
    if (lootrich != -1)
    {
        for (int cnt = 0, cnt_end = (lootrich); cnt < cnt_end; ++cnt)
        {
            flt(calcobjlv(cdata[rc].level));
            flttypeminor = 77001;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
        if (rnd(3) == 0)
        {
            flt();
            itemcreate(-1, 284, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    drop(0) = -1;
    drop(1) = -1;
    drop(2) = -1;
    drop(3) = -1;
    drop(4) = -1;
    drop(5) = -1;
    drop(6) = -1;
    drop(7) = -1;
    drop(8) = -1;
    drop(9) = -1;
    drop(10) = -1;
    if (cdata[rc].id == 140)
    {
        drop(0) = 355;
        drop(1) = -1;
    }
    if (cdata[rc].id == 175 || cdata[rc].id == 177 || cdata[rc].id == 178)
    {
        if (npcmemory(0, cdata[rc].id) == 0)
        {
            drop(0) = 290;
            drop(1) = 421;
            drop(2) = -1;
        }
        else
        {
            drop(0) = 421;
            drop(1) = 421;
            drop(2) = -1;
        }
    }
    if (gdata_current_map == 37)
    {
        if (cdata[rc].id == 257)
        {
            drop(0) = 665;
            drop(1) = 55;
            drop(2) = 55;
            drop(3) = 55;
            drop(4) = 55;
            drop(5) = 621;
            drop(6) = -1;
        }
    }
    for (int cnt = 0; cnt < 10; ++cnt)
    {
        if (drop(cnt) == -1)
        {
            break;
        }
        flt();
        itemcreate(
            -1, drop(cnt), cdata[rc].position.x, cdata[rc].position.y, 0);
    }
    if (cdata[rc].id == 5)
    {
        if (rnd(200) == 0)
        {
            flt();
            itemcreate(-1, 702, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 21)
    {
        if (rnd(100) == 0)
        {
            flt();
            itemcreate(-1, 706, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 34)
    {
        if (rnd(4) == 0)
        {
            flt();
            itemcreate(-1, 717, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 99 || cdata[rc].id == 100 || cdata[rc].id == 101)
    {
        if (rnd(300) == 0)
        {
            flt();
            itemcreate(-1, 708, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 117)
    {
        if (rnd(140) == 0)
        {
            flt();
            itemcreate(-1, 720, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 168)
    {
        if (rnd(100) == 0)
        {
            flt();
            itemcreate(-1, 740, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id >= 302 && cdata[rc].id <= 305)
    {
        if (rnd(90) == 0)
        {
            flt();
            itemcreate(-1, 771, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 307 || cdata[rc].id == 308)
    {
        if (rnd(150) == 0)
        {
            flt();
            itemcreate(-1, 709, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 189 || cdata[rc].id == 190)
    {
        if (rnd(150) == 0)
        {
            flt();
            itemcreate(-1, 711, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 213)
    {
        if (rnd(50) == 0)
        {
            flt();
            int stat = itemcreate(
                -1, 672, cdata[rc].position.x, cdata[rc].position.y, 0);
            if (stat != 0)
            {
                inv[ci].param1 = 169;
            }
        }
    }
    if (cdata[rc].id == 250)
    {
        if (rnd(200) == 0)
        {
            flt();
            itemcreate(-1, 715, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 341)
    {
        if (rnd(3) == 0)
        {
            flt();
            itemcreate(-1, 635, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 85 || cdata[rc].id == 86 || cdata[rc].id == 87)
    {
        if (rnd(600) == 0)
        {
            flt();
            itemcreate(-1, 737, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 302)
    {
        for (int cnt = 0, cnt_end = (2 + rnd(4)); cnt < cnt_end; ++cnt)
        {
            flt(cdata[rc].level, 2);
            flttypemajor = 92000;
            itemcreate(-1, 0, cdata[rc].position.x, cdata[rc].position.y, 0);
            if (inv[ci].value < 800)
            {
                inv[ci].number = rnd(5) + 1;
            }
        }
    }
    if (cdata[rc].id == 328)
    {
        flt();
        itemcreate(
            -1, 55, cdata[rc].position.x, cdata[rc].position.y, 1 + rnd(3));
        if (rnd(2))
        {
            flt();
            itemcreate(-1, 622, cdata[rc].position.x, cdata[rc].position.y, 0);
        }
    }
    if (cdata[rc].id == 329)
    {
        flt();
        itemcreate(
            -1,
            54,
            cdata[rc].position.x,
            cdata[rc].position.y,
            2500 + rnd((cdata[0].fame + 1000)));
    }
    if (cdata[rc].id == 351 || cdata[rc].id == 352)
    {
        for (int cnt = 0; cnt < 12; ++cnt)
        {
            flt();
            nostack = 1;
            int stat = itemcreate(
                -1, 772, cdata[rc].position.x, cdata[rc].position.y, 0);
            if (rnd(2))
            {
                if (stat != 0)
                {
                    inv[ci].param3 = -1;
                    inv[ci].image = 336;
                }
            }
        }
    }
    cell_refresh(cdata[rc].position.x, cdata[rc].position.y);
    if (cdata[rc].character_role == 13)
    {
        supply_new_equipment();
    }
    return;
}



void label_1576()
{
    i = gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
        + gdata_year * 24 * 30 * 12;
    for (int cnt = 0; cnt < 246; ++cnt)
    {
        if (cnt == ELONA_MAX_CHARACTERS)
        {
            p = -1;
        }
        else
        {
            p = cnt;
            if (cdata[p].state == 0)
            {
                continue;
            }
        }
        for (const auto& cnt : items(p))
        {
            if (inv[cnt].number == 0)
            {
                continue;
            }
            if (inv[cnt].material == 35)
            {
                if (inv[cnt].param3 > 0)
                {
                    if (inv[cnt].param3 <= i)
                    {
                        if (inv[cnt].own_state <= 0)
                        {
                            if (p == -1)
                            {
                                if (inv[cnt].id == 204)
                                {
                                    if (chipm(
                                            0,
                                            map(inv[cnt].position.x,
                                                inv[cnt].position.y,
                                                0))
                                        == 1)
                                    {
                                        if (gdata_weather != 0)
                                        {
                                            continue;
                                        }
                                        txt(lang(
                                            itemname(cnt)
                                                + u8"は上手い具合に干された。"s,
                                            itemname(cnt) + is2(inv[cnt].number)
                                                + u8" dried up in the sun."s));
                                        inv[cnt].param3 = gdata_hour
                                            + gdata_day * 24
                                            + gdata_month * 24 * 30
                                            + gdata_year * 24 * 30 * 12 + 2160;
                                        inv[cnt].image = 337;
                                        inv[cnt].id = 571;
                                        inv[cnt].param1 = 0;
                                        inv[cnt].param2 = 5;
                                        cell_refresh(
                                            inv[cnt].position.x,
                                            inv[cnt].position.y);
                                        continue;
                                    }
                                }
                            }
                            if (p != -1)
                            {
                                if (p < 16)
                                {
                                    txt(lang(
                                        itemname(cnt) + u8"は腐った。"s,
                                        itemname(cnt) + u8" rot"s
                                            + _s2(inv[cnt].number) + u8"."s));
                                }
                            }
                            inv[cnt].param3 = -1;
                            inv[cnt].image = 336;
                            if (p == -1)
                            {
                                cell_refresh(
                                    inv[cnt].position.x, inv[cnt].position.y);
                            }
                            if (p == 0)
                            {
                                if (cdata[0].god_id == core_god::kumiromi)
                                {
                                    i = the_item_db[inv[cnt].id]->subcategory;
                                    if (rnd(3) == 0)
                                    {
                                        txt(lang(
                                            u8"あなたは"s + itemname(cnt)
                                                + u8"から種を取り出した。"s,
                                            u8"You extract plant seeds from "s
                                                + itemname(cnt) + u8"."s));
                                        p = rnd(inv[cnt].number) + 1;
                                        removeitem(cnt, inv[cnt].number);
                                        flt(calcobjlv(cdata[0].level));
                                        flttypeminor = 58500;
                                        itemcreate(0, 0, -1, -1, p);
                                        p = 0;
                                        i = gdata_hour + gdata_day * 24
                                            + gdata_month * 24 * 30
                                            + gdata_year * 24 * 30 * 12;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}



void label_1577()
{
    if (rnd(4) == 0)
    {
        dmghp(cc, cdata[cc].hp * (5 + cdata[cc].curse_power / 5) / 100, -5);
        return;
    }
    if (mdata(6) != 1)
    {
        if (rnd(10 - clamp(cdata[cc].curse_power / 10, 0, 9)) == 0)
        {
            efid = 408;
            tc = cc;
            magic();
            return;
        }
    }
    if (rnd(10) == 0)
    {
        if (cdata[cc].gold > 0)
        {
            p = rnd(cdata[cc].gold) / 100 + rnd(10) + 1;
            if (p > cdata[cc].gold)
            {
                p = cdata[cc].gold;
            }
            cdata[cc].gold -= p;
            if (is_in_fov(cc))
            {
                txtef(8);
                txt(lang(
                    u8"悪意のある手が"s + name(cc)
                        + u8"の財布から金貨を抜き去った。"s,
                    u8"A malicious hand filches some gold pieces from "s
                        + name(cc) + your(cc) + u8" wallet."s));
            }
            return;
        }
    }
    return;
}



void label_1578()
{
    if (rnd(15) == 0)
    {
        if (is_in_fov(cc))
        {
            txt(lang(
                name(cc) + u8"は不安げに腹を押さえた。"s,
                name(cc) + u8" pat"s + _s(cc) + u8" "s + his(cc)
                    + u8" stomach uneasily."s));
            txt(lang(
                    u8"「なにかが産まれそうだよ！」"s,
                    u8"\"I'm going to have a baby!\""s),
                lang(
                    u8"「腹になにかが…」"s,
                    u8"\"Something is wrong with my stomach...\""s));
        }
    }
    if (mdata(6) != 1)
    {
        if (rnd(30) == 0)
        {
            if (is_in_fov(cc))
            {
                txt(lang(
                    u8"何かが"s + name(cc) + u8"の腹を破り飛び出した！"s,
                    u8"Something splits "s + name(cc) + your(cc)
                        + u8" body and breaks out!"s));
            }
            cdata[cc].bleeding += 15;
            flt();
            initlv = cdata[cc].level / 2 + 1;
            novoidlv = 1;
            int stat = chara_create(
                -1, 330, cdata[cc].position.x, cdata[cc].position.y);
            if (stat != 0)
            {
                if (strlen_u(cdatan(0, cc)) > 10
                    || instr(cdatan(0, cc), 0, lang(u8"の子供"s, u8"child"s))
                        != -1)
                {
                    cdatan(0, rc) = lang(u8"エイリアンの子供"s, u8"alien kid"s);
                }
                else
                {
                    cdatan(0, rc) = lang(
                        cdatan(0, cc) + u8"の子供"s,
                        u8"child of "s + cdatan(0, cc));
                }
            }
        }
    }
    return;
}



void label_1579()
{
    for (int i = 0; i < 30; ++i)
    {
        if (cdata_body_part(cc, i) % 10000 == 0)
        {
            continue;
        }
        ci = cdata_body_part(cc, i) % 10000 - 1;
        for (int cnt = 0; cnt < 15; ++cnt)
        {
            if (inv[ci].enchantments[cnt].id == 0)
            {
                break;
            }
            if (inv[ci].enchantments[cnt].id == 21)
            {
                if (mdata(6) != 1)
                {
                    if (rnd(25)
                        < clamp(
                              std::abs(inv[ci].enchantments[cnt].power) / 50,
                              1,
                              25))
                    {
                        efid = 408;
                        tc = cc;
                        magic();
                        continue;
                    }
                }
            }
            if (inv[ci].enchantments[cnt].id == 45)
            {
                if (rnd(4) == 0)
                {
                    if (is_in_fov(cc))
                    {
                        txtef(8);
                        txt(lang(
                            u8"何かが"s + name(cc) + u8"の血を吸った。"s,
                            u8"Something sucks "s + name(cc) + your(cc)
                                + u8" blood."s));
                    }
                    cdata[cc].bleeding +=
                        std::abs(inv[ci].enchantments[cnt].power) / 25 + 3;
                    continue;
                }
            }
            if (inv[ci].enchantments[cnt].id == 46)
            {
                if (rnd(20) == 0)
                {
                    if (is_in_fov(cc))
                    {
                        txtef(8);
                        txt(lang(
                            name(cc) + u8"は未熟になった。"s,
                            name(cc) + u8" become"s + _s(cc)
                                + u8" inexperienced."s));
                    }
                    cdata[cc].experience -= cdata[cc].required_experience
                            / (300
                               - clamp(
                                     std::abs(inv[ci].enchantments[cnt].power)
                                         / 2,
                                     0,
                                     50))
                        + rnd(100);
                    if (cdata[cc].experience < 0)
                    {
                        cdata[cc].experience = 0;
                    }
                    continue;
                }
            }
            if (inv[ci].enchantments[cnt].id == 47)
            {
                if (mdata(6) != 1)
                {
                    if (mdata(6) != 5)
                    {
                        if (rnd(50)
                            < clamp(
                                  std::abs(inv[ci].enchantments[cnt].power)
                                      / 50,
                                  1,
                                  50))
                        {
                            if (is_in_fov(cc))
                            {
                                txtef(8);
                                txt(lang(
                                    u8"魔力の渦が何かを召喚した！"s,
                                    u8"Several creatures are summoned from a vortex of magic."s));
                            }
                            for (int cnt = 0, cnt_end = (1 + rnd(3));
                                 cnt < cnt_end;
                                 ++cnt)
                            {
                                flt(calcobjlv(cdata[0].level * 3 / 2 + 3),
                                    calcfixlv(2));
                                chara_create(
                                    -1,
                                    0,
                                    cdata[cc].position.x,
                                    cdata[cc].position.y);
                            }
                            continue;
                        }
                    }
                }
            }
        }
    }
    return;
}



void label_1580()
{
    if (cdata[0].confused != 0 || cdata[0].sleep != 0 || cdata[0].paralyzed != 0
        || cdata[0].choked != 0)
    {
        return;
    }
    for (const auto& cnt : items(0))
    {
        if (inv[cnt].number == 0
            || inv[cnt].identification_state
                == identification_state_t::completely_identified)
        {
            continue;
        }
        if (the_item_db[inv[cnt].id]->category >= 50000)
        {
            continue;
        }
        ci = cnt;
        p(0) = sdata(13, 0) + sdata(162, 0) * 5;
        p(1) = 1500 + inv[ci].difficulty_of_identification * 5;
        if (p > rnd(p(1) * 5))
        {
            s = itemname(ci);
            item_identify(
                inv[ci], identification_state_t::completely_identified);
            itemmemory(0, inv[ci].id) = 1;
            if (config::instance().hideautoidentify <= 1)
            {
                txt(lang(
                    u8"バックパックの中の"s + s + u8"は"s + itemname(ci)
                        + u8"だと判明した。"s,
                    u8"You appraise "s + s + u8" as "s + itemname(ci)
                        + u8"."s));
            }
            skillexp(162, 0, 50);
        }
        if (inv[ci].identification_state
            <= identification_state_t::partly_identified)
        {
            if (p > rnd(p(1)))
            {
                if (config::instance().hideautoidentify == 0)
                {
                    txt(lang(
                        u8"バックパックの中の"s + itemname(ci) + u8"は"s
                            + i18n::_(
                                  u8"ui", u8"quality", u8"_"s + inv[ci].quality)
                            + u8"だという感じがする。"s,
                        u8"You sense the quality of "s + itemname(ci)
                            + u8" is "s
                            + i18n::_(
                                  u8"ui", u8"quality", u8"_"s + inv[ci].quality)
                            + u8"."s));
                }
                item_identify(
                    inv[ci], identification_state_t::almost_identified);
                skillexp(162, 0, 50);
            }
        }
    }
    return;
}



void lovemiracle(int prm_932)
{
    if (rnd(2) || prm_932 == 0)
    {
        return;
    }
    cibk = ci;
    txtef(9);
    txt(lang(u8"「あ…！」"s, u8"\"Uh...!\""s));
    flt();
    if (rnd(2))
    {
        int stat = itemcreate(
            -1, 573, cdata[prm_932].position.x, cdata[prm_932].position.y, 0);
        if (stat)
        {
            inv[ci].subname = cdata[prm_932].id;
            inv[ci].weight = cdata[prm_932].weight * 10 + 250;
            inv[ci].value = clamp(
                cdata[prm_932].weight * cdata[prm_932].weight / 10000,
                200,
                40000);
        }
    }
    else
    {
        int stat = itemcreate(
            -1, 574, cdata[prm_932].position.x, cdata[prm_932].position.y, 0);
        if (stat)
        {
            inv[ci].subname = cdata[prm_932].id;
        }
    }
    ci = cibk;
    snd(107);
    animeload(15, prm_932);
    return;
}





void monster_respawn()
{
    if (adata(16, gdata_current_map) == 101
        || adata(16, gdata_current_map) == 102)
    {
        if (gdata_other_character_count < mdata(10) / 2)
        {
            if (rnd(2) == 0)
            {
                chara_set_generation_filter();
                chara_create(-1, dbid, -2, 0);
            }
        }
    }
    if (mdata(10) == 0)
    {
        return;
    }
    if (gdata_other_character_count < mdata(10) / 4)
    {
        if (rnd(2) == 0)
        {
            chara_set_generation_filter();
            chara_create(-1, dbid, -2, 0);
        }
    }
    if (gdata_other_character_count < mdata(10) / 2)
    {
        if (rnd(4) == 0)
        {
            chara_set_generation_filter();
            chara_create(-1, dbid, -2, 0);
        }
    }
    if (gdata_other_character_count < mdata(10))
    {
        if (rnd(8) == 0)
        {
            chara_set_generation_filter();
            chara_create(-1, dbid, -2, 0);
        }
    }
    return;
}



turn_result_t exit_map()
{
    int fixstart = 0;
    gdata_left_minutes_of_executing_quest = 0;
    gdata(171) = 0;
    if (mdata(6) == 5)
    {
        if (config::instance().extrahelp)
        {
            if (gdata(201) == 0)
            {
                if (mode == 0)
                {
                    if (cdata[0].continuous_action_turn == 0)
                    {
                        gdata(201) = 1;
                        ghelp = 1;
                        show_ex_help();
                    }
                }
            }
        }
    }
    msg_newline();
    msgtemp = u8"  "s;
    if (gdata_current_map == 35 || gdata_current_map == 6
        || gdata_current_map == 40)
    {
        usermapid = 0;
    }
    rdtry = 0;
    fixstart = 0;
    if (adata(16, gdata_current_map) == 8)
    {
        if (gdata_current_dungeon_level == adata(10, gdata_current_map))
        {
            if (adata(20, gdata_current_map) > 0)
            {
                chara_vanquish(adata(20, gdata_current_map));
                adata(20, gdata_current_map) = -1;
            }
        }
    }
    gdata_previous_map = gdata_current_map;
    if (gdata_previous_map == 30)
    {
        gdata_current_map = gdata_previous_map2;
        gdata_current_dungeon_level = gdata_previous_dungeon_level;
        mapstartx = gdata_previous_x;
        mapstarty = gdata_previous_y;
        gdata_entrance_type = 7;
        fixstart = 1;
        levelexitby = 5;
    }
    if (levelexitby == 4)
    {
        cell_featread(cdata[0].position.x, cdata[0].position.y);
        if (gdata_current_map == 7)
        {
            if (mapitemfind(cdata[cc].position.x, cdata[cc].position.y, 751)
                != -1)
            {
                feat(1) = 11;
                feat(2) = 0;
            }
            if (mapitemfind(cdata[cc].position.x, cdata[cc].position.y, 750)
                != -1)
            {
                feat(1) = 10;
                feat(2) = 0;
            }
        }
        f = 0;
        if (feat(1) == 11)
        {
            msgtemp +=
                lang(u8"階段を降りた。"s, u8"You walk down the stairs. "s);
            f = 1;
            gdata_entrance_type = 1;
            mdata(4) = cdata[0].position.y * 1000 + cdata[0].position.x;
            if (feat(2) == 0)
            {
                ++gdata_current_dungeon_level;
                gdata_current_dungeon_level -=
                    (adata(3, gdata_current_map) == 2) * 2;
            }
            else
            {
                gdata_current_dungeon_level = feat(2);
            }
        }
        if (feat(1) == 10)
        {
            msgtemp += lang(u8"階段を昇った。"s, u8"You walk up the stairs. "s);
            f = 1;
            gdata_entrance_type = 2;
            mdata(5) = cdata[0].position.y * 1000 + cdata[0].position.x;
            if (feat(2) == 0)
            {
                --gdata_current_dungeon_level;
                gdata_current_dungeon_level +=
                    (adata(3, gdata_current_map) == 2) * 2;
            }
            else
            {
                gdata_current_dungeon_level = feat(2);
            }
        }
        if (f == 0)
        {
            if (mdata(6) == 1)
            {
                gdata_pc_home_x = cdata[cc].position.x;
                gdata_pc_home_y = cdata[cc].position.y;
                gdata_current_dungeon_level = 1;
                if (feat(2) != 0 || feat(3) != 0)
                {
                    gdata_current_map = feat(2) + feat(3) * 100;
                    if (adata(16, gdata_current_map) == 42)
                    {
                        if (itemfind(0, 742) == -1)
                        {
                            txt(lang(
                                u8"あなたはこの洞窟の探索を許可されていない。"s,
                                u8"You are not permitted to explore this dungeon."s));
                            msg_halt();
                            gdata_current_map = 2;
                        }
                    }
                    if (adata(16, gdata_current_map) == 37)
                    {
                        if (gdata_pyramid_trial == 0)
                        {
                            txt(lang(
                                u8"あなたはピラミッドへの招待状を持っていない。"s,
                                u8"You don't have an invitation."s));
                            msg_halt();
                            gdata_current_map = 2;
                        }
                    }
                    if (adata(16, gdata_current_map) == 41)
                    {
                        txt(lang(
                            u8"あなたはガードに追い返された。"s,
                            u8"The guards turn you away from the jail."s));
                        msg_halt();
                        gdata_current_map = 2;
                    }
                }
                else
                {
                    gdata_current_map = 2;
                }
                if (feat(1) != 15)
                {
                    gdata_current_map = 2;
                }
            }
            if (mdata(6) == 3 || mdata(6) == 4 || mdata(6) == 5 || mdata(6) == 6
                || mdata(6) == 2)
            {
                gdata_current_map = gdata(850);
            }
        }
        if (mdata(6) >= 20 || mdata(6) == 6)
        {
            if (gdata_current_dungeon_level < adata(17, gdata_current_map))
            {
                gdata_current_map = gdata(850);
            }
        }
        if (adata(0, gdata_current_map) == 3)
        {
            if (gdata_current_map == gdata_previous_map)
            {
                if (gdata_current_dungeon_level == 1)
                {
                    msgtemp += lang(
                        mapname(gdata_current_map) + u8"の表層に戻った。"s,
                        u8"You returned to the surface of "s
                            + mapname(gdata_current_map));
                }
                else
                {
                    msgtemp += lang(
                        mapname(gdata_current_map) + u8"の表層を後にした。"s,
                        u8"You left the surface of "s
                            + mapname(gdata_current_map) + u8"."s);
                }
            }
        }
    }
    if (mdata(6) == 7)
    {
        rq = gdata_executing_immediate_quest;
        quest_exit_map();
        gdata_current_map = gdata_previous_map2;
        gdata_current_dungeon_level = gdata_previous_dungeon_level;
        mapstartx = gdata_previous_x;
        mapstarty = gdata_previous_y;
        gdata_entrance_type = 7;
        fixstart = 1;
    }
    if (levelexitby == 2)
    {
        gdata_current_map = gdata_destination_map;
        gdata_current_dungeon_level = gdata_destination_dungeon_level;
        if (gdata_executing_immediate_quest_type == 0)
        {
            if (gdata_previous_map != 2)
            {
                gdata_pc_home_x = adata(1, gdata_current_map);
                gdata_pc_home_y = adata(2, gdata_current_map);
                label_2735();
            }
        }
        if (gdata_current_map == 41)
        {
            mapstartx = 29;
            mapstarty = 3;
            gdata_entrance_type = 7;
            fixstart = 1;
        }
    }
    if (cdata[0].state == 0)
    {
        rc = 0;
        label_1540();
        gdata_current_map = 7;
        gdata(850) = adata(30, 7);
        gdata_current_dungeon_level = 1;
    }
    if (rdtry > 1)
    {
        msgtemp += u8"(再生成"s + rdtry + u8"回)"s;
    }
    if (gdata_current_map != gdata_previous_map)
    {
        if (mdata(6) == 3 || gdata_departure_date == 0
            || gdata_previous_map == 7 || mdata(6) == 2)
        {
            gdata_departure_date = gdata_hour + gdata_day * 24
                + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12;
            gdata_distance_between_town = 0;
            gdata_left_town_map = gdata_previous_map;
        }
        if (adata(0, gdata_current_map) != 1 && adata(0, gdata_current_map) != 4
            && gdata_current_map != 35)
        {
            autosave = 1 * (gdata_current_map != 35);
        }
        if (mdata(6) != 1)
        {
            if (mdata(6) != 4 || mdata(6) == 5)
            {
                if (fixtransfermap == 0)
                {
                    gdata_pc_home_x = adata(1, gdata_previous_map);
                    gdata_pc_home_y = adata(2, gdata_previous_map);
                }
                else
                {
                    fixtransfermap = 0;
                }
            }
        }
        if (fixstart == 0)
        {
            gdata_entrance_type = adata(3, gdata_current_map);
        }
        if (event_find(6))
        {
            msgtemp += lang(
                u8"あなたは家まで運ばれた。"s,
                u8"You were delivered to your home."s);
            label_2735();
        }
        else if (adata(0, gdata_previous_map) == 1)
        {
            msgtemp += lang(
                mapname(gdata_current_map) + u8"に入った。"s,
                u8"You entered "s + mapname(gdata_current_map) + u8"."s);
        }
        else if (mdata(6) == 7)
        {
            msgtemp += lang(
                mapname(gdata_current_map) + u8"に戻った。"s,
                u8"You returned to "s + mapname(gdata_current_map));
        }
        else
        {
            msgtemp += lang(
                mapname(gdata_previous_map) + u8"を後にした。"s,
                u8"You left "s + mapname(gdata_previous_map) + u8"."s);
        }
        if (gdata_cargo_weight > gdata_current_cart_limit)
        {
            if (adata(0, gdata_current_map) == 1
                || adata(0, gdata_current_map) == 4)
            {
                msgtemp += lang(
                    u8"荷車の重量超過でかなり鈍足になっている！ "s,
                    u8"The weight of your cargo burdens your traveling speed."s);
            }
        }
    }
    if (gdata_current_map == 25)
    {
        if (gdata_current_dungeon_level == 2)
        {
            gdata_current_map = 26;
            gdata_current_dungeon_level = adata(10, gdata_current_map) - 1;
            gdata_entrance_type = 1;
            msgtemp += lang(
                u8"マウンテンパスに降りた。"s,
                u8"You entered the Mountain Pass."s);
        }
    }
    if (gdata_current_map == 26)
    {
        if (gdata_current_dungeon_level == adata(10, gdata_current_map))
        {
            gdata_current_map = 25;
            gdata_current_dungeon_level = 1;
            gdata_entrance_type = 2;
            msgtemp += lang(
                u8"ラーナの村に辿りついた。"s,
                u8"You reached the town of Larna."s);
        }
    }
    for (int cnt = 0; cnt < 16; ++cnt)
    {
        cdata[cnt].hate = 0;
        cdata[cnt].enemy_id = 0;
        rowactend(cnt);
        if (cdata[cnt].state != 1)
        {
            if (cdata[cnt].state == 8)
            {
                cdata[cnt].state = 1;
            }
            continue;
        }
        map(cdata[cnt].position.x, cdata[cnt].position.y, 1) = 0;
        if (cnt != 0)
        {
            if (cdata[cnt].current_map != 0)
            {
                cdata[cnt].state = 9;
            }
        }
    }
    if (mdata(7) == 1)
    {
        label_1739();
    }
    else
    {
        label_1738();
        if (fs::exists(filesystem::dir::tmp() / (u8"mdata_"s + mid + u8".s2")))
        {
            ctrl_file(file_operation_t::_11);
        }
        for (int cnt = ELONA_MAX_PARTY_CHARACTERS; cnt < ELONA_MAX_CHARACTERS;
             ++cnt)
        {
            if (cdata[cnt].state != 0)
            {
                --npcmemory(1, cdata[cnt].id);
            }
        }
    }
    mode = 2;
    return turn_result_t::initialize_map;
}



void label_1738()
{
    for (int cnt = 0; cnt < 57; ++cnt)
    {
        rowactend(cnt);
        cdata[cnt].item_which_will_be_used = 0;
    }
    for (int cnt = 16; cnt < 55; ++cnt)
    {
        if (cdata[cnt].state == 1)
        {
            map(cdata[cnt].position.x, cdata[cnt].position.y, 1) = 0;
            cdata[cnt].state = 3;
        }
    }
    return;
}



void label_1739()
{
    label_1738();
    for (int cnt = 0, cnt_end = (mdata(1)); cnt < cnt_end; ++cnt)
    {
        y = cnt;
        for (int cnt = 0, cnt_end = (mdata(0)); cnt < cnt_end; ++cnt)
        {
            map(cnt, y, 7) = 0;
        }
    }
    ctrl_file(file_operation_t::_2);
    ctrl_file(file_operation2_t::_4, u8"inv_"s + mid + u8".s2");
    return;
}



void label_1745()
{
    if (gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
            + gdata_year * 24 * 30 * 12
        >= adata(25, gdata_current_map))
    {
        adata(24, gdata_current_map) = rnd(10000);
        adata(25, gdata_current_map) = gdata_hour + gdata_day * 24
            + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12 + 24;
    }
    if (gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
            + gdata_year * 24 * 30 * 12
        >= mdata(3))
    {
        if (mdata(17) == 0)
        {
            if (mdata(3) != 0)
            {
                for (int cnt = 0, cnt_end = (mdata(1)); cnt < cnt_end; ++cnt)
                {
                    y = cnt;
                    for (int cnt = 0, cnt_end = (mdata(0)); cnt < cnt_end;
                         ++cnt)
                    {
                        if (map(cnt, y, 6) != 0)
                        {
                            cell_featread(cnt, y);
                            if (feat(1) >= 24 && feat(1) <= 28)
                            {
                                cell_featclear(cnt, y);
                            }
                        }
                    }
                }
                for (const auto& cnt : items(-1))
                {
                    if (inv[cnt].number == 0)
                    {
                        continue;
                    }
                    if (inv[cnt].id == 526)
                    {
                        if (inv[cnt].param1 < 10)
                        {
                            inv[cnt].param1 += 1;
                            inv[cnt].image = 591;
                            cell_refresh(
                                inv[cnt].position.x, inv[cnt].position.y);
                        }
                    }
                    if (mdata(6) == 3 || mdata(6) == 2)
                    {
                        if (inv[cnt].own_state < 0)
                        {
                            ++inv[cnt].own_state;
                        }
                        if (inv[cnt].own_state == 0)
                        {
                            inv[cnt].number = 0;
                            cell_refresh(
                                inv[cnt].position.x, inv[cnt].position.y);
                        }
                    }
                }
            }
            for (int cnt = ELONA_MAX_PARTY_CHARACTERS;
                 cnt < ELONA_MAX_CHARACTERS;
                 ++cnt)
            {
                rc = cnt;
                label_1539();
                if (cdata[cnt].state != 1)
                {
                    continue;
                }
                if (cdata[cnt].is_temporary() == 1)
                {
                    if (rnd(2))
                    {
                        cdata[cnt].state = 0;
                        map(cdata[cnt].position.x, cdata[cnt].position.y, 1) =
                            0;
                    }
                }
            }
            p = rnd(mdata(1) * mdata(0) / 400 + 3);
            if (mdata(6) == 1)
            {
                p = rnd(40);
            }
            if (mdata(6) == 3)
            {
                p = rnd(rnd(rnd(12) + 1) + 1);
            }
            if (mdata(6) == 2)
            {
                p = rnd(p + 1);
            }
            for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
            {
                map_randsite();
            }
        }
        mdata(3) = gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
            + gdata_year * 24 * 30 * 12 + 120;
    }
    if (gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
            + gdata_year * 24 * 30 * 12
        >= mdata(16))
    {
        if (mdata(16) == 0)
        {
            renewmulti = 1;
        }
        else
        {
            renewmulti = (gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
                          + gdata_year * 24 * 30 * 12 - mdata(16))
                    / 24
                + 1;
        }
        if (adata(16, gdata_current_map) == 31)
        {
            update_ranch();
        }
        if (adata(16, gdata_current_map) == 7)
        {
            for (int cnt = ELONA_MAX_PARTY_CHARACTERS;
                 cnt < ELONA_MAX_CHARACTERS;
                 ++cnt)
            {
                if (cdata[cnt].state != 1)
                {
                    continue;
                }
                if (cdata[cnt].is_temporary() == 1)
                {
                    cdata[cnt].state = 0;
                    map(cdata[cnt].position.x, cdata[cnt].position.y, 1) = 0;
                }
            }
        }
        if (mdata(17) == 0)
        {
            for (int cnt = 0, cnt_end = (mdata(1)); cnt < cnt_end; ++cnt)
            {
                y = cnt;
                for (int cnt = 0, cnt_end = (mdata(0)); cnt < cnt_end; ++cnt)
                {
                    if (map(cnt, y, 6) != 0)
                    {
                        cell_featread(cnt, y);
                        x = cnt;
                        if (feat(1) == 29)
                        {
                            for (int cnt = 0, cnt_end = (renewmulti);
                                 cnt < cnt_end;
                                 ++cnt)
                            {
                                if (feat >= tile_plant + 2)
                                {
                                    break;
                                }
                                label_2233(chipm(0, map(x, y, 0)) == 2 ? 1 : 0);
                            }
                            cell_featset(
                                cnt, y, feat, feat(1), feat(2), feat(3));
                        }
                    }
                }
            }
            if (mdata(6) == 3 || mdata(6) == 2 || gdata_current_map == 7)
            {
                for (int cnt = ELONA_MAX_PARTY_CHARACTERS;
                     cnt < ELONA_MAX_CHARACTERS;
                     ++cnt)
                {
                    if (cdata[cnt].state != 1)
                    {
                        continue;
                    }
                    generatemoney(cnt);
                    if (cdata[cnt].id == 326)
                    {
                        if (itemfind(cnt, 60005, 1) == -1)
                        {
                            if (rnd(150) == 0)
                            {
                                flt();
                                itemcreate(cnt, 707, -1, -1, 0);
                            }
                            else
                            {
                                flt(calcobjlv(cdata[cnt].level), calcfixlv());
                                flttypeminor = 60005;
                                itemcreate(cnt, 0, -1, -1, 0);
                            }
                        }
                    }
                    rc = cnt;
                    if (rnd(5) == 0)
                    {
                        supply_new_equipment();
                    }
                    if (rnd(2) == 0)
                    {
                        if (inv_sum(rc) < 8)
                        {
                            flt(calcobjlv(cdata[rc].level), calcfixlv(2));
                            int stat = itemcreate(rc, 0, -1, -1, 0);
                            if (stat != 0)
                            {
                                if (inv[ci].weight <= 0
                                    || inv[ci].weight >= 4000)
                                {
                                    inv[ci].number = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        mdata(16) = gdata_hour + gdata_day * 24 + gdata_month * 24 * 30
            + gdata_year * 24 * 30 * 12 + 24;
    }
    return;
}



void label_1746()
{
    gsel(6);
    if (mdata(2) != mtilefilecur)
    {
        pos(0, 0);
        picload(
            filesystem::dir::graphic() / (u8"map"s + mdata(2) + u8".bmp"), 1);
        mtilefilecur = mdata(2);
        initialize_map_chip();
    }
    map_tileset(mdata(12));
    gsel(2);
    gmode(0);
    pos(0, 0);
    gcopy(6, 0, 0, 33 * inf_tiles, 25 * inf_tiles);
    p(0) = 5;
    p(1) = 5;
    p(2) = 5;
    shade = 15;
    if (mdata(14) == 2)
    {
        if (gdata_hour >= 24)
        {
            p(0) = 110;
            p(1) = 90;
            p(2) = 60;
            shade = 8;
        }
        if (gdata_hour >= 0 && gdata_hour < 4)
        {
            p(0) = 110;
            p(1) = 90;
            p(2) = 60;
            shade = 8;
        }
        if (gdata_hour >= 4 && gdata_hour < 10)
        {
            p(0) = 70 - (gdata_hour - 3) * 10;
            p(1) = 80 - (gdata_hour - 3) * 12;
            p(2) = 60 - (gdata_hour - 3) * 10;
            if (p < 10)
            {
                p = 10;
                shade = (gdata_hour - 3) * 2 + 8;
            }
        }
        if (gdata_hour >= 10 && gdata_hour < 12)
        {
            p(0) = 10;
            p(1) = 10;
            p(2) = 10;
            shade = 25;
        }
        if (gdata_hour >= 12 && gdata_hour < 17)
        {
            p(0) = 0;
            p(1) = 0;
            p(2) = 0;
            shade = 25;
        }
        if (gdata_hour >= 17 && gdata_hour < 21)
        {
            p(0) = 0 + (gdata_hour - 17) * 20;
            p(1) = 15 + (gdata_hour - 16) * 15;
            p(2) = 10 + (gdata_hour - 16) * 10;
            shade = 12;
        }
        if (gdata_hour >= 21 && gdata_hour < 24)
        {
            p(0) = 80 + (gdata_hour - 21) * 10;
            p(1) = 70 + (gdata_hour - 21) * 10;
            p(2) = 40 + (gdata_hour - 21) * 5;
            shade = 7;
        }
        if (gdata_weather == 3)
        {
            if (p < 40)
            {
                p(0) = 40;
                p(1) = 40;
                p(2) = 40;
            }
        }
        if (gdata_weather == 4)
        {
            if (p < 65)
            {
                p(0) = 65;
                p(1) = 65;
                p(2) = 65;
            }
        }
        if (gdata_current_map == 33)
        {
            if (gdata_hour >= 17 || gdata_hour < 7)
            {
                p += 35;
                p(1) += 35;
                p(2) += 35;
            }
        }
    }
    pos(0, 0);
    gfini(33 * inf_tiles, 25 * inf_tiles);
    gfdec2(p, p(1), p(2));
    gmode(4, -1, -1, 30);
    if (mdata(2) == 0)
    {
        pos(0, 192);
        gcopy(6, 0, 192, 1360, 48);
    }
    if (mdata(2) == 1)
    {
        pos(0, 1056);
        gcopy(6, 0, 1056, 1360, 48);
    }
    if (mdata(2) != 2)
    {
        pos(0, 336);
        gcopy(6, 0, 336, 1360, 48);
    }
    gmode(0);
    gsel(0);
    gmode(2, 24, 24);
    return;
}


int random_material(int prm_1028, int prm_1029)
{
    int f_at_m174 = 0;
    int lv_at_m174 = 0;
    int rare_at_m174 = 0;
    int loc_at_m174 = 0;
    int p_at_m174 = 0;
    int f2_at_m174 = 0;
    int p2_at_m174 = 0;
    f_at_m174 = 0;
    lv_at_m174 = prm_1028;
    rare_at_m174 = prm_1029;
    loc_at_m174 = atxspot;
    for (int cnt = 0; cnt < 500; ++cnt)
    {
        p_at_m174 = rnd(55);
        if (p_at_m174 == 0)
        {
            continue;
        }
        if (cnt % 10 == 0)
        {
            ++lv_at_m174;
            ++rare_at_m174;
        }
        if (matref(0, p_at_m174) > lv_at_m174)
        {
            continue;
        }
        if (matref(1, p_at_m174) > rare_at_m174)
        {
            continue;
        }
        f2_at_m174 = 0;
        for (int cnt = 0; cnt < 5; ++cnt)
        {
            p2_at_m174 = matspot(cnt, p_at_m174);
            if (p2_at_m174 == 0)
            {
                break;
            }
            if (p2_at_m174 == loc_at_m174 || p2_at_m174 == 18)
            {
                f2_at_m174 = 1;
                break;
            }
        }
        if (f2_at_m174 == 0)
        {
            continue;
        }
        if (rnd(matref(1, p_at_m174)) == 0)
        {
            f_at_m174 = p_at_m174;
            break;
        }
    }
    return f_at_m174;
}



void matgetmain(int prm_1030, int prm_1031, int prm_1032)
{
    n_at_m174 = prm_1031;
    if (n_at_m174 == 0)
    {
        n_at_m174 = 1;
    }
    mat(prm_1030) += n_at_m174;
    snd(21);
    if (en)
    {
        s_at_m174 =
            u8"You get "s + n_at_m174 + u8" "s + matname(prm_1030) + u8". "s;
    }
    else
    {
        s_at_m174 = u8"マテリアル:"s + matname(prm_1030) + u8"を"s + n_at_m174
            + u8"個"s;
        if (prm_1032 == 1)
        {
            s_at_m174 += u8"掘り当てた"s;
        }
        if (prm_1032 == 2)
        {
            s_at_m174 += u8"釣り上げた。"s;
        }
        if (prm_1032 == 3)
        {
            s_at_m174 += u8"採取した。"s;
        }
        if (prm_1032 == 5)
        {
            s_at_m174 += u8"見つけた。"s;
        }
        if (prm_1032 == 0)
        {
            s_at_m174 += u8"入手した"s;
        }
    }
    txtef(4);
    txt(s_at_m174 + u8"("s + mat(prm_1030) + u8") "s);
    return;
}



void matdelmain(int prm_1033, int prm_1034)
{
    n_at_m174 = prm_1034;
    if (n_at_m174 == 0)
    {
        n_at_m174 = 1;
    }
    mat(prm_1033) -= n_at_m174;
    s_at_m174 = u8"マテリアル:"s + matname(prm_1033) + u8"を"s + n_at_m174
        + u8"個失った"s;
    txtef(4);
    txt(s_at_m174 + u8"(残り "s + mat(prm_1033) + u8"個) "s);
    return;
}



void atxinit()
{
    if (atxthrough == 0)
    {
        atbuff = "";
        notesel(atbuff);
    }
    atxthrough = 0;
    listmax = 0;
    cs = 0;
    page = 0;
    cc = 0;
    cs_bk = -1;
    pagesize = 16;
    chatesc = -1;
    if (atxbg != atxbgbk)
    {
        mode = 0;
        screenupdate = -1;
        update_screen();
        mode = 9;
        atxbgbk = atxbg;
        gsel(4);
        gmode(0);
        pos(0, 0);
        picload(filesystem::dir::graphic() / (atxbg + u8".bmp"s), 1);
        pos(0, inf_msgh);
        gzoom(4, 0, 0, 240, 160, windoww, windowh - inf_verh - inf_msgh);
        gmode(2);
        p = windoww / 192;
        for (int cnt = 0, cnt_end = (p + 1); cnt < cnt_end; ++cnt)
        {
            if (cnt == p)
            {
                sx = windoww % 192;
            }
            else
            {
                sx = 192;
            }
            pos(cnt * 192, 0);
            gcopy(3, 496, 528, sx, inf_msgh);
        }
        window2(windoww - 208, 0, 208, 98, 0, 0);
        pos(windoww - 204, 4);
        gzoom(0, 120, 88, windoww - 120, windowh - inf_verh - 112, 200, 90);
        gsel(0);
    }
    return;
}



void begintempinv()
{
    ctrl_file(file_operation2_t::_4, u8"shoptmp.s2");
    for (const auto& cnt : items(-1))
    {
        inv[cnt].number = 0;
    }
    return;
}



void exittempinv()
{
    ctrl_file(file_operation2_t::_3, u8"shoptmp.s2");
    return;
}


void label_1892()
{
    efid = 1114;
    efp = 500;
    tc = 0;
    magic();
    if (rnd(2))
    {
        efid = 622;
        efp = 250;
        tc = 0;
        magic();
        snd(63);
    }
    if (rnd(2))
    {
        efid = 1106;
        efp = 100;
        tc = 0;
        magic();
    }
    return;
}





void label_1901()
{
    invfile = 4;
    ctrl_file(file_operation2_t::_4, u8"shoptmp.s2");
    if (fs::exists(filesystem::dir::tmp() / u8"shop4.s2"s))
    {
        ctrl_file(file_operation2_t::_3, u8"shop4.s2"s);
    }
    else
    {
        for (const auto& cnt : items(-1))
        {
            inv[cnt].number = 0;
        }
    }
    mode = 6;
    income(0) = 0;
    income(1) = 0;
    for (int cnt = 0; cnt < 9; ++cnt)
    {
        if (gdata(120 + cnt) >= 10000)
        {
            continue;
        }
        p = calcincome(cnt) + rnd((calcincome(cnt) / 3 + 1))
            - rnd((calcincome(cnt) / 3 + 1));
        income += p;
        flt();
        itemcreate(-1, 54, -1, -1, p);
        if (cnt == 5 || cnt == 6)
        {
            continue;
        }
        if (cnt == 5 || cnt == 6 || cnt == 3)
        {
            continue;
        }
        p = rnd(rnd(3) + 1) + 1;
        int cnt2 = cnt;
        for (int cnt = 0, cnt_end = (p); cnt < cnt_end; ++cnt)
        {
            dbid = 0;
            flt(calcobjlv((100 - gdata((120 + cnt2)) / 100) / 2 + 1),
                calcfixlv(3 + (rnd(12) < trait(39))));
            flttypemajor = fsetincome(rnd(length(fsetincome)));
            if (rnd(5) == 0)
            {
                flttypemajor = fsetwear(rnd(length(fsetwear)));
            }
            if (rnd(100 + gdata((120 + cnt2)) / 5) < 2)
            {
                dbid = 559;
            }
            itemcreate(-1, dbid, -1, -1, 0);
            ++income(1);
        }
    }
    if (cdata[0].fame > 0)
    {
        p = clamp(cdata[0].fame / 10, 100, 25000);
        if (cdata[0].fame >= 25000)
        {
            p += (cdata[0].fame - 25000) / 100;
        }
        income += p;
        flt();
        itemcreate(-1, 54, -1, -1, p);
    }
    s = ""s + income + lang(u8"gold"s, u8" gold pieces"s);
    if (income(1) != 0)
    {
        s += lang(
            u8"と"s + income(1) + u8"個のアイテム"s,
            u8" and "s + income(1) + u8" items"s);
    }
    if (income != 0 || income(1) != 0)
    {
        snd(24);
        txtef(5);
        txt(lang(
            s + u8"が給料として振り込まれた。"s,
            u8"As a salary, "s + s + u8" have been sent to your house."s));
        autosave = 1 * (gdata_current_map != 35);
    }
    if (gdata_day == 1)
    {
        if (cdata[0].level > 5)
        {
            autosave = 1 * (gdata_current_map != 35);
            p = -1;
            for (const auto& cnt : items(-1))
            {
                if (inv[cnt].number == 0)
                {
                    p = cnt;
                    break;
                }
            }
            if (p == -1)
            {
                inv_compress(-1);
            }
            flt();
            itemcreate(-1, 615, -1, -1, 0);
            inv[ci].subname =
                gdata_cost_to_hire + calccostbuilding() + calccosttax();
            inv[ci].subname = inv[ci].subname * (100 + rnd(20)) / 100;
            mode = 0;
            ++gdata_left_bill;
            txt(lang(
                u8"請求書が送られてきた。"s,
                u8"A bill has been sent to your house."s));
            if (gdata_left_bill > 1)
            {
                if (gdata_left_bill <= 4)
                {
                    txtef(3);
                    if (gdata_left_bill > 3)
                    {
                        s(0) = lang(u8"警告！！"s, u8"Warning!! "s);
                        s(1) = lang(
                            u8"早急にパルミア大使館まで行き、税金を納めなければならない。"s,
                            u8"You have to go to the Embassy of Palmia and pay tax at once. "s);
                    }
                    else
                    {
                        s(0) = lang(u8"注意！"s, u8"Caution! "s);
                        s(1) = "";
                    }
                    txt(s
                        + lang(
                              (u8"税金を"s + (gdata_left_bill - 1)
                               + u8"ヶ月分滞納している。"s),
                              (u8"You are in arrears with your tax for "s
                               + (gdata_left_bill - 1) + u8" months."s))
                        + s(1));
                }
            }
            if (gdata_left_bill > 4)
            {
                txtef(3);
                txt(lang(
                    u8"あなたは税金を"s + (gdata_left_bill - 1)
                        + u8"ヶ月滞納した罪で訴えられた。"s,
                    u8"You have been accused for being in arrears with your tax for"s
                        + (gdata_left_bill - 1) + u8" months."s));
                int stat = decfame(0, 50);
                p = stat;
                txtef(3);
                txt(lang(
                    u8"名声値を"s + p + u8"失った。"s,
                    u8"You lose "s + p + u8" fame."s));
                modify_karma(0, -30 * 2);
            }
        }
        else
        {
            txt(lang(
                u8"レベルが6に達していないので納税の義務はない。"s,
                u8"You don't have to pay tax until you hit level 6."s));
        }
    }
    ctrl_file(file_operation2_t::_4, u8"shop"s + invfile + u8".s2");
    ctrl_file(file_operation2_t::_3, u8"shoptmp.s2");
    mode = 0;
    if (config::instance().extrahelp)
    {
        if (gdata(216) == 0)
        {
            if (mode == 0)
            {
                if (cdata[0].continuous_action_turn == 0)
                {
                    gdata(216) = 1;
                    ghelp = 16;
                    show_ex_help();
                }
            }
        }
    }
    return;
}



std::string txtitemoncell(int prm_1055, int prm_1056)
{
    elona_vector1<int> p_at_m185;
    elona_vector1<int> i_at_m185;
    const auto item_info = cell_itemoncell({prm_1055, prm_1056});
    const auto number = item_info.first;
    const auto item = item_info.second;

    if (number <= 3)
    {
        if (map(prm_1055, prm_1056, 5) < 0)
        {
            rtvaln = "";
            p_at_m185(0) = -map(prm_1055, prm_1056, 5);
            p_at_m185(1) = 0;
            i_at_m185(0) = p_at_m185 % 1000 + 5080;
            i_at_m185(1) = p_at_m185 / 1000 % 1000 + 5080;
            i_at_m185(2) = p_at_m185 / 1000000 % 1000 + 5080;
            for (int cnt = 0; cnt < 3; ++cnt)
            {
                if (i_at_m185(cnt) == 6079)
                {
                    continue;
                }
                if (p_at_m185(1) != 0)
                {
                    rtvaln += lang(u8"と"s, u8" and "s);
                }
                rtvaln += itemname(i_at_m185(cnt));
                ++p_at_m185(1);
            }
        }
        else
        {
            rtvaln = itemname(item);
        }
        if (inv[item].own_state <= 0)
        {
            return lang(
                rtvaln + u8"が落ちている。"s,
                u8"You see "s + rtvaln + u8" here."s);
        }
        else if (inv[item].own_state == 3)
        {
            return lang(
                rtvaln + u8"が設置されている。"s,
                ""s + rtvaln + u8" is constructed here."s);
        }
        else
        {
            return lang(
                rtvaln + u8"がある。"s,
                u8"You see "s + rtvaln + u8" placed here."s);
        }
    }
    else
    {
        return lang(
            u8"ここには"s + number + u8"種類のアイテムがある。"s,
            u8"There are "s + number + u8" items lying here."s);
    }
}



void txttargetnpc(int prm_1057, int prm_1058, int prm_1059)
{
    int dy_at_m186 = 0;
    int i_at_m186 = 0;
    int p_at_m186 = 0;
    dy_at_m186 = 0;
    font(14 - en * 2);
    if (prm_1059 == 0)
    {
        if (fov_los(
                cdata[0].position.x, cdata[0].position.y, prm_1057, prm_1058)
                == 0
            || dist(
                   cdata[0].position.x, cdata[0].position.y, prm_1057, prm_1058)
                > cdata[0].vision_distance / 2)
        {
            pos(100, windowh - inf_verh - 45 - dy_at_m186 * 20);
            ++dy_at_m186;
            bmes(
                lang(u8"視界範囲外"s, u8"This location is out of sight."s),
                255,
                2552,
                255);
            cansee = 0;
            return;
        }
    }
    if (map(prm_1057, prm_1058, 1) != 0)
    {
        i_at_m186 = map(prm_1057, prm_1058, 1) - 1;
        if (cdata[i_at_m186].is_invisible() == 0 || cdata[0].can_see_invisible()
            || cdata[i_at_m186].wet)
        {
            tc = i_at_m186;
            s = txttargetlevel(cc, tc);
            pos(100, windowh - inf_verh - 45 - dy_at_m186 * 20);
            ++dy_at_m186;
            bmes(s, 255, 2552, 255);
            pos(100, windowh - inf_verh - 45 - dy_at_m186 * 20);
            ++dy_at_m186;
            bmes(
                lang(u8"現在のターゲットは"s, u8"You are targeting "s)
                    + name(i_at_m186) + lang(u8" (距離 "s, u8".(Distance "s)
                    + dist(
                          cdata[0].position.x,
                          cdata[0].position.y,
                          cdata[i_at_m186].position.x,
                          cdata[i_at_m186].position.y)
                    + u8")"s,
                255,
                2552,
                255);
        }
    }
    if (map(prm_1057, prm_1058, 5) != 0)
    {
        pos(100, windowh - inf_verh - 45 - dy_at_m186 * 20);
        ++dy_at_m186;
        bmes(txtitemoncell(prm_1057, prm_1058), 255, 2552, 255);
    }
    if (map(prm_1057, prm_1058, 6) != 0)
    {
        if (mdata(6) == 1)
        {
            if (map(prm_1057, prm_1058, 6) / 1000 % 100 == 15)
            {
                p_at_m186 = map(prm_1057, prm_1058, 6) / 100000 % 100
                    + map(prm_1057, prm_1058, 6) / 10000000 * 100;
                pos(100, windowh - inf_verh - 45 - dy_at_m186 * 20);
                ++dy_at_m186;
                bmes(mapname(p_at_m186, true), 255, 2552, 255);
            }
            if (map(prm_1057, prm_1058, 6) / 1000 % 100 == 34)
            {
                pos(100, windowh - inf_verh - 45 - dy_at_m186 * 20);
                ++dy_at_m186;
                bmes(
                    txtbuilding(
                        map(prm_1057, prm_1058, 6) / 100000 % 100,
                        map(prm_1057, prm_1058, 6) / 10000000),
                    255,
                    2552,
                    255);
            }
        }
    }
    cansee = 1;
    return;
}



int key_direction()
{
    kdx = 0;
    kdy = 0;
    if (key == key_north)
    {
        --kdy;
        return 1;
    }
    if (key == key_south)
    {
        ++kdy;
        return 1;
    }
    if (key == key_west)
    {
        --kdx;
        return 1;
    }
    if (key == key_east)
    {
        ++kdx;
        return 1;
    }
    if (key == key_northwest)
    {
        --kdx;
        --kdy;
        return 1;
    }
    if (key == key_northeast)
    {
        ++kdx;
        --kdy;
        return 1;
    }
    if (key == key_southwest)
    {
        --kdx;
        ++kdy;
        return 1;
    }
    if (key == key_southeast)
    {
        ++kdx;
        ++kdy;
        return 1;
    }
    return 0;
}




int label_19432()
{
label_19431_internal:
    if (1 && gdata_wizard == 0)
    {
        snd(44);
        do_save_game();
        txtef(5);
        txt(lang(u8" *保存* "s, u8"*saving*"s));
    }
    snd(26);
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    SDIM2(headtemp, 1024);
    const auto pattern =
        comctrl == 1 ? std::regex{u8R"(.*\.ept)"} : std::regex{u8R"(.*\.eum)"};
    for (const auto& entry :
         filesystem::dir_entries{filesystem::dir::user(),
                                 filesystem::dir_entries::type::file,
                                 pattern})
    {
        const auto path = entry.path();
        if (path == filesystem::path(u8"temp.enum")
            || path == filesystem::path(u8"temp.ept"))
            continue;
        bload(path, headtemp, 1024);
        notesel(headtemp);
        noteget(s, 0);
        noteget(s(1), 1);
        noteget(s(2), 4);
        list(0, listmax) = listmax;
        list(1, listmax) = 0;
        list(2, listmax) = elona::stoi(s(2));
        listn(0, listmax) = u8"("s
            + filesystem::make_preferred_path_in_utf8(path.filename()) + u8") "s
            + s + u8" "s + s(1);
        listn(1, listmax) = filesystem::to_utf8_path(path.filename());
        ++listmax;
        noteunsel();
    }
    if (config::instance().net != 0)
    {
        if (comctrl == 1)
        {
            net_dllist(u8".ept"s, 1);
        }
        if (comctrl == 2)
        {
            net_dllist(u8".eum"s, 1);
        }
    }
    if (comctrl == 0 || comctrl == 2)
    {
        txt(lang(
            u8"どのルームを訪問する？ "s,
            u8"Which room do you want to visit? "s));
    }
    if (comctrl == 1)
    {
        txt(lang(
            u8"どのチームと対戦する？"s,
            u8"Which team do you want to play a match? "s));
    }
    windowshadow = 1;
label_1944_internal:
    cs_bk = -1;
    pagemax = (listmax - 1) / pagesize;
    if (page < 0)
    {
        page = pagemax;
    }
    else if (page > pagemax)
    {
        page = 0;
    }
label_1945_internal:
    if (comctrl == 0 || comctrl == 2)
    {
        s = lang(u8"ルーム一覧"s, u8"Room List"s);
    }
    if (comctrl == 1)
    {
        s = lang(u8"ペットチーム一覧"s, u8"Team List"s);
    }
    s(1) = lang(u8"BackSpace [削除]  "s, u8"BackSpace [Delete]  "s) + strhint2
        + strhint3;
    display_window((windoww - 640) / 2 + inf_screenx, winposy(448), 640, 448);
    if (comctrl == 0 || comctrl == 2)
    {
        s = lang(u8"ルームの名称"s, u8"Name"s);
    }
    if (comctrl == 1)
    {
        s = lang(u8"チームの名称"s, u8"Name"s);
    }
    display_topic(s, wx + 28, wy + 36);
    display_topic(u8"Ver"s, wx + 534, wy + 36);
    keyrange = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        if (cnt % 2 == 0)
        {
            pos(wx + 70, wy + 66 + cnt * 19);
            gfini(540, 18);
            gfdec2(12, 14, 16);
        }
        display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
    }
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        s = listn(0, p);
        cutname(s, 64);
        cs_list(cs == cnt, s, wx + 84, wy + 66 + cnt * 19 - 1);
        pos(wx + 570, wy + 66 + cnt * 19);
        mes(""s + (list(2, p) % 1000 + 1) + u8".0"s);
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        if (p == -999)
        {
            snd(27);
            txt(lang(
                u8"互換性のないバージョンのファイルです。"s,
                u8"Selected item is incompatible."s));
            goto label_1944_internal;
        }
        if (listn(1, p) == u8"net"s)
        {
            if (comctrl == 1)
            {
                s = u8".ept"s;
            }
            else
            {
                s = u8".eum"s;
            }
            int stat = net_dl(""s + list(1, p) + s, u8"temp"s + s);
            usermapid = list(1, p);
            if (stat == 0)
            {
                txt(lang(
                    u8"ファイルの取得に失敗した。"s,
                    u8"Failed to retrieve designated files."s));
                goto label_1944_internal;
            }
            userfile = u8"temp"s + s;
        }
        else
        {
            userfile = listn(1, p);
        }
        bload(filesystem::dir::user() / userfile, headtemp, 1024);
        notesel(headtemp);
        noteget(s, 5);
        noteget(s(1), 6);
        if (comctrl == 0 || comctrl == 2)
        {
            username = ""s + s;
            userrelation = elona::stoi(s(1));
        }
        if (comctrl == 1)
        {
            rtval(0) = elona::stoi(s(0));
            rtval(1) = elona::stoi(s(1));
        }
        return 1;
    }
    if (ginfo(2) == 0)
    {
        if (noteinfo() != 0)
        {
            if (getkey(snail::key::backspace))
            {
                userfile = listn(1, cs + pagesize * page);
                if (!fs::exists(filesystem::path(u8"./user/"s + userfile)))
                {
                    goto label_1944_internal;
                }
                txt(lang(
                    u8"本当に"s + userfile + u8"を削除する？ "s,
                    u8"Do you really want to delete "s + userfile + u8"? "s));
                ELONA_YES_NO_PROMPT();
                rtval = show_prompt(promptx, prompty, 160);
                if (rtval == 0)
                {
                    elona_delete(filesystem::path(u8"./user/"s + userfile));
                    goto label_19431_internal;
                }
                goto label_1944_internal;
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_1944_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_1944_internal;
        }
    }
    if (key == key_cancel)
    {
        update_screen();
        return 0;
    }
    goto label_1945_internal;
}




void label_1955()
{
    ww = 33;
    wh = 20;
    wx = 0;
    wy = 0;
    snd(26);
label_1956_internal:
    gmode(0);
    p = 0;
    for (int cnt = 0, cnt_end = (wh); cnt < cnt_end; ++cnt)
    {
        int cnt2 = cnt;
        for (int cnt = 0, cnt_end = (ww); cnt < cnt_end; ++cnt)
        {
            if (p < listmax)
            {
                pos(wx + cnt * 24, wy + cnt2 * 24);
                gzoom(
                    2,
                    list(0, p) % ww * 48,
                    list(0, p) / ww * 48,
                    48,
                    48,
                    24,
                    24);
                if (chipm(7, list(0, p)) & 4)
                {
                    color(240, 230, 220);
                    boxl(
                        wx + cnt * 24,
                        wy + cnt2 * 24,
                        wx + cnt * 24 + 24,
                        wy + cnt2 * 24 + 24);
                    color(0, 0, 0);
                }
            }
            ++p;
        }
    }
    gmode(2);
    redraw();
    await(config::instance().wait1);
    int a{};
    stick(a);
    if (a == 256)
    {
        p = mousex / 24 + mousey / 24 * ww;
        if (p >= listmax)
        {
            snd(27);
            goto label_1956_internal;
        }
        tile = list(0, p);
        snd(20);
        label_1958();
        return;
    }
    if (a == 512)
    {
        label_1958();
        return;
    }
    goto label_1956_internal;
}




turn_result_t show_skill_list()
{
    listmax = 0;
    page = 0;
    pagesize = 16;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    cs = commark(0) % 1000;
    page = commark(0) / 1000;
    curmenu = 1;
    for (int cnt = 300; cnt < 400; ++cnt)
    {
        if (sdata(cnt, cc) > 0)
        {
            list(0, listmax) = cnt;
            list(1, listmax) =
                the_ability_db[cnt]->related_basic_attribute * 1000 + cnt;
            ++listmax;
        }
    }
    for (int cnt = 0; cnt < 61; ++cnt)
    {
        if (spact(cnt) != 0)
        {
            list(0, listmax) = cnt + 600;
            list(1, listmax) =
                the_ability_db[cnt + 600]->related_basic_attribute * 1000 + cnt;
            ++listmax;
        }
    }
    sort_list_by_column1();
    gsel(3);
    pos(960, 96);
    picload(filesystem::dir::graphic() / u8"deco_skill.bmp", 1);
    gsel(0);
    windowshadow = 1;
label_2008_internal:
    cs_bk = -1;
    pagemax = (listmax - 1) / pagesize;
    if (page < 0)
    {
        page = pagemax;
    }
    else if (page > pagemax)
    {
        page = 0;
    }
    drawmenu(1);
label_2009_internal:
    s(0) = lang(u8"能力の発動"s, u8"Skill"s);
    s(1) = strhint2 + strhint3 + strhint7;
    display_window(
        (windoww - 600) / 2 + inf_screenx, winposy(438), 600, 438, 0, 60);
    display_topic(lang(u8"能力の名称"s, u8"Name"s), wx + 28, wy + 36);
    display_topic(lang(u8"コスト"s, u8"Cost"s), wx + 220, wy + 36);
    display_topic(lang(u8"能力の効果"s, u8"Detail"s), wx + 320, wy + 36);
    pos(wx + 46, wy - 16);
    gcopy(3, 960, 48, 48, 48);
    pos(wx + ww - 78, wy + wh - 165);
    gcopy(3, 960, 96, 72, 144);
    pos(wx + ww - 168, wy);
    gcopy(3, 1032, 96, 102, 48);
    keyrange = 0;
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        key_list(cnt) = key_select(cnt);
        ++keyrange;
        if (cnt % 2 == 0)
        {
            pos(wx + 70, wy + 66 + cnt * 19);
            gfini(490, 18);
            gfdec2(12, 14, 16);
        }
        display_key(wx + 58, wy + 66 + cnt * 19 - 2, cnt);
    }
    font(14 - en * 2);
    cs_listbk();
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        i = list(0, p);
        pos(wx + 40, wy + 74 + cnt * 19);
        gmode(2, inf_tiles, inf_tiles);
        grotate(
            1,
            (the_ability_db[list(0, p)]->related_basic_attribute - 10)
                * inf_tiles,
            672,
            0,
            inf_tiles,
            inf_tiles);
        s = "";
        for (int cnt = 0; cnt < 20; ++cnt)
        {
            if (gdata(40 + cnt) == list(0, p))
            {
                s = u8"{"s + cnt + u8"}"s;
            }
        }
        cs_list(
            cs == cnt,
            i18n::_(u8"ability", std::to_string(list(0, p)), u8"name") + s,
            wx + 84,
            wy + 66 + cnt * 19 - 1);
        s = ""s + the_ability_db[list(0, p)]->cost + u8" Sp"s;
        pos(wx + 288 - strlen_u(s) * 7, wy + 66 + cnt * 19 + 2);
        mes(s);
        label_2031();
        pos(wx + 325, wy + 66 + cnt * 19 + 2);
        mes(strmid(s, 0, 34));
    }
    if (keyrange != 0)
    {
        cs_bk = cs;
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    commark(0) = page * 1000 + cs;
    ELONA_GET_SELECTED_ITEM(p, 0);
    if (p != -1)
    {
        menucycle = 0;
        efid = p;
        screenupdate = -1;
        update_screen();
        return do_use_magic();
    }
    if (key == u8"sc"s)
    {
        snd(20);
        p = list(0, pagesize * page + cs);
        if (gdata(40 + sc) == p)
        {
            gdata(40 + sc) = 0;
            goto label_2008_internal;
        }
        for (int cnt = 0; cnt < 20; ++cnt)
        {
            if (gdata(40 + cnt) == p)
            {
                gdata(40 + cnt) = 0;
            }
        }
        gdata(40 + sc) = p;
        txt(lang(
            u8"{"s + sc + u8"}キーにショートカットを割り当てた。"s,
            u8"You have assigned the shortcut to {"s + sc + u8"} key."s));
        display_msg(inf_screeny + inf_tiles);
        goto label_2008_internal;
    }
    if (menucycle == 1)
    {
        if (key == key_next || key == key_prev)
        {
            p = curmenu;
            if (key == key_next)
            {
                ++curmenu;
                if (curmenu > 1)
                {
                    curmenu = 0;
                }
            }
            if (key == key_prev)
            {
                --curmenu;
                if (curmenu < 0)
                {
                    curmenu = 1;
                }
            }
            if (p != curmenu)
            {
                screenupdate = -1;
                update_screen();
                if (curmenu == 0)
                {
                    snd(102);
                    return turn_result_t::show_spell_list;
                }
                if (curmenu == 1)
                {
                    snd(101);
                    return turn_result_t::show_skill_list;
                }
            }
        }
    }
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2008_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2008_internal;
        }
    }
    if (key == key_cancel)
    {
        update_screen();
        return turn_result_t::pc_turn_user_error;
    }
    goto label_2009_internal;
}



void sort_list_by_column1()
{
    if (listmax < 1)
    {
        return;
    }
    while (1)
    {
        p = 0;
        for (int cnt = 0, cnt_end = (listmax - 1); cnt < cnt_end; ++cnt)
        {
            if (list(1, cnt) > list(1, cnt + 1))
            {
                p(0) = list(0, cnt);
                p(1) = list(1, cnt);
                list(0, cnt) = list(0, cnt + 1);
                list(1, cnt) = list(1, cnt + 1);
                list(0, cnt + 1) = p;
                list(1, cnt + 1) = p(1);
                p = 1;
            }
        }
        if (p == 0)
        {
            break;
        }
    }
    return;
}



void label_2057()
{
    if (listmax < 1)
    {
        return;
    }
    while (1)
    {
        p = 0;
        for (int cnt = 0, cnt_end = (listmax - 1); cnt < cnt_end; ++cnt)
        {
            if (list(1, cnt) > list(1, cnt + 1))
            {
                p(0) = list(0, cnt);
                p(1) = list(1, cnt);
                list(0, cnt) = list(0, cnt + 1);
                list(1, cnt) = list(1, cnt + 1);
                list(0, cnt + 1) = p;
                list(1, cnt + 1) = p(1);
                s(0) = listn(0, cnt);
                s(1) = listn(1, cnt);
                listn(0, cnt) = listn(0, cnt + 1);
                listn(1, cnt) = listn(1, cnt + 1);
                listn(0, cnt + 1) = s;
                listn(1, cnt + 1) = s(1);
                p = 1;
            }
        }
        if (p == 0)
        {
            break;
        }
    }
    return;
}



void savecycle()
{
    if (menucycle == 1)
    {
        menucycle = 0;
        if (invally == 0 && invcontainer == 0)
        {
            lastctrl = invctrl;
        }
    }
    return;
}



void show_item_description()
{
    int inhmax = 0;
    if (ci < 0)
    {
        dialog(
            u8"暫定エラー回避処置です。お手数ですが、どの持ち物メニュー(例えば飲む、振る、食べるなど）から調査(xキー)を押したか報告お願いします。"s);
        return;
    }
    snd(26);
    page_save();
    page = 0;
    pagesize = 15;
    listmax = 0;
    p = 0;
    s = "";
    reftype = the_item_db[inv[ci].id]->category;
    getinheritance(ci, inhlist, inhmax);
    dbid = inv[ci].id;
    access_item_db(2);
    access_item_db(17);
    if (inv[ci].identification_state
        == identification_state_t::completely_identified)
    {
        std::string buf = trim_item_description(description(3), true);
        if (buf != ""s)
        {
            list(0, p) = 7;
            listn(0, p) = buf;
            ++p;
        }
    }
    if (inv[ci].identification_state
        >= identification_state_t::almost_identified)
    {
        if (inv[ci].material != 0)
        {
            list(0, p) = 7;
            listn(0, p) = lang(
                u8"それは"s
                    + i18n::_(
                          u8"item_material",
                          std::to_string(inv[ci].material),
                          u8"name")
                    + u8"で作られている"s,
                u8"It is made of "s
                    + i18n::_(
                          u8"item_material",
                          std::to_string(inv[ci].material),
                          u8"name")
                    + u8"."s);
            ++p;
        }
        if (inv[ci].material == 8)
        {
            list(0, p) = 7;
            listn(0, p) = lang(
                u8"それは装備している間、エーテルの病の進行を早める"s,
                u8"It speeds up the ether disease while equipping."s);
            ++p;
        }
        if (ibit(1, ci))
        {
            list(0, p) = 7;
            listn(0, p) =
                lang(u8"それは酸では傷つかない"s, u8"It is acidproof."s);
            ++p;
        }
        if (ibit(2, ci))
        {
            list(0, p) = 7;
            listn(0, p) =
                lang(u8"それは炎では燃えない"s, u8"It is fireproof."s);
            ++p;
        }
        if (ibit(5, ci))
        {
            list(0, p) = 7;
            listn(0, p) = lang(u8"それは貴重な品だ"s, u8"It is precious."s);
            ++p;
        }
        if (ibit(8, ci))
        {
            list(0, p) = 7;
            listn(0, p) = lang(
                u8"それはエヘカトルの祝福を受けている"s,
                u8"It is blessed by Ehekatl."s);
            ++p;
        }
        if (ibit(9, ci))
        {
            list(0, p) = 7;
            listn(0, p) = lang(u8"それは盗品だ"s, u8"It is a stolen item."s);
            ++p;
        }
        if (ibit(10, ci))
        {
            list(0, p) = 7;
            listn(0, p) = lang(u8"それは生きている"s, u8"It is alive."s)
                + u8" [Lv:"s + inv[ci].param1 + u8" Exp:"s
                + clamp(inv[ci].param2 * 100 / calcexpalive(inv[ci].param1),
                        0,
                        100)
                + u8"%]"s;
            ++p;
        }
        if (ibit(16, ci))
        {
            list(0, p) = 7;
            listn(0, p) = lang(
                u8"それはショウルームでのみ使用できる。"s,
                u8"It can be only used in a show room."s);
            ++p;
        }
        if (ibit(17, ci))
        {
            list(0, p) = 7;
            listn(0, p) = lang(
                u8"それは心温まる手作り品だ"s, u8"It is a hand-made item."s);
            ++p;
        }
        if (inv[ci].dice_x != 0)
        {
            const auto pierce = calc_rate_to_pierce(inv[ci].id);
            list(0, p) = 5;
            listn(0, p) = lang(
                              u8"それは武器として扱うことができる"s,
                              u8"It can be wielded as a weapon."s)
                + u8" ("s + inv[ci].dice_x + u8"d"s + inv[ci].dice_y
                + lang(u8" 貫通 "s, u8" Pierce "s) + pierce + u8"%)"s;
            ++p;
            if (reftype == 10000)
            {
                if (inv[ci].weight <= 1500)
                {
                    list(0, p) = 5;
                    listn(0, p) = lang(
                        u8"それは片手でも扱いやすい"s,
                        u8"It is a light weapon."s);
                    ++p;
                }
                if (inv[ci].weight >= 4000)
                {
                    list(0, p) = 5;
                    listn(0, p) = lang(
                        u8"それは両手持ちに適している"s,
                        u8"It is a heavy weapon."s);
                    ++p;
                }
            }
        }
        if (inv[ci].hit_bonus != 0 || inv[ci].damage_bonus != 0)
        {
            list(0, p) = 5;
            listn(0, p) = lang(
                u8"それは攻撃修正に"s + inv[ci].hit_bonus
                    + u8"を加え、ダメージを"s + inv[ci].damage_bonus
                    + u8"増加させる"s,
                u8"It modifies hit bonus by "s + inv[ci].hit_bonus
                    + u8" and damage bonus by "s + inv[ci].damage_bonus
                    + u8"."s);
            ++p;
        }
        if (inv[ci].pv != 0 || inv[ci].dv != 0)
        {
            list(0, p) = 6;
            listn(0, p) = lang(
                u8"それはDVを"s + inv[ci].dv + u8"上昇させ、PVを"s + inv[ci].pv
                    + u8"上昇させる"s,
                u8"It modifies DV by "s + inv[ci].dv + u8" and PV by "s
                    + inv[ci].pv + u8"."s);
            ++p;
        }
        if (inv[ci].id == 701)
        {
            int card_count{};
            for (int i = 0; i < 1000; ++i)
            {
                if (card(0, i))
                    ++card_count;
            }
            int npc_count{};
            for (const auto& discord : the_character_db)
            {
                (void)discord;
                ++npc_count;
            }
            const auto percentage = std::min(100 * card_count / npc_count, 100);
            list(0, p) = 7;
            listn(0, p) = lang(
                u8"集めたカード: "s + card_count + u8"/" + npc_count + u8"("
                    + percentage + u8"%)",
                u8"Collected cards: "s + card_count + u8"/" + npc_count + u8"("
                    + percentage + u8"%)");
            ++p;
        }
    }
    if (inv[ci].identification_state
        <= identification_state_t::partly_identified)
    {
        list(0, p) = 0;
        listn(0, p) = lang(
            u8"このアイテムに関する知識を得るには、鑑定する必要がある。"s,
            u8"You have to identify the item to gain knowledge."s);
        ++p;
    }
    if (inv[ci].identification_state
        == identification_state_t::completely_identified)
    {
        for (int cnt = 0; cnt < 15; ++cnt)
        {
            if (inv[ci].enchantments[cnt].id == 0)
            {
                break;
            }
            get_enchantment_description(
                inv[ci].enchantments[cnt].id,
                inv[ci].enchantments[cnt].power,
                reftype);
            listn(0, p) = lang(u8"それは"s, u8"It "s) + s;
            list(0, p) = rtval;
            list(1, p) = rtval(1);
            if (inhmax > 0)
            {
                int cnt2 = cnt;
                for (int cnt = 0, cnt_end = (inhmax); cnt < cnt_end; ++cnt)
                {
                    if (cnt2 == inhlist(cnt))
                    {
                        list(0, p) += 10000;
                        break;
                    }
                }
            }
            ++p;
        }
        if (ibit(15, ci))
        {
            list(0, p) = 4;
            listn(0, p) = lang(u8"相手は死ぬ"s, u8"The enemy dies."s);
            ++p;
        }
        if (jp)
        {
            for (int cnt = 0; cnt < 3; ++cnt)
            {
                if (description(cnt) == ""s)
                {
                    continue;
                }
                list(0, p) = 0;
                listn(0, p) = "";
                ++p;
                std::string buf =
                    trim_item_description(description(cnt), false);
                notesel(buf);
                for (int cnt = 0, cnt_end = (noteinfo()); cnt < cnt_end; ++cnt)
                {
                    noteget(q, cnt);
                    constexpr size_t max_width = 66;
                    if (strlen_u(q) > max_width)
                    {
                        p(2) = 0;
                        for (size_t i = 0; i < strlen_u(q) / max_width + 1; ++i)
                        {
                            auto one_line = strutil::take_by_width(
                                q(0).substr(p(2)), max_width);
                            p(1) = one_line.size();
                            if (strutil::starts_with(q, u8"。", p(1) + p(2)))
                            {
                                one_line += u8"。";
                                p(1) += std::strlen(u8"。");
                            }
                            if (strutil::starts_with(q, u8"、", p(1) + p(2)))
                            {
                                one_line += u8"、";
                                p(1) += std::strlen(u8"、");
                            }
                            if (strmid(q, p(2), p(1)) == ""s)
                            {
                                break;
                            }
                            list(0, p) = -1;
                            listn(0, p) = one_line;
                            ++p;
                            p(2) += p(1);
                        }
                    }
                    else
                    {
                        list(0, p) = 0;
                        listn(0, p) = q;
                        if (cnt == noteinfo() - 1)
                        {
                            list(0, p) = -2;
                        }
                        ++p;
                    }
                }
            }
        }
    }
    if (p == 0)
    {
        list(0, p) = 0;
        listn(0, p) = lang(
            u8"特に情報はない"s,
            u8"There is no information about this object."s);
        ++p;
    }
    listmax = p;
    pagemax = (listmax - 1) / pagesize;
    if (dump_return == 1)
    {
        dump_return = 0;
        return;
    }
    windowshadow = 1;
label_2069_internal:
    key_list = key_enter;
    keyrange = 0;
    cs_bk = -1;
    pagemax = (listmax - 1) / pagesize;
    if (page < 0)
    {
        page = pagemax;
    }
    else if (page > pagemax)
    {
        page = 0;
    }
label_2070_internal:
    s(0) = lang(u8"アイテムの知識"s, u8"Known Information"s);
    s(1) = strhint4 + strhint3;
    display_window((windoww - 600) / 2 + inf_screenx, winposy(408), 600, 408);
    display_topic(itemname(ci), wx + 28, wy + 34);
    for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
    {
        p = pagesize * page + cnt;
        if (p >= listmax)
        {
            break;
        }
        font(14 - en * 2);
        color(0, 0, 0);
        pos(wx + 68, wy + 68 + cnt * 18);
        if (list(0, p) % 10000 == 1)
        {
            color(0, 100, 0);
        }
        if (list(0, p) % 10000 == 2)
        {
            color(0, 0, 100);
        }
        if (list(0, p) % 10000 == 3)
        {
            color(80, 100, 0);
        }
        if (list(0, p) % 10000 == 4)
        {
            color(80, 50, 0);
        }
        if (list(0, p) % 10000 == 9)
        {
            color(180, 0, 0);
        }
        if (list(0, p) % 10000 == 8)
        {
            color(0, 100, 100);
        }
        if (list(0, p) == -1)
        {
            font(13 - en * 2);
        }
        if (list(0, p) == -2)
        {
            font(13 - en * 2, snail::font_t::style_t::italic);
            pos(wx + ww - strlen_u(listn(0, p)) * 6 - 80, wy + 68 + cnt * 18);
        }
        mes(listn(0, p));
        color(0, 0, 0);
        if (list(0, p) > 0)
        {
            pos(wx + 40, wy + 61 + cnt * 18);
            gcopy(3, 72 + list(0, p) % 10000 * 24, 336, 24, 24);
        }
        if (list(0, p) > 10000)
        {
            pos(wx + 15, wy + 63 + cnt * 18);
            gcopy(3, 384, 360, 24, 24);
        }
    }
    redraw();
    await(config::instance().wait1);
    key_check();
    cursor_check();
    if (key == key_pageup)
    {
        if (pagemax != 0)
        {
            snd(1);
            ++page;
            goto label_2069_internal;
        }
    }
    if (key == key_pagedown)
    {
        if (pagemax != 0)
        {
            snd(1);
            --page;
            goto label_2069_internal;
        }
    }
    if (key == key_cancel || key == key_enter)
    {
        returnfromidentify = 1;
        return;
    }
    goto label_2070_internal;
}



int label_2072()
{
    if (cdata[cdata[cc].enemy_id].state != 1)
    {
        cdata[cc].enemy_id = 0;
    }
    else if (is_in_fov(cdata[cc].enemy_id) == 0)
    {
        cdata[cc].enemy_id = 0;
    }
    if (cdata[cc].enemy_id == 0)
    {
        label_2076();
        if (listmax != 0)
        {
            f = 0;
            if (cc == 0 || cdata[cc].relationship >= 0)
            {
                p(0) = -3;
                p(1) = -1;
                p(2) = 0;
            }
            else
            {
                p(0) = 10;
                p(1) = 0;
                p(2) = 0;
            }
            for (int cnt = 0; cnt < 3; ++cnt)
            {
                int cnt2 = cnt;
                for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
                {
                    if (cdata[list(0, cnt)].relationship <= p(cnt2))
                    {
                        cdata[cc].enemy_id = list(0, cnt);
                        f = 1;
                        break;
                    }
                }
                if (f == 1)
                {
                    break;
                }
            }
        }
    }
    if (cdata[cc].enemy_id == 0 || cdata[cc].blind != 0)
    {
        if (cc == 0)
        {
            txt(lang(
                u8"ターゲットが見当たらない。"s, u8"You find no target."s));
            update_screen();
        }
        return 0;
    }
    if (cdata[cc].enemy_id < 0)
    {
        cdata[cc].enemy_id = 0;
    }
    return 1;
}



int prompt_really_attack()
{
    s = txttargetlevel(cc, tc);
    txt(s);
    txt(lang(
        u8"本当に"s + name(tc) + u8"を攻撃する？ "s,
        u8"Really attack "s + name(tc) + u8"? "s));
    ELONA_YES_NO_PROMPT();
    rtval = show_prompt(promptx, prompty, 160);
    if (rtval == 0)
    {
        update_screen();
        return 1;
    }
    else
    {
        update_screen();
        return 0;
    }
}



int can_do_ranged_attack()
{
    cw = -1;
    ammo = -1;
    for (int cnt = 0; cnt < 30; ++cnt)
    {
        body = 100 + cnt;
        if (cdata_body_part(cc, cnt) % 10000 == 0)
        {
            continue;
        }
        if (cdata_body_part(cc, cnt) / 10000 == 10)
        {
            cw = cdata_body_part(cc, cnt) % 10000 - 1;
        }
        if (cdata_body_part(cc, cnt) / 10000 == 11)
        {
            ammo = cdata_body_part(cc, cnt) % 10000 - 1;
        }
    }
    if (cw == -1)
    {
        cw = 0;
        return -1;
    }
    if (ammo == -1)
    {
        if (inv[cw].skill != 111)
        {
            cw = 0;
            return -2;
        }
    }
    if (ammo != -1)
    {
        if (inv[cw].skill != inv[ammo].skill)
        {
            return -3;
        }
    }
    attackskill = inv[cw].skill;
    return 1;
}



void label_2076()
{
    listmax = 0;
    for (int cnt = 0; cnt < 2; ++cnt)
    {
        for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
        {
            if (cdata[cnt].state != 1)
            {
                continue;
            }
            if (is_in_fov(cnt) == 0)
            {
                continue;
            }
            if (cc == 0 || cdata[cc].relationship >= 0)
            {
                if (cdata[cnt].relationship == 10)
                {
                    if (cnt == 0)
                    {
                        continue;
                    }
                }
            }
            x = cdata[cnt].position.x;
            y = cdata[cnt].position.y;
            if (x == cdata[cc].position.x && y == cdata[cc].position.y)
            {
                continue;
            }
            if (fov_los(cdata[cc].position.x, cdata[cc].position.y, x, y) == 0)
            {
                continue;
            }
            if (cdata[cnt].is_invisible() == 1)
            {
                if (cdata[cc].can_see_invisible() == 0)
                {
                    if (cdata[cnt].wet == 0)
                    {
                        continue;
                    }
                }
            }
            list(0, listmax) = cnt;
            list(1, listmax) = x;
            list(2, listmax) = y;
            ++listmax;
            if (listmax >= 499)
            {
                break;
            }
        }
        if (listmax != 0)
        {
            break;
        }
    }
    while (1)
    {
        if (listmax < 2)
        {
            break;
        }
        i = 1;
        for (int cnt = 0, cnt_end = (listmax - 1); cnt < cnt_end; ++cnt)
        {
            p = dist(
                cdata[cc].position.x,
                cdata[cc].position.y,
                list(1, cnt),
                list(2, cnt));
            p(1) = dist(
                cdata[cc].position.x,
                cdata[cc].position.y,
                list(1, cnt + 1),
                list(2, cnt + 1));
            if (p(1) < p)
            {
                p(0) = list(0, cnt);
                p(1) = list(1, cnt);
                p(2) = list(2, cnt);
                list(0, cnt) = list(0, cnt + 1);
                list(1, cnt) = list(1, cnt + 1);
                list(2, cnt) = list(2, cnt + 1);
                list(0, cnt + 1) = p;
                list(1, cnt + 1) = p(1);
                list(2, cnt + 1) = p(2);
                i = 0;
                break;
            }
        }
        if (i == 1)
        {
            break;
        }
    }
    return;
}



void dump_player_info()
{
    cc = 0;
    access_class_info(2, cdatan(3, 0));
    buff = "";
    notesel(buff);
    noteadd(latest_version.long_string());
    noteadd(
        u8"キャラクター情報 "s + gdata_year + u8"年"s + gdata_month + u8"月"s
        + gdata_day + u8"日 "s + gdata_hour + u8"時"s + gdata_minute + u8"分  "s
        + mdatan(0));
    noteadd(""s);
    noteadd(
        u8"  "s + fixtxt((""s + cdatan(1, 0) + cdatan(0, 0)), 34)
        + i18n::_(u8"ui", u8"sex", u8"_"s + cdata[0].sex) + u8" "s + calcage(0)
        + u8"歳"s + u8"  "s + cdata[0].height + u8"cm"s + u8" "s
        + cdata[0].weight + u8"kg"s);
    noteadd(""s);
    noteadd(
        fixtxt(
            u8"種族       : "s + i18n::_(u8"race", cdatan(2, 0), u8"name"), 30)
        + fixtxt(
              (u8"信仰      : "s + i18n::_(u8"god", cdata[0].god_id, u8"name")),
              32));
    noteadd(
        fixtxt(u8"職業       : "s + classname, 30)
        + fixtxt((u8"所属      : "s + guildname()), 32));
    noteadd(
        fixtxt(u8"レベル     : "s + cdata[0].level, 30)
        + fixtxt((u8"経過日数  : "s + gdata_play_days), 32));
    noteadd(
        fixtxt(u8"残りBP     : "s + cdata[0].skill_bonus, 30)
        + fixtxt((u8"経過ターン: "s + gdata_play_turns), 32));
    noteadd(
        fixtxt(u8"金貨       : "s + cdata[0].gold, 30)
        + fixtxt((u8"殺害数    : "s + gdata_kill_count), 32));
    noteadd(
        fixtxt(u8"プラチナ   : "s + cdata[0].platinum_coin, 30)
        + fixtxt(
              (u8"最深到達  : "s + gdata_deepest_dungeon_level + u8"階相当"s),
              32));
    noteadd(fixtxt(
        u8"プレイ時間 : "s
            + cnvplaytime(
                  (gdata_play_time + timeGetTime() / 1000 - time_begin)),
        30));
    noteadd(""s);
    s(1) = u8"生命力    : "s + sdata(2, 0) + u8"("s
        + sdata.get(2, 0).original_level + u8")"s;
    s(2) = u8"マナ      : "s + sdata(3, 0) + u8"("s
        + sdata.get(3, 0).original_level + u8")"s;
    s(3) = u8"狂気度    : "s + cdata[0].insanity;
    s(4) = u8"速度      : "s + cdata[0].current_speed;
    s(5) = u8"名声度    : "s + cdata[0].fame;
    s(6) = u8"カルマ    : "s + cdata[0].karma;
    s(7) = u8"DV        : "s + cdata[0].dv;
    s(8) = u8"PV        : "s + cdata[0].pv;
    s(9) = "";
    for (int cnt = 0; cnt < 8; ++cnt)
    {
        s = "";
        p = sdata.get(10 + cnt, 0).potential;
        if (p >= 200)
        {
            s += u8"superb"s;
        }
        else if (p >= 150)
        {
            s += u8"great"s;
        }
        else if (p >= 100)
        {
            s += u8"good"s;
        }
        else if (p >= 50)
        {
            s += u8"bad"s;
        }
        else
        {
            s += u8"hopeless"s;
        }
        s = fixtxt(s, 15);
        s = fixtxt(
                i18n::_(u8"ui", u8"attribute", u8"_"s + cnt) + u8"    : "s
                    + sdata((10 + cnt), 0) + u8"("s
                    + sdata.get(10 + cnt, 0).original_level + u8")"s,
                24)
            + s;
        noteadd(s + s((1 + cnt)));
    }
    noteadd(""s);
    label_2047(1);
    tc = 0;
    attackskill = 106;
    int evade = calc_evasion(tc);
    prot = calcattackdmg(2);
    noteadd(u8"回避    : "s + evade + u8"%"s);
    noteadd(
        u8"軽減    : "s + (100 - 10000 / (prot + 100)) + u8"% + "s + protdice1
        + u8"d"s + protdice2);
    noteadd(""s);
    noteadd(
        u8"------------------------------ 装備品 合計重量"s
        + cnvweight(cdata[cc].sum_of_equipment_weight) + u8" "s
        + cnveqweight(cc));
    noteadd(""s);
    listmax = 0;
    for (int cnt = 100; cnt < 130; ++cnt)
    {
        if (cdata_body_part(cc, cnt) != 0)
        {
            if (trait(206) != 0)
            {
                if (cdata_body_part(cc, cnt) / 10000 == 2)
                {
                    continue;
                }
            }
            if (trait(203) != 0)
            {
                if (cdata_body_part(cc, cnt) / 10000 == 9)
                {
                    continue;
                }
            }
            if (trait(205) != 0)
            {
                if (cdata_body_part(cc, cnt) / 10000 == 3)
                {
                    continue;
                }
            }
            p(1) = cdata_body_part(cc, cnt);
            q(0) = u8"装備なし"s;
            q(1) = "";
            if (p(1) % 10000 != 0)
            {
                p(1) = p(1) % 10000 - 1;
                q(0) = itemname(p(1));
                q(1) = cnvweight(inv[p(1)].weight);
                ci = p(1);
                dump_return = 1;
                show_item_description();
            }
            else
            {
                listmax = 0;
            }
            s = i18n::_(
                u8"ui",
                u8"body_part",
                u8"_"s + (cdata_body_part(cc, cnt) / 10000));
            if (strmid(s, 0, 2) == u8"　"s)
            {
                s = strmid(s, 2, 2);
            }
            noteadd(s + u8":"s);
            noteadd(q + u8" "s + q(1));
            for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
            {
                noteadd(listn(0, cnt));
            }
            noteadd(""s);
        }
    }
    noteadd(u8"------------------------------ 特徴"s);
    noteadd(""s);
    dump_return = 1;
    tc = 0;
    label_1969();
    for (int cnt = 0, cnt_end = (listmax); cnt < cnt_end; ++cnt)
    {
        if (list(0, cnt) < 0)
        {
            continue;
        }
        if (list(1, cnt) < 10000)
        {
            continue;
        }
        noteadd(listn(0, cnt));
    }
    noteadd(""s);
    noteadd(u8"------------------------------ 仲間"s);
    noteadd(""s);
    for (int cnt = 0; cnt < 16; ++cnt)
    {
        if (cdata[cnt].state == 0)
        {
            continue;
        }
        if (cnt == 0)
        {
            continue;
        }
        access_class_info(2, cdatan(3, cnt));
        noteadd(
            cdatan(0, cnt) + u8" "s
            + i18n::_(u8"race", cdatan(2, cnt), u8"name") + u8"の"s + classname
            + u8" "s + i18n::_(u8"ui", u8"sex", u8"_"s + cdata[cnt].sex)
            + u8" "s + calcage(cnt) + u8"歳"s + u8"  "s + cdata[cnt].height
            + u8"cm"s + u8" "s + cdata[cnt].weight + u8"kg"s);
        s = u8"レベル "s + cdata[cnt].level + u8" "s;
        if (cdata[cnt].is_married() == 1)
        {
            s += u8"婚約済み "s;
        }
        noteadd(s);
        noteadd(""s);
    }
    noteadd(u8"------------------------------ 称号"s);
    noteadd(""s);
    for (int cnt = 0; cnt < 9; ++cnt)
    {
        if (gdata(120 + cnt) < 10000)
        {
            noteadd(
                ""s + ranktitle(cnt) + u8" Rank."s + gdata((120 + cnt)) / 100);
            s = u8"給料: 約 "s + calcincome(cnt) + u8" gold  "s + u8"ノルマ: "s;
            gold += calcincome(cnt);
            if (cnt == 3 || cnt == 4 || cnt == 5 || cnt == 8)
            {
                s += u8"なし"s;
            }
            else
            {
                s += ""s + gdata((140 + cnt)) + u8"日以内"s;
            }
            noteadd(s);
            noteadd(""s);
        }
    }
    std::ofstream out{
        (filesystem::dir::save() / (playerid + u8".txt")).native(),
        std::ios::binary};
    out << s(0) << std::endl;
}



void remove_card_and_figures()
{
    for (const auto& cnt : items(-1))
    {
        if (inv[cnt].id == 504 || inv[cnt].id == 503)
        {
            inv[cnt].number = 0;
        }
    }
    return;
}



void label_2088()
{
    for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
    {
        if (cdata[cnt].state == 1)
        {
            if (cdata[cnt].position.x < 0 || cdata[cnt].position.x >= mdata(0)
                || cdata[cnt].position.y < 0
                || cdata[cnt].position.y >= mdata(1))
            {
                cdata[cnt].position.x = 0;
                cdata[cnt].position.y = 0;
            }
        }
    }
    if (mdata(6) != 5)
    {
        if (mdata(18) != 0)
        {
            mdata(18) = 0;
        }
    }
    if (gdata_current_map == 7)
    {
        adata(17, gdata_current_map) = 0;
        adata(10, gdata_current_map) = 10;
        adata(12, gdata_current_map) = 1;
        mdata(8) = 1;
        event_add(17);
        calccosthire();
    }
    return;
}



void migrate_save_data()
{
    int p1 = 0;
    int p3 = 0;
    if (gdata_version != 1220)
    {
        dialog(lang(
            u8"Ver."s + gdata_version
                + u8"のセーブデータをアップデートします。"s,
            u8"Updating your save data from Ver."s + gdata_version
                + u8" now."s));
    }
    if (gdata_rights_to_succeed_to < 0)
    {
        gdata_rights_to_succeed_to = 1;
    }
    if (gdata_departure_date == 0)
    {
        gdata_departure_date = gdata_hour + gdata_day * 24
            + gdata_month * 24 * 30 + gdata_year * 24 * 30 * 12;
    }
    for (int i = 0; i < 20; ++i)
    {
        if (gdata(120 + i) == 0)
        {
            gdata(120 + i) = 10000;
        }
    }
    itemmemory(2, 289) = 0;
    itemmemory(2, 732) = 0;
    if (gdata_version < 1170)
    {
        if (adata(0, gdata_current_map) != 3)
        {
            dialog(lang(
                u8"アップデートを行うには、街中でセーブしたセーブデータが必要です。"s,
                u8"To update your game, please save your game in a town in the previous version then retry."s));
            finish_elona();
            return;
        }
        dialog(lang(
            u8"次のプロセスの完了までには、しばらく時間がかかることがあります。"s,
            u8"The next updating process may take a while to complete."s));
        memcpy(adata, 0, 300, adata, 0, 100, 8000);
        memcpy(adata, 0, 450, adata, 0, 150, 8000);
        for (int cnt = 100; cnt < 200; ++cnt)
        {
            p = cnt;
            for (int cnt = 0; cnt < 40; ++cnt)
            {
                adata(cnt, p) = 0;
            }
            for (const auto& entry : filesystem::dir_entries{
                     filesystem::dir::tmp(),
                     filesystem::dir_entries::type::file,
                     std::regex{u8R"(.*_)"s + std::to_string(p)
                                + u8R"(_.*\..*)"}})
            {
                const auto file = entry.path().filename();
                p1 = instr(filesystem::to_utf8_path(file), 0, u8"_"s);
                p2 = instr(filesystem::to_utf8_path(file), p1 + 1, u8"_"s);
                if (p >= 150)
                {
                    p3 = p - 150 + 450;
                }
                else
                {
                    p3 = p - 100 + 300;
                }
                const auto file_cnv = filesystem::dir::tmp()
                    / ((strmid(filesystem::to_utf8_path(file), 0, (p1 + 1))
                        + p3)
                       + strmid(
                             filesystem::to_utf8_path(file),
                             (p1 + p2 + 1),
                             20));
                const auto file_ = filesystem::dir::tmp() / file;
                bcopy(file_, file_cnv);
                fileadd(file_cnv);
                elona_delete(file_);
                fileadd(file_, 1);
            }
        }
        for (int cnt = 0; cnt < 500; ++cnt)
        {
            if (adata(16, cnt) != 0)
            {
                if (adata(30, cnt) == 0)
                {
                    adata(30, cnt) = 4;
                }
            }
        }
        gdata(850) = 4;
    }
    if (gdata_version != 1220)
    {
        gdata(79) = 1;
    }
    chara_delete(56);
    if (gdata_version > 1220)
    {
        dialog(u8"invalid version"s);
        finish_elona();
        return;
    }
    gdata_version = 1220;
    cdata[0].has_own_sprite() = true;
    initialize_recipememory();
    return;
}



void label_2090()
{
    ++gdata(184);
    DIM3(qdata, 20, 500);
    SDIM3(qname, 40, 500);
    gdata_number_of_existing_quests = 0;
    initialize_adata();
    return;
}



void get_inheritance()
{
    ctrl_file(file_operation2_t::_4, u8"shop3.s2");
    p = 0;
    i = 0;
    for (int cnt = 0; cnt < 600; ++cnt)
    {
        if (cnt >= 10 && cnt < 20)
        {
            p += sdata.get(cnt, 56).original_level;
        }
        if (cnt >= 100 && cnt < 400)
        {
            i += sdata.get(cnt, 56).original_level;
        }
    }
    p = (p - 250) / 7;
    if (p < 5)
    {
        p = 5;
    }
    else if (p > 50)
    {
        p = 50;
    }
    i = (p - 250) / 8;
    if (i < 5)
    {
        i = 5;
    }
    else if (i > 40)
    {
        i = 40;
    }
    cdata[0].gold += clamp(cdata[56].gold / 100, 1000, 100000);
    cdata[0].platinum_coin += p;
    cdata[0].skill_bonus += i;
    for (int cnt = 0; cnt < 400; ++cnt)
    {
        mat(cnt) = mat(cnt) / 3;
    }
    return;
}



void load_gene_files()
{
    ctrl_file(file_operation_t::_15);
    DIM2(spell, 200);
    DIM2(spact, 500);
    for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
    {
        cdata[cnt].state = 0;
    }
    sdata.copy(56, 0);
    sdata.clear(0);
    cdata(56) = cdata(0);
    cdata(0).clear();
    for (const auto& cnt : items(-1))
    {
        inv[cnt].number = 0;
    }
    for (const auto& cnt : items(0))
    {
        if (inv[cnt].number == 0)
        {
            continue;
        }
        if (inv[cnt].id == 717)
        {
            lomiaseaster = 1;
        }
        if (inv[cnt].id == 511
            || the_item_db[inv[cnt].id]->subcategory == 53100)
        {
            continue;
        }
        if (inv[cnt].id == 578)
        {
            continue;
        }
        if (inv[cnt].quality == 6)
        {
            continue;
        }
        if (ibit(5, cnt))
        {
            continue;
        }
        if (the_item_db[inv[cnt].id]->category == 25000)
        {
            inv[cnt].count = -1;
        }
        inv[cnt].body_part = 0;
        item_copy(cnt, inv_getfreeid(-1));
    }
    for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
    {
        chara_delete(cnt);
    }
    gdata_play_time = genetemp(805);
    return;
}



void save_gene()
{
    ctrl_file(file_operation_t::_14);
}



std::string getnpctxt(const std::string& prm_1068, const std::string& prm_1069)
{
    int p_at_m189 = 0;
    p_at_m189 = instr(txtbuff, 0, prm_1068);
    if (p_at_m189 == -1)
    {
        return prm_1069;
    }
    p_at_m189 += instr(txtbuff, p_at_m189, u8"\""s);
    if (p_at_m189 == -1)
    {
        return prm_1069;
    }
    return strmid(
        txtbuff,
        p_at_m189 + 1,
        clamp(instr(txtbuff, p_at_m189 + 1, u8"\""s), 0, 70));
}



void create_cnpc()
{
    elona_vector1<std::string> unres;
    if (initunid)
    {
        cdata[rc].cnpc_id = initunid - 1;
        initunid = 0;
    }
    cun = cdata[rc].cnpc_id;
    cdata[rc].id = 343;
    if (initlv != 0)
    {
        cdata[rc].level = initlv;
    }
    else
    {
        cdata[rc].level = clamp(userdata(2, cun), 1, 100);
    }
    cdata[rc].portrait = -1;
    creaturepack = 0;
    cdata[rc].ai_act_sub_freq = unaiactsubfreq(cun);

    cdata[rc].normal_actions.clear();
    for (int cnt = 0; cnt < 5; ++cnt)
    {
        int action = userdata(15 + cnt, cun);
        if (action == 0)
            break;
        cdata[rc].normal_actions.push_back(action);
    }
    cdata[rc].special_actions.clear();
    for (int cnt = 0; cnt < 5; ++cnt)
    {
        int action = userdata(20 + cnt, cun);
        if (action == 0)
            break;
        cdata[rc].special_actions.push_back(action);
    }

    if (userdatan(5, cun) != ""s)
    {
        SDIM3(unres, 6, 32);
        csvstr2(unres, userdatan(5, cun));
        for (int cnt = 0;; ++cnt)
        {
            if (cnt > 15 || unres(cnt) == ""s)
            {
                break;
            }
            sdata(elona::stoi(unres(cnt * 2)), rc) +=
                elona::stoi(unres((cnt * 2 + 1))) * 50;
        }
    }
    cdata[rc].can_talk = 1;
    cdatan(0, rc) = userdatan(1, cun);
    cdatan(5, rc) = userdatan(0, cun);
    cdata[rc].relationship = clamp(userdata(4, cun), -3, 10);
    cdata[rc].ai_calm = clamp(userdata(7, cun), 1, 4);
    cdata[rc].ai_move = clamp(userdata(8, cun), 0, 100);
    cdata[rc].ai_dist = clamp(userdata(9, cun), 1, 100);
    if (userdata(11, cun))
    {
        cdata[rc].element_of_unarmed_attack =
            clamp(userdata(11, cun), 5000000, 6400000);
    }
    cdata[rc].ai_heal = userdata(10, cun);
    access_race_info(3, userdatan(2, cun));
    access_class_info(3, userdatan(3, cun));
    cdata[rc].sex = userdata(3, cun);
    if (userdata(3, cun) != 0)
    {
        if (userdata(3, cun) != 1)
        {
            cdata[rc].sex = rnd(2);
        }
    }
    if (userdata(5, cun))
    {
        fixlv = clamp(userdata(5, cun), 0, 6);
    }
    cspecialeq = 0;
    cdata[rc].original_relationship = cdata[rc].relationship;
    fixaiact(rc);
    setunid(rc, cun);
    return;
}

int efstatusfix(int doomed, int cursed, int none, int blessed)
{
    switch (efstatus)
    {
    case curse_state_t::doomed: return doomed;
    case curse_state_t::cursed: return cursed;
    case curse_state_t::none: return none;
    case curse_state_t::blessed: return blessed;
    default: assert(0);
    }
}



void activate_trap()
{
label_21451_internal:
    if (config::instance().scroll)
    {
        if (cc == 0)
        {
            label_1438();
        }
    }
    map(cdata[cc].position.x, cdata[cc].position.y, 1) = 0;
    cdata[cc].position.x = cdata[cc].next_position.x;
    cdata[cc].position.y = cdata[cc].next_position.y;
    map(cdata[cc].next_position.x, cdata[cc].next_position.y, 1) = cc + 1;
    if (cc == 0)
    {
        if (gdata_mount != 0)
        {
            cdata[gdata_mount].position.x = cdata[0].position.x;
            cdata[gdata_mount].position.y = cdata[0].position.y;
        }
    }
    movx = cdata[cc].position.x;
    movy = cdata[cc].position.y;
    if (map(movx, movy, 6) != 0)
    {
        cell_featread(movx, movy);
        if (feat(1) == 14)
        {
            if (feat(2) == 7)
            {
                if ((cdata[cc].is_floating() == 1 && cdata[cc].gravity == 0)
                    || cdata[cc].is_immune_to_mine() == 1)
                {
                    return;
                }
                if (feat(3) != 0)
                {
                    if (cc > 16)
                    {
                        return;
                    }
                }
            }
            if (feat(0) != tile_trap && cc == 0)
            {
                int stat = try_to_reveal();
                if (stat == 1)
                {
                    refx = movx;
                    refy = movy;
                    discover_trap();
                    feat(0) = tile_trap;
                }
            }
            refdiff = 100 + gdata_current_dungeon_level * 3;
            if (feat(0) == tile_trap)
            {
                refdiff = refdiff / 3;
                if (cc == 0)
                {
                    if (sdata(175, cc) != 0)
                    {
                        int stat = try_to_disarm_trap();
                        if (stat == 1)
                        {
                            do_disarm_trap();
                            return;
                        }
                        else
                        {
                            txt(lang(
                                u8"解除に失敗した。"s,
                                u8"You fail to disarm the trap."s));
                        }
                    }
                }
            }
            int stat = can_evade_trap();
            if (stat == 1)
            {
                if (is_in_fov(cc))
                {
                    txt(lang(
                        name(cc) + u8"は罠を避けた。"s,
                        name(cc) + u8" evade"s + _s(cc) + u8" a trap."s));
                }
                if (cc == 0)
                {
                    refx = movx;
                    refy = movy;
                    discover_trap();
                }
            }
            else
            {
                if (cc == 0)
                {
                    refx = movx;
                    refy = movy;
                    discover_trap();
                    snd(70);
                }
                efsource = 5;
                if (is_in_fov(cc))
                {
                    txt(lang(
                        aln(cc) + u8"罠にかかった！"s,
                        name(cc) + u8" activate"s + _s(cc) + u8" a trap!"s));
                }
                if (feat(2) == 4)
                {
                    if (is_in_fov(cc))
                    {
                        txt(lang(
                            u8"墨が噴き出した。"s,
                            u8"Gallons of ink spreads."s));
                    }
                    dmgcon(cc, 4, 100 + gdata_current_dungeon_level * 2);
                }
                if (feat(2) == 6)
                {
                    if (is_in_fov(cc))
                    {
                        txt(lang(
                            u8"刺激的な匂いがただよう。"s,
                            u8"Stimulative gas spreads."s));
                    }
                    dmgcon(cc, 3, 100 + gdata_current_dungeon_level * 2);
                }
                if (feat(2) == 5)
                {
                    if (is_in_fov(cc))
                    {
                        txt(lang(
                            u8"臭い匂いがたちこめた。"s,
                            u8"Smelly gas spreads."s));
                    }
                    dmgcon(cc, 5, 100 + gdata_current_dungeon_level * 2);
                }
                if (feat(2) == 3)
                {
                    cell_featset(movx, movy, 0);
                    if (encfind(cc, 22) != -1)
                    {
                        if (is_in_fov(cc))
                        {
                            txt(lang(
                                u8"魔法の力がテレポートを防いだ。"s,
                                u8"Magical field prevents teleportation."s));
                        }
                        return;
                    }
                    for (int cnt = 0; cnt < 200; ++cnt)
                    {
                        cdata[cc].next_position.x = rnd(mdata(0) - 2) + 1;
                        cdata[cc].next_position.y = rnd(mdata(1) - 2) + 1;
                        cell_check(
                            cdata[cc].next_position.x,
                            cdata[cc].next_position.y);
                        if (cellaccess == 1)
                        {
                            if (is_in_fov(cc))
                            {
                                snd(72);
                                txt(lang(
                                    name(cc) + u8"は突然消えた。"s,
                                    u8"Suddenly, "s + name(cc) + u8" disappear"s
                                        + _s(cc) + u8"."s));
                            }
                            rowactend(cc);
                            update_screen();
                            break;
                        }
                    }
                    goto label_21451_internal;
                }
                if (feat(2) == 0)
                {
                    if (is_in_fov(cc))
                    {
                        txt(lang(
                            u8"槍が地面から飛び出した。"s,
                            u8"Several spears fly out from the ground."s));
                    }
                    if (cdata[cc].is_floating() == 1 && cdata[cc].gravity == 0)
                    {
                        if (is_in_fov(cc))
                        {
                            txt(lang(
                                u8"しかし"s + name(cc)
                                    + u8"には届かなかった。"s,
                                u8"But they don't reach "s + name(cc)
                                    + u8"."s));
                        }
                    }
                    else
                    {
                        dmghp(
                            cc, rnd(gdata_current_dungeon_level * 2 + 10), -1);
                    }
                }
                if (feat(2) == 1)
                {
                    if (is_in_fov(cc))
                    {
                        txt(lang(
                            u8"毒ガスが噴き出した。"s,
                            u8"Poisonous gas spreads."s));
                    }
                    dmgcon(cc, 1, 100 + gdata_current_dungeon_level * 2);
                }
                if (feat(2) == 2)
                {
                    if (is_in_fov(cc))
                    {
                        txt(lang(
                            u8"催眠ガスが噴き出した。"s,
                            u8"Sleeping gas spreads."s));
                    }
                    dmgcon(cc, 2, 100 + gdata_current_dungeon_level * 2);
                }
                if (feat(2) == 7)
                {
                    txtef(9);
                    txt(lang(u8" *チュドーン！* "s, u8"*kabooom*"s));
                    aniref = 0;
                    anix = movx;
                    aniy = movy;
                    play_animation(2);
                    cell_featset(movx, movy, 0);
                    dmghp(cc, 100 + rnd(200), -1);
                }
                efsource = 0;
            }
        }
    }
    return;
}



void proc_world_map_movement_event()
{
    if (cdata[cc].continuous_action_id == 0)
    {
        cdata[cc].continuous_action_id = 3;
        cdata[cc].continuous_action_turn = 20;
        if (gdata_weather == 3)
        {
            cdata[cc].continuous_action_turn =
                cdata[cc].continuous_action_turn * 13 / 10;
        }
        if (gdata_weather == 4)
        {
            cdata[cc].continuous_action_turn =
                cdata[cc].continuous_action_turn * 16 / 10;
        }
        if (gdata_weather == 2
            || chipm(0, map(cdata[cc].position.x, cdata[cc].position.y, 0))
                == 4)
        {
            cdata[cc].continuous_action_turn =
                cdata[cc].continuous_action_turn * 22 / 10;
        }
        if (gdata_weather == 1)
        {
            cdata[cc].continuous_action_turn =
                cdata[cc].continuous_action_turn * 5 / 10;
        }
        cdata[cc].continuous_action_turn = cdata[cc].continuous_action_turn
            * 100 / (100 + gdata_seven_league_boot_effect + sdata(182, 0));
        return;
    }
    if (cdata[0].nutrition <= 5000)
    {
        f = 0;
        for (const auto& cnt : items(cc))
        {
            if (inv[cnt].number == 0)
            {
                continue;
            }
            if (the_item_db[inv[cnt].id]->category == 91000)
            {
                f = 1;
                ci = cnt;
                break;
            }
        }
        if (f == 1)
        {
            if (is_in_fov(cc))
            {
                txt(lang(
                    npcn(cc) + itemname(ci, 1) + u8"を食べ終えた。"s,
                    name(cc) + u8" "s + have(cc) + u8" finished eating "s
                        + itemname(ci, 1) + u8"."s));
            }
            continuous_action_eating_finish();
        }
    }
    if (gdata_weather == 2
        || chipm(0, map(cdata[cc].position.x, cdata[cc].position.y, 0)) == 4)
    {
        if (gdata_protects_from_bad_weather == 0)
        {
            if (rnd(100) == 0)
            {
                if (cdata[0].is_floating() == 0 || cdata[0].gravity > 0)
                {
                    txtef(9);
                    if (jp)
                    {
                        txt(u8" *ずぶっ* "s,
                            u8" *ザシュ* "s,
                            u8" *ズボ* "s,
                            u8" *ズサッ* "s);
                    }
                    if (en)
                    {
                        txt(u8" *kisssh*"s,
                            u8"*thudd*"s,
                            u8"*siz*"s,
                            u8"*clump*"s,
                            u8"*skritch*"s);
                    }
                    cdata[cc].continuous_action_turn += 10;
                }
            }
            if (rnd(1000) == 0)
            {
                txtef(8);
                if (jp)
                {
                    txt(u8"積雪のせいで旅程が遅れている。"s,
                        u8"雪道を進むのは大変な苦労だ。"s,
                        u8"深い雪に脚をとられている。"s);
                }
                if (en)
                {
                    txt(u8"Snow delays your travel."s,
                        u8"You are caught in a snowdrift."s,
                        u8"It's hard to walk on a snowy road."s);
                }
                cdata[cc].continuous_action_turn += 50;
            }
        }
        if (cdata[0].nutrition <= 2000)
        {
            if (cdata[0].has_anorexia() == 0)
            {
                snd(18);
                txt(lang(
                    u8"空腹のあまり、あなたは積もっている雪を腹にかきこんだ。"s,
                    u8"You are too hungry. You chow down snow."s));
                cdata[cc].nutrition += 5000;
                label_2162();
                dmgcon(0, 7, 1000);
            }
        }
    }
    if (gdata_weather == 4)
    {
        if (gdata_protects_from_bad_weather == 0)
        {
            if (rnd(100) == 0)
            {
                if (cdata[0].is_floating() == 0 || cdata[0].gravity > 0)
                {
                    txtef(9);
                    if (jp)
                    {
                        txt(u8" *びしゃ* "s,
                            u8" *ザブッ* "s,
                            u8" *パシャッ* "s,
                            u8" *ざぶ* "s);
                    }
                    if (en)
                    {
                        txt(u8"*drip*"s,
                            u8"*sip*"s,
                            u8"*drizzle*"s,
                            u8"*splash*"s,
                            u8"*kissh*"s);
                    }
                    cdata[cc].continuous_action_turn += 5;
                }
            }
            if (cdata[0].confused == 0)
            {
                if (rnd(500) == 0)
                {
                    txtef(8);
                    if (jp)
                    {
                        txt(u8"雨が激しすぎてどこを歩いているかもわからない！"s,
                            u8"あまりにも視界が悪すぎる。"s,
                            u8"豪雨のせいで前が全く見えない。"s);
                    }
                    if (en)
                    {
                        txt(u8"It's raining heavily. You lose your way."s,
                            u8"You can't see a thing!"s);
                    }
                    cdata[0].confused = 10;
                }
            }
            else if (rnd(5) == 0)
            {
                cdata[0].confused = 10;
            }
        }
        cdata[0].blind = 3;
    }
    if (cdata[cc].continuous_action_turn > 0)
    {
        ++gdata_minute;
        return;
    }
    traveldone = 1;
    gdata_distance_between_town += 4;
    rowactend(cc);
    return;
}






int label_2174()
{
    if (efid == 646)
    {
        if (cdata[tc].is_sentenced_daeth() == 1)
        {
            if (cdata[cc].relationship == -3)
            {
                for (int cnt = 0; cnt < ELONA_MAX_PARTY_CHARACTERS; ++cnt)
                {
                    if (cdata[cnt].state == 1)
                    {
                        if (cdata[cnt].relationship == 10)
                        {
                            cdata[cc].enemy_id = cnt;
                            break;
                        }
                    }
                }
            }
            return 0;
        }
    }
    {
        int stat = query_magic_direction();
        if (stat == 0)
        {
            return 0;
        }
    }
    if (the_ability_db[efid]->sdataref3 / 1000 * 1000 != 3000
        && the_ability_db[efid]->sdataref3 / 1000 * 1000 != 10000)
    {
        if (cdata[cc].confused != 0 || cdata[cc].blind != 0)
        {
            if (rnd(5) == 0)
            {
                if (is_in_fov(cc))
                {
                    txt(lang(
                        name(cc) + u8"は頭を振った。"s,
                        name(cc) + u8" shake"s + _s(cc) + u8" "s + his(cc)
                            + u8" head."s));
                }
                return 1;
            }
        }
    }
    if (efid >= 600)
    {
        if (cc == 0)
        {
            if (cdata[0].sp < 50)
            {
                if (cdata[0].sp < rnd(75))
                {
                    txt(lang(
                        u8"疲労し過ぎて失敗した！"s,
                        u8"You are too exhausted!"s));
                    dmgsp(0, the_ability_db[efid]->cost / 2 + 1);
                    return 1;
                }
            }
            dmgsp(
                0,
                rnd(the_ability_db[efid]->cost / 2 + 1)
                    + the_ability_db[efid]->cost / 2 + 1);
            skillexp(the_ability_db[efid]->related_basic_attribute, cc, 25);
        }
    }
    efp = calcspellpower(efid, cc);
    if (noeffect == 1)
    {
        if (efid != 300)
        {
            txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
            return 1;
        }
    }
    {
        int stat = magic();
        if (stat == 0)
        {
            return 0;
        }
    }
    return 1;
}



int query_magic_direction()
{
    noeffect = 0;
    if (efid > 661)
    {
        tc = cc;
        return 1;
    }
    tg = the_ability_db[efid]->sdataref3 / 1000 * 1000;
    if (efsource == 1)
    {
        if (tg == 3000)
        {
            tg = 8000;
        }
    }
    if (the_ability_db[efid]->sdataref1 == 7)
    {
        if (cc == 0)
        {
            tc = 0;
            return 1;
        }
    }
    if (tg == 8000)
    {
        if (cc == 0)
        {
            tc = 0;
            txt(lang(u8"どの方向？"s, u8"Which direction?"s));
            update_screen();
            int stat = ask_direction();
            if (stat == 0)
            {
                return 0;
            }
            if (map(tlocx, tlocy, 1) == 0)
            {
                noeffect = 1;
                obvious = 0;
                return 1;
            }
            tc = map(tlocx, tlocy, 1) - 1;
        }
        else
        {
            if (dist(
                    cdata[tc].position.x,
                    cdata[tc].position.y,
                    cdata[cc].position.x,
                    cdata[cc].position.y)
                > the_ability_db[efid]->sdataref3 % 1000 + 1)
            {
                return 0;
            }
            tlocx = cdata[tc].position.x;
            tlocy = cdata[tc].position.y;
        }
        return 1;
    }
    if (tg == 7000 || (tg == 9000 && tgloc == 1 && cc == 0))
    {
        if (cc == 0)
        {
            if (tg == 9000)
            {
                if (int stat = fov_los(
                                   cdata[cc].position.x,
                                   cdata[cc].position.y,
                                   tglocx,
                                   tglocy)
                        == 0)
                {
                    if (stat == 0)
                    {
                        txt(lang(
                            u8"その場所は見えない。"s,
                            u8"You can't see the location."s));
                        update_screen();
                    }
                    obvious = 0;
                    return 0;
                }
                tlocx = tglocx;
                tlocy = tglocy;
            }
            else
            {
                int stat = target_position();
                if (stat != 1)
                {
                    if (stat == 0)
                    {
                        txt(lang(
                            u8"その場所は見えない。"s,
                            u8"You can't see the location."s));
                        update_screen();
                    }
                    obvious = 0;
                    return 0;
                }
            }
        }
        else
        {
            if (fov_los(
                    cdata[tc].position.x,
                    cdata[tc].position.y,
                    cdata[cc].position.x,
                    cdata[cc].position.y)
                == 0)
            {
                return 0;
            }
            tlocx = cdata[tc].position.x;
            tlocy = cdata[tc].position.y;
        }
        return 1;
    }
    if (tg == 3000 || tg == 10000)
    {
        if (cc != 0)
        {
            if (the_ability_db[efid]->sdataref1 == 3)
            {
                if (dist(
                        cdata[tc].position.x,
                        cdata[tc].position.y,
                        cdata[cc].position.x,
                        cdata[cc].position.y)
                    > the_ability_db[efid]->sdataref3 % 1000 + 1)
                {
                    return 0;
                }
                if (fov_los(
                        cdata[cc].position.x,
                        cdata[cc].position.y,
                        cdata[tc].position.x,
                        cdata[tc].position.y)
                    == 0)
                {
                    return 0;
                }
            }
        }
        tc = cc;
        tlocx = cdata[cc].position.x;
        tlocy = cdata[cc].position.y;
        return 1;
    }
    if (tg == 2000 || tg == 6000 || tg == 9000)
    {
        if (cc == 0)
        {
            int stat = label_2072();
            if (stat == 0)
            {
                obvious = 0;
                return 0;
            }
            tc = cdata[0].enemy_id;
            if (cdata[tc].relationship >= 0)
            {
                int stat = prompt_really_attack();
                if (stat == 0)
                {
                    obvious = 0;
                    return 0;
                }
            }
        }
        if (dist(
                cdata[tc].position.x,
                cdata[tc].position.y,
                cdata[cc].position.x,
                cdata[cc].position.y)
            > the_ability_db[efid]->sdataref3 % 1000 + 1)
        {
            if (cc == 0)
            {
                ++msgdup;
                txt(lang(u8"射程距離外だ。"s, u8"It's out of range."s));
                update_screen();
            }
            return 0;
        }
        if (fov_los(
                cdata[cc].position.x,
                cdata[cc].position.y,
                cdata[tc].position.x,
                cdata[tc].position.y)
            == 0)
        {
            return 0;
        }
        tlocx = cdata[tc].position.x;
        tlocy = cdata[tc].position.y;
        return 1;
    }
    if (tg == 5000)
    {
        if (cc == 0)
        {
            if (efsource == 3)
            {
                txt(lang(
                    u8"どの方向に唱える？ "s,
                    u8"Which direction do you want to cast the spell? "s));
            }
            else
            {
                txt(lang(
                    u8"どの方向に振る？ "s,
                    u8"Which direction do you want to zap the wand? "s));
            }
            update_screen();
            int stat = ask_direction();
            if (stat == 0)
            {
                txt(lang(u8"それは無理だ。"s, u8"It's impossible."s));
                obvious = 0;
                return 0;
            }
        }
    }
    return 1;
}



void label_2187()
{
    int subloop = 0;
    subloop = 1;
    if (gdata_mount != 0)
    {
        if (tc == gdata_mount || tc == 0)
        {
            subloop = 2;
            if (tc == gdata_mount)
            {
                tc(1) = 0;
            }
            else
            {
                tc(1) = gdata_mount;
            }
        }
    }
    for (int cnt = 0, cnt_end = (subloop); cnt < cnt_end; ++cnt)
    {
        healhp(tc(cnt), roll(dice1, dice2, bonus));
        healcon(tc(cnt), 6);
        healcon(tc(cnt), 1, 50);
        healcon(tc(cnt), 5, 50);
        healcon(tc(cnt), 7, 30);
        healcon(tc(cnt), 9, 20);
        healsan(tc(cnt), 1);
    }
    return;
}



void label_2188()
{
    cdata[tc].poisoned = 0;
    cdata[tc].sleep = 0;
    cdata[tc].confused = 0;
    cdata[tc].blind = 0;
    cdata[tc].paralyzed = 0;
    cdata[tc].choked = 0;
    cdata[tc].dimmed = 0;
    cdata[tc].drunk = 0;
    cdata[tc].bleeding = 0;
    gdata_continuous_active_hours = 0;
    cdata[tc].hp = cdata[tc].max_hp;
    cdata[tc].mp = cdata[tc].max_mp;
    cdata[tc].sp = cdata[tc].max_sp;
    return;
}

turn_result_t proc_movement_event()
{
    if (cdata[cc].is_ridden())
    {
        return turn_result_t::turn_end;
    }
    dx = cdata[cc].next_position.x;
    dy = cdata[cc].next_position.y;
    if (cc < 16)
    {
        if (cc != 0)
        {
            if (dx != cdata[cc].position.x)
            {
                if (cdata[cc].position.x > dx)
                {
                    cdata[cc].direction = 1;
                }
                else
                {
                    cdata[cc].direction = 2;
                }
            }
            if (dy != cdata[cc].position.y)
            {
                if (cdata[cc].position.y > dy)
                {
                    cdata[cc].direction = 3;
                }
                else
                {
                    cdata[cc].direction = 0;
                }
            }
        }
    }
    if (map(dx, dy, 6) != 0)
    {
        cell_featread(dx, dy);
        if (feat(1) == 21)
        {
            return try_to_open_locked_door();
        }
        if (feat(1) == 30)
        {
            x = dx;
            y = dy;
            return do_bash();
        }
    }
    if (map(cdata[cc].position.x, cdata[cc].position.y, 8) != 0)
    {
        bool turn_ended = mef_proc_from_movement(cc);
        if (turn_ended)
        {
            return turn_result_t::turn_end;
        }
    }
    if (mdata(6) == 1)
    {
        if (cc == 0)
        {
            if (traveldone == 0)
            {
                proc_world_map_movement_event();
                keybd_wait = 1;
                return turn_result_t::turn_end;
            }
            else
            {
                traveldone = 0;
            }
        }
    }
    activate_trap();
    p = map(cdata[cc].position.x, cdata[cc].position.y, 0);
    if (chipm(0, p) == 3)
    {
        if (chipm(1, p) == 5)
        {
            healsan(cc, 1);
        }
        addefmap(cdata[cc].position.x, cdata[cc].position.y, 1, 3);
        if (cdata[cc].wet == 0)
        {
            wet(cc, 20);
        }
    }
    show_item_on_ground_info();
    if (mdata(6) == 1)
    {
        if (cc == 0)
        {
            encounter = 0;
            gdata(62) = map(cdata[cc].position.x, cdata[cc].position.y, 0);
            if (map(cdata[cc].position.x, cdata[cc].position.y, 6) == 0)
            {
                p = map(cdata[cc].position.x, cdata[cc].position.y, 0);
                if (rnd(30) == 0)
                {
                    encounter = 1;
                }
                if (gdata_weather == 4)
                {
                    if (rnd(10) == 0)
                    {
                        encounter = 1;
                    }
                }
                if (gdata_weather == 1)
                {
                    if (rnd(13) == 0)
                    {
                        encounter = 1;
                    }
                }
                if (33 <= p && p < 66)
                {
                    if (rnd(2) == 0)
                    {
                        encounter = 0;
                    }
                    if (rnd(100) == 0)
                    {
                        encounter = 2;
                    }
                }
                if (rnd(220 + cdata[0].level * 10
                        - clamp(
                              gdata_cargo_weight * 150
                                  / (gdata_current_cart_limit + 1),
                              0,
                              (210 + cdata[0].level * 10)))
                    == 0)
                {
                    encounter = 4;
                }
                if (rnd(20) == 0)
                {
                    for (int cnt = 0; cnt < 5; ++cnt)
                    {
                        rq = gdata(160 + cnt);
                        if (qdata(3, rq) == 1007)
                        {
                            if (qdata(8, rq) == 1)
                            {
                                if (qdata(4, rq) == 0)
                                {
                                    if (qdata(10, rq) < 2)
                                    {
                                        encounter = 3;
                                        encounterref = rq;
                                        ++qdata(10, rq);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (encounter == 4)
            {
                encounterlv = cdata[0].fame / 1000;
                if (encounterlv == 0)
                {
                    encounterlv = 1;
                }
                levelexitby = 4;
                return turn_result_t::exit_map;
            }
            if (encounter == 3)
            {
                txt(lang(
                    u8"暗殺者につかまった。あなたはクライアントを守らなければならない。"s,
                    u8"You are caught by assassins. You have to protect your client."s));
                msg_halt();
                gdata_previous_map2 = gdata_current_map;
                gdata_previous_dungeon_level = gdata_current_dungeon_level;
                gdata_previous_x = cdata[0].position.x;
                gdata_previous_y = cdata[0].position.y;
                levelexitby = 4;
                return turn_result_t::exit_map;
            }
            if (encounter == 2)
            {
                encounterlv = 10 + rnd(100);
                levelexitby = 4;
                return turn_result_t::exit_map;
            }
            if (encounter == 1)
            {
                p = dist_town();
                encounterlv = p * 3 / 2 - 10;
                if (cdata[0].level <= 5)
                {
                    encounterlv /= 2;
                }
                if (33 <= map(cdata[cc].position.x, cdata[cc].position.y, 0)
                    && map(cdata[cc].position.x, cdata[cc].position.y, 0) < 66)
                {
                    encounterlv /= 2;
                }
                else if (gdata_weather == 1)
                {
                    encounterlv = encounterlv * 3 / 2 + 10;
                }
                if (encounterlv < 0)
                {
                    encounterlv = 1;
                }
                auto valn = lang(
                    u8" (最も近い街までの距離:"s + p + u8" 敵勢力:"s,
                    u8" (Distance from nearest town:"s + p
                        + u8" Enemy strength:"s);
                for (int cnt = 0; cnt < 1; ++cnt)
                {
                    if (encounterlv < 5)
                    {
                        valn += lang(u8"プチ級"s, u8"Putit Rank"s);
                        break;
                    }
                    if (encounterlv < 10)
                    {
                        valn += lang(u8"オーク級"s, u8"Orc Rank"s);
                        break;
                    }
                    if (encounterlv < 20)
                    {
                        valn += lang(u8"グリズリー級"s, u8"Grizzly Bear Rank"s);
                        break;
                    }
                    if (encounterlv < 30)
                    {
                        valn += lang(u8"ドレイク級"s, u8"Drake Rank"s);
                        break;
                    }
                    if (encounterlv < 40)
                    {
                        valn += lang(u8"リッチ級"s, u8"Lich Rank"s);
                        break;
                    }
                    valn += lang(u8"ドラゴン級"s, u8"Dragon Rank"s);
                }
                valn += u8")"s;
                txt(lang(u8"襲撃だ！"s, u8"Ambush!"s) + valn);
                msg_halt();
                levelexitby = 4;
                return turn_result_t::exit_map;
            }
            encounter = 0;
        }
    }
    return turn_result_t::turn_end;
}



void proc_autopick()
{
    if (!config::instance().use_autopick)
        return;
    if (key_ctrl)
        return;
    if (adata(0, gdata_current_map) == 5 && adata(16, gdata_current_map) != 30
        && adata(16, gdata_current_map) != 31)
        return;


    for (const auto& ci : items(-1))
    {
        if (inv[ci].number == 0)
            continue;
        if (inv[ci].position != cdata[0].position)
            continue;
        if (inv[ci].own_state > 0)
            continue;

        item_checkknown(ci);

        const auto x = cdata[0].position.x;
        const auto y = cdata[0].position.y;

        bool did_something = true;
        const auto op = autopick::instance().get_operation(inv[ci]);
        switch (op.type)
        {
        case autopick::operation::type_t::do_nothing:
            did_something = false;
            break;
        case autopick::operation::type_t::pick_up:
        case autopick::operation::type_t::save:
        case autopick::operation::type_t::no_drop:
        case autopick::operation::type_t::save_and_no_drop:
            // FIXME: DRY
            if (op.show_prompt)
            {
                txt(i18n::fmt(u8"ui", u8"autopick", u8"do_you_really_pick_up")(
                    itemname(ci)));
                ELONA_YES_NO_PROMPT();
                show_prompt(promptx, prompty, 160);
                if (rtval != 0)
                {
                    did_something = false;
                    break;
                }
            }
            in = inv[ci].number;
            elona::ci = ci;
            pick_up_item();
            if (int(op.type) & int(autopick::operation::type_t::no_drop))
            {
                ibitmod(13, ti, 1);
                txt(lang(
                    itemname(ti) + u8"を大事なものに指定した。"s,
                    u8"You set "s + itemname(ti) + u8" as no-drop."s));
            }
            if (int(op.type) & int(autopick::operation::type_t::save))
            {
                if (gdata_wizard == 0)
                {
                    snd(44);
                    do_save_game();
                    txtef(5);
                    txt(lang(u8" *保存* "s, u8"*saving*"s));
                }
            }
            break;
        case autopick::operation::type_t::destroy:
            // FIXME: DRY
            if (op.show_prompt)
            {
                txt(i18n::fmt(u8"ui", u8"autopick", u8"do_you_really_destroy")(
                    itemname(ci)));
                ELONA_YES_NO_PROMPT();
                show_prompt(promptx, prompty, 160);
                if (rtval != 0)
                {
                    did_something = false;
                    break;
                }
            }
            snd(45);
            txt(i18n::fmt(u8"ui", u8"autopick", u8"destroyed")(itemname(ci)));
            inv[ci].number = 0;
            cell_refresh(x, y);
            map(x, y, 5) = map(x, y, 4);
            break;
        case autopick::operation::type_t::open:
            // FIXME: DRY
            if (op.show_prompt)
            {
                txt(i18n::fmt(u8"ui", u8"autopick", u8"do_you_really_open")(
                    itemname(ci)));
                ELONA_YES_NO_PROMPT();
                show_prompt(promptx, prompty, 160);
                if (rtval != 0)
                {
                    did_something = false;
                    break;
                }
            }
            elona::ci = ci;
            turn_result_t turn_result = do_open_command();
        }
        if (did_something && !op.sound.empty())
        {
            DSLOADFNAME(filesystem::dir::sound() / op.sound, 15);
            DSPLAY(15, 0);
        }
    }
}



void show_item_on_ground_info()
{
    if (cc == 0)
    {
        gdata(60) = -1;
        gdata(61) = -1;
        x = cdata[0].position.x;
        y = cdata[0].position.y;
        if (key_shift && gdata(30) == 0 && cdata[0].confused == 0
            && cdata[0].dimmed == 0)
        {
            if (mdata(6) != 1)
            {
                gdata(30) = 1;
                cell_check(cdata[cc].position.x + 1, cdata[cc].position.y);
                gdata(33) = cellaccess;
                cell_check(cdata[cc].position.x - 1, cdata[cc].position.y);
                gdata(31) = cellaccess;
                cell_check(cdata[cc].position.x, cdata[cc].position.y + 1);
                gdata(34) = cellaccess;
                cell_check(cdata[cc].position.x, cdata[cc].position.y - 1);
                gdata(32) = cellaccess;
            }
        }
        if (map(x, y, 4) != 0)
        {
            if (cdata[0].blind == 0)
            {
                txt(txtitemoncell(x, y));
                proc_autopick();
            }
            else
            {
                txt(lang(
                    u8"地面に何かがあるようだ。"s,
                    u8"You sense something under your foot."s));
            }
        }
        p = chipm(0, map(x, y, 0));
        if (p != 0)
        {
            if (tname(p) != ""s)
            {
                txt(lang(
                    u8"足元には"s + tname(p) + u8"がある。"s,
                    u8"You walk into "s + tname(p) + u8"."s));
            }
            if (p == 3)
            {
                snd(46);
            }
            if (p == 4)
            {
                addefmap(
                    cdata[cc].position.x,
                    cdata[cc].position.y,
                    3,
                    10,
                    dirsub,
                    rnd(2));
                if (keybd_wait <= config::instance().walkwait
                            * config::instance().startrun
                    || cdata[0].turn % 2 == 0 || mdata(6) == 1)
                {
                    snd(83 + foot % 3);
                    foot += 1 + rnd(2);
                }
            }
        }
        else if (mdata(6) == 1)
        {
            addefmap(cdata[cc].position.x, cdata[cc].position.y, 2, 10, dirsub);
            snd(81 + foot % 2);
            ++foot;
        }
        if (map(x, y, 6) != 0)
        {
            cell_featread(x, y);
            if (feat(1) == 32)
            {
                txtef(5);
                txt(lang(u8" *キラン* "s, u8"*twinkle*"s));
            }
            if (feat(1) == 15)
            {
                txt(mapname(feat(2) + feat(3) * 100, true));
                if (adata(16, feat(2) + feat(3) * 100) == 8)
                {
                    if (config::instance().extrahelp)
                    {
                        if (gdata(206) == 0)
                        {
                            if (mode == 0)
                            {
                                if (cdata[0].continuous_action_turn == 0)
                                {
                                    gdata(206) = 1;
                                    ghelp = 6;
                                    show_ex_help();
                                }
                            }
                        }
                    }
                }
            }
            if (feat(1) == 34)
            {
                txt(txtbuilding(feat(2), feat(3)));
            }
            if (feat(1) == 11)
            {
                txt(lang(
                    u8"降り階段がある。"s,
                    u8"There is a stair leading downwards here."s));
            }
            if (feat(1) == 10)
            {
                txt(lang(
                    u8"昇り階段がある。"s,
                    u8"There is a stair leading upwards here."s));
            }
            if (feat(1) == 24)
            {
                txt(lang(
                    u8"何かが見つかりそうだ。"s,
                    u8"You can search this location to collect some materials."s));
            }
            if (feat(1) == 27)
            {
                txt(lang(
                    u8"何かの残骸がある。"s,
                    u8"You can collect materials from the remains lying here."s));
            }
            if (feat(1) == 25)
            {
                txt(lang(
                    u8"採掘場がある。"s,
                    u8"You can collect materials from the mining spot here."s));
            }
            if (feat(1) == 26)
            {
                txt(lang(
                    u8"泉がある。"s,
                    u8"You can collect materials from the spring here."s));
            }
            if (feat(1) == 28)
            {
                txt(lang(
                    u8"色んな植物が生えている。"s,
                    u8"You can collect materials from plants here."s));
            }
            if (feat(1) == 29)
            {
                if (feat(2) == 36)
                {
                    s = lang(u8"野菜"s, u8"vegetable"s);
                }
                if (feat(2) == 37)
                {
                    s = lang(u8"果物"s, u8"fruit"s);
                }
                if (feat(2) == 38)
                {
                    s = lang(u8"ハーブ"s, u8"herb"s);
                }
                if (feat(2) == 39)
                {
                    s = lang(u8"何か"s, u8"strange"s);
                }
                if (feat(2) == 40)
                {
                    s = lang(u8"アーティファクト"s, u8"artifact"s);
                }
                if (feat(2) == 41)
                {
                    s = lang(u8"宝石"s, u8"gem"s);
                }
                if (feat(2) == 42)
                {
                    s = lang(u8"魔法の木"s, u8"magic"s);
                }
                if (feat == tile_plant)
                {
                    txt(lang(
                        ""s + s + u8"の種が植えてある。"s,
                        u8"A "s + s + u8" seed is planted."s));
                }
                if (feat == tile_plant + 1)
                {
                    txt(lang(
                        ""s + s + u8"が育っている。"s,
                        u8"A "s + s + u8" bud is growing."s));
                }
                if (feat == tile_plant + 2)
                {
                    txt(lang(
                        ""s + s + u8"の実が成っている。"s,
                        u8"A"s + s + u8" tree has bore a lot of fruit."s));
                }
                if (feat == tile_plant + 3)
                {
                    txt(lang(
                        ""s + s + u8"の草は枯れてしまっている… "s,
                        u8"A "s + s + u8" tree has withered..."s));
                }
            }
            if (feat(1) >= 24 && feat(1) <= 28)
            {
                if (config::instance().extrahelp)
                {
                    if (gdata(205) == 0)
                    {
                        if (mode == 0)
                        {
                            if (cdata[0].continuous_action_turn == 0)
                            {
                                gdata(205) = 1;
                                ghelp = 5;
                                show_ex_help();
                            }
                        }
                    }
                }
            }
        }
    }
    return;
}





turn_result_t npc_turn()
{
    int searchfov = 0;
    if (cdata[cc].is_hung_on_sand_bag())
    {
        if (is_in_fov(cc))
        {
            if (rnd(30) == 0)
            {
                tc = cc;
                txt(lang(
                        u8"「もっとぶって"s + _yo(2) + u8"」"s,
                        u8"\"Release me now.\""s),
                    lang(
                        u8"「こんなことして、許さない"s + _yo() + u8"」"s,
                        u8"\"I won't forget this.\""s),
                    lang(
                        u8"「何をする"s + _nda(2) + u8"」"s, u8"\"Hit me!\""s));
            }
        }
        cdata[cc].hate = 0;
        return turn_result_t::turn_end;
    }
    if (is_in_fov(cc) == 0)
    {
        if (cdata[0].blind == 0)
        {
            if (rnd(4) == 0)
            {
                if (mdata(6) != 1)
                {
                    if (cdata[cc].is_leashed())
                    {
                        if (gdata_current_map != 40)
                        {
                            if (cc < 16)
                            {
                                cdata[cc].hate = 0;
                                cdata[cc].enemy_id = 0;
                            }
                            else
                            {
                                if (rnd(2))
                                {
                                    txt(lang(u8"「痛っ！」"s, u8"\"Ouch!\""s),
                                        lang(
                                            u8"「やめて！」"s,
                                            u8"\"Stop it!\""s));
                                    hostileaction(0, cc);
                                }
                                if (rnd(4) == 0)
                                {
                                    cdata[cc].is_leashed() = false;
                                    txtef(9);
                                    txt(lang(
                                        name(cc)
                                            + u8"は巻きついていた紐をほどいた。"s,
                                        name(cc) + u8" untangle"s + _s(cc)
                                            + u8" the leash."s));
                                }
                            }
                            tc = 0;
                            efid = 619;
                            magic();
                            return turn_result_t::turn_end;
                        }
                    }
                }
            }
        }
    }
    if (cdata[cc].will_explode_soon())
    {
        tlocx = cdata[cc].position.x;
        tlocy = cdata[cc].position.y;
        efid = 644;
        magic();
        return turn_result_t::turn_end;
    }
    if (cdata[cc].relationship >= 10)
    {
        --cdata[cc].hate;
        if (cdata[cc].enemy_id == 0 || cdata[cc].hate <= 0
            || (cdata[cdata[cc].enemy_id].relationship >= -2
                && cdata[cdata[cc].enemy_id].enemy_id != cc))
        {
            cdata[cc].enemy_id = 0;
            if (pcattacker != 0)
            {
                if (cdata[pcattacker].relationship <= -3)
                {
                    if (cdata[pcattacker].state == 1)
                    {
                        if (fov_los(
                                cdata[cc].position.x,
                                cdata[cc].position.y,
                                cdata[pcattacker].position.x,
                                cdata[pcattacker].position.y))
                        {
                            cdata[cc].hate = 5;
                            cdata[cc].enemy_id = pcattacker;
                        }
                    }
                }
            }
            if (cdata[cc].enemy_id == 0)
            {
                if (cdata[0].enemy_id != 0
                    && cdata[cdata[0].enemy_id].relationship <= -3)
                {
                    if (cdata[cdata[0].enemy_id].state == 1)
                    {
                        if (fov_los(
                                cdata[cc].position.x,
                                cdata[cc].position.y,
                                cdata[cdata[0].enemy_id].position.x,
                                cdata[cdata[0].enemy_id].position.y))
                        {
                            cdata[cc].hate = 5;
                            cdata[cc].enemy_id = cdata[0].enemy_id;
                        }
                    }
                }
            }
        }
        if (cdata[cdata[cc].enemy_id].is_invisible() == 1)
        {
            if (cdata[cc].can_see_invisible() == 0)
            {
                if (cdata[cdata[cc].enemy_id].wet == 0)
                {
                    if (rnd(5))
                    {
                        cdata[cc].enemy_id = 0;
                    }
                }
            }
        }
    }
    if (cdata[cc].enemy_id != 0)
    {
        if (cdata[cdata[cc].enemy_id].state != 1)
        {
            cdata[cc].enemy_id = 0;
            cdata[cc].hate = 0;
        }
    }
    if (gdata_current_map == 40)
    {
        if (cdata[cc].relationship != -3)
        {
            if (cdata[cc].relationship != 10)
            {
                if (rnd(40) == 0)
                {
                    txtef(4);
                    txt(lang(u8"「いいぞ！」"s, u8"\"Come on!\""s),
                        lang(u8"「もっとやれー」"s, u8"\"More blood!\""s),
                        lang(u8"「血をみせろー」"s, u8"\"Beat'em!\""s),
                        lang(u8"「頑張って！」"s, u8"\"Use your brain!\""s),
                        lang(u8"「うぉぉぉぉ！」"s, u8"\"Wooooo!\""s),
                        lang(u8"「行けぇ！」"s, u8"\"Go go!\""s),
                        lang(u8"「頭を使えよ」"s, u8"\"Good fighting.\""s),
                        lang(u8"「きゃー」"s, u8"\"Yeeee!\""s));
                }
                return ai_proc_misc_map_events();
            }
        }
        cdata[cc].hate = 100;
        if (cdata[cc].relationship == 10)
        {
            p(0) = -3;
            p(1) = enemyteam;
            p(2) = 15;
        }
        else
        {
            p(0) = 10;
            p(1) = 1;
            p(2) = 16;
        }
        i = cdata[cc].enemy_id;
        if (cdata[i].relationship == p && cdata[i].state == 1 && i >= p(1)
            && i < p(1) + p(2))
        {
            if (rnd(10) != 0)
            {
                tc = i;
                goto label_2689_internal;
            }
        }
        cdata[cc].enemy_id = p(1);
        for (int cnt = 0; cnt < 100; ++cnt)
        {
            i = rnd(p(2)) + p(1);
            if (cdata[i].state == 1)
            {
                if (cdata[i].relationship == p)
                {
                    cdata[cc].enemy_id = i;
                    break;
                }
            }
        }
        if (cdata[cdata[cc].enemy_id].relationship != p
            || cdata[cdata[cc].enemy_id].state != 1)
        {
            f = 0;
            for (int cnt = p(1), cnt_end = cnt + (p(2)); cnt < cnt_end; ++cnt)
            {
                if (cdata[cnt].state == 1)
                {
                    if (cdata[cnt].relationship == p)
                    {
                        cdata[cc].enemy_id = cnt;
                        f = 1;
                        break;
                    }
                }
            }
            if (f == 0)
            {
                if (cdata[cc].relationship == 10)
                {
                    petarenawin = 1;
                }
                else
                {
                    petarenawin = 2;
                }
                return turn_result_t::turn_end;
            }
        }
    }
    if (gdata_current_map == 33)
    {
        if (cc != gdata_fire_giant)
        {
            if (cc > 16)
            {
                if (gdata_released_fire_giant != 0)
                {
                    if (cdata[gdata_fire_giant].state == 1)
                    {
                        cdata[cc].enemy_id = gdata_fire_giant;
                        cdata[cc].hate = 500;
                    }
                }
            }
        }
    }
    if (gdata_mount != 0)
    {
        if (cdata[cc].enemy_id == gdata_mount)
        {
            if (rnd(3))
            {
                cdata[cc].enemy_id = 0;
            }
        }
        if (cdata[cc].enemy_id == 0)
        {
            if (cdata[cc].relationship <= -2)
            {
                if (rnd(3) == 0)
                {
                    cdata[cc].enemy_id = gdata_mount;
                }
            }
        }
    }
    tc = cdata[cc].enemy_id;
    if (cdatan(4, cc) != ""s)
    {
        if (cdata[cc].has_custom_talk() == 0)
        {
            if (rnd(30) == 0)
            {
                txtef(9);
                txt(""s + cdatan(4, cc));
            }
        }
    }
    else if (cdata[cc].can_talk != 0 || cdata[cc].has_custom_talk())
    {
        if (cdata[cc].is_silent() == 0)
        {
            if (cdata[cc].turn % 5 == 0)
            {
                if (rnd(4) == 0)
                {
                    if (cdata[0].position.x > cdata[cc].position.x - 10
                        && cdata[0].position.x < cdata[cc].position.x + 10)
                    {
                        if (cdata[0].position.y > cdata[cc].position.y - 10
                            && cdata[0].position.y < cdata[cc].position.y + 10)
                        {
                            if (cdata[0].continuous_action_id != 6)
                            {
                                if (cdata[cc].hate <= 0)
                                {
                                    chara_custom_talk(cc, 100);
                                }
                                if (cdata[cc].hate > 0)
                                {
                                    chara_custom_talk(cc, 101);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (cdata[cc].relationship >= 0)
    {
        if (cdata[0].choked)
        {
            if (dist(
                    cdata[0].position.x,
                    cdata[0].position.y,
                    cdata[cc].position.x,
                    cdata[cc].position.y)
                == 1)
            {
                x = cdata[0].position.x;
                y = cdata[0].position.y;
                return do_bash();
            }
        }
    }
    if (cdata[cc].ai_heal != 0)
    {
        if (cdata[cc].hp < cdata[cc].max_hp / 4)
        {
            if (cdata[cc].mp > 0 || rnd(5) == 0)
            {
                efid = cdata[cc].ai_heal;
                if (efid >= 400 && efid < 467)
                {
                    npccostmp = 1;
                    int stat = label_2167();
                    if (stat == 1)
                    {
                        return turn_result_t::turn_end;
                    }
                }
                else if (efid >= 600)
                {
                    int stat = label_2174();
                    if (stat == 1)
                    {
                        return turn_result_t::turn_end;
                    }
                }
            }
        }
    }
    if (cdata[cc].item_which_will_be_used == 0)
    {
        goto label_2689_internal;
    }
    ci = cdata[cc].item_which_will_be_used;
    if (inv[ci].number == 0)
    {
        cdata[cc].item_which_will_be_used = 0;
        goto label_2689_internal;
    }
    if (cdata[cc].relationship != 0)
    {
        cdata[cc].item_which_will_be_used = 0;
    }
    {
        int category = the_item_db[inv[ci].id]->category;
        if (category == 57000)
        {
            if (cdata[cc].relationship != 10 || cdata[cc].nutrition <= 6000)
            {
                return do_eat_command();
            }
        }
        if (category == 52000)
        {
            return do_drink_command();
        }
        if (category == 53000)
        {
            return do_read_command();
        }
    }
    cdata[cc].item_which_will_be_used = 0;
label_2689_internal:
    if (cdata[cc].hate > 0 || cdata[cc].relationship == 10)
    {
        distance = dist(
            cdata[tc].position.x,
            cdata[tc].position.y,
            cdata[cc].position.x,
            cdata[cc].position.y);
        if (cdata[cc].blind != 0)
        {
            if (rnd(10) > 2)
            {
                return ai_proc_misc_map_events();
            }
        }
        if (cdata[cc].confused != 0)
        {
            if (rnd(10) > 3)
            {
                return ai_proc_misc_map_events();
            }
        }
        if (cdata[cc].relationship == 10)
        {
            if (tc == 0)
            {
                if (map(cdata[cc].position.x, cdata[cc].position.y, 4) != 0)
                {
                    const auto item_info = cell_itemoncell(cdata[cc].position);
                    const auto number = item_info.first;
                    const auto item = item_info.second;
                    if (number == 1)
                    {
                        ci = item;
                        p = the_item_db[inv[ci].id]->category;
                        if (cdata[cc].nutrition <= 6000)
                        {
                            if (p == 57000)
                            {
                                if (inv[ci].own_state <= 0)
                                {
                                    if (!is_cursed(inv[ci].curse_state))
                                    {
                                        return do_eat_command();
                                    }
                                }
                            }
                            if (p == 60001)
                            {
                                if (inv[ci].own_state <= 1)
                                {
                                    if (inv[ci].param1 >= -5)
                                    {
                                        if (inv[ci].param3 < 20)
                                        {
                                            if (inv[ci].id != 602)
                                            {
                                                return do_drink_command();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        if (p == 68000 || p == 77000)
                        {
                            if (inv[ci].own_state <= 0)
                            {
                                if (ibit(5, ci) == 0)
                                {
                                    if (mdata(6) != 5)
                                    {
                                        in = inv[ci].number;
                                        if (gdata_mount != cc)
                                        {
                                            int stat = pick_up_item();
                                            if (stat == 1)
                                            {
                                                return turn_result_t::turn_end;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (cdata[cc].current_map == gdata_current_map)
                {
                    if (cdata[cc].is_contracting() == 0)
                    {
                        return ai_proc_misc_map_events();
                    }
                }
                if (distance > 2 || rnd(3))
                {
                    return proc_npc_movement_event();
                }
                else
                {
                    return ai_proc_misc_map_events();
                }
            }
        }
        if (cdata[cc].fear != 0)
        {
            return proc_npc_movement_event(true);
        }
        if (cdata[cc].blind != 0)
        {
            if (rnd(3))
            {
                return ai_proc_misc_map_events();
            }
        }
        if (distance != cdata[cc].ai_dist)
        {
            if (rnd(100) < cdata[cc].ai_move)
            {
                return proc_npc_movement_event();
            }
        }
        return ai_proc_basic();
    }
    if (cdata[cc].turn % 10 == 1)
    {
        searchfov = 5;
        f = 0;
        for (int cnt = 0, cnt_end = (searchfov); cnt < cnt_end; ++cnt)
        {
            y = cdata[cc].position.y - 2 + cnt;
            if (y < 0 || y >= mdata(1))
            {
                continue;
            }
            int c{};
            for (int cnt = 0, cnt_end = (searchfov); cnt < cnt_end; ++cnt)
            {
                x = cdata[cc].position.x - 2 + cnt;
                if (x < 0 || x >= mdata(0))
                {
                    continue;
                }
                c = map(x, y, 1) - 1;
                if (c <= 0)
                {
                    continue;
                }
                if (cdata[cc].original_relationship <= -3)
                {
                    if (cdata[c].relationship > -3)
                    {
                        if (cdata[c].does_not_search_enemy() == 0)
                        {
                            f = 1;
                            break;
                        }
                    }
                }
                else if (c >= 57)
                {
                    if (cdata[c].original_relationship <= -3)
                    {
                        if (cdata[c].does_not_search_enemy() == 0)
                        {
                            f = 1;
                            break;
                        }
                    }
                }
            }
            if (f)
            {
                if (cdata[cc].does_not_search_enemy() == 0)
                {
                    cdata[cc].enemy_id = c;
                    cdata[cc].hate = 30;
                    cdata[cc].emotion_icon = 218;
                    break;
                }
            }
        }
    }
    if (tc == 0)
    {
        r2 = cc;
        int stat = try_to_perceive_npc(tc);
        if (stat == 1)
        {
            if (cdata[cc].relationship == -3)
            {
                cdata[cc].hate = 30;
            }
        }
    }
    return ai_proc_misc_map_events();
}



bool turn_wrapper()
{
    bool finished = false;
    turn_result_t result = turn_begin();
    while (!finished)
    {
        switch (result)
        {
            // Turn lifecycle

        case turn_result_t::turn_begin:
            result = turn_begin();
            break;
        case turn_result_t::turn_end:
            result = turn_end();
            break;
        case turn_result_t::pass_one_turn:
            result = pass_turns(true);
            break;
        case turn_result_t::pass_one_turn_freeze_time:
            result = pass_turns(false);
            break;
        case turn_result_t::pc_turn:
            result = pc_turn();
            break;
        case turn_result_t::npc_turn:
            result = npc_turn();
            break;
        case turn_result_t::pc_turn_user_error:
            result = pc_turn(false);
            break;
        case turn_result_t::pc_died:
            result = pc_died();
            break;
        case turn_result_t::initialize_map:
            result = initialize_map();
            break;
        case turn_result_t::exit_map:
            result = exit_map();
            break;
        case turn_result_t::play_scene:
            result = play_scene();
            break;
        case turn_result_t::finish_elona:
            finish_elona();
            finished = true;
            break;

            // Menus that don't return success status

        case turn_result_t::show_chat_history:
            result = show_chat_history();
            break;
        case turn_result_t::show_message_log:
            result = show_message_log();
            break;
        case turn_result_t::show_house_board:
            result = show_house_board();
            break;
        case turn_result_t::show_quest_board:
            result = show_quest_board();
            break;
        case turn_result_t::show_skill_list:
            result = show_skill_list();
            break;
        case turn_result_t::show_spell_list:
            result = show_spell_list();
            break;

            // Menus with a success status

        case turn_result_t::menu_journal:
            result = menu_journal().turn_result;
            break;
        case turn_result_t::menu_materials:
            result = menu_materials().turn_result;
            break;
        case turn_result_t::menu_character_sheet:
            result = menu_character_sheet().turn_result;
            break;
        case turn_result_t::menu_equipment:
            result = menu_equipment().turn_result;
            break;
        case turn_result_t::menu_feats:
            result = menu_feats().turn_result;
            break;
        case turn_result_t::ctrl_inventory:
            result = ctrl_inventory().turn_result;
            break;

        case turn_result_t::all_turns_finished:
            result = turn_result_t::turn_begin;
            break;
        case turn_result_t::none:
        default:
            assert(0);
            break;
        }
    }
    return finished;
}

turn_result_t pass_turns(bool time)
{
    bool finished = false;
    turn_result_t result = pass_one_turn(time);
    while (!finished)
    {
        switch (result)
        {
            case turn_result_t::all_turns_finished:
                finished = true;
                break;
            case turn_result_t::pass_one_turn:
                time = true;
                break;
            case turn_result_t::pass_one_turn_freeze_time:
                time = false;
                break;
            default:
                return result;
        }
        result = pass_one_turn(time);
    }
    return turn_result_t::all_turns_finished;
}

turn_result_t turn_begin()
{
    int turncost = 0;
    int spd = 0;
    ct = 0;
    mef_update();
    gspd = cdata[0].current_speed * (100 + cdata[0].speed_percentage) / 100;
    if (gspd < 10)
    {
        gspd = 10;
    }
    turncost = (mdata(9) - cdata[0].turn_cost) / gspd + 1;
    if (event_was_set())
    {
        return event_start_proc(); // TODO avoid evnum side effect
    }
    if (cdata[0].state != 1)
    {
        return turn_result_t::pc_died;
    }

    bool update_turn_cost = true;
    if (mdata(6) == 1)
    {
        if (cdata[0].continuous_action_turn > 2)
        {
            cdata[0].turn_cost = mdata(9);
            update_turn_cost = false;
        }
    }
    if (update_turn_cost)
    {
        for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
        {
            if (cdata[cnt].state != 1)
            {
                continue;
            }
            spd = cdata[cnt].current_speed * (100 + cdata[cnt].speed_percentage)
                / 100;
            if (spd < 10)
            {
                spd = 10;
            }
            cdata[cnt].turn_cost += spd * turncost;
        }
    }

    gdata_second += turncost / 5 + 1;
    if (gdata_second >= 60)
    {
        ++gdata_play_turns;
        cc = 0;
        if (gdata_play_turns % 20 == 0)
        {
            monster_respawn();
        }
        if (gdata_play_turns % 10 == 1)
        {
            label_1580();
        }
        gdata_minute += gdata_second / 60;
        if (gdata_left_minutes_of_executing_quest > 0)
        {
            gdata_left_minutes_of_executing_quest -= gdata_second / 60;
            if (gdata(87) > gdata_left_minutes_of_executing_quest / 10)
            {
                txtef(9);
                txt(lang(
                    u8"クエスト[残り"s
                        + (gdata_left_minutes_of_executing_quest + 1)
                        + u8"分]"s,
                    ""s + (gdata_left_minutes_of_executing_quest + 1)
                        + u8" min left for the quest."s));
                gdata(87) = gdata_left_minutes_of_executing_quest / 10;
            }
            if (gdata_left_minutes_of_executing_quest <= 0)
            {
                gdata_left_minutes_of_executing_quest = 0;
                event_add(14);
            }
        }
        gdata_second = gdata_second % 60;
        if (gdata_minute >= 60)
        {
            gdata_hour += gdata_minute / 60;
            gdata_minute = gdata_minute % 60;
            label_2736();
        }
    }
    return turn_result_t::pass_one_turn;
}

void label_2735()
{
    if (gdata_weather == 2)
    {
        if (gdata_pc_home_x < 65 && gdata_pc_home_y > 10)
        {
            gdata_weather = 3;
            envonly = 1;
            play_music();
            gdata_hours_until_weather_changes += 3;
            txt(lang(u8"天候が変わった。"s, u8"The weather changes."s));
        }
    }
    if (gdata_weather == 4 || gdata_weather == 3)
    {
        if (gdata_pc_home_x > 65 || gdata_pc_home_y < 10)
        {
            gdata_weather = 2;
            envonly = 1;
            play_music();
            gdata_hours_until_weather_changes += 3;
            txt(lang(u8"天候が変わった。"s, u8"The weather changes."s));
        }
    }
    return;
}



void label_2736()
{
    if (adata(16, gdata_current_map) == 101)
    {
        update_museum();
    }
    if (gdata_current_map == 7)
    {
        label_1730();
    }
    if (mdata(6) == 1)
    {
        gdata_pc_home_x = cdata[0].position.x;
        gdata_pc_home_y = cdata[0].position.y;
    }
    --gdata_hours_until_weather_changes;
    label_2735();
    if (gdata_hours_until_weather_changes < 0)
    {
        gdata_hours_until_weather_changes = rnd(22) + 2;
        p = gdata_weather;
        for (int cnt = 0; cnt < 1; ++cnt)
        {
            if (gdata_month % 3 == 0)
            {
                if (gdata_day >= 1 && gdata_day <= 10)
                {
                    if (gdata_last_etherwind_month != gdata_month)
                    {
                        if (rnd(15) < gdata_day + 5)
                        {
                            gdata_weather = 1;
                            txtef(3);
                            txt(lang(
                                u8"エーテルの風が吹き始めた。すぐに避難しなくては。"s,
                                u8"Ether Wind starts to blow. You need to find a shelter!"s));
                            gdata_last_etherwind_month = gdata_month;
                            gdata_hours_until_weather_changes = rnd(24) + 24;
                            break;
                        }
                    }
                }
            }
            if (p == 0)
            {
                weatherbk = 0;
                if (trait(209) != 0)
                {
                    if (rnd(4) == 0)
                    {
                        gdata_weather = 3;
                        txt(lang(
                            u8"あなたは雨雲を引き寄せた。"s,
                            u8"You draw a rain cloud."s));
                        break;
                    }
                }
                if (gdata_pc_home_x > 65 || gdata_pc_home_y < 10)
                {
                    if (rnd(2) == 0)
                    {
                        gdata_weather = 2;
                        txt(lang(
                            u8"雪が降ってきた。"s, u8"It starts to snow."s));
                        break;
                    }
                }
                else
                {
                    if (rnd(10) == 0)
                    {
                        gdata_weather = 3;
                        txt(lang(
                            u8"雨が降り出した。"s, u8"It starts to rain."s));
                        break;
                    }
                    if (rnd(40) == 0)
                    {
                        gdata_weather = 4;
                        txt(lang(
                            u8"突然どしゃぶりになった。"s,
                            u8"Suddenly, rain begins to pour down from the sky."s));
                        break;
                    }
                    if (rnd(60) == 0)
                    {
                        gdata_weather = 2;
                        txt(lang(
                            u8"雪が降ってきた。"s, u8"It starts to snow."s));
                        break;
                    }
                }
            }
            if (p == 3)
            {
                if (rnd(4) == 0)
                {
                    gdata_weather = 0;
                    txt(lang(u8"雨は止んだ。"s, u8"It stops raining."s));
                    break;
                }
                if (rnd(15) == 0)
                {
                    gdata_weather = 4;
                    txt(lang(
                        u8"雨が本格的に降り出した。"s,
                        u8"The rain becomes heavier."s));
                    break;
                }
            }
            if (p == 4)
            {
                if (rnd(3) == 0)
                {
                    gdata_weather = 3;
                    txt(lang(
                        u8"雨は小降りになった。"s,
                        u8"The rain becomes lighter."s));
                    break;
                }
            }
            if (p == 1)
            {
                if (rnd(2) == 0)
                {
                    gdata_weather = 0;
                    txt(lang(
                        u8"エーテルの風は止んだ。"s,
                        u8"The Ether Wind dissipates."s));
                    break;
                }
            }
            if (p == 2)
            {
                if (rnd(3) == 0)
                {
                    gdata_weather = 0;
                    txt(lang(u8"雪は止んだ。"s, u8"It stops raining."s));
                    break;
                }
            }
        }
        if (gdata_weather == 4)
        {
            if (config::instance().extrahelp)
            {
                if (gdata(211) == 0)
                {
                    if (mode == 0)
                    {
                        if (cdata[0].continuous_action_turn == 0)
                        {
                            gdata(211) = 1;
                            ghelp = 11;
                            show_ex_help();
                        }
                    }
                }
            }
        }
        if (gdata_weather == 2)
        {
            if (config::instance().extrahelp)
            {
                if (gdata(212) == 0)
                {
                    if (mode == 0)
                    {
                        if (cdata[0].continuous_action_turn == 0)
                        {
                            gdata(212) = 1;
                            ghelp = 12;
                            show_ex_help();
                        }
                    }
                }
            }
        }
        if (gdata_weather == 1)
        {
            if (config::instance().extrahelp)
            {
                if (gdata(213) == 0)
                {
                    if (mode == 0)
                    {
                        if (cdata[0].continuous_action_turn == 0)
                        {
                            gdata(213) = 1;
                            ghelp = 13;
                            show_ex_help();
                        }
                    }
                }
            }
        }
        if (p != gdata_weather)
        {
            envonly = 1;
            play_music();
        }
    }
    label_1746();
    label_2662();
    label_1576();
    if (mdata(6) == 1)
    {
        if (rnd(3) == 0)
        {
            ++gdata_continuous_active_hours;
        }
        if (rnd(15) == 0)
        {
            if (mode == 0)
            {
                txt(lang(u8"仮眠をとった。"s, u8"You take a nap."s));
                gdata_continuous_active_hours -= 3;
                if (gdata_continuous_active_hours < 0)
                {
                    gdata_continuous_active_hours = 0;
                }
            }
        }
    }
    else if (gdata_current_map != 30)
    {
        ++gdata_continuous_active_hours;
    }
    if (gdata_hour == 6)
    {
        txtef(5);
        txt(lang(u8"夜が明けた。"s, u8"Day breaks."s));
    }
    if (gdata_continuous_active_hours >= 15)
    {
        if (config::instance().extrahelp)
        {
            if (gdata(209) == 0)
            {
                if (mode == 0)
                {
                    if (cdata[0].continuous_action_turn == 0)
                    {
                        gdata(209) = 1;
                        ghelp = 9;
                        show_ex_help();
                    }
                }
            }
        }
    }
    if (cdata[0].nutrition < 5000)
    {
        if (config::instance().extrahelp)
        {
            if (gdata(210) == 0)
            {
                if (mode == 0)
                {
                    if (cdata[0].continuous_action_turn == 0)
                    {
                        gdata(210) = 1;
                        ghelp = 10;
                        show_ex_help();
                    }
                }
            }
        }
    }
    if (gdata_hour >= 24)
    {
        if (gdata_number_of_waiting_guests < 3)
        {
            if (rnd(8 + gdata_number_of_waiting_guests * 5) == 0)
            {
                ++gdata_number_of_waiting_guests;
            }
        }
        txtef(5);
        txt(lang(
            u8"一日が終わり、日付が変わった。"s,
            u8"A day passes and a new day begins."s));
        update_shop_and_report();
        for (int cnt = 0; cnt < 9; ++cnt)
        {
            p = 120 + cnt;
            if (gdata(p) >= 10000)
            {
                gdata(p) = 10000;
                continue;
            }
            if (cnt == 3 || cnt == 4 || cnt == 5 || cnt == 8)
            {
                continue;
            }
            --gdata(140 + cnt);
            if (gdata(140 + cnt) <= 0)
            {
                modrank(cnt, (gdata(p) / 12 + 100) * -1);
                gdata(140 + cnt) = ranknorma(cnt);
            }
        }
        snd(74);
        event_add(10);
        gdata_play_days += gdata_hour / 24;
        gdata_day += gdata_hour / 24;
        gdata_hour = gdata_hour % 24;
        if (gdata_day >= 31)
        {
            ++gdata_month;
            gdata_day = gdata_day - 30;
            if (gdata_month % 2 == 0)
            {
                ++gdata_holy_well_count;
            }
        }
        if (gdata_month >= 13)
        {
            ++gdata_year;
            gdata_month = 1;
            gdata_last_month_when_trainer_visited = 0;
            gdata_wish_count = clamp(gdata_wish_count - 1, 0, 10);
            gdata_lost_wallet_count =
                clamp(gdata_lost_wallet_count - 1, 0, 999999);
        }
        if (gdata_day == 1 || gdata_day == 15)
        {
            label_1901();
        }
        if (gdata_pael_and_her_mom == 1 || gdata_pael_and_her_mom == 3
            || gdata_pael_and_her_mom == 5 || gdata_pael_and_her_mom == 7
            || gdata_pael_and_her_mom == 9)
        {
            if (adata(16, gdata_current_map) != 33)
            {
                if (rnd(20) == 0)
                {
                    ++gdata_pael_and_her_mom;
                    snd(44);
                    txtef(2);
                    txt(lang(
                        u8"ジャーナルが更新された。"s,
                        u8"Your journal has been updated."s));
                }
            }
        }
    }
    if (mode == 0)
    {
        if (mdata(6) == 1)
        {
            if (rnd(40) == 0)
            {
                --cdata[0].piety_point;
            }
            cdata[0].praying_point += 4;
        }
        else
        {
            if (rnd(5) == 0)
            {
                --cdata[0].piety_point;
            }
            cdata[0].praying_point += 32;
        }
    }
    if (cdata[0].piety_point < 0)
    {
        cdata[0].piety_point = 0;
    }
    if (cdata[0].continuous_action_turn != 0)
    {
        if (cdata[0].continuous_action_id != 3)
        {
            update_screen();
        }
    }
    if (gdata_current_map != 30)
    {
        if (gdata_current_map != 41)
        {
            proc_random_event();
        }
    }
}



turn_result_t pass_one_turn(bool label_2738_flg)
{
    if (label_2738_flg)
    {
        while (ct < ELONA_MAX_CHARACTERS)
        {
            if (cdata[ct].state != 1)
            {
                ++ct;
                continue;
            }
            if (cdata[ct].turn_cost >= mdata(9))
            {
                cdata[ct].turn_cost -= mdata(9);
                break;
            }
            else
            {
                ++ct;
            }
        }
        if (ct >= ELONA_MAX_CHARACTERS)
        {
            return turn_result_t::all_turns_finished;
        }
    }
    cc = ct;
    cdata[cc].speed_percentage = cdata[cc].speed_percentage_in_next_turn;
    ++cdata[cc].turn;
    label_27412();
    if (ct == 0)
    {
        tnew = 1;
        pcnoise = 0;
        refreshspeed(0);
        p = cdata[0].turn % 10;
        if (p == 1)
        {
            for (int cnt = 0; cnt < 16; ++cnt)
            {
                if (cdata[cnt].state == 1)
                {
                    label_1464(cnt);
                }
            }
        }
        if (p == 2)
        {
            label_1465(0);
        }
        if (p == 3)
        {
            label_1468(0);
        }
        if (p == 4)
        {
            if (cdata[0].continuous_action_id == 0)
            {
                healsp(0, 2);
            }
        }
        if (gdata_is_returning_or_escaping != 0)
        {
            --gdata_is_returning_or_escaping;
            if (mdata(6) == 7 || gdata_current_map == 30
                || gdata_current_map == 41)
            {
                gdata_is_returning_or_escaping = 0;
                txt(lang(
                    u8"不思議な力が帰還を阻止した。"s,
                    u8"Strange power prevents you from returning."s));
                goto label_2740_internal;
            }
            if (gdata_is_returning_or_escaping <= 0 && !event_was_set())
            {
                f = 0;
                for (int cnt = 1; cnt < 16; ++cnt)
                {
                    if (cdata[cnt].state != 1)
                    {
                        continue;
                    }
                    if (cdata[cnt].is_escorted_in_sub_quest() == 1)
                    {
                        f = 1;
                    }
                }
                if (f)
                {
                    txt(lang(
                        u8"今は帰還できない仲間を連れている。"s,
                        u8"One of your allies prevents you from returning."s));
                    goto label_2740_internal;
                }
                if (1 && cdata[0].inventory_weight_type >= 4)
                {
                    txt(lang(
                        u8"どこからか声が聞こえた。「悪いが重量オーバーだ」"s,
                        u8"Someone shouts, \"Sorry, overweight.\""s));
                    goto label_2740_internal;
                }
                if (gdata_destination_map == gdata(850))
                {
                    if (gdata_current_map == gdata(850))
                    {
                        txt(lang(u8"何もおきない… "s, u8"Nothing happens..."s));
                        goto label_2740_internal;
                    }
                }
                int stat = quest_is_return_forbidden();
                if (stat == 1)
                {
                    txt(lang(
                        u8"あなたは法を犯した。"s, u8"You commit a crime."s));
                    modify_karma(0, -10);
                }
                snd(72);
                txt(lang(
                    u8"あなたは次元の扉を開けた。"s,
                    u8"A dimensional door opens in front of you."s));
                if (gdata_destination_map == 41)
                {
                    txt(lang(
                        u8"気まぐれな時の管理者により次元は歪められた！"s,
                        u8"The capricious controller of time has changed your destination!"s));
                }
                msg_halt();
                update_screen();
                levelexitby = 2;
                return turn_result_t::exit_map;
            }
            goto label_2740_internal;
        }
    label_2740_internal:
        label_1754();
        if (cdata[0].state != 1)
        {
            return turn_result_t::pc_died;
        }
        if (gdata_weather == 1)
        {
            if (mdata(14) == 2)
            {
                if (rnd(2) == 0)
                {
                    if (gdata_protects_from_etherwind == 0)
                    {
                        modcorrupt(5 + clamp(gdata_play_turns / 20000, 0, 15));
                    }
                    else if (rnd(10) == 0)
                    {
                        modcorrupt(5);
                    }
                }
                if (gdata_protects_from_etherwind == 0 || rnd(4) == 0)
                {
                    if (rnd(2000) == 0)
                    {
                        efid = 454;
                        efp = 100;
                        tc = cc;
                        magic();
                    }
                }
            }
        }
        else if (rnd(1500) == 0)
        {
            if (adata(16, gdata_current_map) != 7 && gdata_current_map != 30)
            {
                modcorrupt(10);
            }
        }
    }
    tc = cc;
    if (map(cdata[tc].position.x, cdata[tc].position.y, 8) != 0)
    {
        mef_proc(tc);
    }
    if (cdata[cc].buffs[0].id != 0)
    {
        for (int cnt = 0; cnt < 16; ++cnt)
        {
            if (cdata[cc].buffs[cnt].id == 0)
            {
                break;
            }
            --cdata[cc].buffs[cnt].turns;
            if (cdata[cc].buffs[cnt].turns <= 0)
            {
                if (cdata[cc].buffs[cnt].id == 16)
                {
                    dmghp(cc, 9999, -11);
                }
                delbuff(cc, cnt);
                --cnt;
                continue;
            }
        }
    }
    if (cdata[cc].choked > 0 || cdata[cc].sleep > 0 || cdata[cc].paralyzed > 0
        || cdata[cc].dimmed >= 60)
    {
        if (cc == 0)
        {
            if (cdata[cc].choked)
            {
                await(120);
            }
            await(60);
            sxfix = 0;
            syfix = 0;
            update_screen();
        }
        return turn_result_t::turn_end;
    }
    if (cdata[cc].drunk != 0)
    {
        if (rnd(200) == 0)
        {
            if (cc != 0)
            {
                for (int cnt = 0; cnt < ELONA_MAX_CHARACTERS; ++cnt)
                {
                    if (cdata[cnt].state != 1)
                    {
                        continue;
                    }
                    if (dist(
                            cdata[cc].position.x,
                            cdata[cc].position.y,
                            cdata[cnt].position.x,
                            cdata[cnt].position.y)
                        > 5)
                    {
                        continue;
                    }
                    if (fov_los(
                            cdata[cc].position.x,
                            cdata[cc].position.y,
                            cdata[cnt].position.x,
                            cdata[cnt].position.y)
                        == 0)
                    {
                        continue;
                    }
                    if (cnt == cc || rnd(3) || mdata(6) == 1)
                    {
                        continue;
                    }
                    tc = cnt;
                    if (is_in_fov(cc) || is_in_fov(tc))
                    {
                        txtef(9);
                        txt(lang(
                            name(cc) + u8"は酔っ払って"s + name(tc)
                                + u8"にからんだ。"s,
                            name(cc)
                                + u8" gets the worse for drink and catches "s
                                + name(tc) + u8"."s));
                        if (jp)
                        {
                            txt(u8"「一杯どうだい？」"s,
                                u8"「飲んでないよ」"s,
                                u8"「何見てるのさ」"s,
                                u8"「遊ぼうぜ」"s);
                        }
                        if (en)
                        {
                            txt(u8"\"Have a drink baby.\""s,
                                u8"\"What are you looking at?\""s,
                                u8"\"I ain't drunk.\""s,
                                u8"\"Let's have fun.\""s);
                        }
                    }
                    if (rnd(4) == 0)
                    {
                        if (tc != 0)
                        {
                            if (is_in_fov(cc) || is_in_fov(tc))
                            {
                                txtef(9);
                                txt(lang(
                                    name(tc) + u8"はカチンときた。"s,
                                    name(tc)
                                        + u8" is pretty annoyed with the drunkard."s));
                                txt(lang(
                                    u8"「酔っ払いにはうんざり！」"s,
                                    u8"\"Your time is over, drunk!\""s));
                            }
                            cdata[tc].hate = 20;
                            cdata[tc].enemy_id = cc;
                            cdata[tc].emotion_icon = 218;
                        }
                    }
                    break;
                }
            }
        }
    }
    if (cdata[cc].drunk >= 45 || cdata[cc].nutrition > 35000)
    {
        if (rnd(60) == 0)
        {
            chara_vomit(cc);
            return turn_result_t::turn_end;
        }
    }
    if (cdata[cc].stops_continuous_action_if_damaged == 1)
    {
        if (cc == 0)
        {
            if (cdata[cc].continuous_action_id != 1
                && cdata[cc].continuous_action_id != 2
                && cdata[cc].continuous_action_id != 3)
            {
                rtval = 0;
            }
            else
            {
                screenupdate = -1;
                update_screen();
                prompt_stop_continuous_action();
            }
        }
        if (cc != 0 || rtval == 0)
        {
            if (is_in_fov(cc))
            {
                txt(lang(
                    name(cc) + u8"は"s
                        + i18n::_(
                              u8"ui",
                              u8"action",
                              u8"_"s + cdata[cc].continuous_action_id)
                        + u8"を中断した。"s,
                    name(cc) + u8" stop"s + _s(cc) + u8" "s
                        + i18n::_(
                              u8"ui",
                              u8"action",
                              u8"_"s + cdata[cc].continuous_action_id)
                        + u8"."s));
            }
            rowactend(cc);
        }
        screenupdate = -1;
        update_screen();
        cdata[cc].stops_continuous_action_if_damaged = 0;
    }
    if (cdata[cc].turn % 25 == 0)
    {
        if (cdata[cc].curse_power != 0)
        {
            label_1577();
        }
        if (cdata[cc].has_cursed_equipments())
        {
            label_1579();
        }
        if (cdata[cc].is_pregnant())
        {
            label_1578();
        }
    }
    if (cdata[cc].continuous_action_id != 0)
    {
        ci = cdata[cc].continuous_action_item;
        --cdata[cc].continuous_action_turn;
        if (cdata[cc].continuous_action_id == 7)
        {
            auto_turn(40);
            spot_fishing();
        }
        if (cdata[cc].continuous_action_id == 5)
        {
            auto_turn(15);
            spot_mining_or_wall();
        }
        if (cdata[cc].continuous_action_id == 8)
        {
            auto_turn(15);
            spot_material();
        }
        if (cdata[cc].continuous_action_id == 9)
        {
            auto_turn(15);
            spot_digging();
        }
        if (cdata[cc].continuous_action_id == 4)
        {
            auto_turn(5);
            do_rest();
        }
        if (cdata[cc].continuous_action_id == 1)
        {
            auto_turn(100);
            return do_eat_command();
        }
        if (cdata[cc].continuous_action_id == 2)
        {
            auto_turn(25);
            return do_read_command();
        }
        if (cdata[cc].continuous_action_id == 11)
        {
            auto_turn(50);
            do_thing_that_feels_good();
        }
        if (cdata[cc].continuous_action_id == 10)
        {
            if (gdata(91) == 103)
            {
                auto_turn(40);
            }
            else if (gdata(91) == 104)
            {
                auto_turn(40);
            }
            else if (gdata(91) == 105)
            {
                auto_turn(50);
            }
            else
            {
                auto_turn(20);
            }
            continuous_action_others();
        }
        if (cdata[cc].continuous_action_id == 12)
        {
            auto_turn(20);
            label_19342();
        }
        if (cdata[cc].continuous_action_id == 6)
        {
            auto_turn(40);
            do_perform();
        }
        if (cdata[cc].continuous_action_id == 3)
        {
            proc_world_map_movement_event();
            return proc_movement_event();
        }
        if (cdata[cc].continuous_action_turn > 0)
        {
            return turn_result_t::turn_end;
        }
        rowactend(cc);
        if (cc == 0)
        {
            if (chatteleport == 1)
            {
                chatteleport = 0;
                return turn_result_t::exit_map;
            }
        }
    }
    if (cdata[cc].needs_refreshing_status())
    {
        chara_refresh(cc);
    }
    if (cdata[cc].state == 1)
    {
        if (ct == 0)
        {
            return turn_result_t::pc_turn;
        }
        else
        {
            return turn_result_t::npc_turn;
        }
    }
    return turn_result_t::pass_one_turn;
}



void label_27412()
{
label_27411_internal:
    cdata[cc].emotion_icon -= 100;
    if (cdata[cc].emotion_icon < 0)
    {
        cdata[cc].emotion_icon = 0;
    }
    if (mdata(14) == 2)
    {
        if (gdata_weather >= 3)
        {
            cdata[cc].wet = 50;
        }
    }
    if (cdata[cc].experience >= cdata[cc].required_experience)
    {
        if (cc == 0)
        {
            snd(60);
            msgalert = 1;
        }
        r2 = 0;
        gain_level(cc);
        goto label_27411_internal;
    }
    return;
}



turn_result_t turn_end()
{
    cc = ct;
    if (cdata[cc].state != 1)
    {
        return turn_result_t::pass_one_turn;
    }
    damage_apply_status_effects(cc);
    if (cc == 0)
    {
        chatturn = 10;
        if (gdata(98) != 0)
        {
            if (gdata(98) < 10000)
            {
                gdata(98) += 10000;
                gdata(91) = 100;
                continuous_action_others();
            }
        }
        if (cdata[0].inventory_weight_type >= 3)
        {
            if (rnd(20) == 0)
            {
                txt(lang(
                    name(0) + u8"は荷物に圧迫されもがいた。"s,
                    u8"Your backpack is squashing you!"s));
                dmghp(
                    cc,
                    cdata[cc].max_hp
                            * (cdata[0].inventory_weight * 10
                                   / cdata[0].max_inventory_weight
                               + 10)
                            / 200
                        + 1,
                    -6);
            }
        }
        get_hungry(cc);
        refreshspeed(cc);
    }
    else if (mdata(6) != 1)
    {
        cdata[cc].nutrition -= 16;
        if (cdata[cc].nutrition < 6000)
        {
            if (cdata[cc].has_anorexia() == 0)
            {
                cdata[cc].nutrition = 6000;
            }
        }
    }
    if (gdata_left_turns_of_timestop > 0)
    {
        --gdata_left_turns_of_timestop;
        if (cdata[cc].state != 1 || gdata_left_turns_of_timestop == 0)
        {
            txtef(9);
            txt(lang(
                u8"時は再び動き出した。"s, u8"Time starts to run again."s));
        }
        else
        {
            if (cc != 0)
            {
                update_screen();
                await(200);
            }
            txtef(9);
            txt(u8" *tick* "s);
            return turn_result_t::pass_one_turn_freeze_time;
        }
    }
    return turn_result_t::pass_one_turn;
}



turn_result_t pc_turn(bool advance_time)
{
    if (advance_time)
    {
        if (gdata_catches_god_signal)
        {
            if (rnd(1000) == 0)
            {
                txtgod(cdata[0].god_id, 12);
            }
        }
        gdata(808) = 0;
        tgloc = 0;
        if (gdata_mount != 0)
        {
            cdata[gdata_mount].position = cdata[0].position;
        }
        if (mdata(6) == 1)
        {
            map(cdata[0].position.x, cdata[0].position.y, 1) = 1;
        }
        if (gdata_ether_disease_stage >= 20000)
        {
            dmghp(0, 999999, -14);
        }
        if (cdata[0].state != 1)
        {
            return turn_result_t::pc_died;
        }
        if (gdata(30))
        {
            await(10);
            for (int dy = -1; dy <= 1; ++dy)
            {
                y = cdata[0].position.y + dy;
                if (y < 0 || y <= mdata(1))
                    continue;
                for (int dx = -1; dx <= 1; ++dx)
                {
                    x = cdata[0].position.x + dx;
                    if (x < 0 || x <= mdata(0))
                        continue;
                    if (map(x, y, 1) != 0)
                    {
                        p = map(x, y, 1) - 1;
                        if (p != 0 && cdata[p].relationship <= -3)
                        {
                            gdata(30) = 0;
                        }
                    }
                }
            }
            x = cdata[0].position.x;
            y = cdata[0].position.y;
            cdata[0].next_position.x = x + dirxy(0, gdata(35));
            cdata[0].next_position.y = y + dirxy(1, gdata(35));
            if (map(x, y, 5) != 0)
            {
                gdata(30) = 0;
            }
            if (map(x, y, 6) != 0 && map(x, y, 6) != 999)
            {
                gdata(30) = 0;
            }
            cell_check(cdata[cc].position.x + 1, cdata[cc].position.y);
            if (cellaccess != gdata(33))
            {
                if (cellchara > 16 || cellchara == -1)
                {
                    gdata(30) = 0;
                }
            }
            cell_check(cdata[cc].position.x - 1, cdata[cc].position.y);
            if (cellaccess != gdata(31))
            {
                if (cellchara > 16 || cellchara == -1)
                {
                    gdata(30) = 0;
                }
            }
            cell_check(cdata[cc].position.x, cdata[cc].position.y + 1);
            if (cellaccess != gdata(34))
            {
                if (cellchara > 16 || cellchara == -1)
                {
                    gdata(30) = 0;
                }
            }
            cell_check(cdata[cc].position.x, cdata[cc].position.y - 1);
            if (cellaccess != gdata(32))
            {
                if (cellchara > 16 || cellchara == -1)
                {
                    gdata(30) = 0;
                }
            }
            cell_check(cdata[0].next_position.x, cdata[0].next_position.y);
            if (cellaccess == 0)
            {
                if (cellchara > 16 || cellchara == -1)
                {
                    gdata(30) = 0;
                }
            }
        }
        if (autosave)
        {
            autosave = 0;
            if (gdata_wizard == 0 && gdata_current_map != 40
                && config::instance().autosave)
            {
                snd(44);
                do_save_game();
                txtef(5);
                txt(lang(u8" *保存* "s, u8"*saving*"s));
            }
        }
        if (autoturn == 1)
        {
            autoturn = 0;
            update_screen();
        }
        else
        {
            update_screen();
        }
        if (gdata_current_map == 40)
        {
            gdata(73) = 3;
            bool pet_exists = false;
            for (int cc = 1; cc < 16; ++cc)
            {
                if (cdata[cc].state == 1 && cdata[cc].relationship == 10)
                {
                    pet_exists = true;
                    break;
                }
            }
            if (!pet_exists)
            {
                if (petarenawin == 0)
                {
                    petarenawin = 2;
                }
            }
            if (petarenawin != 0)
            {
                quest_team_victorious();
                msg_halt();
                levelexitby = 4;
                snd(49);
                for (int cc = 0; cc < 16; ++cc)
                {
                    if (arenaop == 0 && followerin(cc) == 1
                        && cdata[cc].state == 6)
                        continue;
                    if (petarenawin != 1 && followerin(cc) == 1
                        && cdata[cc].state == 6 && rnd(5) == 0)
                        continue;
                    cdata[cc].state = followerexist(cc);
                }
                return turn_result_t::exit_map;
            }
        label_2744_internal:
            await(config::instance().wait1);
            cdata[0].direction = 0;
            key_check();
            f = 0;
            for (int cnt = 0; cnt < 16; ++cnt)
            {
                if (key == key_south || key == key_west)
                {
                    p = 15 - cnt;
                }
                else
                {
                    p = cnt;
                }
                if (cdata[p].state != 1)
                {
                    continue;
                }
                if (p == 0)
                {
                    continue;
                }
                if (cdata[p].relationship != 10)
                {
                    continue;
                }
                if (cdata[camera].state != 1 || camera == 0)
                {
                    camera = p;
                    break;
                }
                if (key == key_north || key == key_east)
                {
                    f = 1;
                    snd(5);
                    if (p > camera)
                    {
                        camera = p;
                        key = "";
                        break;
                    }
                }
                if (key == key_south || key == key_west)
                {
                    f = 1;
                    snd(5);
                    if (p < camera)
                    {
                        camera = p;
                        key = "";
                        break;
                    }
                }
            }
            if (f == 1)
            {
                key = ""s;
            }
            update_screen();
            if (key == key_goup || key_escape == 1)
            {
                txt(lang(
                    u8"試合を放棄する？"s,
                    u8"Do you want to give up the game?"s));
                ELONA_YES_NO_PROMPT();
                rtval = show_prompt(promptx, prompty, 160);
                if (rtval == 0)
                {
                    petarenawin = 2;
                    return turn_result_t::turn_end;
                }
                goto label_2744_internal;
            }
            if (key != key_cancel && key != ""s)
            {
                return turn_result_t::turn_end;
            }
            goto label_2744_internal;
        }
        if (trait(210) != 0 && rnd(5) == 0)
        {
            ci = get_random_inv(0);
            if (inv[ci].number > 0
                && the_item_db[inv[ci].id]->category == 52000)
            {
                dbid = inv[ci].id;
                access_item_db(15);
            }
        }
        if (trait(214) != 0 && rnd(250) == 0 && mdata(6) != 1)
        {
            efid = 408;
            magic();
        }
        if (cdata[cdata[0].enemy_id].is_invisible() == 1
            && cdata[0].can_see_invisible() == 0
            && cdata[cdata[0].enemy_id].wet == 0)
        {
            cdata[0].enemy_id = 0;
        }
        t = 1;
        keylog = "";
        key = "";
        objprm(0, ""s);
    }

label_2747:
    if (firstturn == 1)
    {
        if (gdata_catches_god_signal)
        {
            txtgod(cdata[0].god_id, 11);
        }
        firstturn = 0;
    }

    if (gdata(808))
    {
        txt(lang(u8"装備を変更した。"s, u8"You change your equipment."s));
        return turn_result_t::turn_end;
    }
    ++t;
    if (t % config::instance().scrsync == 1)
    {
        ++scrturn;
        label_1420();
    }

    if (config::instance().net && config::instance().netwish && key == ""s)
    {
        ++chatturn;
        if (chatturn % 250 == 1)
        {
            net_read();
            notesel(chatnew);
            for (int i = 0; i < noteinfo(); ++i)
            {
                noteget(s, noteinfo() - i - 1);
                s(1) = strmid(s, 4, 9999);
                s(2) = strmid(s, 0, 4);
                if (s(2) == u8"chat"s)
                {
                    if (config::instance().netchat)
                    {
                        continue;
                    }
                }
                if (en)
                {
                    cnv_str(s(1), u8"&quot;"s, u8"\""s);
                }
                txtef(6);
                txt(""s + s(1));
            }
        }
    }

    if (timeGetTime() / 1000 - time_warn > 3600)
    {
        time_warn = timeGetTime() / 1000;
        wishfilter = 0;
        ++hour_played;
        s = lang(
            u8"Elonaをはじめてから"s + hour_played
                + u8"時間が経過しています。"s,
            u8"You have been playing Elona for "s + hour_played + u8" hour"s
                + _s2(hour_played) + u8"."s);
        s += cheer_up_message(hour_played);
        txtef(5);
        txt(s);
    }

    await(config::instance().wait1);
    key_check(1);

    if (ginfo(2) != 0)
    {
        goto label_2747;
    }

    if (gdata_wizard)
    {
        if (getkey(snail::key::f5))
        {
            what_do_you_wish_for();
            return turn_result_t::turn_end;
        }
        if (getkey(snail::key::f6))
        {
            dbg_skipevent = 1;
            ++gdata_hour;
            label_2736();
            dbg_skipevent = 0;
            mode = 0;
            return turn_result_t::turn_end;
        }
        if (getkey(snail::key::f7))
        {
            if (mdata(6) != 3)
            {
                dbg_revealmap = 1;
                ++gdata_current_dungeon_level;
                txt(u8"lv:"s + gdata_current_dungeon_level);
                ctrl_file(file_operation_t::_11);
                mode = 2;
                levelexitby = 4;
                return turn_result_t::initialize_map;
            }
        }
    }

    if (key == key_quicksave)
    {
        key = "";
        do_save_game();
        txt(lang(u8" *保存* "s, u8" *Save* "s));
        await(100);
    }
    if (key == key_quickload)
    {
        key = "";
        msg_newline();
        msgtemp = u8"  "s;
        firstturn = 1;
        do_load_save_data();
        mode = 3;
        return turn_result_t::initialize_map;
    }

    if (getkey(snail::key::f3))
    {
        tcgmain();
        label_1746();
        update_entire_screen();
        return turn_result_t::turn_end;
    }
    if (getkey(snail::key::f9))
    {
        gmode(2);
        sxfix = 0;
        syfix = 0;
        label_1428();
        label_1429();
        label_1433();
        p = windoww / 192;
        for (int i = 0; i < p + 1; ++i)
        {
            if (i == p)
            {
                sx = windoww % 192;
            }
            else
            {
                sx = 192;
            }
            pos(i * 192, inf_msgy);
            gcopy(3, 496, 528, sx, 5);
        }
        redraw();
        press();
        update_entire_screen();
        snd(20);
        goto label_2747;
    }
    if (getkey(snail::key::f11))
    {
        dump_player_info();
        await(500);
        goto label_2747;
    }
    if (getkey(snail::key::f12))
    {
        debug::voldemort = true;
        if (debug::voldemort)
        {
            gdata_wizard = 1;
            for (int i = 400; i < 467; ++i)
            {
                if (i != 426 && i != 427)
                {
                    skillgain(0, i, 100, 10000);
                }
            }
            goto label_2747;
        }
        return do_debug_console();
    }
    if (key == ""s)
    {
        goto label_2747;
    }
    if (key == key_enter)
    {
        key = key_search;
        cell_featread(cdata[cc].position.x, cdata[cc].position.y);
        if (feat(1) == 11 || mdata(6) == 1)
        {
            key = key_godown;
        }
        if (feat(1) == 10)
        {
            key = key_goup;
        }
        if (feat(1) >= 24 && feat(1) <= 28)
        {
            key = key_search;
        }
        p = 0;
        for (const auto& ci : items(-1))
        {
            if (inv[ci].number == 0)
                continue;
            if (inv[ci].position != cdata[cc].position)
                continue;
            if (the_item_db[inv[ci].id]->category == 72000)
            {
                p = 1;
            }
            if (the_item_db[inv[ci].id]->subcategory == 60001)
            {
                p = 2;
            }
            if (the_item_db[inv[ci].id]->category == 60002)
            {
                p(0) = 3;
                p(1) = ci;
            }
            if (inv[ci].function != 0
                || the_item_db[inv[ci].id]->is_usable != 0)
            {
                p = 4;
            }
            if (the_item_db[inv[ci].id]->is_readable != 0)
            {
                p = 5;
            }
            if (inv[ci].id == 631)
            {
                key = key_godown;
            }
            if (inv[ci].id == 750 && gdata_current_map == 7)
            {
                key = key_goup;
            }
            if (inv[ci].id == 751 && gdata_current_map == 7)
            {
                key = key_godown;
            }
            if (inv[ci].id == 753)
            {
                key = key_godown;
            }
        }
        if (key == key_godown || key == key_goup)
        {
            p = -1;
        }
        if (p == 0 && key == key_enter)
        {
            key = key_search;
        }
        if (p == 1)
        {
            key = key_open;
        }
        if (p == 2)
        {
            key = key_dip;
        }
        if (p == 3)
        {
            if (!cdata[0].god_id.empty())
            {
                key = key_offer;
            }
            else
            {
                key = key_pray;
            }
        }
        if (p == 4)
        {
            key = key_use;
        }
        if (p == 5)
        {
            key = key_read;
        }
    }

    if (mousel == 1)
    {
        ematan(p, windoww / 2 - mousex, (windowh - inf_verh) / 2 - mousey);
        p = p * 360 / 255;
        if (p >= 338)
        {
            p = p - 360;
        }
        s(0) = key_south;
        s(1) = key_southwest;
        s(2) = key_west;
        s(3) = key_northwest;
        s(4) = key_north;
        s(5) = key_northeast;
        s(6) = key_east;
        s(7) = key_southeast;
        for (int cnt = 0; cnt < 8; ++cnt)
        {
            if (p <= cnt * 45 + 23 && p > cnt * 45 - 23)
            {
                key = s(cnt);
            }
        }
        await(100);
    }

    if (key != ""s)
    {
        const auto angband_result = check_angband();
        if (angband_result)
        {
            return *angband_result;
        }
    }
    if (key == key_quick)
    {
        show_quick_menu();
        update_screen();
    }
    if (key == u8"sc"s)
    {
        return do_short_cut_command();
    }
    menucycle = 1;
    if (key == key_quickinv)
    {
        invctrl = lastctrl;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (key == key_throw)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            invctrl = 26;
            snd(100);
            menu_result mr = ctrl_inventory();
            assert(mr.turn_result != turn_result_t::none);
            return mr.turn_result;
        }
    }
    if (key == key_inventory)
    {
        invctrl = 1;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (key == key_drop)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            invctrl = 2;
            snd(100);
            menu_result mr = ctrl_inventory();
            assert(mr.turn_result != turn_result_t::none);
            return mr.turn_result;
        }
    }
    if (key == key_eat)
    {
        invctrl = 5;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (key == key_read)
    {
        invctrl = 7;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (key == key_drink)
    {
        invctrl = 8;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (key == key_zap)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            invctrl = 9;
            snd(100);
            menu_result mr = ctrl_inventory();
            assert(mr.turn_result != turn_result_t::none);
            return mr.turn_result;
        }
    }
    if (key == key_use)
    {
        invctrl = 14;
        snd(100);
        menu_result mr = ctrl_inventory();
        assert(mr.turn_result != turn_result_t::none);
        return mr.turn_result;
    }
    if (key == key_open)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            invctrl = 15;
            snd(100);
            menu_result mr = ctrl_inventory();
            assert(mr.turn_result != turn_result_t::none);
            return mr.turn_result;
        }
    }
    if (key == key_dip)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            invctrl = 17;
            snd(100);
            return turn_result_t::ctrl_inventory;
        }
    }
    if (key == key_charainfo)
    {
        csctrl = 0;
        return turn_result_t::menu_character_sheet;
    }
    if (key == key_material)
    {
        return turn_result_t::menu_materials;
    }
    if (key == key_trait)
    {
        return turn_result_t::menu_feats;
    }
    if (key == key_wear)
    {
        return turn_result_t::menu_equipment;
    }
    if (key == key_cast)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            snd(102);
            return turn_result_t::show_spell_list;
        }
    }
    if (key == key_skill)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            snd(101);
            return turn_result_t::show_skill_list;
        }
    }
    if (key == key_msglog)
    {
        return turn_result_t::show_message_log;
    }
    if (key == key_journal)
    {
        return turn_result_t::menu_journal;
    }
    menucycle = 0;
    if (key == key_offer)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            invctrl = 19;
            snd(100);
            menu_result mr = ctrl_inventory();
            assert(mr.turn_result != turn_result_t::none);
            return mr.turn_result;
        }
    }
    if (key == key_help)
    {
        show_game_help();
        update_screen();
        goto label_2747;
    }
    if (key == key_rest)
    {
        return do_rest_command();
    }
    if (key == key_interact)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            return do_interact_command();
        }
    }
    if (key == key_target)
    {
        findlocmode = 1;
        target_position();
        findlocmode = 0;
        goto label_2747;
    }
    if (key == key_fire)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            return do_fire_command();
        }
    }
    if (key == key_give)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            return do_give_command();
        }
    }
    if (key == key_get || key == key_get2)
    {
        return do_get_command();
    }
    if (key == key_look)
    {
        if (mdata(6) != 1)
        {
            return do_look_command();
        }
        else
        {
            target_position();
            goto label_2747;
        }
    }
    if (key == key_save || key_escape == 1)
    {
        return do_exit_command();
    }
    if (key == key_dig)
    {
        return do_dig_command();
    }

    if (key == key_autodig)
    {
        foobar_save.is_autodig_enabled = !foobar_save.is_autodig_enabled;
        txt(i18n::_(
            u8"ui",
            u8"autodig",
            foobar_save.is_autodig_enabled ? u8"enabled" : u8"disabled"));
        goto label_2747;
    }

    if (key == key_bash)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            return do_bash_command();
        }
    }
    if (key == key_search)
    {
        return do_search_command();
    }
    if (key == key_close)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            return do_close_command();
        }
    }
    if (key == key_pray)
    {
        if (mdata(6) == 1)
        {
            txtnew();
            txt(lang(
                u8"その行為は、ワールドマップにいる間はできない。"s,
                u8"You can't do that while you're in a global area."s));
            display_msg();
            redraw();
            goto label_2747;
        }
        else
        {
            return do_pray_command();
        }
    }
    if (key == key_ammo)
    {
        return do_change_ammo_command();
    }
    if (key_tab)
    {
        label_1399();
        update_screen();
        goto label_2747;
    }
    p = 0;
    if (key == key_north)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x;
        cdata[0].next_position.y = cdata[0].position.y - 1;
        gdata(35) = 3;
        dirsub = 0;
    }
    if (key == key_south)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x;
        cdata[0].next_position.y = cdata[0].position.y + 1;
        gdata(35) = 0;
        dirsub = 4;
    }
    if (key == key_west)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x - 1;
        cdata[0].next_position.y = cdata[0].position.y;
        gdata(35) = 1;
        dirsub = 6;
    }
    if (key == key_east)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x + 1;
        cdata[0].next_position.y = cdata[0].position.y;
        gdata(35) = 2;
        dirsub = 2;
    }
    if (key == key_northwest)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x - 1;
        cdata[0].next_position.y = cdata[0].position.y - 1;
        gdata(35) = 3;
        dirsub = 7;
    }
    if (key == key_northeast)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x + 1;
        cdata[0].next_position.y = cdata[0].position.y - 1;
        gdata(35) = 3;
        dirsub = 1;
    }
    if (key == key_southwest)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x - 1;
        cdata[0].next_position.y = cdata[0].position.y + 1;
        gdata(35) = 0;
        dirsub = 5;
    }
    if (key == key_southeast)
    {
        p = 1;
        cdata[0].next_position.x = cdata[0].position.x + 1;
        cdata[0].next_position.y = cdata[0].position.y + 1;
        gdata(35) = 0;
        dirsub = 3;
    }
    cdata[0].direction = gdata(35);
    if (p == 1)
    {
        // Autodig
        int x = cdata[0].next_position.x;
        int y = cdata[0].next_position.y;
        if (foobar_save.is_autodig_enabled)
        {
            if (0 <= x && x < mdata(0) && 0 <= y && y < mdata(1)
                && (chipm(7, map(x, y, 0)) & 4) && chipm(0, map(x, y, 0)) != 3
                && mdata(6) != 1)
            {
                refx = x;
                refy = y;
                tlocx = x;
                tlocy = y;
                screenupdate = -1;
                update_screen();
                return do_dig_after_sp_check();
            }
        }
        return do_movement_command();
    }
    if (key == key_godown)
    {
        return do_use_stairs_command(1);
    }
    if (key == key_goup)
    {
        return do_use_stairs_command(2);
    }
    if (key == key_wait)
    {
        return turn_result_t::turn_end;
    }
    if (key == key_enter)
    {
        update_screen();
        goto label_2747;
    }
    if (key != ""s && key != key_cancel && key != key_alter)
    {
        ++msgdup;
        txt(lang(
            u8"?キーを押すと、コマンドの一覧が見られる。"s,
            u8"Hit ? key to display help."s));
        update_screen();
    }

    goto label_2747;
}




void conquer_lesimas()
{
    std::string wincomment;
    snd(51);
    play_music(-1);
    txt(lang(
        u8"信じられない！あなたはネフィアの迷宮「レシマス」を制覇した！"s,
        u8"Unbelievable! You conquered Lesimas!"s));
    update_screen();
    const auto win_words = txtsetwinword(3);
    for (int cnt = 0; cnt < 3; ++cnt)
    {
        ELONA_APPEND_PROMPT(win_words[cnt], key_select(cnt), ""s + promptmax);
    }
    rtval = show_prompt(promptx, prompty, 310, show_prompt_type::cannot_cancel);
    wincomment = ""s + promptl(0, rtval);
    mode = 7;
    screenupdate = -1;
    update_screen();
    if (jp)
    {
        msg_clear();
        txt(u8"「お前がここに辿り着くことは」台座から、何かの声が聞こえる。"s);
        flt();
        chara_create(-1, 23, cdata[0].position.x, cdata[0].position.y);
        play_music(69);
        msg_halt();
        msg_clear();
        txt(u8"「決まっていたことなのだ…遅かれ早かれな」"s);
        txt(u8"部屋の空気が突然緊張し、あなたの前に端麗な青年が現れた。"s);
        txt(u8"「我々からすれば、複雑性の一面に過ぎないが、人間は運命とでも呼ぶのだろう？」"s);
        msg_halt();
        msg_clear();
        txt(u8"あなたは懸命に脚の震えを抑えようとしたが、難しかった。"s);
        txt(u8"華奢に見える幼顔の男の影は、人のものではない。"s);
        txt(u8"あどけない瞳の奥に、あなたは底知れない力と闇を感じた。"s);
        msg_halt();
        msg_clear();
        txt(u8"「ネフィアの永遠の盟約に基づき」青年は台座の横の死体を指し、皮肉な微笑を送った。"s);
        txt(u8"「この哀れな老人が守っていたものは、今からお前のものだ」"s);
        msg_halt();
        msg_clear();
        txt(u8"あなたは、台座の上に置かれている絢爛な装飾の本を、いぶかしげに眺めた。"s);
        msg_halt();
        tc = chara_find(23);
        talk_to_npc();
        msg_clear();
        txt(u8"青年は悪戯っぽくニヤリと笑い、壁に寄りかかった。"s);
        msg_halt();
        msg_clear();
        chara_vanquish(chara_find(23));
        screenupdate = -1;
        update_screen();
        txt(u8"…どれくらい時間がたっただろう。氷の瞳の男は、いつの間にか姿を消していた。あなたは不安を振り払い、ゆっくりと本に手を伸ばした…"s);
        msg_halt();
    }
    mode = 0;
    play_music(71);
    label_1442();
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"void.bmp", 1);
    pos(0, 0);
    gzoom(4, 0, 0, 640, 480, windoww, windowh);
    gsel(0);
    label_1443();
    pos(0, 0);
    gcopy(4, 0, 0, windoww, windowh);
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"g1.bmp", 1);
    gsel(0);
    s = lang(
        ""s + cdatan(1, 0) + cdatan(0, 0)
            + u8"に祝福あれ！あなたは遂にレシマスの秘宝を手にいれた！"s,
        u8"Blessing to "s + cdatan(0, 0) + u8", "s + cdatan(1, 0)
            + u8"! You've finally acquired the codex!"s);
    draw_caption();
    s(0) = lang(u8"*勝利*"s, u8"*Win*"s);
    s(1) = ""s + strhint3;
    windowshadow = 1;
    ww = 680;
    wh = 488;
    pagesize = 0;
    display_window(windoww / 2 - ww / 2, windowh / 2 - wh / 2, ww, wh);
    cmbg = 0;
    x = ww / 3 - 20;
    y = wh - 140;
    gmode(4, 180, 300, 250);
    pos(wx + ww - 120, wy + wh / 2);
    grotate(4, cmbg / 4 % 4 * 180, cmbg / 4 / 4 % 2 * 300, 0, x, y);
    gmode(2);
    display_topic(lang(u8"制覇までの軌跡"s, u8"Trace"s), wx + 28, wy + 40);
    font(14 - en * 2);
    pos(wx + 40, wy + 76);
    mes(lang(
        ""s + 517 + u8"年"s + 8 + u8"月"s + 12
            + u8"日に、あなたはノースティリスに到着した。"s,
        u8"In the year "s + 517 + u8", "s + 12 + u8"/"s + 8
            + u8", you arrived at North Tyris."s));
    pos(wx + 40, wy + 116);
    mes(lang(
        u8"最深で"s + gdata_deepest_dungeon_level + u8"階相当まで到達し、"s
            + gdata_kill_count + u8"匹の敵を殺して、"s,
        u8"You've killed "s + gdata_kill_count
            + u8" creatures and reached\nmaximum of "s
            + cnvrank(gdata_deepest_dungeon_level) + u8" level of dungeons."s));
    pos(wx + 40, wy + 146);
    mes(lang(
        u8"現在"s + calcscore() + u8"点のスコアを叩き出している。"s,
        u8"Your score is "s + calcscore() + u8" points now."s));
    pos(wx + 40, wy + 186);
    mes(lang(
        ""s + gdata_year + u8"年"s + gdata_month + u8"月"s + gdata_day
            + u8"日にレシマスを制覇して、"s,
        u8"In the year "s + gdata_year + u8", "s + gdata_day + u8"/"s
            + gdata_month + u8", you conquered Lesimas."s));
    pos(wx + 40, wy + 206);
    mes(lang(
        u8"あなたは「"s + wincomment + u8"」とコメントした。"s,
        u8"Upon killing Zeome, you said, \""s + wincomment + u8"\""));
    pos(wx + 40, wy + 246);
    mes(lang(
        u8"…あなたの旅はまだ終わらない。"s, u8"Your journey continues..."s));
    redraw();
    key_list = key_enter;
    keyrange = 0;

    while (1)
    {
        await(config::instance().wait1);
        key_check();
        cursor_check();
        if (key == key_cancel)
        {
            play_the_last_scene_again();
            return;
        }
    }
}


void play_the_last_scene_again()
{
    update_entire_screen();
    txt(lang(
        u8"達成のシーンをもう一度再現する？"s,
        u8"You want to watch this event again?"s));
    ELONA_YES_NO_PROMPT();
    rtval = show_prompt(promptx, prompty, 160);
    if (rtval == 0)
    {
        conquer_lesimas();
        return;
    }
    gdata_main_quest_flag = 180;
    update_screen();
    return;
}




turn_result_t pc_died()
{
    cc = 0;
    snd(50);
    screenupdate = -1;
    update_screen();
    if (gdata_executing_immediate_quest_type)
    {
        return quest_pc_died_during_immediate_quest();
    }
    txt(lang(u8"さようなら… "s, u8"Good bye... "s));
    txt(lang(u8"遺言は？"s, u8"You leave a dying message."s));
    inputlog = "";
    input_text_dialog((windoww - 310) / 2 + inf_screenx, winposy(90), 16);
    if (inputlog == ""s)
    {
        txtsetlastword();
    }
    else
    {
        lastword = lang(u8"「"s, u8"\""s) + inputlog + lang(u8"」"s, u8"\""s);
    }
    buff = "";
    notesel(buff);
    const auto bone_filepath = filesystem::dir::save() / u8"bone.txt";
    if (fs::exists(bone_filepath))
    {
        std::ifstream in{bone_filepath.native(), std::ios::binary};
        std::string tmp;
        while (std::getline(in, tmp))
        {
            buff(0) += tmp + '\n';
        }
    }
    s = cdatan(1, cc) + u8" "s + cdatan(0, cc) + lang(""s, u8" "s) + lastword;
    lenfix(s, 60);
    s += lang(
        ""s + gdata_year + u8"年"s + gdata_month + u8"月"s + gdata_day
            + u8"日"s,
        ""s + gdata_year + u8"/"s + gdata_month + u8"/"s + gdata_day);
    noteadd(""s, 0);
    noteadd(s, 1);
    s = lang(
        mdatan(0) + u8"で"s + ndeathcause,
        cnven(ndeathcause) + u8" in "s + mdatan(0) + u8"."s);
    noteadd(s, 2);
    s = ""s + calcscore() + u8","s + cdata[cc].image;
    noteadd(s, 3);
    if (noteinfo() >= 320)
        for (int cnt = 320, cnt_end = cnt + (noteinfo() - 320); cnt < cnt_end;
             ++cnt)
        {
            notedel(320);
        }
    page = 0;
    while (1)
    {
        p(1) = 0;
        for (int cnt = 0; cnt < 79; ++cnt)
        {
            p = cnt * 4;
            noteget(s(10), p + 3);
            csvsort(s, s(10), 44);
            p(2) = elona::stoi(s(0));
            noteget(s(10), p + 7);
            csvsort(s, s(10), 44);
            p(3) = elona::stoi(s(0));
            if (p(3) > p(2))
            {
                if (cnt == page)
                {
                    page = cnt + 1;
                }
                else if (cnt + 1 == page)
                {
                    page = cnt;
                }
                for (int cnt = 1; cnt < 4; ++cnt)
                {
                    noteget(s, p + cnt);
                    noteget(s(1), p + 4 + cnt);
                    noteadd(s, p + 4 + cnt, 1);
                    noteadd(s(1), p + cnt, 1);
                }
                p(1) = 1;
            }
        }
        if (p(1) == 0)
        {
            break;
        }
    }
    for (int cnt = 0, cnt_end = (noteinfo() / 4); cnt < cnt_end; ++cnt)
    {
        noteadd(""s + cnvrank((cnt + 1)) + lang(u8"位"s, ""s), cnt * 4, 1);
    }
    {
        std::ofstream out{bone_filepath.native(), std::ios::binary};
        out << buff(0) << std::endl;
    }
    gsel(4);
    pos(0, 0);
    picload(filesystem::dir::graphic() / u8"void.bmp", 1);
    gsel(0);
    show_game_score_ranking();
    s = lang(
        u8"あともう少しで埋葬される…"s, u8"You are about to be buried..."s);
    draw_caption();
    ELONA_APPEND_PROMPT(
        lang(u8"這い上がる"s, u8"Crawl up"s), u8"a"s, ""s + promptmax);
    ELONA_APPEND_PROMPT(
        lang(u8"埋まる"s, u8"Lie on your back"s), u8"b"s, ""s + promptmax);
    rtval = show_prompt(promptx, 100, 240);
    if (rtval == 1)
    {
        show_game_score_ranking();
        s = lang(
            u8"あなたは埋められた。さようなら…（キーを押すと終了します）"s,
            u8"You have been buried. Bye...(Hit any key to exit)"s);
        draw_caption();
        redraw();
        press();
        return turn_result_t::finish_elona;
    }
    s = u8"dead"s
        + lang(
              (cdatan(1, 0) + cdatan(0, 0) + u8"は"s + mdatan(0) + u8"で"s
               + ndeathcause + lastword),
              (cdatan(1, 0) + u8" "s + cdatan(0, 0) + u8" "s + ndeathcause
               + u8" in "s + mdatan(0) + u8" "s + lastword));
    if (gdata_wizard == 0)
    {
        net_send(s);
    }
    screenupdate = -1;
    update_entire_screen();
    levelexitby = 3;
    return turn_result_t::exit_map;
}



void show_game_score_ranking()
{
    notesel(buff);
    gmode(0);
    pos(0, 0);
    gzoom(4, 0, 0, 800, 600, windoww, windowh);
    gmode(2);
    x = 135;
    y = 134;
    font(14 - en * 2);
    p = page - 4;
    if (p >= 80)
    {
        p = 72;
    }
    if (p < 0)
    {
        p = 0;
    }
    color(138, 131, 100);
    for (int cnt = p, cnt_end = cnt + (8); cnt < cnt_end; ++cnt)
    {
        p = cnt * 4;
        y += 46;
        if (cnt == page)
        {
            s = u8"New!"s;
        }
        else
        {
            s = u8" "s + cnvrank((cnt + 1)) + lang(u8"位"s, ""s);
        }
        pos(x - 80, y + 10);
        color(10, 10, 10);
        mes(s);
        color(0, 0, 0);
        bool no_entry = false;
        if (p >= noteinfo())
        {
            no_entry = true;
        }
        else
        {
            noteget(s, p + 1);
            if (s(0).empty())
            {
                no_entry = true;
            }
        }
        pos(x, y);
        color(10, 10, 10);
        if (no_entry)
        {
            mes(u8"no entry");
            color(0, 0, 0);
            continue;
        }
        mes(s);
        noteget(s, p + 2);
        pos(x, y + 20);
        mes(s);
        noteget(s(10), p + 3);
        csvsort(s, s(10), 44);
        pos(x + 480, y + 20);
        mes(""s + s + lang(u8"点"s, ""s));
        p = elona::stoi(s(1)) % 1000;
        chara_preparepic(p);
        pos(x - 22, y + 12);
        gmode(2, chipc(2, p), chipc(3, p));
        grotate(
            5,
            0,
            960,
            0,
            chipc(2, p) / (1 + (chipc(3, p) > inf_tiles)),
            inf_tiles);
        color(0, 0, 0);
    }
    color(0, 0, 0);
    return;
}


} // namespace elona
