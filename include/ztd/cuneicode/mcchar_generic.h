// ============================================================================
//
// ztd.cuneicode
// Copyright © 2022-2023 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
// Contact, opensource@soasis.org
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

#ifndef ZTD_CUNEICODE_MCCHAR_GENERIC_H
#define ZTD_CUNEICODE_MCCHAR_GENERIC_H

#pragma once

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/mcerr.h>
#include <ztd/cuneicode/detail/mccharn.h>
#include <ztd/cuneicode/detail/mccharn_with_handler.h>
#include <ztd/cuneicode/error_handler.h>

#include <ztd/idk/generic.h>

#define __cnc_detail_cxntoc32n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src)         \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32ntoc32n, ztd_char16_t,           \
	     cnc_c16ntoc32n, ztd_char8_t, cnc_c8ntoc32n, ztd_wchar_t, cnc_mwcntoc32n, ztd_char_t, \
	     cnc_mcntoc32n, ZTDC_DEFAULT, cnc_c8ntoc32n)

#define __cnc_detail_cxntoc16n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src)         \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32ntoc16n, ztd_char16_t,           \
	     cnc_c16ntoc16n, ztd_char8_t, cnc_c8ntoc16n, ztd_wchar_t, cnc_mwcntoc16n, ztd_char_t, \
	     cnc_mcntoc16n, ZTDC_DEFAULT, cnc_c8ntoc16n)

#define __cnc_detail_cxntoc8n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src)              \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32ntoc8n, ztd_char16_t, cnc_c16ntoc8n, \
	     ztd_char8_t, cnc_c8ntoc8n, ztd_wchar_t, cnc_mwcntoc8n, ztd_char_t, cnc_mcntoc8n,         \
	     ZTDC_DEFAULT, cnc_c8ntoc8n)

#define __cnc_detail_cxntomwcn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src)         \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32ntomwcn, ztd_char16_t,           \
	     cnc_c16ntomwcn, ztd_char8_t, cnc_c8ntomwcn, ztd_wchar_t, cnc_mwcntomwcn, ztd_char_t, \
	     cnc_mcntomwcn, ZTDC_DEFAULT, cnc_c8ntomwcn)

#define __cnc_detail_cxntomcn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src)              \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32ntomcn, ztd_char16_t, cnc_c16ntomcn, \
	     ztd_char8_t, cnc_c8ntomcn, ztd_wchar_t, cnc_mwcntomcn, ztd_char_t, cnc_mcntomcn,         \
	     ZTDC_DEFAULT, cnc_c8ntomcn)

#define __cnc_detail_cxnrtoc32n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state) \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32nrtoc32n, ztd_char16_t,              \
	     cnc_c16nrtoc32n, ztd_char8_t, cnc_c8nrtoc32n, ztd_wchar_t, cnc_mwcnrtoc32n, ztd_char_t,  \
	     cnc_mcnrtoc32n, ZTDC_DEFAULT, cnc_c8nrtoc32n)

#define __cnc_detail_cxnrtoc16n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state) \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32nrtoc16n, ztd_char16_t,              \
	     cnc_c16nrtoc16n, ztd_char8_t, cnc_c8nrtoc16n, ztd_wchar_t, cnc_mwcnrtoc16n, ztd_char_t,  \
	     cnc_mcnrtoc16n, ZTDC_DEFAULT, cnc_c8nrtoc16n)

#define __cnc_detail_cxnrtoc8n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state) \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32nrtoc8n, ztd_char16_t,              \
	     cnc_c16nrtoc8n, ztd_char8_t, cnc_c8nrtoc8n, ztd_wchar_t, cnc_mwcnrtoc8n, ztd_char_t,    \
	     cnc_mcnrtoc8n, ZTDC_DEFAULT, cnc_c8nrtoc8n)

#define __cnc_detail_cxnrtomwcn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state) \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32nrtomwcn, ztd_char16_t,              \
	     cnc_c16nrtomwcn, ztd_char8_t, cnc_c8nrtomwcn, ztd_wchar_t, cnc_mwcnrtomwcn, ztd_char_t,  \
	     cnc_mcnrtomwcn, ZTDC_DEFAULT, cnc_c8nrtomwcn)

