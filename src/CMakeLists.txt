set(CMAKE_INCLUDE_CURRENT_DIR ON)

# find_package(Vulkan REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)

add_executable(my-graphics
  main.cc
  ${IMGUI_BACKEND_OPENGL3_SOURCE}
  ${IMGUI_BACKEND_GLFW_SOURCE})

set_property(TARGET my-graphics
  PROPERTY CXX_STANDARD 20)

target_link_libraries(my-graphics
  Boost::headers
  OpenGL::GL
  # Vulkan::Vulkan
  glfw
  glad
  assimp
  spdlog::spdlog
  imgui
  ${CMAKE_DL_LIBS}
  skia
  )

if(UNIX)
  target_link_libraries(my-graphics glm)
else(WIN32)
  target_link_libraries(my-graphics glm::glm)
endif(UNIX)

get_target_property(SKIA_INCLUDE_PATH skia INCLUDE_DIRECTORIES)

target_include_directories(my-graphics
  SYSTEM
  PRIVATE
  ${RXCPP_INCLUDE_PATH}
  ${STB_INCLUDE_PATH}
  ${SKIA_INCLUDE_PATH}
  )
