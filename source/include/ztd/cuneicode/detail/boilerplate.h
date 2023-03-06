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

#ifndef ZTD_CUNEICODE_SOURCE_DETAIL_BOILERPLATE_H
#define ZTD_CUNEICODE_SOURCE_DETAIL_BOILERPLATE_H

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/detail/decl_boilerplate.h>

#include <ztd/idk/assert.h>
#include <ztd/idk/static_assert.h>
#include <ztd/idk/detail/unicode.h>

#if ZTD_IS_OFF(ZTD_C)
#include <cstddef>
#else
#include <stddef.h>
#include <stdbool.h>
#endif

#define _CNC_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS_WITH(_DEFAULT_ERR, _P_DEST, _P_DEST_SIZE, \
     _SRC_TYPE, _P_SRC_SIZE, _P_SRC, _SRC_NAME, _SRC_SIZE_NAME, _IS_COUNTING_NAME,          \
     _IS_UNBOUNDED_NAME)                                                                    \
	if ((_P_SRC) == NULL || *(_P_SRC) == NULL) {                                           \
		return _DEFAULT_ERR;                                                              \
	}                                                                                      \
	ZTD_ASSERT((_P_SRC_SIZE) != NULL);                                                     \
	const _SRC_TYPE* const _SRC_NAME = *(_P_SRC);                                          \
	const size_t _SRC_SIZE_NAME      = *(_P_SRC_SIZE);                                     \
	if (__src_len < 1) {                                                                   \
		return _DEFAULT_ERR;                                                              \
	}                                                                                      \
	const bool _IS_COUNTING_NAME  = (_P_DEST) == NULL || (_P_DEST)[0] == NULL;             \
	const bool _IS_UNBOUNDED_NAME = (_P_DEST_SIZE) == NULL

#define _CNC_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS_WITH_RET(_DEFAULT_ERR, _SRC_TYPE)          \
	_CNC_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS_WITH(_DEFAULT_ERR, __p_maybe_dst,             \
	     __p_maybe_dst_len, _SRC_TYPE, __p_src_len, __p_src, __src, __src_len, _IsCounting, \
	     _IsUnbounded)

#define _CNC_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS(_SRC_TYPE)                                 \
	_CNC_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS_WITH(cnc_mcerr_ok, __p_maybe_dst,             \
	     __p_maybe_dst_len, _SRC_TYPE, __p_src_len, __p_src, __src, __src_len, _IsCounting, \
	     _IsUnbounded)

#define _CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__CURRENT_ERROR, _P_SRC_SIZE, _P_SRC) \
	if ((_P_SRC_SIZE) == NULL || (_P_SRC) == NULL || (_P_SRC[0]) == NULL) {    \
		return (__CURRENT_ERROR);                                             \
	}                                                                          \
	ztdc_static_assert(1, "")

#define _CNC_ERROR_INITIAL_DEST_CHECK_IMPL(                                     \
     _P_DEST_SIZE, _P_DEST, _IS_COUNTING_NAME, _IS_UNBOUNDED_NAME)              \
	const bool _IS_COUNTING_NAME  = (_P_DEST) == NULL || (_P_DEST)[0] == NULL; \
	const bool _IS_UNBOUNDED_NAME = (_P_DEST_SIZE) == NULL

#define _CNC_REPLACE_ERROR_IMPL_C8(                                                  \
     _CURRENT_ERROR, _DEST_TYPE, _P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED) \
	if (!_IS_UNBOUNDED) {                                                           \
		if ((_P_DEST_SIZE)[0] < 3) {                                               \
			return (_CURRENT_ERROR);                                              \
		}                                                                          \
		(_P_DEST_SIZE)[0] -= 3;                                                    \
	}                                                                               \
	if (!_IS_COUNTING) {                                                            \
		(_P_DEST)[0][0] = (_DEST_TYPE)'\xEF';                                      \
		(_P_DEST)[0][1] = (_DEST_TYPE)'\xBF';                                      \
		(_P_DEST)[0][2] = (_DEST_TYPE)'\xBD';                                      \
		(_P_DEST)[0] += 3;                                                         \
	}

#define _CNC_REPLACE_ERROR_IMPL_C16(                                                 \
     _CURRENT_ERROR, _DEST_TYPE, _P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED) \
	if (!_IS_UNBOUNDED) {                                                           \
		if ((_P_DEST_SIZE)[0] < 1) {                                               \
			return (_CURRENT_ERROR);                                              \
		}                                                                          \
		(_P_DEST_SIZE)[0] -= 1;                                                    \
	}                                                                               \
	if (!_IS_COUNTING) {                                                            \
		(_P_DEST)[0][0] = (_DEST_TYPE)0xFFDD;                                      \
		(_P_DEST)[0] += 1;                                                         \
	}

#define _CNC_REPLACE_ERROR_IMPL_C32(                                                 \
     _CURRENT_ERROR, _DEST_TYPE, _P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED) \
	if (!_IS_UNBOUNDED) {                                                           \
		if ((_P_DEST_SIZE)[0] < 1) {                                               \
			return (_CURRENT_ERROR);                                              \
		}                                                                          \
		(_P_DEST_SIZE)[0] -= 1;                                                    \
	}                                                                               \
	if (!_IS_COUNTING) {                                                            \
		(_P_DEST)[0][0] = (_DEST_TYPE)0x0000FFDD;                                  \
		(_P_DEST)[0] += 1;                                                         \
	}

