add_requires("entt")

target("Function")
    set_kind("static")

    add_deps("Platform")
    add_files("src/**/*.cpp")
    add_includedirs("src")
    add_includedirs("include", {public = true})

    add_packages("entt", {public = true})
