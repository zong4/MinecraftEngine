add_requires("gtest")

target("Test")
    set_kind("binary")

    add_deps("Function")
    add_files("src/**.cpp")
    add_includedirs("src")
    add_includedirs("include", {public = true})

    add_packages("gtest")