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

#ifndef ZTD_CUNEICODE_RESULT_H
#define ZTD_CUNEICODE_RESULT_H

#pragma once

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/mcerr.h>
#include <ztd/cuneicode/max_input_output.h>
#include <ztd/cuneicode/prefix.h>

#include <ztd/idk/charN_t.h>
#include <ztd/idk/generic.h>
#include <ztd/idk/declval.h>
#include <ztd/idk/typeof.h>

#if ZTD_IS_ON(ZTD_CXX)
#include <cstddef>
#else
#include <stddef.h>
#include <stdbool.h>
#endif

//////
/// @addtogroup ztd_cuneicode_conversion_result Conversion Function Result Structures
///
/// @{

//////
/// @brief A structure that does not contain the input and output and only the error code and error
/// count.
typedef struct cnc_error_result_t {
	//////
	/// @brief The error code.
	cnc_mcerr error_code;
	//////
	/// @brief The number of errors encountered in the input.
	size_t error_count;
} cnc_error_result_t;

//////
/// @brief A structure for the input and output values of an encoding, decoding, or transcoding
/// operation using `size_t`-based counts.
typedef struct cnc_count_result_t {
	//////
	/// @brief The error code.
	cnc_mcerr error_code;
	//////
	/// @brief The size of the input that was consumed.
	size_t input_count;
	//////
	/// @brief The size of the output that was consumed.
	size_t output_count;
} cnc_count_result_t;

#define ZTD_CNC_SINGLE_RESULT_DECL_I_(_NAME, _IN_TYPE, _OUT_TYPE, _MAXSIZE) \
	typedef struct _NAME {                                                 \
		cnc_mcerr error_code;                                             \
		size_t input_count;                                               \
		_IN_TYPE const* input;                                            \
		size_t output_count;                                              \
		_OUT_TYPE output[_MAXSIZE];                                       \
	} _NAME

#define ZTD_CNC_RESULT_DECL_I_(_NAME, _IN_TYPE, _OUT_TYPE) \
	typedef struct _NAME {                                \
		cnc_mcerr error_code;                            \
		size_t input_count;                              \
		_IN_TYPE const* input;                           \
		size_t output_count;                             \
		_OUT_TYPE* output;                               \
	} _NAME

#define ZTD_CNC_SINGLE_ERROR_RESULT_DECL_I_(_NAME, _IN_TYPE, _OUT_TYPE, _MAXSIZE) \
	typedef struct _NAME {                                                       \
		cnc_mcerr error_code;                                                   \
		size_t error_count;                                                     \
		size_t input_count;                                                     \
		_IN_TYPE const* input;                                                  \
		size_t output_count;                                                    \
		_OUT_TYPE output[_MAXSIZE];                                             \
	} _NAME

#define ZTD_CNC_ERROR_RESULT_DECL_I_(_NAME, _IN_TYPE, _OUT_TYPE) \
	typedef struct _NAME {                                      \
		cnc_mcerr error_code;                                  \
		size_t error_count;                                    \
		size_t input_count;                                    \
		_IN_TYPE const* input;                                 \
		size_t output_count;                                   \
		_OUT_TYPE* output;                                     \
	} _NAME


//////
/// @brief Produces the expected name out the structure matching the 2 types for the desired name of
/// the `cnc_*_reuslt` type.
///
/// @param[in] _IN_PREFIX The desired input type prefix name.
/// @param[in] _OUT_PREFIX The desired output type prefix name.
///
/// @remarks The affiliation is:
///
/// - uc: `unsigned char`
/// - mc: `ztd_char_t`
/// - mwc: `ztd_wchar_t`
/// - c8: `char8_t`
/// - c16: `char16_t`
/// - c32: `char32_t`
///
/// The `unsigned char` / `char8_t` types will technically be identical in C versions, but in
/// C++ `char8_t` is a distinct type. Similarly, `char16_t` and `char32_t` are also typedefs in C,
/// and so have the very tiny (but not zero) chance of overlapping with other typedefs. A more
/// practical example is `wchar_t`, which on Windows-based machines can have `char16_t` and
/// `wchar_t` both be aliased to `unsigned short`.
#define cnc_single_result_name(_IN_PREFIX, _OUT_PREFIX) \
	cnc_##_IN_PREFIX##_OUT_PREFIX##_single_result_t
