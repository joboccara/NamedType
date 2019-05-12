cmake_minimum_required(VERSION 3.14)

project(NamedType)

#-Wall
#-Wcast-align
#-Wcast-qual
#-Wconversion
#-Wctor-dtor-privacy
#-Wdouble-promotion
#-Werror
#-Wextra
#-Wlogical-op
#-Wnoexcept
#-Wold-style-cast
#-Woverloaded-virtual
#-Wredundant-decls
#-Wstack-protector
#-Wstrict-null-sentinel
#-Wzero-as-null-pointer-constant

set(CMAKE_CXX_FLAGS
    "
    ${CMAKE_CXX_FLAGS} \
    -std=c++17 \
    -Wall \
    -Wcast-align \
    -Wcast-qual \
    -Wconversion \
    -Wctor-dtor-privacy \
    -Wdouble-promotion \
    -Werror \
    -Wextra \
    -Wlogical-op \
    -Wnoexcept \
    -Wold-style-cast \
    -Woverloaded-virtual \
    -Wpedantic \
    -Wredundant-decls \
    -Wstack-protector \
    -Wstrict-null-sentinel \
    -Wzero-as-null-pointer-constant \
    -Wfloat-equal \
    -Wshadow \
    -Wuseless-cast \
    -Weffc++ \
    "
)

set(src
    "main.cpp"
    "catch.hpp"
    "crtp.hpp"
    "named_type.hpp"
    "named_type_impl.hpp"
    "underlying_functionalities.hpp"
)

add_executable(${PROJECT_NAME} ${src})
