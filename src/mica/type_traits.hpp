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

template<typename T, typename = void>
struct is_char_type : std::false_type
{};

template<typename T>
struct is_char_type<T, std::void_t<typename std::char_traits<T>::char_type>> : std::true_type
{};

template<typename T>
constexpr bool is_char_type_v = is_char_type<T>::value;

template<typename CharT, std::size_t N>
struct is_string_literal<const CharT (&)[N]> : std::bool_constant<is_char_type<CharT>::value>
{};

template<typename CharT, std::size_t N>
constexpr bool is_string_literal_v = is_string_literal<CharT, N>::value;

} // namespace mica
