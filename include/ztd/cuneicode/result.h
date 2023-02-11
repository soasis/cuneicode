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

#ifndef ZTD_CUNEICODE_reuslt_H
#define ZTD_CUNEICODE_reuslt_H

#pragma once

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/mcerr.h>
#include <ztd/idk/charN_t.h>

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
/// @brief A structure for the input and output values of an encoding, decoding, or transcoding
/// operation using `size_t`-based counts.
typedef struct cnc_count_result {
	//////
	/// @brief The error code.
	cnc_mcerr error;
	//////
	/// @brief The size of the input that was consumed.
	size_t input_count;
	//////
	/// @brief The size of the output that was consumed.
	size_t output_count;
} cnc_count_result;

#define ZTD_CNC_SINGLE_RESULT_STRUCT_I_(_NAME, _OUTTYPE, _MAXSIZE) \
	typedef struct _NAME {                                        \
		cnc_mcerr error;                                         \
		size_t input_count;                                      \
		size_t output_count;                                     \
		_OUTTYPE output[_MAXSIZE];                               \
	} _NAME

#define ZTD_CNC_RESULT_STRUCT_I_(_NAME, _INTYPE, _OUTTYPE) \
	typedef struct _NAME {                                \
		cnc_mcerr error;                                 \
		_INTYPE* input;                                  \
		_OUTTYPE* output;                                \
	} _NAME

//////
/// @brief Produces the expected name out the structure matching the 2 types for the desired name of
/// the `cnc_*_reuslt` type.
///
/// @remarks The affiliation is:
///
/// - uc: `unsigned char`
/// - mc: `char`
/// - mwc: `wchar_t`
/// - c8: `char8_t`
/// - c16: `char16_t`
/// - c32: `char32_t`
///
/// The `unsigned char` / `char8_t` types will technically be identical in C versions, but in C++
/// `char8_t` is a distinct type. Similarly, `char16_t` and `char32_t` are also typedefs in C, and
/// so have the very tiny (but not zero) chance of overlapping with other typedefs, meaning it is
/// not necessarily stable to use _Generic over the inner contents of the input/output of the
/// structure.
#define cnc_result(_INNAME, _OUTNAME) cnc_##_INNAME##_OUTNAME##_single_result

ZTD_CNC_RESULT_STRUCT_I_(cnc_result(uc, ), unsigned char, unsigned char);
typedef cnc_result(uc, ) cnc_result(uc, uc);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(mc, uc), ztd_char_t, unsigned char);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(mwc, uc), ztd_wchar_t, unsigned char);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c8, uc), ztd_char8_t, unsigned char);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c16, uc), ztd_char16_t, unsigned char);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c32, uc), ztd_char32_t, unsigned char);

ZTD_CNC_RESULT_STRUCT_I_(cnc_result(mc, ), ztd_char_t, ztd_char_t);
typedef cnc_result(mc, ) cnc_result(mc, mc);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(uc, mc), unsigned char, ztd_char_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(mwc, mc), ztd_wchar_t, ztd_char_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c8, mc), ztd_char8_t, ztd_char_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c16, mc), ztd_char16_t, ztd_char_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c32, mc), ztd_char32_t, ztd_char_t);

ZTD_CNC_RESULT_STRUCT_I_(cnc_result(mwc, ), ztd_wchar_t, ztd_wchar_t);
typedef cnc_result(mwc, ) cnc_result(mwc, mwc);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(uc, mwc), unsigned char, ztd_wchar_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(mc, mwc), ztd_char_t, ztd_wchar_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c8, mwc), ztd_char8_t, ztd_wchar_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c16, mwc), ztd_char16_t, ztd_wchar_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c32, mwc), ztd_char32_t, ztd_wchar_t);

ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c8, ), ztd_char8_t, ztd_char8_t);
typedef cnc_result(c8, ) cnc_result(c8, c8);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(uc, c8), unsigned char, ztd_char8_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(mc, c8), ztd_char_t, ztd_char8_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(mwc, c8), ztd_wchar_t, ztd_char8_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c16, c8), ztd_char16_t, ztd_char8_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c32, c8), ztd_char32_t, ztd_char8_t);

ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c16, ), ztd_char16_t, ztd_char16_t);
typedef cnc_result(c16, ) cnc_result(c16, c16);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(uc, c16), unsigned char, ztd_char16_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(mc, c16), ztd_char_t, ztd_char16_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(mwc, c16), ztd_wchar_t, ztd_char16_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c8, c16), ztd_char8_t, ztd_char16_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c32, c16), ztd_char32_t, ztd_char16_t);

ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c32, ), ztd_char32_t, ztd_char32_t);
typedef cnc_result(c32, ) cnc_result(c32, c32);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(uc, c32), unsigned char, ztd_char32_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(mc, c32), ztd_char_t, ztd_char32_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(mwc, c32), ztd_wchar_t, ztd_char32_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c8, c32), ztd_char8_t, ztd_char32_t);
ZTD_CNC_RESULT_STRUCT_I_(cnc_result(c16, c32), ztd_char16_t, ztd_char32_t);



//////
/// @brief Produces the expected name out the structure matching the 2 types for the desired name of
/// the `cnc_*_reuslt` type.
///
/// @remarks The affiliation is:
///
/// - uc: `unsigned char`
/// - mc: `char`
/// - mwc: `wchar_t`
/// - c8: `char8_t`
/// - c16: `char16_t`
/// - c32: `char32_t`
///
/// The `unsigned char` / `char8_t` types will technically be identical in C versions, but in C++
/// `char8_t` is a distinct type. Similarly, `char16_t` and `char32_t` are also typedefs in C, and
/// so have the very tiny (but not zero) chance of overlapping with other typedefs, meaning it is
/// not necessarily stable to use _Generic over the inner contents of the input/output of the
/// structure.
#define cnc_single_result(_OUTNAME) cnc_##_OUTNAME##_result

ZTD_CNC_SINGLE_RESULT_STRUCT_I_(cnc_single_result(uc), unsigned char, CNC_MC_MAX);
ZTD_CNC_SINGLE_RESULT_STRUCT_I_(cnc_single_result(mc), ztd_char_t, CNC_MC_MAX);
ZTD_CNC_SINGLE_RESULT_STRUCT_I_(cnc_single_result(mwc), ztd_wchar_t, CNC_MWC_MAX);
ZTD_CNC_SINGLE_RESULT_STRUCT_I_(cnc_single_result(c8), ztd_char8_t, CNC_C8_MAX);
ZTD_CNC_SINGLE_RESULT_STRUCT_I_(cnc_single_result(c16), ztd_char16_t, CNC_C16_MAX);
ZTD_CNC_SINGLE_RESULT_STRUCT_I_(cnc_single_result(c32), ztd_char32_t, CNC_C32_MAX);

//////
/// @}

#endif
