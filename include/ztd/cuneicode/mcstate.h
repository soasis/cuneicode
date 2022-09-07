// ============================================================================
//
// ztd.cuneicode
// Copyright © 2022-2022 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
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
// 		http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ========================================================================= //

#ifndef ZTD_CUNEICODE_MCSTATE_H
#define ZTD_CUNEICODE_MCSTATE_H

#pragma once

#include <ztd/cuneicode/version.h>

#if ZTD_IS_ON(ZTD_CXX)
#include <cstddef>
#include <climits>
#else
#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#endif
#if ZTD_IS_ON(ZTD_CWCHAR)
#include <cwchar>
#elif ZTD_IS_ON(ZTD_WCHAR)
#include <wchar.h>
#endif
#if ZTD_IS_ON(ZTD_CUCHAR)
#include <cuchar>
#elif ZTD_IS_ON(ZTD_UCHAR)
ZTD_EXTERN_C_OPEN_I_
#include <uchar.h>
ZTD_EXTERN_C_CLOSE_I_
#endif

typedef union cnc_mcstate_t cnc_mcstate_t;

//////
/// @addtogroup ztd_cuneicode_conversion_state Typed Conversion Function State
///
/// @{
//////

//////
/// @brief The function signature used to check whether or not a state has finished processing
/// all of its data.
///
/// @param[in] __state The cnc_mcstate_t object being checked.
/// @param[in] __data The fixed-size data within the cnc_mcstate_t object.
/// @param[in] __data_size The size of the fixed-size data kept within the cnc_mcstate_t, in number
/// of bytes.
///
/// @remarks The cnc_mcstate_t object should only be touched through its `raw` member and is only
/// provided for the use of its `raw` member and the member variables contained within `raw`. The
/// `raw.indcator` member must be set to CNC_MCSTATE_INDICATOR_RAW by a user. Any data needing to be
/// checked or stored should be accessed in the provided [`__data`, `__data_size`] region.
typedef bool(state_is_complete_function)(
     const cnc_mcstate_t* __state, const unsigned char* __data, size_t __data_size);

//////
/// @brief An enumeration containing the current state indications.
///
/// @remarks May only use 8 bits of information.
//////
typedef enum cnc_mcstate_indicator {
	//////
	/// @brief Reserved, do NOT use.
	CNC_MCSTATE_INDICATOR___RESERVED0 = 0,
	//////
	/// @brief The state indicator that indicates a user is using the "raw" portion for their
	/// custom conversion routine.
	CNC_MCSTATE_INDICATOR_RAW = 1,
	//////
	/// @brief Reserved, do NOT use.
	CNC_MCSTATE_INDICATOR___RESERVED1 = 2
} cnc_mcstate_indicator;

//////
/// @brief The state for the typed conversion functions.
///
/// @remarks This is a complete object, but none of its members should be accessed or relied
/// upon in any way, shape or form. If you do so, it is Undefined Behavior.
typedef union cnc_mcstate_t {
#if ZTD_IS_ON(ZTD_CWCHAR) || ZTD_IS_ON(ZTD_WCHAR) || ZTD_IS_ON(ZTD_CUCHAR) || ZTD_IS_ON(ZTD_UCHAR)
	//////
	/// @brief Private, do not access.
	struct __locale_t {
		//////
		/// @brief Private, do not access.
		cnc_mcstate_indicator __indicator : CHAR_BIT;
		//////
		/// @brief Private, do not access.
		unsigned int __padding : (sizeof(cnc_mcstate_indicator) * CHAR_BIT) - CHAR_BIT;
		//////
		/// @brief Private, do not access.
		mbstate_t __state0;
		//////
		/// @brief Private, do not access.
		mbstate_t __state1;
	} __locale;
#endif
	//////
	/// @brief The raw type for user use.
	struct __raw_t {
		//////
		/// @brief The indactor. Must be set by any custom encoding routine using mcstate_t and
		/// desiring custom completion behavior to CNC_MCSTATE_INDICATOR_RAW.
		//////
		cnc_mcstate_indicator indicator : CHAR_BIT;
		//////
		/// @brief Padding to keep consistent sizing. Not meant to be part of any location.
		//////
		unsigned int __paddding : (sizeof(cnc_mcstate_indicator) * CHAR_BIT) - CHAR_BIT;
		//////
		/// @brief The completion function. If behavior beyond a check for the provided
		/// fixed-size data is zero is desired, then this must be set to a valid
		/// function pointer. Otherwise, it must be a null pointer.
		state_is_complete_function* completion_function;
		//////
		/// @brief Leftover data blob for use by the user. The user is responsible for its
		/// management within the conversion functions.
		unsigned char raw_data[(sizeof(void*) * 3)];
	} raw;
} cnc_mcstate_t;

//////
/// @brief Returns whether or not the given cnc_mcstate_t has no more data that needs to be output.
ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ bool cnc_mcstate_is_complete(
     const cnc_mcstate_t* __state);

//////
/// @}
//////

#endif // ZTD_CUNEICODE_DETAIL_MCSTATE_H
