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

#include <ztd/cuneicode.h>

#include <ztd/idk/size.h>

#include <stdbool.h>
#include <stddef.h>
#include <limits.h>

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
#if ZTD_IS_ON(ZTD_CUNEICODE_EXTENSION_FUNCTIONS)
	// This only works if we support extension functions!
	const ztd_char8_t str[]
	     = u8"\"Saw a \U0001F9DC \u2014"
	       u8"didn't catch her\u2026 \U0001F61E\"\n\t- Sniff";
	const size_t str_n = ztdc_c_array_size(str);

	if (!cnc_cxsntocysn_into_is_valid(c16, str_n, str)) {
		// input not valid
		return 1;
	}

	ztd_char16_t utf16_str[CNC_C16_MAX * ztdc_c_array_size(str)];
	const size_t utf16_str_max_size = ztdc_c_array_size(utf16_str);
	cnc_count_result_t utf16_str_count_result
	     = cnc_cxsntocysn_into_count(c16, str_n, str);
	if (utf16_str_max_size < utf16_str_count_result.output_count) {
		// buffer too small
		return 2;
	}
	cnc_c8c16_result_t unbounded_result
	     = cnc_cxsntocys_into_unbounded(utf16_str, str_n, str);
	if (unbounded_result.error_code != cnc_mcerr_ok) {
		// write failed
		return 3;
	}
	return 0;
#else
	return 0;
#endif
}
