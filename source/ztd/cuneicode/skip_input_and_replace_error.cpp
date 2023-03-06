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

#include <ztd/cuneicode/skip_input_and_replace_error.h>
#include <ztd/cuneicode/replace_error.h>
#include <ztd/cuneicode/detail/boilerplate.h>

#include <ztd/idk/encoding_detection.h>

ZTD_EXTERN_C_I_ cnc_skip_input_and_replace_error_cxntocyn_t
     cnc_skip_input_and_replace_error_cxntocyn
     = { 0 };

// to_mcn
ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c32ntomcn(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
	}
	return cnc_replace_error_c32ntomcn(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c32ntomcn_utf8(cnc_mcerr __current_error,
     size_t* __p_maybe_dst_len, ztd_char_t** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char32_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
	}
	return cnc_replace_error_c32ntomcn_utf8(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c16ntomcn(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char16_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
	}
	return cnc_replace_error_c16ntomcn(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c8ntomcn(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char8_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
	}
	return cnc_replace_error_c8ntomcn(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_mwcntomcn(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char_t** __p_maybe_dst, size_t* __p_src_len, const ztd_wchar_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		if (ztdc_is_wide_execution_encoding_utf8()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
		}
		else if (ztdc_is_wide_execution_encoding_utf16()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
		}
		else if (ztdc_is_wide_execution_encoding_utf32()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
		}
		else {
			_CNC_SKIP_INPUT_ERROR_IMPL_NONE(__p_src_len, __p_src);
		}
	}
	return cnc_replace_error_mwcntomcn(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_mcntomcn(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		if (ztdc_is_execution_encoding_utf8()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
		}
		else if (ztdc_is_execution_encoding_utf16()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
		}
		else if (ztdc_is_execution_encoding_utf32()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
		}
	}
	return cnc_replace_error_mcntomcn(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_mcntomcn_utf8(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		if (ztdc_is_execution_encoding_utf8()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
		}
		else if (ztdc_is_execution_encoding_utf16()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
		}
		else if (ztdc_is_execution_encoding_utf32()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
		}
	}
	return cnc_replace_error_mcntomcn(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

// to_mwcn
ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c32ntomwcn(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_wchar_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src,
     void* __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
	}
	return cnc_replace_error_c32ntomwcn(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c16ntomwcn(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_wchar_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char16_t** __p_src,
     void* __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
	}
	return cnc_replace_error_c16ntomwcn(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c8ntomwcn(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_wchar_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char8_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	(void)__p_src;
	(void)__p_src_len;
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
	}
	return cnc_replace_error_c8ntomwcn(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_mwcntomwcn(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_wchar_t** __p_maybe_dst, size_t* __p_src_len, const ztd_wchar_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		if (ztdc_is_wide_execution_encoding_utf8()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
		}
		else if (ztdc_is_wide_execution_encoding_utf16()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
		}
		else if (ztdc_is_wide_execution_encoding_utf32()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
		}
		else {
			_CNC_SKIP_INPUT_ERROR_IMPL_NONE(__p_src_len, __p_src);
		}
	}
	return cnc_replace_error_mwcntomwcn(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_mcntomwcn(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_wchar_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		if (ztdc_is_execution_encoding_utf8()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
		}
		else if (ztdc_is_execution_encoding_utf16()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
		}
		else if (ztdc_is_execution_encoding_utf32()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
		}
		else {
			_CNC_SKIP_INPUT_ERROR_IMPL_NONE(__p_src_len, __p_src);
		}
	}
	return cnc_replace_error_mcntomwcn(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

// to_c8n
ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c32ntoc8n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char8_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src,
     void* __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
	}
	return cnc_replace_error_c32ntoc8n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c16ntoc8n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char8_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char16_t** __p_src,
     void* __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
	}
	return cnc_replace_error_c16ntoc8n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c8ntoc8n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char8_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char8_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
	}
	return cnc_replace_error_c8ntoc8n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_mwcntoc8n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char8_t** __p_maybe_dst, size_t* __p_src_len, const ztd_wchar_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		if (ztdc_is_wide_execution_encoding_utf8()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
		}
		else if (ztdc_is_wide_execution_encoding_utf16()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
		}
		else if (ztdc_is_wide_execution_encoding_utf32()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
		}
		else {
			_CNC_SKIP_INPUT_ERROR_IMPL_NONE(__p_src_len, __p_src);
		}
	}
	return cnc_replace_error_mwcntoc8n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_mcntoc8n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char8_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		if (ztdc_is_execution_encoding_utf8()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
		}
		else if (ztdc_is_execution_encoding_utf16()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
		}
		else if (ztdc_is_execution_encoding_utf32()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
		}
		else {
			_CNC_SKIP_INPUT_ERROR_IMPL_NONE(__p_src_len, __p_src);
		}
	}
	return cnc_replace_error_mcntoc8n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

// to_c16n
ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c32ntoc16n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char16_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src,
     void* __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
	}
	return cnc_replace_error_c32ntoc16n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c16ntoc16n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char16_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char16_t** __p_src,
     void* __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
	}
	return cnc_replace_error_c16ntoc16n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c8ntoc16n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char16_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char8_t** __p_src,
     void* __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
	}
	return cnc_replace_error_c8ntoc16n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_mwcntoc16n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char16_t** __p_maybe_dst, size_t* __p_src_len, const ztd_wchar_t** __p_src,
     void* __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		if (ztdc_is_wide_execution_encoding_utf8()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
		}
		else if (ztdc_is_wide_execution_encoding_utf16()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
		}
		else if (ztdc_is_wide_execution_encoding_utf32()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
		}
		else {
			_CNC_SKIP_INPUT_ERROR_IMPL_NONE(__p_src_len, __p_src);
		}
	}
	return cnc_replace_error_mwcntoc16n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_mcntoc16n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char16_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		if (ztdc_is_execution_encoding_utf8()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
		}
		else if (ztdc_is_execution_encoding_utf16()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
		}
		else if (ztdc_is_execution_encoding_utf32()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
		}
		else {
			_CNC_SKIP_INPUT_ERROR_IMPL_NONE(__p_src_len, __p_src);
		}
	}
	return cnc_replace_error_mcntoc16n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

// to_c32n
ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c32ntoc32n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src,
     void* __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
	}
	return cnc_replace_error_c32ntoc32n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c16ntoc32n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char16_t** __p_src,
     void* __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
	}
	return cnc_replace_error_c16ntoc32n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_c8ntoc32n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char8_t** __p_src,
     void* __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
	}
	return cnc_replace_error_c8ntoc32n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_mwcntoc32n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const ztd_wchar_t** __p_src,
     void* __p_state, void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		if (ztdc_is_wide_execution_encoding_utf8()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
		}
		else if (ztdc_is_wide_execution_encoding_utf16()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
		}
		else if (ztdc_is_wide_execution_encoding_utf32()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
		}
		else {
			_CNC_SKIP_INPUT_ERROR_IMPL_NONE(__p_src_len, __p_src);
		}
	}
	return cnc_replace_error_mwcntoc32n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr
cnc_skip_input_and_replace_error_mcntoc32n(cnc_mcerr __current_error, size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char_t** __p_src, void* __p_state,
     void* restrict __p_user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__current_error != cnc_mcerr_incomplete_input) {
		_CNC_ERROR_INITIAL_SRC_CHECK_IMPL(__current_error, __p_src, __p_src_len);
		if (ztdc_is_execution_encoding_utf8()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C8(__p_src_len, __p_src);
		}
		else if (ztdc_is_execution_encoding_utf16()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C16(__p_src_len, __p_src);
		}
		else if (ztdc_is_execution_encoding_utf32()) {
			_CNC_SKIP_INPUT_ERROR_IMPL_C32(__p_src_len, __p_src);
		}
		else {
			_CNC_SKIP_INPUT_ERROR_IMPL_NONE(__p_src_len, __p_src);
		}
	}
	return cnc_replace_error_mcntoc32n(__current_error, __p_maybe_dst_len, __p_maybe_dst,
	     __p_src_len, __p_src, __p_state, __p_user_data);
}
