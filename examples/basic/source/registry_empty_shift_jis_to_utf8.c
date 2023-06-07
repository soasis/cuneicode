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
#include <ztd/idk/assert.h>
#include <ztd/idk/align.h>
#include <ztd/idk/assume_aligned.h>

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdalign.h>


static inline cnc_mcerr shift_jis_x0208_to_utf32(cnc_conversion* conversion,
     size_t* p_bytes_out_count, unsigned char** p_bytes_out,
     size_t* p_bytes_in_count, const unsigned char** p_bytes_in,
     cnc_pivot_info* p_pivot_info, void* state) {
	// since we know our conversion we can safely ignore many of the parameters
	(void)conversion;
	(void)p_pivot_info;
	(void)state;
	// set up variables for use
	const char* elements_in       = NULL;
	ztd_char32_t* elements_out    = NULL;
	const char** p_elements_in    = &elements_in;
	ztd_char32_t** p_elements_out = &elements_out;
	size_t elements_in_count      = 0;
	size_t elements_out_count     = 0;
	size_t* p_elements_in_count   = NULL;
	size_t* p_elements_out_count  = NULL;

	// if the counts are non-null, adjust them to be element counts
	if (p_bytes_in_count) {
		elements_in_count   = ((*p_bytes_in_count) / sizeof(char));
		p_elements_in_count = &elements_in_count;
	}
	if (p_bytes_out_count) {
		elements_out_count   = ((*p_bytes_out_count) / sizeof(ztd_char32_t));
		p_elements_out_count = &elements_out_count;
	}

	// if the pointers are not null, set their values up here
	if (p_bytes_in) {
		elements_in
		     = ZTD_ASSUME_ALIGNED(alignof(char), (const char*)*p_bytes_in);
	}
	if (p_bytes_out) {
		// if the pointer is non-null, double-check alignment to prevent UB from
		// unaligned reads
		// NOTE: a more sophisticated implementation would do the conversion and
		// then break each write down into one-byte-at-a-time writes to allow for
		// unaligned pointers. We don't do that here, for simplicity's sake.
		ZTD_ASSERT(p_bytes_out
		          ? ztdc_is_aligned(alignof(ztd_char32_t), *p_bytes_out)
		          : true);
		elements_out = ZTD_ASSUME_ALIGNED(
		     alignof(ztd_char32_t), (ztd_char32_t*)*p_bytes_out);
	}

	// do actual conversion
	// NOTE: we're just going to use what's provided by cuneicode,
	// but we COULD write out own here!
	cnc_mcerr err = cnc_mcntoc32n_shift_jis_x0208(p_elements_out_count,
	     p_elements_out, p_elements_in_count, p_elements_in);

	// translate pointers back to byte pointers
	if (p_bytes_in) {
		*p_bytes_in = (const unsigned char*)elements_in;
	}
	if (p_bytes_out) {
		*p_bytes_out = (unsigned char*)elements_out;
	}
	// If the counts are non-null, translate them back into normal byte counts.
	if (p_bytes_in_count) {
		*p_bytes_in_count = elements_in_count * sizeof(char);
	}
	if (p_bytes_out_count) {
		*p_bytes_out_count = elements_out_count * sizeof(ztd_char32_t);
	}
	return err;
}

static inline cnc_mcerr utf32_to_utf8(cnc_conversion* conversion,
     size_t* p_bytes_out_count, unsigned char** p_bytes_out,
     size_t* p_bytes_in_count, const unsigned char** p_bytes_in,
     cnc_pivot_info* p_pivot_info, void* state) {
	// since we know our conversion we can safely ignore many of the parameters
	(void)conversion;
	(void)p_pivot_info;
	(void)state;
	// set up variables for use
	const ztd_char32_t* elements_in    = NULL;
	char* elements_out                 = NULL;
	const ztd_char32_t** p_elements_in = &elements_in;
	char** p_elements_out              = &elements_out;
	size_t elements_in_count           = 0;
	size_t elements_out_count          = 0;
	size_t* p_elements_in_count        = NULL;
	size_t* p_elements_out_count       = NULL;

	// if the counts are non-null, adjust them to be element counts
	if (p_bytes_in_count) {
		elements_in_count   = ((*p_bytes_in_count) / sizeof(ztd_char32_t));
		p_elements_in_count = &elements_in_count;
	}
	if (p_bytes_out_count) {
		elements_out_count   = ((*p_bytes_out_count) / sizeof(char));
		p_elements_out_count = &elements_out_count;
	}

	// if the pointers are not null, set their values up here
	if (p_bytes_in) {
		// if the pointer is non-null, double-check alignment to prevent UB from
		// unaligned reads
		// NOTE: a more sophisticated implementation would do the conversion and
		// then break each read down into one-byte-at-a-time reads to allow for
		// unaligned pointers. We don't do that here, for simplicity's sake.
		ZTD_ASSERT(p_bytes_in
		          ? ztdc_is_aligned(alignof(ztd_char32_t), *p_bytes_in)
		          : true);
		elements_in = ZTD_ASSUME_ALIGNED(
		     alignof(ztd_char32_t), (const ztd_char32_t*)*p_bytes_in);
		p_elements_in = &elements_in;
	}
	if (p_bytes_out) {
		elements_out   = ZTD_ASSUME_ALIGNED(alignof(char), (char*)*p_bytes_out);
		p_elements_out = &elements_out;
	}

	// do actual conversion
	// NOTE: we're just going to use what's provided by cuneicode,
	// but we COULD write out own here!
	cnc_mcerr err = cnc_c32ntomcn_utf8(p_elements_out_count, p_elements_out,
	     p_elements_in_count, p_elements_in);

	// translate pointers back to byte pointers
	if (p_bytes_in) {
		*p_bytes_in = (const unsigned char*)elements_in;
	}
	if (p_bytes_out) {
		*p_bytes_out = (unsigned char*)elements_out;
	}
	// If the counts are non-null, translate them back into normal byte counts.
	if (p_bytes_in_count) {
		*p_bytes_in_count = elements_in_count * sizeof(ztd_char32_t);
	}
	if (p_bytes_out_count) {
		*p_bytes_out_count = elements_out_count * sizeof(char);
	}
	return err;
}

