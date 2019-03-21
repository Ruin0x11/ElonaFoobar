require "tests/lua/support/minctest"

local Config = Elona.require("Config")

lrun("test Config.register", function()
        print("run the test")
        lok(pcall(function() Config.register("support/config_def.hcl") end))
        lok(not pcall(function() Config.register("support/config_def.hcl") end))
end)
