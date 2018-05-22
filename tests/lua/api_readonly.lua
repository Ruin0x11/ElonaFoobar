require "tests/lua/support/minctest"

stds = {}
files = {} -- so the below dofile doesn't error out

-- Load all global tables that are defined inside the .luacheckrc, including "stds"
dofile(".luacheckrc")

local function attempt_assignment(table_name)
   Elona[table_name] = nil
end

local function test_readonly(table_name)
   lrun("test that Elona." .. table_name .. " is read-only", function()
           local could_overwrite = pcall(attempt_assignment, table_name)
           lok((not could_overwrite), "Table Elona." .. table_name .. " can be overwritten")
   end)
end

local function test_all_tables_defined()
   lrun("test that all API tables are defined in .luacheckrc ", function()
           for table_name, _ in pairs(stds.elona.read_globals.Elona.fields) do
              lok(Elona[table_name] ~= nil,
                  "Table Elona." .. table_name .. " defined in .luacheckrc but undefined in Lua")
           end
   end)
   lrun("test that all API tables that are defined in .luacheckrc exist ", function()
           for table_name, _ in pairs(Elona) do
              lok(stds.elona.read_globals.Elona.fields[table_name] ~= nil,
                  "Table Elona." .. table_name .. " not defined in .luacheckrc"
              )
           end
   end)
end

for table_name, _ in pairs(stds.elona.read_globals.Elona.fields) do
   test_readonly(table_name)
end

test_all_tables_defined()

assert(lresults())