//////
/// @copydoc cnc_single_result_name
#define cnc_result_name(_IN_PREFIX, _OUT_PREFIX) cnc_##_IN_PREFIX##_OUT_PREFIX##_result_t
//////
/// @copydoc cnc_single_result_name
#define cnc_single_error_result_name(_IN_PREFIX, _OUT_PREFIX) \
	cnc_##_IN_PREFIX##_OUT_PREFIX##_single_error_result_t
//////
/// @copydoc cnc_single_result_name
#define cnc_error_result_name(_IN_PREFIX, _OUT_PREFIX) \
	cnc_##_IN_PREFIX##_OUT_PREFIX##_error_result_t

#define ZTD_CNC_ALL_RESULT_DECLS_I_(_PREFIX, _SUFFIX, _IN_TYPE, _OUT_TYPE, _MAXSIZE)      \
	ZTD_CNC_SINGLE_RESULT_DECL_I_(                                                       \
	     cnc_single_result_name(_PREFIX, _SUFFIX), _IN_TYPE, _OUT_TYPE, _MAXSIZE);       \
	ZTD_CNC_RESULT_DECL_I_(cnc_result_name(_PREFIX, _SUFFIX), _IN_TYPE, _OUT_TYPE);      \
	ZTD_CNC_SINGLE_ERROR_RESULT_DECL_I_(                                                 \
	     cnc_single_error_result_name(_PREFIX, _SUFFIX), _IN_TYPE, _OUT_TYPE, _MAXSIZE); \
	ZTD_CNC_ERROR_RESULT_DECL_I_(cnc_error_result_name(_PREFIX, _SUFFIX), _IN_TYPE, _OUT_TYPE)


ZTD_CNC_ALL_RESULT_DECLS_I_(uc, uc, unsigned char, unsigned char, (CNC_MC_MAX * 2));
ZTD_CNC_ALL_RESULT_DECLS_I_(c8, uc, ztd_char8_t, unsigned char, (CNC_MC_MAX * 2));
ZTD_CNC_ALL_RESULT_DECLS_I_(mc, uc, ztd_char_t, unsigned char, (CNC_MC_MAX * 2));
ZTD_CNC_ALL_RESULT_DECLS_I_(mwc, uc, ztd_wchar_t, unsigned char, (CNC_MC_MAX * 2));
ZTD_CNC_ALL_RESULT_DECLS_I_(c16, uc, ztd_char16_t, unsigned char, (CNC_MC_MAX * 2));
ZTD_CNC_ALL_RESULT_DECLS_I_(c32, uc, ztd_char32_t, unsigned char, (CNC_MC_MAX * 2));


