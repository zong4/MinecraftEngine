add_requires("spdlog", "glm")

target("Core")
    set_kind("static")
  
    add_files("src/**/*.cpp")
    add_includedirs("src")
    add_includedirs("include", {public = true})
    
    add_packages("spdlog")
    add_packages("glm", {public = true})
