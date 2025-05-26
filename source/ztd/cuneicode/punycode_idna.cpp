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

#include <ztd/cuneicode/punycode_idna.h>
#include <ztd/cuneicode/punycode.h>
#include <ztd/cuneicode/max_input_output.h>
#include <ztd/cuneicode/detail/transcode.hpp>
#include <ztd/cuneicode/detail/punycode.hpp>

#include <ztd/idk/assert.hpp>
#include <ztd/idk/unreachable.hpp>
#include <ztd/idk/detail/unicode.hpp>

#include <memory>

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c32ntomcn_punycode_idna(size_t* __p_maybe_dst_len, ztd_char_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char32_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_pny_encode_state_t __state    = {};
	cnc_pny_encode_state_t* __p_state = ::std::addressof(__state);
	return cnc_c32nrtomcn_punycode_idna(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c32nrtomcn_punycode_idna(size_t* __p_maybe_dst_len, ztd_char_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char32_t** __p_src, cnc_pny_encode_state_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if (__p_src == nullptr || *__p_src == nullptr) {
		return cnc_mcerr_ok;
	}
	if (!__p_state->__is_initialized) {
		::cnc::__cnc_detail::__init_pny_encode_state(__p_state, true);
	}
	return cnc_c32nrtomcn_punycode(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcntoc32n_punycode_idna(size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_pny_decode_state_t __state    = {};
	cnc_pny_decode_state_t* __p_state = ::std::addressof(__state);
	return cnc_mcnrtoc32n_punycode_idna(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcnrtoc32n_punycode_idna(size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char_t** __p_src, cnc_pny_decode_state_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return ::cnc::__cnc_detail::__cnc_mcnrtoc32n_punycode_maybe_idna<true>(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c32sntomcsn_punycode_idna(size_t* __p_maybe_dst_len, ztd_char_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char32_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_pny_encode_state_t __state    = {};
	cnc_pny_encode_state_t* __p_state = ::std::addressof(__state);
	return cnc_c32snrtomcsn_punycode_idna(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c32snrtomcsn_punycode_idna(size_t* __p_maybe_dst_len, ztd_char_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char32_t** __p_src, cnc_pny_encode_state_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_c32nrtomcn_punycode_idna),
	     &cnc_c32nrtomcn_punycode_idna, ztd_char32_t, ztd_char_t,
	     decltype(&cnc_pny_encode_state_is_complete), &cnc_pny_encode_state_is_complete);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcsntoc32sn_punycode_idna(size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_pny_decode_state_t __state    = {};
	cnc_pny_decode_state_t* __p_state = ::std::addressof(__state);
	return cnc_mcsnrtoc32sn_punycode_idna(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcsnrtoc32sn_punycode_idna(size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char_t** __p_src, cnc_pny_decode_state_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_mcnrtoc32n_punycode_idna),
	     &cnc_mcnrtoc32n_punycode_idna, ztd_char_t, ztd_char32_t,
	     decltype(&cnc_pny_decode_state_is_complete), &cnc_pny_decode_state_is_complete);
}
