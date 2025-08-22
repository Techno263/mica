#include <exception>
#include <functional>
#include <functional>
#include <type_traits>
#include <utility>

namespace mica {

// Handle free functions
template<auto Func, typename... Args>
requires(
    std::invocable<decltype(Func), Args...>
)
constexpr
std::expected<std::invoke_result_t<decltype(Func), Args...>, std::string>
make_noexcept(Args&&... args) noexcept
{
    static_assert(
        !std::is_nothrow_invocable_v<decltype(Func), Args...>,
        "It is unnecessary to wrap a noexcept function with make_noexcept"
    );
    using R = std::invoke_result_t<decltype(Func), Args...>;
    try {
        if constexpr (std::is_void_v<R>) {
            std::invoke(Func, std::forward<Args>(args)...);
            return std::expected<R, std::string>();
        } else {
            return std::invoke(Func, std::forward<Args>(args)...);
        }
    } catch (const std::exception& e) {
        return std::unexpected(e.what());
    } catch (...) {
        return std::unexpected("unexpected error");
    }
}

// Handle member function pointers
template<auto Func, typename T, typename... Args>
requires (
    std::invocable<decltype(Func), T&&, Args&&...>
)
constexpr
std::expected<std::invoke_result_t<decltype(Func), T&&, Args&&...>, std::string>
make_noexcept(T&& obj, Args&&... args) noexcept
{
    static_assert(
        !std::is_nothrow_invocable_v<decltype(Func), T&&, Args&&...>,
        "It is unnecessary to wrap a noexcept member function with make_noexcept"
    );
    using R = std::invoke_result_t<decltype(Func), T&&, Args&&...>;
    try {
        if constexpr (std::is_void_v<R>) {
            std::invoke(Func, std::forward<T>(obj), std::forward<Args>(args)...);
            return std::expected<R, std::string>();
        } else {
            return std::invoke(Func, std::forward<T>(obj), std::forward<Args>(args)...);
        }
    } catch (const std::exception& e) {
        return std::unexpected(e.what());
    } catch (...) {
        return std::unexpected("unexpected error");
    }
}

template<typename Lambda, typename... Args>
requires(
    std::invocable<Lambda, Args&&...>
)
constexpr
std::expected<std::invoke_result_t<Lambda, Args&&...>, std::string>
make_noexcept(Lambda&& lambda, Args&&... args) noexcept
{
    static_assert(
        !std::is_nothrow_invocable_v<Lambda, Args&&...>,
        "It is unnecessary to wrap a noexcept lambda with make_noexcept"
    );
    using R = std::invoke_result_t<Lambda, Args&&...>;
    try {
        if constexpr (std::is_void_v<R>) {
            std::invoke(std::forward<Lambda>(lambda), std::forward<Args>(args)...);
            return std::expected<R, std::string>();
        } else {
            return std::invoke(std::forward<Lambda>(lambda), std::forward<Args>(args)...);
        }
    } catch (const std::exception& e) {
        return std::unexpected(e.what());
    } catch (...) {
        return std::unexpected("unexpected error");
    }
}

} // namespace mica
