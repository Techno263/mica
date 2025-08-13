set(MICA_UNITTEST_SOURCES
    make_noexcept_test.cpp
    try_test.cpp
)

prepend_paths(
    "${MICA_UNITTEST_SOURCES}"
    "src/mica_test"
    "MICA_UNITTEST_SOURCES"
)
