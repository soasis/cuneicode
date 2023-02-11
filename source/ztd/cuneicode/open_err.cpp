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

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/open_err.h>

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ const char* cnc_open_err_to_str(
     cnc_open_err __err) {
	switch (__err) {
	case cnc_open_err_ok:
		return "cnc_open_err_ok";
	case cnc_open_err_no_conversion_path:
		return "cnc_open_err_incomplete_input";
	case cnc_open_err_allocation_failure:
		return "cnc_open_err_allocation_failure";
	case cnc_open_err_insufficient_output:
		return "cnc_open_err_insufficient_output";
	case cnc_open_err_invalid_parameter:
		return "cnc_open_err_invalid_parameter";
	}
	return "cnc_open_err_(unknown_error_code)";
}
