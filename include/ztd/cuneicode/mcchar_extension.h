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

#ifndef ZTD_CUNEICODE_MCCHAR_EXTENSION_H
#define ZTD_CUNEICODE_MCCHAR_EXTENSION_H

#pragma once

#include <ztd/cuneicode/version.h>

#if ZTD_IS_ON(ZTD_CUNEICODE_EXTENSION_FUNCTIONS)
// For the simple macros/functions using statement expressions

#include <ztd/cuneicode/mcerr.h>
#include <ztd/cuneicode/mcchar_generic.h>
#include <ztd/cuneicode/prefix.h>
#include <ztd/cuneicode/result.h>

#include <ztd/idk/size.h>
#include <ztd/idk/null.h>
#include <ztd/idk/auto.h>

#if ZTD_IS_ON(ZTD_C)
#include <stdlib.h>
#include <stddef.h>
#else
#include <cstdlib>
#include <cstddef>
#endif

//////
/// @addtogroup ztd_cuneicode_generic_typed_conversions Generic Typed Conversion Functions
///
/// @{

//////
/// @brief Calls a conversion function associated with the types of the input types `_TO` and
/// `_FROM`, handling errors and only exiting on a non-handled error or when the input is exhausted.
///
/// @param[in] _TO_N The size of the ouput buffer.
/// @param[in] _TO The output buffer to write into.
/// @param[in] _FROM_N The size of the input buffer to read from.
/// @param[in] _FROM The input buffer to read from.
/// @param[in] _P_STATE A pointer to the state to use for this conversion.
/// @param[in] _ERROR_HANDLER The error handler to use. Must be one of the generic error handler
/// types.
/// @param[in] _USER_DATA The user data that is passed along to the error handler.
#define cnc_cxsnrtocysn_into_with_handler(                                                  \
     _TO_N, _TO, _FROM_N, _FROM, _P_STATE, _ERROR_HANDLER, _USER_DATA)                      \
	ZTD_STMT_EXPR_BEGIN()                                                                  \
	typedef typeof(*(_FROM)) __from_t;                                                     \
	typedef typeof(*(_TO)) __to_t;                                                         \
	typedef cnc_error_result(__from_t, __to_t) __result_t;                                 \
	typedef typeof(*_P_STATE) __state_t;                                                   \
	__from_t const* const __start_from = (_FROM);                                          \
	__from_t const* __from             = __start_from;                                     \
	const size_t __start_from_n        = (_FROM_N);                                        \
	size_t __from_n                    = __start_from_n;                                   \
	__to_t* const __start_to           = (_TO);                                            \
	__to_t* __to                       = __start_to;                                       \
	const size_t __start_to_n          = (_TO_N);                                          \
	size_t __to_n                      = __start_to_n;                                     \
	__state_t* __p_state               = (_P_STATE);                                       \
	__result_t __result                = {};                                               \
	void* __p_user_data                = (_USER_DATA);                                     \
                                                                                            \
	ztd_auto __error_hander                                                                \
	     = __cnc_detail_select_error_handler_cxntocyn(_ERROR_HANDLER, &__to, &__from);     \
                                                                                            \
	for (;;) {                                                                             \
		cnc_mcerr __err = cnc_cxsnrtocysn(&__to_n, &__to, &__from_n, &__from, __p_state); \
		if (__err != cnc_mcerr_ok) {                                                      \
			__result.error_count += 1;                                                   \
			__err = __error_hander(                                                      \
			     __err, &__to_n, &__to, &__from_n, &__from, __p_state, __p_user_data);   \
		}                                                                                 \
		if (__err != cnc_mcerr_ok) {                                                      \
			__result.error_code = __err;                                                 \
			break;                                                                       \
		}                                                                                 \
		if (__from_n == 0) {                                                              \
			if (cnc_mcstate_is_complete(__p_state)) {                                    \
				break;                                                                  \
			}                                                                            \
			continue;                                                                    \
		}                                                                                 \
	}                                                                                      \
	__result.input_count  = __start_from_n - __from_n;                                     \
	__result.output_count = __start_to_n - __to_n;                                         \
	__result.input        = __start_from;                                                  \
	__result.output       = __start_to;                                                    \
	ZTD_STMT_EXPR_RETURN(__result);                                                        \
	ZTD_STMT_EXPR_END()

