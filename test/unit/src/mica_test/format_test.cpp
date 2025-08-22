#include <catch2/catch_test_macros.hpp>
#include <mica/mica.hpp>

namespace mica_test {

TEST_CASE("format 1")
{
    auto&& exp = mica::format("foobar: {}, {}", 1, "hello");
    REQUIRE(exp.has_value());
    REQUIRE(exp.value() == "foobar: 1, hello");
}

} // namespace mica_test
