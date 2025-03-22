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

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/mcstate.h>

#include <ztd/cuneicode/detail/state.hpp>
#include <ztd/idk/unreachable.h>
#include <ztd/idk/size.h>

#include <memory>
#include <cstring>

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ bool cnc_mcstate_is_assuming_valid(const cnc_mcstate_t* __state) {
	if (__state == nullptr) {
		return false;
	}
	return __state->header.__assume_valid == static_cast<unsigned int>(1);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_mcstate_set_assume_valid(
     cnc_mcstate_t* __state, bool __check_validity) {
	if (__state == nullptr) {
		return;
	}
	__state->header.__assume_valid = static_cast<unsigned int>(__check_validity ? 1 : 0);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ bool cnc_mcstate_is_complete(const cnc_mcstate_t* __state) {
	if (__state == nullptr) {
		return true;
	}
	switch (__state->raw.indicator) {
	case __mc_s_i_locale:
#if ZTD_IS_ON(ZTD_HEADER_CWCHAR) || ZTD_IS_ON(ZTD_HEADER_WCHAR_H) || ZTD_IS_ON(ZTD_HEADER_CUCHAR) \
     || ZTD_IS_ON(ZTD_HEADER_UCHAR_H)
		return ZTD_MBSTATE_SCOPE_I_ mbsinit(::std::addressof(__state->__locale.__state0)) != 0
		     && ZTD_MBSTATE_SCOPE_I_ mbsinit(::std::addressof(__state->__locale.__state1)) != 0;
#else
		return true;
#endif
	case __mc_s_i_raw: {
		if (__state->raw.completion_function != nullptr) {
			return __state->raw.completion_function(
			     __state, __state->raw.raw_data, ztdc_c_array_size(__state->raw.raw_data));
		}
		else {
			for (std::size_t __index = 0; __index < ztdc_c_array_size(__state->raw.raw_data);
			     ++__index) {
				if (__state->raw.raw_data[__index] != 0) {
					return false;
				}
			}
			return true;
		}
	}
	default:
		break;
	}
	return true;
}