#define __cnc_detail_cxnrtomcn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state) \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32nrtomcn, ztd_char16_t,              \
	     cnc_c16nrtomcn, ztd_char8_t, cnc_c8nrtomcn, ztd_wchar_t, cnc_mwcnrtomcn, ztd_char_t,    \
	     cnc_mcnrtomcn, ZTDC_DEFAULT, cnc_c8nrtomcn)

#define __cnc_detail_cxsntoc32sn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src) \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32sntoc32sn, ztd_char16_t,   \
	     cnc_c16sntoc32sn, ztd_char8_t, cnc_c8sntoc32sn, ztd_wchar_t, cnc_mwcsntoc32sn, \
	     ztd_char_t, cnc_mcsntoc32sn, ZTDC_DEFAULT, cnc_c8sntoc32sn)

#define __cnc_detail_cxsntoc16sn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src) \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32sntoc16sn, ztd_char16_t,   \
	     cnc_c16sntoc16sn, ztd_char8_t, cnc_c8sntoc16sn, ztd_wchar_t, cnc_mwcsntoc16sn, \
	     ztd_char_t, cnc_mcsntoc16sn, ZTDC_DEFAULT, cnc_c8sntoc16sn)

#define __cnc_detail_cxsntoc8sn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src)           \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32sntoc8sn, ztd_char16_t,             \
	     cnc_c16sntoc8sn, ztd_char8_t, cnc_c8sntoc8sn, ztd_wchar_t, cnc_mwcsntoc8sn, ztd_char_t, \
	     cnc_mcsntoc8sn, ZTDC_DEFAULT, cnc_c8sntoc8sn)

#define __cnc_detail_cxsntomwcsn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src) \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32sntomwcsn, ztd_char16_t,   \
	     cnc_c16sntomwcsn, ztd_char8_t, cnc_c8sntomwcsn, ztd_wchar_t, cnc_mwcsntomwcsn, \
	     ztd_char_t, cnc_mcsntomwcsn, ZTDC_DEFAULT, cnc_c8sntomwcsn)

#define __cnc_detail_cxsntomcsn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src)           \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32sntomcsn, ztd_char16_t,             \
	     cnc_c16sntomcsn, ztd_char8_t, cnc_c8sntomcsn, ztd_wchar_t, cnc_mwcsntomcsn, ztd_char_t, \
	     cnc_mcsntomcsn, ZTDC_DEFAULT, cnc_c8sntomcsn)

#define __cnc_detail_cxsnrtoc32sn(                                                          \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state)                     \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32snrtoc32sn, ztd_char16_t,     \
	     cnc_c16snrtoc32sn, ztd_char8_t, cnc_c8snrtoc32sn, ztd_wchar_t, cnc_mwcsnrtoc32sn, \
	     ztd_char_t, cnc_mcsnrtoc32sn, ZTDC_DEFAULT, cnc_c8snrtoc32sn)

#define __cnc_detail_cxsnrtoc16sn(                                                          \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state)                     \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32snrtoc16sn, ztd_char16_t,     \
	     cnc_c16snrtoc16sn, ztd_char8_t, cnc_c8snrtoc16sn, ztd_wchar_t, cnc_mwcsnrtoc16sn, \
	     ztd_char_t, cnc_mcsnrtoc16sn, ZTDC_DEFAULT, cnc_c8snrtoc16sn)

#define __cnc_detail_cxsnrtoc8sn(                                                        \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state)                  \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32snrtoc8sn, ztd_char16_t,   \
	     cnc_c16snrtoc8sn, ztd_char8_t, cnc_c8snrtoc8sn, ztd_wchar_t, cnc_mwcsnrtoc8sn, \
	     ztd_char_t, cnc_mcsnrtoc8sn, ZTDC_DEFAULT, cnc_c8snrtoc8sn)

#define __cnc_detail_cxsnrtomwcsn(                                                          \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state)                     \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32snrtomwcsn, ztd_char16_t,     \
	     cnc_c16snrtomwcsn, ztd_char8_t, cnc_c8snrtomwcsn, ztd_wchar_t, cnc_mwcsnrtomwcsn, \
	     ztd_char_t, cnc_mcsnrtomwcsn, ZTDC_DEFAULT, cnc_c8snrtomwcsn)

