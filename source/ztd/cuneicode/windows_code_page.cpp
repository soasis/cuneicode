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

#include <ztd/cuneicode/windows_code_page.h>
#include <ztd/cuneicode/mcchar.h>
#include <ztd/cuneicode/max_output.h>
#include <ztd/cuneicode/detail/transcode.hpp>

#include <ztd/idk/size.hpp>
#include <ztd/idk/detail/windows.hpp>

#include <memory>
#include <climits>

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c32ntomcn_windows_code_page(size_t* __p_maybe_dst_len, char** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char32_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_c32nrtomcn_windows_code_page(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c32nrtomcn_windows_code_page(size_t* __p_maybe_dst_len, char** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char32_t** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
#if ZTD_IS_ON(ZTD_PLATFORM_WINDOWS)
	if (__p_src == nullptr || *__p_src == nullptr) {
		return cnc_mcerr_ok;
	}
	if (__p_src_len == nullptr || *__p_src_len == 0) {
		return cnc_mcerr_ok;
	}
	const bool __is_counting          = __p_maybe_dst == nullptr || __p_maybe_dst != nullptr;
	const bool __is_unbounded         = __p_maybe_dst_len == nullptr;
	const ztd_char32_t* __initial_src = *__p_src;
	size_t __initial_src_len          = *__p_src_len;
	ztd_wchar_t __intermediate_output[CNC_MWC_MAX];
	size_t __intermediate_output_initial_size    = ztd_c_array_size(__intermediate_output);
	size_t* __p_intermediate_output_initial_size = &__intermediate_output_initial_size;
	ztd_wchar_t* __p_intermediate_output         = __intermediate_output;
	const cnc_mcerr __intermediate_err = cnc_c32nrtomwcn(__p_intermediate_output_initial_size,
	     &__p_intermediate_output, __p_src_len, __p_src, __p_state);
	if (__intermediate_err != cnc_mcerr_ok) {
		return __intermediate_err;
	}
	const uint32_t __code_page_id   = cnc_mcstate_get_win32_code_page(__p_state);
	BOOL __p_used_default_character = false;
	const size_t __intermediate_size
	     = ztd_c_array_size(__intermediate_output) - __intermediate_output_initial_size;
	char __win32_blackhole_buffer[CNC_MC_MAX];
	char* __win32_dst         = __p_maybe_dst ? *__p_maybe_dst : __win32_blackhole_buffer;
	const int __win32_dst_len = static_cast<int>(__is_counting
	          ? (*__p_maybe_dst_len > static_cast<size_t>(INT_MAX) ? INT_MAX : *__p_maybe_dst_len)
	          : INT_MAX);
	for (size_t __allowed_size = 1; __allowed_size < __intermediate_size; ++__allowed_size) {
		const int __win32_err = ::WideCharToMultiByte(static_cast<UINT>(__code_page_id),
		     WC_ERR_INVALID_CHARS, __intermediate_output, static_cast<int>(__allowed_size),
		     __win32_dst, __win32_dst_len, nullptr, __p_used_default_character);
		if (__win32_err == 0) {
			if (::GetLastError() == ERROR_NO_UNICODE_TRANSLATION) {
				// we can break early: it was illegal stuff that can't translate
				return cnc_mcerr_invalid_sequence;
			}
			else {
				continue;
			}
		}
		else {
			// okay, it should be good
			if (!__is_unbounded) {
				if (__p_maybe_dst_len[0] < __win32_err) {
					__p_src[0]     = __initial_src;
					__p_src_len[0] = __initial_src_len;
					return cnc_mcerr_insufficient_output;
				}
				__p_maybe_dst_len[0] -= __win32_err;
			}
			if (!__is_counting) {
				__p_maybe_dst[0] += __win32_err;
			}
			return cnc_mcerr_ok;
		}
	}
	__p_src[0]     = __initial_src;
	__p_src_len[0] = __initial_src_len;
	return ::GetLastError() == ERROR_INSUFFICIENT_BUFFER ? cnc_mcerr_insufficient_output
	                                                     : cnc_mcerr_invalid_sequence;
#else
	(void)__p_maybe_dst_len;
	(void)__p_maybe_dst;
	(void)__p_src_len;
	(void)__p_src;
	(void)__p_state;
	return cnc_mcerr_invalid_sequence;
#endif
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcntoc32n_windows_code_page(size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcnrtoc32n_windows_code_page(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcnrtoc32n_windows_code_page(size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
#if ZTD_IS_ON(ZTD_PLATFORM_WINDOWS)
	if (__p_src == nullptr || *__p_src == nullptr) {
		return cnc_mcerr_ok;
	}
	if (__p_src_len == nullptr || *__p_src_len == 0) {
		return cnc_mcerr_ok;
	}
	const bool __is_counting       = __p_maybe_dst == nullptr || __p_maybe_dst != nullptr;
	const bool __is_unbounded      = __p_maybe_dst_len == nullptr;
	const char* __initial_src      = *__p_src;
	const size_t __initial_src_len = *__p_src_len;
	ztd_wchar_t __intermediate_output[CNC_MWC_MAX];
	size_t __intermediate_output_initial_size = ztd_c_array_size(__intermediate_output);
	size_t __intermediate_size                = 0;
	size_t __input_read_size                  = 1;
	ztd_wchar_t* __p_intermediate_output      = __intermediate_output;
	const uint32_t __code_page_id             = cnc_mcstate_get_win32_code_page(__p_state);
	for (; __input_read_size < __initial_src_len; ++__input_read_size) {
		if (__input_read_size > CNC_MC_MAX) {
			// can't do much else
			return cnc_mcerr_invalid_sequence;
		}
		const int __win32_err = ::MultiByteToWideChar(static_cast<UINT>(__code_page_id),
		     MB_ERR_INVALID_CHARS, __initial_src, static_cast<int>(__input_read_size),
		     __p_intermediate_output, __intermediate_output_initial_size);
		if (__win32_err == 0) {
			if (::GetLastError() == ERROR_NO_UNICODE_TRANSLATION) {
				return cnc_mcerr_invalid_sequence;
			}
		}
		else {
			__intermediate_size = static_cast<size_t>(__win32_err);
			break;
		}
	}
	cnc_mcerr __err = cnc_mwcnrtoc32n(__p_maybe_dst_len, __p_maybe_dst, &__intermediate_size,
	     &__p_intermediate_output, __p_state);
	if (__err != cnc_mcerr_ok) {
		return __err;
	}
	__p_src[0] += __input_read_size;
	__p_src_len[0] -= __input_read_size;
	return cnc_mcerr_ok;
#else
	(void)__p_maybe_dst_len;
	(void)__p_maybe_dst;
	(void)__p_src_len;
	(void)__p_src;
	(void)__p_state;
	return cnc_mcerr_invalid_sequence;
#endif
}



ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c32sntomcsn_windows_code_page(size_t* __p_maybe_dst_len, char** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char32_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_c32snrtomcsn_windows_code_page(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c32snrtomcsn_windows_code_page(size_t* __p_maybe_dst_len, char** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char32_t** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_c32nrtomcn_windows_code_page),
	     &cnc_c32nrtomcn_windows_code_page, ztd_char32_t, ztd_char_t);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcsntoc32sn_windows_code_page(size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcsnrtoc32sn_windows_code_page(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcsnrtoc32sn_windows_code_page(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const char** __p_src,
     cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_mcnrtoc32n_windows_code_page),
	     &cnc_mcnrtoc32n_windows_code_page, ztd_char_t, ztd_char32_t);
}



ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mwcntomcn_windows_code_page(size_t* __p_maybe_dst_len, char** __p_maybe_dst,
     size_t* __p_src_len, const ztd_wchar_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mwcnrtomcn_windows_code_page(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mwcnrtomcn_windows_code_page(size_t* __p_maybe_dst_len, char** __p_maybe_dst,
     size_t* __p_src_len, const ztd_wchar_t** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
#if ZTD_IS_ON(ZTD_PLATFORM_WINDOWS)
	if (__p_src == nullptr || *__p_src == nullptr) {
		return cnc_mcerr_ok;
	}
	if (__p_src_len == nullptr || *__p_src_len == 0) {
		return cnc_mcerr_ok;
	}
	const bool __is_counting         = __p_maybe_dst == nullptr || __p_maybe_dst != nullptr;
	const bool __is_unbounded        = __p_maybe_dst_len == nullptr;
	const ztd_wchar_t* __initial_src = *__p_src;
	const size_t __initial_src_len   = *__p_src_len;
	char __win32_blackhole_buffer[CNC_MC_MAX];
	char* __dst = __is_counting ? __win32_blackhole_buffer : *__p_maybe_dst;
	const size_t __dst_size
	     = __is_unbounded ? ztd_c_array_size(__win32_blackhole_buffer) : *__p_maybe_dst_len;
	const uint32_t __code_page_id = cnc_mcstate_get_win32_code_page(__p_state);
	BOOL __used_default_char      = false;
	for (size_t __input_read_size = 1; __input_read_size < __initial_src_len;
	     ++__input_read_size) {
		if (__input_read_size > CNC_MWC_MAX) {
			return cnc_mcerr_invalid_sequence;
		}
		const int __win32_err = ::WideCharToMultiByte(static_cast<UINT>(__code_page_id),
		     WC_ERR_INVALID_CHARS, __initial_src, static_cast<int>(__input_read_size), __dst,
		     static_cast<int>(__dst_size), nullptr, &__used_default_char);
		if (__win32_err == 0) {
			if (::GetLastError() == ERROR_NO_UNICODE_TRANSLATION) {
				return cnc_mcerr_invalid_sequence;
			}
		}
		else {
			if (!__is_unbounded) {
				if (__p_maybe_dst_len[0] < __win32_err) {
					return cnc_mcerr_insufficient_output;
				}
				__p_maybe_dst_len[0] -= static_cast<size_t>(__win32_err);
			}
			if (!__is_counting) {
				__p_maybe_dst[0] += static_cast<size_t>(__win32_err);
			}
			__p_src[0] += __input_read_size;
			__p_src_len[0] -= __input_read_size;
			return cnc_mcerr_ok;
		}
	}
	return cnc_mcerr_invalid_sequence;
#else
	(void)__p_maybe_dst_len;
	(void)__p_maybe_dst;
	(void)__p_src_len;
	(void)__p_src;
	(void)__p_state;
	return cnc_mcerr_invalid_sequence;
#endif
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcntomwcn_windows_code_page(size_t* __p_maybe_dst_len, ztd_wchar_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcnrtomwcn_windows_code_page(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcnrtomwcn_windows_code_page(size_t* __p_maybe_dst_len, ztd_wchar_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
#if ZTD_IS_ON(ZTD_PLATFORM_WINDOWS)
	if (__p_src == nullptr || *__p_src == nullptr) {
		return cnc_mcerr_ok;
	}
	if (__p_src_len == nullptr || *__p_src_len == 0) {
		return cnc_mcerr_ok;
	}
	const bool __is_counting       = __p_maybe_dst == nullptr || __p_maybe_dst != nullptr;
	const bool __is_unbounded      = __p_maybe_dst_len == nullptr;
	const char* __initial_src      = *__p_src;
	const size_t __initial_src_len = *__p_src_len;
	ztd_wchar_t __win32_blackhole_buffer[CNC_MWC_MAX];
	ztd_wchar_t* __dst = __is_counting ? __win32_blackhole_buffer : *__p_maybe_dst;
	const size_t __dst_size
	     = __is_unbounded ? ztd_c_array_size(__win32_blackhole_buffer) : *__p_maybe_dst_len;
	const uint32_t __code_page_id = cnc_mcstate_get_win32_code_page(__p_state);
	for (size_t __input_read_size = 1; __input_read_size < __initial_src_len;
	     ++__input_read_size) {
		if (__input_read_size > CNC_MC_MAX) {
			return cnc_mcerr_invalid_sequence;
		}
		const int __win32_err = ::MultiByteToWideChar(static_cast<UINT>(__code_page_id),
		     MB_ERR_INVALID_CHARS, __initial_src, static_cast<int>(__input_read_size), __dst,
		     static_cast<int>(__dst_size));
		if (__win32_err == 0) {
			if (::GetLastError() == ERROR_NO_UNICODE_TRANSLATION) {
				return cnc_mcerr_invalid_sequence;
			}
		}
		else {
			if (!__is_unbounded) {
				if (__p_maybe_dst_len[0] < __win32_err) {
					return cnc_mcerr_insufficient_output;
				}
				__p_maybe_dst_len[0] -= static_cast<size_t>(__win32_err);
			}
			if (!__is_counting) {
				__p_maybe_dst[0] += static_cast<size_t>(__win32_err);
			}
			__p_src[0] += __input_read_size;
			__p_src_len[0] -= __input_read_size;
			return cnc_mcerr_ok;
		}
	}
	return cnc_mcerr_invalid_sequence;
#else
	(void)__p_maybe_dst_len;
	(void)__p_maybe_dst;
	(void)__p_src_len;
	(void)__p_src;
	(void)__p_state;
	return cnc_mcerr_invalid_sequence;
#endif
}



ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mwcsntomcsn_windows_code_page(size_t* __p_maybe_dst_len, char** __p_maybe_dst,
     size_t* __p_src_len, const ztd_wchar_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mwcsnrtomcsn_windows_code_page(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mwcsnrtomcsn_windows_code_page(size_t* __p_maybe_dst_len, char** __p_maybe_dst,
     size_t* __p_src_len, const ztd_wchar_t** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_mwcnrtomcn_windows_code_page),
	     &cnc_mwcnrtomcn_windows_code_page, ztd_wchar_t, ztd_char_t);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcsntomwcsn_windows_code_page(size_t* __p_maybe_dst_len, ztd_wchar_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcsnrtomwcsn_windows_code_page(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcsnrtomwcsn_windows_code_page(size_t* __p_maybe_dst_len, ztd_wchar_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_mcnrtomwcn_windows_code_page),
	     &cnc_mcnrtomwcn_windows_code_page, ztd_char_t, ztd_wchar_t);
}



ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16ntomcn_windows_code_page(size_t* __p_maybe_dst_len, char** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char16_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_c16nrtomcn_windows_code_page(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16nrtomcn_windows_code_page(size_t* __p_maybe_dst_len, char** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char16_t** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	const ztd_wchar_t** __p_internal_src = reinterpret_cast<const ztd_wchar_t**>(__p_src);
	cnc_mcerr __err                      = cnc_mwcnrtomcn_windows_code_page(
          __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_internal_src, __p_state);
	__p_src = reinterpret_cast<const ztd_char16_t**>(__p_internal_src);
	return __err;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcntoc16n_windows_code_page(size_t* __p_maybe_dst_len, ztd_char16_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcnrtoc16n_windows_code_page(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcnrtoc16n_windows_code_page(size_t* __p_maybe_dst_len, ztd_char16_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	ztd_wchar_t** __p_maybe_internal_dst = reinterpret_cast<ztd_wchar_t**>(__p_maybe_dst);
	cnc_mcerr __err                      = cnc_mcnrtomwcn_windows_code_page(
          __p_maybe_dst_len, __p_maybe_internal_dst, __p_src_len, __p_src, __p_state);
	__p_maybe_dst = reinterpret_cast<ztd_char16_t**>(__p_maybe_internal_dst);
	return __err;
}



ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16sntomcsn_windows_code_page(size_t* __p_maybe_dst_len, char** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char16_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_c16snrtomcsn_windows_code_page(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16snrtomcsn_windows_code_page(size_t* __p_maybe_dst_len, char** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char16_t** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_c16nrtomcn_windows_code_page),
	     &cnc_c16nrtomcn_windows_code_page, ztd_char16_t, ztd_char_t);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcsntoc16sn_windows_code_page(size_t* __p_maybe_dst_len, ztd_char16_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return cnc_mcsnrtoc16sn_windows_code_page(
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcsnrtoc16sn_windows_code_page(size_t* __p_maybe_dst_len,
     ztd_char16_t** __p_maybe_dst, size_t* __p_src_len, const char** __p_src,
     cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	_ZTDC_CUNEICODE_TRANSCODE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, CNC_MC_MAX, decltype(&cnc_mcnrtoc16n_windows_code_page),
	     &cnc_mcnrtoc16n_windows_code_page, ztd_char_t, ztd_char16_t);
}
