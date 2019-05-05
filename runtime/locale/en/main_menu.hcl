locale {
    main_menu {
        title_menu {
            continue = "Restore an Adventurer"
            new = "Generate an Adventurer"
            incarnate = "Incarnate an Adventurer"
            about = "About"
            options = "Options"
            mods = "Mods"
            exit = "Exit"
        }

        continue {
            which_save = "Which save game do you want to continue?"
            title = "Game Selection"
            key_hint = "BackSpace [Delete]  "
            no_save = "No save files found"
            delete = "Do you really want to delete ${_1} ?"
            delete2 = "Are you sure you really want to delete ${_1} ?"
        }

        incarnate {
            which_gene = "Which gene do you want to incarnate?"
            title = "Gene Selection"
            no_gene = "No gene files found"
        }

        about {
            title = "About"
            vanilla_homepage = "Vanilla Elona Homepage"
            foobar_homepage = "Elona foobar Homepage"
            foobar_changelog = "Elona foobar Changelog"
            license = "License"
            credits = "Credits"
        }

        about_changelog {
            title = "Changelogs"
        }

        mods {
            title {
                installed = "Installed Mods"
                download = "Download Mods"
            }

            caption {
                installed = "You can enable and disable mods here."
                download = "You can download new mods from the online repository here."
                confirm = "Download this mod?"
            }

            no_readme = "(No README available.)"

            info {
                title = "Information"
                name = "Name"
                id = "ID"
                author = "Author"
                version = "Version"
                description = "Description"
                updated = "Updated"
                downloads = "Downloads"
            }

            date = "${_3}-${_1}-${_2} ${_4}:${_5}:${_6}"

            download {
                failed = "Could not download mod list."
            }

            hint {
                toggle = "[Enable/Disable]"
                info = "[Mod Info]"
                do_download = "[Download Mod]"
                download = "[Switch To Download]"
                installed = "[Switch To Installed]"
            }
        }
    }
}
