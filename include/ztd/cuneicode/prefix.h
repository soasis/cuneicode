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

#ifndef ZTD_CUNEICODE_PREFIX_H
#define ZTD_CUNEICODE_PREFIX_H

#pragma once

#include <ztd/cuneicode/version.h>

#include <ztd/idk/charN_t.h>

#define __cnc_prefix_to_type_uc() unsigned char
#define __cnc_prefix_to_type_mc() ztd_wchar_t
#define __cnc_prefix_to_type_mwc() ztd_wchar_t
#define __cnc_prefix_to_type_c8() ztd_char8_t
#define __cnc_prefix_to_type_c16() ztd_char16_t
#define __cnc_prefix_to_type_c32() ztd_char32_t

//////
/// @brief Converts the given lowercase prefix name to the type it represents in the system.
///
/// @param[in] _PREFIX The identifier that presents the prefix to convert to a type.
///
/// @remarks The prefix ↔ type association is as follows:
///
/// - uc: `unsigned char` (UTF-8)
/// - mc: `ztd_char_t` (Execution)
/// - mwc: `ztd_wchar_t` (Wide Execution)
/// - c8: `char8_t` (UTF-8)
/// - c16: `char16_t` (UTF-16)
/// - c32: `char32_t` (UTF-32)
///
/// Use of other prefixes is an error.
#define cnc_prefix_to_type(_PREFIX) ZTD_CONCAT_TOKENS_I_(__cnc_prefix_to_type_, _PREFIX)()

#endif
