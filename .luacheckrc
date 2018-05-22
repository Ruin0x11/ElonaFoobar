-------------------------------------------------------------------------------
--[LICENSE]--
-------------------------------------------------------------------------------
-- .luacheckrc
-- This file is free and unencumbered software released into the public domain.
--
-- Anyone is free to copy, modify, publish, use, compile, sell, or
-- distribute this file, either in source code form or as a compiled
-- binary, for any purpose, commercial or non-commercial, and by any
-- means.
--
-- In jurisdictions that recognize copyright laws, the author or authors
-- of this file dedicate any and all copyright interest in the
-- software to the public domain. We make this dedication for the benefit
-- of the public at large and to the detriment of our heirs and
-- successors. We intend this dedication to be an overt act of
-- relinquishment in perpetuity of all present and future rights to this
-- software under copyright law.
--
-- THE FILE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
-- EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
-- MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
-- IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
-- OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
-- ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
-- OTHER DEALINGS IN THE SOFTWARE.
--
-- For more information, please refer to <http://unlicense.org/>
-- ]]

-------------------------------------------------------------------------------
--[.luacheckrc]
-------------------------------------------------------------------------------
-- Set up the the standards for this file.
files['.luacheckrc'] = {
    std = 'lua52c',
    globals = {'files', 'exclude_files', 'not_globals', 'stds', 'std', 'max_line_length', 'ignore', 'quiet', "max_cyclomatic_complexity", "codes"},
    max_line_length = false --turn of line length warnings for this file
}

-------------------------------------------------------------------------------
--[Set Defaults]--
-------------------------------------------------------------------------------
local LINE_LENGTH = false
local IGNORE = {'21./%w+_$', '21./^_%w+$', '213/[ijk]', '213/index', '213/key'}
local NOT_GLOBALS = {'coroutine', 'io', 'socket', 'dofile', 'loadfile'}

local STD_ELONA = 'lua52c+elona+stdlib'

-------------------------------------------------------------------------------
--[Standards]--
-------------------------------------------------------------------------------
std = STD_ELONA
max_line_length = LINE_LENGTH

not_globals = NOT_GLOBALS
ignore = IGNORE
quiet = 1 -- pass -q option
max_cyclomatic_complexity = 75
codes = true

--List of files and directories to exclude
exclude_files = {
    --Ignore special folders
    '**/.*/*', --Ignore if path starts with .
    '**/mod/stdlib/', --Ignore from symlinked
    '**/vendor/',
}

-------------------------------------------------------------------------------
--[Mod Prototypes]--
-------------------------------------------------------------------------------
--Set default prototype files
-- files['**/data.lua'].std = STD_DATA

-------------------------------------------------------------------------------
--[Base]--
-------------------------------------------------------------------------------

local std_core = {
   std = STD_ELONA,
   ignore = {'122'} -- The core mod is special, since we add on parts of the API that are implemented in Lua there, so don't check for read-only fields.
}

files['**/mods/'] = {std = STD_ELONA}
files['**/mods/core'] = std_core
files['**/tests/lua'] = {std = STD_ELONA .. "+tests"}

