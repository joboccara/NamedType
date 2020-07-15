cmake_minimum_required(VERSION 3.10)

project(NamedTypeTest CXX)

enable_testing()

set(testSources
	"main.cpp"
	"catch.hpp"
)

add_executable(${PROJECT_NAME} ${testSources})

target_include_directories(${PROJECT_NAME} PUBLIC "${NamedType_SOURCE_DIR}/include/")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

if (MSVC)
	string(REGEX REPLACE " /W[0-4]" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
	string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

	target_compile_options(
		${PROJECT_NAME}
		PRIVATE
		"/W4"
		"/WX"
		"/diagnostics:caret"
	)
else()
	target_compile_options(
		${PROJECT_NAME}
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

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})
