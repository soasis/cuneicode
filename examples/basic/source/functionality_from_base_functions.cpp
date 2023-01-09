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

#include <ztd/cuneicode.h>

#include <ztd/idk/size.h>

bool is_valid_utf16_from_utf8(size_t str_n, const ztd_char8_t str[]) {
	cnc_mcstate_t state = {};
	cnc_mcerr err = cnc_c8snrtoc16sn(nullptr, nullptr, &str_n, &str, &state);
	return err == cnc_mcerr_ok;
}

size_t count_utf16_from_utf8(size_t str_n, const ztd_char8_t str[]) {
	cnc_mcstate_t state         = {};
	const size_t utf16_before_n = SIZE_MAX;
	size_t utf16_after_n        = utf16_before_n;
	cnc_mcerr err
	     = cnc_c8snrtoc16sn(&utf16_after_n, nullptr, &str_n, &str, &state);
	return err == cnc_mcerr_ok ? utf16_before_n - utf16_after_n : 0;
}

bool unbounded_conversion_utf16_from_utf8(
     size_t str_n, const ztd_char8_t str[], char16_t* dest_str) {
	cnc_mcstate_t state = {};
	cnc_mcerr err = cnc_c8snrtoc16sn(nullptr, &dest_str, &str_n, &str, &state);
	return err == cnc_mcerr_ok;
}

int main() {
	constexpr const ztd_char8_t str[]
	     = u8"\"Saw a \U0001F9DC \u2014"
	       u8"didn't catch her\u2026 \U0001F61E\"\n\t- Sniff";
	constexpr const size_t str_n        = ztd_c_array_size(str);
	constexpr size_t utf16_str_max_size = CNC_C16_MAX * ztd_c_array_size(str);

	if (!is_valid_utf16_from_utf8(str_n, str)) {
		// input not valid
		return 1;
	}

	size_t utf16_str_n = count_utf16_from_utf8(str_n, str);
	char16_t utf16_str[utf16_str_max_size];
	if (utf16_str_max_size < utf16_str_n) {
		// buffer too small
		return 2;
	}

	if (!unbounded_conversion_utf16_from_utf8(str_n, str, utf16_str)) {
		// write failed
		return 3;
	}
	return 0;
}
