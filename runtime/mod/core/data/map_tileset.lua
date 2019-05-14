-- TODO: check which tilesets are possible in world map
-- TODO: door_closed/door_open can be unified

local function tile(atlas, id)
   return "core." .. tostring(atlas) .. "_" .. tostring(id)
end

local function chance(atlas, id, count, chance)
   if chance == nil then chance = 1 end
   if count == nil then count = 1 end
   local other = {}
   for i=0,count do
      other[#other+1] = tile(atlas, id + i)
   end
   return {
      id = tile(atlas, id),
      chance = chance,
      other = other
   }
end

-- Assumes that tile comes from atlas 0, so (0 * 824) + tile == tile
local tiles = {
   [1] = {4, 5, 6, 7, 8},
   [2] = {264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362},
   [3] = {9, 10, 11, 12},
   [4] = {13, 14, 15, 16},
}

local tiles_set = {}

for k, v in ipairs(tiles) do
   tiles_set[k] = {}
   for _, i in ipairs(v) do
      tiles_set[k][i] = true
   end
end

local function contained(i)
   for k, _ in ipairs(tiles) do
      if tiles_set[k][i] == true then
         return k
      end
   end

   return nil
end

local function tiles_field(atlas)
   return function(e)
      local World = Elona.require("World")
      local t = e.stood_world_map_tile
      local result = {
         default = tile(atlas, 0),
         fog = tile(atlas, 528)
      }
      if contained(t) == 1 then
         result.default = tile(atlas, 7)
         result.fog = tile(atlas, 528)
      end
      if contained(t) == 2 then
         result.default = tile(atlas, 12)
      end
      if contained(t) == 3 then
         result.fog = tile(atlas, 528)
         result.default = tile(atlas, 3)
      end
      if contained(t) == 4 then
         result.fog = tile(atlas, 531)
         result.default = tile(atlas, 19)
      end
      if data.raw["core.map_chip"]["core.0_" .. tostring(t)].kind == 4 then
         result.fog = tile(atlas, 532)
         result.default = tile(atlas, 45)
      end

      return result
   end
end

data:define_type("map_tileset")
data:add_multi(
   "core.map_tileset",
   {
      {
         id = "0_0",
         tiles = {
            door_closed = tile(0, 726),
            door_open = tile(0, 236),
            default = chance(0, 396),
            fog = chance(0, 531),
            wall = chance(0, 462),
            room = chance(0, 13),
            board = tile(0, 727),
            vote_box = tile(0, 729),
            town_board = tile(0, 732),
            tunnel = chance(0, 33),
            pot = tile(0, 242),
         },
      },
      {
         id = "1_default",
         tiles = {
            door_closed = tile(1, 726),
            door_open = tile(1, 236),
            default = chance(1, 396),
            fog = chance(1, 531),
            wall = chance(1, 462),
            room = chance(1, 13),
            board = tile(1, 727),
            vote_box = tile(1, 729),
            town_board = tile(1, 732),
            tunnel = chance(1, 33),
            pot = tile(1, 242),
         }
      },
      {
         id = "2_default",
         tiles = {
            door_closed = tile(2, 726),
            door_open = tile(2, 236),
            default = chance(2, 396),
            fog = chance(2, 531),
            wall = chance(2, 462),
            room = chance(2, 13),
            board = tile(2, 727),
            vote_box = tile(2, 729),
            town_board = tile(2, 732),
            tunnel = chance(2, 33),
            pot = tile(2, 242),
         }
      },
      {
         id = "1_12",
         parent = "core.1_default",
         tiles = {
            door_closed = tile(1, 733),
            door_open = tile(1, 265),
         }
      },
      {
         id = "2_12",
         parent = "core.2_default",
         tiles = {
            door_closed = tile(2, 733),
            door_open = tile(2, 265),
         }
      },
      {
         id = "1_8",
         parent = "core.1_default",
         tiles = {
            door_closed = tile(1, 728),
            door_open = tile(1, 241),
         }
      },
      {
         id = "2_8",
         parent = "core.2_default",
         tiles = {
            door_closed = tile(2, 728),
            door_open = tile(2, 241),
         }
      },
      {
         id = "1_9",
         parent = "core.1_default",
         tiles = {
            door_closed = tile(1, 730),
            door_open = tile(1, 264),
         }
      },
      {
         id = "2_9",
         parent = "core.2_default",
         tiles = {
            door_closed = tile(2, 730),
            door_open = tile(2, 264),
         }
      },
      {
         id = "1_3",
         parent = "core.1_default",
         tiles = {
            room = tile(1, 41),
            tunnel = tile(1, 41),
            wall = tile(1, 469),
            fog = tile(1, 469 + 66),
         }
      },
      {
         id = "2_3",
         parent = "core.2_default",
         tiles = {
            room = tile(2, 41),
            tunnel = tile(2, 41),
            wall = tile(2, 469),
            fog = tile(2, 469 + 66),
         }
      },
      {
         id = "1_2",
         parent = "core.1_default",
         tiles = {
            wall = tile(1, 462),
            fog = tile(1, 529),
         }
      },
      {
         id = "2_2",
         parent = "core.2_default",
         tiles = {
            wall = tile(2, 462),
            fog = tile(2, 529),
         }
      },
      {
         id = "1_10",
         parent = "core.1_default",
         tiles = {
            room = tile(1, 165),
            tunnel = chance(1, 33, 4, 2),
            wall = tile(1, 469),
            default = tile(1, 469),
         }
      },
      {
         id = "2_10",
         parent = "core.2_default",
         tiles = {
            room = tile(2, 165),
            tunnel = chance(2, 33, 4, 2),
            wall = tile(2, 469),
            default = tile(2, 469),
         }
      },
      {
         id = "1_11",
         parent = "core.1_default",
         tiles = {
            default = tile(1, 465),
            fog = tile(1, 530),
            tunnel = tile(1, 212),
            wall = tile(1, 467),
            room = tile(1, 203),
         }
      },
      {
         id = "2_11",
         parent = "core.2_default",
         tiles = {
            default = tile(2, 465),
            fog = tile(2, 530),
            tunnel = tile(2, 212),
            wall = tile(2, 467),
            room = tile(2, 203),
         }
      },
      {
         id = "1_0",
         parent = "core.1_default",
         tiles = {
            default = tile(1, 469),
            fog = tile(1, 529),
            tunnel = chance(1, 33, 4, 2),
            wall = tile(1, 469),
            room = chance(1, 33, 6, 3),
         }
      },
      {
         id = "2_0",
         parent = "core.2_default",
         tiles = {
            default = tile(2, 469),
            fog = tile(2, 529),
            tunnel = chance(2, 33, 4, 2),
            wall = tile(2, 469),
            room = chance(2, 33, 6, 3),
         }
      },
      {
         id = "1_6",
         parent = "core.1_default",
         tiles = {
            default = tile(1, 469),
            fog = tile(1, 530),
            tunnel = tile(1, 68),
            wall = tile(1, 474),
            room = chance(1, 66, 2, 2),
         }
      },
      {
         id = "2_6",
         parent = "core.2_default",
         tiles = {
            default = tile(2, 469),
            fog = tile(2, 530),
            tunnel = tile(2, 68),
            wall = tile(2, 474),
            room = chance(2, 66, 2, 2),
         }
      },
      {
         id = "1_7",
         parent = "core.1_default",
         tiles = {
            default = tile(1, 474),
            fog = tile(1, 528),
            tunnel = tile(1, 0),
            wall = tile(1, 475),
            room = chance(1, 0, 6, 6),
         }
      },
      {
         id = "2_7",
         parent = "core.2_default",
         tiles = {
            default = tile(2, 474),
            fog = tile(2, 528),
            tunnel = tile(2, 0),
            wall = tile(2, 475),
            room = chance(2, 0, 6, 6),
         }
      },
      {
         id = "1_300",
         parent = "core.1_default",
         tiles = {
            default = tile(1, 475),
            fog = tile(1, 528),
            tunnel = tile(1, 0),
            wall = tile(1, 475),
            room = chance(1, 0, 6, 6),
         }
      },
      {
         id = "2_300",
         parent = "core.2_default",
         tiles = {
            default = tile(2, 475),
            fog = tile(2, 528),
            tunnel = tile(2, 0),
            wall = tile(2, 475),
            room = chance(2, 0, 6, 6),
         }
      },
      {
         id = "1_100",
         parent = "core.1_default",
         tiles = {
            default = tile(1, 472),
            fog = tile(1, 530),
            tunnel = tile(1, 103),
            wall = tile(1, 473),
            room = chance(1, 132, 3, 2),
         }
      },
      {
         id = "2_100",
         parent = "core.2_default",
         tiles = {
            default = tile(2, 472),
            fog = tile(2, 530),
            tunnel = tile(2, 103),
            wall = tile(2, 473),
            room = chance(2, 132, 3, 2),
         }
      },
      {
         id = "1_101",
         parent = "core.1_default",
         tiles = {
            default = tile(1, 477),
            fog = tile(1, 530),
            tunnel = tile(1, 100),
            wall = tile(1, 477),
            room = tile(1, 99),
         }
      },
      {
         id = "2_101",
         parent = "core.2_default",
         tiles = {
            default = tile(2, 477),
            fog = tile(2, 530),
            tunnel = tile(2, 100),
            wall = tile(2, 477),
            room = tile(2, 99),
         }
      },
      {
         id = "1_200",
         parent = "core.1_default",
         tiles = {
            default = tile(1, 200),
            fog = tile(1, 531),
            tunnel = chance(1, 33, 4, 2),
            wall = tile(1, 462),
            room = chance(1, 119, 4, 2),
         }
      },
      {
         id = "2_200",
         parent = "core.2_default",
         tiles = {
            default = tile(2, 200),
            fog = tile(2, 531),
            tunnel = chance(2, 33, 4, 2),
            wall = tile(2, 462),
            room = chance(2, 119, 4, 2),
         }
      },
      {
         id = "1_1",
         parent = "core.1_default",
         tiles = {
            wall = tile(1, -1), -- TODO
            fog = chance(1, 396, 4, 2),
         }
      },
      {
         id = "2_1",
         parent = "core.2_default",
         tiles = {
            wall = tile(2, -1), -- TODO
            fog = chance(2, 396, 4, 2),
         }
      },
      {
         id = "1_4",
         parent = "core.1_default",
         tiles = tiles_field(1),
      },
      {
         id = "2_4",
         parent = "core.2_default",
         tiles = tiles_field(1),
      }
})