#define __cnc_detail_cxsnrtomcsn(                                                        \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state)                  \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32snrtomcsn, ztd_char16_t,   \
	     cnc_c16snrtomcsn, ztd_char8_t, cnc_c8snrtomcsn, ztd_wchar_t, cnc_mwcsnrtomcsn, \
	     ztd_char_t, cnc_mcsnrtomcsn, ZTDC_DEFAULT, cnc_c8snrtomcsn)

#define __cnc_detail_cxntocxn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src)             \
	ZTDC_CASCADING_GENERIC(**(__p_maybe_dst), ztd_char32_t,                                      \
	     __cnc_detail_cxntoc32n((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src)), \
	     ztd_char16_t,                                                                           \
	     __cnc_detail_cxntoc16n((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src)), \
	     ztd_char8_t,                                                                            \
	     __cnc_detail_cxntoc8n((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src)),  \
	     ztd_wchar_t,                                                                            \
	     __cnc_detail_cxntomwcn((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src)), \
	     ztd_char_t,                                                                             \
	     __cnc_detail_cxntomcn((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src)),  \
	     ZTDC_DEFAULT,                                                                           \
	     __cnc_detail_cxntoc8n((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src)))

#define __cnc_detail_cxnrtocxn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state) \
	ZTDC_CASCADING_GENERIC(**(__p_maybe_dst), ztd_char32_t,                                      \
	     __cnc_detail_cxnrtoc32n(                                                                \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src), (__p_state)),      \
	     ztd_char16_t,                                                                           \
	     __cnc_detail_cxnrtoc16n(                                                                \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src), (__p_state)),      \
	     ztd_char8_t,                                                                            \
	     __cnc_detail_cxnrtoc8n(                                                                 \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src), (__p_state)),      \
	     ztd_wchar_t,                                                                            \
	     __cnc_detail_cxnrtomwcn(                                                                \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src), (__p_state)),      \
	     ztd_char_t,                                                                             \
	     __cnc_detail_cxnrtomcn(                                                                 \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src), (__p_state)),      \
	     ZTDC_DEFAULT,                                                                           \
	     __cnc_detail_cxnrtoc8n(                                                                 \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src), (__p_state)))

#define __cnc_detail_cxsntocxsn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src)            \
	ZTDC_CASCADING_GENERIC(**(__p_maybe_dst), ztd_char32_t,                                       \
	     __cnc_detail_cxsntoc32sn(                                                                \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src)),                    \
	     ztd_char16_t,                                                                            \
	     __cnc_detail_cxsntoc16sn(                                                                \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src)),                    \
	     ztd_char8_t,                                                                             \
	     __cnc_detail_cxsntoc8sn((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src)), \
	     ztd_wchar_t,                                                                             \
	     __cnc_detail_cxsntomwcsn(                                                                \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src)),                    \
	     ztd_char_t,                                                                              \
	     __cnc_detail_cxsntomcsn((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src)), \
	     ZTDC_DEFAULT,                                                                            \
	     __cnc_detail_cxsntoc8sn((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src)))

#define __cnc_detail_cxsnrtocxsn(                                                             \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state)                       \
	ZTDC_CASCADING_GENERIC(**(__p_maybe_dst), ztd_char32_t,                                  \
	     (__cnc_detail_cxsnrtoc32sn(                                                         \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src), (__p_state))), \
	     ztd_char16_t,                                                                       \
	     (__cnc_detail_cxsnrtoc16sn(                                                         \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src), (__p_state))), \
	     ztd_char8_t,                                                                        \
	     (__cnc_detail_cxsnrtoc8sn(                                                          \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src), (__p_state))), \
	     ztd_wchar_t,                                                                        \
	     (__cnc_detail_cxsnrtomwcsn(                                                         \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src), (__p_state))), \
	     ztd_char_t,                                                                         \
	     (__cnc_detail_cxsnrtomcsn(                                                          \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src), (__p_state))), \
	     ZTDC_DEFAULT,                                                                       \
	     (__cnc_detail_cxsnrtoc8sn(                                                          \
	          (__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), (__p_src), (__p_state))))

#define __cnc_detail_cxntoc32n_with_handler(                                                      \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __error_handler, __p_user_data)      \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32ntoc32n_with_handler, ztd_char16_t, \
	     cnc_c16ntoc32n_with_handler, ztd_char8_t, cnc_c8ntoc32n_with_handler, ztd_wchar_t,      \
	     cnc_mwcntoc32n_with_handler, ztd_char_t, cnc_mcntoc32n_with_handler, ZTDC_DEFAULT,      \
	     cnc_c8ntoc32n_with_handler)

