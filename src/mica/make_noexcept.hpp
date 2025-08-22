#pragma once

#include <concepts>
#include <expected>
#include <string>

namespace mica {

// Handle free functions
template<auto Func, typename... Args>
requires(
    std::invocable<decltype(Func), Args...>
)
constexpr
std::expected<std::invoke_result_t<decltype(Func), Args...>, std::string>
make_noexcept(Args&&... args) noexcept;

// Handle member function pointers
template<auto Func, typename T, typename... Args>
requires (
    std::invocable<decltype(Func), T&&, Args&&...>
)
constexpr
std::expected<std::invoke_result_t<decltype(Func), T&&, Args&&...>, std::string>
make_noexcept(T&& obj, Args&&... args) noexcept;

// Handle capturing lambdas
template<typename Lambda, typename... Args>
requires(
    std::invocable<Lambda, Args&&...>
)
constexpr
std::expected<std::invoke_result_t<Lambda, Args&&...>, std::string>
make_noexcept(Lambda&& lambda, Args&&... args) noexcept;

} // namespace mica

#include <mica/make_noexcept.inl>
