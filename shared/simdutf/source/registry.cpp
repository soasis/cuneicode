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
#include <ztd/idk/char_traits.hpp>

#include <simdutf.h>

#define UTF_CONVERT_DEFINITION(                                                                              \
     FROM_N, TO_N, FROM_BIG_SUFFIX, FROM_LIL_SUFFIX, TO_BIG_SUFFIX, TO_LIL_SUFFIX)                           \
	static cnc_mcerror simdutf_utf##FROM_N##_to_utf##TO_N##_convert(cnc_conversion*,                        \
	     size_t* p_output_bytes_size, unsigned char** p_output_bytes, size_t* p_input_bytes_size,           \
	     const unsigned char** p_input_bytes, cnc_pivot_info*, void*) {                                     \
		using from_char_t = std::conditional_t<FROM_N == 8, char, ztd_char##FROM_N##_t>;                   \
		using to_char_t   = std::conditional_t<TO_N == 8, char, ztd_char##TO_N##_t>;                       \
		if (p_input_bytes == nullptr || *p_input_bytes == nullptr) {                                       \
			return CNC_MCERROR_OK;                                                                        \
		}                                                                                                  \
		ZTD_ASSERT(p_input_bytes_size != nullptr);                                                         \
		if (*p_input_bytes_size == 0) {                                                                    \
			return CNC_MCERROR_OK;                                                                        \
		}                                                                                                  \
		const unsigned char*& input_bytes = *p_input_bytes;                                                \
		size_t& input_bytes_size          = *p_input_bytes_size;                                           \
		const bool is_counting_only   = p_output_bytes == nullptr || *p_output_bytes == nullptr;           \
		const bool is_unbounded_write = p_output_bytes_size == nullptr;                                    \
		if (!is_counting_only && is_unbounded_write) {                                                     \
			simdutf::result result = ztd::endian::native == ztd::endian::big                              \
			     ? simdutf::                                                                              \
			          convert_utf##FROM_N##FROM_BIG_SUFFIX##_to_utf##TO_N##TO_BIG_SUFFIX##_with_errors(   \
			               (const from_char_t*)input_bytes,                                               \
			               input_bytes_size / sizeof(from_char_t), (to_char_t*)*p_output_bytes)           \
			     : simdutf::                                                                              \
			          convert_utf##FROM_N##FROM_LIL_SUFFIX##_to_utf##TO_N##TO_LIL_SUFFIX##_with_errors(   \
			               (const from_char_t*)input_bytes,                                               \
			               input_bytes_size / sizeof(from_char_t),                                        \
			               (to_char_t*)*p_output_bytes);                                                  \
			if (result.error == simdutf::error_code::SUCCESS) {                                           \
				input_bytes += input_bytes_size;                                                         \
				input_bytes_size = 0;                                                                    \
				*p_output_bytes -= result.count * sizeof(to_char_t);                                     \
				return CNC_MCERROR_OK;                                                                   \
			}                                                                                             \
		}                                                                                                  \
		bool valid_utf##FROM_N = ztd::endian::native == ztd::endian::big                                   \
		     ? simdutf::validate_utf##FROM_N##FROM_BIG_SUFFIX(                                             \
		          (const from_char_t*)input_bytes, input_bytes_size / sizeof(const from_char_t))           \
		     : simdutf::validate_utf##FROM_N##FROM_LIL_SUFFIX((const from_char_t*)input_bytes,             \
		          input_bytes_size / sizeof(const from_char_t));                                           \
		if (valid_utf##FROM_N) {                                                                           \
			if (is_counting_only) {                                                                       \
				/* VALIDATION/COUNT CASE */                                                              \
				if (!is_unbounded_write) {                                                               \
					size_t& output_bytes_size = *p_output_bytes_size;                                   \
					const size_t write_size   = ztd::endian::native == ztd::endian::big                 \
					       ? simdutf::utf##TO_N##_length_from_utf##FROM_N##FROM_BIG_SUFFIX(             \
					            (const from_char_t*)input_bytes,                                        \
					            input_bytes_size / sizeof(const from_char_t))                           \
					       : simdutf::utf##TO_N##_length_from_utf##FROM_N##FROM_BIG_SUFFIX(             \
					            (const from_char_t*)input_bytes,                                        \
					            input_bytes_size / sizeof(const from_char_t));                          \
					[[maybe_unused]] const size_t write_byte_size                                       \
					     = (write_size * sizeof(to_char_t));                                            \
					ZTD_ASSERT(write_byte_size <= output_bytes_size);                                   \
					output_bytes_size -= write_byte_size;                                               \
				}                                                                                        \
				input_bytes += input_bytes_size;                                                         \
				input_bytes_size -= input_bytes_size;                                                    \
				return CNC_MCERROR_OK;                                                                   \
			}                                                                                             \
			else {                                                                                        \
				const size_t initial_write_size = ztd::endian::native == ztd::endian::big                \
				     ? simdutf::utf##TO_N##_length_from_utf##FROM_N##FROM_BIG_SUFFIX(                    \
				          (const from_char_t*)input_bytes,                                               \
				          input_bytes_size / sizeof(const from_char_t))                                  \
				     : simdutf::utf##TO_N##_length_from_utf##FROM_N##FROM_LIL_SUFFIX(                    \
				          (const from_char_t*)input_bytes,                                               \
				          input_bytes_size / sizeof(const from_char_t));                                 \
				if (is_unbounded_write || *p_output_bytes_size >= initial_write_size) {                  \
					const size_t write_size = ztd::endian::native == ztd::endian::big                   \
					     ? simdutf::                                                                    \
					          convert_valid_utf##FROM_N##FROM_BIG_SUFFIX##_to_utf##TO_N##TO_BIG_SUFFIX( \
					               (const from_char_t*)input_bytes,                                     \
					               input_bytes_size / sizeof(const from_char_t),                        \
					               (to_char_t*)*p_output_bytes)                                         \
					     : simdutf::                                                                    \
					          convert_valid_utf##FROM_N##FROM_LIL_SUFFIX##_to_utf##TO_N##TO_LIL_SUFFIX( \
					               (const from_char_t*)input_bytes,                                     \
					               input_bytes_size / sizeof(const from_char_t),                        \
					               (to_char_t*)*p_output_bytes);                                        \
                                                                                                             \
                                                                                                             \
					if (!is_unbounded_write) {                                                          \
						ZTD_ASSERT(initial_write_size == write_size);                                  \
						const size_t write_byte_size = write_size * sizeof(to_char_t);                 \
						*p_output_bytes_size -= write_byte_size;                                       \
					}                                                                                   \
					input_bytes += input_bytes_size;                                                    \
					input_bytes_size -= input_bytes_size;                                               \
					return CNC_MCERROR_OK;                                                              \
				}                                                                                        \
			}                                                                                             \
		}                                                                                                  \
                                                                                                             \
		ztd_char##TO_N##_t* output                                                                         \
		     = is_counting_only ? nullptr : (ztd_char##TO_N##_t*)*p_output_bytes;                          \
		const ztd_char##FROM_N##_t* input = (ztd_char##FROM_N##_t*)*p_input_bytes;                         \
		size_t output_size = is_unbounded_write ? 0 : *p_output_bytes_size / sizeof(to_char_t);            \
		size_t input_size  = *p_input_bytes_size / sizeof(from_char_t);                                    \
		cnc_mcerror err    = cnc_c##FROM_N##sntoc##TO_N##sn(                                               \
		        is_unbounded_write ? &output_size : nullptr, &output, &input_size, &input);                \
		if (!is_unbounded_write) {                                                                         \
			*p_output_bytes_size = output_size * sizeof(to_char_t);                                       \
		}                                                                                                  \
		if (!is_counting_only) {                                                                           \
			*p_output_bytes = (unsigned char*)(output);                                                   \
		}                                                                                                  \
		*p_input_bytes_size = input_size * sizeof(from_char_t);                                            \
		*p_input_bytes      = (const unsigned char*)(input);                                               \
		return err;                                                                                        \
	}                                                                                                       \
	static_assert(true, "")

UTF_CONVERT_DEFINITION(8, 16, , , be, le);
UTF_CONVERT_DEFINITION(16, 8, be, le, , );
UTF_CONVERT_DEFINITION(8, 32, , , , );
UTF_CONVERT_DEFINITION(32, 8, , , , );
UTF_CONVERT_DEFINITION(32, 16, , , be, le);
UTF_CONVERT_DEFINITION(16, 32, be, le, , );

#undef UTF_CONVERT_DEFINITION
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
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf16";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf32";
		cnc_open_error err = cnc_add_to_registry_multi(registry, from_code.data(), to_code.data(),
		     simdutf_utf16_to_utf32_convert, nullptr, nullptr);
		if (err != CNC_OPEN_ERROR_OKAY) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf32";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf16";
		cnc_open_error err = cnc_add_to_registry_multi(registry, from_code.data(), to_code.data(),
		     simdutf_utf32_to_utf16_convert, nullptr, nullptr);
		if (err != CNC_OPEN_ERROR_OKAY) {
			return false;
		}
	}
	return true;
}
