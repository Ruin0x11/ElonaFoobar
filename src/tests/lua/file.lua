require "tests/lua/support/minctest"

local File = Elona.require("File")

local TEST_PATH = "__USER__/テスト"

local function cleanup()
   File.remove_path(TEST_PATH)
   File.create_directory(TEST_PATH)
end

lrun("test File.create_directory", function()
        cleanup()

        lok(not File.exists(TEST_PATH .. "/テスト"))
        lok(not File.is_directory(TEST_PATH .. "/テスト"))
        File.create_directory(TEST_PATH .. "/テスト")
        lok(File.exists(TEST_PATH .. "/テスト"))
        lok(File.is_directory(TEST_PATH .. "/テスト"))

        lok(not pcall(function() File.create_directory("./テスト") end))
        lok(not pcall(function() File.create_directory("__BUILTIN__/テスト") end))
end)

lrun("test File.write_file", function()
        cleanup()

        lok(not File.exists(TEST_PATH .. "/テスト"))
        lok(not File.is_directory(TEST_PATH .. "/テスト"))
        File.write_file(TEST_PATH .. "/テスト", "テスト" )
        lok(File.exists(TEST_PATH .. "/テスト"))
        lok(not File.is_directory(TEST_PATH .. "/テスト"))

        File.create_directory(TEST_PATH .. "/テスト2")
        lok(not pcall(function() File.write_file(TEST_PATH .. "/テスト2", "テスト") end))
end)

lrun("test File.write_file (append)", function()
        cleanup()

        File.write_file(TEST_PATH .. "/テスト", "テスト\nテスト")
        local text = File.read_file_to_string(TEST_PATH .. "/テスト")
        lequal(text, "テスト\nテスト")

        File.write_file(TEST_PATH .. "/テスト", "テスト\nテスト", true)
        text = File.read_file_to_string(TEST_PATH .. "/テスト")
        lequal(text, "テスト\nテストテスト\nテスト")

        File.write_file(TEST_PATH .. "/テスト", "テスト\nテスト")
        text = File.read_file_to_string(TEST_PATH .. "/テスト")
        lequal(text, "テスト\nテスト")
end)

lrun("test File.copy_path", function()
        cleanup()

        lok(not File.exists(TEST_PATH .. "/テスト"))
        lok(not File.exists(TEST_PATH .. "/テスト2"))
        File.write_file(TEST_PATH .. "/テスト", "テスト")
        lok(File.exists(TEST_PATH .. "/テスト"))
        File.copy_path(TEST_PATH .. "/テスト", TEST_PATH .. "/テスト2")
        lok(File.exists(TEST_PATH .. "/テスト2"))
        lok(not File.is_directory(TEST_PATH .. "/テスト2"))

        lok(not pcall(function() File.copy_path(TEST_PATH .. "/テスト", TEST_PATH .. "/テスト2") end))
        lok(pcall(function() File.copy_path(TEST_PATH .. "/テスト", TEST_PATH .. "/テスト2", true) end))
        lok(not pcall(function() File.copy_path(TEST_PATH .. "/テスト", "__BUILTIN__/..") end))
end)

lrun("test File.copy_path (directory)", function()
        cleanup()

        lok(not File.exists(TEST_PATH .. "/テスト2"))
        File.create_directory(TEST_PATH .. "/テスト", "テスト")
        File.write_file(TEST_PATH .. "/テスト/テスト", "テスト")
        File.copy_path(TEST_PATH .. "/テスト", TEST_PATH .. "/テスト2")
        lok(File.exists(TEST_PATH .. "/テスト2"))
        lok(File.is_directory(TEST_PATH .. "/テスト2"))
        lok(File.exists(TEST_PATH .. "/テスト2/テスト"))
end)

lrun("test File.read_file_to_string", function()
        cleanup()

        File.write_file(TEST_PATH .. "/テスト", "テスト\nテスト")
        local text = File.read_file_to_string(TEST_PATH .. "/テスト")
        lequal(text, "テスト\nテスト")

        File.create_directory(TEST_PATH .. "/テスト2")
        lok(not pcall(function() File.read_file_to_string(TEST_PATH .. "/テスト2") end))
        lok(not pcall(function() File.read_file_to_string("__BUILTIN__/") end))
end)

lrun("test File.remove_path", function()
        cleanup()

        lok(not pcall(function() File.remove_path(".") end))
        lok(not pcall(function() File.remove_path("__BUILTIN__") end))
        lok(not pcall(function() File.remove_path("__BUILTIN__/") end))
        lok(not pcall(function() File.remove_path("__BUILTIN__/..") end))
        lok(not pcall(function() File.remove_path("__USER__/") end))
        lok(not pcall(function() File.remove_path("__BUILTIN__/graphic") end))
        lok(not pcall(function() File.remove_path("__BUILTIN__/data") end))
        lok(not pcall(function() File.remove_path("__BUILTIN__/locale") end))
        lok(not pcall(function() File.remove_path("__BUILTIN__/log") end))
        lok(not pcall(function() File.remove_path("__BUILTIN__/profile") end))
        lok(not pcall(function() File.remove_path("__BUILTIN__/profile/default/mod") end))
        lok(not pcall(function() File.remove_path("__BUILTIN__/profile/default/save") end))
        lok(not pcall(function() File.remove_path("__BUILTIN__/..") end))
        lok(not pcall(function() File.remove_path("__BUILTIN__/./..") end))
        lok(not pcall(function() File.remove_path("__core__") end))
        lok(not pcall(function() File.remove_path("__core__/") end))
        lok(not pcall(function() File.remove_path("__core__/..") end))
        lok(not pcall(function() File.remove_path("__core__/init.lua") end))
        lok(not pcall(function() File.remove_path("__core__/graphic") end))
        lok(not pcall(function() File.remove_path("/") end))
        lok(not pcall(function() File.remove_path("C:\\") end))

        File.write_file(TEST_PATH .. "/テスト", "テスト")
        lok(pcall(function() File.remove_path(TEST_PATH .. "/テスト") end))
        File.create_directory(TEST_PATH .. "/テスト")
        lok(pcall(function() File.remove_path(TEST_PATH .. "/テスト") end))
end)


lrun("test File.filename", function()
        cleanup()

        lequal(File.filename("テスト"), "テスト")
        lequal(File.filename("テスト/"), ".")
        lequal(File.filename("__BUILTIN__/テスト/テスト.txt"), "テスト.txt")
        lequal(File.filename("テスト/テスト.txt"), "テスト.txt")
        lequal(File.filename("テスト.txt"), "テスト.txt")
        lequal(File.filename(""), "")
end)
