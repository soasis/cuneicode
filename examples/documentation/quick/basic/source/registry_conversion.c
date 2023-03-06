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
#include <string.h>
#include <stdlib.h>

int main() {

	cnc_conversion_registry* registry = NULL;
	{
		cnc_open_err err
		     = cnc_registry_new(&registry, cnc_registry_options_default);
		if (err != cnc_open_err_ok) {
			const char* err_str = cnc_open_err_to_str(err);
			fprintf(stderr,
			     "An unexpected error has occurred: '%s' (code: '%d')", err_str,
			     (int)err);
			return 1;
		}
	}

	// Now that we've allocated, have a return value
	// just in case
	int return_value     = 0;
	cnc_conversion* conv = NULL;
	{
		cnc_conversion_info conv_info = { 0 };
		cnc_open_err err              = cnc_conv_new(
               registry, "windows-1251", "utf-8", &conv, &conv_info);
		if (err != cnc_open_err_ok) {
			const char* err_str = cnc_open_err_to_str(err);
			fprintf(stderr, "An unexpected error has occurred: %s (code: '%d')",
			     err_str, (int)err);
			return_value = 2;
			goto early_exit0;
		}
		// the conversion info structure can tell us about things
		printf(
		     "Successfully opened a registry conversion between %.*s and "
		     "%.*s!\n",
		     (int)conv_info.from_code_size,
		     (const char*)conv_info.from_code_data, (int)conv_info.to_code_size,
		     (const char*)conv_info.to_code_data);
		if (conv_info.is_indirect) {
			// the strings used for this printf are UTF-8 encoded, but we
			// know the names are ASCII-compatible, charset-invariant strings
			// thanks to the request above, so we don't do the special
			// printing method.
			printf(
			     "(It is an indirect conversion, going from %.*s to %.*s, "
			     "then %.*s to %.*s for the conversion.)\n",
			     (int)conv_info.from_code_size,
			     (const char*)conv_info.from_code_data,
			     (int)conv_info.indirect_code_size,
			     (const char*)conv_info.indirect_code_data,
			     (int)conv_info.indirect_code_size,
			     (const char*)conv_info.indirect_code_data,
			     (int)conv_info.to_code_size,
			     (const char*)conv_info.to_code_data);
		}
		else {
			printf(
			     "(The conversion is a direct conversion and deos not take an "
			     "intermediate conversion path.)\n");
		}
	}

	const char input[]
	     = "\xd1\xeb\xe0\xe2\xe0\x20\xd3\xea\xf0\xe0\xbf\xed\xb3\x21\x0a";

	const unsigned char* count_input_last = (const unsigned char*)input;
	size_t count_input_byte_size_leftover = ztdc_c_array_size(input);
	size_t count_output_byte_size         = SIZE_MAX;

	const cnc_mcerr count_err = cnc_conv(conv, &count_output_byte_size, NULL,
	     &count_input_byte_size_leftover, &count_input_last);
	const size_t output_byte_size_needed = SIZE_MAX - count_output_byte_size;
	const size_t count_input_byte_size_consumed
	     = ztdc_c_array_size(input) - count_input_byte_size_leftover;
	if (count_err != cnc_mcerr_ok) {
		const char* err_str = cnc_mcerr_to_str(count_err);
		fprintf(stderr,
		     "The counting step failed with the error %s (code: "
		     "'%d') at byte #%zu in the input (which presently needs %zu bytes "
		     "of output space)",
		     err_str, (int)count_err, count_input_byte_size_consumed,
		     output_byte_size_needed);
		return_value = 3;
		goto early_exit1;
	}

	const unsigned char* input_last = (const unsigned char*)input;
	size_t input_size               = ztdc_c_array_size(input);
	// not strictly necessary to multiply by unsigned char since it's
	// defined to be 1, but it's consistent with other places where
	// malloc gets used...
	unsigned char* output
	     = malloc(output_byte_size_needed * sizeof(unsigned char));
	if (output == NULL) {
		return_value = 4;
		goto early_exit2;
	}
	unsigned char* output_last       = output;
	size_t output_byte_size_leftover = output_byte_size_needed;
	cnc_mcerr err = cnc_conv(conv, &output_byte_size_leftover, &output_last,
	     &input_size, &input_last);
	const size_t output_byte_size_written
	     = output_byte_size_needed - output_byte_size_leftover;
	const size_t input_byte_size_consumed
	     = ztdc_c_array_size(input) - count_input_byte_size_leftover;
	if (err != cnc_mcerr_ok) {
		const char* err_str = cnc_mcerr_to_str(err);
		fprintf(stderr,
		     "The conversion step failed to convert with the error %s "
		     "(code: '%d') at byte #%zu in the input after writing as far as "
		     "byte #%zu in the output",
		     err_str, (int)count_err, input_byte_size_consumed,
		     output_byte_size_written);
		return_value = 5;
		goto early_exit2;
	}

	printf(
	     "The registry conversion was successful, writing %zu output bytes after "
	     "reading %zu input bytes.\nThe output is:\n",
	     output_byte_size_written, input_byte_size_consumed);
	// It's UTF-8: this should print correctly on a UTF-8 capable terminal.
	// We do not mill through `printf` because it can do a (potentially lossy)
	// conversion.
	fwrite((const char*)output, sizeof(char), output_byte_size_written, stdout);
	printf("\n");

early_exit2:
	if (output != NULL) {
		free(output);
	}
early_exit1:
	if (conv != NULL) {
		cnc_conv_delete(conv);
	}
early_exit0:
	if (registry != NULL) {
		cnc_registry_delete(registry);
	}
	return return_value;
}
