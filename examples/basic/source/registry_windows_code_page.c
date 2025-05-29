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
#include <ztd/cuneicode/io.h>

#include <ztd/idk/size.h>

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#if ZTD_IS_ON(ZTD_PLATFORM_WINDOWS)

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;

	cnc_conversion_registry* registry = NULL;
	{
		cnc_open_err err
		     = cnc_registry_new(&registry, cnc_registry_options_default);
		if (err != cnc_open_err_ok) {
			fprintf(stderr, "[error] could not open a new registry");
			return 1;
		}
	}

	cnc_conversion* conversion          = NULL;
	cnc_conversion_info conversion_info = { 0 };
	{
		cnc_open_err err = cnc_conv_new(registry, "utf-8",
		     "win32-code_page-57007", &conversion, &conversion_info);
		if (err != cnc_open_err_ok) {
			fprintf(stderr, "[error] could not open a new registry conversion");
			cnc_registry_delete(registry);
			return 2;
		}
	}

	cnc_conversion* conversion_reverse          = NULL;
	cnc_conversion_info conversion_reverse_info = { 0 };
	{
		cnc_open_err err = cnc_conv_new(registry, "win32-code_page-57007",
		     "utf-8", &conversion_reverse, &conversion_reverse_info);
		if (err != cnc_open_err_ok) {
			fprintf(stderr, "[error] could not open a new registry conversion");
			cnc_conv_delete(conversion);
			cnc_registry_delete(registry);
			return 3;
		}
	}

	fprintf(stdout, "Opened a conversion from \"");
	fwrite(conversion_info.from_code_data,
	     sizeof(*conversion_info.from_code_data), conversion_info.from_code_size,
	     stdout);
	fprintf(stdout, "\" to \"");
	fwrite(conversion_info.to_code_data, sizeof(*conversion_info.to_code_data),
	     conversion_info.to_code_size, stdout);
	if (conversion_info.is_indirect) {
		fprintf(stdout, "\" (through \"");
		fwrite(conversion_info.indirect_code_data,
		     sizeof(*conversion_info.indirect_code_data),
		     conversion_info.indirect_code_size, stdout);
		fprintf(stdout, "\")");
	}
	else {
		fprintf(stdout, "\"");
	}
	fprintf(stdout, "\n");

	const char* const failed_conversion_result_title_str
	     = "Conversion failed... \xF0\x9F\x98\xAD"; // UTF-8 bytes for 😭
	const char* const success_conversion_result_title_str
	     = "Conversion succeeded \xF0\x9F\x8E\x89"; // UTF-8 bytes for 🎉
	const size_t success_conversion_result_title_str_size
	     = strlen(success_conversion_result_title_str);
	const size_t failed_conversion_result_title_str_size
	     = strlen(failed_conversion_result_title_str);

	const ztd_char32_t input_data[] = U"ଜତି, ଭଷ ଓ ସାହିତ୍ୟର ପରିଚୟ ଏବଂ ଏକ ସଂଗେ ଯୋଡ";

	char intermediate_data[ztdc_c_array_size(input_data) * CNC_MC_MAX] = { 0 };
	const size_t starting_input_byte_size = sizeof(input_data);
	size_t input_byte_size                = starting_input_byte_size;
	const unsigned char* input_bytes      = (const unsigned char*)input_data;
	const size_t starting_intermediate_byte_size = sizeof(intermediate_data);
	size_t intermediate_byte_size     = starting_intermediate_byte_size;
	unsigned char* intermediate_bytes = (unsigned char*)intermediate_data;
	cnc_mcerr intermediate_err    = cnc_conv(conversion, &intermediate_byte_size,
	        &intermediate_bytes, &input_byte_size, &input_bytes);
	const size_t input_bytes_read = starting_input_byte_size - input_byte_size;
	const size_t intermediate_bytes_written
	     = starting_intermediate_byte_size - intermediate_byte_size;

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
		     (size_t)(input_bytes_read / sizeof(*input_data)),
		     (size_t)(sizeof(*input_data) * CHAR_BIT),
		     (size_t)(intermediate_bytes_written),
		     (size_t)(sizeof(*intermediate_data) * CHAR_BIT));

		cnc_conv_delete(conversion);
		cnc_conv_delete(conversion_reverse);
		cnc_registry_delete(registry);
		return 5;
	}
	else {
		fprintf(stdout, "Intermediate conversion:\n");
		fwrite(success_conversion_result_title_str,
		     sizeof(*success_conversion_result_title_str),
		     success_conversion_result_title_str_size, stdout);
		fprintf(stdout,
		     "\n\tRead: %zu %zu-bit elements"
		     "\n\tWrote: %zu %zu-bit elements\n\n",
		     (size_t)(input_bytes_read / sizeof(*input_data)),
		     (size_t)(sizeof(*input_data) * CHAR_BIT),
		     (size_t)(intermediate_bytes_written),
		     (size_t)(sizeof(*intermediate_data) * CHAR_BIT));
	}

	ztd_char8_t output_data[ztdc_c_array_size(input_data)] = { 0 };
	size_t intermediate_input_byte_size = intermediate_bytes_written;
	const unsigned char* intermediate_input_bytes
	     = (const unsigned char*)intermediate_data;
	const size_t starting_output_byte_size = sizeof(output_data);
	size_t output_byte_size                = starting_output_byte_size;
	unsigned char* output_bytes            = (unsigned char*)output_data;
	cnc_mcerr err = cnc_conv(conversion_reverse, &output_byte_size, &output_bytes,
	     &intermediate_input_byte_size, &intermediate_input_bytes);
	const size_t intermediate_bytes_read
	     = intermediate_bytes_written - intermediate_input_byte_size;
	const size_t output_bytes_written
	     = starting_output_byte_size - output_byte_size;
	if (err != cnc_mcerr_ok) {
		// failed back to the original
		fprintf(stderr, "Output conversion:\n");
		fwrite(failed_conversion_result_title_str,
		     sizeof(*failed_conversion_result_title_str),
		     failed_conversion_result_title_str_size, stderr);
		fprintf(stderr,
		     "\n\tRead: %zu %zu-bit elements"
		     "\n\tWrote: %zu %zu-bit elements\n\n",
		     (size_t)(intermediate_bytes_read / sizeof(*intermediate_data)),
		     (size_t)(sizeof(*intermediate_data) * CHAR_BIT),
		     (size_t)(output_bytes_written),
		     (size_t)(sizeof(*output_data) * CHAR_BIT));

		cnc_conv_delete(conversion);
		cnc_conv_delete(conversion_reverse);
		cnc_registry_delete(registry);
		return 6;
	}
	else {
		fprintf(stdout, "Output conversion:\n");
		fwrite(success_conversion_result_title_str,
		     sizeof(*success_conversion_result_title_str),
		     success_conversion_result_title_str_size, stdout);
		fprintf(stdout,
		     "\n\tRead: %zu %zu-bit elements"
		     "\n\tWrote: %zu %zu-bit elements\n\n",
		     (size_t)(intermediate_bytes_read / sizeof(*intermediate_data)),
		     (size_t)(sizeof(*intermediate_data) * CHAR_BIT),
		     (size_t)(output_bytes_written),
		     (size_t)(sizeof(*output_data) * CHAR_BIT));
	}

	printf("Output conversion result:\n");
	cnc_print_str_c32n(output_bytes_written / sizeof(*output_data), output_data);
	printf("\nOriginal Input:\n");
	cnc_print_str_c32n(ztdc_c_string_array_size(input_data), input_data);
	printf("\n\n");

	cnc_conv_delete(conversion);
	cnc_conv_delete(conversion_reverse);
	cnc_registry_delete(registry);
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
