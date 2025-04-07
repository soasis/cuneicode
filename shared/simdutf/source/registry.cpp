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

#include <ztd/cuneicode/shared/simdutf/registry.hpp>

#include <ztd/cuneicode.h>

#include <ztd/idk/assert.hpp>
#include <ztd/idk/endian.hpp>
#include <ztd/idk/unreachable.hpp>
#include <ztd/idk/char_traits.hpp>
#include <ztd/idk/align.hpp>

#include <simdutf.h>
#include <cstring>

static inline cnc_mcstate_t* mcstate_get(unsigned char* erased_state,
     size_t available_space = sizeof(cnc_mcstate_t) + (sizeof(cnc_mcstate_t) - 1)) {
	void* extra_start = static_cast<void*>(erased_state);
	auto aligned
	     = ztdc_align(alignof(cnc_mcstate_t), sizeof(cnc_mcstate_t), extra_start, available_space);
	if (aligned.ptr == nullptr) {
		return nullptr;
	}
	return static_cast<cnc_mcstate_t*>(static_cast<void*>(aligned.ptr));
}

static inline cnc_mcstate_t* mcstate_get(void* erased_state,
     size_t available_space = sizeof(cnc_mcstate_t) + (sizeof(cnc_mcstate_t) - 1)) noexcept {
	return mcstate_get(static_cast<unsigned char*>(erased_state), available_space);
}

static inline void mcstate_close(void* erased_state) noexcept {
	cnc_mcstate_t* state = mcstate_get(erased_state);
	state->~cnc_mcstate_t();
}

static inline cnc_open_err mcstate_open(cnc_conversion_registry* registry,
     cnc_conversion* conversion, size_t* p_available_space, size_t* p_max_alignment,
     void** p_space) noexcept {
	const bool is_counting = conversion == nullptr;
	*p_max_alignment = ::std::max(*p_max_alignment, static_cast<size_t>(alignof(cnc_mcstate_t)));
	//[[maybe_unused]] const size_t starting_available_space = *p_available_space;
	void* const starting_p_space = p_space == nullptr ? nullptr : *p_space;
	cnc_mcstate_t* state_aligned = mcstate_get(starting_p_space, *p_available_space);
	if (state_aligned == nullptr) {
		// Ffffffffflubberbuckets.
		return cnc_open_err_allocation_failure;
	}
	unsigned char* const aligned_space
	     = static_cast<unsigned char*>(static_cast<void*>(state_aligned + 1));
	const size_t used_space = (aligned_space - static_cast<unsigned char*>(starting_p_space));
	if (is_counting) {
		*p_available_space -= (used_space);
		return cnc_open_err_ok;
	}
	cnc_mcstate_t* state = new (static_cast<void*>(state_aligned)) cnc_mcstate_t();
	cnc_mcstate_set_assume_valid(state, false);
	*p_available_space -= used_space;
	*p_space = aligned_space;
	return cnc_open_err_ok;
}

static inline cnc_open_err mcstate_unchecked_open(cnc_conversion_registry* registry,
     cnc_conversion* conversion, size_t* p_available_space, size_t* p_max_alignment,
     void** p_space) noexcept {
	const bool is_counting = conversion == nullptr;
	*p_max_alignment = ::std::max(*p_max_alignment, static_cast<size_t>(alignof(cnc_mcstate_t)));
	//[[maybe_unused]] const size_t starting_available_space = *p_available_space;
	void* const starting_p_space = p_space == nullptr ? nullptr : *p_space;
	cnc_mcstate_t* state_aligned = mcstate_get(starting_p_space, *p_available_space);
	if (state_aligned == nullptr) {
		// Ffffffffflubberbuckets.
		return cnc_open_err_allocation_failure;
	}
	unsigned char* const aligned_space
	     = static_cast<unsigned char*>(static_cast<void*>(state_aligned + 1));
	const size_t used_space = (aligned_space - static_cast<unsigned char*>(starting_p_space));
	if (is_counting) {
		*p_available_space -= (used_space);
		return cnc_open_err_ok;
	}
	cnc_mcstate_t* state = new (static_cast<void*>(state_aligned)) cnc_mcstate_t();
	cnc_mcstate_set_assume_valid(state, true);
	*p_available_space -= used_space;
	*p_space = aligned_space;
	return cnc_open_err_ok;
}

