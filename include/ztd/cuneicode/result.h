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

#ifndef ZTD_CUNEICODE_RESULT_H
#define ZTD_CUNEICODE_RESULT_H

#pragma once

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/mcerr.h>
#include <ztd/cuneicode/max_output.h>

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
typedef struct cnc_error_result {
	//////
	/// @brief The error code.
	cnc_mcerr error_code;
	//////
	/// @brief The number of errors encountered in the input.
	size_t error_count;
} cnc_error_result;

//////
/// @brief A structure for the input and output values of an encoding, decoding, or transcoding
/// operation using `size_t`-based counts.
typedef struct cnc_count_result {
	//////
	/// @brief The error code.
	cnc_mcerr error_code;
	//////
	/// @brief The size of the input that was consumed.
	size_t input_count;
	//////
	/// @brief The size of the output that was consumed.
	size_t output_count;
} cnc_count_result;

#define ZTD_cnc_xy_single_result_STRUCT_I_(_NAME, _IN_TYPE, _OUT_TYPE, _MAXSIZE) \
	typedef struct _NAME {                                                      \
		cnc_mcerr error_code;                                                  \
		size_t input_count;                                                    \
		_IN_TYPE const* input;                                                 \
		size_t output_count;                                                   \
		_OUT_TYPE output[_MAXSIZE];                                            \
	} _NAME

#define ZTD_cnc_xy_result_STRUCT_I_(_NAME, _IN_TYPE, _OUT_TYPE) \
	typedef struct _NAME {                                     \
		cnc_mcerr error_code;                                 \
		size_t input_count;                                   \
		_IN_TYPE const* input;                                \
		size_t output_count;                                  \
		_OUT_TYPE* output;                                    \
	} _NAME

#define ZTD_cnc_xy_single_error_result_STRUCT_I_(_NAME, _IN_TYPE, _OUT_TYPE, _MAXSIZE) \
	typedef struct _NAME {                                                            \
		cnc_mcerr error_code;                                                        \
		size_t error_count;                                                          \
		size_t input_count;                                                          \
		_IN_TYPE const* input;                                                       \
		size_t output_count;                                                         \
		_OUT_TYPE output[_MAXSIZE];                                                  \
	} _NAME

#define ZTD_cnc_xy_error_result_STRUCT_I_(_NAME, _IN_TYPE, _OUT_TYPE) \
	typedef struct _NAME {                                           \
		cnc_mcerr error_code;                                       \
		size_t error_count;                                         \
		size_t input_count;                                         \
		_IN_TYPE const* input;                                      \
		size_t output_count;                                        \
		_OUT_TYPE* output;                                          \
	} _NAME


//////
/// @brief Produces the expected name out the structure matching the 2 types for the desired name of
/// the `cnc_*_reuslt` type.
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
/// and so have the very tiny (but not zero) chance of overlapping with other typedefs, meaning it
/// is not necessarily stable to use ZTDC_CASCADING_GENERIC over the inner contents of the
/// input/output of the structure.
#define cnc_xy_single_result(_INNAME, _OUTNAME) cnc_##_INNAME##_OUTNAME##_single_result
#define cnc_xy_result(_INNAME, _OUTNAME) cnc_##_INNAME##_OUTNAME##_result

#define cnc_xy_single_error_result(_INNAME, _OUTNAME) cnc_##_INNAME##_OUTNAME##_single_error_result
#define cnc_xy_error_result(_INNAME, _OUTNAME) cnc_##_INNAME##_OUTNAME##_error_result

#define ZTD_CNC_ALL_RESULT_STRUCTS_I_(_PREFIX, _SUFFIX, _IN_TYPE, _OUT_TYPE, _MAXSIZE)  \
	ZTD_cnc_xy_single_result_STRUCT_I_(                                                \
	     cnc_xy_single_result(_PREFIX, _SUFFIX), _IN_TYPE, _OUT_TYPE, _MAXSIZE);       \
	ZTD_cnc_xy_result_STRUCT_I_(cnc_xy_result(_PREFIX, _SUFFIX), _IN_TYPE, _OUT_TYPE); \
	ZTD_cnc_xy_single_error_result_STRUCT_I_(                                          \
	     cnc_xy_single_error_result(_PREFIX, _SUFFIX), _IN_TYPE, _OUT_TYPE, _MAXSIZE); \
	ZTD_cnc_xy_error_result_STRUCT_I_(cnc_xy_error_result(_PREFIX, _SUFFIX), _IN_TYPE, _OUT_TYPE)


ZTD_CNC_ALL_RESULT_STRUCTS_I_(uc, uc, unsigned char, unsigned char, (CNC_MC_MAX * 2));

