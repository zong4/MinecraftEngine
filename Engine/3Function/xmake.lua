add_requires("entt")
add_requires("nlohmann_json")
add_requires("bullet3")
add_requires("miniaudio")

target("Function")
    set_kind("static")

    add_deps("Platform")
    add_files("src/**/*.cpp")
    add_includedirs("src")
    add_includedirs("include", {public = true})

    add_packages("nlohmann_json")
    add_packages("entt", {public = true})
    add_packages("bullet3", {public = true})
    add_packages("miniaudio", {public = true})
