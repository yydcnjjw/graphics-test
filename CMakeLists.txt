cmake_minimum_required(VERSION 3.11)

project(graphics-test LANGUAGES CXX C)

set(VENDOR_SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(VENDOR_BINARY_DIR ${PROJECT_BINARY_DIR}/vendor)

add_subdirectory(vendor)
add_subdirectory(src)
add_subdirectory(test)
