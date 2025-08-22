#include <catch2/catch_test_macros.hpp>
#include <mica/mica.hpp>
#include <stdexcept>
#include <format>
#include <functional>

#include <string_view>

namespace mica_test {

namespace {

constexpr const std::string ERROR_MSG("output is even");

template<typename T>
T free_function_test(T a, T b)
{
    T output = a + b;
    if (output % 2 == 0) {
    
        throw std::runtime_error(ERROR_MSG);
    }
    return output;
}

} // unnamed namespace

template <typename Ret, typename... Args>
constexpr auto resolve(Ret(*func)(Args...)) -> Ret(*)(Args...) {
    return func;
}

TEST_CASE("test std::format")
{
    // These work
    //auto f = resolve<std::string, std::format_string<int, const char*>, int&&, const char*&&>(&std::format);
    //auto f = static_cast<std::string(*)(std::format_string<int, const char*>, int&&, const char*&&)>(&std::format);
    //REQUIRE(f("foobar: {}, {}", 1, "hello") == "");

    //auto x = std::invoke(
    //    static_cast<std::string(*)(std::format_string<int, const char*>, int, const char*)>(&std::format<int, const char*>),
    //    "foobar: {}, {}", 1, "hello"
    //);

    //const auto lambda = []<typename... Args>(std::format_string<Args...>&& fmt, Args&&... args) mutable {
    //    return std::format<Args...>(fmt, std::forward<Args>(args)...);
    //};
    //auto x = std::invoke(lambda, "foobar: {}, {}", 1, "hello");

    //const auto lambda = []<typename... Args>(std::format_string<Args...>&& fmt, Args&&... args) mutable {
    //    return std::format<Args...>(fmt, std::forward<Args>(args)...);
    //};
    //auto&& exp = mica::make_noexcept<
    //    &decltype(lambda)::template operator()<int, const char*>
    //>(std::move(lambda), "foobar: {}, {}", 1, "hello");



    //CHECK(std::is_pointer_v<std::format<int, const char*>> == 2);
    //CHECK(std::is_function_v<std::remove_pointer_t<decltype(std::format<int, const char*>)>> == 2);
    //REQUIRE(1 == 2);

    //auto&& exp = mica::make_noexcept<std::format<int, const char*>>("foobar: {}, {}", 1, "hello");
    //REQUIRE(exp.has_value());
    //REQUIRE(exp.value() == "a");


    //auto lambda = []<typename... Args>(std::format_string<Args...>&& fmt, Args&&... args) constexpr {
    //    return std::format<Args...>(fmt, std::forward<Args>(args)...);
    //};
    //auto p = &decltype(lambda)::template operator()<int, const char*>;
    //auto x = (lambda.*p)("foobar: {}, {}", 1, "hello");
    //REQUIRE(x == "");

    //auto&& exp = mica::make_noexcept<&decltype(lambda)::template operator()<int, const char*>>(lambda, "foobar: {}, {}", 1, "hello");
    //REQUIRE(exp.has_value());
    //REQUIRE(exp.value() == "a");



    //auto&& exp = MICA_WRAP_FUNC2(std::format, "foobar: {}, {}", 1, "hello");
    //REQUIRE(exp.value() == "a");

    // This works
    //auto&& exp = []() -> std::expected<std::string, std::string> {
    //    std::string foo;
    //    MICA_WRAP_FUNC(foo, std::format, "foobar: {}, {}", 1, "hello");
    //    return foo;
    //}();
    //REQUIRE(exp.value() == "a");

    //auto&& exp = mica::make_noexcept<MICA_WRAP_TEMPLATE(std::format)>("foobar: {}, {}", 1, "hello");
    //REQUIRE(exp.has_value());
    //REQUIRE(exp.value() == "foobar: 1, hello");

    //auto&& lambda = []<typename... Args>(Args&&... args) {
    //    return std::format(std::forward<Args>(args)...);
    //};
    //auto&& exp = mica::make_noexcept(lambda, std::format_string<int, const char[6]>("foobar: {}, {}"), 1, "hello");
    //REQUIRE(exp.has_value());
    //REQUIRE(exp.value() == "foobar: 1, hello");

    //auto&& lambda = []<const char* Fmt, typename... Args>(Args&&... args) constexpr {
    //    return std::format(Fmt, std::forward<Args>(args)...);
    //};
    //auto&& x = lambda.operator()<"foobar: {}, {}">(1, "hello");
    //REQUIRE_FALSE(x == "foobar: 1, hello");

    //auto&& exp = mica::make_noexcept(lambda, "foobar: {}, {}", 1, "hello");
    //REQUIRE(exp.has_value());
    //REQUIRE(exp.value() == "foobar: 1, hello");
}

TEST_CASE("free function template")
{
    //auto&& exp = mica::make_noexcept<MICA_WRAP_TEMPLATE(free_function_test)>(1, 2);
    //REQUIRE(exp.has_value());
    //REQUIRE(exp.value() == 3);
}

TEST_CASE("free function template error")
{

}

TEST_CASE("member function template")
{

}

TEST_CASE("member function template error")
{

}

TEST_CASE("non-capturing lambda template")
{

}

TEST_CASE("non-capturing lambda template error")
{

}

TEST_CASE("capturing lambda template")
{

}

TEST_CASE("capturing lambda template error")
{

}

} // namespace mica_test
