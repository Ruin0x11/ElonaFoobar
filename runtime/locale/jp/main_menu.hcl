locale {
    main_menu {
        title_menu {
            continue = "冒険を再開する"
            new = "新しい冒険者を作成する"
            incarnate = "冒険者の引継ぎ"
            about = "このゲームについて"
            options = "設定の変更"
            mods = "MODリスト"
            exit = "終了"
        }

        continue {
            which_save = "どの冒険を再開するんだい？"
            title = "冒険者の選択"
            key_hint = "BackSpace [削除]  "
            no_save = "No save files found"
            delete = "本当に${_1}を削除していいのかい？"
            delete2 = "本当の本当に${_1}を削除していいのかい？"
        }

        incarnate {
            which_gene = "どの遺伝子を引き継ぐ？"
            title = "遺伝子の選択"
            no_gene = "No gene files found"
        }

        about {
            title = "About"
            vanilla_homepage = "本家ホームページ"
            foobar_homepage = "Elona foobar ホームページ"
            foobar_changelog = "Elona foobar 変更履歴"
            license = "ライセンス"
            credits = "クレジット"
        }

        about_changelog {
            title = "更新履歴"
        }

        mods {
            title {
                installed = "インストール済みのMOD"
                download = "MODダウンロード"
            }

            caption {
                installed = "ここにMODの切替ができます。"
                download = "ここに新しいMODをサーバーから手に入れられます。"
                confirm = "このMODをダウンロードしますか？"
            }

            no_readme = "(READMEはありません)"

            info {
                title = "情報"
                id = "ID"
                name = "名前"
                author = "作者"
                version = "バージョン"
                description = "説明文"
                updated = "更新日時"
                downloads = "ダウンロード数"
            }

            date = "${_1}年${_2}月${_3}日 ${_4}:${_5}:${_6}"

            download {
                failed = "MODリストのダウンロードに失敗しました。"
            }

            hint {
                toggle = "[有効/無効にする]"
                info = "[MODの情報]"
                do_download = "[ダウンロードの実行]"
                download = "[ダウンロード]"
                installed = "[インストール済み]"
            }
        }
    }
}