//////
/// @brief Calls cnc_cxsnrtocysn_with_handler with the `state` pointer set to a null pointer
/// constant.
///
/// @param[in] _TO_N The size of the ouput buffer.
/// @param[in] _TO The output buffer to write into.
/// @param[in] _FROM_N The size of the input buffer to read from.
/// @param[in] _FROM The input buffer to read from.
/// @param[in] _ERROR_HANDLER The error handler to use. Must be one of the generic error handler
/// types.
/// @param[in] _USER_DATA The user data that is passed along to the error handler.
#define cnc_cxsntocysn_into_with_handler(_TO_N, _TO, _FROM_N, _FROM, _ERROR_HANDLER, _USER_DATA) \
	cnc_cxsnrtocysn_with_handler(                                                               \
	     _TO_N, _TO, _FROM_N, _FROM, ztd_null_of(cnc_mcstate_t), _ERROR_HANDLER, _USER_DATA)

//////
/// @brief Calls a conversion function associated with the types of the input types `_TO` and
/// `_FROM`, handling errors and only exiting on a non-handled error or when the input is exhausted.
///
/// @param[in] _TO_N The size of the ouput buffer.
/// @param[in] _TO The output buffer to write into.
/// @param[in] _FROM The null-terminated input buffer to read from.
/// @param[in] _P_STATE A pointer to the state to use for this conversion.
/// @param[in] _ERROR_HANDLER The error handler to use. Must be one of the generic error handler
/// types.
/// @param[in] _USER_DATA The user data that is passed along to the error handler.
#define cnc_cxsrtocysn_into_with_handler(_TO_N, _TO, _FROM, _P_STATE, _ERROR_HANDLER, _USER_DATA) \
	ZTD_STMT_EXPR_BEGIN()                                                                        \
	typedef typeof(*(_FROM)) __from_t;                                                           \
	typedef typeof(*(_TO)) __to_t;                                                               \
	typedef cnc_error_result(__from_t, __to_t) __result_t;                                       \
	typedef typeof(*_P_STATE) __state_t;                                                         \
	__from_t const* const __start_from = (_FROM);                                                \
	__from_t const* __from             = __start_from;                                           \
	const size_t __start_from_n        = ztdc_c_string_ptr_size(__from);                         \
	size_t __from_n                    = __start_from_n;                                         \
	__to_t* const __start_to           = (_TO);                                                  \
	__to_t* __to                       = __start_to;                                             \
	const size_t __start_to_n          = (_TO_N);                                                \
	size_t __to_n                      = __start_to_n;                                           \
	__state_t* __p_state               = (_P_STATE);                                             \
	__result_t __result                = {};                                                     \
	void* __p_user_data                = (_USER_DATA);                                           \
                                                                                                  \
	ztd_auto __error_hander                                                                      \
	     = __cnc_detail_select_error_handler_cxntocyn(_ERROR_HANDLER, &__to, &__from);           \
                                                                                                  \
	for (;;) {                                                                                   \
		cnc_mcerr __err = cnc_cxsnrtocysn(&__to_n, &__to, &__from_n, &__from, __p_state);       \
		if (__err != cnc_mcerr_ok) {                                                            \
			__result.error_count += 1;                                                         \
			__err = __error_hander(                                                            \
			     __err, &__to_n, &__to, &__from_n, &__from, __p_state, __p_user_data);         \
		}                                                                                       \
		if (__err != cnc_mcerr_ok) {                                                            \
			__result.error_code = __err;                                                       \
			break;                                                                             \
		}                                                                                       \
		if (__from_n == 0) {                                                                    \
			if (cnc_mcstate_is_complete(__p_state)) {                                          \
				break;                                                                        \
			}                                                                                  \
			continue;                                                                          \
		}                                                                                       \
	}                                                                                            \
	__result.input_count  = __start_from_n - __from_n;                                           \
	__result.output_count = __start_to_n - __to_n;                                               \
	__result.input        = __start_from;                                                        \
	__result.output       = __start_to;                                                          \
	ZTD_STMT_EXPR_RETURN(__result);                                                              \
	ZTD_STMT_EXPR_END()

