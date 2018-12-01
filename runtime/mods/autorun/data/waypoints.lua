local roles_to_names = {
   [5] = "wizard",
   [7] = "trainer",
   [8] = "informer",
   [9] = "bartender",
   [10] = "arena_master",
   [11] = "pet_arena_master",
   [12] = "healer",
   [15] = "wizard",
   [17] = "slave_trader",
   [19] = "sister",
   [21] = "wizard",
   [22] = "horse_master",
   [23] = "caravan_master",
   [1001] = "blacksmith",
   [1002] = "food_vendor",
   [1003] = "baker",
   [1004] = "magic_vendor",
   [1005] = "innkeeper",
   [1006] = "general_vendor",
   [1007] = "blackmarket",
   [1008] = "goods_vendor",
   [1009] = "trader",
   [1011] = "sales_person_a",
   [1012] = "sales_person_c",
   [1013] = "sales_person_b",
   [1014] = "fishery",
   [1015] = "moyer",
   [1016] = "miral",
   [1017] = "dye_vendor",
   [1018] = "souvenir_vendor",
   [1019] = "younger_sister_of_mansion",
   [1020] = "spell_writer",
   [1021] = "the_fence",
   [1022] = "street_vendor",
}

local names_to_roles = {}
for role, name in pairs(roles_to_names) do
   names_to_roles[name] = role
end

local function generate_waypoints(obj)
   obj.type = "autorun.waypoints"

   for _, v in pairs(obj.waypoints) do
      v.localized_name = v[1]
      v.target = { _type = "character", role = names_to_roles[v[1]] }
      v.pos = v[2]

      v[1] = nil
      v[2] = nil
   end
   data:add({obj})
end


data:define_type("waypoints")

-- vernis
generate_waypoints(
   {
      name = "vernis",
      map_id = "core.vernis",
      map_level = 1,
      waypoints = {
         { "trainer",        { x = 27, y = 16 } },
         { "magic_vendor",   { x = 7,  y = 26 } },
         { "innkeeper",      { x = 14, y = 25 } },
         { "baker",          { x = 22, y = 26 } },
         { "general_vendor", { x = 10, y = 15 } },
         { "trader",         { x = 39, y = 27 } },
         { "blacksmith",     { x = 14, y = 12 } },
         { "fishery",        { x = 47, y = 9  } },
         { "wizard",         { x = 28, y = 16 } },
         { "bartender",      { x = 38, y = 27 } },
         { "healer",         { x = 6,  y = 25 } },
         { "informer",       { x = 25, y = 16 } },
      }
   }
)

-- derphy
generate_waypoints(
   {
      name = "derphy",
      map_id = "core.derphy",
      map_level = 1,
      waypoints = {
         { "trader",         { x = 10, y = 17 } },
         { "bartender",      { x = 38, y = 27 } },
         { "general_vendor", { x = 13, y = 3  } },
         { "innkeeper",      { x = 29, y = 23 } },
         { "goods_vendor",   { x = 26, y = 7  } },
         { "blackmarket",    { x = 30, y = 4  } },
         { "slave_trader",   { x = 29, y = 4  } },
         { "blacksmith",     { x = 10, y = 6  } },
         { "arena_master",   { x = 7,  y = 15 } },
         { "trainer",        { x = 13, y = 18 } },
         { "wizard",         { x = 5,  y = 26 } },
         { "informer",       { x = 3,  y = 28 } },
      }
   }
)

-- thieves guild
generate_waypoints(
   {
      name = "thieves_guild",
      map_id = "core.derphy",
      map_level = 3,
      waypoints = {
         { "trainer",     { x = 3,  y = 6  } },
         { "wizard",      { x = 3,  y = 12 } },
         { "blackmarket", { x = 5,  y = 18 } },
         { "blackmarket", { x = 27, y = 13 } },
         { "the_fence",   { x = 21, y = 19 } },
      }
   }
)

-- palmia
generate_waypoints(
   {
      name = "palmia",
      map_id = "core.palmia",
      map_level = 1,
      waypoints = {
         { "bartender",      { x = 42, y = 27 } },
         { "healer",         { x = 34, y = 3  } },
         { "arena_master",   { x = 22, y = 31 } },
         { "general_vendor", { x = 48, y = 18 } },
         { "innkeeper",      { x = 30, y = 17 } },
         { "goods_vendor",   { x = 48, y = 3  } },
         { "blacksmith",     { x = 42, y = 17 } },
         { "baker",          { x = 11, y = 14 } },
         { "magic_vendor",   { x = 41, y = 3  } },
         { "trader",         { x = 41, y = 28 } },
         { "wizard",         { x = 7,  y = 2  } },
         { "wizard",         { x = 6,  y = 2  } },
         { "trainer",        { x = 30, y = 27 } },
         { "wizard",         { x = 32, y = 27 } },
         { "informer",       { x = 29, y = 28 } },
      }
   }
)

