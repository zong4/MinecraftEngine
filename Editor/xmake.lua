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

    add_packages("nlohmann_json")

    if is_mode("release") then
        set_targetdir("../Dist/")
        after_build(function(target)
            local installDir = path.join(os.curdir(), "Dist")
            os.cp("Editor/configs", path.join(installDir, "Configs"))
            os.cp("Editor/assets", path.join(installDir, "Assets"))
            os.cp("Editor/resources", path.join(installDir, "Resources"))
            os.rm(path.join(installDir, "bin"))
        end)
    end
