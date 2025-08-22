#pragma once

#include <format>

namespace mica {

template<typename... Args>
std::expected<std::string, std::string>
format(std::format_string<Args...> fmt, Args&&... args) noexcept;

} // namespace mica

#include <mica/format.inl>
