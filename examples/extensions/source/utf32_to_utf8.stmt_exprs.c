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

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main() {
#if ZTD_IS_ON(ZTD_CUNEICODE_EXTENSION_FUNCTIONS)
	// This only works if we support extension functions!
	const ztd_char32_t input_data[] = U"Bark Bark Bark 🐕‍🦺!";
	char output_data[ztdc_c_array_size(input_data) * 4] = { 0 };
	cnc_mcstate_t state                                 = { 0 };
	// set the "do UB shit if invalid" bit to true
	cnc_mcstate_set_assume_valid(&state, true);
	cnc_c32mc_result_t result = cnc_cxsrtocysn_into(
	     ztdc_c_array_size(output_data), output_data, input_data, &state);
	const bool has_err = result.error_code != cnc_mcerr_ok;
	const char* const conversion_result_title_str = (has_err
	          ? "Conversion failed... \xF0\x9F\x98\xAD"   // UTF-8 bytes for 😭
	          : "Conversion succeeded \xF0\x9F\x8E\x89"); // UTF-8 bytes for 🎉
	const size_t conversion_result_title_str_size
	     = strlen(conversion_result_title_str);
	// Use fwrite to prevent conversions / locale-sensitive-probing from
	// fprintf family of functions
	fwrite(conversion_result_title_str, sizeof(*conversion_result_title_str),
	     conversion_result_title_str_size, has_err ? stderr : stdout);
	fprintf(has_err ? stderr : stdout,
	     "\n\tRead: %zu %zu-bit elements"
	     "\n\tWrote: %zu %zu-bit elements\n",
	     (size_t)(result.input_count), (size_t)(sizeof(*result.input) * CHAR_BIT),
	     (size_t)(result.output_count),
	     (size_t)(sizeof(*result.output) * CHAR_BIT));
	fprintf(stdout, "%s Conversion Result:\n", has_err ? "Partial" : "Complete");
	fwrite(result.output, sizeof(*result.output), result.output_count, stdout);
	// the stream (may be) line-buffered, so make sure an extra "\n" is written
	// out this is actually critical for some forms of stdout/stderr mirrors; they
	// won't show the last line even if you manually call fflush(…) !
	fwrite("\n", sizeof(char), 1, stdout);
	return has_err ? 1 : 0;
#else
	return 0;
#endif
}
