local Config = Elona.require("Config")

local Exports = {}

Exports.eating_effect = require("exports/eating_effect")

Exports.impl = require("exports/impl")

require("i18n/init")

Config.register("config_def.hcl")

return {
   Exports = Exports
}
