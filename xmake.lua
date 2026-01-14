set_project("MinecraftEngine")

-- Basic Settings
set_version("1.0.0")
set_languages("c++20")

-- Setting Warnings as Errors
add_cxxflags("-Werror", {tools = {"gcc", "clang"}}) -- GCC / Clang
add_cflags("-Werror", {tools = {"gcc", "clang"}}) -- GCC / Clang
add_cxxflags("/WX", {tools = "cl"}) -- MSVC
add_cflags("/WX", {tools = "cl"}) -- MSVC

-- Build Modes
add_rules("mode.debug", "mode.release")
if is_mode("debug") then
    set_symbols("debug")
    set_optimize("none")
    add_defines("DEBUG")
else
    set_optimize("fast")
end

-- Global Definitions
add_defines('PROJECT_ROOT=R"($(projectdir)/)"')
add_defines('EDITOR_ROOT=R"($(projectdir)/Editor/)"')
add_defines('FUNCTION_ROOT=R"($(projectdir)/Engine/3Function/)"')

-- VS Code Specific Settings
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

-- Sub-Projects
includes("Engine")
includes("Editor")