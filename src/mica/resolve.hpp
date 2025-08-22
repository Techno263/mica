#pragma once

namespace mica {

// Resolve free function
template<typename Sig, Sig* Func>
constexpr auto resolve() -> Sig*;

} // namespace mica

#include <mica/resolve.inl>
