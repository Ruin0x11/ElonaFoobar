require "tests/lua/minctest"

files = {}
stds = {}

-- Load all global tables that are defined inside the .luacheckrc
dofile(".luacheckrc")

local function attempt_assignment(table_name)
   Elona[table_name] = nil
end

local function test_readonly(table_name)
   lrun("test that Elona." .. table_name .. " is read-only", function()
           if pcall(attempt_assignment, table_name) then
              -- No errors occurred trying to assign.
              error("Table Elona." .. table_name .. " can be overwritten")
           end
   end)
end

local function test_all_tables_defined()
   lrun("test that all API tables are defined in .luacheckrc ", function()
           for table_name, mod in pairs(stds.elona.read_globals.Elona.fields) do
              if stds.elona.read_globals.Elona.fields[table_name] == nil then
                 error("Table Elona." .. table_name .. " not defined in .luacheckrc")
              end
           end
   end)
end

lrun("test that root Elona table is read-only", function()
        if pcall(function() Elona = nil end) then
           -- No errors occurred trying to assign.
           error("Table Elona can be overwritten")
        end
end)

for table_name, mod in pairs(stds.elona.read_globals.Elona.fields) do
   test_readonly(table_name)
end

test_all_tables_defined()
