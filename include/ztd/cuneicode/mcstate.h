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
#else
#include <stddef.h>
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

//////
/// @addtogroup ztd_cuneicode_conversion_state Typed Conversion Function State
///
/// @{
//////

//////
/// @brief The state for the typed conversion functions.
///
/// @remarks This is a complete object, but none of its members should be accessed or relied
/// upon in any way, shape or form. If you do so, it is Undefined Behavior.
typedef struct cnc_mcstate_t {
#if ZTD_IS_ON(ZTD_CWCHAR) || ZTD_IS_ON(ZTD_WCHAR) || ZTD_IS_ON(ZTD_CUCHAR) || ZTD_IS_ON(ZTD_UCHAR)
	//////
	/// @brief Private, do not access.
	mbstate_t __state0;
	//////
	/// @brief Private, do not access.
	mbstate_t __state1;
#else
	//////
	/// @brief Private, do not access.
	unsigned char __state0[8];
	//////
	/// @brief Private, do not access.
	unsigned char __state1[8];
#endif
} cnc_mcstate_t;

//////
/// @}
//////

#endif // ZTD_CUNEICODE_DETAIL_MCSTATE_H
