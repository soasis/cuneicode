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

#ifndef ZTD_CUNEICODE_OPEN_ERR_H
#define ZTD_CUNEICODE_OPEN_ERR_H

#pragma once

#include <ztd/cuneicode/version.h>

//////
/// @addtogroup ztd_cuneicode_registry_error_types Registry Error Types
///
/// @{

//////
/// @brief The error that occurred when trying to open or create a conversion resource.
typedef enum cnc_open_err {
	//////
	/// @brief Returned when everything was okay.
	cnc_open_err_ok = 0,
	//////
	/// @brief Returned when there is no conversion path between the specified from and to
	/// encodings.
	cnc_open_err_no_conversion_path = -1,
	//////
	/// @brief Returned when there iss not enough output space to write into for creating the
	/// resource.
	cnc_open_err_insufficient_output = -2,
	//////
	/// @brief Returned when there is an invalid parameter passed in for creating the
	/// resource.
	cnc_open_err_invalid_parameter = -3,
	//////
	/// @brief Returned when a heap-related or allocation-related failure occurred.
	cnc_open_err_allocation_failure = -4
} cnc_open_err;


//////
/// @brief Returns a string representing the error code's name.
ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ const char* cnc_open_err_to_str(
     cnc_open_err __err);

//////
/// @}

#endif
