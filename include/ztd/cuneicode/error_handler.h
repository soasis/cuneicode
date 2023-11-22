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

#ifndef ZTD_CUNEICODE_ERROR_HANDLER_H
#define ZTD_CUNEICODE_ERROR_HANDLER_H

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/replace_error.h>
#include <ztd/cuneicode/skip_input_error.h>
#include <ztd/cuneicode/skip_input_and_replace_error.h>

#include <ztd/idk/generic.h>

#define __cnc_detail_select_error_handler_siare_cxntoc32n(__p_maybe_dst, __p_src) \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t,                            \
	     cnc_skip_input_and_replace_error_c32ntoc32n, ztd_char16_t,              \
	     cnc_skip_input_and_replace_error_c16ntoc32n, ztd_char8_t,               \
	     cnc_skip_input_and_replace_error_c8ntoc32n, ztd_wchar_t,                \
	     cnc_skip_input_and_replace_error_mwcntoc32n, ztd_char_t,                \
	     cnc_skip_input_and_replace_error_mcntoc32n, ZTDC_DEFAULT,               \
	     cnc_skip_input_and_replace_error_c8ntoc32n)

#define __cnc_detail_select_error_handler_siare_cxntoc16n(__p_maybe_dst, __p_src) \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t,                            \
	     cnc_skip_input_and_replace_error_c32ntoc16n, ztd_char16_t,              \
	     cnc_skip_input_and_replace_error_c16ntoc16n, ztd_char8_t,               \
	     cnc_skip_input_and_replace_error_c8ntoc16n, ztd_wchar_t,                \
	     cnc_skip_input_and_replace_error_mwcntoc16n, ztd_char_t,                \
	     cnc_skip_input_and_replace_error_mcntoc16n, ZTDC_DEFAULT,               \
	     cnc_skip_input_and_replace_error_c8ntoc16n)

#define __cnc_detail_select_error_handler_siare_cxntoc8n(__p_maybe_dst, __p_src)                   \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_skip_input_and_replace_error_c32ntoc8n, \
	     ztd_char16_t, cnc_skip_input_and_replace_error_c16ntoc8n, ztd_char8_t,                   \
	     cnc_skip_input_and_replace_error_c8ntoc8n, ztd_wchar_t,                                  \
	     cnc_skip_input_and_replace_error_mwcntoc8n, ztd_char_t,                                  \
	     cnc_skip_input_and_replace_error_mcntoc8n, ZTDC_DEFAULT,                                 \
	     cnc_skip_input_and_replace_error_c8ntoc8n)

#define __cnc_detail_select_error_handler_siare_cxntomwcn(__p_maybe_dst, __p_src) \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t,                            \
	     cnc_skip_input_and_replace_error_c32ntomwcn, ztd_char16_t,              \
	     cnc_skip_input_and_replace_error_c16ntomwcn, ztd_char8_t,               \
	     cnc_skip_input_and_replace_error_c8ntomwcn, ztd_wchar_t,                \
	     cnc_skip_input_and_replace_error_mwcntomwcn, ztd_char_t,                \
	     cnc_skip_input_and_replace_error_mcntomwcn, ZTDC_DEFAULT,               \
	     cnc_skip_input_and_replace_error_c8ntomwcn)

#define __cnc_detail_select_error_handler_siare_cxntomcn(__p_maybe_dst, __p_src)                   \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_skip_input_and_replace_error_c32ntomcn, \
	     ztd_char16_t, cnc_skip_input_and_replace_error_c16ntomcn, ztd_char8_t,                   \
	     cnc_skip_input_and_replace_error_c8ntomcn, ztd_wchar_t,                                  \
	     cnc_skip_input_and_replace_error_mwcntomcn, ztd_char_t,                                  \
	     cnc_skip_input_and_replace_error_mcntomcn, ZTDC_DEFAULT,                                 \
	     cnc_skip_input_and_replace_error_c8ntomcn)

#define __cnc_detail_select_error_handler_siare_cxntocxn(__p_maybe_dst, __p_src)         \
	ZTDC_CASCADING_GENERIC(**(__p_maybe_dst), ztd_char32_t,                             \
	     __cnc_detail_select_error_handler_siare_cxntoc32n((__p_maybe_dst), (__p_src)), \
	     ztd_char16_t,                                                                  \
	     __cnc_detail_select_error_handler_siare_cxntoc16n((__p_maybe_dst), (__p_src)), \
	     ztd_char8_t,                                                                   \
	     __cnc_detail_select_error_handler_siare_cxntoc8n((__p_maybe_dst), (__p_src)),  \
	     ztd_wchar_t,                                                                   \
	     __cnc_detail_select_error_handler_siare_cxntomwcn((__p_maybe_dst), (__p_src)), \
	     ztd_char_t,                                                                    \
	     __cnc_detail_select_error_handler_siare_cxntomcn((__p_maybe_dst), (__p_src)),  \
	     ZTDC_DEFAULT,                                                                  \
	     __cnc_detail_select_error_handler_siare_cxntoc8n((__p_maybe_dst), (__p_src)))