#define __cnc_detail_cxntoc16n_with_handler(                                                      \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __error_handler, __p_user_data)      \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32ntoc16n_with_handler, ztd_char16_t, \
	     cnc_c16ntoc16n_with_handler, ztd_char8_t, cnc_c8ntoc16n_with_handler, ztd_wchar_t,      \
	     cnc_mwcntoc16n_with_handler, ztd_char_t, cnc_mcntoc16n_with_handler, ZTDC_DEFAULT,      \
	     cnc_c8ntoc16n_with_handler)

#define __cnc_detail_cxntoc8n_with_handler(                                                      \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __error_handler, __p_user_data)     \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32ntoc8n_with_handler, ztd_char16_t, \
	     cnc_c16ntoc8n_with_handler, ztd_char8_t, cnc_c8ntoc8n_with_handler, ztd_wchar_t,       \
	     cnc_mwcntoc8n_with_handler, ztd_char_t, cnc_mcntoc8n_with_handler, ZTDC_DEFAULT,       \
	     cnc_c8ntoc8n_with_handler)

#define __cnc_detail_cxntomwcn_with_handler(                                                      \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __error_handler, __p_user_data)      \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32ntomwcn_with_handler, ztd_char16_t, \
	     cnc_c16ntomwcn_with_handler, ztd_char8_t, cnc_c8ntomwcn_with_handler, ztd_wchar_t,      \
	     cnc_mwcntomwcn_with_handler, ztd_char_t, cnc_mcntomwcn_with_handler, ZTDC_DEFAULT,      \
	     cnc_c8ntomwcn_with_handler)

#define __cnc_detail_cxntomcn_with_handler(                                                      \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __error_handler, __p_user_data)     \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32ntomcn_with_handler, ztd_char16_t, \
	     cnc_c16ntomcn_with_handler, ztd_char8_t, cnc_c8ntomcn_with_handler, ztd_wchar_t,       \
	     cnc_mwcntomcn_with_handler, ztd_char_t, cnc_mcntomcn_with_handler, ZTDC_DEFAULT,       \
	     cnc_c8ntomcn_with_handler)

#define __cnc_detail_cxnrtoc32n_with_handler(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,        \
     __p_src, __p_state, __error_handler, __p_user_data)                                           \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32nrtoc32n_with_handler, ztd_char16_t, \
	     cnc_c16nrtoc32n_with_handler, ztd_char8_t, cnc_c8nrtoc32n_with_handler, ztd_wchar_t,     \
	     cnc_mwcnrtoc32n_with_handler, ztd_char_t, cnc_mcnrtoc32n_with_handler, ZTDC_DEFAULT,     \
	     cnc_c8nrtoc32n_with_handler)

#define __cnc_detail_cxnrtoc16n_with_handler(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,        \
     __p_src, __p_state, __error_handler, __p_user_data)                                           \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32nrtoc16n_with_handler, ztd_char16_t, \
	     cnc_c16nrtoc16n_with_handler, ztd_char8_t, cnc_c8nrtoc16n_with_handler, ztd_wchar_t,     \
	     cnc_mwcnrtoc16n_with_handler, ztd_char_t, cnc_mcnrtoc16n_with_handler, ZTDC_DEFAULT,     \
	     cnc_c8nrtoc16n_with_handler)

#define __cnc_detail_cxnrtoc8n_with_handler(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,        \
     __p_src, __p_state, __error_handler, __p_user_data)                                          \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32nrtoc8n_with_handler, ztd_char16_t, \
	     cnc_c16nrtoc8n_with_handler, ztd_char8_t, cnc_c8nrtoc8n_with_handler, ztd_wchar_t,      \
	     cnc_mwcnrtoc8n_with_handler, ztd_char_t, cnc_mcnrtoc8n_with_handler, ZTDC_DEFAULT,      \
	     cnc_c8nrtoc8n_with_handler)

