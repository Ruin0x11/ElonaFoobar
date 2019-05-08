local socket = require("socket")
local http   = require("socket.http")
local ssl    = require("ssl")

local Net = {}

local function parse_url(url)
   local scheme, domain = url:match("^([a-z]+)://([^/]+)(/.*)$")
   domain = domain:match("^([^:]+)")

   return scheme, domain
end

local function https_create()
   local t = {tcp=socket.tcp()}

   function t:connect(host, port)
      self.tcp:settimeout(15)
      port = 443
      local success, err = self.tcp:connect(host, port)
      if err then
         return nil, err
      end

      local ssl_params = {
         mode = "client",
         protocol = "sslv23",
         verify = "none",
         options = "all",
      }

      self.tcp = socket.try(ssl.wrap(self.tcp, ssl_params))
      local s = socket.try(self.tcp:dohandshake())

      return 1
   end

   function __index (tbl, key)
      return function (proxy, ...)
         local tcp = proxy.tcp
         return tcp[key](tcp,...)
      end
   end

   return setmetatable(t, {__index = __index})
end

local function get_http(url, headers)
   local resp_body = {}

   local _, resp_status, resp_headers = http.request({
      url = url,
      sink = ltn12.sink.table(resp_body),
      headers = headers,
      redirect = true
   })

   return table.concat(resp_body, ""), resp_status, resp_headers
end

local function get_https(url, headers, domain)
   local ip = socket.dns.toip(domain)

   local resp_body = {}
   local _, resp_status, resp_headers = http.request({
      url = url,
      headers = headers,
      sink = ltn12.sink.table(resp_body),
      create = https_create,
      redirect = true
   })

   return table.concat(resp_body, ""), resp_status, resp_headers
end

function Net.http_get(url, headers)
   if not headers and url:match("^http://") then
      local resp_body, resp_status, resp_headers = http.request(url)
      return (resp_body or ""), resp_status, resp_headers
   end

   scheme, domain = parse_url(url)

   if scheme == "http" then
      return get_http(url, headers)
   elseif scheme == "https" then
      return get_https(url, headers, domain)
   else
      return nil, "unknown scheme " .. scheme, nil
   end
end

return Net