ZTD_CNC_ALL_RESULT_STRUCTS_I_(mc, mc, ztd_char_t, ztd_char_t, CNC_MC_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(mwc, mc, ztd_wchar_t, ztd_char_t, CNC_MC_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(c8, mc, ztd_char8_t, ztd_char_t, CNC_MC_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(c16, mc, ztd_char16_t, ztd_char_t, CNC_MC_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(c32, mc, ztd_char32_t, ztd_char_t, CNC_MC_MAX);

ZTD_CNC_ALL_RESULT_STRUCTS_I_(mwc, mwc, ztd_wchar_t, ztd_wchar_t, CNC_MWC_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(mc, mwc, ztd_char_t, ztd_wchar_t, CNC_MWC_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(c8, mwc, ztd_char8_t, ztd_wchar_t, CNC_MWC_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(c16, mwc, ztd_char16_t, ztd_wchar_t, CNC_MWC_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(c32, mwc, ztd_char32_t, ztd_wchar_t, CNC_MWC_MAX);

ZTD_CNC_ALL_RESULT_STRUCTS_I_(c8, c8, ztd_char8_t, ztd_char8_t, CNC_C8_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(mc, c8, ztd_char_t, ztd_char8_t, CNC_C8_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(mwc, c8, ztd_wchar_t, ztd_char8_t, CNC_C8_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(c16, c8, ztd_char16_t, ztd_char8_t, CNC_C8_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(c32, c8, ztd_char32_t, ztd_char8_t, CNC_C8_MAX);

ZTD_CNC_ALL_RESULT_STRUCTS_I_(c16, c16, ztd_char16_t, ztd_char16_t, CNC_C16_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(mc, c16, ztd_char_t, ztd_char16_t, CNC_C16_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(mwc, c16, ztd_wchar_t, ztd_char16_t, CNC_C16_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(c8, c16, ztd_char8_t, ztd_char16_t, CNC_C16_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(c32, c16, ztd_char32_t, ztd_char16_t, CNC_C16_MAX);

ZTD_CNC_ALL_RESULT_STRUCTS_I_(c32, c32, ztd_char32_t, ztd_char32_t, CNC_C32_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(mc, c32, ztd_char_t, ztd_char32_t, CNC_C32_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(mwc, c32, ztd_wchar_t, ztd_char32_t, CNC_C32_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(c8, c32, ztd_char8_t, ztd_char32_t, CNC_C32_MAX);
ZTD_CNC_ALL_RESULT_STRUCTS_I_(c16, c32, ztd_char16_t, ztd_char32_t, CNC_C32_MAX);

//////
/// @}

#define __cnc_single_mc_result(_OUT_TYPE)                               \
	typeof(ZTDC_CASCADING_GENERIC(ZTDC_DECLVAL(_OUT_TYPE), ztd_char_t, \
	     ZTDC_DECLVAL(cnc_mcmc_single_result), ztd_wchar_t,            \
	     ZTDC_DECLVAL(cnc_mcmwc_single_result), ztd_char8_t,           \
	     ZTDC_DECLVAL(cnc_mcc8_single_result), ztd_char16_t,           \
	     ZTDC_DECLVAL(cnc_mcc16_single_result), ztd_char32_t,          \
	     ZTDC_DECLVAL(cnc_mcc32_single_result)))
#define __cnc_single_mwc_result(_OUT_TYPE)                              \
	typeof(ZTDC_CASCADING_GENERIC(ZTDC_DECLVAL(_OUT_TYPE), ztd_char_t, \
	     ZTDC_DECLVAL(cnc_mwcmc_single_result), ztd_wchar_t,           \
	     ZTDC_DECLVAL(cnc_mwcmwc_single_result), ztd_char8_t,          \
	     ZTDC_DECLVAL(cnc_mwcc8_single_result), ztd_char16_t,          \
	     ZTDC_DECLVAL(cnc_mwcc16_single_result), ztd_char32_t,         \
	     ZTDC_DECLVAL(cnc_mwcc32_single_result)))
#define __cnc_single_c8_result(_OUT_TYPE)                               \
	typeof(ZTDC_CASCADING_GENERIC(ZTDC_DECLVAL(_OUT_TYPE), ztd_char_t, \
	     ZTDC_DECLVAL(cnc_c8mc_single_result), ztd_wchar_t,            \
	     ZTDC_DECLVAL(cnc_c8mwc_single_result), ztd_char8_t,           \
	     ZTDC_DECLVAL(cnc_c8c8_single_result), ztd_char16_t,           \
	     ZTDC_DECLVAL(cnc_c8c16_single_result), ztd_char32_t,          \
	     ZTDC_DECLVAL(cnc_c8c32_single_result)))
#define __cnc_single_c16_result(_OUT_TYPE)                              \
	typeof(ZTDC_CASCADING_GENERIC(ZTDC_DECLVAL(_OUT_TYPE), ztd_char_t, \
	     ZTDC_DECLVAL(cnc_c16mc_single_result), ztd_wchar_t,           \
	     ZTDC_DECLVAL(cnc_c16mwc_single_result), ztd_char8_t,          \
	     ZTDC_DECLVAL(cnc_c16c8_single_result), ztd_char16_t,          \
	     ZTDC_DECLVAL(cnc_c16c16_single_result), ztd_char32_t,         \
	     ZTDC_DECLVAL(cnc_c16c32_single_result)))
#define __cnc_single_c32_result(_OUT_TYPE)                              \
	typeof(ZTDC_CASCADING_GENERIC(ZTDC_DECLVAL(_OUT_TYPE), ztd_char_t, \
	     ZTDC_DECLVAL(cnc_c32mc_single_result), ztd_wchar_t,           \
	     ZTDC_DECLVAL(cnc_c32mwc_single_result), ztd_char8_t,          \
	     ZTDC_DECLVAL(cnc_c32c8_single_result), ztd_char16_t,          \
	     ZTDC_DECLVAL(cnc_c32c16_single_result), ztd_char32_t,         \
	     ZTDC_DECLVAL(cnc_c32c32_single_result)))


#define cnc_xy_single_result_type(_IN_TYPE, _OUT_TYPE)                    \
	typeof(ZTDC_CASCADING_GENERIC(ZTDC_DECLVAL(_IN_TYPE), ztd_char_t,    \
	     ZTDC_DECLVAL(__cnc_single_mc_result(_OUT_TYPE)), ztd_wchar_t,   \
	     ZTDC_DECLVAL(__cnc_single_mwc_result(_OUT_TYPE)), ztd_char8_t,  \
	     ZTDC_DECLVAL(__cnc_single_c8_result(_OUT_TYPE)), ztd_char16_t,  \
	     ZTDC_DECLVAL(__cnc_single_c16_result(_OUT_TYPE)), ztd_char32_t, \
	     ZTDC_DECLVAL(__cnc_single_c32_result(_OUT_TYPE))))

#define cnc_xy_result_type(_IN_TYPE, _OUT_TYPE)                        \
	typeof(ZTDC_CASCADING_GENERIC(ZTDC_DECLVAL(_IN_TYPE), ztd_char_t, \
	     ZTDC_DECLVAL(__cnc_mc_result(_OUT_TYPE)), ztd_wchar_t,       \
	     ZTDC_DECLVAL(__cnc_mwc_result(_OUT_TYPE)), ztd_char8_t,      \
	     ZTDC_DECLVAL(__cnc_c8_result(_OUT_TYPE)), ztd_char16_t,      \
	     ZTDC_DECLVAL(__cnc_c16_result(_OUT_TYPE)), ztd_char32_t,     \
	     ZTDC_DECLVAL(__cnc_c32_result(_OUT_TYPE))))

#define cnc_xy_single_error_result_type(_IN_TYPE, _OUT_TYPE)                    \
	typeof(ZTDC_CASCADING_GENERIC(ZTDC_DECLVAL(_IN_TYPE), ztd_char_t,          \
	     ZTDC_DECLVAL(__cnc_mc_single_error_result(_OUT_TYPE)), ztd_wchar_t,   \
	     ZTDC_DECLVAL(__cnc_mwc_single_error_result(_OUT_TYPE)), ztd_char8_t,  \
	     ZTDC_DECLVAL(__cnc_c8_single_error_result(_OUT_TYPE)), ztd_char16_t,  \
	     ZTDC_DECLVAL(__cnc_c16_single_error_result(_OUT_TYPE)), ztd_char32_t, \
	     ZTDC_DECLVAL(__cnc_c32_single_error_result(_OUT_TYPE))))

#define cnc_xy_error_result_type(_IN_TYPE, _OUT_TYPE)                    \
	typeof(ZTDC_CASCADING_GENERIC(ZTDC_DECLVAL(_IN_TYPE), ztd_char_t,   \
	     ZTDC_DECLVAL(__cnc_mc_error_result(_OUT_TYPE)), ztd_wchar_t,   \
	     ZTDC_DECLVAL(__cnc_mwc_error_result(_OUT_TYPE)), ztd_char8_t,  \
	     ZTDC_DECLVAL(__cnc_c8_error_result(_OUT_TYPE)), ztd_char16_t,  \
	     ZTDC_DECLVAL(__cnc_c16_error_result(_OUT_TYPE)), ztd_char32_t, \
	     ZTDC_DECLVAL(__cnc_c32_error_result(_OUT_TYPE))))


#endif
