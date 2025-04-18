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

#include <ztd/cuneicode/mcchar.h>

#include <ztd/cuneicode/detail/err.hpp>
#include <ztd/cuneicode/detail/transcode.hpp>
#include <ztd/cuneicode/detail/core_mcharn.hpp>

#include <memory>

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16nrtomcn(size_t* __p_maybe_dst_len, char** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char16_t** __p_src, cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __substitute_state {};
	if (__p_state == nullptr)
		__p_state = &__substitute_state;
	_ZTDC_CUNEICODE_TRANSCODE_ONE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, decltype(&cnc_c16nrtoc32n), &cnc_c16nrtoc32n, decltype(&cnc_c32nrtomcn),
	     &cnc_c32nrtomcn);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16ntomcn(size_t* __p_maybe_dst_len, char** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char16_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return ::cnc_c16nrtomcn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16nrtomwcn(size_t* __p_maybe_dst_len, wchar_t** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char16_t** __p_src, cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __substitute_state {};
	if (__p_state == nullptr)
		__p_state = &__substitute_state;
	if (ztdc_is_wide_execution_encoding_utf16()) {
		return ::cnc_c16nrtoc16n(__p_maybe_dst_len,
		     reinterpret_cast<ztd_char16_t**>(__p_maybe_dst), __p_src_len, __p_src, __p_state);
	}
	else if (ztdc_is_wide_execution_encoding_utf32()) {
		return ::cnc_c16nrtoc32n(__p_maybe_dst_len,
		     reinterpret_cast<ztd_char32_t**>(__p_maybe_dst), __p_src_len, __p_src, __p_state);
	}
	_ZTDC_CUNEICODE_TRANSCODE_ONE_BODY(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src,
	     __p_state, decltype(&cnc_c16nrtoc32n), &cnc_c16nrtoc32n, decltype(&cnc_c32nrtomwcn),
	     &cnc_c32nrtomwcn);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16ntomwcn(size_t* __p_maybe_dst_len, wchar_t** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char16_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return ::cnc_c16nrtomwcn(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16nrtoc8n(size_t* __p_maybe_dst_len, ztd_char8_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char16_t** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __substitute_state {};
	if (__p_state == nullptr)
		__p_state = &__substitute_state;
	_ZTDC_CUNEICODE_SINGLE_N_DEST_TEMPLATE_BODY(::cnc::__cnc_detail::__c16nrtoc8n,
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16ntoc8n(size_t* __p_maybe_dst_len, ztd_char8_t** __p_maybe_dst, size_t* __p_src_len,
     const ztd_char16_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return ::cnc_c16nrtoc8n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16nrtoc16n(size_t* __p_maybe_dst_len, ztd_char16_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char16_t** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __substitute_state {};
	if (__p_state == nullptr)
		__p_state = &__substitute_state;
	_ZTDC_CUNEICODE_SINGLE_N_DEST_TEMPLATE_BODY(::cnc::__cnc_detail::__c16nrtoc16n,
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16ntoc16n(size_t* __p_maybe_dst_len, ztd_char16_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char16_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return ::cnc_c16nrtoc16n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16nrtoc32n(size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char16_t** __p_src, cnc_mcstate_t* __p_state)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __substitute_state {};
	if (__p_state == nullptr)
		__p_state = &__substitute_state;
	_ZTDC_CUNEICODE_SINGLE_N_DEST_TEMPLATE_BODY(::cnc::__cnc_detail::__c16nrtoc32n,
	     __p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_mcerr cnc_c16ntoc32n(size_t* __p_maybe_dst_len, ztd_char32_t** __p_maybe_dst,
     size_t* __p_src_len, const ztd_char16_t** __p_src) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_mcstate_t __state    = {};
	cnc_mcstate_t* __p_state = ::std::addressof(__state);
	return ::cnc_c16nrtoc32n(__p_maybe_dst_len, __p_maybe_dst, __p_src_len, __p_src, __p_state);
}
