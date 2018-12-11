local Event = Elona.require("Event")
local Input = Elona.require("Input")
local Item = Elona.require("Item")
local Chara = Elona.require("Chara")
local GUI = Elona.require("GUI")

local switcher = nil
local talk = nil

local function make_sandbag(x, y, chara_id)
   Item.create(x, y, "core.sand_bag", 1)
   local chara = Chara.create(x, y, chara_id)
   chara:set_flag("IsHungOnSandBag", true)
   return chara
end

local choices = {
   "Dood.",
   "Okay.",
   { index = 99, text = "My choice." }
}

local function query_dialog()
   GUI.txt("Which? ")
   local index = Input.prompt_choice({"Dialog", "Dialog (Impress)", "Dialog with Chip", "Dialog with Chip (impress)"})
   if index then
      local result
      if index == 1 then
         result = Input.prompt_dialog("The test of ages.", "core.lomias", "Lomias the dude", choices)
      elseif index == 2 then
         result = Input.prompt_dialog("The test of ages.", "core.lomias", "Lomias the dude", 100, 150, choices)
      elseif index == 3 then
         result = Input.prompt_dialog("The test of ages.", 346 + (11 * 1000), "Lomias the dude", choices)
      elseif index == 4 then
         result = Input.prompt_dialog("The test of ages.", 346 + (11 * 1000), "Lomias the dude", 100, 150, choices)
      end
      GUI.txtef("Gold")
      GUI.txt(tostring(result))
   end
end

local dialog = {
   _start = {
      text = "foo",
      inherit_choices = "a"
   },
   a = {
      text = "bar",
      choices = {
         {
            text = "core.locale.test.choice_a",
            node = "a",
         },
         {
            text = "Raw string.",
            node = "core.test:a",
         },
         {
            text = "Test cancel.",
            node = "__end__"
         }
      }
   }
}

local function go()
   -- Begin dialog as if character were bumped into.
   Dialog.start(talk)

   -- Begin dialog from structure.
   Dialog.start_with(talk, dialog)

   -- Begin dialog from data entry.
   Dialog.start_with(talk, "core.whom_dwell_in_the_vanity")
end


local function setup()
   switcher = make_sandbag(25, 23, "core.silver_cat")
   Chara.create(23, 23, "core.shopkeeper").role = 4
   talk = Chara.create(27, 23, "core.putit")
end

Event.register(Event.EventKind.MapInitialized, setup)
Event.register(Event.EventKind.CharaDamaged, function(chara)
                  if chara.index == switcher.index then
                     query_dialog()
                  end
end)
