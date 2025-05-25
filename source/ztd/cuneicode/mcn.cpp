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
// See the License for the specific language governing 141 and
// limitations under the License.
//
// ========================================================================= //

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/mcchar.h>

#include <ztd/cuneicode/detail/transcode.hpp>
#include <ztd/cuneicode/detail/core_mcharn.hpp>

#include <memory>

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcnrtomcn(size_t* __p_maybe_dst_len, char** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src, cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if constexpr ((sizeof(ztd_char_t) == sizeof(ztd_char8_t))
	     && (alignof(ztd_char_t) == alignof(ztd_char8_t))) {
		if (ztdc_is_execution_encoding_utf8()) {
			return ::cnc_c8nrtoc8n(__p_maybe_dst_len,
			     reinterpret_cast<ztd_char8_t**>(__p_maybe_dst), __p_src_len,
			     reinterpret_cast<const ztd_char8_t**>(__p_src), __p_state);
		}
	}
	cnc_mcstate_t __substitute_state {};
	if (__p_state == nullptr)
		__p_state = &__substitute_state;
	_ZTDC_CUNEICODE_TRANSCODE_ONE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, decltype(&cnc_mcnrtoc32n), &cnc_mcnrtoc32n, decltype(&cnc_c32nrtomcn),
	     &cnc_c32nrtomcn);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcntomcn(size_t* __p_maybe_dst_len, char** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return ::cnc_mcnrtomcn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcnrtomwcn(size_t* __p_maybe_dst_len, wchar_t** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src, cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __substitute_state {};
	if (__p_state == nullptr)
		__p_state = &__substitute_state;
	_ZTDC_CUNEICODE_SINGLE_N_DEST_TEMPLATE_BODY(::cnc::__cnc_detail::__mcnrtomwcn,
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcntomwcn(size_t* __p_maybe_dst_len, wchar_t** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return ::cnc_mcnrtomwcn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcnrtoc8n(size_t* __p_maybe_dst_len, ztd_char8_t** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src, cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if constexpr ((sizeof(ztd_char_t) == sizeof(ztd_char8_t))
	     && (alignof(ztd_char_t) == alignof(ztd_char8_t))) {
		if (ztdc_is_execution_encoding_utf8()) {
			return ::cnc_c8nrtoc8n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,
			     reinterpret_cast<const ztd_char8_t**>(__p_src), __p_state);
		}
	}
	cnc_mcstate_t __substitute_state {};
	if (__p_state == nullptr)
		__p_state = &__substitute_state;
	_ZTDC_CUNEICODE_TRANSCODE_ONE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, decltype(&cnc_mcnrtoc32n), &cnc_mcnrtoc32n, decltype(&cnc_c32nrtoc8n),
	     &cnc_c32nrtoc8n);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcntoc8n(size_t* __p_maybe_dst_len, ztd_char8_t** __p_maybe_dst, size_t* __p_src_len,
     const char** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return ::cnc_mcnrtoc8n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcnrtoc16n(size_t* __p_maybe_dst_len, ztd_char16_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if constexpr ((sizeof(ztd_wchar_t) == sizeof(ztd_char16_t))
	     && (alignof(ztd_wchar_t) == alignof(ztd_char16_t)) && ZTD_IS_ON(ZTD_PLATFORM_WINDOWS)) {
		return ::cnc_mcnrtomwcn(__p_maybe_dst_len, reinterpret_cast<ztd_wchar_t**>(__p_maybe_dst),
		     __p_src_len, __p_src, __p_state);
	}
	else {
		if constexpr ((sizeof(ztd_char_t) == sizeof(ztd_char8_t))
		     && (alignof(ztd_char_t) == alignof(ztd_char8_t))) {
			if (ztdc_is_execution_encoding_utf8()) {
				return ::cnc_c8nrtoc16n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len,
				     reinterpret_cast<const ztd_char8_t**>(__p_src), __p_state);
			}
		}
		cnc_mcstate_t __substitute_state {};
		if (__p_state == nullptr)
			__p_state = &__substitute_state;
		_ZTDC_CUNEICODE_TRANSCODE_ONE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
		     __p_state, decltype(&cnc_mcnrtoc32n), &cnc_mcnrtoc32n, decltype(&cnc_c32nrtoc16n),
		     &cnc_c32nrtoc16n);
	}
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcntoc16n(size_t* __p_maybe_dst_len, ztd_char16_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return ::cnc_mcnrtoc16n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcnrtoc32n(size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __substitute_state {};
	if (__p_state == nullptr)
		__p_state = &__substitute_state;
	_ZTDC_CUNEICODE_SINGLE_N_DEST_TEMPLATE_BODY(::cnc::__cnc_detail::__mcnrtoc32n,
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_mcntoc32n(size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst,
     size_t* __p_src_len, const char** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return ::cnc_mcnrtoc32n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}
