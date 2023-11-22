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

#include <ztd/cuneicode/result.h>

#define MAKE_GENERIC_TYPEDEFS(DECL_NAME)                                          \
	typedef DECL_NAME(char, char) ZTD_CONCAT_TOKENS_I_(t, __COUNTER__);          \
	typedef DECL_NAME(char, unsigned char) ZTD_CONCAT_TOKENS_I_(t, __COUNTER__); \
	typedef DECL_NAME(char, ztd_char_t) ZTD_CONCAT_TOKENS_I_(t, __COUNTER__);    \
	typedef DECL_NAME(char, ztd_wchar_t) ZTD_CONCAT_TOKENS_I_(t, __COUNTER__);   \
	typedef DECL_NAME(char, ztd_char8_t) ZTD_CONCAT_TOKENS_I_(t, __COUNTER__);   \
	typedef DECL_NAME(char, ztd_char16_t) ZTD_CONCAT_TOKENS_I_(t, __COUNTER__);  \
	typedef DECL_NAME(char, ztd_char32_t) ZTD_CONCAT_TOKENS_I_(t, __COUNTER__)

MAKE_GENERIC_TYPEDEFS(cnc_result);
MAKE_GENERIC_TYPEDEFS(cnc_error_result);
MAKE_GENERIC_TYPEDEFS(cnc_single_result);
MAKE_GENERIC_TYPEDEFS(cnc_single_error_result);