ZTD_CNC_ALL_RESULT_DECLS_I_(uc, mc, unsigned char, ztd_char_t, CNC_MC_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(mc, mc, ztd_char_t, ztd_char_t, CNC_MC_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(mwc, mc, ztd_wchar_t, ztd_char_t, CNC_MC_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c8, mc, ztd_char8_t, ztd_char_t, CNC_MC_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c16, mc, ztd_char16_t, ztd_char_t, CNC_MC_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c32, mc, ztd_char32_t, ztd_char_t, CNC_MC_MAX);

ZTD_CNC_ALL_RESULT_DECLS_I_(uc, mwc, unsigned char, ztd_wchar_t, CNC_MWC_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(mwc, mwc, ztd_wchar_t, ztd_wchar_t, CNC_MWC_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(mc, mwc, ztd_char_t, ztd_wchar_t, CNC_MWC_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c8, mwc, ztd_char8_t, ztd_wchar_t, CNC_MWC_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c16, mwc, ztd_char16_t, ztd_wchar_t, CNC_MWC_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c32, mwc, ztd_char32_t, ztd_wchar_t, CNC_MWC_MAX);

ZTD_CNC_ALL_RESULT_DECLS_I_(uc, c8, unsigned char, ztd_char8_t, CNC_C8_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c8, c8, ztd_char8_t, ztd_char8_t, CNC_C8_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(mc, c8, ztd_char_t, ztd_char8_t, CNC_C8_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(mwc, c8, ztd_wchar_t, ztd_char8_t, CNC_C8_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c16, c8, ztd_char16_t, ztd_char8_t, CNC_C8_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c32, c8, ztd_char32_t, ztd_char8_t, CNC_C8_MAX);

ZTD_CNC_ALL_RESULT_DECLS_I_(c16, c16, ztd_char16_t, ztd_char16_t, CNC_C16_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(uc, c16, unsigned char, ztd_char16_t, CNC_C16_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(mc, c16, ztd_char_t, ztd_char16_t, CNC_C16_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(mwc, c16, ztd_wchar_t, ztd_char16_t, CNC_C16_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c8, c16, ztd_char8_t, ztd_char16_t, CNC_C16_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c32, c16, ztd_char32_t, ztd_char16_t, CNC_C16_MAX);

ZTD_CNC_ALL_RESULT_DECLS_I_(uc, c32, unsigned char, ztd_char32_t, CNC_C32_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c32, c32, ztd_char32_t, ztd_char32_t, CNC_C32_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(mc, c32, ztd_char_t, ztd_char32_t, CNC_C32_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(mwc, c32, ztd_wchar_t, ztd_char32_t, CNC_C32_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c8, c32, ztd_char8_t, ztd_char32_t, CNC_C32_MAX);
ZTD_CNC_ALL_RESULT_DECLS_I_(c16, c32, ztd_char16_t, ztd_char32_t, CNC_C32_MAX);

#undef ZTD_CNC_ALL_RESULT_DECLS_I_
#undef ZTD_CNC_SINGLE_ERROR_RESULT_DECL_I_
#undef ZTD_CNC_SINGLE_RESULT_DECL_I_
#undef ZTD_CNC_ERROR_RESULT_DECL_I_
#undef ZTD_CNC_RESULT_DECL_I_
//////
/// @}


#if ZTD_IS_ON(ZTD_C)
// clang-format off
#define __cnc_uc_result(_OUT_TYPE, _PREFIX)                                                                                  \
	ztd_typeof(_Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                                 \
		ztd_char8_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_ucc8_,_PREFIX),result_t)),        \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char16_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_ucc16_,_PREFIX),result_t)), \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char32_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_ucc32_,_PREFIX),result_t)), \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_wchar_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_ucmwc_,_PREFIX),result_t)),  \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_ucmc_,_PREFIX),result_t)),    \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			unsigned char: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_ucuc_,_PREFIX),result_t)), \
			default: ztdc_uneval_void                                                                                      \
		)))))                                                                                                               \
	)())


#define __cnc_mc_result(_OUT_TYPE, _PREFIX)                                                                                  \
	ztd_typeof(_Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                                 \
		ztd_char8_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_mcc8_,_PREFIX),result_t)),        \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char16_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_mcc16_,_PREFIX),result_t)), \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char32_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_mcc32_,_PREFIX),result_t)), \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_wchar_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_mcmwc_,_PREFIX),result_t)),  \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_mcmc_,_PREFIX),result_t)),    \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			unsigned char: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_mcuc_,_PREFIX),result_t)), \
			default: ztdc_uneval_void                                                                                      \
		)))))                                                                                                               \
	)())


#define __cnc_mwc_result(_OUT_TYPE, _PREFIX)                                                                                  \
	ztd_typeof(_Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                                 \
		ztd_char8_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_mwcc8_,_PREFIX),result_t)),        \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char16_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_mwcc16_,_PREFIX),result_t)), \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char32_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_mwcc32_,_PREFIX),result_t)), \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_wchar_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_mwcmwc_,_PREFIX),result_t)),  \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_mwcmc_,_PREFIX),result_t)),    \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			unsigned char: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_mwcuc_,_PREFIX),result_t)), \
			default: ztdc_uneval_void                                                                                      \
		)))))                                                                                                               \
	)())


#define __cnc_c8_result(_OUT_TYPE, _PREFIX)                                                                                  \
	ztd_typeof(_Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                                 \
		ztd_char8_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c8c8_,_PREFIX),result_t)),        \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char16_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c8c16_,_PREFIX),result_t)), \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char32_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c8c32_,_PREFIX),result_t)), \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_wchar_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c8mwc_,_PREFIX),result_t)),  \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c8mc_,_PREFIX),result_t)),    \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			unsigned char: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c8uc_,_PREFIX),result_t)), \
			default: ztdc_uneval_void                                                                                      \
		)))))                                                                                                               \
	)())