//////
/// @brief Calls cnc_cxsrtocysn_with_handler with the `state` pointer set to a null pointer
/// constant.
#define cnc_cxstocysn_into_with_handler(_TO_N, _TO, _FROM, _ERROR_HANDLER, _USER_DATA) \
	cnc_cxsrtocysn_with_handler(                                                      \
	     _TO_N, _TO, _FROM, ztd_null_of(cnc_mcstate_t), _ERROR_HANDLER, _USER_DATA)

//////
/// @brief Calls a conversion function associated with the types of the input types `_TO` and
/// `_FROM`, handling errors and only exiting on a non-handled error or when the input is exhausted.
///
/// @param[in] _TO_N The size of the ouput buffer.
/// @param[in] _TO The output buffer to write into.
/// @param[in] _FROM_N The size of the input buffer to read from.
/// @param[in] _FROM The input buffer to read from.
/// @param[in] _P_STATE A pointer to the state to use for this conversion.
#define cnc_cxsnrtocysn_into(_TO_N, _TO, _FROM_N, _FROM, _P_STATE)                           \
	ZTD_STMT_EXPR_BEGIN()                                                                   \
	typedef typeof(*(_FROM)) __from_t;                                                      \
	typedef typeof(*(_TO)) __to_t;                                                          \
	typedef cnc_result(__from_t, __to_t) __result_t;                                        \
	typedef typeof(*(_P_STATE)) __state_t;                                                  \
	__from_t const* const __start_from = (_FROM);                                           \
	__from_t const* __from             = __start_from;                                      \
	const size_t __start_from_n        = (_FROM_N);                                         \
	size_t __from_n                    = __start_from_n;                                    \
	__to_t* const __start_to           = (_TO);                                             \
	__to_t* __to                       = __start_to;                                        \
	const size_t __start_to_n          = (_TO_N);                                           \
	size_t __to_n                      = __start_to_n;                                      \
	__state_t* __p_state               = (_P_STATE);                                        \
	__result_t __result                = {};                                                \
	__result.error_code   = cnc_cxsnrtocysn(&__to_n, &__to, &__from_n, &__from, __p_state); \
	__result.input_count  = __start_from_n - __from_n;                                      \
	__result.output_count = __start_to_n - __to_n;                                          \
	__result.input        = __start_from;                                                   \
	__result.output       = __start_to;                                                     \
	ZTD_STMT_EXPR_RETURN(__result);                                                         \
	ZTD_STMT_EXPR_END()

//////
/// @brief Calls cnc_cxsnrtocysn_into with the `state` pointer set to a null pointer constant.
///
/// @param[in] _TO_N The size of the ouput buffer.
/// @param[in] _TO The output buffer to write into.
/// @param[in] _FROM_N The size of the input buffer to read from.
/// @param[in] _FROM The input buffer to read from.
#define cnc_cxsntocysn_into(_TO_N, _TO, _FROM_N, _FROM) \
	cnc_cxsnrtocysn_into(_TO_N, _TO, _FROM_N, _FROM, ztd_null_of(cnc_mcstate_t))

