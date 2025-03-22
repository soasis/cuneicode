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

#include <ztd/cuneicode/iso_8859_16.h>
#include <ztd/cuneicode/max_output.h>
#include <ztd/cuneicode/detail/transcode.hpp>

#include <ztd/encoding_tables/iso_8859_16.tables.hpp>

#include <memory>

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32ntomcn_iso_8859_16(size_t* __p_maybe_dst_len,
     char** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_c32nrtomcn_iso_8859_16(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32nrtomcn_iso_8859_16(size_t* __p_maybe_dst_len,
     char** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src, cnc_mcstate_t*)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_SINGLE_ASCII_BYTE_HIGH_BIT_ENCODING_BYTE_TO_CODE_POINT(
	     ztd_char32_t, ztd_char_t, ::ztd::et::iso_8859_16_index_to_code_point);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcntoc32n_iso_8859_16(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const char** __p_src)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcnrtoc32n_iso_8859_16(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcnrtoc32n_iso_8859_16(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const char** __p_src, cnc_mcstate_t*)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_SINGLE_ASCII_BYTE_HIGH_BIT_ENCODING_CODE_POINT_TO_BYTE(
	     ztd_char_t, ztd_char32_t, ::ztd::et::iso_8859_16_code_point_to_index);
}



ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32sntomcsn_iso_8859_16(size_t* __p_maybe_dst_len,
     char** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_c32snrtomcsn_iso_8859_16(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32snrtomcsn_iso_8859_16(size_t* __p_maybe_dst_len,
     char** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src,
     cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_c32nrtomcn_iso_8859_16), &cnc_c32nrtomcn_iso_8859_16,
	     ztd_char32_t, ztd_char_t);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcsntoc32sn_iso_8859_16(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const char** __p_src)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcsnrtoc32sn_iso_8859_16(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcsnrtoc32sn_iso_8859_16(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const char** __p_src,
     cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_mcnrtoc32n_iso_8859_16), &cnc_mcnrtoc32n_iso_8859_16,
	     ztd_char_t, ztd_char32_t);
}