#define __cnc_c16_result(_OUT_TYPE, _PREFIX)                                                                                  \
	ztd_typeof(_Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                                 \
		ztd_char8_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c16c8_,_PREFIX),result_t)),        \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char16_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c16c16_,_PREFIX),result_t)), \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char32_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c16c32_,_PREFIX),result_t)), \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_wchar_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c16mwc_,_PREFIX),result_t)),  \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c16mc_,_PREFIX),result_t)),    \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			unsigned char: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c16uc_,_PREFIX),result_t)), \
			default: ztdc_uneval_void                                                                                      \
		)))))                                                                                                               \
	)())


#define __cnc_c32_result(_OUT_TYPE, _PREFIX)                                                                                  \
	ztd_typeof(_Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                                 \
		ztd_char8_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c32c8_,_PREFIX),result_t)),        \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char16_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c32c16_,_PREFIX),result_t)), \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char32_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c32c32_,_PREFIX),result_t)), \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_wchar_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c32mwc_,_PREFIX),result_t)),  \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			ztd_char_t: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c32mc_,_PREFIX),result_t)),    \
		default: _Generic(ZTDC_DECLVAL(_OUT_TYPE),                                                                          \
			unsigned char: ZTDC_TYPE_SELECT_FUNC(ZTD_CONCAT_TOKENS_I_(ZTD_CONCAT_TOKENS_I_(cnc_c32uc_,_PREFIX),result_t)), \
			default: ztdc_uneval_void                                                                                      \
		)))))                                                                                                               \
	)())

#define __cnc_result(_IN_TYPE, _OUT_TYPE)                                                                             \
	ztd_typeof(_Generic(ZTDC_DECLVAL(_IN_TYPE),                                                                          \
		ztd_char8_t: ZTDC_TYPE_SELECT_FUNC(__cnc_c8_result(_OUT_TYPE, )),                                             \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_char16_t: ZTDC_TYPE_SELECT_FUNC(__cnc_c16_result(_OUT_TYPE, )), \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_char32_t: ZTDC_TYPE_SELECT_FUNC(__cnc_c32_result(_OUT_TYPE, )), \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_char_t: ZTDC_TYPE_SELECT_FUNC(__cnc_mc_result(_OUT_TYPE, )),    \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_wchar_t: ZTDC_TYPE_SELECT_FUNC(__cnc_mwc_result(_OUT_TYPE, )),  \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), unsigned char: ZTDC_TYPE_SELECT_FUNC(__cnc_uc_result(_OUT_TYPE, )), \
		default: ztdc_uneval_void                                                                                   \
		)))))                                                                                                       \
	)())


#define __cnc_single_result(_IN_TYPE, _OUT_TYPE)                                                                             \
	ztd_typeof(_Generic(ZTDC_DECLVAL(_IN_TYPE),                                                                          \
		ztd_char8_t: ZTDC_TYPE_SELECT_FUNC(__cnc_c8_result(_OUT_TYPE, single_)),                                             \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_char16_t: ZTDC_TYPE_SELECT_FUNC(__cnc_c16_result(_OUT_TYPE, single_)), \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_char32_t: ZTDC_TYPE_SELECT_FUNC(__cnc_c32_result(_OUT_TYPE, single_)), \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_char_t: ZTDC_TYPE_SELECT_FUNC(__cnc_mc_result(_OUT_TYPE, single_)),    \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_wchar_t: ZTDC_TYPE_SELECT_FUNC(__cnc_mwc_result(_OUT_TYPE, single_)),  \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), unsigned char: ZTDC_TYPE_SELECT_FUNC(__cnc_uc_result(_OUT_TYPE, single_)), \
		default: ztdc_uneval_void                                                                                   \
		)))))                                                                                                       \
	)())

