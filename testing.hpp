#pragma once

namespace elona
{
namespace testing
{

void pre_init();
void post_run();

/***
 * Sets world state to a clean base for testing:
 * - Wipes and reinitializes everything.
 * - Resets Lua mods state.
 * - Creates a blank 50x50 map.
 * - Places player at (25,25).
 */
void reset_state();

/***
 * Starts an interactive session.
 * After this is run you can use command(key) to simulate player input.
 */
void run_game();

}
}
