include(CheckIPOSupported)

check_ipo_supported(
    RESULT check_ipo_result
    OUTPUT check_ipo_output
)
if(NOT check_ipo_result)
    message(WARNING "IPO/LTO is not supported: ${check_ipo_output}")
endif()

find_package(Catch2 REQUIRED COMPONENTS Catch2 Catch2Main)

set(UNITTEST_NAME "${PROJECT_NAME}_unittest")

set(MICA_UNITTEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
include("${CMAKE_CURRENT_LIST_DIR}/cmake/Sources.cmake")

add_executable("${UNITTEST_NAME}" ${MICA_UNITTEST_SOURCES})
target_compile_features("${UNITTEST_NAME}"
    PRIVATE cxx_std_23
)
target_compile_options("${UNITTEST_NAME}"
    PRIVATE -fconcepts-diagnostics-depth=2
)
if(check_ipo_result)
    set_target_properties("${UNITTEST_NAME}"
        PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION TRUE
    )
endif()
target_link_libraries("${UNITTEST_NAME}"
    PRIVATE
        "${PROJECT_NAME}"
        Catch2::Catch2
        Catch2::Catch2Main
)

add_test(
    NAME "${UNITTEST_NAME}"
    COMMAND "${UNITTEST_NAME}"
)
