locale {
    config {
        common {
            menu = "項目"
            require_restart = "* 印のついた項目は、ゲームの再起動後に適用されます"
            assign_button = "ボタンを割り当てたい項目にカーソルをあわせて\nゲームパッドのボタンを押してください。(L),(R)の付いている\n項目は、メニュー画面でタブの移動に使われます。"

            yes_no {
                # NOTE: This text is implicitly chosen for boolean values without an explicit
                # "yes_no" option.
                default {
                    yes = "する"
                    no = "しない"
                }

                ari_nashi {
                    yes = "あり"
                    no = "なし"
                }

                shiyou_suru_shinai {
                    yes = "使用する"
                    no = "使用しない"
                }

                kougashitsu_teigashitsu {
                    yes = "高画質"
                    no = "低画質"
                }

                saisei_suru_shinai {
                    yes = "再生する"
                    no = "再生しない"
                }

                anime_ari_nashi {
                    yes = "アニメあり"
                    no = "アニメなし"
                }

                ari_nashi_slow_fast {
                    yes = "あり（低速）"
                    no = "なし（高速）"
                }

                tsukau_tsukawanai {
                    yes = "使う"
                    no = "使わない"
                }

                yuukou_mukou {
                    yes = "有効"
                    no = "無効"
                }

                windowed_fullscreen {
                    yes = "フルスクリーン"
                    no = "ウィンドウ"
                }

                unsupported {
                    yes = "オフ(未実装)"
                    no = "オフ(未実装)"
                }
            }

            formatter {
                wait = "${_1} wait"
            }
        }
        menu {
            name = "オプション"
            language {
                name = "言語(Language)"

                language {
                    name = "言語*"
                    variants {
                        jp = "Japanese"
                        en = "English"
                    }
                }
            }
            screen {
                name = "画面と音の設定"
                fullscreen {
                    name = "画面モード*"
                    variants {
                        windowed = "ウィンドウ"
                        fullscreen = "フルスクリーン"
                        desktop_fullscreen = "フルスクリーン2"
                    }
                }
                display_mode {
                    name = "画面の大きさ*"
                    # Variants are injected at runtime.
                }
                fullscreen_android {
                    name = "画面モード*"
                    yes_no = core.locale.config.common.yes_no.windowed_fullscreen
                }
                fullscreen_scale {
                    name = "フルスクリーンスケール*"
                }
                window_mode {
                    name = "ウィンドウの大きさ*"
                }
                orientation {
                    name = "ローテーション"
                    variants {
                        sensor_landscape = "横向き (自動)"
                        sensor_portrait = "縦向き (自動)"
                        sensor = "自動回転"
                        landscape = "横向き"
                        portrait = "縦向き"
                        reverse_landscape = "横向き (反転)"
                        reverse_portrait = "縦向き (反転)"
                    }
                }
                music {
                    name = "midiの再生*"
                    variants {
                        none = "なし"
                        direct_music = "direct music"
                        mci = "MCI"
                    }
                }
                sound {
                    name = "サウンドの再生*"
                    yes_no = core.locale.config.common.yes_no.ari_nashi
                }
                high_quality_shadows {
                    name = "光源の描写"
                    yes_no = core.locale.config.common.yes_no.kougashitsu_teigashitsu
                }
                object_shadows {
                    name = "アイテムの影描写"
                    yes_no = core.locale.config.common.yes_no.ari_nashi_slow_fast
                }
                heartbeat {
                    name = "心臓の音"
                    yes_no = core.locale.config.common.yes_no.saisei_suru_shinai
                }
                skip_random_event_popups {
                    name = "イベントの短縮"
                }
            }
            net {
                name = "ネット機能の設定"
                enabled {
                    name = "ネットの使用"
                }
                wish {
                    name = "定期的に接続"
                }
                chat {
                    name = "チャットをフィルタ"
                }
                server_list {
                    name = "カスタムサーバー"
                    yes_no = core.locale.config.common.yes_no.shiyou_suru_shinai
                }
            }
            anime {
                name = "アニメ設定"
                scroll {
                    name = "スクロール"
                }
                always_center {
                    name = "主人公中心に描写"
                }
                scroll_when_run {
                    name = "走り時スクロール"
                }
                general_wait {
                    name = "通常ウェイト"
                    formatter = core.locale.config.common.formatter.wait
                }
                anime_wait {
                    name = "アニメウェイト"
                    formatter = core.locale.config.common.formatter.wait
                }
                alert_wait {
                    name = "アラートウェイト"
                }
                auto_turn_speed {
                    name = "オートターンの挙動"
                    variants {
                        normal = "普通"
                        high = "速め"
                        highest = "省略"
                    }
                }
                attack_anime {
                    name = "攻撃時アニメ"
                }
                weather_effect {
                    name = "天候エフェクト"
                }
                title_effect {
                    name = "タイトルの水の波紋"
                    yes_no = core.locale.config.common.yes_no.unsupported
                }
                window_anime {
                    name = "ウィンドウアニメ"
                }
                screen_refresh {
                    name = "画面の更新頻度"
                    formatter = core.locale.config.common.formatter.wait
                }
            }

            font {
                name = "フォント設定"
                vertical_offset {
                    name = "垂直オフセット"
                }
                size_adjustment {
                    name = "サイズの調整"
                }
            }

            game {
                name = "ゲーム設定"
                default_save {
                    name = "デフォルトセーブ"
                    variants {
                        "" = "使用しない"
                    }
                }
                attack_neutral_npcs {
                    name = "非好戦的NPCを攻撃"
                }
                story {
                    name = "シーンの再生"
                    yes_no = core.locale.config.common.yes_no.saisei_suru_shinai
                }
                extra_help {
                    name = "ノルンの冒険ガイド"
                }
                hide_autoidentify {
                    name = "自然鑑定ﾒｯｾｰｼﾞの非表示"
                }
                hide_shop_updates {
                    name = "店ﾒｯｾｰｼﾞの非表示"
                }
            }

            message {
                name = "メッセージとログ"
                add_timestamps {
                    name = "ﾒｯｾｰｼﾞに分表示追加"
                }
                transparency {
                    name = "過去のﾒｯｾｰｼﾞの透過"
                    formatter = "${_1}*10 %" #TODO
                }
            }

            input {
                name = "入力設定"
                joypad {
                    name = "ゲームパッド"
                    yes_no = core.locale.config.common.yes_no.unsupported # core.locale.config.common.yes_no.shiyou_suru_shinai
                }
                autodisable_numlock {
                    name = "numlockを自動制御"
                }
                walk_wait {
                    name = "歩きの速さ"
                    formatter = core.locale.config.common.formatter.wait
                }
                run_wait {
                    name = "走りの速さ"
                    formatter = core.locale.config.common.formatter.wait
                }
                start_run_wait {
                    name = "走り開始の速さ"
                    formatter = "${After } ${_1}+1 steps" # TODO
                }
                attack_wait {
                    name = "攻撃の間隔"
                    formatter = core.locale.config.common.formatter.wait
                }
                key_wait {
                    name = "キーウェイト"
                    formatter = core.locale.config.common.formatter.wait
                }
                select_wait {
                    name = "選択ウェイト"
                    formatter = core.locale.config.common.formatter.wait
                }
                select_fast_start_wait {
                    name = "選択ウェイト(早い,開始)"
                    formatter = core.locale.config.common.formatter.wait
                }
                select_fast_wait {
                    name = "選択ウェイト(早い)"
                    formatter = core.locale.config.common.formatter.wait
                }
            }

            balance {
                name = "ゲームバランス設定"
                restock_interval {
                    name = "入荷頻度"
                    formatter = "${_1}日" # TODO
                }
                extra_race {
                    name = "Extra種族"
                }
                extra_class {
                    name = "Extra職業"
                }
            }

            foobar {
                name = "拡張設定(Foobar)"

                hp_bar_position {
                    name = "ペットのHPバー"
                    variants {
                        hide = "表示しない"
                        left = "左側に表示"
                        right = "右側に表示"
                    }
                }
                leash_icon {
                    name = "紐のアイコン表示"
                }
                autopick {
                    name = "Autopick"
                    yes_no = core.locale.config.common.yes_no.tsukau_tsukawanai
                }
                autosave {
                    name = "オートセーブ"
                    yes_no = core.locale.config.common.yes_no.yuukou_mukou
                }
                damage_popup {
                    name = "ダメージポップアップ"
                    yes_no = core.locale.config.common.yes_no.ari_nashi
                }
                startup_script {
                }
            }
        }
    }
}
