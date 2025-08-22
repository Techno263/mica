set(MICA_UNITTEST_SOURCES
    make_noexcept_capturing_lambda_test.cpp
    make_noexcept_free_function_test.cpp
    make_noexcept_member_function_test.cpp
    make_noexcept_noncapturing_lambda_test.cpp
    try_test.cpp
)

prepend_paths(
    "${MICA_UNITTEST_SOURCES}"
    "src/mica_test"
    "MICA_UNITTEST_SOURCES"
)
