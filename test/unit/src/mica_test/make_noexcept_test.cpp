#include <catch2/catch_test_macros.hpp>
#include <expected>
#include <mica/macro.hpp>
#include <mica/make_noexcept.hpp>
#include <mica/try.hpp>
#include <stdexcept>
#include <string>

#define MICA_TEST_MAKE_NONCAPTURE_LAMBDA(lambda_name_) \
auto lambda_name_ = [](int a, int b) -> int { \
    return free_function_test(a, b); \
}

#define MICA_TEST_MAKE_NONCAPTURE_LAMBDA_VOID_RET(lambda_name_) \
auto lambda_name_ = [](int a, int b) -> void { \
    free_function_test(a, b); \
}

#define _MICA_TEST_INTERNAL_CAPTURE_LAMBDA(lambda_name_, a_val_, tmp_a_var_) \
int tmp_a_var_ = (a_val_); \
auto lambda_name_ = [&tmp_a_var_](int b) -> int { \
    return free_function_test(tmp_a_var_, b); \
}

#define MICA_TEST_MAKE_CAPTURE_LAMBDA(lambda_name_, a_val_) \
    _MICA_TEST_INTERNAL_CAPTURE_LAMBDA(lambda_name_, a_val_, MICA_TMP_VAR_DEFAULT)

#define _MICA_TEST_INTERNAL_CAPTURE_LAMBDA_VOID_RET(lambda_name_, a_val_, tmp_a_var_) \
int tmp_a_var_ = (a_val_); \
auto lambda_name_ = [&tmp_a_var_](int b) -> void { \
    free_function_test(tmp_a_var_, b); \
}

#define MICA_TEST_MAKE_CAPTURE_LAMBDA_VOID_RET(lambda_name_, a_val_) \
    _MICA_TEST_INTERNAL_CAPTURE_LAMBDA_VOID_RET(lambda_name_, a_val_, MICA_TMP_VAR_DEFAULT)

#define _MICA_TEST_INTERNAL_MUTABLE_CAPTURE_LAMBDA(lambda_name_, a_val_, tmp_a_var_) \
int tmp_a_var_ = (a_val_); \
auto lambda_name_ = [&tmp_a_var_](int b) mutable -> int { \
    return free_function_test(tmp_a_var_, b); \
}

#define MICA_TEST_MAKE_MUTABLE_CAPTURE_LAMBDA(lambda_name_, a_val_) \
    _MICA_TEST_INTERNAL_CAPTURE_LAMBDA(lambda_name_, a_val_, MICA_TMP_VAR_DEFAULT)

#define _MICA_TEST_INTERNAL_MUTABLE_CAPTURE_LAMBDA_VOID_RET(lambda_name_, a_val_, tmp_a_var_) \
int tmp_a_var_ = (a_val_); \
auto lambda_name_ = [&tmp_a_var_](int b) mutable -> void { \
    free_function_test(tmp_a_var_, b); \
}

#define MICA_TEST_MAKE_MUTABLE_CAPTURE_LAMBDA_VOID_RET(lambda_name_, a_val_) \
    _MICA_TEST_INTERNAL_CAPTURE_LAMBDA_VOID_RET(lambda_name_, a_val_, MICA_TMP_VAR_DEFAULT)

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

class member_function_test {
public:
    int test_func(int a, int b)
    {
        return free_function_test(a, b);
    }

    int test_func_const(int a, int b) const
    {
        return free_function_test(a, b);
    }

    void test_func_void_return(int a, int b)
    {
        free_function_test_void_return(a, b);
    }

    void test_func_const_void_return(int a, int b) const
    {
        free_function_test_void_return(a, b);
    }
};

} // unnamed namespace

TEST_CASE("make_noexcept free function")
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

TEST_CASE("make_noexcept free function void return")
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

TEST_CASE("make_noexcept member function")
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

TEST_CASE("make_noexcept const member function")
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

TEST_CASE("make_noexcept member function void return")
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

TEST_CASE("make_noexcept const member function void return")
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

TEST_CASE("make_noexcept member function with object pointer")
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

TEST_CASE("make_noexcept const member function with object pointer")
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

TEST_CASE("make_noexcept member function void return with object pointer")
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

