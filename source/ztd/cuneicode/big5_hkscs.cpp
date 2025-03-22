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

#include <ztd/cuneicode/big5_hkscs.h>
#include <ztd/cuneicode/max_output.h>
#include <ztd/cuneicode/detail/transcode.hpp>
#include <ztd/encoding_tables/big5_hkscs.tables.hpp>

#include <ztd/idk/assert.hpp>

#include <memory>

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32ntomcn_big5_hkscs(size_t* __p_maybe_dst_len,
     char** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_c32nrtomcn_big5_hkscs(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32nrtomcn_big5_hkscs(size_t* __p_maybe_dst_len,
     char** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src, cnc_mcstate_t*)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	using code_point = ztd_char32_t;
	using code_unit  = ztd_char_t;

	_CNC_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS(ztd_char32_t);

	const code_point __code = __src[0];
	if (__code <= 0x7F) {
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
		__p_src[0] += 1;
		__p_src_len[0] -= 1;
		return cnc_mcerr_ok;
	}

	if (!_IsUnbounded) {
		if (__p_maybe_dst_len[0] < 2) {
			// output is empty :(
			return cnc_mcerr_insufficient_output;
		}
	}

	::std::optional<::std::size_t> __maybe_index
	     = ::ztd::et::big5_hkscs_code_point_to_index(__code);
	if (__maybe_index) {
		::std::size_t __index  = *__maybe_index;
		::std::size_t __lead   = (__index / 157) + 0x81;
		::std::size_t __trail  = __index % 157;
		::std::size_t __offset = __trail < 0x3F ? 0x40 : 0x62;

		if (!_IsUnbounded) {
			__p_maybe_dst_len[0] -= 2;
		}
		if (!_IsCounting) {
			__p_maybe_dst[0][0] = static_cast<code_unit>(__lead);
			__p_maybe_dst[0][1] = static_cast<code_unit>(__trail + __offset);
			__p_maybe_dst[0] += 2;
		}
		__p_src[0] += 1;
		__p_src_len[0] -= 1;
		return cnc_mcerr_ok;
	}

	return cnc_mcerr_invalid_sequence;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcntoc32n_big5_hkscs(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const char** __p_src)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcnrtoc32n_big5_hkscs(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcnrtoc32n_big5_hkscs(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const char** __p_src, cnc_mcstate_t*)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	using code_point = ztd_char32_t;
	using code_unit  = ztd_char_t;

	_CNC_BOILERPLATE_NULLPTR_AND_EMPTY_CHECKS(ztd_char_t);

	unsigned char __first_byte = static_cast<unsigned char>(__src[0]);
	if (__first_byte <= 0x7F) {
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
			__p_maybe_dst[0][0] = static_cast<code_unit>(__code);
			__p_maybe_dst[0] += 1;
		}

		__p_src[0] += 1;
		__p_src_len[0] -= 1;
		return cnc_mcerr_ok;
	}
	else if (__first_byte >= 0xFF || __first_byte == 0x80) {
		return cnc_mcerr_invalid_sequence;
	}

	// Case: must have 2 bytes
	if (__src_len < 2) {
		return cnc_mcerr_incomplete_input;
	}
	unsigned char __second_byte  = static_cast<unsigned char>(__src[1]);
	const ::std::size_t __offset = __second_byte < 0x7F ? 0x40 : 0x62;
	const ::std::size_t __index  = ((__first_byte - 0x81) * 157) + (__second_byte - __offset);
	constexpr const char32_t __double_codes[][2] = { { U'\u00CA', U'\u0304' },
		{ U'\u00CA', U'\u030C' }, { U'\u00EA', U'\u0304' }, { U'\u00EA', U'\u030C' } };
	const auto __write_double_output = [&_IsUnbounded, &_IsCounting, &__p_src, &__p_src_len,
	                                        __p_maybe_dst_len, &__p_maybe_dst,
	                                        &__double_codes](auto __code_index_type) noexcept {
		constexpr const ::std::size_t __code_index = decltype(__code_index_type)::value;
		if (!_IsUnbounded) {
			if (__p_maybe_dst_len[0] < 2) {
				return cnc_mcerr_insufficient_output;
			}
			__p_maybe_dst_len[0] -= 2;
		}
		if (!_IsCounting) {
			__p_maybe_dst[0][0] = static_cast<code_point>(__double_codes[__code_index][0]);
			__p_maybe_dst[0][1] = static_cast<code_point>(__double_codes[__code_index][1]);
			__p_maybe_dst[0] += 2;
		}
		__p_src[0] += 2;
		__p_src_len[0] -= 2;
		return cnc_mcerr_ok;
	};

	switch (__index) {
	case 1133:
		return __write_double_output(::std::integral_constant<::std::size_t, 0>());
	case 1135:
		return __write_double_output(::std::integral_constant<::std::size_t, 1>());
	case 1164:
		return __write_double_output(::std::integral_constant<::std::size_t, 2>());
	case 1166:
		return __write_double_output(::std::integral_constant<::std::size_t, 3>());
	default:
		if (!_IsUnbounded) {
			if (__p_maybe_dst_len[0] < 1) {
				return cnc_mcerr_insufficient_output;
			}
		}
		::std::optional<::std::uint_least32_t> __maybe_code
		     = ::ztd::et::big5_hkscs_index_to_code_point(__index);
		if (__maybe_code) {
			const char32_t __code = static_cast<char32_t>(*__maybe_code);
			if (!_IsUnbounded) {
				__p_maybe_dst_len[0] -= 1;
			}
			if (!_IsCounting) {
				__p_maybe_dst[0][0] = static_cast<code_point>(__code);
				__p_maybe_dst[0] += 1;
			}
			__p_src[0] += 2;
			__p_src_len[0] -= 2;
			return cnc_mcerr_ok;
		}
	}
	return cnc_mcerr_invalid_sequence;
}



ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32sntomcsn_big5_hkscs(size_t* __p_maybe_dst_len,
     char** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_c32snrtomcsn_big5_hkscs(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32snrtomcsn_big5_hkscs(size_t* __p_maybe_dst_len,
     char** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src,
     cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_c32nrtomcn_big5_hkscs), &cnc_c32nrtomcn_big5_hkscs,
	     ztd_char32_t, ztd_char_t);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcsntoc32sn_big5_hkscs(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const char** __p_src)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcsnrtoc32sn_big5_hkscs(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcsnrtoc32sn_big5_hkscs(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const char** __p_src,
     cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_mcnrtoc32n_big5_hkscs), &cnc_mcnrtoc32n_big5_hkscs,
	     ztd_char_t, ztd_char32_t);
}
