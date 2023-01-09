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

#include <ztd/cuneicode/shift_jis_x0208.h>
#include <ztd/cuneicode/max_output.h>
#include <ztd/cuneicode/detail/transcode.hpp>

#include <ztd/idk/assert.hpp>
#include <ztd/encoding_tables/shift_jis_x0208.tables.hpp>

#include <memory>
#include <optional>

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32ntomcn_shift_jis_x0208(
     size_t* __p_maybe_dst_len, char** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char32_t** __p_src) ZTD_NOEXCEPT_IF_CXX_I_ {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_c32nrtomcn_shift_jis_x0208(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32nrtomcn_shift_jis_x0208(
     size_t* __p_maybe_dst_len, char** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char32_t** __p_src, cnc_mcstate_t*) ZTD_NOEXCEPT_IF_CXX_I_ {
	using code_point = ztd_char32_t;
	using code_unit  = ztd_char_t;

	_ZTDC_CUNEICODE_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS(code_point);

	code_point __code = __src[0];
	if (__code <= 0x80) {
		if (!_IsUnbounded) {
			if (__p_maybe_dst_len[0] == 0) {
				// output is empty :(
				return cnc_mcerr_insufficient_output;
			}
			__p_maybe_dst_len[0] -= 1;
		}
		if (!_IsCounting) {
			__p_maybe_dst[0][0] = static_cast<code_unit>(__code);
			__p_maybe_dst[0] += 1;
		}
		__src += 1;
		__src_len -= 1;
		return cnc_mcerr_ok;
	}
	else if (__code == 0x00A5) {
		if (!_IsUnbounded) {
			if (__p_maybe_dst_len[0] == 0) {
				// output is empty :(
				return cnc_mcerr_insufficient_output;
			}
			__p_maybe_dst_len[0] -= 1;
		}
		if (!_IsCounting) {
			__p_maybe_dst[0][0] = static_cast<code_unit>(0x5C);
			__p_maybe_dst[0] += 1;
		}
		__src += 1;
		__src_len -= 1;
		return cnc_mcerr_ok;
	}
	else if (__code == 0x203E) {
		if (!_IsUnbounded) {
			if (__p_maybe_dst_len[0] == 0) {
				// output is empty :(
				return cnc_mcerr_insufficient_output;
			}
			__p_maybe_dst_len[0] -= 1;
		}
		if (!_IsCounting) {
			__p_maybe_dst[0][0] = static_cast<code_unit>(0x7E);
			__p_maybe_dst[0] += 1;
		}
		__src += 1;
		__src_len -= 1;
		return cnc_mcerr_ok;
	}
	else if (__code >= 0xFF61 && __code <= 0xFF9F) {
		if (!_IsUnbounded) {
			if (__p_maybe_dst_len[0] == 0) {
				// output is empty :(
				return cnc_mcerr_insufficient_output;
			}
			__p_maybe_dst_len[0] -= 1;
		}
		if (!_IsCounting) {
			code_point __intermediate = (__code - 0xFF61);
			__p_maybe_dst[0][0]       = static_cast<code_unit>(__intermediate + 0xA1);
			__p_maybe_dst[0] += 1;
		}
		__src += 1;
		__src_len -= 1;
		return cnc_mcerr_ok;
	}

	if (__code == 0x2212) {
		__code = 0xFF0D;
	}

	if (!_IsUnbounded) {
		if (__p_maybe_dst_len[0] < 2) {
			// output is empty :(
			return cnc_mcerr_insufficient_output;
		}
	}
	::std::optional<::std::size_t> __maybe_index
	     = ::ztd::et::shift_jis_x0208_code_point_to_index(static_cast<uint_least32_t>(__code));
	if (__maybe_index) {
		::std::size_t __index         = *__maybe_index;
		::std::size_t __first         = __index / 188;
		::std::size_t __first_offset  = __first < 0x1F ? 0x81 : 0xC1;
		::std::size_t __second        = __index % 188;
		::std::size_t __second_offset = __second < 0x3F ? 0x40 : 0x41;

		if (!_IsUnbounded) {
			__p_maybe_dst_len[0] -= 2;
		}
		if (!_IsCounting) {
			__p_maybe_dst[0][0] = static_cast<code_unit>(__first + __first_offset);
			__p_maybe_dst[0][1] = static_cast<code_unit>(__second + __second_offset);
			__p_maybe_dst[0] += 2;
		}
		__src += 1;
		__src_len -= 1;
		return cnc_mcerr_ok;
	}

	return cnc_mcerr_invalid_sequence;
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcntoc32n_shift_jis_x0208(
     size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src) ZTD_NOEXCEPT_IF_CXX_I_ {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcnrtoc32n_shift_jis_x0208(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcnrtoc32n_shift_jis_x0208(
     size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src, cnc_mcstate_t*) ZTD_NOEXCEPT_IF_CXX_I_ {
	using code_point = ztd_char32_t;
	using code_unit  = ztd_char_t;

	_ZTDC_CUNEICODE_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS(code_unit);

	unsigned char __first_byte = static_cast<unsigned char>(__src[0]);

	if (__first_byte <= 0x80) {
		// Top-Level case 0: it's an ASCII byte
		code_point __code = static_cast<code_point>(__first_byte);
		if (!_IsUnbounded) {
			if (__p_maybe_dst_len[0] == 0) {
				// output is empty :(
				return cnc_mcerr_insufficient_output;
			}
			__p_maybe_dst_len[0] -= 1;
		}
		if (!_IsCounting) {
			__p_maybe_dst[0][0] = __code;
			__p_maybe_dst[0] += 1;
		}
		__src += 1;
		__src_len -= 1;
		return cnc_mcerr_ok;
	}
	else if (__first_byte <= 0xDF && __first_byte >= 0xA1) {
		// Top-Level case 1: it's non-ASCII single-byte
		code_point __code = static_cast<code_point>((0xFF61 - 0xA1) + __first_byte);
		if (!_IsUnbounded) {
			if (__p_maybe_dst_len[0] == 0) {
				// output is empty :(
				return cnc_mcerr_insufficient_output;
			}
			__p_maybe_dst_len[0] -= 1;
		}
		if (!_IsCounting) {
			__p_maybe_dst[0][0] = __code;
			__p_maybe_dst[0] += 1;
		}
		__src += 1;
		__src_len -= 1;
		return cnc_mcerr_ok;
	}
	else if ((__first_byte <= 0x9F && __first_byte >= 0x81)
	     || (__first_byte <= 0xFC && __first_byte >= 0xE0)) {
		// Top-Level case 2: this is a double-byte sequence!
		if (__src_len < 2) {
			return cnc_mcerr_incomplete_input;
		}
		unsigned char __second_byte        = static_cast<unsigned char>(__src[1]);
		unsigned char lookup_offset        = __second_byte < 0x7F ? 0x40 : 0x41;
		unsigned char __lead_lookup_offset = __first_byte < 0xA0 ? 0x81 : 0xC1;
		std::size_t __lookup_index         = 0;
		if ((__second_byte <= 0x7E && __second_byte >= 0x40)
		     || (__second_byte <= 0xFC && __second_byte >= 0x80)) {
			__lookup_index = (((__first_byte - __lead_lookup_offset) * 188) + __second_byte)
			     - lookup_offset;
			if (__lookup_index <= 10715 && __lookup_index >= 8836) {
				code_point __code = static_cast<code_point>(0xE000 - 8836 + __lookup_index);
				if (!_IsUnbounded) {
					if (__p_maybe_dst_len[0] == 0) {
						// output is empty :(
						return cnc_mcerr_insufficient_output;
					}
					__p_maybe_dst_len[0] -= 1;
				}
				if (!_IsCounting) {
					__p_maybe_dst[0][0] = __code;
					__p_maybe_dst[0] += 1;
				}
			}
			else {
				::std::optional<::std::uint_least32_t> maybe_code
				     = ::ztd::et::shift_jis_x0208_index_to_code_point(__lookup_index);
				if (maybe_code) {
					const code_point& __code = *maybe_code;
					if (!_IsUnbounded) {
						if (__p_maybe_dst_len[0] == 0) {
							// output is empty :(
							return cnc_mcerr_insufficient_output;
						}
						__p_maybe_dst_len[0] -= 1;
					}
					if (!_IsCounting) {
						__p_maybe_dst[0][0] = __code;
						__p_maybe_dst[0] += 1;
					}
				}
			}
			__src += 2;
			__src_len -= 2;
			return cnc_mcerr_ok;
		}
	}

	// Top-Level case 3 (default): unrecognized byte sequence!!
	return cnc_mcerr_invalid_sequence;
}



ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32sntomcsn_shift_jis_x0208(
     size_t* __p_maybe_dst_len, char** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char32_t** __p_src) ZTD_NOEXCEPT_IF_CXX_I_ {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_c32snrtomcsn_shift_jis_x0208(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32snrtomcsn_shift_jis_x0208(
     size_t* __p_maybe_dst_len, char** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char32_t** __p_src, cnc_mcstate_t* __p_state) ZTD_NOEXCEPT_IF_CXX_I_ {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_c32nrtomcn_shift_jis_x0208),
	     &cnc_c32nrtomcn_shift_jis_x0208, ztd_char32_t, ztd_char_t);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcsntoc32sn_shift_jis_x0208(
     size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src) ZTD_NOEXCEPT_IF_CXX_I_ {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcsnrtoc32sn_shift_jis_x0208(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcsnrtoc32sn_shift_jis_x0208(
     size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src, cnc_mcstate_t* __p_state) ZTD_NOEXCEPT_IF_CXX_I_ {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_mcnrtoc32n_shift_jis_x0208),
	     &cnc_mcnrtoc32n_shift_jis_x0208, ztd_char_t, ztd_char32_t);
}
