cmake_minimum_required(VERSION 3.5)

project(my-anything LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(VENDOR_SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(VENDOR_BINARY_DIR ${PROJECT_BINARY_DIR}/vendor)

add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(test)
