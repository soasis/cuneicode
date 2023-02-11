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

	const ztd_char16_t utf16_text[] = u"🥺🙏";

	const ztd_char16_t* count_input_ptr = utf16_text;
	// ztdc_c_array_size INCLUDES the null terminator in the size!
	const size_t initial_count_input_size = ztdc_c_array_size(utf16_text);
	size_t count_input_size               = initial_count_input_size;
	cnc_mcstate_t count_state             = { 0 };
	// Use the function but with "nullptr" for the output pointer
	cnc_mcerr err = cnc_c16snrtoc8sn(
	     // To get the proper size for this conversion, we use the same
	     // function but with "NULL" specificers:
	     NULL, NULL,
	     // input second
	     &count_input_size, &count_input_ptr,
	     // state parameter
	     &count_state);
	size_t input_read = (size_t)(initial_count_input_size - count_input_size);
	if (err != cnc_mcerr_ok) {
		const char* err_str = cnc_mcerr_to_str(err);
		fprintf(stderr,
		     "An (unexpected) error occurred and the counting/validating could "
		     "not happen!\nThe error happened at code unit %zu in the UTF-16 "
		     "input.\nError string: %s (code: '%d')\n",
		     input_read, err_str, (int)err);
	}

	printf(
	     "The input UTF-16 is valid and consumed all %zu code units (elements) "
	     "of input.\n",
	     input_read);

	return 0;
}