#define __cnc_single_error_result(_IN_TYPE, _OUT_TYPE)                                                                \
	ztd_typeof(_Generic(ZTDC_DECLVAL(_IN_TYPE),                                                                          \
		ztd_char8_t: ZTDC_TYPE_SELECT_FUNC(__cnc_c8_result(_OUT_TYPE, single_error_)),                                \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_char16_t: ZTDC_TYPE_SELECT_FUNC(__cnc_c16_result(_OUT_TYPE, single_error_)), \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_char32_t: ZTDC_TYPE_SELECT_FUNC(__cnc_c32_result(_OUT_TYPE, single_error_)), \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_char_t: ZTDC_TYPE_SELECT_FUNC(__cnc_mc_result(_OUT_TYPE, single_error_)),    \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_wchar_t: ZTDC_TYPE_SELECT_FUNC(__cnc_mwc_result(_OUT_TYPE, single_error_)),  \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), unsigned char: ZTDC_TYPE_SELECT_FUNC(__cnc_uc_result(_OUT_TYPE, single_error_)), \
		default: ztdc_uneval_void                                                                                   \
		)))))                                                                                                       \
	)())

#define __cnc_error_result(_IN_TYPE, _OUT_TYPE)                                                                               \
	ztd_typeof(_Generic(ZTDC_DECLVAL(_IN_TYPE),                                                                                  \
		ztd_char8_t: ZTDC_TYPE_SELECT_FUNC(__cnc_c8_result(_OUT_TYPE, error_)),                                                     \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_char16_t: ZTDC_TYPE_SELECT_FUNC(__cnc_c16_result(_OUT_TYPE, error_)), \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_char32_t: ZTDC_TYPE_SELECT_FUNC(__cnc_c32_result(_OUT_TYPE, error_)), \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_char_t: ZTDC_TYPE_SELECT_FUNC(__cnc_mc_result(_OUT_TYPE, error_)),    \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), ztd_wchar_t: ZTDC_TYPE_SELECT_FUNC(__cnc_mwc_result(_OUT_TYPE, error_)),  \
		default: _Generic(ZTDC_DECLVAL(_IN_TYPE), unsigned char: ZTDC_TYPE_SELECT_FUNC(__cnc_uc_result(_OUT_TYPE, error_)), \
		default: ztdc_uneval_void                                                                                           \
		)))))                                                                                                               \
	)())
// clang-format on
#else

#include <type_traits>

#define __cnc_mc_result(_OUT_TYPE)                                                              \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_mcmc_result_t,                                                                    \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_mcmwc_result_t,                                                              \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_mcc8_result_t,                                                          \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_mcc16_result_t,                                                    \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_mcc32_result_t, void>>>>>

#define __cnc_mwc_result(_OUT_TYPE)                                                             \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_mwcmc_result_t,                                                                   \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_mwcmwc_result_t,                                                             \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_mwcc8_result_t,                                                         \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_mwcc16_result_t,                                                   \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_mwcc32_result_t, void>>>>>

#define __cnc_c8_result(_OUT_TYPE)                                                              \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_c8mc_result_t,                                                                    \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_c8mwc_result_t,                                                              \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_c8c8_result_t,                                                          \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_c8c16_result_t,                                                    \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_c8c32_result_t, void>>>>>

#define __cnc_c16_result(_OUT_TYPE)                                                             \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_c16mc_result_t,                                                                   \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_c16mwc_result_t,                                                             \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_c16c8_result_t,                                                         \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_c16c16_result_t,                                                   \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_c16c32_result_t, void>>>>>

#define __cnc_c32_result(_OUT_TYPE)                                                             \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_c32mc_result_t,                                                                   \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_c32mwc_result_t,                                                             \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_c32c8_result_t,                                                         \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_c32c16_result_t,                                                   \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_c32c32_result_t, void>>>>>

#define __cnc_mc_single_result(_OUT_TYPE)                                                       \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_mcmc_single_result_t,                                                             \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_mcmwc_single_result_t,                                                       \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_mcc8_single_result_t,                                                   \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_mcc16_single_result_t,                                             \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_mcc32_single_result_t, void>>>>>

#define __cnc_mwc_single_result(_OUT_TYPE)                                                      \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_mwcmc_single_result_t,                                                            \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_mwcmwc_single_result_t,                                                      \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_mwcc8_single_result_t,                                                  \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_mwcc16_single_result_t,                                            \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_mwcc32_single_result_t, void>>>>>

#define __cnc_c8_single_result(_OUT_TYPE)                                                       \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_c8mc_single_result_t,                                                             \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_c8mwc_single_result_t,                                                       \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_c8c8_single_result_t,                                                   \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_c8c16_single_result_t,                                             \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_c8c32_single_result_t, void>>>>>

