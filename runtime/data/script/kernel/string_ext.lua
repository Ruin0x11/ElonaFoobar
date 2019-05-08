local string = require "string"

function string.split(s, sep)
    local sep, fields = sep or "\n", {}
    local pattern = string.format("([^%s]+)", sep)
    s:gsub(pattern, function(c) fields[#fields+1] = c end)
    return fields
end

return string