//////
/// @brief Calls a conversion function associated with the types of the input types `_TO` and
/// `_FROM`, handling errors and only exiting on a non-handled error or when the input is exhausted.
///
/// @param[in] _TO_N The size of the ouput buffer.
/// @param[in] _TO The output buffer to write into.
/// @param[in] _FROM The null-terminated input buffer to read from.
/// @param[in] _P_STATE A pointer to the state to use for this conversion.
#define cnc_cxsrtocysn_into(_TO_N, _TO, _FROM, _P_STATE)                                     \
	ZTD_STMT_EXPR_BEGIN()                                                                   \
	typedef typeof(*(_FROM)) __from_t;                                                      \
	typedef typeof(*(_TO)) __to_t;                                                          \
	typedef cnc_result(__from_t, __to_t) __result_t;                                        \
	typedef typeof(*(_P_STATE)) __state_t;                                                  \
	__from_t const* const __start_from = (_FROM);                                           \
	__from_t const* __from             = __start_from;                                      \
	const size_t __start_from_n        = ztdc_c_string_ptr_size(__from);                    \
	size_t __from_n                    = __start_from_n;                                    \
	__to_t* const __start_to           = (_TO);                                             \
	__to_t* __to                       = __start_to;                                        \
	const size_t __start_to_n          = (_TO_N);                                           \
	size_t __to_n                      = __start_to_n;                                      \
	__state_t* __p_state               = (_P_STATE);                                        \
	__result_t __result                = {};                                                \
	__result.error_code   = cnc_cxsnrtocysn(&__to_n, &__to, &__from_n, &__from, __p_state); \
	__result.input_count  = __start_from_n - __from_n;                                      \
	__result.output_count = __start_to_n - __to_n;                                          \
	__result.input        = __start_from;                                                   \
	__result.output       = __start_to;                                                     \
	ZTD_STMT_EXPR_RETURN(__result);                                                         \
	ZTD_STMT_EXPR_END()

//////
/// @brief Calls cnc_cxsrtocysn_into with the `state` pointer set to a null pointer constant.
///
/// @param[in] _TO_N The size of the ouput buffer.
/// @param[in] _TO The output buffer to write into.
/// @param[in] _FROM The null-terminated input buffer to read from.
#define cnc_cxstocysn_into(_TO_N, _TO, _FROM) \
	cnc_cxsrtocysn_into(_TO_N, _TO, _FROM, ztd_null_of(cnc_mcstate_t))

//////
/// @brief Determines whether or not the input is valid to be converted from itself to the given
/// `_TO_PREFIX`-specified name.
///
/// @param[in] _TO_PREFIX The prefix that identifies what this conversion is converting to.
/// @param[in] _FROM_N The size of the input buffer to read from.
/// @param[in] _FROM The input buffer to read from.
/// @param[in] _P_STATE A pointer to the state to use for this conversion.
#define cnc_cxsnrtocysn_into_is_valid(_TO_PREFIX, _FROM_N, _FROM, _P_STATE)          \
	ZTD_STMT_EXPR_BEGIN()                                                           \
	typedef typeof(*(_FROM)) __from_t;                                              \
	typedef cnc_prefix_to_type(_TO_PREFIX) __to_t;                                  \
	typedef typeof(*(_P_STATE)) __state_t;                                          \
	__from_t const* const __start_from = (_FROM);                                   \
	__from_t const* __from             = __start_from;                              \
	const size_t __start_from_n        = (_FROM_N);                                 \
	size_t __from_n                    = __start_from_n;                            \
	__state_t* __p_state               = (_P_STATE);                                \
                                                                                     \
	cnc_mcerr _err = cnc_cxsnrtocysn(                                               \
	     ztd_null_of(size_t), ztd_null_of(__to_t*), &__from_n, &__from, __p_state); \
	const bool __result = _err == cnc_mcerr_ok;                                     \
	ZTD_STMT_EXPR_RETURN(__result);                                                 \
	ZTD_STMT_EXPR_END()

