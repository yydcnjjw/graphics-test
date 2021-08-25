add_library(glad SHARED glad/src/gl.cc)
target_include_directories(glad
  PUBLIC glad/include)

set(RXCPP_INCLUDE_PATH ${VENDOR_SOURCE_DIR}/RxCpp/Rx/v2/src CACHE PATH "")

set(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(assimp)

set(STB_INCLUDE_PATH ${VENDOR_SOURCE_DIR}/stb/ CACHE PATH "")


set(IMGUI_DIR ${VENDOR_SOURCE_DIR}/imgui)
set(IMGUI_CORE_SOURCE
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/imgui.cpp)

set(IMGUI_BACKEND_OPENGL3_SOURCE
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp CACHE PATH "")

set(IMGUI_BACKEND_GLFW_SOURCE
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp CACHE PATH "")

add_library(imgui SHARED)
target_sources(imgui
  PRIVATE
  ${IMGUI_CORE_SOURCE})

target_include_directories(imgui
  PUBLIC
  ${IMGUI_DIR}
  )
