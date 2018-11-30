locale {
    key_pressed = "Key was pressed. Stopping ${_1}."
    confused = "You are confused. Stopping ${_1}."

    menu {
        prompt = "What would you like to do?"

        travel = "Travel"
        explore = "Explore"
        waypoints = "Waypoints"
    }

    travel {
        name = "travel"

        prompt = "Where would you like to go?"
        already_there = "You're already there!"
        start = "Traveling."
    }

    explore {
        name = "exploration"

        cannot_in_overworld = "You can't explore in the overworld."
        start = "Exploring."
    }

    waypoint {
        stairs {
            up = "Stairs up"
            down = "Stairs down"
        }

        none_available = "No waypoints available for this area."
        prompt = "Which waypoint?"
        start = "Traveling to ${_1}."
    }

    pathing {
        aborted = "Aborting ${_1}."
        finished = "${_1} finished."
        waypoint_target_missing = "The waypoint target isn't here."

        halt {
            default = "There's no unblocked path available."
            unknown_tile = "You don't know what's there."
            solid = "The destination is blocked."
            chara = "${name(_1)} is in the way."
            danger = "Something dangerous is there."
        }
    }
}