#define __cnc_detail_cxnrtomwcn_with_handler(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,        \
     __p_src, __p_state, __error_handler, __p_user_data)                                           \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32nrtomwcn_with_handler, ztd_char16_t, \
	     cnc_c16nrtomwcn_with_handler, ztd_char8_t, cnc_c8nrtomwcn_with_handler, ztd_wchar_t,     \
	     cnc_mwcnrtomwcn_with_handler, ztd_char_t, cnc_mcnrtomwcn_with_handler, ZTDC_DEFAULT,     \
	     cnc_c8nrtomwcn_with_handler)

#define __cnc_detail_cxnrtomcn_with_handler(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,        \
     __p_src, __p_state, __error_handler, __p_user_data)                                          \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32nrtomcn_with_handler, ztd_char16_t, \
	     cnc_c16nrtomcn_with_handler, ztd_char8_t, cnc_c8nrtomcn_with_handler, ztd_wchar_t,      \
	     cnc_mwcnrtomcn_with_handler, ztd_char_t, cnc_mcnrtomcn_with_handler, ZTDC_DEFAULT,      \
	     cnc_c8nrtomcn_with_handler)

#define __cnc_detail_cxsntoc32sn_with_handler(                                                    \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __error_handler, __p_user_data)      \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32sntoc32sn_with_handler,             \
	     ztd_char16_t, cnc_c16sntoc32sn_with_handler, ztd_char8_t, cnc_c8sntoc32sn_with_handler, \
	     ztd_wchar_t, cnc_mwcsntoc32sn_with_handler, ztd_char_t, cnc_mcsntoc32sn_with_handler,   \
	     ZTDC_DEFAULT, cnc_c8sntoc32sn_with_handler)

#define __cnc_detail_cxsntoc16sn_with_handler(                                                    \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __error_handler, __p_user_data)      \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32sntoc16sn_with_handler,             \
	     ztd_char16_t, cnc_c16sntoc16sn_with_handler, ztd_char8_t, cnc_c8sntoc16sn_with_handler, \
	     ztd_wchar_t, cnc_mwcsntoc16sn_with_handler, ztd_char_t, cnc_mcsntoc16sn_with_handler,   \
	     ZTDC_DEFAULT, cnc_c8sntoc16sn_with_handler)

#define __cnc_detail_cxsntoc8sn_with_handler(                                                      \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __error_handler, __p_user_data)       \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32sntoc8sn_with_handler, ztd_char16_t, \
	     cnc_c16sntoc8sn_with_handler, ztd_char8_t, cnc_c8sntoc8sn_with_handler, ztd_wchar_t,     \
	     cnc_mwcsntoc8sn_with_handler, ztd_char_t, cnc_mcsntoc8sn_with_handler, ZTDC_DEFAULT,     \
	     cnc_c8sntoc8sn_with_handler)

#define __cnc_detail_cxsntomwcsn_with_handler(                                                    \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __error_handler, __p_user_data)      \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32sntomwcsn_with_handler,             \
	     ztd_char16_t, cnc_c16sntomwcsn_with_handler, ztd_char8_t, cnc_c8sntomwcsn_with_handler, \
	     ztd_wchar_t, cnc_mwcsntomwcsn_with_handler, ztd_char_t, cnc_mcsntomwcsn_with_handler,   \
	     ZTDC_DEFAULT, cnc_c8sntomwcsn_with_handler)

#define __cnc_detail_cxsntomcsn_with_handler(                                                      \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __error_handler, __p_user_data)       \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32sntomcsn_with_handler, ztd_char16_t, \
	     cnc_c16sntomcsn_with_handler, ztd_char8_t, cnc_c8sntomcsn_with_handler, ztd_wchar_t,     \
	     cnc_mwcsntomcsn_with_handler, ztd_char_t, cnc_mcsntomcsn_with_handler, ZTDC_DEFAULT,     \
	     cnc_c8sntomcsn_with_handler)

#define __cnc_detail_cxsnrtoc32sn_with_handler(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,     \
     __p_src, __p_state, __error_handler, __p_user_data)                                          \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32snrtoc32sn_with_handler,            \
	     ztd_char16_t, cnc_c16snrtoc32sn_with_handler, ztd_char8_t,                              \
	     cnc_c8snrtoc32sn_with_handler, ztd_wchar_t, cnc_mwcsnrtoc32sn_with_handler, ztd_char_t, \
	     cnc_mcsnrtoc32sn_with_handler, ZTDC_DEFAULT, cnc_c8snrtoc32sn_with_handler)