#define UTF_CONVERT_DEFINITION(                                                                                   \
     FROM_N, TO_N, FROM_BIG_SUFFIX, FROM_LIL_SUFFIX, TO_BIG_SUFFIX, TO_LIL_SUFFIX)                                \
	static cnc_mcerr simdutf_utf##FROM_N##_to_utf##TO_N##_convert(cnc_conversion*,                               \
	     size_t* p_output_bytes_size, unsigned char** p_output_bytes, size_t* p_input_bytes_size,                \
	     const unsigned char** p_input_bytes, cnc_pivot_info*, void* erased_state) {                             \
		using from_char_t = std::conditional_t<FROM_N == 8, char, ztd_char##FROM_N##_t>;                        \
		using to_char_t   = std::conditional_t<TO_N == 8, char, ztd_char##TO_N##_t>;                            \
		if (p_input_bytes == nullptr || p_input_bytes[0] == nullptr) {                                          \
			return cnc_mcerr_ok;                                                                               \
		}                                                                                                       \
		ZTD_ASSERT(p_input_bytes_size != nullptr);                                                              \
		size_t& input_bytes_size = p_input_bytes_size[0];                                                       \
		if (input_bytes_size < sizeof(from_char_t)) {                                                           \
			return input_bytes_size == 0 ? cnc_mcerr_ok : cnc_mcerr_incomplete_input;                          \
		}                                                                                                       \
		size_t input_size                 = input_bytes_size / sizeof(from_char_t);                             \
		cnc_mcstate_t* state              = mcstate_get(erased_state);                                          \
		const unsigned char*& input_bytes = p_input_bytes[0];                                                   \
		const bool is_counting_only   = p_output_bytes == nullptr || p_output_bytes[0] == nullptr;              \
		const bool is_unbounded_write = p_output_bytes_size == nullptr;                                         \
		const bool assume_valid       = cnc_mcstate_is_assuming_valid(state);                                   \
		if (!is_counting_only && is_unbounded_write) {                                                          \
			if (assume_valid) {                                                                                \
				size_t output_written        = ztd::endian::native == ztd::endian::big                        \
				            ? simdutf::                                                                       \
                                convert_valid_utf##FROM_N##FROM_BIG_SUFFIX##_to_utf##TO_N##TO_BIG_SUFFIX(  \
                                     (const from_char_t*)input_bytes, input_size,                          \
                                     (to_char_t*)p_output_bytes[0])                                        \
				            : simdutf::                                                                       \
                                convert_valid_utf##FROM_N##FROM_LIL_SUFFIX##_to_utf##TO_N##TO_LIL_SUFFIX(  \
                                     (const from_char_t*)input_bytes, input_size,                          \
                                     (to_char_t*)p_output_bytes[0]);                                       \
				const size_t write_byte_size = output_written * sizeof(to_char_t);                            \
				input_bytes += input_bytes_size;                                                              \
				input_bytes_size = 0;                                                                         \
				p_output_bytes[0] += write_byte_size;                                                         \
				return cnc_mcerr_ok;                                                                          \
			}                                                                                                  \
			else {                                                                                             \
				simdutf::result result = ztd::endian::native == ztd::endian::big                              \
				     ? simdutf::                                                                              \
				            convert_utf##FROM_N##FROM_BIG_SUFFIX##_to_utf##TO_N##TO_BIG_SUFFIX##_with_errors( \
				                 (const from_char_t*)input_bytes, input_size,                                 \
				                 (to_char_t*)p_output_bytes[0])                                               \
				     : simdutf::                                                                              \
				            convert_utf##FROM_N##FROM_LIL_SUFFIX##_to_utf##TO_N##TO_LIL_SUFFIX##_with_errors( \
				                 (const from_char_t*)input_bytes, input_size,                                 \
				                 (to_char_t*)p_output_bytes[0]);                                              \
				if (result.error == simdutf::error_code::SUCCESS) {                                           \
					const size_t write_byte_size = result.count * sizeof(to_char_t);                         \
					input_bytes += input_bytes_size;                                                         \
					input_bytes_size = 0;                                                                    \
					p_output_bytes[0] += write_byte_size;                                                    \
					return cnc_mcerr_ok;                                                                     \
				}                                                                                             \
			}                                                                                                  \
		}                                                                                                       \
		bool valid_utf##FROM_N = assume_valid                                                                   \
		     ? true                                                                                             \
		     : (ztd::endian::native == ztd::endian::big                                                         \
		                 ? simdutf::validate_utf##FROM_N##FROM_BIG_SUFFIX(                                      \
		                        (const from_char_t*)input_bytes, input_size)                                    \
		                 : simdutf::validate_utf##FROM_N##FROM_LIL_SUFFIX(                                      \
		                        (const from_char_t*)input_bytes, input_size));                                  \
		if (!valid_utf##FROM_N) {                                                                               \
			ztd_char##TO_N##_t* output                                                                         \
			     = is_counting_only ? nullptr : (ztd_char##TO_N##_t*)p_output_bytes[0];                        \
			const ztd_char##FROM_N##_t* input = (ztd_char##FROM_N##_t*)p_input_bytes[0];                       \
			size_t output_size                                                                                 \
			     = is_unbounded_write ? 0 : *p_output_bytes_size / sizeof(to_char_t);                          \
			cnc_mcerr err                                                                                      \
			     = cnc_c##FROM_N##snrtoc##TO_N##sn(is_unbounded_write ? nullptr : &output_size,                \
			          &output, &input_size, &input, state);                                                    \
			if (!is_unbounded_write) {                                                                         \
				*p_output_bytes_size = output_size * sizeof(to_char_t);                                       \
			}                                                                                                  \
			if (!is_counting_only) {                                                                           \
				p_output_bytes[0] = (unsigned char*)(output);                                                 \
			}                                                                                                  \
			p_input_bytes_size[0] = input_size * sizeof(from_char_t);                                          \
			p_input_bytes[0]      = (const unsigned char*)(input);                                             \
			return err;                                                                                        \
		}                                                                                                       \
		if (is_counting_only) {                                                                                 \
			if (!is_unbounded_write) {                                                                         \
				size_t& output_bytes_size    = *p_output_bytes_size;                                          \
				const size_t write_size      = ztd::endian::native == ztd::endian::big                        \
				          ? simdutf::utf##TO_N##_length_from_utf##FROM_N##FROM_BIG_SUFFIX(                    \
                                (const from_char_t*)input_bytes, input_size)                                 \
				          : simdutf::utf##TO_N##_length_from_utf##FROM_N##FROM_BIG_SUFFIX(                    \
                                (const from_char_t*)input_bytes, input_size);                                \
				const size_t write_byte_size = (write_size * sizeof(to_char_t));                              \
				ZTD_ASSERT(write_byte_size <= output_bytes_size);                                             \
				output_bytes_size -= write_byte_size;                                                         \
			}                                                                                                  \
			input_bytes += input_bytes_size;                                                                   \
			input_bytes_size -= input_bytes_size;                                                              \
			return cnc_mcerr_ok;                                                                               \
		}                                                                                                       \
		else {                                                                                                  \
			const size_t initial_write_size = ztd::endian::native == ztd::endian::big                          \
			     ? simdutf::utf##TO_N##_length_from_utf##FROM_N##FROM_BIG_SUFFIX(                              \
			            (const from_char_t*)input_bytes, input_size)                                           \
			     : simdutf::utf##TO_N##_length_from_utf##FROM_N##FROM_LIL_SUFFIX(                              \
			            (const from_char_t*)input_bytes, input_size);                                          \
			if (is_unbounded_write || *p_output_bytes_size >= initial_write_size) {                            \
				const size_t write_size = ztd::endian::native == ztd::endian::big                             \
				     ? simdutf::                                                                              \
				            convert_valid_utf##FROM_N##FROM_BIG_SUFFIX##_to_utf##TO_N##TO_BIG_SUFFIX(         \
				                 (const from_char_t*)input_bytes, input_size,                                 \
				                 (to_char_t*)p_output_bytes[0])                                               \
				     : simdutf::                                                                              \
				            convert_valid_utf##FROM_N##FROM_LIL_SUFFIX##_to_utf##TO_N##TO_LIL_SUFFIX(         \
				                 (const from_char_t*)input_bytes, input_size,                                 \
				                 (to_char_t*)p_output_bytes[0]);                                              \
                                                                                                                  \
                                                                                                                  \
				const size_t write_byte_size = write_size * sizeof(to_char_t);                                \
				if (!is_unbounded_write) {                                                                    \
					ZTD_ASSERT(initial_write_size == write_size);                                            \
					*p_output_bytes_size -= write_byte_size;                                                 \
				}                                                                                             \
				p_output_bytes[0] += write_byte_size;                                                         \
				input_bytes += input_bytes_size;                                                              \
				input_bytes_size -= input_bytes_size;                                                         \
				return cnc_mcerr_ok;                                                                          \
			}                                                                                                  \
		}                                                                                                       \
                                                                                                                  \
		ztd_char##TO_N##_t* output                                                                              \
		     = is_counting_only ? nullptr : (ztd_char##TO_N##_t*)p_output_bytes[0];                             \
		const ztd_char##FROM_N##_t* input = (ztd_char##FROM_N##_t*)p_input_bytes[0];                            \
		size_t output_size = is_unbounded_write ? 0 : *p_output_bytes_size / sizeof(to_char_t);                 \
		cnc_mcerr err      = cnc_c##FROM_N##snrtoc##TO_N##sn(                                                   \
               is_unbounded_write ? nullptr : &output_size, &output, &input_size, &input, state);            \
		if (!is_unbounded_write) {                                                                              \
			*p_output_bytes_size = output_size * sizeof(to_char_t);                                            \
		}                                                                                                       \
		if (!is_counting_only) {                                                                                \
			p_output_bytes[0] = (unsigned char*)(output);                                                      \
		}                                                                                                       \
		p_input_bytes_size[0] = input_size * sizeof(from_char_t);                                               \
		p_input_bytes[0]      = (const unsigned char*)(input);                                                  \
		return err;                                                                                             \
	}                                                                                                            \
	static_assert(true, "")

