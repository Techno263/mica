#include <catch2/catch_test_macros.hpp>
#include <expected>
#include <mica/mica.hpp>
#include <string>

namespace mica_test {

namespace {

constexpr const std::string ERROR_MSG("output is even");

int free_function_test(int a, int b)
{
    int output = a + b;
    if (output % 2 == 0) {
        throw std::runtime_error(ERROR_MSG);
    }
    return output;
}

void free_function_test_void_return(int a, int b) {
    free_function_test(a, b);
}

} // unnamed namespace

TEST_CASE("MICA_TRY")
{
    auto&& exp = []() noexcept -> std::expected<int, std::string> {
        int output = 0;
        MICA_TRY(output, mica::make_noexcept<free_function_test>(1, 2));
        return output;
    }();
    REQUIRE(exp.has_value());
    REQUIRE(exp.value() == 3);
}

TEST_CASE("MICA_TRY error")
{
    auto&& exp = []() noexcept -> std::expected<int, std::string> {
        int output = 0;
        MICA_TRY(output, mica::make_noexcept<free_function_test>(2, 4));
        return output;
    }();
    REQUIRE_FALSE(exp.has_value());
    REQUIRE(exp.error() == ERROR_MSG);
}

TEST_CASE("MICA_TRY_VOID")
{
    auto&& exp = []() noexcept -> std::expected<void, std::string> {
        MICA_TRY_VOID(mica::make_noexcept<free_function_test_void_return>(1, 2));
        return std::expected<void, std::string>();
    }();
    REQUIRE(exp.has_value());
}

TEST_CASE("MICA_TRY_VOID error")
{
    auto&& exp = []() noexcept -> std::expected<void, std::string> {
        MICA_TRY_VOID(mica::make_noexcept<free_function_test_void_return>(2, 4));
        return std::expected<void, std::string>();
    }();
    REQUIRE_FALSE(exp.has_value());
    REQUIRE(exp.error() == ERROR_MSG);
}

TEST_CASE("MICA_TRY_STATIC")
{
    auto&& exp = []() noexcept -> std::expected<int, std::string> {
        int output = 0;
        MICA_TRY_STATIC(output, mica::make_noexcept<free_function_test>(1, 2), "foobar error");
        return output;
    }();
    REQUIRE(exp.has_value());
    REQUIRE(exp.value() == 3);
}

TEST_CASE("MICA_TRY_STATIC error")
{
    auto&& exp = []() noexcept -> std::expected<int, std::string> {
        int output = 0;
        MICA_TRY_STATIC(output, mica::make_noexcept<free_function_test>(2, 4), "foobar error");
        return output;
    }();
    REQUIRE_FALSE(exp.has_value());
    REQUIRE(exp.error() == "foobar error");
}

TEST_CASE("MICA_TRY_STATIC_VOID")
{
    auto&& exp = []() noexcept -> std::expected<void, std::string> {
        MICA_TRY_STATIC_VOID(mica::make_noexcept<free_function_test_void_return>(1, 2), "foobar error");
        return std::expected<void, std::string>();
    }();
    REQUIRE(exp.has_value());
}

TEST_CASE("MICA_TRY_STATIC_VOID error")
{
    auto&& exp = []() noexcept -> std::expected<void, std::string> {
        MICA_TRY_STATIC_VOID(mica::make_noexcept<free_function_test_void_return>(2, 4), "foobar error");
        return std::expected<void, std::string>();
    }();
    REQUIRE_FALSE(exp.has_value());
    REQUIRE(exp.error() == "foobar error");
}

} // namespace mica_test
