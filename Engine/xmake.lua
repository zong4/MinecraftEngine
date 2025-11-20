includes("Core")
includes("Platform")
includes("Function")

target("Engine")
    set_kind("static")

    add_deps("Function")
    add_files("*.cpp")
    