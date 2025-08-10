#pragma once

#include <expected>
#include <string>
#include <type_traits>

namespace mica {

// Handle free functions with return value
template<auto Func, typename... Args>
requires (
    std::is_pointer_v<decltype(Func)>
    && std::is_function_v<std::remove_pointer_t<decltype(Func)>>
    && std::is_invocable_v<decltype(Func), Args...>
    && !std::is_nothrow_invocable_v<decltype(Func), Args...>
    && !std::is_void_v<std::invoke_result_t<decltype(Func), Args...>>
)
constexpr
std::expected<std::invoke_result_t<decltype(Func), Args...>, std::string>
make_noexcept(Args&&... args) noexcept;

// Handle free functions with void return
template<auto Func, typename... Args>
requires (
    std::is_pointer_v<decltype(Func)>
    && std::is_function_v<std::remove_pointer_t<decltype(Func)>>
    && std::is_invocable_v<decltype(Func), Args...>
    && !std::is_nothrow_invocable_v<decltype(Func), Args...>
    && std::is_void_v<std::invoke_result_t<decltype(Func), Args...>>
)
constexpr
std::expected<std::invoke_result_t<decltype(Func), Args...>, std::string>
make_noexcept(Args&&... args) noexcept;

// Handle member functions with return value
template<auto Func, typename T, typename... Args>
requires (
    std::is_member_function_pointer_v<decltype(Func)>
    && std::is_invocable_v<decltype(Func), T, Args...>
    && !std::is_nothrow_invocable_v<decltype(Func), T, Args...>
    && !std::is_pointer_v<std::remove_reference_t<T>>
    && !std::is_void_v<std::invoke_result_t<decltype(Func), T, Args...>>
)
constexpr
std::expected<std::invoke_result_t<decltype(Func), T, Args...>, std::string>
make_noexcept(T&& obj, Args&&... args) noexcept;

// Handle member functions with void return
template<auto Func, typename T, typename... Args>
requires (
    std::is_member_function_pointer_v<decltype(Func)>
    && std::is_invocable_v<decltype(Func), T, Args...>
    && !std::is_nothrow_invocable_v<decltype(Func), T, Args...>
    && !std::is_pointer_v<std::remove_reference_t<T>>
    && std::is_void_v<std::invoke_result_t<decltype(Func), T, Args...>>
)
constexpr
std::expected<std::invoke_result_t<decltype(Func), T, Args...>, std::string>
make_noexcept(T&& obj, Args&&... args) noexcept;

// Handle member functions with obj pointer and return value
template<auto Func, typename T, typename... Args>
requires (
    std::is_member_function_pointer_v<decltype(Func)>
    && std::is_invocable_v<decltype(Func), T, Args...>
    && !std::is_nothrow_invocable_v<decltype(Func), T, Args...>
    && std::is_pointer_v<std::remove_reference_t<T>>
)
constexpr
std::expected<std::invoke_result_t<decltype(Func), T, Args...>, std::string>
make_noexcept(T&& obj, Args&&... args) noexcept;

// Handle non-capturing lambdas with return value
template<auto Lambda, typename... Args>
requires (
    !std::is_pointer_v<decltype(Lambda)>
    && !std::is_function_v<decltype(Lambda)>
    && std::is_invocable_v<decltype(Lambda), Args...>
    && !std::is_nothrow_invocable_v<decltype(Lambda), Args...>
    && !std::is_void_v<std::invoke_result_t<decltype(Lambda), Args...>>
)
std::expected<std::invoke_result_t<decltype(Lambda), Args...>, std::string>
make_noexcept(Args&&... args) noexcept;

// Handle non-capturing lambdas with void return
template<auto Lambda, typename... Args>
requires (
    !std::is_pointer_v<decltype(Lambda)>
    && !std::is_function_v<decltype(Lambda)>
    && std::is_invocable_v<decltype(Lambda), Args...>
    && !std::is_nothrow_invocable_v<decltype(Lambda), Args...>
    && std::is_void_v<std::invoke_result_t<decltype(Lambda), Args...>>
)
std::expected<std::invoke_result_t<decltype(Lambda), Args...>, std::string>
make_noexcept(Args&&... args) noexcept;

// Handle capturing lambdas with return value
template<typename Lambda, typename... Args>
requires (
    !std::is_pointer_v<Lambda>
    && !std::is_function_v<Lambda>
    && std::is_invocable_v<Lambda, Args...>
    && !std::is_nothrow_invocable_v<Lambda, Args...>
    && !std::is_void_v<std::invoke_result_t<Lambda, Args...>>
)
std::expected<std::invoke_result_t<Lambda, Args...>, std::string>
make_noexcept(Lambda&& lambda, Args&&... args) noexcept;

// Handle capturing lambdas with void return
template<typename Lambda, typename... Args>
requires (
    !std::is_pointer_v<Lambda>
    && !std::is_function_v<Lambda>
    && std::is_invocable_v<Lambda, Args...>
    && !std::is_nothrow_invocable_v<Lambda, Args...>
    && std::is_void_v<std::invoke_result_t<Lambda, Args...>>
)
std::expected<std::invoke_result_t<Lambda, Args...>, std::string>
make_noexcept(Lambda&& lambda, Args&&... args) noexcept;

} // namespace mica

#include <mica/make_noexcept.inl>
