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

#include <my_conversion.h>

#include <ztd/idk/charN_t.h>

#include <stdio.h>

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	const char input[]              = "abc...def.''ghi'''..";
	const char* input_ptr           = input;
	const size_t initial_input_size = ztdc_c_array_size(input);
	size_t input_size               = initial_input_size;

	ztd_char32_t output[C32_MAX_MY * ztdc_c_array_size(input)] = { 0 };
	const ztd_char32_t expected_output[] = U"abc\u2026def.\u2033ghi\u2034..";
	ztd_char32_t* output_ptr             = output;
	const size_t initial_output_size     = ztdc_c_array_size(output);
	size_t output_size                   = initial_output_size;
	my_state state                       = { 0 };

	fprintf(stderr, "Original text:\n%s\n\n", input);

	const cnc_mcerr err = my_str_to_c32_str(
	     &output_size, &output_ptr, &input_size, &input_ptr, &state);
	const size_t input_read     = initial_input_size - input_size;
	const size_t output_written = initial_output_size - output_size;
	if (err != cnc_mcerr_ok) {
		fprintf(stderr, "The conversion failed with an unexpected error of %s.",
		     cnc_mcerr_to_str(err));
		return 1;
	}

	if (memcmp(output, expected_output, output_written) != 0) {
		fprintf(stderr,
		     "The output was not the same as the expected output string.");
		return 2;
	}

	{
		// convert to UTF-8 encoding to print.
		const ztd_char32_t* print_input_ptr = output;
		size_t print_input_size             = output_written;
		ztd_char8_t print_output[ztdc_c_array_size(output) * CNC_C8_MAX] = { 0 };
		ztd_char8_t* print_output_ptr = print_output;
		cnc_mcerr print_err           = cnc_c32sntoc8sn(
               NULL, &print_output_ptr, &print_input_size, &print_input_ptr);
		if (print_err != cnc_mcerr_ok) {
			fprintf(stderr,
			     "The conversion to print failed with an unexpected error of "
			     "%s.",
			     cnc_mcerr_to_str(err));
			return 3;
		}
		fprintf(stdout,
		     "my encoding to UTF-32:\n-----\nRead: %zu "
		     "units\nWrote: %zu "
		     "units\nWritten value: %s\n\n",
		     input_read, output_written, (const char*)print_output);
	}

	// and convert back, expecting the exact same input
	char roundtrip_output[MC_MAX_MY * ztdc_c_array_size(output)] = { 0 };
	const char expected_roundtrip_output[]  = "abc...def.''ghi'''..";
	const ztd_char32_t* roundtrip_input_ptr = output;
	size_t roundtrip_input_size             = output_written;
	char* roundtrip_output_ptr              = roundtrip_output;
	const size_t initial_roundtrip_output_size
	     = ztdc_c_array_size(roundtrip_output);
	size_t roundtrip_output_size = initial_roundtrip_output_size;
	my_state roundtrip_state     = { 0 };

	const cnc_mcerr roundtrip_err
	     = c32_str_to_my_str(&roundtrip_output_size, &roundtrip_output_ptr,
	          &roundtrip_input_size, &roundtrip_input_ptr, &roundtrip_state);
	const size_t roundtrip_input_read
	     = initial_output_size - roundtrip_input_size;
	const size_t roundtrip_output_written
	     = initial_roundtrip_output_size - roundtrip_output_size;
	if (roundtrip_err != cnc_mcerr_ok) {
		fprintf(stderr, "The conversion failed with an unexpected error of %s.",
		     cnc_mcerr_to_str(err));
		return 4;
	}

	if (memcmp(roundtrip_output, expected_roundtrip_output,
	         roundtrip_output_written)
	     != 0) {
		fprintf(stderr,
		     "The roundtrip output was not the same as its expected string.");
		return 5;
	}

	fprintf(stdout,
	     "UTF-32 to my encoding (roundtrip):\n-----\nRead: %zu units\nWrote: %zu "
	     "units\nWritten value: %s\n",
	     roundtrip_input_read, roundtrip_output_written,
	     (const char*)roundtrip_output);

	return 0;
}
