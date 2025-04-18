// ============================================================================
//
// ztd.cuneicode
// Copyright © JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
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

#ifndef ZTD_CUNEICODE_SOURCE_DETAIL_DEFN_BOILERPLATE_H
#define ZTD_CUNEICODE_SOURCE_DETAIL_DEFN_BOILERPLATE_H

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/detail/decl_boilerplate.h>

#include <ztd/idk/static_assert.h>
#include <ztd/idk/charN_t.h>
#include <ztd/idk/generic.h>

#include <ztd/idk/assert.h>

#if ZTD_IS_ON(ZTD_CXX)
#include <cstddef>
#else
#include <stddef.h>
#include <stdbool.h>
#endif

#define CNC_TYPED_CONVERSION_ERROR_DEFN_ROOT_I_(_SHIM_NAME, _SHIM_R_NAME, _SHIM_FROM, _SHIM_TO,   \
     _INTERNAL_NAME, _INTERNAL_R_NAME, _INTERNAL_FROM, _INTERNAL_TO, _STATE_TYPE,                 \
     _STATE_COMPLETION_FUNCTION)                                                                  \
	ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)                                                              \
	ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)                                                           \
	cnc_error_result_t _SHIM_R_NAME(size_t* restrict __p_maybe_dst_len,                          \
	     _SHIM_TO* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,               \
	     const _SHIM_FROM* restrict* restrict __p_src, _STATE_TYPE* restrict __p_state,          \
	     cnc_mcerr (*__error_handler)(cnc_mcerr, size_t* restrict, _SHIM_TO* restrict* restrict, \
	          size_t* restrict, const _SHIM_FROM* restrict* restrict, void* restrict,            \
	          void* restrict),                                                                   \
	     void* restrict __p_user_data) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {                            \
		cnc_error_result_t __result = {};                                                       \
		if (__p_src == NULL || __p_src[0] == NULL || __p_src_len == NULL) {                     \
			__result.error_code                                                                \
			     = (_INTERNAL_R_NAME)(__p_maybe_dst_len, (_INTERNAL_TO**)__p_maybe_dst,        \
			          __p_src_len, (const _INTERNAL_FROM**)__p_src, __p_state);                \
			return __result;                                                                   \
		}                                                                                       \
		for (;;) {                                                                              \
			cnc_mcerr __err                                                                    \
			     = (_INTERNAL_R_NAME)(__p_maybe_dst_len, (_INTERNAL_TO**)__p_maybe_dst,        \
			          __p_src_len, (const _INTERNAL_FROM**)__p_src, __p_state);                \
			if (__err != cnc_mcerr_ok) {                                                       \
				__result.error_count += 1;                                                    \
				__err = __error_handler(__err, __p_maybe_dst_len, __p_maybe_dst, __p_src_len, \
				     __p_src, __p_state, __p_user_data);                                      \
			}                                                                                  \
			if (__err != cnc_mcerr_ok) {                                                       \
				__result.error_code = __err;                                                  \
				break;                                                                        \
			}                                                                                  \
			if (__p_src_len[0] == 0) {                                                         \
				if ((_STATE_COMPLETION_FUNCTION)(__p_state)) {                                \
					break;                                                                   \
				}                                                                             \
				continue;                                                                     \
			}                                                                                  \
		}                                                                                       \
		return __result;                                                                        \
	}                                                                                            \
	ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)                                                              \
	ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)                                                           \
	cnc_error_result_t _SHIM_NAME(size_t* restrict __p_maybe_dst_len,                            \
	     _SHIM_TO* restrict* restrict __p_maybe_dst, size_t* restrict __p_src_len,               \
	     const _SHIM_FROM* restrict* restrict __p_src,                                           \
	     cnc_mcerr (*__error_handler)(cnc_mcerr, size_t* restrict, _SHIM_TO* restrict* restrict, \
	          size_t* restrict, const _SHIM_FROM* restrict* restrict, void* restrict,            \
	          void* restrict),                                                                   \
	     void* restrict __p_user_data) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {                            \
		_STATE_TYPE __state = {};                                                               \
		return _SHIM_R_NAME(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, &__state,   \
		     __error_handler, __p_user_data);                                                   \
	}                                                                                            \
	static_assert(1, "")


