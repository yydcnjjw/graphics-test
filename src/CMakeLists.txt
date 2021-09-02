set(CMAKE_INCLUDE_CURRENT_DIR ON)

# find_package(Vulkan REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
# find_package(cairomm REQUIRED)

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
  glm::glm
  glad
  assimp
  spdlog::spdlog
  imgui
  ${CMAKE_DL_LIBS}
  # ${cairomm_LIBRARIES}
  # skia
  )

target_include_directories(my-graphics
  SYSTEM
  PRIVATE
  ${RXCPP_INCLUDE_PATH}
  ${STB_INCLUDE_PATH}
  # ${cairomm_INCLUDE_DIRS}
  )

if(MINGW)
  target_link_libraries(my-graphics
    ssp)
  add_custom_command(TARGET my-graphics POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:imgui>
    $<TARGET_FILE:glad>
    $<TARGET_FILE:spdlog::spdlog>
    $<TARGET_FILE:assimp::assimp>
    # $<TARGET_FILE:ssp>
    $<TARGET_FILE_DIR:my-graphics>)
  install(TARGETS my-graphics)
endif(MINGW)
