set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)

add_executable(graphics-test
  main.cc
  ${IMGUI_BACKEND_OPENGL3_SOURCE}
  ${IMGUI_BACKEND_GLFW_SOURCE})

target_link_libraries(graphics-test
  OpenGL::GL
  Vulkan::Vulkan
  glfw
  glad
  glm
  assimp
  spdlog::spdlog
  imgui
  )

target_include_directories(graphics-test
  SYSTEM
  PRIVATE
  ${RXCPP_INCLUDE_PATH}
  ${STB_INCLUDE_PATH}
  )