#define __cnc_detail_select_error_handler_sie_cxntoc32n(__p_maybe_dst, __p_src)         \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_skip_input_error_c32ntoc32n, \
	     ztd_char16_t, cnc_skip_input_error_c16ntoc32n, ztd_char8_t,                   \
	     cnc_skip_input_error_c8ntoc32n, ztd_wchar_t, cnc_skip_input_error_mwcntoc32n, \
	     ztd_char_t, cnc_skip_input_error_mcntoc32n, ZTDC_DEFAULT,                     \
	     cnc_skip_input_error_c8ntoc32n)

#define __cnc_detail_select_error_handler_sie_cxntoc16n(__p_maybe_dst, __p_src)         \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_skip_input_error_c32ntoc16n, \
	     ztd_char16_t, cnc_skip_input_error_c16ntoc16n, ztd_char8_t,                   \
	     cnc_skip_input_error_c8ntoc16n, ztd_wchar_t, cnc_skip_input_error_mwcntoc16n, \
	     ztd_char_t, cnc_skip_input_error_mcntoc16n, ZTDC_DEFAULT,                     \
	     cnc_skip_input_error_c8ntoc16n)

#define __cnc_detail_select_error_handler_sie_cxntoc8n(__p_maybe_dst, __p_src)                    \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_skip_input_error_c32ntoc8n,            \
	     ztd_char16_t, cnc_skip_input_error_c16ntoc8n, ztd_char8_t,                              \
	     cnc_skip_input_error_c8ntoc8n, ztd_wchar_t, cnc_skip_input_error_mwcntoc8n, ztd_char_t, \
	     cnc_skip_input_error_mcntoc8n, ZTDC_DEFAULT, cnc_skip_input_error_c8ntoc8n)

#define __cnc_detail_select_error_handler_sie_cxntomwcn(__p_maybe_dst, __p_src)         \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_skip_input_error_c32ntomwcn, \
	     ztd_char16_t, cnc_skip_input_error_c16ntomwcn, ztd_char8_t,                   \
	     cnc_skip_input_error_c8ntomwcn, ztd_wchar_t, cnc_skip_input_error_mwcntomwcn, \
	     ztd_char_t, cnc_skip_input_error_mcntomwcn, ZTDC_DEFAULT,                     \
	     cnc_skip_input_error_c8ntomwcn)

#define __cnc_detail_select_error_handler_sie_cxntomcn(__p_maybe_dst, __p_src)                    \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_skip_input_error_c32ntomcn,            \
	     ztd_char16_t, cnc_skip_input_error_c16ntomcn, ztd_char8_t,                              \
	     cnc_skip_input_error_c8ntomcn, ztd_wchar_t, cnc_skip_input_error_mwcntomcn, ztd_char_t, \
	     cnc_skip_input_error_mcntomcn, ZTDC_DEFAULT, cnc_skip_input_error_c8ntomcn)

#define __cnc_detail_select_error_handler_sie_cxntocxn(__p_maybe_dst, __p_src)                     \
	ZTDC_CASCADING_GENERIC(**(__p_maybe_dst), ztd_char32_t,                                       \
	     __cnc_detail_select_error_handler_sie_cxntoc32n((__p_maybe_dst), (__p_src)),             \
	     ztd_char16_t,                                                                            \
	     __cnc_detail_select_error_handler_sie_cxntoc16n((__p_maybe_dst), (__p_src)),             \
	     ztd_char8_t, __cnc_detail_select_error_handler_sie_cxntoc8n((__p_maybe_dst), (__p_src)), \
	     ztd_wchar_t,                                                                             \
	     __cnc_detail_select_error_handler_sie_cxntomwcn((__p_maybe_dst), (__p_src)), ztd_char_t, \
	     __cnc_detail_select_error_handler_sie_cxntomcn((__p_maybe_dst), (__p_src)),              \
	     ZTDC_DEFAULT,                                                                            \
	     __cnc_detail_select_error_handler_sie_cxntoc8n((__p_maybe_dst), (__p_src)))



