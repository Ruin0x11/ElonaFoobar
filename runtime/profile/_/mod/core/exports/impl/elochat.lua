local Config = Elona.require("Config")
local Event = Elona.require("Event")
local GUI = Elona.require("GUI")
local Net = Elona.require("Net")
local Util = Elona.require("Util")
local string = Elona.require("string")

local Elochat = {}

local chat_deny = false
local chat_turn = 0
local last_chat = 0

local function is_japanese()
   return Config.get_string("core.language.language") == "jp"
end

function Elochat.dood()
   print("asd")
end

function Elochat.go()
   local t = Elona.require("Debug").traceback
   local log = Elochat.get_log()

   for _, line in ipairs(log) do
      local skip = false -- line.kind == "chat" and not Config.get_bool("core.net.netchat")

      if not skip then
         if not is_japanese() then
            line[4] = line[4]:gsub("&quot;", "\"")
         end

         -- BUG: GUI.txt(nil, "Yellow") ?
         GUI.txt(line[4], "Yellow")
      end
   end
end

local function update_chat_idle()
   chat_turn = chat_turn + 1

   if chat_turn == Config.get_int("core.net.chat_idle_delay") then
      local log = Elochat.get_log()

      for _, line in ipairs(log) do
         local skip = line.kind == "chat" and not Config.get_bool("core.net.netchat")

         if not skip then
            if not is_japanese() then
               line.message = line.message:gsub("&quot;", "\"")
            end

            GUI.txt(line.message, "Yellow")
         end
      end
   end
end

local function is_enabled()
   return Config.get_bool("core.net.enabled")
end

local function get_endpoint(jp, en)
   local domain = "http://" .. "localhost:4567" -- Config.get_string("core.net.server_domain")

   if is_japanese() then
      return domain .. jp
   else
      return domain .. en
   end
end

function Elochat.get_alias()
   local url = get_endpoint("/vote.txt", "/voteen.txt")
   local buf = Net.http_get(url)

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
         print("no last")
         break
      end
      if id <= last_chat then
         print("last " .. latest)
         break
      end

      local date = fields[2]
      local text = fields[3]

      local kind = text:sub(0, 4)
      local message = Util.from_shift_jis(text:sub(5))

      log[#log+1] = { id, date, kind, message }
   end

   print("STORE " .. latest)
   last_chat = latest

   return log
end

function Elochat.send_vote()
   local url = get_endpoint("/cgi-bin/vote/votec.cgi?no=1&mode=wri", "/cgi-bin/vote/votec.cgi?no=0&mode=wri")
end

function Elochat.send_chat()
   local url = get_endpoint("/cgi-bin/wtalk/wtalk2.cgi?mode=regist", "/cgi-bin/wtalken/wtalk2.cgi?mode=regist")
end


-- Event.register("core.input_frame_passed", mod_chat_deny)


return Elochat
