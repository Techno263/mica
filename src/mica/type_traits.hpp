#pragma once

#include <expected>
#include <type_traits>

namespace mica {

template<typename T>
struct is_expected : std::false_type
{};

template<typename T, typename E>
struct is_expected<std::expected<T, E>> : std::true_type
{};

template<typename T, typename E>
constexpr bool is_expected_v = is_expected<T, E>::value;

} // namespace mica
