add_requires("entt", "yaml-cpp")

target("Function")
    set_kind("static")

    add_deps("Platform")
    add_files("src/**/*.cpp")
    add_includedirs("src", {public = true})

    add_packages("entt", {public = true})
    add_packages("yaml-cpp")
