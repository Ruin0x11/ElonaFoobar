#include "audio.hpp"
#include "config.hpp"
#include "elona.hpp"
#include "macro.hpp"
#include "variables.hpp"

using namespace elona;


namespace
{


class config_menu_item_base
{
public:
    std::string name;

    config_menu_item_base(const std::string& name)
        : name(name)
    {
    }

    virtual ~config_menu_item_base() noexcept = default;
};


class config_menu_item_yesno : public config_menu_item_base
{
public:
    int& variable;
    std::string yes;
    std::string no;

    config_menu_item_yesno(
        const std::string& name,
        int& variable,
        const std::string& yes,
        const std::string& no)
        : config_menu_item_base(name)
        , variable(variable)
        , yes(yes)
        , no(no)
    {
    }

    virtual ~config_menu_item_yesno() noexcept = default;
};


class config_menu_item_info : public config_menu_item_base
{
public:
    std::string info;

    config_menu_item_info(const std::string& name, const std::string& info)
        : config_menu_item_base(name)
        , info(info)
    {
    }

    virtual ~config_menu_item_info() noexcept = default;
};


class config_menu_item_integer : public config_menu_item_base
{
public:
    int& variable;
    std::string text;

    config_menu_item_integer(
        const std::string& name,
        int& variable,
        const std::string& text)
        : config_menu_item_base(name)
        , variable(variable)
        , text(text)
    {
    }


    std::string get_text()
    {
        auto ret{text};
        ret.replace(
            ret.find(marker), std::strlen(marker), std::to_string(variable));
        return ret;
    }

    virtual ~config_menu_item_integer() noexcept = default;


private:
    static constexpr const char* marker = u8"{}";
};


class config_menu_item_choice : public config_menu_item_base
{
public:
    int& variable;
    std::vector<std::string> texts;

    config_menu_item_choice(
        const std::string& name,
        int& variable,
        const std::vector<std::string>& texts)
        : config_menu_item_base(name)
        , variable(variable)
        , texts(texts)
    {
    }


    std::string get_text()
    {
        return texts[variable];
    }

    virtual ~config_menu_item_choice() noexcept = default;
};


class config_menu
{
public:
    std::string title;
    std::vector<std::unique_ptr<config_menu_item_base>> items;
    int width;
    int height;

