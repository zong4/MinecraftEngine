set_project("MinecraftEngine")

-- Basic Settings
set_version("1.0.0")
set_languages("c++20")

-- Setting Warnings as Errors
add_cflags("-Werror")
add_cxxflags("-Werror")

-- Build Modes
add_rules("mode.debug", "mode.release")
if is_mode("debug") then
    add_cxxflags("-O0", "-g")
    add_defines("DEBUG")
else
    add_cxxflags("-O2")
end

-- Global Definitions
add_defines('PROJECT_ROOT=R"($(projectdir)/)"')

-- VS Code Specific Settings
add_rules("plugin.compile_commands.autoupdate", {outputdir = ".vscode"})

-- Sub-Projects
includes("Engine")
includes("Editor")