UTF_CONVERT_DEFINITION(8, 16, , , be, le);
UTF_CONVERT_DEFINITION(16, 8, be, le, , );
UTF_CONVERT_DEFINITION(32, 8, , , , );
UTF_CONVERT_DEFINITION(32, 16, , , be, le);
UTF_CONVERT_DEFINITION(16, 32, be, le, , );
#undef UTF_CONVERT_DEFINITION

#define UTF_CONVERT_DEFINITION(N, BIG_SUFFIX, LIL_SUFFIX)                                            \
	static cnc_mcerr simdutf_utf##N##_to_utf##N##_convert(cnc_conversion*,                          \
	     size_t* p_output_bytes_size, unsigned char** p_output_bytes, size_t* p_input_bytes_size,   \
	     const unsigned char** p_input_bytes, cnc_pivot_info*, void* erased_state) {                \
		using from_char_t = std::conditional_t<N == 8, char, ztd_char##N##_t>;                     \
		using to_char_t   = std::conditional_t<N == 8, char, ztd_char##N##_t>;                     \
		if (p_input_bytes == nullptr || p_input_bytes[0] == nullptr) {                             \
			return cnc_mcerr_ok;                                                                  \
		}                                                                                          \
		ZTD_ASSERT(p_input_bytes_size != nullptr);                                                 \
		size_t& input_bytes_size = p_input_bytes_size[0];                                          \
		if (input_bytes_size < sizeof(from_char_t)) {                                              \
			return input_bytes_size == 0 ? cnc_mcerr_ok : cnc_mcerr_incomplete_input;             \
		}                                                                                          \
		size_t input_size                 = input_bytes_size / sizeof(from_char_t);                \
		cnc_mcstate_t* state              = mcstate_get(erased_state);                             \
		const unsigned char*& input_bytes = p_input_bytes[0];                                      \
		const bool is_counting_only   = p_output_bytes == nullptr || p_output_bytes[0] == nullptr; \
		const bool is_unbounded_write = p_output_bytes_size == nullptr;                            \
		const bool assume_valid       = cnc_mcstate_is_assuming_valid(state);                      \
		if (!assume_valid) { /* if we can't assume it's valid, we must check validating before     \
			                   copying... */                                                      \
			const from_char_t* input  = (const from_char_t*)input_bytes;                          \
			simdutf::result valid_utf = ztd::endian::native == ztd::endian::big                   \
			     ? simdutf::validate_utf##N##BIG_SUFFIX##_with_errors(input, input_size)          \
			     : simdutf::validate_utf##N##LIL_SUFFIX##_with_errors(input, input_size);         \
			/* perform a partial copy of the data, if needed */                                   \
			const size_t initial_output_bytes_size = valid_utf.count * sizeof(from_char_t);       \
			size_t output_bytes_size               = initial_output_bytes_size;                   \
			size_t available_output_bytes_size     = initial_output_bytes_size;                   \
			if (!is_unbounded_write) {                                                            \
				available_output_bytes_size = p_output_bytes_size[0];                            \
				output_bytes_size                                                                \
				     = (std::min)(output_bytes_size, available_output_bytes_size);               \
				p_output_bytes_size[0] -= output_bytes_size;                                     \
			}                                                                                     \
			if (!is_counting_only) {                                                              \
				std::memcpy(p_output_bytes[0], input_bytes, output_bytes_size);                  \
				p_output_bytes[0] += output_bytes_size;                                          \
			}                                                                                     \
			p_input_bytes_size[0] -= output_bytes_size;                                           \
			p_input_bytes[0] += output_bytes_size;                                                \
			if (valid_utf.error != simdutf::error_code::SUCCESS) {                                \
				if constexpr (N == 32) {                                                         \
					return cnc_mcerr_invalid_sequence;                                          \
				}                                                                                \
				else if constexpr (N == 16) {                                                    \
					return valid_utf.error == simdutf::error_code::SURROGATE                    \
					          && (input_size % 2 != 0)                                          \
					     ? cnc_mcerr_incomplete_input                                           \
					     : cnc_mcerr_invalid_sequence;                                          \
				}                                                                                \
				else {                                                                           \
					return valid_utf.error == simdutf::error_code::TOO_SHORT                    \
					     ? cnc_mcerr_incomplete_input                                           \
					     : cnc_mcerr_invalid_sequence;                                          \
				}                                                                                \
			}                                                                                     \
			return initial_output_bytes_size > available_output_bytes_size                        \
			     ? cnc_mcerr_insufficient_output                                                  \
			     : cnc_mcerr_ok;                                                                  \
		}                                                                                          \
		else {                                                                                     \
			const size_t output_bytes_size = input_bytes_size;                                    \
			if (!is_unbounded_write) {                                                            \
				p_output_bytes_size[0] -= input_bytes_size;                                      \
			}                                                                                     \
			if (!is_counting_only) { /* just memcpy the whole damn thing, then */                 \
				std::memcpy(p_output_bytes[0], input_bytes, output_bytes_size);                  \
				p_output_bytes[0] += output_bytes_size;                                          \
			}                                                                                     \
			p_input_bytes_size[0] -= output_bytes_size;                                           \
			p_input_bytes[0] += output_bytes_size;                                                \
			return cnc_mcerr_ok;                                                                  \
		}                                                                                          \
	}                                                                                               \
	static_assert(true, "")

