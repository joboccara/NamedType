cmake_minimum_required(VERSION 3.10)

project(NamedType CXX)

set(librarySources
	"crtp.hpp"
	"named_type.hpp"
	"named_type_impl.hpp"
	"underlying_functionalities.hpp"
)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE .)

set(ENABLE_TESTS ON CACHE BOOL "Enable tests")

if (ENABLE_TESTS)
	enable_testing()

	set(TestTargetName "${PROJECT_NAME}Test")

	set(testSources
		${librarySources}
		"main.cpp"
		"catch.hpp"
	)

	add_executable(${TestTargetName} ${testSources})

	set_property(TARGET ${TestTargetName} PROPERTY CXX_STANDARD 17)

	if (MSVC)
		string(REGEX REPLACE " /W[0-4]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
		string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

		target_compile_options(
			${TestTargetName}
			PRIVATE
			"/W4"
			"/WX"
			"/diagnostics:caret"
		)
	else()
		target_compile_options(
			${TestTargetName}
			PRIVATE
			-Wall
			-Wcast-align
			-Wcast-qual
			-Wconversion
			-Wctor-dtor-privacy
			-Wdouble-promotion
			-Werror
			-Wextra
			-Wlogical-op
			-Wnoexcept
			-Wold-style-cast
			-Woverloaded-virtual
			-Wpedantic
			-Wredundant-decls
			-Wstack-protector
			-Wstrict-null-sentinel
			-Wzero-as-null-pointer-constant
			-Wfloat-equal
			-Wshadow
			-Wuseless-cast
			-Weffc++
		)
	endif()

	add_test(NAME ${TestTargetName} COMMAND ${TestTargetName})
endif()
