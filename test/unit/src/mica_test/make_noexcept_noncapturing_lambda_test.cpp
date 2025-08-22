#include <catch2/catch_test_macros.hpp>
#include <expected>
#include <mica/mica.hpp>
#include <stdexcept>
#include <string>

namespace mica {

namespace {

constexpr const std::string ERROR_MSG("output is even");

int test_function(int a, int b)
{
    int output = a + b;
    if (output % 2 == 0) {
        throw std::runtime_error(ERROR_MSG);
    }
    return output;
}

} // unnamed namespace

TEST_CASE("make_noexcept non-capturing lambda success")
{
    {
        auto lambda = [](int a, int b) -> int {
            return test_function(a, b);
        };
        auto&& exp = mica::make_noexcept<lambda>(1, 2);
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            auto lambda = [](int a, int b) -> int {
                return test_function(a, b);
            };
            MICA_TRY(output, mica::make_noexcept<lambda>(1, 2));
            return output;
        }();
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
}

TEST_CASE("make_noexcept non-capturing lambda error")
{
    {
        auto lambda = [](int a, int b) -> int {
            return test_function(a, b);
        };
        auto&& exp = mica::make_noexcept<lambda>(2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            auto lambda = [](int a, int b) -> int {
                return test_function(a, b);
            };
            MICA_TRY(output, mica::make_noexcept<lambda>(2, 4));
            return output;
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept non-capturing lambda void return success")
{
    {
        auto lambda = [](int a, int b) -> void {
            test_function(a, b);
        };
        auto&& exp = mica::make_noexcept<lambda>(1, 2);
        REQUIRE(exp.has_value());
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            auto lambda = [](int a, int b) -> void {
                test_function(a, b);
            };
            MICA_TRY_VOID(mica::make_noexcept<lambda>(1, 2));
            return std::expected<void, std::string>();
        }();
        REQUIRE(exp.has_value());
    }
}

TEST_CASE("make_noexcept non-capturing lambda void return error")
{
    {
        auto lambda = [](int a, int b) -> void {
            test_function(a, b);
        };
        auto&& exp = mica::make_noexcept<lambda>(2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            auto lambda = [](int a, int b) -> void {
                test_function(a, b);
            };
            MICA_TRY_VOID(mica::make_noexcept<lambda>(2, 4));
            return std::expected<void, std::string>();
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

} // namespace mica
