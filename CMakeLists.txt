cmake_minimum_required(VERSION 3.10)

project(NamedType VERSION 1.1.0 LANGUAGES CXX)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
    cmake_policy(SET CMP0076 NEW)
endif()

set(MASTER_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MASTER_PROJECT ON)
endif()

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

set(ENABLE_TEST ON CACHE BOOL "Enable test")

if (ENABLE_TEST AND ${MASTER_PROJECT})
    enable_testing()
    add_subdirectory(test)
endif()

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS NamedType
        EXPORT ${PROJECT_NAME}Targets)

# Makes the project importable from the build directory
export(EXPORT ${PROJECT_NAME}Targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

install(DIRECTORY include/NamedType
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_package_config_file(cmake/${PROJECT_NAME}Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION lib/cmake/)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                                 VERSION ${${PROJECT_NAME}_VERSION}
                                 COMPATIBILITY AnyNewerVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION lib/cmake/)
install(EXPORT ${PROJECT_NAME}Targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION lib/cmake/)
