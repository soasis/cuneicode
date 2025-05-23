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

#define _CRT_SECURE_NO_WARNINGS

#include <ztd/cuneicode.h>

#include <ztd/idk/size.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#if ZTD_IS_ON(ZTD_PLATFORM_WINDOWS)

int main(int argc, char* argv[]) {
	const char* const failed_conversion_result_title_str
	     = "Conversion failed... \xF0\x9F\x98\xAD"; // UTF-8 bytes for 😭
	const char* const success_conversion_result_title_str
	     = "Conversion succeeded \xF0\x9F\x8E\x89"; // UTF-8 bytes for 🎉
	const size_t success_conversion_result_title_str_size
	     = strlen(success_conversion_result_title_str);
	const size_t failed_conversion_result_title_str_size
	     = strlen(failed_conversion_result_title_str);
	(void)argc;
	(void)argv;
	const ztd_char32_t input_data[]
	     = U"ସମସ୍ତ ମନୁଷ୍ୟ ଜନ୍ମକାଳରୁ ସ୍ଧୀନ ଏବଂ ମର୍ଯ୍ୟାଦା ଓ ଅଧିକାରରେ ସମାନ. ସେମାନଙ୍କଠାରେ ବୁଦ୍ଧି "
	       U"ଆଉ ବିବେକ ନିହିତ ଅଛି ଏବଂ ସେମାନଙ୍କୁ ପରସ୍ପର ପ୍ରତି ଭ୍ରାତୃତ୍ ମନୋଭାବରେ ବ୍ୟବହାର କରିବା ଉଚିତ୍";
	const uint32_t win32_odia_code_page                                = 57007u;
	char intermediate_data[ztdc_c_array_size(input_data) * CNC_MC_MAX] = { 0 };
	cnc_mcstate_t intermediate_state                                   = { 0 };

	cnc_mcstate_set_win32_code_page(&intermediate_state, win32_odia_code_page);
	const size_t starting_input_size = ztdc_c_string_array_size(input_data);
	size_t input_size                = starting_input_size;
	const ztd_char32_t* input        = input_data;
	const size_t starting_intermediate_size
	     = ztdc_c_array_size(intermediate_data);
	size_t intermediate_size = starting_intermediate_size;
	char* intermediate       = intermediate_data;
	cnc_mcerr intermediate_err
	     = cnc_c32snrtomcsn_windows_code_page(&intermediate_size, &intermediate,
	          &input_size, &input, &intermediate_state);
	const size_t input_read = starting_input_size - input_size;
	const size_t intermediate_written
	     = starting_intermediate_size - intermediate_size;

	if (intermediate_err != cnc_mcerr_ok) {
		// failed initial conversion
		// Use fwrite to prevent conversions / locale-sensitive-probing from
		// fprintf family of functions
		fprintf(stderr, "Intermediate conversion:\n");
		fwrite(failed_conversion_result_title_str,
		     sizeof(*failed_conversion_result_title_str),
		     failed_conversion_result_title_str_size, stderr);
		fprintf(stderr,
		     "\n\tRead: %zu %zu-bit elements"
		     "\n\tWrote: %zu %zu-bit elements\n\n",
		     (size_t)(input_read), (size_t)(sizeof(*input) * CHAR_BIT),
		     (size_t)(intermediate_written),
		     (size_t)(sizeof(*intermediate) * CHAR_BIT));
		return 1;
	}
	else {
		fprintf(stdout, "Intermediate conversion:\n");
		fwrite(success_conversion_result_title_str,
		     sizeof(*success_conversion_result_title_str),
		     success_conversion_result_title_str_size, stdout);
		fprintf(stdout,
		     "\n\tRead: %zu %zu-bit elements"
		     "\n\tWrote: %zu %zu-bit elements\n\n",
		     (size_t)(input_read), (size_t)(sizeof(*input) * CHAR_BIT),
		     (size_t)(intermediate_written),
		     (size_t)(sizeof(*intermediate) * CHAR_BIT));
	}

	ztd_char32_t output_data[ztdc_c_array_size(input_data)] = { 0 };
	cnc_mcstate_t state                                     = { 0 };
	cnc_mcstate_set_win32_code_page(&state, win32_odia_code_page);
	size_t intermediate_input_size    = intermediate_written;
	const char* intermediate_input    = intermediate_data;
	const size_t starting_output_size = ztdc_c_array_size(output_data);
	size_t output_size                = starting_output_size;
	ztd_char32_t* output              = output_data;
	cnc_mcerr err = cnc_mcsnrtoc32sn_windows_code_page(&output_size, &output,
	     &intermediate_input_size, &intermediate_input, &state);
	const size_t intermediate_read
	     = intermediate_written - intermediate_input_size;
	const size_t output_written = starting_output_size - output_size;
	if (err != cnc_mcerr_ok) {
		// failed back to the original
		fprintf(stderr, "Output conversion:\n");
		fwrite(failed_conversion_result_title_str,
		     sizeof(*failed_conversion_result_title_str),
		     failed_conversion_result_title_str_size, stderr);
		fprintf(stderr,
		     "\n\tRead: %zu %zu-bit elements"
		     "\n\tWrote: %zu %zu-bit elements\n\n",
		     (size_t)(intermediate_read),
		     (size_t)(sizeof(*intermediate) * CHAR_BIT),
		     (size_t)(output_written), (size_t)(sizeof(*output) * CHAR_BIT));
		return 2;
	}
	else {
		fprintf(stdout, "Output conversion:\n");
		fwrite(success_conversion_result_title_str,
		     sizeof(*success_conversion_result_title_str),
		     success_conversion_result_title_str_size, stdout);
		fprintf(stdout,
		     "\n\tRead: %zu %zu-bit elements"
		     "\n\tWrote: %zu %zu-bit elements\n\n",
		     (size_t)(intermediate_read),
		     (size_t)(sizeof(*intermediate) * CHAR_BIT),
		     (size_t)(output_written), (size_t)(sizeof(*output) * CHAR_BIT));
	}

	return 0;
}

#else

int main(int argc, char* argv[]) {
	// does not work on this platform
	(void)argc;
	(void)argv;
	return 0;
}

#endif
