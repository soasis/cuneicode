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
	ztd_char8_t utf8_text[9]        = { 0 };

	// Now, actually output it
	const ztd_char16_t* p_input = utf16_text;
	ztd_char8_t* p_output       = utf8_text;
	size_t input_size           = ztdc_c_string_array_size(utf16_text);
	size_t output_size          = ztdc_c_array_size(utf8_text);
	cnc_mcstate_t state         = { 0 };
	// call the function with the right parameters!
	cnc_mcerr err               = cnc_c16snrtoc8sn( // formatting
          &output_size, &p_output,     // output first
          &input_size, &p_input,       // input second
          &state);                     // state parameter
	const size_t input_consumed = (ztdc_c_array_size(utf16_text) - input_size);
	const size_t output_written = (ztdc_c_array_size(utf8_text) - output_size);
	if (err != cnc_mcerr_ok) {
		const char* err_str = cnc_mcerr_to_str(err);
		fprintf(stderr,
		     "An (unexpected) error occurred and the conversion could not "
		     "happen! Error string: %s (code: '%d')\n",
		     err_str, (int)err);
		return 1;
	}

	printf(
	     "Converted %zu UTF-16 code units to %zu UTF-8 code units, giving the "
	     "text:",
	     input_consumed, output_written);
	// requires a capable terminal / output, but will be
	// UTF-8 text!
	fwrite(utf8_text, sizeof(ztd_char8_t), output_written, stdout);
	printf("\n");

	return 0;
}