    config_menu(const std::string& title, int width, int height)
        : title(title)
        , width(width)
        , height(height)
    {
    }
};



std::vector<config_menu> create_config_menu()
{
    std::vector<config_menu> ret;

#define ELONA_CONFIG_ITEM(name) \
    ret.back().items.emplace_back(std::make_unique<config_menu_item_base>(name))

#define ELONA_CONFIG_ITEM_YESNO(name, var, yes, no) \
    ret.back().items.emplace_back( \
        std::make_unique<config_menu_item_yesno>(name, var, yes, no))

#define ELONA_CONFIG_ITEM_INFO(name, info) \
    ret.back().items.emplace_back( \
        std::make_unique<config_menu_item_info>(name, info))

#define ELONA_CONFIG_ITEM_INTEGER(name, var, text) \
    ret.back().items.emplace_back( \
        std::make_unique<config_menu_item_integer>(name, var, text))

#define ELONA_CONFIG_ITEM_CHOICE(name, var, ...) \
    ret.back().items.emplace_back(std::make_unique<config_menu_item_choice>( \
        name, var, std::vector<std::string>{__VA_ARGS__}))

    ret.emplace_back(lang(u8"オプション", u8"Option"), 370, 285);
    ELONA_CONFIG_ITEM(lang(u8"ゲームの設定", u8"Game Setting"));
    ELONA_CONFIG_ITEM(lang(u8"画面と音の設定", u8"Screen & Sound"));
    ELONA_CONFIG_ITEM(lang(u8"ネット機能の設定", u8"Network Setting"));
    ELONA_CONFIG_ITEM(lang(u8"詳細な設定", u8"Detailed Setting"));
    ELONA_CONFIG_ITEM(lang(u8"ゲームパッド", u8"Game Pad"));
    ELONA_CONFIG_ITEM(lang(u8"メッセージとログ", u8"Message & Log"));
    ELONA_CONFIG_ITEM(lang(u8"言語(Language)", u8"Language"));
    ELONA_CONFIG_ITEM(lang(u8"拡張設定(Foobar)", u8"Ex setting(Foobar)"));

    ret.emplace_back(lang(u8"ゲームの設定", u8"Game Setting"), 440, 340);
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"ノルンの冒険ガイド", u8"Extra Help"),
        config::instance().extrahelp,
        lang(u8"あり", u8"Show"),
        lang(u8"なし", u8"Don't show"));
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"非好戦的NPCを無視", u8"Neutral Npcs"),
        config::instance().ignoredislike,
        lang(u8"する", u8"Ignore"),
        lang(u8"しない", u8"Attack"));
    ELONA_CONFIG_ITEM_CHOICE(
        lang(u8"zキーの割当て", u8"Assign z key"),
        config::instance().zkey,
        lang(u8"ｸｨｯｸﾒﾆｭｰ", u8"Quick menu"),
        lang(u8"道具を振る", u8"Zap"),
        lang(u8"割当なし", u8"Don't assign"), );
    ELONA_CONFIG_ITEM_CHOICE(
        lang(u8"xキーの割当て", u8"Assign x key"),
        config::instance().xkey,
        lang(u8"ｸｲｯｸｲﾝﾍﾞﾝﾄﾘ", u8"Quick Inv"),
        lang(u8"道具を調べる", u8"Identify"),
        lang(u8"割当なし", u8"Don't assign"), );
    ELONA_CONFIG_ITEM(lang(u8"走り始める歩数", u8"Start Running After"));
    ELONA_CONFIG_ITEM_INTEGER(
        lang(u8"歩きの速さ", u8"Walk Speed"),
        config::instance().walkwait,
        u8"{} wait");
    ELONA_CONFIG_ITEM_INTEGER(
        lang(u8"攻撃の間隔", u8"Attack Interval"),
        config::instance().attackwait,
        u8"{} wait");

    ret.emplace_back(lang(u8"画面と音の設定", u8"Screen & Sound"), 440, 370);
    ELONA_CONFIG_ITEM(lang(u8"サウンドの再生*", u8"Sound*"));
    ELONA_CONFIG_ITEM(lang(u8"midiの再生*", u8"Music*"));
    ELONA_CONFIG_ITEM(lang(u8"画面モード*", u8"Screen Mode*"));
    ELONA_CONFIG_ITEM(lang(u8"画面の大きさ*", u8"Screen Resolution*"));
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"スクロール", u8"Smooth Scroll"),
        config::instance().scroll,
        lang(u8"する", u8"Yes"),
        lang(u8"しない", u8"No"));
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"主人公中心に描写", u8"Always Center"),
        config::instance().alwayscenter,
        lang(u8"する", u8"Yes"),
        lang(u8"しない", u8"No"));
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"心臓の音", u8"Heartbeat Sound"),
        config::instance().heart,
        lang(u8"再生する", u8"Play"),
        lang(u8"再生しない", u8"Don't play"));
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"攻撃時アニメ", u8"Attack Animation"),
        config::instance().attackanime,
        lang(u8"アニメあり", u8"Yes"),
        lang(u8"アニメなし", u8"No"));
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"天候エフェクト", u8"Weather Effect"),
        config::instance().env,
        lang(u8"アニメあり", u8"Always"),
        lang(u8"アニメなし", u8"No animation"));
    ELONA_CONFIG_ITEM_CHOICE(
        lang(u8"光源の描写", u8"Lighting"),
        config::instance().shadow,
        lang(u8"高画質", u8"High"),
        lang(u8"低画質", u8"Low"), );
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"アイテムの影描写", u8"object Shadow"),
        config::instance().objectshadow,
        lang(u8"あり（低速）", u8"Yes(Fast)"),
        lang(u8"なし（高速）", u8"No(Slow)"));

    ret.emplace_back(lang(u8"ネット機能の設定", u8"Network Setting"), 440, 300);
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"ネットの使用", u8"Use Network"),
        config::instance().net,
        lang(u8"する", u8"Yes"),
        lang(u8"しない", u8"No"));
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"定期的に接続", u8"Constantly Connect"),
        config::instance().netwish,
        lang(u8"する", u8"Yes"),
        lang(u8"しない", u8"No"));
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"チャットをフィルタ", u8"Filter Chat"),
        config::instance().netchat,
        lang(u8"する", u8"Yes"),
        lang(u8"しない", u8"No"));

    ret.emplace_back(lang(u8"詳細な設定", u8"Detailed Setting"), 440, 300);
    ELONA_CONFIG_ITEM_INTEGER(
        lang(u8"走りの速さ", u8"Run Speed"),
        config::instance().runwait,
        u8"{} wait");
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"numlockを自動制御", u8"Auto Numlock"),
        config::instance().autonumlock,
        lang(u8"する", u8"Yes"),
        lang(u8"しない", u8"No"));
    ELONA_CONFIG_ITEM_INFO(
        lang(u8"タイトルの水の波紋", u8"Title Water Effect"),
        lang(u8"オフ(未実装)", u8"No(unsupported)"));
    ELONA_CONFIG_ITEM_INTEGER(
        lang(u8"画面の更新頻度", u8"Screen Refresh"),
        config::instance().scrsync,
        u8"{} wait");
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"走り時スクロール", u8"Scroll When Run"),
        config::instance().runscroll,
        lang(u8"する", u8"Yes"),
        lang(u8"しない", u8"No"));
    ELONA_CONFIG_ITEM_CHOICE(
        lang(u8"オートターンの挙動", u8"Auto Turn Speed"),
        config::instance().autoturn,
        lang(u8"普通", u8"Normal"),
        lang(u8"速め", u8"High"),
        lang(u8"省略", u8"Highest"));
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"イベントの短縮", u8"Skip Random Events"),
        config::instance().skiprandevents,
        lang(u8"する", u8"Yes"),
        lang(u8"しない", u8"No"));

    ret.emplace_back(lang(u8"ゲームパッド", u8"Game Pad"), 440, 430);
    ELONA_CONFIG_ITEM(lang(u8"ゲームパッド", u8"Game Pad"));
    ELONA_CONFIG_ITEM(lang(u8"決定キー", u8"Enter/Ok"));
    ELONA_CONFIG_ITEM(lang(u8"キャンセルキー", u8"Cancel"));
    ELONA_CONFIG_ITEM(lang(u8"所持品", u8"Inventory"));
    ELONA_CONFIG_ITEM(lang(u8"行動", u8"Action"));
    ELONA_CONFIG_ITEM(lang(u8"斜め移動(L)", u8"diagonal Move/(L)"));
    ELONA_CONFIG_ITEM(lang(u8"拾う(R)", u8"Pick/(R)"));
    ELONA_CONFIG_ITEM(lang(u8"射撃(R)", u8"Shoot/(R)"));
    ELONA_CONFIG_ITEM(lang(u8"目標(L)", u8"Target/(L)"));
    ELONA_CONFIG_ITEM(lang(u8"ポーズメニュー", u8"Pause/Menu"));
    ELONA_CONFIG_ITEM(lang(u8"ヘルプ", u8"Help"));
    ELONA_CONFIG_ITEM(lang(u8"キャラ情報", u8"Chara-sheet"));
    ELONA_CONFIG_ITEM(lang(u8"装填", u8"Reload"));

    ret.emplace_back(lang(u8"メッセージとログ", u8"Message&Log"), 440, 300);
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"ﾒｯｾｰｼﾞに分表示追加", u8"Add time info"),
        config::instance().msgaddtime,
        lang(u8"する", u8"Yes"),
        lang(u8"しない", u8"No"));
    ELONA_CONFIG_ITEM(lang(u8"過去のﾒｯｾｰｼﾞの透過", u8"Transparency"));

    ret.emplace_back(lang(u8"言語(Language)", u8"Language"), 440, 300);
    ELONA_CONFIG_ITEM_CHOICE(
        lang(u8"言語*", u8"Language*"),
        config::instance().language,
        u8"Japanese",
        u8"English");

    ret.emplace_back(
        lang(u8"拡張設定(Foobar)", u8"Ex setting(Foobar)"), 440, 300);
    ELONA_CONFIG_ITEM_CHOICE(
        lang(u8"ペットのHPバー", u8"Pets' HP bar"),
        config::instance().hp_bar,
        lang(u8"表示しない", u8"Don't show"),
        lang(u8"左側に表示", u8"Show left side"),
        lang(u8"右側に表示", u8"Show right side"));
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"紐のアイコン表示", u8"Leash icon"),
        config::instance().leash_icon,
        lang(u8"する", u8"Show"),
        lang(u8"しない", u8"Don't show"));
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"Autopick", u8"Autopick"),
        config::instance().use_autopick,
        lang(u8"使う", u8"On"),
        lang(u8"使わない", u8"Off"));
    ELONA_CONFIG_ITEM_YESNO(
        lang(u8"オートセーブ", u8"Autosave"),
        config::instance().autosave,
        lang(u8"有効", u8"On"),
        lang(u8"無効", u8"Off"));

