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

#include <ztd/cuneicode/windows_1252.h>
#include <ztd/cuneicode/max_output.h>
#include <ztd/cuneicode/detail/transcode.hpp>

#include <ztd/encoding_tables/windows_1252.tables.hpp>

#include <memory>

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerror cnc_c32ntomcn_windows_1252(
     size_t* __p_maybe_dst_len, char** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char32_t** __p_src) ZTD_NOEXCEPT_IF_CXX_I_ {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_c32nrtomcn_windows_1252(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerror cnc_c32nrtomcn_windows_1252(
     size_t* __p_maybe_dst_len, char** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char32_t** __p_src, cnc_mcstate_t*) ZTD_NOEXCEPT_IF_CXX_I_ {

	_ZTDC_CUNEICODE_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS(ztd_char32_t);

	const ztd_char32_t __code_point = ((ztd_char32_t)*__src);

	if (__code_point <= 0x7F) {
		// ASCII character
		if (!_IsUnbounded) {
			if (__p_maybe_dst_len[0] < 1) {
				return CNC_MCERROR_INSUFFICIENT_OUTPUT;
			}
			__p_maybe_dst_len[0] -= 1;
		}
		if (!_IsCounting) {
			__p_maybe_dst[0][0] = (char)*__src;
			__p_maybe_dst[0] += 1;
		}
		__src += 1;
		__src_len -= 1;
		return CNC_MCERROR_OK;
	}

	::std::optional<::std::size_t> __maybe_index
	     = ::ztd::et::windows_1252_index_to_code_point(__code_point);
	if (__maybe_index) {
		const ::std::size_t __index      = *__maybe_index;
		const unsigned char __code_unit0 = (unsigned char)(__index + 0x80);
		if (!_IsUnbounded) {
			if (__p_maybe_dst_len[0] < 1) {
				return CNC_MCERROR_INSUFFICIENT_OUTPUT;
			}
			__p_maybe_dst_len[0] -= 1;
		}
		if (!_IsCounting) {
			__p_maybe_dst[0][0] = static_cast<char>(__code_unit0);
			__p_maybe_dst[0] += 1;
		}
		__src += 1;
		__src_len -= 1;
		return CNC_MCERROR_OK;
	}

	return CNC_MCERROR_INVALID_SEQUENCE;
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerror cnc_mcntoc32n_windows_1252(
     size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src) ZTD_NOEXCEPT_IF_CXX_I_ {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcnrtoc32n_windows_1252(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerror cnc_mcnrtoc32n_windows_1252(
     size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src, cnc_mcstate_t*) ZTD_NOEXCEPT_IF_CXX_I_ {

	_ZTDC_CUNEICODE_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS(char);

	const unsigned char __code_unit0 = ((unsigned char)*__src);

	if (__code_unit0 <= 0x7F) {
		// ASCII character
		if (!_IsUnbounded) {
			if (__p_maybe_dst_len[0] < 1) {
				return CNC_MCERROR_INSUFFICIENT_OUTPUT;
			}
			__p_maybe_dst_len[0] -= 1;
		}
		if (!_IsCounting) {
			__p_maybe_dst[0][0] = (ztd_char32_t)*__src;
			__p_maybe_dst[0] += 1;
		}
		__src += 1;
		__src_len -= 1;
		return CNC_MCERROR_OK;
	}

	::std::size_t __lookup_index = static_cast<::std::size_t>(__code_unit0 - 0x80);
	::std::optional<::std::uint_least32_t> __maybe_code_point
	     = ::ztd::et::windows_1252_index_to_code_point(__lookup_index);
	if (__maybe_code_point) {
		const ztd_char32_t __code_point = (ztd_char32_t)(*__maybe_code_point);
		if (!_IsUnbounded) {
			if (__p_maybe_dst_len[0] < 1) {
				return CNC_MCERROR_INSUFFICIENT_OUTPUT;
			}
			__p_maybe_dst_len[0] -= 1;
		}
		if (!_IsCounting) {
			__p_maybe_dst[0][0] = __code_point;
			__p_maybe_dst[0] += 1;
		}
		__src += 1;
		__src_len -= 1;
		return CNC_MCERROR_OK;
	}

	return CNC_MCERROR_INVALID_SEQUENCE;
}



ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerror cnc_c32sntomcsn_windows_1252(
     size_t* __p_maybe_dst_len, char** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char32_t** __p_src) ZTD_NOEXCEPT_IF_CXX_I_ {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_c32snrtomcsn_windows_1252(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerror cnc_c32snrtomcsn_windows_1252(
     size_t* __p_maybe_dst_len, char** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char32_t** __p_src, cnc_mcstate_t* __p_state) ZTD_NOEXCEPT_IF_CXX_I_ {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_c32nrtomcn_windows_1252),
	     &cnc_c32nrtomcn_windows_1252, ztd_char32_t, ztd_char_t);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerror cnc_mcsntoc32sn_windows_1252(
     size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src) ZTD_NOEXCEPT_IF_CXX_I_ {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcsnrtoc32sn_windows_1252(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerror cnc_mcsnrtoc32sn_windows_1252(
     size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src, cnc_mcstate_t* __p_state) ZTD_NOEXCEPT_IF_CXX_I_ {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_mcnrtoc32n_windows_1252),
	     &cnc_mcnrtoc32n_windows_1252, ztd_char_t, ztd_char32_t);
}
