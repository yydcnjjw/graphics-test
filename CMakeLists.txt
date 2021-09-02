cmake_minimum_required(VERSION 3.11)

project(my-graphics LANGUAGES CXX C)

set(VENDOR_SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(VENDOR_BINARY_DIR ${PROJECT_BINARY_DIR}/vendor)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH
  ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(test)
