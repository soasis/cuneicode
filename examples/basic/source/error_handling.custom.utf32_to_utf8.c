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
#include <stddef.h>

typedef struct first_error_at {
	const ztd_char32_t* first;
	size_t index;
} first_error_at;

cnc_mcerr my_error_handler(cnc_mcerr initial_error, size_t* restrict p_output_size,
     ztd_char8_t* restrict* restrict p_output_buffer,
     size_t* restrict p_input_size,
     const ztd_char32_t* restrict* restrict p_input_buffer, void* restrict p_state,
     void* restrict user_data) {
	(void)p_output_buffer;
	(void)p_output_size;
	(void)p_input_size;
	(void)p_state;
	first_error_at* p_first_error = (first_error_at*)user_data;
	if (p_first_error->first) {
		// return the error that triggered this to stop immediately.
		p_first_error->index = p_input_buffer[0] - p_first_error->first;
		p_first_error->first = NULL;
	}
	return cnc_skip_input_and_replace_error_c32ntoc8n(initial_error,
	     p_output_size, p_output_buffer, p_input_size, p_input_buffer, p_state,
	     user_data);
}

int main() {
	const ztd_char32_t input_data[] = U"Bark Bark Bark \xFFFFFFFF🐕‍🦺!";
	ztd_char8_t output_data[ztdc_c_array_size(input_data) * CNC_C8_MAX] = { 0 };
	cnc_mcstate_t state                                                 = { 0 };
	const size_t starting_input_size  = ztdc_c_string_array_size(input_data);
	size_t input_size                 = starting_input_size;
	const ztd_char32_t* input         = input_data;
	const size_t starting_output_size = ztdc_c_array_size(output_data);
	size_t output_size                = starting_output_size;
	ztd_char8_t* output               = output_data;

	first_error_at first_error    = { .first = input_data, .index = 0 };
	cnc_error_result_t err_result = cnc_cxsnrtocysn_with_handler(&output_size,
	     &output, &input_size, &input, &state, my_error_handler, &first_error);

	const size_t input_read     = starting_input_size - input_size;
	const size_t output_written = starting_output_size - output_size;
	const bool has_err          = err_result.error_code != cnc_mcerr_ok;
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
	     "\n\tWrote: %zu %zu-bit elements"
	     "\n\tTotal # of errors handled in input: %zu"
	     "\n\tFirst error at input data index: %zu\n",
	     input_read, (size_t)(sizeof(*input) * CHAR_BIT), output_written,
	     (size_t)(sizeof(*output) * CHAR_BIT), err_result.error_count,
	     first_error.index);
	fprintf(stdout, "%s Conversion Result:\n", has_err ? "Partial" : "Complete");
	fwrite(output_data, sizeof(*output_data), output_written, stdout);
	// the stream (may be) line-buffered, so make sure an extra "\n" is written
	// out this is actually critical for some forms of stdout/stderr mirrors; they
	// won't show the last line even if you manually call fflush(…) !
	fwrite("\n", sizeof(char), 1, stdout);
	return has_err ? 1 : 0;
}
