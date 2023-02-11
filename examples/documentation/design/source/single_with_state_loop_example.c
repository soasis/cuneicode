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
#include <ztd/idk/restrict.h>

#include <stdio.h>
#include <string.h>
#include <stddef.h>

cnc_mcerr my_bulk_mcsnrtoc8sn(size_t* output_size, unsigned char** output,
     size_t* input_size, const char** restrict input, cnc_mcstate_t* state) {
	cnc_mcstate_t invocation_unique_internal_state;
	if (state == NULL) {
		invocation_unique_internal_state = (cnc_mcstate_t) { 0 };
		state                            = &invocation_unique_internal_state;
	}
	if (input == NULL || *input == NULL) {
		return cnc_mcnrtoc8n(output_size, output, input_size, input, state);
	}
	for (;;) {
		cnc_mcerr err
		     = cnc_mcnrtoc8n(output_size, output, input_size, input, state);
		if (err != cnc_mcerr_ok) {
			return err;
		}
		if (*input_size > 0) {
			continue;
		}
		bool state_finished = cnc_mcstate_is_complete(state);
		if (!state_finished) {
			continue;
		}
		return err;
	}
}

int main() {
	const char input[]              = "abc";
	const char* input_ptr           = input;
	const size_t initial_input_size = ztdc_c_array_size(input);
	size_t input_size               = initial_input_size;

	unsigned char output[CNC_C8_MAX * ztdc_c_array_size(input)] = { 0 };
	unsigned char* output_ptr                                  = output;
	const size_t initial_output_size = ztdc_c_array_size(output);
	size_t output_size               = initial_output_size;

	cnc_mcerr err = my_bulk_mcsnrtoc8sn(
	     &output_size, &output_ptr, &input_size, &input_ptr, NULL);
	const size_t output_written = initial_output_size - output_size;
	const size_t input_read     = initial_input_size - input_size;
	if (err != cnc_mcerr_ok) {
		fprintf(stderr, "The conversion failed with an unexpected error of %s.",
		     cnc_mcerr_to_str(err));
		return 1;
	}

	const unsigned char expected_output[] = "abc";

	if (memcmp(output, expected_output, output_written) != 0) {
		fprintf(stderr,
		     "The expected intput was not the same as the expected output.");
		return 2;
	}

	fprintf(stdout, "Read: %zu units\nWrote: %zu units\nWritten value: %s\n",
	     input_read, output_written, (const char*)output);

	return 0;
}