#undef ELONA_CONFIG_ITEM
#undef ELONA_CONFIG_ITEM_YESNO
#undef ELONA_CONFIG_ITEM_INFO
#undef ELONA_CONFIG_ITEM_INTEGER
#undef ELONA_CONFIG_ITEM_CHOICE

    return ret;
}



} // namespace



namespace elona
{

int submenu = 0;


void set_option()
{
    int cfg_sound2 = config::instance().sound;
    int cfg_music2 = config::instance().music;
    int cfg_fullscreen2 = config::instance().fullscreen;
    int windoww2 = windoww;
    int windowh2 = windowh;

    const auto config_menu_definitions = create_config_menu();

    listmax = 0;
    page = 0;
    pagesize = 18;
    cs = 0;
    cc = 0;
    cs_bk = -1;
    page_bk = 0;
    cs_bk2 = 0;

    gsel(mode == 10 ? 2 : 4);
    for (int i = 0; i < 8; ++i)
    {
        pos(i % 4 * 180, i / 4 * 300);
        picload(filesystem::dir::graphic() / (u8"g"s + (i + 1) + u8".bmp"), 1);
    }
    gsel(0);

    if (mode == 0)
    {
        screenupdate = -1;
        update_screen();
    }
    if (mode == 10)
    {
        gsel(4);
        gmode(0);
        pos(0, 0);
        picload(filesystem::dir::graphic() / u8"title.bmp", 1);
        gzoom(4, 0, 0, 800, 600, windoww, windowh);
        gsel(0);
        gmode(0);
        pos(0, 0);
        gcopy(4, 0, 0, windoww, windowh);
        gmode(2);
    }

    const auto& menu_def = config_menu_definitions[submenu];
    const auto menu_title = menu_def.title;
    const auto width = menu_def.width;
    const auto height = menu_def.height;

    for (const auto& menu_item : menu_def.items)
    {
        list(0, listmax) = listmax;
        listn(0, listmax) = menu_item->name;
        ++listmax;
    }

    windowshadow = 1;

    bool reset_page = true;
    while (1)
    {
        if (reset_page)
        {
            if (config::instance().zkey == 0)
            {
                key_quick = u8"z"s;
                key_zap = u8"Z"s;
            }
            else
            {
                key_zap = u8"z"s;
                key_quick = u8"Z"s;
            }

            if (config::instance().xkey == 0)
            {
                key_quickinv = u8"x"s;
                key_inventory = u8"X"s;
            }
            else
            {
                key_inventory = u8"x"s;
                key_quickinv = u8"X"s;
            }

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
            reset_page = false;
        }
        s(0) = menu_title;
        s(1) = strhint3;
        pagesize = 0;
        if (mode == 1)
        {
            display_window(
                (windoww - width) / 2 + inf_screenx,
                winposy(height, 1),
                width,
                height);
        }
        else
        {
            display_window(
                (windoww - width) / 2 + inf_screenx,
                winposy(height) - 12,
                width,
                height);
        }
        pagesize = listmax;
        display_topic(lang(u8"項目"s, u8"Menu"s), wx + 34, wy + 36);
        if (mode == 10)
        {
            p = 2;
        }
        else
        {
            p = 4;
        }
        x = ww / 5 * 3;
        y = wh - 80;
        gmode(4, 180, 300, 50);
        pos(wx + ww / 3, wy + wh / 2);
        grotate(p, cmbg / 4 % 4 * 180, cmbg / 4 / 4 % 2 * 300, 0, x, y);
        gmode(2);
        keyrange = 0;
        for (int cnt = 0, cnt_end = (pagesize); cnt < cnt_end; ++cnt)
        {
            p = pagesize * page + cnt;
            if (p >= listmax)
            {
                break;
            }
            if (submenu == 0)
            {
                key_list(cnt) = key_select(cnt);
                ++keyrange;
                display_key(wx + 32, wy + 66 + cnt * 19 - 2, cnt);
            }
            else
            {
                key_list(cnt) = key_cancel;
                ++keyrange;
            }
        }
        if (submenu == 5)
        {
            p(0) = 0;
            p(1) = cnvjkey(key_enter);
            p(2) = cnvjkey(key_cancel);
            p(3) = cnvjkey(key_quickinv);
            p(4) = cnvjkey(key_quick);
            p(5) = cnvjkey(key_alter);
            p(6) = cnvjkey(key_get);
            p(7) = cnvjkey(key_fire);
            p(8) = cnvjkey(key_target);
            p(9) = cnvjkey(key_esc);
            p(10) = cnvjkey(key_help);
            p(11) = cnvjkey(key_charainfo);
            p(12) = cnvjkey(key_ammo);
            for (int cnt = 1; cnt < 20; ++cnt)
            {
                list(1, cnt) = p(cnt);
            }
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
            s = listn(0, p);
            if (submenu == 0)
            {
                x = 8;
            }
            else
            {
                x = 0;
            }
            if (submenu == 3)
            {
                if (config::instance().net == 0)
                {
                    if (cnt >= 1)
                    {
                        continue;
                    }
                }
            }
            cs_list(cs == cnt, s, wx + 56 + x, wy + 66 + cnt * 19 - 1, 0);
            if ((submenu != 5 || cnt <= 0) && submenu != 0)
            {
                pos(wx + 220, wy + 66 + cnt * 19 - 5);
                gcopy(3, 312, 336, 24, 24);
                pos(wx + 358, wy + 66 + cnt * 19 - 5);
                gcopy(3, 336, 336, 24, 24);
            }
            pos(wx + 250, wy + 66 + cnt * 19);
            if (auto ptr = dynamic_cast<config_menu_item_yesno*>(
                    config_menu_definitions[submenu].items[cnt].get()))
            {
                mes(ptr->variable ? ptr->yes : ptr->no);
            }
            else if (
                auto ptr = dynamic_cast<config_menu_item_info*>(
                    config_menu_definitions[submenu].items[cnt].get()))
            {
                mes(ptr->info);
            }
            else if (
                auto ptr = dynamic_cast<config_menu_item_integer*>(
                    config_menu_definitions[submenu].items[cnt].get()))
            {
                mes(ptr->get_text());
            }
            else if (
                auto ptr = dynamic_cast<config_menu_item_choice*>(
                    config_menu_definitions[submenu].items[cnt].get()))
            {
                mes(ptr->get_text());
            }
            else if (submenu == 1)
            {
                if (cnt == 4)
                {
                    if (config::instance().startrun >= 20)
                    {
                        mes(lang(u8"走らない"s, u8"Don't run"s));
                    }
                    else
                    {
                        mes(lang(
                            ""s + (config::instance().startrun + 1)
                                + u8"歩目から"s,
                            u8"After "s + (config::instance().startrun + 1)
                                + u8" steps"s));
                    }
                }
            }
            else if (submenu == 2)
            {
                if (cnt == 0)
                {
                    s(0) = lang(u8"なし"s, u8"Off"s);
                    s(1) = lang(u8"あり"s, u8"On"s);
                    mes(s(cfg_sound2));
                }
                if (cnt == 1)
                {
                    s(0) = lang(u8"なし"s, u8"None"s);
                    s(1) = lang(u8"direct music"s, u8"Direct music"s);
                    s(2) = lang(u8"MCI"s, u8"MCI"s);
                    mes(s(cfg_music2));
                }
                if (cnt == 2)
                {
                    s(0) = lang(u8"ウィンドウ"s, u8"Window mode"s);
                    s(1) = lang(u8"フルスクリーン"s, u8"Full screen"s);
                    mes(s(cfg_fullscreen2));
                }
                if (cnt == 3)
                {
                    mes(""s + windoww2 + u8" * "s + windowh2);
                }
            }
            else if (submenu == 5)
            {
                if (cnt == 0)
                {
                    s(0) = lang(u8"使用しない"s, u8"Don't use"s);
                    s(1) = lang(u8"使用する"s, u8"Use"s);
                    mes(s(config::instance().joypad));
                }
                else if (list(1, cnt) == -1)
                {
                    mes(lang(u8"未設定"s, u8"Unassigned"s));
                }
                else
                {
                    mes(lang(u8"ボタン"s, u8"Button"s) + list(1, cnt));
                }
            }
            else if (submenu == 6)
            {
                if (cnt == 1)
                {
                    mes(""s + config::instance().msgtrans * 10 + u8" %"s);
                }
            }
        }
        if (submenu != 0)
        {
            if (submenu != 5)
            {
                pos(wx + 40, wy + wh - 70);
                font(12 + sizefix - en * 2);
                if (jp)
                {
                    mes(u8"* 印のついた項目は、ゲームの再起動後に適用されます"s);
                }
                else
                {
                    mes(u8"Items marked with * require restart to apply changes."s);
                }
            }
        }
        if (submenu == 5)
        {
            pos(wx + 40, wy + wh - 110);
            font(12 + sizefix - en * 2);
            if (jp)
            {
                mes(u8"ボタンを割り当てたい項目にカーソルをあわせて\nゲームパッドのボタンを押してください。(L),(R)の付いている\n項目は、メニュー画面でタブの移動に使われます。"s);
            }
            else
            {
                mes(u8"To assign a button, move the cursor to\nan item and press the button."s);
            }
        }
        if (keyrange != 0)
        {
            cs_bk = cs;
        }
        redraw();
        await(config::instance().wait1);
        key_check();
        cursor_check();
        ELONA_GET_SELECTED_ITEM(p, cs = i);
        if (p != -1)
        {
            if (submenu == 0)
            {
                submenu = p + 1;
                snd(20);
                set_option();
                return;
            }
        }
        if (submenu == 5)
        {
            if (cs > 0)
            {
                int j0 = 0;
                DIGETJOYSTATE(j0, 0);
                f = 0;
                for (int cnt = 0; cnt < 12; ++cnt)
                {
                    if (HMMBITCHECK(j0, 4 + cnt))
                    {
                        p = cnt;
                        for (int cnt = 0; cnt < 12; ++cnt)
                        {
                            jkey(cnt) = "";
                        }
                        list(1, cs) = p;
                        for (int cnt = 1; cnt < 20; ++cnt)
                        {
                            if (cs == cnt)
                            {
                                continue;
                            }
                            --list(1, cnt);
                            if (list(1, cnt) == p || list(1, cnt) <= -1)
                            {
                                list(1, cnt) = 12;
                            }
                        }
                        set_config(u8"enter", key_enter, list(1, 1));
                        set_config(u8"cancel", key_cancel, list(1, 2));
                        set_config(u8"quickinv", key_quickinv, list(1, 3));
                        set_config(u8"quick", key_quick, list(1, 4));
                        set_config(u8"alter", key_alter, list(1, 5));
                        set_config(u8"get", key_get, list(1, 6));
                        set_config(u8"fire", key_fire, list(1, 7));
                        set_config(u8"target", key_target, list(1, 8));
                        set_config(u8"esc", key_esc, list(1, 9));
                        set_config(u8"help", key_help, list(1, 10));
                        set_config(u8"charainfo", key_charainfo, list(1, 11));
                        set_config(u8"ammo", key_ammo, list(1, 12));
                        p(0) = 0;
                        p(1) = cnvjkey(key_enter);
                        p(2) = cnvjkey(key_cancel);
                        p(3) = cnvjkey(key_quickinv);
                        p(4) = cnvjkey(key_quick);
                        p(5) = cnvjkey(key_alter);
                        p(6) = cnvjkey(key_get);
                        p(7) = cnvjkey(key_fire);
                        p(8) = cnvjkey(key_target);
                        p(9) = cnvjkey(key_esc);
                        p(10) = cnvjkey(key_help);
                        p(11) = cnvjkey(key_charainfo);
                        p(12) = cnvjkey(key_ammo);
                        for (int cnt = 1; cnt < 20; ++cnt)
                        {
                            list(1, cnt) = p(cnt);
                        }
                        f = 1;
                        break;
                    }
                }
                if (f)
                {
                    snd(20);
                    await(200);
                    reset_page = true;
                    continue;
                }
            }
        }
        if (key == key_pageup || key == key_pagedown)
        {
            if (key == key_pageup)
            {
                p = 1;
            }
            else
            {
                p = -1;
            }
            if (submenu == 1)
            {
                if (cs == 0)
                {
                    config::instance().extrahelp += p;
                    if (config::instance().extrahelp > 1)
                    {
                        config::instance().extrahelp = 1;
                    }
                    else if (config::instance().extrahelp < 0)
                    {
                        config::instance().extrahelp = 0;
                    }
                    snd(20);
                    set_config(u8"extraHelp", config::instance().extrahelp);
                    reset_page = true;
                    continue;
                }
                if (cs == 1)
                {
                    config::instance().ignoredislike += p;
                    if (config::instance().ignoredislike > 1)
                    {
                        config::instance().ignoredislike = 1;
                    }
                    else if (config::instance().ignoredislike < 0)
                    {
                        config::instance().ignoredislike = 0;
                    }
                    snd(20);
                    set_config(
                        u8"ignoreDislike", config::instance().ignoredislike);
                    reset_page = true;
                    continue;
                }
                if (cs == 2)
                {
                    config::instance().zkey += p;
                    if (config::instance().zkey > 2)
                    {
                        config::instance().zkey = 2;
                    }
                    else if (config::instance().zkey < 0)
                    {
                        config::instance().zkey = 0;
                    }
                    snd(20);
                    set_config(u8"zkey", config::instance().zkey);
                    reset_page = true;
                    continue;
                }
                if (cs == 3)
                {
                    config::instance().xkey += p;
                    if (config::instance().xkey > 2)
                    {
                        config::instance().xkey = 2;
                    }
                    else if (config::instance().xkey < 0)
                    {
                        config::instance().xkey = 0;
                    }
                    snd(20);
                    set_config(u8"xkey", config::instance().xkey);
                    reset_page = true;
                    continue;
                }
                if (cs == 4)
                {
                    config::instance().startrun += p;
                    if (config::instance().startrun > 20)
                    {
                        config::instance().startrun = 20;
                    }
                    else if (config::instance().startrun < 0)
                    {
                        config::instance().startrun = 0;
                    }
                    snd(20);
                    set_config(u8"startRun", config::instance().startrun);
                    reset_page = true;
                    continue;
                }
                if (cs == 5)
                {
                    config::instance().walkwait += p;
                    if (config::instance().walkwait > 10)
                    {
                        config::instance().walkwait = 10;
                    }
                    else if (config::instance().walkwait < 1)
                    {
                        config::instance().walkwait = 1;
                    }
                    snd(20);
                    set_config(u8"walkWait", config::instance().walkwait);
                    reset_page = true;
                    continue;
                }
                if (cs == 6)
                {
                    config::instance().attackwait += p;
                    if (config::instance().attackwait > 20)
                    {
                        config::instance().attackwait = 20;
                    }
                    else if (config::instance().attackwait < 1)
                    {
                        config::instance().attackwait = 1;
                    }
                    snd(20);
                    set_config(u8"attackWait", config::instance().attackwait);
                    reset_page = true;
                    continue;
                }
            }
            if (submenu == 2)
            {
                if (cs == 0)
                {
                    cfg_sound2 += p;
                    if (cfg_sound2 > 1)
                    {
                        cfg_sound2 = 1;
                    }
                    else if (cfg_sound2 < 0)
                    {
                        cfg_sound2 = 0;
                    }
                    snd(20);
                    set_config(u8"sound", cfg_sound2);
                    reset_page = true;
                    continue;
                }
                if (cs == 1)
                {
                    cfg_music2 += p;
                    if (cfg_music2 > 2)
                    {
                        cfg_music2 = 2;
                    }
                    else if (cfg_music2 < 0)
                    {
                        cfg_music2 = 0;
                    }
                    snd(20);
                    set_config(u8"music", cfg_music2);
                    reset_page = true;
                    continue;
                }
                if (cs == 2)
                {
                    cfg_fullscreen2 += p;
                    if (cfg_fullscreen2 > 1)
                    {
                        cfg_fullscreen2 = 1;
                    }
                    else if (cfg_fullscreen2 < 0)
                    {
                        cfg_fullscreen2 = 0;
                    }
                    snd(20);
                    set_config(u8"fullscreen", cfg_fullscreen2);
                    reset_page = true;
                    continue;
                }
                if (cs == 4)
                {
                    config::instance().scroll += p;
                    if (config::instance().scroll > 1)
                    {
                        config::instance().scroll = 1;
                    }
                    else if (config::instance().scroll < 0)
                    {
                        config::instance().scroll = 0;
                    }
                    snd(20);
                    set_config(u8"scroll", config::instance().scroll);
                    reset_page = true;
                    continue;
                }
                if (cs == 5)
                {
                    config::instance().alwayscenter += p;
                    if (config::instance().alwayscenter > 1)
                    {
                        config::instance().alwayscenter = 1;
                    }
                    else if (config::instance().alwayscenter < 0)
                    {
                        config::instance().alwayscenter = 0;
                    }
                    snd(20);
                    set_config(
                        u8"alwaysCenter", config::instance().alwayscenter);
                    reset_page = true;
                    continue;
                }
                if (cs == 6)
                {
                    config::instance().heart += p;
                    if (config::instance().heart > 1)
                    {
                        config::instance().heart = 1;
                    }
                    else if (config::instance().heart < 0)
                    {
                        config::instance().heart = 0;
                    }
                    snd(20);
                    set_config(u8"heartbeat", config::instance().heart);
                    reset_page = true;
                    continue;
                }
                if (cs == 7)
                {
                    config::instance().attackanime += p;
                    if (config::instance().attackanime > 1)
                    {
                        config::instance().attackanime = 1;
                    }
                    else if (config::instance().attackanime < 0)
                    {
                        config::instance().attackanime = 0;
                    }
                    snd(20);
                    set_config(u8"attackAnime", config::instance().attackanime);
                    reset_page = true;
                    continue;
                }
                if (cs == 8)
                {
                    config::instance().env += p;
                    if (config::instance().env > 1)
                    {
                        config::instance().env = 1;
                    }
                    else if (config::instance().env < 0)
                    {
                        config::instance().env = 0;
                    }
                    snd(20);
                    set_config(u8"envEffect", config::instance().env);
                    reset_page = true;
                    continue;
                }
                if (cs == 9)
                {
                    config::instance().shadow += p;
                    if (config::instance().shadow > 1)
                    {
                        config::instance().shadow = 1;
                    }
                    else if (config::instance().shadow < 0)
                    {
                        config::instance().shadow = 0;
                    }
                    snd(20);
                    set_config(u8"shadow", config::instance().shadow);
                    reset_page = true;
                    continue;
                }
                if (cs == 10)
                {
                    config::instance().objectshadow += p;
                    if (config::instance().objectshadow > 1)
                    {
                        config::instance().objectshadow = 1;
                    }
                    else if (config::instance().objectshadow < 0)
                    {
                        config::instance().objectshadow = 0;
                    }
                    snd(20);
                    set_config(
                        u8"objectShadow", config::instance().objectshadow);
                    reset_page = true;
                    continue;
                }
                if (cs == 3)
                {
                    x(0) = 800;
                    x(1) = 800;
                    x(2) = 1024;
                    x(3) = 1152;
                    x(4) = 1280;
                    x(5) = 1280;
                    y(0) = 600;
                    y(1) = 696;
                    y(2) = 768;
                    y(3) = 864;
                    y(4) = 768;
                    y(5) = 1024;
                    i = 0;
                    for (int cnt = 0; cnt < 5; ++cnt)
                    {
                        if (windoww2 == x(cnt))
                        {
                            if (windowh2 == y(cnt))
                            {
                                i = cnt;
                                break;
                            }
                        }
                    }
                    i += p;
                    if (i < 0)
                    {
                        i = 0;
                    }
                    else if (i > 4)
                    {
                        i = 4;
                    }
                    windoww2 = x(i);
                    windowh2 = y(i);
                    set_config(u8"windowW", windoww2);
                    set_config(u8"windowH", windowh2);
                    snd(20);
                    reset_page = true;
                    continue;
                }
            }
            if (submenu == 3)
            {
                if (cs == 0)
                {
                    config::instance().net += p;
                    if (config::instance().net > 1)
                    {
                        config::instance().net = 1;
                    }
                    else if (config::instance().net < 0)
                    {
                        config::instance().net = 0;
                    }
                    snd(20);
                    set_config(u8"net", config::instance().net);
                    reset_page = true;
                    continue;
                }
                if (cs == 1)
                {
                    config::instance().netwish += p;
                    if (config::instance().netwish > 1)
                    {
                        config::instance().netwish = 1;
                    }
                    else if (config::instance().netwish < 0)
                    {
                        config::instance().netwish = 0;
                    }
                    snd(20);
                    set_config(u8"netWish", config::instance().netwish);
                    reset_page = true;
                    continue;
                }
                if (cs == 2)
                {
                    config::instance().netchat += p;
                    if (config::instance().netchat > 1)
                    {
                        config::instance().netchat = 1;
                    }
                    else if (config::instance().netchat < 0)
                    {
                        config::instance().netchat = 0;
                    }
                    snd(20);
                    set_config(u8"netChat", config::instance().netchat);
                    reset_page = true;
                    continue;
                }
            }
            if (submenu == 4)
            {
                if (cs == 0)
                {
                    config::instance().runwait += p;
                    if (config::instance().runwait > 5)
                    {
                        config::instance().runwait = 5;
                    }
                    else if (config::instance().runwait < 2)
                    {
                        config::instance().runwait = 2;
                    }
                    snd(20);
                    set_config(u8"runWait", config::instance().runwait);
                    reset_page = true;
                    continue;
                }
                if (cs == 1)
                {
                    config::instance().autonumlock += p;
                    if (config::instance().autonumlock > 1)
                    {
                        config::instance().autonumlock = 1;
                    }
                    else if (config::instance().autonumlock < 0)
                    {
                        config::instance().autonumlock = 0;
                    }
                    snd(20);
                    set_config(u8"autoNumlock", config::instance().autonumlock);
                    reset_page = true;
                    continue;
                }
                if (cs == 2)
                {
                    snd(20);
                    set_config(u8"titleEffect", 0);
                    reset_page = true;
                    continue;
                }
                if (cs == 3)
                {
                    config::instance().scrsync += p;
                    if (config::instance().scrsync > 25)
                    {
                        config::instance().scrsync = 25;
                    }
                    else if (config::instance().scrsync < 2)
                    {
                        config::instance().scrsync = 2;
                    }
                    snd(20);
                    set_config(u8"scr_sync", config::instance().scrsync);
                    reset_page = true;
                    continue;
                }
                if (cs == 4)
                {
                    config::instance().runscroll += p;
                    if (config::instance().runscroll > 1)
                    {
                        config::instance().runscroll = 1;
                    }
                    else if (config::instance().runscroll < 0)
                    {
                        config::instance().runscroll = 0;
                    }
                    snd(20);
                    set_config(u8"scroll_run", config::instance().runscroll);
                    reset_page = true;
                    continue;
                }
                if (cs == 5)
                {
                    config::instance().autoturn += p;
                    if (config::instance().autoturn > 2)
                    {
                        config::instance().autoturn = 2;
                    }
                    else if (config::instance().autoturn < 0)
                    {
                        config::instance().autoturn = 0;
                    }
                    snd(20);
                    set_config(u8"autoTurnType", config::instance().autoturn);
                    reset_page = true;
                    continue;
                }
                if (cs == 6)
                {
                    config::instance().skiprandevents += p;
                    if (config::instance().skiprandevents > 1)
                    {
                        config::instance().skiprandevents = 1;
                    }
                    else if (config::instance().skiprandevents < 0)
                    {
                        config::instance().skiprandevents = 0;
                    }
                    snd(20);
                    set_config(
                        u8"skipRandEvents", config::instance().skiprandevents);
                    reset_page = true;
                    continue;
                }
            }
            if (submenu == 5)
            {
                if (cs == 0)
                {
                    config::instance().joypad += p;
                    if (config::instance().joypad > 1)
                    {
                        config::instance().joypad = 1;
                    }
                    else if (config::instance().joypad < 0)
                    {
                        config::instance().joypad = 0;
                    }
                    snd(20);
                    set_config(u8"joypad", config::instance().joypad);
                    reset_page = true;
                    continue;
                }
            }
            if (submenu == 6)
            {
                if (cs == 0)
                {
                    config::instance().msgaddtime += p;
                    if (config::instance().msgaddtime > 1)
                    {
                        config::instance().msgaddtime = 1;
                    }
                    else if (config::instance().msgaddtime < 0)
                    {
                        config::instance().msgaddtime = 0;
                    }
                    snd(20);
                    set_config(u8"msg_addTime", config::instance().msgaddtime);
                    reset_page = true;
                    continue;
                }
                if (cs == 1)
                {
                    config::instance().msgtrans += p;
                    if (config::instance().msgtrans > 5)
                    {
                        config::instance().msgtrans = 5;
                    }
                    else if (config::instance().msgtrans < 0)
                    {
                        config::instance().msgtrans = 0;
                    }
                    snd(20);
                    set_config(u8"msg_trans", config::instance().msgtrans);
                    reset_page = true;
                    continue;
                }
            }
            if (submenu == 7)
            {
                if (cs == 0)
                {
                    config::instance().language += p;
                    if (config::instance().language > 1)
                    {
                        config::instance().language = 1;
                    }
                    else if (config::instance().language < 0)
                    {
                        config::instance().language = 0;
                    }
                    snd(20);
                    set_config(u8"language", config::instance().language);
                    reset_page = true;
                    continue;
                }
            }
            if (submenu == 8)
            {
                if (cs == 0)
                {
                    config::instance().hp_bar += p;
                    if (config::instance().hp_bar > 2)
                    {
                        config::instance().hp_bar = 2;
                    }
                    else if (config::instance().hp_bar < 0)
                    {
                        config::instance().hp_bar = 0;
                    }
                    snd(20);
                    set_config(u8"hpBar", config::instance().hp_bar);
                    reset_page = true;
                    continue;
                }
                if (cs == 1)
                {
                    config::instance().leash_icon += p;
                    if (config::instance().leash_icon > 1)
                    {
                        config::instance().leash_icon = 1;
                    }
                    else if (config::instance().leash_icon < 0)
                    {
                        config::instance().leash_icon = 0;
                    }
                    snd(20);
                    set_config(u8"leashIcon", config::instance().leash_icon);
                    reset_page = true;
                    continue;
                }
                if (cs == 2)
                {
                    config::instance().use_autopick += p;
                    if (config::instance().use_autopick > 1)
                    {
                        config::instance().use_autopick = 1;
                    }
                    else if (config::instance().use_autopick < 0)
                    {
                        config::instance().use_autopick = 0;
                    }
                    snd(20);
                    set_config(
                        u8"use_autopick", config::instance().use_autopick);
                    reset_page = true;
                    continue;
                }
                if (cs == 3)
                {
                    config::instance().autosave += p;
                    if (config::instance().autosave > 1)
                    {
                        config::instance().autosave = 1;
                    }
                    else if (config::instance().autosave < 0)
                    {
                        config::instance().autosave = 0;
                    }
                    snd(20);
                    set_config(u8"autosave", config::instance().autosave);
                    reset_page = true;
                    continue;
                }
            }
        }
        if (key == key_cancel)
        {
            if (submenu != 0)
            {
                submenu = 0;
                set_option();
                return;
            }
            load_config();
            if (mode == 0)
            {
                if (config::instance().net)
                {
                    initialize_server_info();
                }
            }
            return;
        }
    }
}



} // namespace elona
