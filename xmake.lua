add_rules("mode.debug", "mode.release")

add_requires("glfw", "glm", "vulkan-headers", "vulkan-loader")

target("DreamsEngine")
  set_kind("binary")
  set_languages("c++26")
  add_files("src/**.cpp")
  add_includedirs("include")
  add_packages("glfw", "glm", "vulkan-headers", "vulkan-loader")
