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

#ifndef ZTD_CUNEICODE_SOURCE_DETAIL_TRANSCODE_HPP
#define ZTD_CUNEICODE_SOURCE_DETAIL_TRANSCODE_HPP

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/max_output.h>
#include <ztd/cuneicode/mcstate.h>
#include <ztd/cuneicode/detail/boilerplate.h>

#include <ztd/idk/assert.hpp>

#include <optional>
#include <cstddef>

namespace cnc {
	ZTD_CUNEICODE_INLINE_ABI_NAMESPACE_OPEN_I_

	namespace __cnc_detail {

		template <bool _IsCounting, bool _IsUnbounded, typename _FromFunc, _FromFunc __xsnrtoisn,
		     typename _ToFunc, _ToFunc __isnrtozsn, typename _IntermediateChar = ztd_char32_t,
		     size_t _IntermediateMax = CNC_C32_MAX, typename _SourceChar, typename _DestChar,
		     typename _State>
		cnc_mcerr __basic_transcode_one(size_t* __p_maybe_dst_len, _DestChar** __p_maybe_dst,
		     size_t* __p_src_len, const _SourceChar** __p_src, _State __p_state) noexcept {
			if (__p_src_len == nullptr || *__p_src_len < 1) {
				// empty sources are just fine (returns 0)
				return cnc_mcerr_ok;
			}
			if (__p_src == nullptr || *__p_src == nullptr) {
				// null sources are treated like empty sources: okay
				return cnc_mcerr_ok;
			}

			const _SourceChar*& __src             = *__p_src;
			size_t& __src_len                     = *__p_src_len;
			const _SourceChar* __intermediate_src = __src;
			size_t __intermediate_src_len         = __src_len;

			_IntermediateChar __intermediate[_IntermediateMax];
			_IntermediateChar* __intermediate_first = __intermediate;
			size_t __intermediate_out_size_after    = _IntermediateMax;
			cnc_mcerr __res_decode                  = __xsnrtoisn(&__intermediate_out_size_after,
			                      &__intermediate_first, &__intermediate_src_len, &__intermediate_src, __p_state);
			switch (__res_decode) {
			case cnc_mcerr_invalid_sequence:
				// error, explode
				return __res_decode;
			case cnc_mcerr_incomplete_input:
				// out of input
				// no need to change __src or __src_len
				return __res_decode;
			case cnc_mcerr_insufficient_output:
				// this error should never happen, ever!
				return __res_decode;
			case cnc_mcerr_ok:
			default:
				// we can keep going!
				break;
			}

			const size_t __intermediate_out_size
			     = _IntermediateMax - __intermediate_out_size_after;
			const _IntermediateChar* __intermediate_input_first = __intermediate;
			size_t __intermediate_input_size_after              = __intermediate_out_size;

			cnc_mcerr __res_encode = __isnrtozsn(__p_maybe_dst_len, __p_maybe_dst,
			     &__intermediate_input_size_after, &__intermediate_input_first, __p_state);
			switch (__res_encode) {
			case cnc_mcerr_invalid_sequence:
				// error, rollback
				return __res_encode;
			case cnc_mcerr_incomplete_input:
				// out of input
				// no need to increment __src or __src_len
				return __res_encode;
			case cnc_mcerr_insufficient_output:
				// this error should never happen, ever!
				return __res_encode;
			case cnc_mcerr_ok:
			default:
				// all good, update original pointers!
				__src     = __intermediate_src;
				__src_len = __intermediate_src_len;
				break;
			}
			return cnc_mcerr_ok;
		}

