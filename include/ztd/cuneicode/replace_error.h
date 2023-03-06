// ============================================================================
//
// ztd.cuneicode
// Copyright © 2022-2023 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
// Contact: opensource@soasis.org
//
// Commercial License Usage
// Licensees holding valid commercial ztd.cuneicode licenses may use this file
// in accordance with the commercial license agreement provided with the
// Software or, alternatively, in accordance with the terms contained in
// a written agreement between you and Shepherd's Oasis, LLC.
// For licensing terms and conditions see your agreement. For
// further information contact opensource@soasis.org.
//
// Apache License Version 2 Usage
// Alternatively, this file may be used under the terms of Apache License
// Version 2.0 (the "License"); you may not use this file except in compliance
// with the License. You may obtain a copy of the License at
//
// https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ========================================================================= //

#ifndef ZTD_CUNEICODE_REPLACE_ERROR_H
#define ZTD_CUNEICODE_REPLACE_ERROR_H

#pragma once

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/mcerr.h>
#include <ztd/cuneicode/mcstate.h>
#include <ztd/cuneicode/error_handler_function.h>
#include <ztd/cuneicode/detail/incompatible_key.h>

#include <ztd/idk/charN_t.h>
#include <ztd/idk/restrict.h>

#if ZTD_IS_ON(ZTD_CXX)
#include <cstddef>
#else
#include <stddef.h>
#endif

typedef struct cnc_replace_error_cxntocyn_t {
	__cnc_detail_incompatible_key __sjkfhdjoiqniognuikhGmpQGnioQGb;
} cnc_replace_error_cxntocyn_t;
ZTD_EXTERN_C_I_ ZTD_EXTERN_IF_C_I_ cnc_replace_error_cxntocyn_t cnc_replace_error_cxntocyn;

//////
/// @addtogroup ztd_cuneicode_error_handlers Error Handler Functions
///
/// @{

// to_mcn
ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c32ntomcn(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char32_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c32ntomcn_utf8(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char32_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c16ntomcn(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char16_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c16ntomcn_utf8(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char16_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c8ntomcn(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char8_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c8ntomcn_utf8(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char8_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mwcntomcn(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_wchar_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_replace_error_mwcntomcn_wide_exec_utf8(cnc_mcerr __current_error,
     size_t* restrict __p_maybe_dst_len, ztd_char_t* restrict* restrict __p_maybe_dst,
     size_t* restrict __p_src_len, const ztd_wchar_t* restrict* restrict __p_src,
     void* restrict __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mcntomcn(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_replace_error_mcntomcn_exec_utf8(cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_replace_error_mcntomcn_utf8_exec(cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

// to_mwcn
ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c32ntomwcn(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_wchar_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char32_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c16ntomwcn(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_wchar_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char16_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c8ntomwcn(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_wchar_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char8_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mwcntomwcn(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_wchar_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_wchar_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mcntomwcn(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_wchar_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_replace_error_mcntomwcn_utf8_wide_exec(cnc_mcerr __current_error,
     size_t* restrict __p_maybe_dst_len, ztd_wchar_t* restrict* restrict __p_maybe_dst,
     size_t* restrict __p_src_len, const ztd_char_t* restrict* restrict __p_src,
     void* restrict __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

// to_c8n
ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c32ntoc8n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char8_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char32_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c16ntoc8n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char8_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char16_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c8ntoc8n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char8_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char8_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mwcntoc8n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char8_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_wchar_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mcntoc8n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char8_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mcntoc8n_utf8(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char8_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

// to_c16n
ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c32ntoc16n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char16_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char32_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c16ntoc16n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char16_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char16_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c8ntoc16n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char16_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char8_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mwcntoc16n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char16_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_wchar_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mcntoc16n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char16_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mcntoc16n_utf8(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char16_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

// to_c32n
ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c32ntoc32n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char32_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char32_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c16ntoc32n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char32_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char16_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_c8ntoc32n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char32_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char8_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mwcntoc32n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char32_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_wchar_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mcntoc32n(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char32_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_replace_error_mcntoc32n_utf8(
     cnc_mcerr __current_error, size_t* restrict __p_maybe_dst_len,
     ztd_char32_t* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,
     const ztd_char_t* restrict* restrict __p_src, void* restrict __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;

//////
/// @}

#endif
