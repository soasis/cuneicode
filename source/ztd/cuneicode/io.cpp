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
// Version 2.0 (the "License"); you may not use this file
// except in compliance with the License. You may obtain a copy of the License at
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

#include <ztd/cuneicode/mcstate.h>

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_mcn(FILE* __file, size_t __str_n, const char* __str)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	const char* __str_left = __str;
	size_t __str_n_left    = __str_n;
	for (; __str_n_left != 0;) {
		// if there's any kind of conversion happening underneath in the C standard library, we
		// need to mill it through; this is different from UTF-8, where we just have to bite the
		// bullet and ship it through to the terminal and pray to God that the system isn't
		// completely fargone and irredeemably stupid.
		int __n   = __str_n_left > INT_MAX ? INT_MAX : (int)__str_n_left;
		int __err = fprintf(__file, "%.*s", __n, __str_left);
		if (__err < 0) {
			// .... well.
			break;
		}
		if (__err < __n) {
			// another error. not sure what to do about this.
			break;
		}
		__str_n_left -= __err;
		__str_left += __err;
	}
	return __str_n - __str_n_left;
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_mc(FILE* __file, const char* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_mcn(__file, ztdc_c_string_ptr_size_c(__str), __str);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_mcn(size_t __str_n, const char* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_mcn(stdout, __str_n, __str);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_mc(const char* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_mcn(stdout, ztdc_c_string_ptr_size_c(__str), __str);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_mwcn(FILE* __file, size_t __str_n, const ztd_wchar_t* __str)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	// convert to UTF-8 first
	cnc_mcstate_t __state = {};
	ztd_char8_t
	     __intermediate_data[ZTD_INTERMEDIATE_BUFFER_SUGGESTED_SIZE_I_(ztd_char8_t) > CNC_C8_MAX
	               ? ZTD_INTERMEDIATE_BUFFER_SUGGESTED_SIZE_I_(ztd_char8_t)
	               : CNC_C8_MAX];
	size_t __input_remaining = __str_n;
	for (; __input_remaining != 0;) {
		const size_t __initial_input_remaining = __input_remaining;
		size_t __intermediate_size             = ztdc_c_array_size(__intermediate_data);
		ztd_char8_t* __intermediate_output     = __intermediate_data;
		cnc_mcerr __conv_err                   = cnc_mwcsnrtoc8sn(
               &__intermediate_size, &__intermediate_output, &__input_remaining, &__str, &__state);
		const size_t __intermediate_written
		     = ztdc_c_array_size(__intermediate_data) - __intermediate_size;
		switch (__conv_err) {
		case cnc_mcerr_invalid_sequence:
			errno = EILSEQ;
			break;
		case cnc_mcerr_incomplete_input:
			if (__initial_input_remaining == __input_remaining && __intermediate_output == 0) {
				// we have incomplete input AND we're not reading or writing anything??
				// we might be busted: get out of here?
				errno = ERANGE;
			}
			break;
		case cnc_mcerr_ok:
		case cnc_mcerr_insufficient_output:
			// these cases are fine: just go with the amount we have
			break;
		}
		size_t __write_err = fwrite(&__intermediate_data[0], sizeof(*__intermediate_data),
		     __intermediate_written, __file);
		if (__write_err < __intermediate_written) {
			break;
		}
	}
	return (size_t)(__str_n - __input_remaining);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_mwc(FILE* __file, const ztd_wchar_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_mwcn(__file, ztdc_c_string_ptr_size_wc(__str), __str);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_mwcn(size_t __str_n, const ztd_wchar_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_mwcn(stdout, __str_n, __str);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_mwc(const ztd_wchar_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_mwcn(stdout, ztdc_c_string_ptr_size_wc(__str), __str);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_c8n(FILE* __file, size_t __str_n, const ztd_char8_t* __str)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	// pipe it straight into the output, pray the terminal/file/whatever isn't hot fucking garbage
	// or something.
	return fwrite(__str, __str_n, sizeof(*__str), __file);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_c8(FILE* __file, const ztd_char8_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_c8n(__file, ztdc_c_string_ptr_size_c8(__str), __str);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_c8n(size_t __str_n, const ztd_char8_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_c8n(stdout, __str_n, __str);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_c8(const ztd_char8_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_c8n(stdout, ztdc_c_string_ptr_size_c8(__str), __str);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_c16n(FILE* __file, size_t __str_n, const ztd_char16_t* __str)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	// convert to UTF-8 first
	cnc_mcstate_t __state = {};
	ztd_char8_t
	     __intermediate_data[ZTD_INTERMEDIATE_BUFFER_SUGGESTED_SIZE_I_(ztd_char8_t) > CNC_C8_MAX
	               ? ZTD_INTERMEDIATE_BUFFER_SUGGESTED_SIZE_I_(ztd_char8_t)
	               : CNC_C8_MAX];
	size_t __input_remaining = __str_n;
	for (; __input_remaining != 0;) {
		const size_t __initial_input_remaining = __input_remaining;
		size_t __intermediate_size             = ztdc_c_array_size(__intermediate_data);
		ztd_char8_t* __intermediate_output     = __intermediate_data;
		cnc_mcerr __conv_err                   = cnc_c16snrtoc8sn(
               &__intermediate_size, &__intermediate_output, &__input_remaining, &__str, &__state);
		const size_t __intermediate_written
		     = ztdc_c_array_size(__intermediate_data) - __intermediate_size;
		switch (__conv_err) {
		case cnc_mcerr_invalid_sequence:
			errno = EILSEQ;
			break;
		case cnc_mcerr_incomplete_input:
			if (__initial_input_remaining == __input_remaining && __intermediate_output == 0) {
				// we have incomplete input AND we're not reading or writing anything??
				// we might be busted: get out of here?
				errno = ERANGE;
			}
			break;
		case cnc_mcerr_ok:
		case cnc_mcerr_insufficient_output:
			// these cases are fine: just go with the amount we have
			break;
		}
		size_t __write_err = fwrite(&__intermediate_data[0], sizeof(*__intermediate_data),
		     __intermediate_written, __file);
		if (__write_err < __intermediate_written) {
			break;
		}
	}
	return (size_t)(__str_n - __input_remaining);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_c16(FILE* __file, const ztd_char16_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_c16n(__file, ztdc_c_string_ptr_size_c16(__str), __str);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_c16n(size_t __str_n, const ztd_char16_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_c16n(stdout, __str_n, __str);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_c16(const ztd_char16_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_c16n(stdout, ztdc_c_string_ptr_size_c16(__str), __str);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_c32n(FILE* __file, size_t __str_n, const ztd_char32_t* __str)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	// convert to UTF-8 first
	cnc_mcstate_t __state = {};
	ztd_char8_t
	     __intermediate_data[ZTD_INTERMEDIATE_BUFFER_SUGGESTED_SIZE_I_(ztd_char8_t) > CNC_C8_MAX
	               ? ZTD_INTERMEDIATE_BUFFER_SUGGESTED_SIZE_I_(ztd_char8_t)
	               : CNC_C8_MAX];
	size_t __input_remaining = __str_n;
	for (; __input_remaining != 0;) {
		const size_t __initial_input_remaining = __input_remaining;
		size_t __intermediate_size             = ztdc_c_array_size(__intermediate_data);
		ztd_char8_t* __intermediate_output     = __intermediate_data;
		cnc_mcerr __conv_err                   = cnc_c32snrtoc8sn(
               &__intermediate_size, &__intermediate_output, &__input_remaining, &__str, &__state);
		const size_t __intermediate_written
		     = ztdc_c_array_size(__intermediate_data) - __intermediate_size;
		switch (__conv_err) {
		case cnc_mcerr_invalid_sequence:
			errno = EILSEQ;
			break;
		case cnc_mcerr_incomplete_input:
			if (__initial_input_remaining == __input_remaining && __intermediate_output == 0) {
				// we have incomplete input AND we're not reading or writing anything??
				// we might be busted: get out of here?
				errno = ERANGE;
			}
			break;
		case cnc_mcerr_ok:
		case cnc_mcerr_insufficient_output:
			// these cases are fine: just go with the amount we have
			break;
		}
		size_t __write_err = fwrite(&__intermediate_data[0], sizeof(*__intermediate_data),
		     __intermediate_written, __file);
		if (__write_err < __intermediate_written) {
			break;
		}
	}
	return (size_t)(__str_n - __input_remaining);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_fprint_str_c32(FILE* __file, const ztd_char32_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_c32n(__file, ztdc_c_string_ptr_size_c32(__str), __str);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_c32n(size_t __str_n, const ztd_char32_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_c32n(stdout, __str_n, __str);
}
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
size_t cnc_print_str_c32(const ztd_char32_t* __str) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_fprint_str_c32n(stdout, ztdc_c_string_ptr_size_c32(__str), __str);
}
