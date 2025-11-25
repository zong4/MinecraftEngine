add_requires("gtest")

target("Test")
    set_kind("binary")

    add_deps("Function")
    add_files("src/**/*.cpp")

    add_packages("gtest")