//////
/// @brief Calls cnc_cxsnrtocysn_into_is_valid with the `state` pointer set to a null pointer
/// constant.
///
/// @param[in] _TO_PREFIX The prefix that identifies what this conversion is converting to.
/// @param[in] _FROM_N The size of the input buffer to read from.
/// @param[in] _FROM The input buffer to read from.
#define cnc_cxsntocysn_into_is_valid(_TO_PREFIX, _FROM_N, _FROM) \
	cnc_cxsnrtocysn_into_is_valid(_TO_PREFIX, _FROM_N, _FROM, ztd_null_of(cnc_mcstate_t))

//////
/// @brief Determines whether or not the input is valid to be converted from itself to the given
/// `_TO_PREFIX`-specified name, which indicates a type. See cnc_prefix_to_type to see the
/// type/encoding association.
///
/// @param[in] _TO_PREFIX The prefix that identifies what this conversion is converting to.
/// @param[in] _FROM The null-terminated input buffer to read from.
/// @param[in] _P_STATE A pointer to the state to use for this conversion.
#define cnc_cxsrtocysn_into_is_valid(_TO_PREFIX, _FROM, _P_STATE)                    \
	ZTD_STMT_EXPR_BEGIN()                                                           \
	typedef typeof(*(_FROM)) __from_t;                                              \
	typedef cnc_prefix_to_type(_TO_PREFIX) __to_t;                                  \
	typedef typeof(*(_P_STATE)) __state_t;                                          \
	__from_t const* const __start_from = (_FROM);                                   \
	__from_t const* __from             = __start_from;                              \
	const size_t __start_from_n        = ztdc_c_string_ptr_size(__from);            \
	size_t __from_n                    = __start_from_n;                            \
	__state_t* __p_state               = (_P_STATE);                                \
                                                                                     \
	cnc_mcerr __err = cnc_cxsnrtocysn(                                              \
	     ztd_null_of(size_t), ztd_null_of(__to_t*), &__from_n, &__from, __p_state); \
	const bool __result = __err == cnc_mcerr_ok;                                    \
	ZTD_STMT_EXPR_RETURN(__result);                                                 \
	ZTD_STMT_EXPR_END()

//////
/// @brief Calls cnc_cxsrtocysn_into_is_valid with the `state` pointer set to a null pointer
/// constant.
///
/// @param[in] _TO_PREFIX The prefix that identifies what this conversion is converting to.
/// @param[in] _FROM The null-terminated input buffer to read from.
#define cnc_cxstocysn_into_is_valid(_TO_PREFIX, _FROM) \
	cnc_cxsrtocysn_into_is_valid(_TO_PREFIX, _FROM, ztd_null_of(cnc_mcstate_t))

//////
/// @brief Determines whether or not the input is valid to be converted from itself to the given
/// `_TO_PREFIX`-specified name.
///
/// @param[in] _TO_PREFIX The prefix that identifies what this conversion is converting to.
/// @param[in] _FROM_N The size of the input buffer to read from.
/// @param[in] _FROM The input buffer to read from.
/// @param[in] _P_STATE A pointer to the state to use for this conversion.
#define cnc_cxsnrtocysn_into_count(_TO_PREFIX, _FROM_N, _FROM, _P_STATE)                   \
	ZTD_STMT_EXPR_BEGIN()                                                                 \
	typedef typeof(*(_FROM)) __from_t;                                                    \
	typedef cnc_prefix_to_type(_TO_PREFIX) __to_t;                                        \
	typedef typeof(*(_P_STATE)) __state_t;                                                \
	typedef cnc_count_result_t __result_t;                                                \
	__from_t const* const __start_from = (_FROM);                                         \
	__from_t const* __from             = __start_from;                                    \
	const size_t __start_from_n        = (_FROM_N);                                       \
	size_t __from_n                    = __start_from_n;                                  \
	const size_t __start_to_n          = SIZE_MAX;                                        \
	size_t __to_n                      = __start_to_n;                                    \
	__state_t* __p_state               = (_P_STATE);                                      \
	__result_t __result                = {};                                              \
	__result.error_code                                                                   \
	     = cnc_cxsnrtocysn(&__to_n, ztd_null_of(__to_t*), &__from_n, &__from, __p_state); \
	__result.input_count  = __start_from_n - __from_n;                                    \
	__result.output_count = __start_to_n - __to_n;                                        \
	ZTD_STMT_EXPR_RETURN(__result);                                                       \
	ZTD_STMT_EXPR_END()

