#include "../thirdparty/catch2/catch.hpp"

#include "../elona/i18n.hpp"
#include "../elona/item.hpp"
#include "../elona/itemgen.hpp"
#include "../elona/testing.hpp"
#include "../elona/ui.hpp"
#include "../elona/variables.hpp"
#include "tests.hpp"

using namespace std::literals::string_literals;
using namespace elona;

TEST_CASE("test foodname (JP)", "[I18N: itemname]")
{
    testing::start_in_debug_map();
    testing::set_japanese();

    SECTION("without subname")
    {
        Item& item = testing::create_item(260);
        update_slight();

        item.param2 = 1;
        REQUIRE(itemname(item.index) == u8"恐怖のパン");
        item.param2 = 8;
        REQUIRE(itemname(item.index) == u8"カレーパン");
    }
    SECTION("with subname")
    {
        Item& item = testing::create_item(573);
        update_slight();

        item.param2 = 1;
        REQUIRE(itemname(item.index) == u8"グロテスクな鳥の卵");
        item.param2 = 8;
        REQUIRE(itemname(item.index) == u8"鳥のレアチーズケーキ");
        item.subname = 3;
        REQUIRE(itemname(item.index) == u8"プチのレアチーズケーキ");
    }
}

TEST_CASE("test itemname additional info (JP)", "[I18N: itemname]")
{
    testing::start_in_debug_map();
    testing::set_japanese();

    SECTION("kitty bank")
    {
        Item& item = testing::create_item(578);

        item.param2 = 3;
        REQUIRE(itemname(item.index) == u8"5万金貨貯金箱");
    }
    SECTION("bait")
    {
        Item& item = testing::create_item(617);

        item.param1 = 3;
        REQUIRE(itemname(item.index) == u8"トンボ");
    }
    SECTION("ancient book")
    {
        Item& item = testing::create_item(687);

        item.param1 = 3;
        REQUIRE(
            itemname(item.index) ==
            u8"《グラーキ黙示録》という題名の古書物(残り1回)");
        item.param2 = 1;
        REQUIRE(
            itemname(item.index) ==
            u8"解読済みの《グラーキ黙示録》という題名の古書物(残り1回)");
    }
    SECTION("recipe")
    {
        Item& item = testing::create_item(783);

        item.param1 = 0;
        REQUIRE(itemname(item.index) == u8"もう使えないレシピ");
        item.param1 = 3;
        REQUIRE(itemname(item.index) == u8"カスタムレシピ");
        item.subname = 200;
        REQUIRE(itemname(item.index) == u8"《媚薬混入食品》のレシピ");
    }
    SECTION("textbook")
    {
        Item& item = testing::create_item(563);

        item.param1 = 101;
        REQUIRE(itemname(item.index) == u8"《短剣》という題名の学習書");
    }
    SECTION("book of rachel")
    {
        Item& item = testing::create_item(668);

        item.param2 = 3;
        REQUIRE(itemname(item.index) == u8"第3巻目のレイチェルの絵本");
    }
    SECTION("book")
    {
        Item& item = testing::create_item(24);

        item.param1 = 3;
        REQUIRE(itemname(item.index) == u8"《見るな》という題名の本");
    }
    SECTION("altar")
    {
        Item& item = testing::create_item(171);

        item.param1 = 2;
        REQUIRE(itemname(item.index) == u8"風のルルウィの祭壇");
    }
    SECTION("fish")
    {
        Item& item = testing::create_item(618);

        item.param1 = 6000;
        item.param2 = 4;
        item.subname = 4;
        REQUIRE(itemname(item.index) == u8"おたまじゃくしの煮込み");
    }
    SECTION("food")
    {
        Item& item = testing::create_item(772);

        item.own_state = 4;
        REQUIRE(itemname(item.index) == u8"超ミニに育ったトマト");
        item.own_state = 0;
        item.param1 = 1002;
        item.param2 = 2;
        item.subname = 2;
        REQUIRE(
            itemname(item.index) == u8"焼け焦げた偽りの預言者『ゼーム』の肉");
    }
    SECTION("fish with subname")
    {
        Item& item = testing::create_item(619);

        item.subname = 1;
        REQUIRE(itemname(item.index) == u8"金魚");
    }
    SECTION("food with subname")
    {
        Item& item = testing::create_item(204);

        item.subname = 1;
        REQUIRE(itemname(item.index) == u8"店主の死体");
    }
    SECTION("card")
    {
        Item& item = testing::create_item(504);

        item.subname = 1;
        REQUIRE(itemname(item.index) == u8"店主のカード");
    }
    SECTION("furniture")
    {
        Item& item = testing::create_item(77);

        item.subname = 1;
        REQUIRE(itemname(item.index) == u8"みすぼらしい鉛細工の丸椅子");
    }
    SECTION("deed")
    {
        Item& item = testing::create_item(344);

        item.param1 = 2;
        item.subname = 1;
        REQUIRE(itemname(item.index) == u8"住み心地のいい家の権利書");
    }
    SECTION("bill")
    {
        Item& item = testing::create_item(615);

        item.subname = 3;
        REQUIRE(itemname(item.index) == u8"★3goldの請求書");
    }
    SECTION("vomit")
    {
        Item& item = testing::create_item(704);

        item.subname = 3;
        REQUIRE(itemname(item.index) == u8"プチのゲロゲロ");
    }
    SECTION("secret treasure")
    {
        Item& item = testing::create_item(672);

        item.param1 = 169;
        REQUIRE(itemname(item.index) == u8"★善人の秘宝");
        item.param1 = 166;
        REQUIRE(itemname(item.index) == u8"★ジュアの秘宝");
    }
}
