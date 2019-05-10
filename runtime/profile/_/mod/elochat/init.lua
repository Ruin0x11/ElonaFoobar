local Chara = Elona.require("Chara")
local Config = Elona.require("Config")
local Event = Elona.require("Event")
local GUI = Elona.require("GUI")
local I18N = Elona.require("I18N")
local Input = Elona.require("Input")
local Net = Elona.require("Net")
local Util = Elona.require("Util")
local World = Elona.require("World")
local string = Elona.require("string")

local Elochat = {}

local chat_deny = false
local chat_turn = 0
local last_chat = 0

local function is_japanese()
   return Config.get_string("core.language.language") == "jp"
end

local function is_enabled()
   return Config.get_bool("elochat.net.use_network")
end

local function init_store()
   -- TODO: ensure this callback is only run once per savefile.
   if Store.global.date_when_vote_available == nil then
      Store.global.date_when_vote_available = 0

      chat_deny = false
      chat_turn = 0
      last_chat = 0
   end
end

local function get_endpoint(jp, en)
   local domain = "http://" .. Config.get_string("elochat.net.server_domain")

   if en == nil or is_japanese() then
      return domain .. jp
   else
      return domain .. en
   end
end

function Elochat.get_text()
   local url = get_endpoint("/text.txt")
   local buf = Net.http_get(url)
   if buf == nil then
      return nil
   end

   local text = {}
   local lines = string.split(buf, "\n")

   for i=3, 4 do
      local line = lines[i]
      text[i-2] = string.split(line, "%")[1]
   end

   return text
end

