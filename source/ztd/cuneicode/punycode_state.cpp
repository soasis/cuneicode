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

#include <ztd/cuneicode/punycode_state.h>
#include <ztd/cuneicode/detail/punycode.hpp>

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ bool cnc_pny_encode_state_is_complete(
     const cnc_pny_encode_state_t* __p_state) {
	if (__p_state == nullptr || !__p_state->__is_initialized) {
		return true;
	}
	return __p_state->input_is_complete
	     && __p_state->__action_state == ::cnc::__cnc_detail::__pny_encode_state_complete;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ bool cnc_pny_decode_state_is_complete(
     const cnc_pny_decode_state_t* __p_state) {
	if (__p_state == nullptr || !__p_state->__is_initialized) {
		return true;
	}
	return __p_state->input_is_complete
	     && __p_state->__action_state == ::cnc::__cnc_detail::__pny_decode_state_complete;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_pny_encode_state_set_input_complete(
     cnc_pny_encode_state_t* __p_state) {
	__p_state->input_is_complete = true;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_pny_decode_state_set_input_complete(
     cnc_pny_decode_state_t* __p_state) {
	__p_state->input_is_complete = true;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ bool cnc_pny_encode_state_is_input_complete(
     const cnc_pny_encode_state_t* __p_state) {
	return __p_state->input_is_complete;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ bool cnc_pny_decode_state_is_input_complete(
     const cnc_pny_decode_state_t* __p_state) {
	return __p_state->input_is_complete;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_pny_encode_state_set_input_incomplete(
     cnc_pny_encode_state_t* __p_state) {
	__p_state->input_is_complete = false;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_pny_decode_state_set_input_incomplete(
     cnc_pny_decode_state_t* __p_state) {
	__p_state->input_is_complete = false;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_pny_encode_state_set_assume_valid(
     cnc_pny_encode_state_t* __p_state, bool __value) {
	__p_state->__assume_valid = __value;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_pny_decode_state_set_assume_valid(
     cnc_pny_decode_state_t* __p_state, bool __value) {
	__p_state->__assume_valid = __value;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ bool cnc_pny_encode_state_is_assuming_valid(
     const cnc_pny_encode_state_t* __p_state) {
	return __p_state->__assume_valid;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ bool cnc_pny_decode_state_is_assuming_valid(
     const cnc_pny_decode_state_t* __p_state) {
	return __p_state->__assume_valid;
}
