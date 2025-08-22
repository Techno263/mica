cmake_minimum_required(VERSION 4.0.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

set(MICA_VERSION "0.0.2")

option(MICA_TESTS "Build test executable")

string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)$" _ "${MICA_VERSION}")
set(MICA_VERSION_MAJOR "${CMAKE_MATCH_1}")
set(MICA_VERSION_MINOR "${CMAKE_MATCH_2}")
set(MICA_VERSION_PATCH "${CMAKE_MATCH_3}")

project(mica
    VERSION "${MICA_VERSION}"
    LANGUAGES CXX
)
set(PROJECT_NAMESPACE "${PROJECT_NAME}")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include("${CMAKE_CURRENT_LIST_DIR}/cmake/Util.cmake")

set(MICA_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library("${PROJECT_NAME}" INTERFACE)
target_include_directories("${PROJECT_NAME}"
    INTERFACE
        $<BUILD_INTERFACE:${MICA_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_features("${PROJECT_NAME}"
    INTERFACE cxx_std_23
)
target_compile_options("${PROJECT_NAME}"
    INTERFACE -Wall -Wextra -Wpedantic -Werror
)

set(MICA_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION "${MICA_VERSION}"
    COMPATIBILITY SameMajorVersion
    ARCH_INDEPENDENT
)
configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    INSTALL_DESTINATION
        "${MICA_CMAKE_CONFIG_DESTINATION}"
    PATH_VARS
        PROJECT_NAME
        PROJECT_NAMESPACE
)

install(
    TARGETS "${PROJECT_NAME}"
    EXPORT "${PROJECT_NAME}Targets"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
install(
    EXPORT "${PROJECT_NAME}Targets"
    NAMESPACE "${PROJECT_NAMESPACE}::"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/mica"
)
install(
    DIRECTORY "src/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION
        "${MICA_CMAKE_CONFIG_DESTINATION}"
)

if(MICA_TESTS)
    message(STATUS "Building ${PROJECT_NAME} tests")
    enable_testing()
    add_subdirectory("test")
endif()
