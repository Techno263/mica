#include <mica/resolve.hpp>
#include <mica/make_noexcept.hpp>
#include <utility>

namespace mica {

namespace internal {

template<typename... Args>
using FormatSig = std::string(std::format_string<Args...>, Args&&...);

template<typename... Args>
constexpr auto format_ptr = mica::resolve<FormatSig<Args...>, &std::format<Args...>>();

} // namespace mica::internal

template<typename... Args>
std::expected<std::string, std::string>
format(std::format_string<Args...> fmt, Args&&... args) noexcept
{
    return make_noexcept<internal::format_ptr<Args...>>(fmt, std::forward<Args>(args)...);
}

} // namespace mica
