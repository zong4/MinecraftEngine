-- Core
add_requires("spdlog", "glm")

-- Platform
add_requires("stb")
add_requires("glfw", "glad")
add_requires("imgui v1.92.1-docking", {configs = {glfw_opengl3 = true}})

-- Function
add_requires("entt", "yaml-cpp")

-- Core Target
target("Core")
    set_kind("static")
  
    add_headerfiles("Core/**/*.hpp")
    add_files("Core/**/*.cpp")
    add_includedirs("Core", {public = true})
    
    add_packages("spdlog")
    add_packages("glm", {public = true})

-- Platform Target
target("Platform")
    set_kind("static")

    add_deps("Core")
    add_headerfiles("Platform/**/*.h")
    add_headerfiles("Platform/**/*.hpp")
    add_files("Platform/**/*.c")
    add_files("Platform/**/*.cpp")
    add_includedirs("Platform", {public = true})

    if is_plat("windows") then
        add_syslinks("ole32", "comdlg32")
    end
    add_packages("stb");
    add_packages("glfw", {public = true});
    add_packages("glad", {public = true});
    add_packages("imgui", {public = true});

-- Function Target
target("Function")
    set_kind("static")

    add_deps("Platform")
    add_headerfiles("Function/**/*.hpp")
    add_files("Function/**/*.cpp")
    add_includedirs("Function", {public = true})

    add_packages("entt", {public = true})
    add_packages("yaml-cpp")

-- Engine Target
target("Engine")
    set_kind("static")

    add_deps("Function")
    add_files("*.cpp")