#define CNC_TYPED_CONVERSION_ERROR_DEFN_I_(_SHIM_PREFIX, _SHIM_SUFFIX, _INTERNAL_PREFIX,         \
     _INTERNAL_SUFFIX, _TAIL, _STATE_TYPE, _STATE_COMPLETION_FUNCTION)                           \
	CNC_TYPED_CONVERSION_ERROR_DEFN_ROOT_I_(                                                    \
	     cnc_##_SHIM_PREFIX##nto##_SHIM_SUFFIX##n##_TAIL##_with_handler,                        \
	     cnc_##_SHIM_PREFIX##nrto##_SHIM_SUFFIX##n##_TAIL##_with_handler,                       \
	     TAG_TO_TYPE_I_(_SHIM_PREFIX), TAG_TO_TYPE_I_(_SHIM_SUFFIX),                            \
	     cnc_##_INTERNAL_PREFIX##nto##_INTERNAL_SUFFIX##n,                                      \
	     cnc_##_INTERNAL_PREFIX##nrto##_INTERNAL_SUFFIX##n, TAG_TO_TYPE_I_(_INTERNAL_PREFIX),   \
	     TAG_TO_TYPE_I_(_INTERNAL_SUFFIX), _STATE_TYPE, _STATE_COMPLETION_FUNCTION);            \
	CNC_TYPED_CONVERSION_ERROR_DEFN_ROOT_I_(                                                    \
	     cnc_##_SHIM_PREFIX##snto##_SHIM_SUFFIX##sn##_TAIL##_with_handler,                      \
	     cnc_##_SHIM_PREFIX##snrto##_SHIM_SUFFIX##sn##_TAIL##_with_handler,                     \
	     TAG_TO_TYPE_I_(_SHIM_PREFIX), TAG_TO_TYPE_I_(_SHIM_SUFFIX),                            \
	     cnc_##_INTERNAL_PREFIX##snto##_INTERNAL_SUFFIX##sn,                                    \
	     cnc_##_INTERNAL_PREFIX##snrto##_INTERNAL_SUFFIX##sn, TAG_TO_TYPE_I_(_INTERNAL_PREFIX), \
	     TAG_TO_TYPE_I_(_INTERNAL_SUFFIX), _STATE_TYPE, _STATE_COMPLETION_FUNCTION)

#define CNC_TYPED_CONVERSION_DEFN_ROOT_I_(_SHIM_NAME, _SHIM_R_NAME, _SHIM_FROM, _SHIM_TO,          \
     _INTERNAL_NAME, _INTERNAL_R_NAME, _INTERNAL_FROM, _INTERNAL_TO)                               \
	ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)                                                               \
	ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)                                                            \
	cnc_mcerr _SHIM_NAME(size_t* __p_maybe_dst_len, _SHIM_TO** __p_maybe_dst,                     \
	     size_t* __p_src_len, const _SHIM_FROM** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {          \
		return (_INTERNAL_NAME)(__p_maybe_dst_len, (_INTERNAL_TO**)__p_maybe_dst, __p_src_len,   \
		     (const _INTERNAL_FROM**)__p_src);                                                   \
	}                                                                                             \
	ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)                                                               \
	ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)                                                            \
	cnc_mcerr _SHIM_R_NAME(size_t* __p_maybe_dst_len, _SHIM_TO** __p_maybe_dst,                   \
	     size_t* __p_src_len, const _SHIM_FROM** __p_src, cnc_mcstate_t* __p_state)               \
	     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {                                                           \
		return (_INTERNAL_R_NAME)(__p_maybe_dst_len, (_INTERNAL_TO**)__p_maybe_dst, __p_src_len, \
		     (const _INTERNAL_FROM**)__p_src, __p_state);                                        \
	}                                                                                             \
	static_assert(true, "")

#define CNC_TYPED_CONVERSION_DEFN_I_(                                                            \
     _SHIM_PREFIX, _SHIM_SUFFIX, _INTERNAL_PREFIX, _INTERNAL_SUFFIX, _TAIL)                      \
	CNC_TYPED_CONVERSION_DEFN_ROOT_I_(cnc_##_SHIM_PREFIX##nto##_SHIM_SUFFIX##n##_TAIL,          \
	     cnc_##_SHIM_PREFIX##nrto##_SHIM_SUFFIX##n##_TAIL, TAG_TO_TYPE_I_(_SHIM_PREFIX),        \
	     TAG_TO_TYPE_I_(_SHIM_SUFFIX), cnc_##_INTERNAL_PREFIX##nto##_INTERNAL_SUFFIX##n,        \
	     cnc_##_INTERNAL_PREFIX##nrto##_INTERNAL_SUFFIX##n, TAG_TO_TYPE_I_(_INTERNAL_PREFIX),   \
	     TAG_TO_TYPE_I_(_INTERNAL_SUFFIX));                                                     \
	CNC_TYPED_CONVERSION_DEFN_ROOT_I_(cnc_##_SHIM_PREFIX##snto##_SHIM_SUFFIX##sn##_TAIL,        \
	     cnc_##_SHIM_PREFIX##snrto##_SHIM_SUFFIX##sn##_TAIL, TAG_TO_TYPE_I_(_SHIM_PREFIX),      \
	     TAG_TO_TYPE_I_(_SHIM_SUFFIX), cnc_##_INTERNAL_PREFIX##snto##_INTERNAL_SUFFIX##sn,      \
	     cnc_##_INTERNAL_PREFIX##snrto##_INTERNAL_SUFFIX##sn, TAG_TO_TYPE_I_(_INTERNAL_PREFIX), \
	     TAG_TO_TYPE_I_(_INTERNAL_SUFFIX))

#endif
