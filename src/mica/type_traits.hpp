#pragma once

#include <cstddef>
#include <expected>
#include <string>
#include <type_traits>

namespace mica {

template<typename T>
struct is_expected : std::false_type
{};

template<typename T, typename E>
struct is_expected<std::expected<T, E>> : std::true_type
{};

template<typename T>
constexpr bool is_expected_v = is_expected<T>::value;

template<typename T>
struct is_string_literal : std::false_type
{};

template<typename CharT, std::size_t N>
struct is_string_literal<const CharT(&)[N]>
    : std::bool_constant<std::disjunction_v<
        std::is_same<CharT, char>,
        std::is_same<CharT, wchar_t>,
        std::is_same<CharT, char8_t>,
        std::is_same<CharT, char16_t>,
        std::is_same<CharT, char32_t>
    >>
{};

template<typename T>
constexpr bool is_string_literal_v = is_string_literal<T>::value;

} // namespace mica
