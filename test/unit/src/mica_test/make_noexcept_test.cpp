#include <catch2/catch_test_macros.hpp>
#include <expected>
#include <mica/mica.hpp>
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

void free_function_test_void_return(int a, int b)
{
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



} // namespace mica_test
