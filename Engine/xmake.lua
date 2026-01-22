includes("1Core")
includes("2Platform")
includes("3Function")
-- includes("0Test")

target("Engine")
    set_kind("static")

    add_deps("Function")
    -- if is_mode("debug") then
    --     add_deps("Test")
    -- end
    add_files("*.cpp")
    