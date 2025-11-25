includes("Core")
includes("Platform")
includes("Function")
includes("Test")

target("Engine")
    set_kind("static")

    add_deps("Function")
    if is_mode("debug") then
        add_deps("Test")
    end
    add_files("*.cpp")
    