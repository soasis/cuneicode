// ============================================================================
//
// ztd.cuneicode
// Copyright © 2022-2022 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
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
// 		http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ========================================================================= //

#include <ztd/cuneicode/shared/simdutf/registry.hpp>

#include <ztd/cuneicode.h>

#include <ztd/idk/assert.hpp>
#include <ztd/idk/endian.hpp>
#include <ztd/idk/unreachable.h>

#include <simdutf.h>

// ======================
// UTF-8 ↔ UTF-16

static cnc_mcerror simdutf_utf8_to_utf16_convert(cnc_conversion*, size_t* p_output_bytes_size,
     unsigned char** p_output_bytes, size_t* p_input_bytes_size,
     const unsigned char** p_input_bytes, void*) {
	if (p_input_bytes == nullptr || *p_input_bytes == nullptr) {
		// we have empty input: just leave
		return CNC_MCERROR_OKAY;
	}
	// the input size must not be the null pointer if the input is valid.
	ZTD_ASSERT(p_input_bytes_size != nullptr);
	if (*p_input_bytes_size == 0) {
		// we have empty input: just leave
		return CNC_MCERROR_OKAY;
	}
	// get the input size as normal variables
	const unsigned char*& input_bytes = *p_input_bytes;
	size_t& input_bytes_size          = *p_input_bytes_size;
	// Use the simdutf API to check the data
	bool valid_utf8
	     = simdutf::validate_utf8((const char*)input_bytes, input_bytes_size / sizeof(const char));
	if (valid_utf8) {
		// we tuck these cases in here because simdutf
		// does not do validation in any of its counting functions,
		// so we have to validate before-hand first.
		if (p_output_bytes == nullptr || *p_output_bytes == nullptr) {
			// VALIDATION/COUNT CASE
			// we are simply doing validation here.
			// validate that the input is okay
			// since it is UTF-8, as long as it is valid UTF-8
			// it is automatically valid UTF-16.
			if (p_output_bytes_size != nullptr) {
				// COUNTING CASE
				size_t& output_bytes_size = *p_output_bytes_size;
				const size_t write_size   = simdutf::utf16_length_from_utf8(
				       (const char*)input_bytes, input_bytes_size / sizeof(const char));
				// set the number of bytes we *should* expect from this input
				// note we multiply since it is byte count, not element count!
				[[maybe_unused]] const size_t write_byte_size
				     = (write_size * sizeof(ztd_char16_t));
				ZTD_ASSERT(write_byte_size <= output_bytes_size);
				output_bytes_size -= write_byte_size;
			}
			// VALIDATION CASE
			// This covers the validation part (updating the input pointer/size)
			input_bytes += input_bytes_size;
			input_bytes_size -= input_bytes_size;
			return CNC_MCERROR_OKAY;
		}
		else {
			// Okay, we have an output pointer. Splat data in.
			const size_t initial_write_size = simdutf::utf16_length_from_utf8(
			     (const char*)input_bytes, input_bytes_size / sizeof(const char));
			const bool is_unbounded_write = p_output_bytes_size == nullptr;
			if (is_unbounded_write || *p_output_bytes_size >= initial_write_size) {
				const size_t write_size =
#if ZTDC_NATIVE_ENDIAN == ZTDC_BIG_ENDIAN
				     simdutf::convert_valid_utf8_to_utf16be(
#else
				     simdutf::convert_valid_utf8_to_utf16le(
#endif

				          (const char*)input_bytes, input_bytes_size / sizeof(const char),
				          (char16_t*)*p_output_bytes);

				if (!is_unbounded_write) {
					// make sure to update the output size
					ZTD_ASSERT(initial_write_size == write_size);
					const size_t write_byte_size = write_size * sizeof(char16_t);
					*p_output_bytes_size -= write_byte_size;
				}
				input_bytes += input_bytes_size;
				input_bytes_size -= input_bytes_size;
				return CNC_MCERROR_OKAY;
			}
		}
	}
	// fall back in all failure cases, if we don't have valid input data
	size_t output_size = p_output_bytes_size ? *p_output_bytes_size / sizeof(ztd_char16_t) : 0;
	cnc_mcerror err    = cnc_c8sntoc16sn(p_output_bytes_size ? &output_size : nullptr,
	     (ztd_char16_t**)p_output_bytes, p_input_bytes_size, (const ztd_char8_t**)p_input_bytes);
	*p_output_bytes_size = output_size * sizeof(ztd_char16_t);
	return err;
}

static cnc_mcerror simdutf_utf16_to_utf8_convert(cnc_conversion*, size_t* p_output_bytes_size,
     unsigned char** p_output_bytes, size_t* p_input_bytes_size,
     const unsigned char** p_input_bytes, void*) {
	if (p_input_bytes == nullptr || *p_input_bytes == nullptr) {
		// we have empty input: just leave
		return CNC_MCERROR_OKAY;
	}
	// the input size must not be the null pointer if the input is valid.
	ZTD_ASSERT(p_input_bytes_size != nullptr);
	if (*p_input_bytes_size == 0) {
		// we have empty input: just leave
		return CNC_MCERROR_OKAY;
	}
	// get the input size as normal variables
	const unsigned char*& input_bytes = *p_input_bytes;
	size_t& input_bytes_size          = *p_input_bytes_size;

	// Use the simdutf API to check the data
	const simdutf::result validate_result
#if ZTDC_NATIVE_ENDIAN == ZTDC_BIG_ENDIAN
	     = simdutf::validate_utf16be_with_errors(
#else
	     = simdutf::validate_utf16le_with_errors(
#endif
	          (const char16_t*)input_bytes, input_bytes_size / sizeof(const char16_t));
	const bool valid_utf8 = validate_result.error == simdutf::error_code::SUCCESS;
	if (valid_utf8) {
		// we tuck these cases in here because simdutf
		// does not do validation in any of its counting functions,
		// so we have to validate before-hand first.
		if (p_output_bytes == nullptr || *p_output_bytes == nullptr) {
			// VALIDATION/COUNT CASE
			// we are simply doing validation here.
			// validate that the input is okay
			// since it is UTF-16, as long as it is valid UTF-16
			// it is automatically valid UTF-8.
			if (p_output_bytes_size != nullptr) {
				// COUNTING CASE
				size_t& output_bytes_size = *p_output_bytes_size;
				const size_t write_byte_size =
#if ZTDC_NATIVE_ENDIAN == ZTDC_BIG_ENDIAN
				     simdutf::utf8_length_from_utf16be(
#else
				     simdutf::utf8_length_from_utf16le(
#endif
				          (const char16_t*)input_bytes,
				          input_bytes_size / sizeof(const char16_t));
				// set the number of bytes we *should* expect from this input
				// note we multiply since it is byte count, not element count!
				ZTD_ASSERT(output_bytes_size > write_byte_size);
				output_bytes_size -= write_byte_size;
			}
			// VALIDATION CASE
			// This covers the validation part (updating the input pointer/size)
			input_bytes += input_bytes_size;
			input_bytes_size -= input_bytes_size;
			return CNC_MCERROR_OKAY;
		}
		else {
			// Okay, we have an output pointer. Splat data in.
			const size_t initial_write_byte_size =
#if ZTDC_NATIVE_ENDIAN == ZTDC_BIG_ENDIAN
			     simdutf::utf8_length_from_utf16be(
#else
			     simdutf::utf8_length_from_utf16le(
#endif
			          (const char16_t*)input_bytes, input_bytes_size / sizeof(const char16_t));
			const bool is_unbounded_write = p_output_bytes_size == nullptr;
			if (is_unbounded_write || *p_output_bytes_size >= initial_write_byte_size) {
				const size_t write_byte_size =
#if ZTDC_NATIVE_ENDIAN == ZTDC_BIG_ENDIAN
				     simdutf::convert_valid_utf16be_to_utf8(
#else
				     simdutf::convert_valid_utf16le_to_utf8(
#endif
				          (const char16_t*)input_bytes, input_bytes_size / sizeof(char16_t),
				          (char*)*p_output_bytes);
				if (!is_unbounded_write) {
					// make sure to update the output size
					ZTD_ASSERT(initial_write_byte_size == write_byte_size);
					*p_output_bytes_size -= write_byte_size;
				}
				input_bytes += input_bytes_size;
				input_bytes_size -= input_bytes_size;
				return CNC_MCERROR_OKAY;
			}
		}
	}
	// fall back in all failure cases, if we don't have valid input data
	size_t input_size   = p_input_bytes_size ? *p_input_bytes_size / sizeof(ztd_char16_t) : 0;
	cnc_mcerror err     = cnc_c16sntoc8sn(p_output_bytes_size, (ztd_char8_t**)p_output_bytes,
          p_input_bytes_size ? &input_size : nullptr, (const ztd_char16_t**)p_input_bytes);
	*p_input_bytes_size = input_size * sizeof(ztd_char8_t);
	return err;
}

// ======================
// UTF-8 ↔ UTF-32

static cnc_mcerror simdutf_utf8_to_utf32_convert(cnc_conversion*, size_t* p_output_bytes_size,
     unsigned char** p_output_bytes, size_t* p_input_bytes_size,
     const unsigned char** p_input_bytes, void*) {
	if (p_input_bytes == nullptr || *p_input_bytes == nullptr) {
		// we have empty input: just leave
		return CNC_MCERROR_OKAY;
	}
	// the input size must not be the null pointer if the input is valid.
	ZTD_ASSERT(p_input_bytes_size != nullptr);
	if (*p_input_bytes_size == 0) {
		// we have empty input: just leave
		return CNC_MCERROR_OKAY;
	}
	// get the input size as normal variables
	const unsigned char*& input_bytes = *p_input_bytes;
	size_t& input_bytes_size          = *p_input_bytes_size;
	// Use the simdutf API to check the data
	bool valid_utf8 = simdutf::validate_utf8((const char*)input_bytes, input_bytes_size);
	if (valid_utf8) {
		// we tuck these cases in here because simdutf
		// does not do validation in any of its counting functions,
		// so we have to validate before-hand first.
		if (p_output_bytes == nullptr || *p_output_bytes == nullptr) {
			// VALIDATION/COUNT CASE
			// we are simply doing validation here.
			// validate that the input is okay
			// since it is UTF-8, as long as it is valid UTF-8
			// it is automatically valid UTF-32.
			if (p_output_bytes_size != nullptr) {
				// COUNTING CASE
				size_t& output_bytes_size = *p_output_bytes_size;
				const size_t write_size   = simdutf::utf32_length_from_utf8(
				       (const char*)input_bytes, input_bytes_size / sizeof(const char));
				// set the number of bytes we *should* expect from this input
				// note we multiply since it is byte count, not element count!
				[[maybe_unused]] const size_t write_byte_size
				     = (write_size * sizeof(ztd_char32_t));
				ZTD_ASSERT(write_byte_size <= output_bytes_size);
				output_bytes_size -= write_byte_size;
			}
			// VALIDATION CASE
			// This covers the validation part (updating the input pointer/size)
			input_bytes += input_bytes_size;
			input_bytes_size -= input_bytes_size;
			return CNC_MCERROR_OKAY;
		}
		else {
			// Okay, we have an output pointer. Splat data in.
			const size_t initial_write_size = simdutf::utf32_length_from_utf8(
			     (const char*)input_bytes, input_bytes_size / sizeof(const char));
			const bool is_unbounded_write = p_output_bytes_size == nullptr;
			if (is_unbounded_write || *p_output_bytes_size >= initial_write_size) {
				const size_t write_size
				     = simdutf::convert_valid_utf8_to_utf32((const char*)input_bytes,
				          input_bytes_size / sizeof(const char), (char32_t*)*p_output_bytes);

				if (!is_unbounded_write) {
					// make sure to update the output size
					ZTD_ASSERT(initial_write_size == write_size);
					const size_t write_byte_size = write_size * sizeof(char32_t);
					*p_output_bytes_size -= write_byte_size;
				}
				input_bytes += input_bytes_size;
				input_bytes_size -= input_bytes_size;
				return CNC_MCERROR_OKAY;
			}
		}
	}
	// fall back in all failure cases, if we don't have valid input data
	size_t output_size = p_output_bytes_size ? *p_output_bytes_size / sizeof(ztd_char32_t) : 0;
	cnc_mcerror err    = cnc_c8sntoc32sn(p_output_bytes_size ? &output_size : nullptr,
	     (ztd_char32_t**)p_output_bytes, p_input_bytes_size, (const ztd_char8_t**)p_input_bytes);
	*p_output_bytes_size = output_size * sizeof(ztd_char32_t);
	return err;
}

static cnc_mcerror simdutf_utf32_to_utf8_convert(cnc_conversion*, size_t* p_output_bytes_size,
     unsigned char** p_output_bytes, size_t* p_input_bytes_size,
     const unsigned char** p_input_bytes, void*) {
	if (p_input_bytes == nullptr || *p_input_bytes == nullptr) {
		// we have empty input: just leave
		return CNC_MCERROR_OKAY;
	}
	// the input size must not be the null pointer if the input is valid.
	ZTD_ASSERT(p_input_bytes_size != nullptr);
	if (*p_input_bytes_size == 0) {
		// we have empty input: just leave
		return CNC_MCERROR_OKAY;
	}
	// get the input size as normal variables
	const unsigned char*& input_bytes = *p_input_bytes;
	size_t& input_bytes_size          = *p_input_bytes_size;
	// Use the simdutf API to check the data
	const simdutf::result validate_result = simdutf::validate_utf32_with_errors(
	     (const char32_t*)input_bytes, input_bytes_size / sizeof(const char32_t));
	const bool valid_utf8 = validate_result.error == simdutf::error_code::SUCCESS;
	if (valid_utf8) {
		// we tuck these cases in here because simdutf
		// does not do validation in any of its counting functions,
		// so we have to validate before-hand first.
		if (p_output_bytes == nullptr || *p_output_bytes == nullptr) {
			// VALIDATION/COUNT CASE
			// we are simply doing validation here.
			// validate that the input is okay
			// since it is UTF-32, as long as it is valid UTF-32
			// it is automatically valid UTF-8.
			if (p_output_bytes_size != nullptr) {
				// COUNTING CASE
				size_t& output_bytes_size    = *p_output_bytes_size;
				const size_t write_byte_size = simdutf::utf8_length_from_utf32(
				     (const char32_t*)input_bytes, input_bytes_size / sizeof(const char32_t));
				// set the number of bytes we *should* expect from this input
				// note we multiply since it is byte count, not element count!
				ZTD_ASSERT(output_bytes_size > write_byte_size);
				output_bytes_size -= write_byte_size;
			}
			// VALIDATION CASE
			// This covers the validation part (updating the input pointer/size)
			input_bytes += input_bytes_size;
			input_bytes_size -= input_bytes_size;
			return CNC_MCERROR_OKAY;
		}
		else {
			// Okay, we have an output pointer. Splat data in.
			const size_t initial_write_byte_size = simdutf::utf8_length_from_utf32(
			     (const char32_t*)input_bytes, input_bytes_size / sizeof(const char32_t));
			const bool is_unbounded_write = p_output_bytes_size == nullptr;
			if (is_unbounded_write || *p_output_bytes_size >= initial_write_byte_size) {
				const size_t write_byte_size
				     = simdutf::convert_valid_utf32_to_utf8((const char32_t*)input_bytes,
				          input_bytes_size / sizeof(char32_t), (char*)*p_output_bytes);
				if (!is_unbounded_write) {
					// make sure to update the output size
					ZTD_ASSERT(initial_write_byte_size == write_byte_size);
					*p_output_bytes_size -= write_byte_size;
				}
				input_bytes += input_bytes_size;
				input_bytes_size -= input_bytes_size;
				return CNC_MCERROR_OKAY;
			}
		}
	}
	// fall back in all failure cases, if we don't have valid input data
	size_t input_size   = p_input_bytes_size ? *p_input_bytes_size / sizeof(ztd_char32_t) : 0;
	cnc_mcerror err     = cnc_c32sntoc8sn(p_output_bytes_size, (ztd_char8_t**)p_output_bytes,
          p_input_bytes_size ? &input_size : nullptr, (const ztd_char32_t**)p_input_bytes);
	*p_input_bytes_size = input_size * sizeof(ztd_char8_t);
	return err;
}

bool add_simdutf_to_registry(cnc_conversion_registry* registry) {
	using utf8string_view = std::basic_string_view<ztd_char8_t>;
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf8";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf16";
		cnc_open_error err = cnc_add_to_registry_multi(registry, from_code.data(), to_code.data(),
		     simdutf_utf8_to_utf16_convert, nullptr, nullptr);
		if (err != CNC_OPEN_ERROR_OKAY) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf16";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf8";
		cnc_open_error err = cnc_add_to_registry_multi(registry, from_code.data(), to_code.data(),
		     simdutf_utf16_to_utf8_convert, nullptr, nullptr);
		if (err != CNC_OPEN_ERROR_OKAY) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf8";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf32";
		cnc_open_error err = cnc_add_to_registry_multi(registry, from_code.data(), to_code.data(),
		     simdutf_utf8_to_utf32_convert, nullptr, nullptr);
		if (err != CNC_OPEN_ERROR_OKAY) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf32";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf8";
		cnc_open_error err = cnc_add_to_registry_multi(registry, from_code.data(), to_code.data(),
		     simdutf_utf32_to_utf8_convert, nullptr, nullptr);
		if (err != CNC_OPEN_ERROR_OKAY) {
			return false;
		}
	}
	return true;
}