#define __cnc_c16_single_result(_OUT_TYPE)                                                      \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_c16mc_single_result_t,                                                            \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_c16mwc_single_result_t,                                                      \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_c16c8_single_result_t,                                                  \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_c16c16_single_result_t,                                            \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_c16c32_single_result_t, void>>>>>


#define __cnc_c32_single_result(_OUT_TYPE)                                                      \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_c32mc_single_result_t,                                                            \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_c32mwc_single_result_t,                                                      \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_c32c8_single_result_t,                                                  \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_c32c16_single_result_t,                                            \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_c32c32_single_result_t, void>>>>>

#define __cnc_mc_error_result(_OUT_TYPE)                                                        \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_mcmc_error_result_t,                                                              \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_mcmwc_error_result_t,                                                        \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_mcc8_error_result_t,                                                    \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_mcc16_error_result_t,                                              \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_mcc32_error_result_t, void>>>>>

#define __cnc_mwc_error_result(_OUT_TYPE)                                                       \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_mwcmc_error_result_t,                                                             \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_mwcmwc_error_result_t,                                                       \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_mwcc8_error_result_t,                                                   \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_mwcc16_error_result_t,                                             \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_mwcc32_error_result_t, void>>>>>

#define __cnc_c8_error_result(_OUT_TYPE)                                                        \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_c8mc_error_result_t,                                                              \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_c8mwc_error_result_t,                                                        \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_c8c8_error_result_t,                                                    \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_c8c16_error_result_t,                                              \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_c8c32_error_result_t, void>>>>>

#define __cnc_c16_error_result(_OUT_TYPE)                                                       \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_c16mc_error_result_t,                                                             \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_c16mwc_error_result_t,                                                       \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_c16c8_error_result_t,                                                   \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_c16c16_error_result_t,                                             \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_c16c32_error_result_t, void>>>>>


#define __cnc_c32_error_result(_OUT_TYPE)                                                       \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_c32mc_error_result_t,                                                             \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_c32mwc_error_result_t,                                                       \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_c32c8_error_result_t,                                                   \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_c32c16_error_result_t,                                             \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_c32c32_error_result_t, void>>>>>

#define __cnc_mc_single_error_result(_OUT_TYPE)                                                 \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_mcmc_single_error_result_t,                                                       \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_mcmwc_single_error_result_t,                                                 \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_mcc8_single_error_result_t,                                             \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_mcc16_single_error_result_t,                                       \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_mcc32_single_error_result_t, void>>>>>

#define __cnc_mwc_single_error_result(_OUT_TYPE)                                                \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_mwcmc_single_error_result_t,                                                      \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_mwcmwc_single_error_result_t,                                                \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_mwcc8_single_error_result_t,                                            \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_mwcc16_single_error_result_t,                                      \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_mwcc32_single_error_result_t, void>>>>>

#define __cnc_c8_single_error_result(_OUT_TYPE)                                                 \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_c8mc_single_error_result_t,                                                       \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_c8mwc_single_error_result_t,                                                 \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_c8c8_single_error_result_t,                                             \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_c8c16_single_error_result_t,                                       \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_c8c32_single_error_result_t, void>>>>>

#define __cnc_c16_single_error_result(_OUT_TYPE)                                                \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_c16mc_single_error_result_t,                                                      \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_c16mwc_single_error_result_t,                                                \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_c16c8_single_error_result_t,                                            \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_c16c16_single_error_result_t,                                      \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_c16c32_single_error_result_t, void>>>>>


#define __cnc_c32_single_error_result(_OUT_TYPE)                                                \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char_t>,       \
	     cnc_c32mc_single_error_result_t,                                                      \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_wchar_t>, \
	          cnc_c32mwc_single_error_result_t,                                                \
	          ::std::conditional_t<                                                            \
	               ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char8_t>,            \
	               cnc_c32c8_single_error_result_t,                                            \
	               ::std::conditional_t<                                                       \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char16_t>,      \
	                    cnc_c32c16_single_error_result_t,                                      \
	                    ::std::conditional_t<                                                  \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_OUT_TYPE>, ztd_char32_t>, \
	                         cnc_c32c32_single_error_result_t, void>>>>>

