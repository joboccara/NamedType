cmake_minimum_required(VERSION 3.10)

project(NamedType CXX)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    cmake_policy(SET CMP0076 NEW)
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE "include/")

target_sources(${PROJECT_NAME} INTERFACE
	"include/NamedType/crtp.hpp"
	"include/NamedType/named_type.hpp"
	"include/NamedType/named_type_impl.hpp"
	"include/NamedType/underlying_functionalities.hpp"
)

set(ENABLE_TEST ON CACHE BOOL "Enable test")

if (ENABLE_TEST)
	add_subdirectory(test)
endif()
