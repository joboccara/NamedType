cmake_minimum_required(VERSION 3.0)
project(NamedTypeTest)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE CONAN_PKG::NamedType)
