add_requires("stb")
add_requires("glfw", "glad")
add_requires("imgui", {configs = {glfw_opengl3 = true}})

target("Platform")
    set_kind("static")

    add_deps("Core")
    add_files("src/**/*.c")
    add_files("src/**/*.cpp")
    add_includedirs("src")
    add_includedirs("include", {public = true})

    if is_plat("windows") then
        add_syslinks("ole32", "comdlg32")
    end
    add_packages("stb");
    add_packages("glfw");
    add_packages("glad", {public = true}); -- todo: fix
    add_packages("imgui", {public = true});
