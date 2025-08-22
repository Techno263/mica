#include <catch2/catch_test_macros.hpp>
#include <expected>
#include <format>
#include <mica/mica.hpp>
#include <stdexcept>
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

void free_function_test_void_return(int a, int b)
{
    free_function_test(a, b);
}

} // unnamed namespace

TEST_CASE("make_noexcept free function success")
{
    {
        auto&& exp = mica::make_noexcept<free_function_test>(1, 2);
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            MICA_TRY(output, mica::make_noexcept<free_function_test>(1, 2));
            return output;
        }();
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
}

TEST_CASE("make_noexcept free function error")
{
    {
        auto&& exp = mica::make_noexcept<free_function_test>(2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            MICA_TRY(output, mica::make_noexcept<free_function_test>(2, 4));
            return output;
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept free function void return success")
{
    {
        auto&& exp = mica::make_noexcept<free_function_test_void_return>(1, 2);
        REQUIRE(exp.has_value());
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            MICA_TRY_VOID(mica::make_noexcept<free_function_test_void_return>(1, 2));
            return std::expected<void, std::string>();
        }();
        REQUIRE(exp.has_value());
    }
}

TEST_CASE("make_noexcept free function void return error")
{
    {
        auto&& exp = mica::make_noexcept<free_function_test_void_return>(2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            MICA_TRY_VOID(mica::make_noexcept<free_function_test_void_return>(2, 4));
            return std::expected<void, std::string>();
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept std::format")
{
    {
        {
            auto&& exp = mica::make_noexcept<
                mica::resolve<
                    std::string(std::format_string<int, const char*>, int&&, const char*&&),
                    &std::format<int, const char*>
                >()
            >(std::format_string<int, const char*>("foobar: {}, {}"), 1, "hello");
            REQUIRE(exp.has_value());
            REQUIRE(exp.value() == "foobar: 1, hello");
        }
    }
}

} // namespace mica_test