TEST_CASE("make_noexcept non-capturing lambda")
{
    {
        MICA_TEST_MAKE_NONCAPTURE_LAMBDA(lambda);
        auto&& exp = mica::make_noexcept<lambda>(1, 2);
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            MICA_TEST_MAKE_NONCAPTURE_LAMBDA(lambda);
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
        MICA_TEST_MAKE_NONCAPTURE_LAMBDA(lambda);
        auto&& exp = mica::make_noexcept<lambda>(2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            MICA_TEST_MAKE_NONCAPTURE_LAMBDA(lambda);
            MICA_TRY(output, mica::make_noexcept<lambda>(2, 4));
            return output;
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept non-capturing lambda void return")
{
    {
        MICA_TEST_MAKE_NONCAPTURE_LAMBDA_VOID_RET(lambda);
        auto&& exp = mica::make_noexcept<lambda>(1, 2);
        REQUIRE(exp.has_value());
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            MICA_TEST_MAKE_NONCAPTURE_LAMBDA_VOID_RET(lambda);
            MICA_TRY_VOID(mica::make_noexcept<lambda>(1, 2));
            return std::expected<void, std::string>();
        }();
        REQUIRE(exp.has_value());
    }
}

TEST_CASE("make_noexcept non-capturing lambda void return error")
{
    {
        MICA_TEST_MAKE_NONCAPTURE_LAMBDA_VOID_RET(lambda);
        auto&& exp = mica::make_noexcept<lambda>(2, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            MICA_TEST_MAKE_NONCAPTURE_LAMBDA_VOID_RET(lambda);
            MICA_TRY_VOID(mica::make_noexcept<lambda>(2, 4));
            return std::expected<void, std::string>();
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept capturing lambda")
{
    {
        int a = 1;
        MICA_TEST_MAKE_CAPTURE_LAMBDA(lambda, a);
        auto&& exp = mica::make_noexcept(lambda, 2);
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            int a = 1;
            MICA_TEST_MAKE_CAPTURE_LAMBDA(lambda, a);
            MICA_TRY(output, mica::make_noexcept(lambda, 2));
            return output;
        }();
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
}

TEST_CASE("make_noexcept capturing lambda error")
{
    {
        int a = 2;
        MICA_TEST_MAKE_CAPTURE_LAMBDA(lambda, a);
        auto&& exp = mica::make_noexcept(lambda, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            int a = 2;
            MICA_TEST_MAKE_CAPTURE_LAMBDA(lambda, a);
            MICA_TRY(output, mica::make_noexcept(lambda, 4));
            return output;
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept mutable capturing lambda")
{
    {
        int a = 1;
        MICA_TEST_MAKE_MUTABLE_CAPTURE_LAMBDA(lambda, a);
        auto&& exp = mica::make_noexcept(lambda, 2);
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            int a = 1;
            MICA_TEST_MAKE_MUTABLE_CAPTURE_LAMBDA(lambda, a);
            MICA_TRY(output, mica::make_noexcept(lambda, 2));
            return output;
        }();
        REQUIRE(exp.has_value());
        REQUIRE(exp.value() == 3);
    }
}

TEST_CASE("make_noexcept mutable capturing lambda error")
{
    {
        int a = 2;
        MICA_TEST_MAKE_MUTABLE_CAPTURE_LAMBDA(lambda, a);
        auto&& exp = mica::make_noexcept(lambda, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<int, std::string> {
            int output = 0;
            int a = 2;
            MICA_TEST_MAKE_MUTABLE_CAPTURE_LAMBDA(lambda, a);
            MICA_TRY(output, mica::make_noexcept(lambda, 4));
            return output;
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept capturing lambda void return")
{
    {
        int a = 1;
        MICA_TEST_MAKE_CAPTURE_LAMBDA_VOID_RET(lambda, a);
        auto&& exp = mica::make_noexcept(lambda, 2);
        REQUIRE(exp.has_value());
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            int a = 1;
            MICA_TEST_MAKE_CAPTURE_LAMBDA_VOID_RET(lambda, a);
            MICA_TRY_VOID(mica::make_noexcept(lambda, 2));
            return std::expected<void, std::string>();
        }();
        REQUIRE(exp.has_value());
    }
}

TEST_CASE("make_noexcept capturing lambda void return error")
{
    {
        int a = 2;
        MICA_TEST_MAKE_CAPTURE_LAMBDA_VOID_RET(lambda, a);
        auto&& exp = mica::make_noexcept(lambda, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            int a = 2;
            MICA_TEST_MAKE_CAPTURE_LAMBDA_VOID_RET(lambda, a);
            MICA_TRY_VOID(mica::make_noexcept(lambda, 4));
            return std::expected<void, std::string>();
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

TEST_CASE("make_noexcept mutable capturing lambda void return")
{
    {
        int a = 1;
        MICA_TEST_MAKE_MUTABLE_CAPTURE_LAMBDA_VOID_RET(lambda, a);
        auto&& exp = mica::make_noexcept(lambda, 2);
        REQUIRE(exp.has_value());
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            int a = 1;
            MICA_TEST_MAKE_MUTABLE_CAPTURE_LAMBDA_VOID_RET(lambda, a);
            MICA_TRY_VOID(mica::make_noexcept(lambda, 2));
            return std::expected<void, std::string>();
        }();
        REQUIRE(exp.has_value());
    }
}

TEST_CASE("make_noexcept mutable capturing lambda void return error")
{
    {
        int a = 2;
        MICA_TEST_MAKE_MUTABLE_CAPTURE_LAMBDA_VOID_RET(lambda, a);
        auto&& exp = mica::make_noexcept(lambda, 4);
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
    {
        auto&& exp = []() noexcept -> std::expected<void, std::string> {
            int a = 2;
            MICA_TEST_MAKE_MUTABLE_CAPTURE_LAMBDA_VOID_RET(lambda, a);
            MICA_TRY_VOID(mica::make_noexcept(lambda, 4));
            return std::expected<void, std::string>();
        }();
        REQUIRE_FALSE(exp.has_value());
        REQUIRE(exp.error() == ERROR_MSG);
    }
}

} // namespace mica_test
