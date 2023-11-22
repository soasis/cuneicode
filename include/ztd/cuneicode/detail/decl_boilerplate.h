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

#pragma once

#ifndef ZTD_CUNEICODE_DETAIL_DECL_BOILERPLATE_H
#define ZTD_CUNEICODE_DETAIL_DECL_BOILERPLATE_H

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/max_output.h>
#include <ztd/cuneicode/mcerr.h>
#include <ztd/cuneicode/mcstate.h>
#include <ztd/cuneicode/result.h>

#include <ztd/idk/static_assert.h>
#include <ztd/idk/charN_t.h>
#include <ztd/idk/generic.h>
#include <ztd/idk/restrict.h>

#if ZTD_IS_ON(ZTD_CXX)
#include <cstddef>
#else
#include <stddef.h>
#include <stdbool.h>
#endif

#define CNC_OUTPUT_TYPE_TO_MAX(_TYPE)                                                           \
	ZTDC_CASCADING_GENERIC(*((_TYPE*)0), ztd_char32_t, CNC_C32_MAX, ztd_char16_t, CNC_C16_MAX, \
	     ztd_char8_t, CNC_C8_MAX, ztd_wchar_t, CNC_MWC_MAX, ztd_char_t, CNC_MC_MAX)

#define mc_CNC_TYPE_I_ ztd_char_t
#define mwc_CNC_TYPE_I_ ztd_wchar_t
#define c8_CNC_TYPE_I_ ztd_char8_t
#define c16_CNC_TYPE_I_ ztd_char16_t
#define c32_CNC_TYPE_I_ ztd_char32_t
#define mcs_CNC_TYPE_I_ ztd_char_t
#define mwcs_CNC_TYPE_I_ ztd_wchar_t
#define c8s_CNC_TYPE_I_ ztd_char8_t
#define c16s_CNC_TYPE_I_ ztd_char16_t
#define c32s_CNC_TYPE_I_ ztd_char32_t

#define TAG_TO_TYPE_EXPANDED_I_(_TAG) _TAG##_CNC_TYPE_I_
#define TAG_TO_TYPE_I_(_TAG) TAG_TO_TYPE_EXPANDED_I_(_TAG)

#define CNC_TYPED_CONVERSION_ERROR_DECL_ROOT_I_(_SHIM_NAME, _SHIM_R_NAME, _SHIM_FROM, _SHIM_TO)   \
	ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_error_result_t _SHIM_NAME(        \
	     size_t* restrict __p_maybe_dst_len, _SHIM_TO* restrict* restrict __p_maybe_dst,         \
	     size_t* restrict __p_src_len, const _SHIM_FROM* restrict* restrict __p_src,             \
                                                                                                  \
	     cnc_mcerr (*__error_handler)(cnc_mcerr, size_t* restrict, _SHIM_TO* restrict* restrict, \
	          size_t* restrict, const _SHIM_FROM* restrict* restrict, void* restrict,            \
	          void* restrict),                                                                   \
	     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_;                                   \
	ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_error_result_t _SHIM_R_NAME(      \
	     size_t* restrict __p_maybe_dst_len, _SHIM_TO* restrict* restrict __p_maybe_dst,         \
	     size_t* restrict __p_src_len, const _SHIM_FROM* restrict* restrict __p_src,             \
	     cnc_mcstate_t* restrict __p_state,                                                      \
	     cnc_mcerr (*__error_handler)(cnc_mcerr, size_t* restrict, _SHIM_TO* restrict* restrict, \
	          size_t* restrict, const _SHIM_FROM* restrict* restrict, void* restrict,            \
	          void* restrict),                                                                   \
	     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_

#define CNC_TYPED_CONVERSION_ERROR_DECL_I_(_SHIM_PREFIX, _SHIM_SUFFIX, _TAIL) \
	CNC_TYPED_CONVERSION_ERROR_DECL_ROOT_I_(                                 \
	     cnc_##_SHIM_PREFIX##nto##_SHIM_SUFFIX##n##_TAIL##_with_handler,     \
	     cnc_##_SHIM_PREFIX##nrto##_SHIM_SUFFIX##n##_TAIL##_with_handler,    \
	     TAG_TO_TYPE_I_(_SHIM_PREFIX), TAG_TO_TYPE_I_(_SHIM_SUFFIX));        \
	CNC_TYPED_CONVERSION_ERROR_DECL_ROOT_I_(                                 \
	     cnc_##_SHIM_PREFIX##snto##_SHIM_SUFFIX##sn##_TAIL##_with_handler,   \
	     cnc_##_SHIM_PREFIX##snrto##_SHIM_SUFFIX##sn##_TAIL##_with_handler,  \
	     TAG_TO_TYPE_I_(_SHIM_PREFIX), TAG_TO_TYPE_I_(_SHIM_SUFFIX))

#define CNC_TYPED_CONVERSION_DECL_ROOT_I_(_SHIM_NAME, _SHIM_R_NAME, _SHIM_FROM, _SHIM_TO) \
	ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr _SHIM_NAME(         \
	     size_t* restrict __p_maybe_dst_len, _SHIM_TO* restrict* restrict __p_maybe_dst, \
	     size_t* restrict __p_src_len, const _SHIM_FROM* restrict* restrict __p_src)     \
	     ZTD_NOEXCEPT_IF_CXX_I_;                                                         \
	ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr _SHIM_R_NAME(       \
	     size_t* restrict __p_maybe_dst_len, _SHIM_TO* restrict* restrict __p_maybe_dst, \
	     size_t* restrict __p_src_len, const _SHIM_FROM* restrict* restrict __p_src,     \
	     cnc_mcstate_t* restrict __p_state) ZTD_NOEXCEPT_IF_CXX_I_;

#define CNC_TYPED_CONVERSION_DECL_I_(_SHIM_PREFIX, _SHIM_SUFFIX, _TAIL)                     \
	CNC_TYPED_CONVERSION_DECL_ROOT_I_(cnc_##_SHIM_PREFIX##nto##_SHIM_SUFFIX##n##_TAIL,     \
	     cnc_##_SHIM_PREFIX##nrto##_SHIM_SUFFIX##n##_TAIL, TAG_TO_TYPE_I_(_SHIM_PREFIX),   \
	     TAG_TO_TYPE_I_(_SHIM_SUFFIX));                                                    \
	CNC_TYPED_CONVERSION_DECL_ROOT_I_(cnc_##_SHIM_PREFIX##snto##_SHIM_SUFFIX##sn##_TAIL,   \
	     cnc_##_SHIM_PREFIX##snrto##_SHIM_SUFFIX##sn##_TAIL, TAG_TO_TYPE_I_(_SHIM_PREFIX), \
	     TAG_TO_TYPE_I_(_SHIM_SUFFIX))

#endif