-------------------------------------------------------------------------------
--[STDS ELONA]--
-------------------------------------------------------------------------------
stds.elona = {
    --Set the read only variables
    read_globals = {
        -- @Elona@: The core Elona API.
        Elona = {
           fields = {
              Rand = {
                 fields = {
                    "rnd", "one_in", "coinflip"
                 },
              },
              Event = {
                 fields = {
                    "register", "clear"
                 },
              },
              Chara = {
                 fields = {
                    "is_alive", "is_player", "is_ally", "player", "create"
                 },
              },
              Map = {
                 fields = {
                    "width", "height", "valid", "can_access", "bound_within", "random_pos", "set_tile", "set_tile_memory",
                 },
              },
              Pos = {
                 fields = {
                    "dist"
                 },
              },
              World = {
                 fields = {
                    "time"
                 },
              },
              Magic = {
                 fields = {
                    "cast"
                 },
              },
              FOV = {
                 fields = {
                    "los", "you_see"
                 },
              },
              Item = {
                 fields = {
                    "create"
                 },
              },
              GUI = {
                 fields = {
                    "txt"
                 },
              },
              Debug = {
                 fields = {
                    "log", "dump_characters", "dump_items", "inspect"
                 }
              },
              ReadOnly = {
                 fields = {
                    "make_read_only"
                 }
              },
              Iter = {
                 fields = {
                    "rectangle_iter", "rectangle_iter_step"
                 }
              },
              Registry = {
                 fields = {
                    "on_event",
                    "register_chara_init",
                    "register_map_init",
                    Data = {
                       read_only = false,
                       other_fields = true
                    }
                 }
              },
              Defines = {
                 fields = {
                    Enums = {},
                    Events = {
                       other_fields = true
                    },
                 }
              },
           }
        },
        -- @Store@: Mod-local data storage.
        Store = {
           other_fields = true
        }
    },
}

--(( stdlib extensions ))--
stds.stdlib = {
    read_globals = {
        table = {
            fields = {
               "find", "size"
            },
        },
    },
}

--(( tests ))--
stds.tests = {
    read_globals = {
        "lrun", "lequal", "lfequal", "lok", "lresults()",
        Testing = {
            fields = {
               "start_in_debug_map", "reset_state"
            },
        },
    },
}

--[[
    "ignore", "std", "globals", "unused_args", "self", "compat", "global", "unused", "redefined",
    "unused_secondaries", "allow_defined", "allow_defined_top", "module",
    "read_globals", "new_globals", "new_read_globals", "enable", "only", "not_globals",
    "max_line_length", "max_code_line_length", "max_string_line_length", "max_comment_line_length",
    "max_cyclomatic_complexity"
--]]

-- Warnings list
-- 011 A syntax error.
-- 021 An invalid inline option.
-- 022 An unpaired inline push directive.
-- 023 An unpaired inline pop directive.
-- 111 Setting an undefined global variable.
-- 112 Mutating an undefined global variable.
-- 113 Accessing an undefined global variable.
-- 121 Setting a read-only global variable.
-- 122 Setting a read-only field of a global variable.
-- 131 Unused implicitly defined global variable.
-- 142 Setting an undefined field of a global variable.
-- 143 Accessing an undefined field of a global variable.
-- 211 Unused local variable.
-- 212 Unused argument.
-- 213 Unused loop variable.
-- 221 Local variable is accessed but never set.
-- 231 Local variable is set but never accessed.
-- 232 An argument is set but never accessed.
-- 233 Loop variable is set but never accessed.
-- 241 Local variable is mutated but never accessed.
-- 311 Value assigned to a local variable is unused.
-- 312 Value of an argument is unused.
-- 313 Value of a loop variable is unused.
-- 314 Value of a field in a table literal is unused.
-- 321 Accessing uninitialized local variable.
-- 331 Value assigned to a local variable is mutated but never accessed.
-- 341 Mutating uninitialized local variable.
-- 411 Redefining a local variable.
-- 412 Redefining an argument.
-- 413 Redefining a loop variable.
-- 421 Shadowing a local variable.
-- 422 Shadowing an argument.
-- 423 Shadowing a loop variable.
-- 431 Shadowing an upvalue.
-- 432 Shadowing an upvalue argument.
-- 433 Shadowing an upvalue loop variable.
-- 511 Unreachable code.
-- 512 Loop can be executed at most once.
-- 521 Unused label.
-- 531 Left-hand side of an assignment is too short.
-- 532 Left-hand side of an assignment is too long.
-- 541 An empty do end block.
-- 542 An empty if branch.
-- 551 An empty statement.
-- 611 A line consists of nothing but whitespace.
-- 612 A line contains trailing whitespace.
-- 613 Trailing whitespace in a string.
-- 614 Trailing whitespace in a comment.
-- 621 Inconsistent indentation (SPACE followed by TAB).
-- 631 Line is too long.
