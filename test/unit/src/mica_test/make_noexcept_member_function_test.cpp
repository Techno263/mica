#include <catch2/catch_test_macros.hpp>
#include <expected>
#include <mica/mica.hpp>
#include <stdexcept>
#include <string>

namespace mica {

namespace {

constexpr const std::string ERROR_MSG("output is even");

class member_function_test {
public:
    int test_func_const(int a, int b) const
    {
        int output = a + b;
        if (output % 2 == 0) {
            throw std::runtime_error(ERROR_MSG);
        }
        return output;
    }

    int test_func(int a, int b)
    {
        return test_func_const(a, b);
    }

    void test_func_const_void_return(int a, int b) const
    {
        test_func_const(a, b);
    }

    void test_func_void_return(int a, int b)
    {
        test_func_const(a, b);
    }
};

} // unnamed namespace

TEST_CASE("make_noexcept member function success")
{
    {
        member_function_test m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func>(m, 1, 2);
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            member_function_test m;
            MICA_TRY(output, mica::make_noexcept<&member_function_test::test_func>(m, 1, 2));
            return output;
        }();
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
}

TEST_CASE("make_noexcept member function error")
{
    {
        member_function_test m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func>(m, 2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            member_function_test m;
            MICA_TRY(output, mica::make_noexcept<&member_function_test::test_func>(m, 2, 4));
            return output;
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept const member function success")
{
    {
        const member_function_test m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func_const>(m, 1, 2);
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            const member_function_test m;
            MICA_TRY(output, mica::make_noexcept<&member_function_test::test_func_const>(m, 1, 2));
            return output;
        }();
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
}

TEST_CASE("make_noexcept const member function error")
{
    {
        const member_function_test m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func_const>(m, 2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            const member_function_test m;
            MICA_TRY(output, mica::make_noexcept<&member_function_test::test_func_const>(m, 2, 4));
            return output;
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept member function void return success")
{
    {
        member_function_test m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func_void_return>(m, 1, 2);
        REQUIRE(exp.has_value());
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            member_function_test m;
            MICA_TRY_VOID(mica::make_noexcept<&member_function_test::test_func_void_return>(m, 1, 2));
            return std::expected<void, std::string>();
        }();
        REQUIRE(exp.has_value());
    }
}

TEST_CASE("make_noexcept member function void return error")
{
    {
        member_function_test m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func_void_return>(m, 2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            member_function_test m;
            MICA_TRY_VOID(mica::make_noexcept<&member_function_test::test_func_void_return>(m, 2, 4));
            return std::expected<void, std::string>();
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept const member function void return success")
{
    {
        const member_function_test m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func_const_void_return>(m, 1, 2);
        REQUIRE(exp.has_value());
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            const member_function_test m;
            MICA_TRY_VOID(mica::make_noexcept<&member_function_test::test_func_const_void_return>(m, 1, 2));
            return std::expected<void, std::string>();
        }();
        REQUIRE(exp.has_value());
    }
}

TEST_CASE("make_noexcept const member function void return error")
{
    {
        const member_function_test m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func_const_void_return>(m, 2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            const member_function_test m;
            MICA_TRY_VOID(mica::make_noexcept<&member_function_test::test_func_const_void_return>(m, 2, 4));
            return std::expected<void, std::string>();
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept member function with object pointer success")
{
    {
        member_function_test m;
        member_function_test* mp = &m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func>(mp, 1, 2);
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            member_function_test m;
            member_function_test* mp = &m;
            MICA_TRY(output, mica::make_noexcept<&member_function_test::test_func>(mp, 1, 2));
            return output;
        }();
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
}

TEST_CASE("make_noexcept member function with object pointer error")
{
    {
        member_function_test m;
        member_function_test* mp = &m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func>(mp, 2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            member_function_test m;
            member_function_test* mp = &m;
            MICA_TRY(output, mica::make_noexcept<&member_function_test::test_func>(mp, 2, 4));
            return output;
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept const member function with object pointer success")
{
    {
        const member_function_test m;
        const member_function_test* mp = &m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func_const>(mp, 1, 2);
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            const member_function_test m;
            const member_function_test* mp = &m;
            MICA_TRY(output, mica::make_noexcept<&member_function_test::test_func_const>(mp, 1, 2));
            return output;
        }();
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
}

TEST_CASE("make_noexcept const member function with object pointer error")
{
    {
        const member_function_test m;
        const member_function_test* mp = &m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func_const>(mp, 2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            const member_function_test m;
            const member_function_test* mp = &m;
            MICA_TRY(output, mica::make_noexcept<&member_function_test::test_func_const>(mp, 2, 4));
            return output;
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept member function void return with object pointer success")
{
    {
        member_function_test m;
        member_function_test* mp = &m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func_void_return>(mp, 1, 2);
        REQUIRE(exp.has_value());
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            member_function_test m;
            member_function_test* mp = &m;
            MICA_TRY_VOID(mica::make_noexcept<&member_function_test::test_func_void_return>(mp, 1, 2));
            return std::expected<void, std::string>();
        }();
        REQUIRE(exp.has_value());
    }
}

TEST_CASE("make_noexcept member function void return with object pointer error")
{
    {
        member_function_test m;
        member_function_test* mp = &m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func_void_return>(mp, 2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            member_function_test m;
            member_function_test* mp = &m;
            MICA_TRY_VOID(mica::make_noexcept<&member_function_test::test_func_void_return>(mp, 2, 4));
            return std::expected<void, std::string>();
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept const member function void return with object pointer")
{
    {
        const member_function_test m;
        const member_function_test* mp = &m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func_const_void_return>(mp, 1, 2);
        REQUIRE(exp.has_value());
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            const member_function_test m;
            const member_function_test* mp = &m;
            MICA_TRY_VOID(mica::make_noexcept<&member_function_test::test_func_const_void_return>(mp, 1, 2));
            return std::expected<void, std::string>();
        }();
        REQUIRE(exp.has_value());
    }
}

TEST_CASE("make_noexcept const member function void return with object pointer error")
{
    {
        const member_function_test m;
        const member_function_test* mp = &m;
        auto&& exp = mica::make_noexcept<&member_function_test::test_func_const_void_return>(mp, 2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            const member_function_test m;
            const member_function_test* mp = &m;
            MICA_TRY_VOID(mica::make_noexcept<&member_function_test::test_func_const_void_return>(mp, 2, 4));
            return std::expected<void, std::string>();
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

} // namespace mica
