#pragma once

#include <expected>
#include <mica/macro.hpp>
#include <mica/type_traits.hpp>
#include <utility>

#define _MICA_INTERNAL_TRY(result, expr, tmp_exp_var) \
do { \
    auto&& tmp_exp_var = (expr); \
    static_assert(is_expected_v<decltype(tmp_exp_var)>); \
    static_assert(std::is_same<std::remove_reference_t<decltype(result)>, decltype(tmp_exp_var)::value_type>); \
    if (!tmp_exp_var.has_error()) [[unlikely]] { \
        return std::unexpected(std::move(tmp_exp_var).error()); \
    } \
    result = std::move(tmp_exp_var).value(); \
} while (0)

#define MICA_TRY(result, expr) \
    _MICA_INTERNAL_TRY(result, expr, MICA_TMP_VAR_DEFAULT)
