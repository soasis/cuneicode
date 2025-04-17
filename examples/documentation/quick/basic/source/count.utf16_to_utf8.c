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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	const ztd_char16_t utf16_text[] = u"🥺🙏";

	const ztd_char16_t* p_count_input = utf16_text;
	// This size does NOT include the null terminating character.
	size_t count_input_size   = ztdc_c_string_array_size(utf16_text);
	cnc_mcstate_t count_state = { 0 };
	size_t output_size_after  = SIZE_MAX;
	// Use the function but with "nullptr" for the output pointer
	cnc_mcerr count_err = cnc_c16snrtoc8sn(
	     // To get the proper size for this conversion, we use the same
	     // function but with "NULL" specificers:
	     &output_size_after, NULL,
	     // input second
	     &count_input_size, &p_count_input,
	     // state parameter
	     &count_state);
	// Compute the needed space:
	const size_t output_size_needed = SIZE_MAX - output_size_after;
	if (count_err != cnc_mcerr_ok) {
		const char* err_str = cnc_mcerr_to_str(count_err);
		fprintf(stderr,
		     "An (unexpected) error occurred and the counting could not "
		     "happen! Error string: %s (code: '%d')\n",
		     err_str, (int)count_err);
		return 1;
	}

	ztd_char8_t* utf8_text = malloc(output_size_needed * sizeof(ztd_char8_t));

	// prepare for potential error return and error handling
	int return_value = 0;

	if (utf8_text == NULL) {
		return_value = 2;
		goto early_exit;
	}
	ztd_char8_t* p_output = utf8_text;
	cnc_mcstate_t state   = { 0 };

	// Now, actually output it
	const ztd_char16_t* p_input = utf16_text;
	// ztdc_c_array_size INCLUDES the null terminator in the size!
	size_t input_size  = ztdc_c_string_array_size(utf16_text);
	size_t output_size = output_size_needed;
	cnc_mcerr err      = cnc_c16snrtoc8sn(
          // output first
          &output_size, &p_output,
          // input second
          &input_size, &p_input,
          // state parameter
          &state);
	const size_t input_consumed
	     = ztdc_c_string_array_size(utf16_text) - input_size;
	const size_t output_written  = output_size_needed - output_size;
	const bool conversion_failed = err != cnc_mcerr_ok;
	if (conversion_failed) {
		// get error string to describe error code
		const char* err_str = cnc_mcerr_to_str(err);
		fprintf(stderr,
		     "An (unexpected) error occurred and the conversion could not "
		     "happen! The error occurred at UTF-16 input element #%zu, and only "
		     "managed to output %zu UTF-8 elements. Error string: %s (code: "
		     "'%d')\n",
		     input_consumed, output_written, err_str, (int)err);
		return_value = 3;
		goto early_exit;
	}
	// requires a capable terminal / output, but will be
	// UTF-8 text!
	printf("Converted UTF-8 text:\n");
	fwrite(utf8_text, sizeof(ztd_char8_t), output_written, stdout);
	printf("\n");

early_exit:
	if (utf8_text != NULL)
		free(utf8_text);

	return return_value;
}
