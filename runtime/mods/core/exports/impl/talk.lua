local GUI = Elona.require("GUI")
local I18N = Elona.require("I18N")
local Input = Elona.require("Input")
local Internal = Elona.require("Internal")

local State = {}

function State.new(talk)
   local s = {}
   s.talk = talk
   s.text_index = 1
   s.node_id = "_start"

   for k, v in pairs(State) do
      s[k] = v
   end

   return s
end

local function print_error(text)
   GUI.txtef("Red")
   GUI.txt(text)
end

local function process_choices(node, state)
   local text
   local choices = {}

   local function add_choices_from_list()
      for i, choice in ipairs(node.choices) do
         choices[i] = choice.text
      end
   end

   if type(node.text) == "table" then
      text = node.text[state.text_index]
      assert(text)

      if state.text_index == #node.text then
         add_choices_from_list()
      else
         choices[1] = I18N.get("core.locale.ui.more")
      end
   else
      add_choices_from_list()
   end

   return text, choices
end

local function run_talk(chara, talk, state)
   local node

   while true do
      if state.node_id == "__end__" then
         return
      end

      local show_impress = chara.quality ~= "Special" or chara.index < 16
      local speaker_name = Internal.talk_speaker_name(chara)

      node = talk[state.node_id]
      if node == nil then
         print_error("No such node " .. state.node_id)
         return
      end

      if node.callback_before then
         node.callback_before()
      end

      -- This may either be chara.image (an integer), or a portrait ID (a string).
      local image
      if chara.portrait ~= "" then
         image = chara.portrait
      else
         image = chara.image
      end

      local text, choices = process_choices(node, state)
      local choice_index
      if show_impress then
         choice_index = Input.prompt_talk(text, image, speaker_name, choices, chara.impress, chara.interest)
      else
         choice_index = Input.prompt_talk(text, image, speaker_name, choices)
      end

      if not choice_index then
         GUI.txtef("Gold")
         GUI.txt("Canceled.")
         return
      end

      if node.callback_after then
         node.callback_after()
      end

      if type(node.text) == "table" and state.text_index < #node.text then
         assert(choice_index == 1)
         state.text_index = state.text_index + 1
      else
         local choice = choices[choice_index]
         assert(choice)
         state.node_id = choice.node
         state.text_index = 1
      end
   end
end


local Talk = {}

function Talk.start_with(chara, talk)
   if not Internal.talk_setup(chara) then
      return
   end

   local state = State.new(talk)
   local _, err = pcall(run_talk(chara, talk, state))
   if err then
      print_error(err)
   end

   Internal.talk_stop(chara)
end

return Talk
