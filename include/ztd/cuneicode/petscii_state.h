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

#ifndef ZTD_CUNEICODE_PETSCII_STATE_H
#define ZTD_CUNEICODE_PETSCII_STATE_H

#pragma once

#include <ztd/cuneicode/version.h>

#if ZTD_IS_OFF(ZTD_CXX)
#include <stdbool.h>
#endif

//////
/// @addtogroup ztd_cuneicode_conversion Conversion Functions
///
/// @{

//////
/// @brief The state of a PETSCII encoding, which is either shifted / unshifted.
typedef struct cnc_petscii_state_t {
	//////
	/// @brief Whether or not the input is assumed to be perfectly in-range and valid.
	bool assume_valid;
	//////
	/// @brief Whether the PETSCII state is shifted/unshifted. This variable is ignored if the
	/// `*_petscii_unshifted` or `*_petscii_shifted` functions are used directly.
	bool shifted;
} cnc_petscii_state_t;

//////
/// @}

#endif
