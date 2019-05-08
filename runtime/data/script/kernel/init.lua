--- WARNING: This file is always implicitly loaded by C++.
--- Don't change anything here unless you know what you're doing!

Elona.core.Debug.inspect = require("inspect").inspect
Elona.core.Debug.traceback = require("debug").traceback
Elona.core.HCL = require "hclua"
Elona.core.Iter = require "iter"
Elona.core.ReadOnly = require "readonly"
Elona.core.string = require "string_ext"

Elona.core.Math = require "math_ext"
Elona.core.table = require "table_ext"
Elona.core.coroutine = require "coroutine"
Elona.core.Net = require "net"
Elona.core.JSON = require "json"

_LOADED = true
