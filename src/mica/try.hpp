#pragma once

#include <expected>
#include <mica/macro.hpp>
#include <mica/type_traits.hpp>
#include <type_traits>
#include <utility>

#define _MICA_INTERNAL_TRY(result_, expr_, tmp_exp_var_) \
do { \
    auto&& tmp_exp_var_ = (expr_); \
    static_assert(mica::is_expected_v<std::remove_reference_t<decltype(tmp_exp_var_)>>); \
    static_assert(std::is_same_v< \
        std::remove_reference_t<decltype(result_)>, \
        std::remove_reference_t<decltype(tmp_exp_var_)>::value_type> \
    ); \
    if (!tmp_exp_var_.has_value()) [[unlikely]] { \
        return std::unexpected(std::move(tmp_exp_var_).error()); \
    } \
    result_ = std::move(tmp_exp_var_).value(); \
} while (0)

#define MICA_TRY(result_, expr_) \
    _MICA_INTERNAL_TRY(result_, expr_, MICA_TMP_VAR_DEFAULT)

#define _MICA_INTERNAL_TRY_VOID(expr_, tmp_exp_var_) \
do { \
    auto&& tmp_exp_var_ = (expr_); \
    static_assert(mica::is_expected_v<std::remove_reference_t<decltype(tmp_exp_var_)>>); \
    static_assert(std::is_void_v<std::remove_reference_t<decltype(tmp_exp_var_)>::value_type>); \
    if (!tmp_exp_var_.has_value()) [[unlikely]] { \
        return std::unexpected(std::move(tmp_exp_var_).error()); \
    } \
} while (0)

#define MICA_TRY_VOID(expr_) \
    _MICA_INTERNAL_TRY_VOID(expr_, MICA_TMP_VAR_DEFAULT)

#define _MICA_INTERNAL_TRY_STATIC(result_, expr_, err_msg_, tmp_exp_var_) \
do { \
    auto&& tmp_exp_var_ = (expr_); \
    static_assert(mica::is_expected_v<std::remove_reference_t<decltype(tmp_exp_var_)>>); \
    static_assert(std::is_same_v< \
        std::remove_reference_t<decltype(result_)>, \
        std::remove_reference_t<decltype(tmp_exp_var_)>::value_type> \
    ); \
    static_assert(mica::is_string_literal(err_msg_)); \
    if (!tmp_exp_var_.has_value()) [[unlikely]] { \
        return std::unexpected(err_msg_); \
    } \
    result_ = std::move(tmp_exp_var_).value(); \
} while (0)

#define MICA_TRY_STATIC(result_, expr_, err_msg_) \
    _MICA_INTERNAL_TRY_STATIC(result_, expr_, err_msg_, MICA_TMP_VAR_DEFAULT)

#define _MICA_INTERNAL_TRY_STATIC_VOID(result_, expr_, err_msg_, tmp_exp_var_) \
do { \
    auto&& tmp_exp_var_ = (expr_); \
    static_assert(mica::is_expected_v<std::remove_reference_t<decltype(tmp_exp_var_)>>); \
    static_assert(std::is_void_v<std::remove_reference_t<decltype(tmp_exp_var_)>::value_type>); \
    static_assert(mica::is_string_literal(err_msg_)); \
    if (!tmp_exp_var_.has_value()) [[unlikely]] { \
        return std::unexpected(err_msg_); \
    } \
} while (0)

#define MICA_TRY_STATIC_VOID(result_, expr_, err_msg_) \
    _MICA_INTERNAL_TRY_STATIC_VOID(result_, expr_, err_msg_, MICA_TMP_VAR_DEFAULT)
