#include <catch2/catch_test_macros.hpp>
#include <mica/make_noexcept.hpp>

namespace mica_test {

namespace {

int free_function_test(int a, int b)
{

}

class member_function_test {
public:
    int test_func1(int a, int b)
    {
        return free_function_test(a, b);
    }

    int test_func2(int a, int b) const
    {
        return free_function_test(a, b);
    }
};

} // unnamed namespace

TEST_CASE("make_noexcept free function")
{

}

TEST_CASE("make_noexcept free function error")
{

}

TEST_CASE("make_noexcept member function")
{

}

TEST_CASE("make_noexcept member function error")
{

}

TEST_CASE("make_noexcept member function with object pointer")
{

}

TEST_CASE("make_noexcept member function with object pointer error")
{

}

TEST_CASE("make_noexcept non-capturing lambda")
{

}

TEST_CASE("make_noexcept non-capturing lambda error")
{

}

TEST_CASE("make_noexcept capturing lambda")
{

}

TEST_CASE("make_noexcept non-capturing lambda error")
{

}

} // namespace mica_test