#define _CNC_REPLACE_ERROR_IMPL_ASCII(                                               \
     _CURRENT_ERROR, _DEST_TYPE, _P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED) \
	if (!_IS_UNBOUNDED) {                                                           \
		if ((_P_DEST_SIZE)[0] < 1) {                                               \
			return (_CURRENT_ERROR);                                              \
		}                                                                          \
		(_P_DEST_SIZE)[0] -= 1;                                                    \
	}                                                                               \
	if (!_IS_COUNTING) {                                                            \
		/* ? in ASCII */                                                           \
		(_P_DEST)[0][0] = (_DEST_TYPE)0x3F;                                        \
		(_P_DEST)[0] += 1;                                                         \
	}


#define _CNC_REPLACE_ERROR_C8(                                                               \
     _CURRENT_ERROR, _DEST_TYPE, _P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED)         \
	_CNC_ERROR_INITIAL_DEST_CHECK_IMPL(_P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED); \
	_CNC_REPLACE_ERROR_IMPL_C8(                                                             \
	     _CURRENT_ERROR, _DEST_TYPE, _P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED);   \
	return cnc_mcerr_ok

#define _CNC_REPLACE_ERROR_C16(                                                              \
     _CURRENT_ERROR, _DEST_TYPE, _P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED)         \
	_CNC_ERROR_INITIAL_DEST_CHECK_IMPL(_P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED); \
	_CNC_REPLACE_ERROR_IMPL_C16(                                                            \
	     _CURRENT_ERROR, _DEST_TYPE, _P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED);   \
	return cnc_mcerr_ok

#define _CNC_REPLACE_ERROR_C32(                                                              \
     _CURRENT_ERROR, _DEST_TYPE, _P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED)         \
	_CNC_ERROR_INITIAL_DEST_CHECK_IMPL(_P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED); \
	_CNC_REPLACE_ERROR_IMPL_C32(                                                            \
	     _CURRENT_ERROR, _DEST_TYPE, _P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED)    \
	return cnc_mcerr_ok

#define _CNC_REPLACE_ERROR_ASCII(                                                            \
     _CURRENT_ERROR, _DEST_TYPE, _P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED)         \
	_CNC_ERROR_INITIAL_DEST_CHECK_IMPL(_P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED); \
	_CNC_REPLACE_ERROR_IMPL_ASCII(                                                          \
	     _CURRENT_ERROR, _DEST_TYPE, _P_DEST_SIZE, _P_DEST, _IS_COUNTING, _IS_UNBOUNDED);   \
	return cnc_mcerr_ok


#define _CNC_SKIP_INPUT_ERROR_IMPL_NONE(_P_SRC_SIZE, _P_SRC) \
	if ((_P_SRC_SIZE)[0] > 1) {                             \
		(_P_SRC_SIZE)[0] -= 1;                             \
		(_P_SRC)[0] += 1;                                  \
	}                                                       \
	ztdc_static_assert(1, "")

#define _CNC_SKIP_INPUT_ERROR_IMPL_C8(_P_SRC_SIZE, _P_SRC)       \
	_CNC_SKIP_INPUT_ERROR_IMPL_NONE(_P_SRC_SIZE, _P_SRC);       \
	while ((_P_SRC_SIZE)[0] > 1) {                              \
		ztd_char8_t __val8 = (ztd_char8_t)((_P_SRC)[0][0]);    \
		if (__ztd_idk_detail_is_single_or_lead_utf8(__val8)) { \
			break;                                            \
		}                                                      \
		(_P_SRC_SIZE)[0] -= 1;                                 \
		(_P_SRC)[0] += 1;                                      \
	}                                                           \
	ztdc_static_assert(1, "")

#define _CNC_SKIP_INPUT_ERROR_IMPL_C16(_P_SRC_SIZE, _P_SRC)        \
	_CNC_SKIP_INPUT_ERROR_IMPL_NONE(_P_SRC_SIZE, _P_SRC);         \
	while ((_P_SRC_SIZE)[0] > 1) {                                \
		ztd_char16_t __val16 = (ztd_char16_t)((_P_SRC)[0][0]);   \
		if (__ztd_idk_detail_is_single_or_lead_utf16(__val16)) { \
			break;                                              \
		}                                                        \
		(_P_SRC_SIZE)[0] -= 1;                                   \
		(_P_SRC)[0] += 1;                                        \
	}                                                             \
	ztdc_static_assert(1, "")

#define _CNC_SKIP_INPUT_ERROR_IMPL_C32(_P_SRC_SIZE, _P_SRC)       \
	_CNC_SKIP_INPUT_ERROR_IMPL_NONE(_P_SRC_SIZE, _P_SRC);        \
	while ((_P_SRC_SIZE)[0] > 1) {                               \
		ztd_char32_t __val32 = (ztd_char32_t)((_P_SRC)[0][0]);  \
		if (__val32 <= __ztd_idk_detail_last_unicode_code_point \
		     && !__ztd_idk_detail_is_surrogate(__val32)) {      \
			break;                                             \
		}                                                       \
		(_P_SRC_SIZE)[0] -= 1;                                  \
		(_P_SRC)[0] += 1;                                       \
	}                                                            \
	ztdc_static_assert(1, "")

#endif
