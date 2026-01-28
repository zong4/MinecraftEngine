add_requires("nlohmann_json")

target("Editor")
    set_kind("binary")
    if is_plat("windows") then
        add_ldflags("/subsystem:console")
    end
    set_default(true)

    add_deps("Engine")
    add_files("src/**/*.cpp")
    add_includedirs("src")
    add_installfiles("assets/Images/**", {prefixdir = "Assets/Images"})
    add_installfiles("assets/Scripts/**", {prefixdir = "Assets/Scripts"})
    add_installfiles("assets/Shaders/**", {prefixdir = "Assets/Shaders"})
    add_installfiles("resources/Audios/**", {prefixdir = "Resources/Audios"})
    add_installfiles("resources/Fonts/**", {prefixdir = "Resources/Fonts"})
    add_installfiles("resources/Icons/**", {prefixdir = "Resources/Icons"})
    add_installfiles("resources/Shaders/**", {prefixdir = "Resources/Shaders"})
    add_installfiles("resources/Textures/**", {prefixdir = "Resources/Textures"})

    add_packages("nlohmann_json")

