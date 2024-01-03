// =============================================================================
//
// ztd.cuneicode
// Copyright © JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
// Contact: opensource@soasis.org
//
// Commercial License Usage
// Licensees holding valid commercial ztd.cuneicode licenses may use this file in
// accordance with the commercial license agreement provided with the
// Software or, alternatively, in accordance with the terms contained in
// a written agreement between you and Shepherd's Oasis, LLC.
// For licensing terms and conditions see your agreement. For
// further information contact opensource@soasis.org.
//
// Apache License Version 2 Usage
// Alternatively, this file may be used under the terms of Apache License
// Version 2.0 (the "License") for non-commercial use; you may not use this
// file except in compliance with the License. You may obtain a copy of the
// License at
//
// https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ============================================================================ //

#include <ztd/cuneicode.h>

#include <ztd/idk/size.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
	if (argc < 2) {
		fprintf(stderr, "A name argument must be given to the program!");
		return 1;
	}

	const char* name      = argv[1];
	const size_t name_len = strlen(name);

	char utf8_name_buffer[4096]    = { 0 };
	const size_t utf8_name_max_len = ztdc_c_string_array_size(utf8_name_buffer);
	char* utf8_name                = utf8_name_buffer;
	const size_t name_len_limit    = (name_len * CNC_C8_MAX);

	if (name_len_limit >= utf8_name_max_len) {
		fprintf(stderr,
		     "The name provided tot hsi program was, unfortunately, too big!");
		return 2;
	}

	size_t utf8_name_len_after = utf8_name_max_len;
	size_t name_len_after      = name_len;
	cnc_mcerr err              = cnc_mcsntomcsn_exec_utf8(
          &utf8_name_len_after, &utf8_name, &name_len_after, &name);
	const size_t input_consumed = name_len - name_len_after;
	const size_t output_written = utf8_name_max_len - utf8_name_len_after;
	if (err != cnc_mcerr_ok) {
		const char* err_str = cnc_mcerr_to_str(err);
		fprintf(stderr,
		     "An error occurred when attempting to transcribe your name from "
		     "the execution encoding to UTF-8, stopping at input element %zu "
		     "and only writing %zu elements "
		     "(error name: %s).",
		     input_consumed, output_written, err_str);
		return 3;
	}

	printf("Hello there, ");
	fwrite(utf8_name_buffer, 1, output_written, stdout);
	printf("!\n");

	return 0;
}
