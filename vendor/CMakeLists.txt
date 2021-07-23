add_library(glad SHARED glad/src/gl.cc)
target_include_directories(glad
  PUBLIC glad/include)

set(RXCPP_INCLUDE_PATH ${VENDOR_SOURCE_DIR}/RxCpp/Rx/v2/src CACHE PATH "")

set(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(assimp)

set(STB_INCLUDE_PATH ${VENDOR_SOURCE_DIR}/stb/ CACHE PATH "")