#define __cnc_detail_select_error_handler_re_cxntoc32n(__p_maybe_dst, __p_src)                     \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_replace_error_c32ntoc32n, ztd_char16_t, \
	     cnc_replace_error_c16ntoc32n, ztd_char8_t, cnc_replace_error_c8ntoc32n, ztd_wchar_t,     \
	     cnc_replace_error_mwcntoc32n, ztd_char_t, cnc_replace_error_mcntoc32n, ZTDC_DEFAULT,     \
	     cnc_replace_error_c8ntoc32n)

#define __cnc_detail_select_error_handler_re_cxntoc16n(__p_maybe_dst, __p_src)                     \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_replace_error_c32ntoc16n, ztd_char16_t, \
	     cnc_replace_error_c16ntoc16n, ztd_char8_t, cnc_replace_error_c8ntoc16n, ztd_wchar_t,     \
	     cnc_replace_error_mwcntoc16n, ztd_char_t, cnc_replace_error_mcntoc16n, ZTDC_DEFAULT,     \
	     cnc_replace_error_c8ntoc16n)

#define __cnc_detail_select_error_handler_re_cxntoc8n(__p_maybe_dst, __p_src)                     \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_replace_error_c32ntoc8n, ztd_char16_t, \
	     cnc_replace_error_c16ntoc8n, ztd_char8_t, cnc_replace_error_c8ntoc8n, ztd_wchar_t,      \
	     cnc_replace_error_mwcntoc8n, ztd_char_t, cnc_replace_error_mcntoc8n, ZTDC_DEFAULT,      \
	     cnc_replace_error_c8ntoc8n)

#define __cnc_detail_select_error_handler_re_cxntomwcn(__p_maybe_dst, __p_src)                     \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_replace_error_c32ntomwcn, ztd_char16_t, \
	     cnc_replace_error_c16ntomwcn, ztd_char8_t, cnc_replace_error_c8ntomwcn, ztd_wchar_t,     \
	     cnc_replace_error_mwcntomwcn, ztd_char_t, cnc_replace_error_mcntomwcn, ZTDC_DEFAULT,     \
	     cnc_replace_error_c8ntomwcn)

#define __cnc_detail_select_error_handler_re_cxntomcn(__p_maybe_dst, __p_src)                     \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_replace_error_c32ntomcn, ztd_char16_t, \
	     cnc_replace_error_c16ntomcn, ztd_char8_t, cnc_replace_error_c8ntomcn, ztd_wchar_t,      \
	     cnc_replace_error_mwcntomcn, ztd_char_t, cnc_replace_error_mcntomcn, ZTDC_DEFAULT,      \
	     cnc_replace_error_c8ntomcn)

#define __cnc_detail_select_error_handler_re_cxntocxn(__p_maybe_dst, __p_src)                      \
	ZTDC_CASCADING_GENERIC(**(__p_maybe_dst), ztd_char32_t,                                       \
	     __cnc_detail_select_error_handler_re_cxntoc32n((__p_maybe_dst), (__p_src)),              \
	     ztd_char16_t,                                                                            \
	     __cnc_detail_select_error_handler_re_cxntoc16n((__p_maybe_dst), (__p_src)), ztd_char8_t, \
	     __cnc_detail_select_error_handler_re_cxntoc8n((__p_maybe_dst), (__p_src)), ztd_wchar_t,  \
	     __cnc_detail_select_error_handler_re_cxntomwcn((__p_maybe_dst), (__p_src)), ztd_char_t,  \
	     __cnc_detail_select_error_handler_re_cxntomcn((__p_maybe_dst), (__p_src)), ZTDC_DEFAULT, \
	     __cnc_detail_select_error_handler_re_cxntoc8n((__p_maybe_dst), (__p_src)))


#define __cnc_detail_select_error_handler_cxntocyn(__error_handler, __p_maybe_dst, __p_src)    \
	ZTDC_CASCADING_GENERIC(__error_handler, cnc_skip_input_and_replace_error_t,               \
	     __cnc_detail_select_error_handler_siare_cxntocxn(__p_maybe_dst, __p_src),            \
	     cnc_skip_input_error_t,                                                              \
	     __cnc_detail_select_error_handler_sie_cxntocxn(__p_maybe_dst, __p_src),              \
	     cnc_replace_error_cxntocyn_t,                                                        \
	     __cnc_detail_select_error_handler_re_cxntocxn(__p_maybe_dst, __p_src), ZTDC_DEFAULT, \
	     __error_handler)

#endif