		template <bool _IsCounting, bool _IsUnbounded, size_t _IntermediateMax, typename _Func,
		     _Func __xsnrtoysn, typename _SourceChar, typename _DestChar,
		     typename _CompletionFunc          = decltype(&cnc_mcstate_is_complete),
		     _CompletionFunc __completion_func = &cnc_mcstate_is_complete, typename _State>
		cnc_mcerr __transcode(size_t* __p_maybe_dst_len, _DestChar** __p_maybe_dst,
		     size_t* __p_src_len, const _SourceChar** __p_src, _State __p_state) noexcept {
			if (__p_src_len == nullptr) {
				return __xsnrtoysn(
				     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
			}
			for (;;) {
				cnc_mcerr __res = __xsnrtoysn(
				     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
				switch (__res) {
				case cnc_mcerr_ok:
					if (*__p_src_len == 0) {
						if (!__completion_func(__p_state)) {
							break;
						}
						return cnc_mcerr_ok;
					}
					break;
				default:
					// error, explode out
					return __res;
				}
			}
		}

#define _ZTDC_CUNEICODE_TRANSCODE_ONE_BODY(_DL, _D, _SL, _S, _ST, ...)                       \
	if (_D == nullptr || *_D == nullptr) {                                                  \
		if (_DL == nullptr) {                                                              \
			return ::cnc::__cnc_detail::__basic_transcode_one<true, true, __VA_ARGS__>(   \
			     _DL, _D, _SL, _S, _ST);                                                  \
		}                                                                                  \
		else {                                                                             \
			return ::cnc::__cnc_detail::__basic_transcode_one<true, false, __VA_ARGS__>(  \
			     _DL, _D, _SL, _S, _ST);                                                  \
		}                                                                                  \
	}                                                                                       \
	else {                                                                                  \
		if (_DL == nullptr) {                                                              \
			return ::cnc::__cnc_detail::__basic_transcode_one<false, true, __VA_ARGS__>(  \
			     _DL, _D, _SL, _S, _ST);                                                  \
		}                                                                                  \
		else {                                                                             \
			return ::cnc::__cnc_detail::__basic_transcode_one<false, false, __VA_ARGS__>( \
			     _DL, _D, _SL, _S, _ST);                                                  \
		}                                                                                  \
	}

#define _ZTDC_CUNEICODE_TRANSCODE_BODY(_DL, _D, _SL, _S, _ST, ...)                 \
	if (_D == nullptr || *_D == nullptr) {                                        \
		if (_DL == nullptr) {                                                    \
			return ::cnc::__cnc_detail::__transcode<true, true, __VA_ARGS__>(   \
			     _DL, _D, _SL, _S, _ST);                                        \
		}                                                                        \
		else {                                                                   \
			return ::cnc::__cnc_detail::__transcode<true, false, __VA_ARGS__>(  \
			     _DL, _D, _SL, _S, _ST);                                        \
		}                                                                        \
	}                                                                             \
	else {                                                                        \
		if (_DL == nullptr) {                                                    \
			return ::cnc::__cnc_detail::__transcode<false, true, __VA_ARGS__>(  \
			     _DL, _D, _SL, _S, _ST);                                        \
		}                                                                        \
		else {                                                                   \
			return ::cnc::__cnc_detail::__transcode<false, false, __VA_ARGS__>( \
			     _DL, _D, _SL, _S, _ST);                                        \
		}                                                                        \
	}

#define _ZTDC_CUNEICODE_SINGLE_ASCII_BYTE_HIGH_BIT_ENCODING_CODE_POINT_TO_BYTE(        \
     _SRC_TYPE, _DEST_TYPE, _BYTE_LOOKUP_FUNC)                                         \
	_CNC_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS(_SRC_TYPE);                             \
                                                                                       \
	const _SRC_TYPE __code_point      = (*__src);                                     \
	const ztd_char32_t __code_point32 = (ztd_char32_t)__code_point;                   \
                                                                                       \
	if (__code_point32 <= static_cast<_SRC_TYPE>(0x7F)) {                             \
		/* ASCII character*/                                                         \
		if (!_IsUnbounded) {                                                         \
			if (__p_maybe_dst_len[0] < 1) {                                         \
				return cnc_mcerr_insufficient_output;                              \
			}                                                                       \
			__p_maybe_dst_len[0] -= 1;                                              \
		}                                                                            \
		if (!_IsCounting) {                                                          \
			__p_maybe_dst[0][0] = (_DEST_TYPE)*__src;                               \
			__p_maybe_dst[0] += 1;                                                  \
		}                                                                            \
		__p_src[0] += 1;                                                             \
		__p_src_len[0] -= 1;                                                         \
		return cnc_mcerr_ok;                                                         \
	}                                                                                 \
                                                                                       \
	::std::optional<::std::size_t> __maybe_index = _BYTE_LOOKUP_FUNC(__code_point32); \
	if (__maybe_index) {                                                              \
		const ::std::size_t __index      = *__maybe_index;                           \
		const unsigned char __code_unit0 = (unsigned char)(__index + 0x80);          \
		if (!_IsUnbounded) {                                                         \
			if (__p_maybe_dst_len[0] < 1) {                                         \
				return cnc_mcerr_insufficient_output;                              \
			}                                                                       \
			__p_maybe_dst_len[0] -= 1;                                              \
		}                                                                            \
		if (!_IsCounting) {                                                          \
			__p_maybe_dst[0][0] = (_DEST_TYPE)__code_unit0;                         \
			__p_maybe_dst[0] += 1;                                                  \
		}                                                                            \
		__p_src[0] += 1;                                                             \
		__p_src_len[0] -= 1;                                                         \
		return cnc_mcerr_ok;                                                         \
	}                                                                                 \
                                                                                       \
	return cnc_mcerr_invalid_sequence;

#define _ZTDC_CUNEICODE_SINGLE_ASCII_BYTE_HIGH_BIT_ENCODING_BYTE_TO_CODE_POINT(      \
     _SRC_TYPE, _DEST_TYPE, _CODE_POINT_LOOKUP_FUNC)                                 \
	_CNC_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS(_SRC_TYPE);                           \
                                                                                     \
	const unsigned char __code_unit0 = ((unsigned char)*__src);                     \
                                                                                     \
	if (__code_unit0 <= 0x7F) {                                                     \
		/* ASCII character */                                                      \
		if (!_IsUnbounded) {                                                       \
			if (__p_maybe_dst_len[0] < 1) {                                       \
				return cnc_mcerr_insufficient_output;                            \
			}                                                                     \
			__p_maybe_dst_len[0] -= 1;                                            \
		}                                                                          \
		if (!_IsCounting) {                                                        \
			__p_maybe_dst[0][0] = (_DEST_TYPE)*__src;                             \
			__p_maybe_dst[0] += 1;                                                \
		}                                                                          \
		__p_src[0] += 1;                                                           \
		__p_src_len[0] -= 1;                                                       \
		return cnc_mcerr_ok;                                                       \
	}                                                                               \
                                                                                     \
	::std::size_t __lookup_index = static_cast<::std::size_t>(__code_unit0 - 0x80); \
	::std::optional<::std::uint_least32_t> __maybe_code_point                       \
	     = _CODE_POINT_LOOKUP_FUNC(__lookup_index);                                 \
	if (__maybe_code_point) {                                                       \
		const ztd_char32_t __code_point = (ztd_char32_t)(*__maybe_code_point);     \
		if (!_IsUnbounded) {                                                       \
			if (__p_maybe_dst_len[0] < 1) {                                       \
				return cnc_mcerr_insufficient_output;                            \
			}                                                                     \
			__p_maybe_dst_len[0] -= 1;                                            \
		}                                                                          \
		if (!_IsCounting) {                                                        \
			__p_maybe_dst[0][0] = (_DEST_TYPE)__code_point;                       \
			__p_maybe_dst[0] += 1;                                                \
		}                                                                          \
		__p_src[0] += 1;                                                           \
		__p_src_len[0] -= 1;                                                       \
		return cnc_mcerr_ok;                                                       \
	}                                                                               \
                                                                                     \
	return cnc_mcerr_invalid_sequence;

#define _ZTDC_CUNEICODE_SINGLE_BYTE_ENCODING_CODE_POINT_TO_BYTE(                       \
     _SRC_TYPE, _DEST_TYPE, _BYTE_LOOKUP_FUNC)                                         \
	_CNC_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS(_SRC_TYPE);                             \
                                                                                       \
	const _SRC_TYPE __code_point      = (*__src);                                     \
	const ztd_char32_t __code_point32 = (ztd_char32_t)__code_point;                   \
                                                                                       \
	::std::optional<::std::size_t> __maybe_index = _BYTE_LOOKUP_FUNC(__code_point32); \
	if (__maybe_index) {                                                              \
		const ::std::size_t __index      = *__maybe_index;                           \
		const unsigned char __code_unit0 = (unsigned char)(__index);                 \
		if (!_IsUnbounded) {                                                         \
			if (__p_maybe_dst_len[0] < 1) {                                         \
				return cnc_mcerr_insufficient_output;                              \
			}                                                                       \
			__p_maybe_dst_len[0] -= 1;                                              \
		}                                                                            \
		if (!_IsCounting) {                                                          \
			__p_maybe_dst[0][0] = (_DEST_TYPE)__code_unit0;                         \
			__p_maybe_dst[0] += 1;                                                  \
		}                                                                            \
		__p_src[0] += 1;                                                             \
		__p_src_len[0] -= 1;                                                         \
		return cnc_mcerr_ok;                                                         \
	}                                                                                 \
                                                                                       \
	return cnc_mcerr_invalid_sequence;

#define _ZTDC_CUNEICODE_SINGLE_BYTE_ENCODING_BYTE_TO_CODE_POINT(                 \
     _SRC_TYPE, _DEST_TYPE, _CODE_POINT_LOOKUP_FUNC)                             \
	_CNC_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS(_SRC_TYPE);                       \
                                                                                 \
	const unsigned char __code_unit0 = ((unsigned char)*__src);                 \
                                                                                 \
	::std::size_t __lookup_index = static_cast<::std::size_t>(__code_unit0);    \
	::std::optional<::std::uint_least32_t> __maybe_code_point                   \
	     = _CODE_POINT_LOOKUP_FUNC(__lookup_index);                             \
	if (__maybe_code_point) {                                                   \
		const ztd_char32_t __code_point = (ztd_char32_t)(*__maybe_code_point); \
		if (!_IsUnbounded) {                                                   \
			if (__p_maybe_dst_len[0] < 1) {                                   \
				return cnc_mcerr_insufficient_output;                        \
			}                                                                 \
			__p_maybe_dst_len[0] -= 1;                                        \
		}                                                                      \
		if (!_IsCounting) {                                                    \
			__p_maybe_dst[0][0] = (_DEST_TYPE)__code_point;                   \
			__p_maybe_dst[0] += 1;                                            \
		}                                                                      \
		__p_src[0] += 1;                                                       \
		__p_src_len[0] -= 1;                                                   \
		return cnc_mcerr_ok;                                                   \
	}                                                                           \
                                                                                 \
	return cnc_mcerr_invalid_sequence;

	} // namespace __cnc_detail
	ZTD_CUNEICODE_INLINE_ABI_NAMESPACE_CLOSE_I_
} // namespace cnc

#endif
