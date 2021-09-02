# glad
add_library(glad SHARED glad/src/gl.cc)
target_include_directories(glad
  PUBLIC glad/include)

# rxcpp
set(RXCPP_INCLUDE_PATH ${VENDOR_SOURCE_DIR}/RxCpp/Rx/v2/src CACHE PATH "")

# stb
set(STB_INCLUDE_PATH ${VENDOR_SOURCE_DIR}/stb/ CACHE PATH "")

# imgui
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

# skia
# set(SKIA_DIR ${VENDOR_SOURCE_DIR}/skia)
# set(SKIA_BUILD_TYPE out/Shared-mingw)
# set(SKIA_CMAKE_DIR ${SKIA_DIR}/${SKIA_BUILD_TYPE})

# execute_process(
#   COMMAND python2 tools/git-sync-deps
#   WORKING_DIRECTORY ${SKIA_DIR})

# # execute_process(
# #   COMMAND bin/gn gen ${SKIA_BUILD_TYPE} "--args=is_official_build=true is_component_build=true" --ide=json --json-ide-script=../../gn/gn_to_cmake.py
# #   WORKING_DIRECTORY ${SKIA_DIR})
# execute_process(
#   COMMAND bin/gn gen ${SKIA_BUILD_TYPE} "--args=cc=\"x86_64-w64-mingw32-gcc\" cxx=\"x86_64-w64-mingw32-g++\"" --ide=json --json-ide-script=../../gn/gn_to_cmake.py
#   WORKING_DIRECTORY ${SKIA_DIR})

# add_subdirectory(${SKIA_CMAKE_DIR})
