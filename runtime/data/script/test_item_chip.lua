local Event = Elona.require("Event")
local Enums = Elona.require("Enums")
local Item = Elona.require("Item")
local Chara = Elona.require("Chara")
local Rand = Elona.require("Rand")

local shopkeeper = 1
local red_putit = 4
local whom_dwell = 28

local short_pic = 378
local tall_pic = 429

local items = {161, 789, 236, 503, 504, 255, 76, 75, 142, 276}

local function setup()
   -- Carpentry, for testing crafting menu
   Chara.player():gain_skill(174, 9999)

   local shop = Chara.create(23, 23, shopkeeper)
   shop.character_role = 1001
   shop.shop_rank = 12

   -- Let's Ragnarok!
   Chara.create(25, 23, whom_dwell)

   for i=0,5 do
      for j=0,10 do
         for k=0,2 do
            local x = 25 - 2 + i
            local y = 25 - 5 + j
            local id
            if Rand.coinflip() then
               id = items[Rand.rnd(7) + 1]
            else
               id = Rand.rnd(500)
            end
            local item = Item.create(x, y, id, 1)

            if id == 503 or id == 504 then
               if Rand.coinflip() then
                  item.param1 = short_pic
               else
                  item.param1 = tall_pic
               end
            end

            if Rand.one_in(6) then
               Chara.create(x, y, red_putit)
            end
         end
      end
   end
end

Event.register(Event.EventKind.GameInitialized, setup)