-- lumiest
generate_waypoints(
   {
      name = "lumiest",
      map_id = "core.lumiest",
      map_level = 1,
      waypoints = {
         { "bartender",      { x = 41, y = 42 } },
         { "healer",         { x = 10, y = 16 } },
         { "general_vendor", { x = 47, y = 30 } },
         { "innkeeper",      { x = 24, y = 47 } },
         { "blacksmith",     { x = 37, y = 30 } },
         { "baker",          { x = 37, y = 12 } },
         { "magic_vendor",   { x = 6,  y = 15 } },
         { "trader",         { x = 33, y = 43 } },
         { "fishery",        { x = 47, y = 12 } },
         { "trainer",        { x = 21, y = 28 } },
         { "wizard",         { x = 21, y = 30 } },
         { "informer",       { x = 23, y = 38 } },
      }
   }
)

-- mages guild
generate_waypoints(
   {
      name = "mages_guild",
      map_id = "core.lumiest",
      map_level = 3,
      waypoints = {
         { "spell_writer", { x = 27, y = 8 } },
         { "magic_vendor", { x = 22, y = 8 } },
         { "healer",       { x = 3,  y = 9 } },
         { "trainer",      { x = 12, y = 6 } },
         { "wizard",       { x = 3,  y = 3 } },
      }
   }
)

-- yowyn
generate_waypoints(
   {
      name = "yowyn",
      map_id = "core.yowyn",
      map_level = 1,
      waypoints = {
         { "general_vendor", { x = 11, y = 5  } },
         { "innkeeper",      { x = 25, y = 8  } },
         { "goods_vendor",   { x = 7,  y = 8  } },
         { "trader",         { x = 14, y = 14 } },
         { "horse_master",   { x = 35, y = 18 } },
         { "trainer",        { x = 20, y = 14 } },
         { "wizard",         { x = 24, y = 16 } },
         { "informer",       { x = 26, y = 16 } },
      }
   }
)

-- noyel
generate_waypoints(
   {
      name = "noyel",
      map_id = "core.noyel",
      map_level = 1,
      waypoints = {
         { "moyer",          { x = 47, y = 18 } },
         { "bartender",      { x = 40, y = 33 } },
         { "healer",         { x = 44, y = 6  } },
         { "sister",         { x = 44, y = 3  } },
         { "blacksmith",     { x = 19, y = 31 } },
         { "general_vendor", { x = 11, y = 31 } },
         { "innkeeper",      { x = 38, y = 34 } },
         { "baker",          { x = 5,  y = 27 } },
         { "magic_vendor",   { x = 56, y = 5  } },
         { "trader",         { x = 39, y = 35 } },
         { "trainer",        { x = 18, y = 20 } },
         { "wizard",         { x = 4,  y = 33 } },
         { "informer",       { x = 6,  y = 33 } },
      }
   }
)

-- port kapul
generate_waypoints(
   {
      name = "port_kapul",
      map_id = "core.port_kapul",
      map_level = 1,
      waypoints = {
         { "trader",           { x = 16, y = 17 } },
         { "blacksmith",       { x = 23, y = 7  } },
         { "general_vendor",   { x = 32, y = 14 } },
         { "goods_vendor",     { x = 22, y = 14 } },
         { "blackmarket",      { x = 16, y = 25 } },
         { "food_vendor",      { x = 17, y = 28 } },
         { "magic_vendor",     { x = 22, y = 22 } },
         { "innkeeper",        { x = 35, y = 3  } },
         { "bartender",        { x = 15, y = 15 } },
         { "arena_master",     { x = 26, y = 3  } },
         { "pet_arena_master", { x = 25, y = 4  } },
         { "trainer",          { x = 16, y = 4  } },
         { "wizard",           { x = 14, y = 4  } },
         { "informer",         { x = 17, y = 5  } },
         { "healer",           { x = 27, y = 11 } },
      }
   }
)

-- fighters guild
generate_waypoints(
   {
      name = "fighters_guild",
      map_id = "core.port_kapul",
      map_level = 3,
      waypoints = {
         { "healer",     { x = 28, y = 10 } },
         { "trainer",    { x = 15, y = 10 } },
         { "wizard",     { x = 14, y = 18 } },
         { "blacksmith", { x = 29, y = 15 } },
      }
   }
)

-- larna
generate_waypoints(
   {
      name = "larna",
      map_id = "core.larna",
      map_level = 1,
      waypoints = {
         { "wizard",          { x = 21, y = 23 } },
         { "dye_vendor",      { x = 9,  y = 44 } },
         { "souvenir_vendor", { x = 13, y = 37 } },
         { "bartender",       { x = 24, y = 48 } },
      }
   }
)

-- cyber dome
generate_waypoints(
   {
      name = "cyber_dome",
      map_id = "core.cyber_dome",
      map_level = 1,
      waypoints = {
         { "sales_person_a", { x = 9, y = 16 } },
         { "sales_person_a", { x = 9, y = 8  } },
      }
   }
)

-- miral and garoks workshop
generate_waypoints(
   {
      name = "miral_and_garoks_workshop",
      map_id = "core.miral_and_garoks_workshop",
      map_level = 1,
      waypoints = {
         { "miral", { x = 8, y = 16 } },
      }
   }
)

-- mansion of younger sister
generate_waypoints(
   {
      name = "mansion_of_younger_sister",
      map_id = "core.mansion_of_younger_sister",
      map_level = 1,
      waypoints = {
         { "younger_sister_of_mansion", { x = 12, y = 6 } }
      }
   }
)