function Elochat.get_vote()
   local url = get_endpoint("/vote.txt", "/voteen.txt")
   local buf = Net.http_get(url)
   if buf == nil then
      return nil
   end

   local aliases = {}
   local lines = string.split(buf, "\n")

   for _, line in ipairs(lines) do
      local fields = string.split(line, "<>")
      local name = Util.from_shift_jis(fields[2])
      local votes = tonumber(fields[3])
      aliases[#aliases+1] = { name, votes }
   end

   return aliases
end

function Elochat.get_log()
   local url = get_endpoint("/log.txt", "/logen.txt")
   local buf = Net.http_get(url)
   if buf == nil then
      return nil
   end

   local log = {}
   local lines = string.split(buf, "\n")
   local start = tonumber(string.split(lines[1], "<C>")[1])
   local latest = 0

   for i=3, #lines-2 do
      if i > 100 then
         break
      end

      local line = lines[i]
      local fields = string.split(line, "%%")

      local id = tonumber(fields[1])

      if i == 3 and id > latest then
         latest = id
      end
      if i == 4 and last_chat == 0 then
         break
      end
      if id <= last_chat then
         break
      end

      local date = fields[2]
      local text = fields[3]

      local kind = text:sub(0, 4)
      local message = Util.from_shift_jis(text:sub(5))

      log[#log+1] = { id, date, kind, message }
   end

   last_chat = latest

   return log
end

local function send_handler(cb)
   local stat = Net.is_connected()
   if not stat then
      GUI.txt("elochat.locale.need_internet")
      return
   end

   local resp_code = cb()

   if resp_code ~= 200 then
      GUI.txt("elochat.locale.failed_to_send")
   end
end

function Elochat.register_alias(alias)
   if false then
      return
   end
   send_handler(function()
         local url = get_endpoint("/cgi-bin/vote/votec.cgi?no=1&mode=wri",
                                  "/cgi-bin/vote/votec.cgi?no=0&mode=wri")
         url = url .. "&vote=" .. Util.to_shift_jis(alias)
         local _, resp_code = Net.http_get(url)
         return resp_code
   end)
end

function Elochat.send_vote(number)
   if false then
      return
   end
   send_handler(function()
         local url = get_endpoint("/cgi-bin/vote/votec.cgi?no=1&mode=wri",
                                  "/cgi-bin/vote/votec.cgi?no=0&mode=wri")
         url = url .. "&namber=" .. tostring(number)
         local _, resp_code = Net.http_get(url)
         return resp_code
   end)
end

local function fix_input_chat(message)
   if not is_japanese() then
      message = message:gsub("fucking", "nyoro~n")
      message = message:gsub("fuck", "nyou talk funny")
   end

   message = message:gsub(" ", "+")
   message = message:gsub("%%", "per")
   message = message:gsub("&", "and")
   message = message:gsub("<", "")
   message = message:gsub(">", "")

   return message
end

function Elochat.send_chat(kind, message)
   if chat_deny then
      if chat_turn > 20 then
      else
         GUI.txt("elochat.locale.chat.wait")
         return
      end
   end

   message = fix_input_chat(message)

   send_handler(function()
         local url = get_endpoint("/cgi-bin/wtalk/wtalk2.cgi?mode=regist",
                                  "/cgi-bin/wtalken/wtalk2.cgi?mode=regist")
         url = url .. "&comment=" .. kind .. Util.to_shift_jis(message)
         local body, resp_code = Net.http_get(url)
         return resp_code
   end)
   chat_turn = 0
   chat_deny = true
end


local function update_chat_idle()
   if is_enabled() and Config.get_bool("elochat.net.constantly_connect") then
      chat_turn = chat_turn + 1
      if chat_turn == Config.get_int("elochat.net.chat_idle_delay") then
         local log = Elochat.get_log()
         if log == nil then
            return
         end

         for _, line in ipairs(log) do
            local show = not Config.get_bool("elochat.net.filter_chat") or line.kind == "chat"

            if show then
               -- BUG: GUI.txt_colored(nil, "Yellow")
               GUI.txt_colored(line[4], "Yellow")
            end
         end
      end
   end
end

local function prompt_ok()
   Input.prompt_choice({
         { I18N.get("elochat.locale.ok"), "y" }
   })
end


local function activate_voting_box()
   GUI.snd("core.chat");

   local text = Elochat.get_text()
   if text == nil then
      GUI.txt("Could not connect to the server.")
      return
   end

   if is_japanese() then
      GUI.txt(text[1])
   else
      GUI.txt(text[2])
   end

   local aliases = Elochat.get_vote()
   if aliases == nil then
      GUI.txt("Could not connect to the server.")
      return
   end

   if Store.global.date_when_vote_available > World.data.date:hours() then
      GUI.txt("elochat.locale.vote.need_to_wait", World.data.date:hours())
      aliases = aliases
   else
      GUI.txt("elochat.locale.vote.which")
   end

   local finished = false

   while not finished do
      local result = Input.query_voting_box(I18N.get("elochat.locale.vote.submit"), aliases)
      if result == nil then
         finished = true
      elseif result == -999 then
         local alias = Chara.player().title .. I18N.space .. Chara.player().basename
         Elochat.register_alias(alias)
         prompt_ok()
      else
         if Store.global.date_when_vote_available > World.data.date:hours() then
            GUI.snd("core.fail1")
            GUI.txt("elochat.locale.vote.need_to_wait")
         else
            Store.global.date_when_vote_available = World.data.date:hours() + 24 * 7
            GUI.txt("elochat.locale.vote.like", aliases[result])
            GUI.txt("elochat.locale.vote.you_vote")
            Elochat.send_vote(result)
            World.modify_ranking(6, 100, 5)
         end
      end
   end
end

Event.register("core.player_turn_started", function() chat_turn = 10 end)
Event.register("core.input_frame_passed", update_chat_idle)
Event.register("core.chat_message_sent", function(e)
                  local message = I18N.get("elochat.locale.chat.chat", Chara.player().title, Chara.player().basename, e.message)
                  Elochat.send_chat("chat", message)
end)
Event.register("core.wish_ended", function(e)
                  if false then
                     return
                  end
                  local message = I18N.get("elochat.locale.chat.wish", Chara.player().title, Chara.player().basename, e.wish_text, e.log_output)
                  Elochat.send_chat("wish", message)
end)
Event.register("core.player_died", function(e)
                  if false then
                     return
                  end
                  if not e.buried then
                     local message = I18N.get("elochat.locale.chat.dead", Chara.player().title, Chara.player().basename, e.map_name, e.death_cause, e.last_words)
                     Elochat.send_chat("dead", message)
                  end
end)
Event.register("core.mod_store_created", init_store)


return { Elochat = Elochat }