UTF_CONVERT_DEFINITION(8, , );
UTF_CONVERT_DEFINITION(16, be, le);
UTF_CONVERT_DEFINITION(32, , );

#undef UTF_CONVERT_SAME_DEFINITION

static inline cnc_mcerr simdutf_utf8_to_utf32_convert(cnc_conversion*, size_t* p_output_bytes_size,
     unsigned char** p_output_bytes, size_t* p_input_bytes_size,
     const unsigned char** p_input_bytes, cnc_pivot_info*, void* erased_state) {
	using from_char_t = char;
	using to_char_t   = char32_t;
	if (p_input_bytes == nullptr || p_input_bytes[0] == nullptr) {
		return cnc_mcerr_ok;
	}
	ZTD_ASSERT(p_input_bytes_size != nullptr);
	size_t& input_bytes_size = p_input_bytes_size[0];
	if (input_bytes_size < sizeof(from_char_t)) {
		return input_bytes_size == 0 ? cnc_mcerr_ok : cnc_mcerr_incomplete_input;
	}
	size_t input_size                 = input_bytes_size / sizeof(from_char_t);
	cnc_mcstate_t* state              = mcstate_get(erased_state);
	const unsigned char*& input_bytes = p_input_bytes[0];
	const bool is_counting_only       = p_output_bytes == nullptr || p_output_bytes[0] == nullptr;
	const bool is_unbounded_write     = p_output_bytes_size == nullptr;
	const bool assume_valid           = cnc_mcstate_is_assuming_valid(state);
	if (!is_counting_only && is_unbounded_write) {
		if (assume_valid) {
			size_t output_written        = ztd::endian::native == ztd::endian::big
			            ? simdutf::convert_valid_utf8_to_utf32(
                           (const from_char_t*)input_bytes, input_size,
                           (to_char_t*)p_output_bytes[0])
			            : simdutf::convert_valid_utf8_to_utf32(
                           (const from_char_t*)input_bytes, input_size,
                           (to_char_t*)p_output_bytes[0]);
			const size_t write_byte_size = output_written * sizeof(to_char_t);
			input_bytes += input_bytes_size;
			input_bytes_size = 0;
			p_output_bytes[0] += write_byte_size;
			return cnc_mcerr_ok;
		}
	}
}