#define __cnc_single_result(_IN_TYPE, _OUT_TYPE)                                               \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char_t>,       \
	     __cnc_mc_single_result(_OUT_TYPE),                                                   \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_wchar_t>, \
	          __cnc_mwc_single_result(_OUT_TYPE),                                             \
	          ::std::conditional_t<                                                           \
	               ::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char8_t>,            \
	               __cnc_c8_single_result(_OUT_TYPE),                                         \
	               ::std::conditional_t<                                                      \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char16_t>,      \
	                    __cnc_c16_single_result(_OUT_TYPE),                                   \
	                    ::std::conditional_t<                                                 \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char32_t>, \
	                         __cnc_c32_single_result(_OUT_TYPE), void>>>>>

#define __cnc_result(_IN_TYPE, _OUT_TYPE)                                                      \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char_t>,       \
	     __cnc_mc_result(_OUT_TYPE),                                                          \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_wchar_t>, \
	          __cnc_mwc_result(_OUT_TYPE),                                                    \
	          ::std::conditional_t<                                                           \
	               ::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char8_t>,            \
	               __cnc_c8_result(_OUT_TYPE),                                                \
	               ::std::conditional_t<                                                      \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char16_t>,      \
	                    __cnc_c16_result(_OUT_TYPE),                                          \
	                    ::std::conditional_t<                                                 \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char32_t>, \
	                         __cnc_c32_result(_OUT_TYPE), void>>>>>

#define __cnc_single_error_result(_IN_TYPE, _OUT_TYPE)                                         \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char_t>,       \
	     __cnc_mc_single_error_result(_OUT_TYPE),                                             \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_wchar_t>, \
	          __cnc_mwc_single_error_result(_OUT_TYPE),                                       \
	          ::std::conditional_t<                                                           \
	               ::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char8_t>,            \
	               __cnc_c8_single_error_result(_OUT_TYPE),                                   \
	               ::std::conditional_t<                                                      \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char16_t>,      \
	                    __cnc_c16_single_error_result(_OUT_TYPE),                             \
	                    ::std::conditional_t<                                                 \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char32_t>, \
	                         __cnc_c32_single_error_result(_OUT_TYPE), void>>>>>

#define __cnc_error_result(_IN_TYPE, _OUT_TYPE)                                                \
	::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char_t>,       \
	     __cnc_mc_error_result(_OUT_TYPE),                                                    \
	     ::std::conditional_t<::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_wchar_t>, \
	          __cnc_mwc_error_result(_OUT_TYPE),                                              \
	          ::std::conditional_t<                                                           \
	               ::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char8_t>,            \
	               __cnc_c8_error_result(_OUT_TYPE),                                          \
	               ::std::conditional_t<                                                      \
	                    ::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char16_t>,      \
	                    __cnc_c16_error_result(_OUT_TYPE),                                    \
	                    ::std::conditional_t<                                                 \
	                         ::std::is_same_v<::ztd::remove_cvref_t<_IN_TYPE>, ztd_char32_t>, \
	                         __cnc_c32_error_result(_OUT_TYPE), void>>>>>

#endif

//////
/// @brief Select a `cnc_*_result` type based on the input and output types placed into the
/// structure.
///
/// @param[in] _IN_TYPE The element type of the input. Can be one of `char`, `char8_t`, `char16_t`,
/// `char32_t`, or `wchar_t`.
/// @param[in] _OUT_TYPE The element type of the output. Can be one of `char`, `char8_t`,
/// `char16_t`, `char32_t`, or `wchar_t`.
#define cnc_single_result(_IN_TYPE, _OUT_TYPE) __cnc_single_result(_IN_TYPE, _OUT_TYPE)
//////
/// @copydoc cnc_single_result
#define cnc_result(_IN_TYPE, _OUT_TYPE) __cnc_result(_IN_TYPE, _OUT_TYPE)
//////
/// @copydoc cnc_single_result
#define cnc_single_error_result(_IN_TYPE, _OUT_TYPE) __cnc_single_error_result(_IN_TYPE, _OUT_TYPE)
//////
/// @copydoc cnc_single_result
#define cnc_error_result(_IN_TYPE, _OUT_TYPE) __cnc_error_result(_IN_TYPE, _OUT_TYPE)

#endif
