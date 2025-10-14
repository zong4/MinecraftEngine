add_requires("nlohmann_json")

target("Editor")
    set_kind("binary")

    if is_plat("windows") then
        add_ldflags("/subsystem:console")
    end

    add_deps("Engine")
    
    add_headerfiles("Sources/**/*.hpp")
    add_files("Sources/**/*.cpp")
    add_includedirs("Sources")

    add_packages("nlohmann_json")