#define __cnc_detail_cxsnrtoc16sn_with_handler(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,     \
     __p_src, __p_state, __error_handler, __p_user_data)                                          \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32snrtoc16sn_with_handler,            \
	     ztd_char16_t, cnc_c16snrtoc16sn_with_handler, ztd_char8_t,                              \
	     cnc_c8snrtoc16sn_with_handler, ztd_wchar_t, cnc_mwcsnrtoc16sn_with_handler, ztd_char_t, \
	     cnc_mcsnrtoc16sn_with_handler, ZTDC_DEFAULT, cnc_c8snrtoc16sn_with_handler)

#define __cnc_detail_cxsnrtoc8sn_with_handler(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,      \
     __p_src, __p_state, __error_handler, __p_user_data)                                          \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32snrtoc8sn_with_handler,             \
	     ztd_char16_t, cnc_c16snrtoc8sn_with_handler, ztd_char8_t, cnc_c8snrtoc8sn_with_handler, \
	     ztd_wchar_t, cnc_mwcsnrtoc8sn_with_handler, ztd_char_t, cnc_mcsnrtoc8sn_with_handler,   \
	     ZTDC_DEFAULT, cnc_c8snrtoc8sn_with_handler)

#define __cnc_detail_cxsnrtomwcsn_with_handler(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,     \
     __p_src, __p_state, __error_handler, __p_user_data)                                          \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32snrtomwcsn_with_handler,            \
	     ztd_char16_t, cnc_c16snrtomwcsn_with_handler, ztd_char8_t,                              \
	     cnc_c8snrtomwcsn_with_handler, ztd_wchar_t, cnc_mwcsnrtomwcsn_with_handler, ztd_char_t, \
	     cnc_mcsnrtomwcsn_with_handler, ZTDC_DEFAULT, cnc_c8snrtomwcsn_with_handler)

#define __cnc_detail_cxsnrtomcsn_with_handler(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,      \
     __p_src, __p_state, __error_handler, __p_user_data)                                          \
	ZTDC_CASCADING_GENERIC(**(__p_src), ztd_char32_t, cnc_c32snrtomcsn_with_handler,             \
	     ztd_char16_t, cnc_c16snrtomcsn_with_handler, ztd_char8_t, cnc_c8snrtomcsn_with_handler, \
	     ztd_wchar_t, cnc_mwcsnrtomcsn_with_handler, ztd_char_t, cnc_mcsnrtomcsn_with_handler,   \
	     ZTDC_DEFAULT, cnc_c8snrtomcsn_with_handler)

#define __cnc_detail_cxntocxn_with_handler(                                                        \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __error_handler, __p_user_data)       \
	ZTDC_CASCADING_GENERIC(**(__p_maybe_dst), ztd_char32_t,                                       \
	     __cnc_detail_cxntoc32n_with_handler((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), \
	          (__p_src), (__error_handler), (__p_user_data)),                                     \
	     ztd_char16_t,                                                                            \
	     __cnc_detail_cxntoc16n_with_handler((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), \
	          (__p_src), (__error_handler), (__p_user_data)),                                     \
	     ztd_char8_t,                                                                             \
	     __cnc_detail_cxntoc8n_with_handler((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len),  \
	          (__p_src), (__error_handler), (__p_user_data)),                                     \
	     ztd_wchar_t,                                                                             \
	     __cnc_detail_cxntomwcn_with_handler((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), \
	          (__p_src), (__error_handler), (__p_user_data)),                                     \
	     ztd_char_t,                                                                              \
	     __cnc_detail_cxntomcn_with_handler((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len),  \
	          (__p_src), (__error_handler), (__p_user_data)),                                     \
	     ZTDC_DEFAULT,                                                                            \
	     __cnc_detail_cxntoc8n_with_handler((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len),  \
	          (__p_src), (__error_handler), (__p_user_data)))