extern bool cnc_shared_add_bulk_simdutf_to_registry(cnc_conversion_registry* registry)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	using utf8string_view = std::basic_string_view<ztd_char8_t>;
	// unchecked functions
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf8-unchecked";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf8-unchecked";
		cnc_open_err err = cnc_registry_add_multi_c8_multimin(registry, from_code.data(),
		     to_code.data(), simdutf_utf8_to_utf8_convert, nullptr, mcstate_unchecked_open,
		     mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf16-unchecked";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf16-unchecked";
		cnc_open_err err = cnc_registry_add_multi_c8_multimin(registry, from_code.data(),
		     to_code.data(), simdutf_utf16_to_utf16_convert, nullptr, mcstate_unchecked_open,
		     mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf32-unchecked";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf32-unchecked";
		cnc_open_err err = cnc_registry_add_multi_c8_multimin(registry, from_code.data(),
		     to_code.data(), simdutf_utf32_to_utf32_convert, nullptr, mcstate_unchecked_open,
		     mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf8-unchecked";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf16-unchecked";
		cnc_open_err err = cnc_registry_add_multi_c8_multimin(registry, from_code.data(),
		     to_code.data(), simdutf_utf8_to_utf16_convert, nullptr, mcstate_unchecked_open,
		     mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf16-unchecked";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf8-unchecked";
		cnc_open_err err = cnc_registry_add_multi_c8_multimin(registry, from_code.data(),
		     to_code.data(), simdutf_utf16_to_utf8_convert, nullptr, mcstate_unchecked_open,
		     mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf8-unchecked";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf32-unchecked";
		cnc_open_err err = cnc_registry_add_multi_c8_multimin(registry, from_code.data(),
		     to_code.data(), simdutf_utf8_to_utf32_convert, nullptr, mcstate_unchecked_open,
		     mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf32-unchecked";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf8-unchecked";
		cnc_open_err err = cnc_registry_add_multi_c8_multimin(registry, from_code.data(),
		     to_code.data(), simdutf_utf32_to_utf8_convert, nullptr, mcstate_unchecked_open,
		     mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf16-unchecked";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf32-unchecked";
		cnc_open_err err = cnc_registry_add_multi_c8_multimin(registry, from_code.data(),
		     to_code.data(), simdutf_utf16_to_utf32_convert, nullptr, mcstate_unchecked_open,
		     mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf32-unchecked";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf16-unchecked";
		cnc_open_err err = cnc_registry_add_multi_c8_multimin(registry, from_code.data(),
		     to_code.data(), simdutf_utf32_to_utf16_convert, nullptr, mcstate_unchecked_open,
		     mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}

	// checked functions
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf8";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf8";
		cnc_open_err err
		     = cnc_registry_add_multi_c8_multimin(registry, from_code.data(), to_code.data(),
		          simdutf_utf8_to_utf8_convert, nullptr, mcstate_open, mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf16";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf16";
		cnc_open_err err
		     = cnc_registry_add_multi_c8_multimin(registry, from_code.data(), to_code.data(),
		          simdutf_utf16_to_utf16_convert, nullptr, mcstate_open, mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf32";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf32";
		cnc_open_err err
		     = cnc_registry_add_multi_c8_multimin(registry, from_code.data(), to_code.data(),
		          simdutf_utf32_to_utf32_convert, nullptr, mcstate_open, mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf8";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf16";
		cnc_open_err err
		     = cnc_registry_add_multi_c8_multimin(registry, from_code.data(), to_code.data(),
		          simdutf_utf8_to_utf16_convert, nullptr, mcstate_open, mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf16";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf8";
		cnc_open_err err
		     = cnc_registry_add_multi_c8_multimin(registry, from_code.data(), to_code.data(),
		          simdutf_utf16_to_utf8_convert, nullptr, mcstate_open, mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf8";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf32";
		cnc_open_err err
		     = cnc_registry_add_multi_c8_multimin(registry, from_code.data(), to_code.data(),
		          simdutf_utf8_to_utf32_convert, nullptr, mcstate_open, mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf32";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf8";
		cnc_open_err err
		     = cnc_registry_add_multi_c8_multimin(registry, from_code.data(), to_code.data(),
		          simdutf_utf32_to_utf8_convert, nullptr, mcstate_open, mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf16";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf32";
		cnc_open_err err
		     = cnc_registry_add_multi_c8_multimin(registry, from_code.data(), to_code.data(),
		          simdutf_utf16_to_utf32_convert, nullptr, mcstate_open, mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	{
		const utf8string_view from_code = (const ztd_char8_t*)"utf32";
		const utf8string_view to_code   = (const ztd_char8_t*)"utf16";
		cnc_open_err err
		     = cnc_registry_add_multi_c8_multimin(registry, from_code.data(), to_code.data(),
		          simdutf_utf32_to_utf16_convert, nullptr, mcstate_open, mcstate_close, 0);
		if (err != cnc_open_err_ok) {
			return false;
		}
	}
	return true;
}
