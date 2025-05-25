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

#ifndef ZTD_CUNEICODE_IO_H
#define ZTD_CUNEICODE_IO_H

#pragma once

#include <ztd/cuneicode/version.h>

#include <ztd/idk/charN_t.h>
#include <ztd/idk/size.h>
#include <ztd/idk/generic.h>

#if ZTD_IS_ON(ZTD_CXX)
#include <cstdio>
#include <cstdint>
#include <cstddef>
#else
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#endif

ZTD_EXTERN_C_OPEN_I_

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_mcn(FILE* __file, size_t __str_n, const char* __str)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_mc(FILE* __file, const char* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_mcn(size_t __str_n, const char* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_mc(const char* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_mwcn(FILE* __file, size_t __str_n, const ztd_wchar_t* __str)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_mwc(FILE* __file, const ztd_wchar_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_mwcn(size_t __str_n, const ztd_wchar_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_mwc(const ztd_wchar_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_c8n(FILE* __file, size_t __str_n, const ztd_char8_t* __str)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_c8(FILE* __file, const ztd_char8_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_c8n(size_t __str_n, const ztd_char8_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_c8(const ztd_char8_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_c16n(FILE* __file, size_t __str_n, const ztd_char16_t* __str)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_c16(FILE* __file, const ztd_char16_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_c16n(size_t __str_n, const ztd_char16_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_c16(const ztd_char16_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_c32n(FILE* __file, size_t __str_n, const ztd_char32_t* __str)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_c32(FILE* __file, const ztd_char32_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_c32n(size_t __str_n, const ztd_char32_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_c32(const ztd_char32_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

ZTD_EXTERN_C_CLOSE_I_

#define cnc_fprint_strn(_FILE, _STR_SIZE, _STR)                                           \
	_ZTDC_CASCADING_GENERIC(char, cnc_fprint_str_mcn(_FILE, _STR), ztd_wchar_t,          \
	     cnc_fprint_str_mwcn(_FILE, _STR), ztd_char8_t, cnc_fprint_str_c8n(_FILE, _STR), \
	     ztd_char16_t, cnc_fprint_str_c16n(_FILE, _STR), ztd_char32_t,                   \
	     cnc_fprint_str_c32n(_FILE, _STR))
#define cnc_fprint_str(_FILE, _STR)                                                     \
	_ZTDC_CASCADING_GENERIC(char, cnc_fprint_str_mc(_FILE, _STR), ztd_wchar_t,         \
	     cnc_fprint_str_mwc(_FILE, _STR), ztd_char8_t, cnc_fprint_str_c8(_FILE, _STR), \
	     ztd_char16_t, cnc_fprint_str_c16(_FILE, _STR), ztd_char32_t,                  \
	     cnc_fprint_str_c32(_FILE, _STR))
#define cnc_print_strn(...) cnc_fprint_strn(stdout, __VA_ARGS__)
#define cnc_print_str(...) cnc_fprint_str(stdout, __VA_ARGS__)

#endif