//////
/// @brief Calls cnc_cxsnrtocysn_into_count with the `state` pointer set to a null pointer
/// constant.
///
/// @param[in] _TO_PREFIX The prefix that identifies what this conversion is converting to.
/// @param[in] _FROM_N The size of the input buffer to read from.
/// @param[in] _FROM The input buffer to read from.
#define cnc_cxsntocysn_into_count(_TO_PREFIX, _FROM_N, _FROM) \
	cnc_cxsnrtocysn_into_count(_TO_PREFIX, _FROM_N, _FROM, ztd_null_of(cnc_mcstate_t))

//////
/// @brief Determines whether or not the input is valid to be converted from itself to the given
/// `_TO_PREFIX`-specified name, which indicates a type. See cnc_prefix_to_type to see the
/// type/encoding association.
///
/// @param[in] _TO_PREFIX The prefix that identifies what this conversion is converting to.
/// @param[in] _FROM The null-terminated input buffer to read from.
/// @param[in] _P_STATE A pointer to the state to use for this conversion.
#define cnc_cxsrtocysn_into_count(_TO_PREFIX, _FROM, _P_STATE)                             \
	ZTD_STMT_EXPR_BEGIN()                                                                 \
	typedef typeof(*(_FROM)) __from_t;                                                    \
	typedef cnc_prefix_to_type(_TO_PREFIX) __to_t;                                        \
	typedef typeof(*(_P_STATE)) __state_t;                                                \
	typedef cnc_count_result_t __result_t;                                                \
	__from_t const* const __start_from = (_FROM);                                         \
	__from_t const* __from             = __start_from;                                    \
	const size_t __start_from_n        = ztdc_c_string_ptr_size(__from);                  \
	size_t __from_n                    = __start_from_n;                                  \
	__state_t* __p_state               = (_P_STATE);                                      \
	__result_t __result                = {};                                              \
	__result.error_code                                                                   \
	     = cnc_cxsnrtocysn(&__to_n, ztd_null_of(__to_t*), &__from_n, &__from, __p_state); \
	__result.input_count  = __start_from_n - __from_n;                                    \
	__result.output_count = __start_to_n - __to_n;                                        \
	ZTD_STMT_EXPR_RETURN(__result);                                                       \
	ZTD_STMT_EXPR_END()

//////
/// @brief Calls cnc_cxsrtocysn_into_count with the `state` pointer set to a null pointer
/// constant.
///
/// @param[in] _TO_PREFIX The prefix that identifies what this conversion is converting to.
/// @param[in] _FROM The null-terminated input buffer to read from.
#define cnc_cxstocysn_into_count(_TO_PREFIX, _FROM) \
	cnc_cxsrtocysn_into_count(_TO_PREFIX, _FROM, ztd_null_of(cnc_mcstate_t))