#define __cnc_detail_cxnrtocxn_with_handler(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,         \
     __p_src, __p_state, __error_handler, __p_user_data)                                           \
	ZTDC_CASCADING_GENERIC(**(__p_maybe_dst), ztd_char32_t,                                       \
	     __cnc_detail_cxnrtoc32n_with_handler((__p_maybe_dst_len), (__p_maybe_dst),               \
	          (__p_src_len), (__p_src), (__p_state), (__error_handler), (__p_user_data)),         \
	     ztd_char16_t,                                                                            \
	     __cnc_detail_cxnrtoc16n_with_handler((__p_maybe_dst_len), (__p_maybe_dst),               \
	          (__p_src_len), (__p_src), (__p_state), (__error_handler), (__p_user_data)),         \
	     ztd_char8_t,                                                                             \
	     __cnc_detail_cxnrtoc8n_with_handler((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), \
	          (__p_src), (__p_state), (__error_handler), (__p_user_data)),                        \
	     ztd_wchar_t,                                                                             \
	     __cnc_detail_cxnrtomwcn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),               \
	          (__p_src_len), (__p_src), (__p_state), (__error_handler), (__p_user_data)),         \
	     ztd_char_t,                                                                              \
	     __cnc_detail_cxnrtomcn_with_handler((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), \
	          (__p_src), (__p_state), (__error_handler), (__p_user_data)),                        \
	     ZTDC_DEFAULT,                                                                            \
	     __cnc_detail_cxnrtoc8n_with_handler((__p_maybe_dst_len), (__p_maybe_dst), (__p_src_len), \
	          (__p_src), (__p_state), (__error_handler), (__p_user_data)))

#define __cnc_detail_cxsntocxsn_with_handler(                                                \
     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __error_handler, __p_user_data) \
	ZTDC_CASCADING_GENERIC(**(__p_maybe_dst), ztd_char32_t,                                 \
	     __cnc_detail_cxsntoc32sn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),        \
	          (__p_src_len), (__p_src), (__error_handler), (__p_user_data)),                \
	     ztd_char16_t,                                                                      \
	     __cnc_detail_cxsntoc16sn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),        \
	          (__p_src_len), (__p_src), (__error_handler), (__p_user_data)),                \
	     ztd_char8_t,                                                                       \
	     __cnc_detail_cxsntoc8sn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),         \
	          (__p_src_len), (__p_src), (__error_handler), (__p_user_data)),                \
	     ztd_wchar_t,                                                                       \
	     __cnc_detail_cxsntomwcsn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),        \
	          (__p_src_len), (__p_src), (__error_handler), (__p_user_data)),                \
	     ztd_char_t,                                                                        \
	     __cnc_detail_cxsntomcsn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),         \
	          (__p_src_len), (__p_src), (__error_handler), (__p_user_data)),                \
	     ZTDC_DEFAULT,                                                                      \
	     __cnc_detail_cxsntoc8sn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),         \
	          (__p_src_len), (__p_src), (__error_handler), (__p_user_data)))

#define __cnc_detail_cxsnrtocxsn_with_handler(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, \
     __p_src, __p_state, __error_handler, __p_user_data)                                     \
	ZTDC_CASCADING_GENERIC(**(__p_maybe_dst), ztd_char32_t,                                 \
	     (__cnc_detail_cxsnrtoc32sn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),      \
	          (__p_src_len), (__p_src), (__p_state), (__error_handler), (__p_user_data))),  \
	     ztd_char16_t,                                                                      \
	     (__cnc_detail_cxsnrtoc16sn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),      \
	          (__p_src_len), (__p_src), (__p_state), (__error_handler), (__p_user_data))),  \
	     ztd_char8_t,                                                                       \
	     (__cnc_detail_cxsnrtoc8sn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),       \
	          (__p_src_len), (__p_src), (__p_state), (__error_handler), (__p_user_data))),  \
	     ztd_wchar_t,                                                                       \
	     (__cnc_detail_cxsnrtomwcsn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),      \
	          (__p_src_len), (__p_src), (__p_state), (__error_handler), (__p_user_data))),  \
	     ztd_char_t,                                                                        \
	     (__cnc_detail_cxsnrtomcsn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),       \
	          (__p_src_len), (__p_src), (__p_state), (__error_handler), (__p_user_data))),  \
	     ZTDC_DEFAULT,                                                                      \
	     (__cnc_detail_cxsnrtoc8sn_with_handler((__p_maybe_dst_len), (__p_maybe_dst),       \
	          (__p_src_len), (__p_src), (__p_state), (__error_handler), (__p_user_data))))

#endif