int main() {
	cnc_conversion_registry* registry = NULL;
	{
		cnc_open_err err
		     = cnc_registry_new(&registry, cnc_registry_options_empty);
		if (err != cnc_open_err_ok) {
			fprintf(stderr, "[error] could not open a new registry");
			return 2;
		}
	}

	// Verify that no conversion exists
	// for Shift-JIS to UTF-8.
	{
		cnc_conversion* conversion          = NULL;
		cnc_conversion_info conversion_info = { 0 };
		cnc_open_err err                    = cnc_conv_new(
               registry, "shift-jis", "utf-8", &conversion, &conversion_info);
		if (err != cnc_open_err_no_conversion_path) {
			fprintf(stderr,
			     "[error] there should be no conversion path, but one exists");
			cnc_registry_delete(registry);
			return 3;
		}
	}

	// Actually add the conversion here that we need.
	{
		// Shift-JIS to UTF-32
		cnc_open_err to_utf32_err
		     = cnc_registry_add_single(registry, "shift-jis-x0208", "utf-32",
		          shift_jis_x0208_to_utf32, NULL, NULL, NULL);
		if (to_utf32_err != cnc_open_err_ok) {
			fprintf(stderr,
			     "[error] could not add conversion from shift-jis-x0208 to "
			     "utf-32");
			cnc_registry_delete(registry);
			return 4;
		}
		// UTF-32 to Shift-JIS
		cnc_open_err to_utf8_err = cnc_registry_add_single(
		     registry, "utf-32", "utf-8", utf32_to_utf8, NULL, NULL, NULL);
		if (to_utf8_err != cnc_open_err_ok) {
			fprintf(stderr,
			     "[error] could not add conversion from utf-32 to utf-8");
			cnc_registry_delete(registry);
			return 5;
		}
	}

	// Ease-of-use alias for Shift-JIS's name
	{
		cnc_open_err err
		     = cnc_registry_add_alias(registry, "shift-jis", "shift-jis-x0208");
		if (err != cnc_open_err_ok) {
			fprintf(stderr,
			     "[error] could not add alias that maps shift-jis to "
			     "shift-jis-x0208");
			cnc_registry_delete(registry);
			return 6;
		}
	}

	cnc_conversion* conversion          = NULL;
	cnc_conversion_info conversion_info = { 0 };
	{
		cnc_open_err err = cnc_conv_new(
		     registry, "shift-jis", "utf-8", &conversion, &conversion_info);
		if (err != cnc_open_err_ok) {
			fprintf(stderr, "[error] could not open a new registry");
			cnc_registry_delete(registry);
			return 7;
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

	const char input_data[]
	     = "\x61\x6c\x6c\x20\x61\x63\x63\x6f\x72\x64\x69\x6e\x67\x20\x74\x6f\x20"
	       "\x82\xAF\x82\xA2\x82\xA9\x82\xAD\x2c\x20\x75\x66\x75\x66\x75\x66\x75"
	       "\x21";
	unsigned char output_data[ztdc_c_array_size(input_data) * 2] = { 0 };

	const size_t starting_input_size  = ztdc_c_string_array_byte_size(input_data);
	size_t input_size                 = starting_input_size;
	const unsigned char* input        = (const unsigned char*)&input_data[0];
	const size_t starting_output_size = ztdc_c_array_byte_size(output_data);
	size_t output_size                = starting_output_size;
	unsigned char* output             = (unsigned char*)&output_data[0];
	cnc_mcerr err
	     = cnc_conv(conversion, &output_size, &output, &input_size, &input);
	const bool has_err          = err != cnc_mcerr_ok;
	const size_t input_read     = starting_input_size - input_size;
	const size_t output_written = starting_output_size - output_size;
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
	     (size_t)(input_read), (size_t)(sizeof(*input) * CHAR_BIT),
	     (size_t)(output_written), (size_t)(sizeof(*output) * CHAR_BIT));
	fprintf(stdout, "%s Conversion Result:\n", has_err ? "Partial" : "Complete");
	fwrite(output_data, sizeof(*output_data), output_written, stdout);
	// the stream (may be) line-buffered, so make sure an extra "\n" is written
	// out this is actually critical for some forms of stdout/stderr mirrors; they
	// won't show the last line even if you manually call fflush(…) !
	fprintf(stdout, "\n");

	// clean up resources
	cnc_conv_delete(conversion);
	cnc_registry_delete(registry);
	return has_err ? 1 : 0;
}