//////
/// @brief Determines whether or not the input is valid to be converted from itself to the given
/// `_TO_PREFIX`-specified name.
///
/// @param[in] _TO The output buffer to write into. Assumed to be large enough to handle the
/// operation.
/// @param[in] _FROM_N The size of the input buffer to read from.
/// @param[in] _FROM The input buffer to read from.
/// @param[in] _P_STATE A pointer to the state to use for this conversion.
#define cnc_cxsnrtocys_into_unbounded(_TO, _FROM_N, _FROM, _P_STATE)                    \
	ZTD_STMT_EXPR_BEGIN()                                                              \
	typedef typeof(*(_FROM)) __from_t;                                                 \
	typedef typeof(*(_TO)) __to_t;                                                     \
	typedef cnc_result(__from_t, __to_t) __result_t;                                   \
	typedef typeof(*(_P_STATE)) __state_t;                                             \
	__from_t const* const __start_from = (_FROM);                                      \
	__from_t const* __from             = __start_from;                                 \
	const size_t __start_from_n        = (_FROM_N);                                    \
	size_t __from_n                    = __start_from_n;                               \
	__to_t* const __start_to           = (_TO);                                        \
	__to_t* __to                       = __start_to;                                   \
	__state_t* __p_state               = (_P_STATE);                                   \
	__result_t __result                = {};                                           \
	__result.error_code                                                                \
	     = cnc_cxsnrtocysn(ztd_null_of(size_t), &__to, &__from_n, &__from, __p_state); \
	__result.input_count  = __start_from_n - __from_n;                                 \
	__result.output_count = __to - __start_to;                                         \
	__result.input        = __start_from;                                              \
	__result.output       = __start_to;                                                \
	ZTD_STMT_EXPR_RETURN(__result);                                                    \
	ZTD_STMT_EXPR_END()

//////
/// @brief Calls cnc_cxsnrtocysn_into_unbounded with the `state` pointer set to a null pointer
/// constant.
///
/// @param[in] _TO The output buffer to write into. Assumed to be large enough to handle the
/// operation.
/// @param[in] _FROM_N The size of the input buffer to read from.
/// @param[in] _FROM The null-terminated input buffer to read from.
#define cnc_cxsntocys_into_unbounded(_TO, _FROM_N, _FROM) \
	cnc_cxsnrtocys_into_unbounded(_TO, _FROM_N, _FROM, ztd_null_of(cnc_mcstate_t))

//////
/// @brief Determines whether or not the input is valid to be converted from itself to the given
/// `_TO_PREFIX`-specified name, which indicates a type. See cnc_prefix_to_type to see the
/// type/encoding association.
///
/// @param[in] _TO The output buffer to write into. Assumed to be large enough to handle the
/// operation.
/// @param[in] _FROM The null-terminated input buffer to read from.
/// @param[in] _P_STATE A pointer to the state to use for this conversion.
#define cnc_cxsrtocys_into_unbounded(_TO, _FROM, _P_STATE)                              \
	ZTD_STMT_EXPR_BEGIN()                                                              \
	typedef typeof(*(_FROM)) __from_t;                                                 \
	typedef typeof(*(_TO)) __to_t;                                                     \
	typedef cnc_result(__from_t, __to_t) __result_t;                                   \
	typedef typeof(*(_P_STATE)) __state_t;                                             \
	__from_t const* const __start_from = (_FROM);                                      \
	__from_t const* __from             = __start_from;                                 \
	const size_t __start_from_n        = ztdc_c_string_ptr_size(__from);               \
	size_t __from_n                    = __start_from_n;                               \
	__to_t* const __start_to           = (_TO);                                        \
	__to_t* __to                       = __start_to;                                   \
	__state_t* __p_state               = (_P_STATE);                                   \
	__result_t __result                = {};                                           \
	__result.error_code                                                                \
	     = cnc_cxsnrtocysn(ztd_null_of(size_t), &__to, &__from_n, &__from, __p_state); \
	__result.input_count  = __start_from - __from;                                     \
	__result.output_count = __start_to - __to;                                         \
	__result.input        = __start_from;                                              \
	__result.output       = __start_to;                                                \
	ZTD_STMT_EXPR_RETURN(__result);                                                    \
	ZTD_STMT_EXPR_END()

//////
/// @brief Calls cnc_cxsrtocysn_into_unbounded with the `state` pointer set to a null pointer
/// constant.
///
/// @param[in] _TO The output buffer to write into. Assumed to be large enough to handle the
/// operation.
/// @param[in] _FROM The null-terminated input buffer to read from.
#define cnc_cxstocys_into_unbounded(_TO, _FROM) \
	cnc_cxsrtocys_into_unbounded(_TO, _FROM, ztd_null_of(cnc_mcstate_t))

//////
/// @}

#endif

#endif
