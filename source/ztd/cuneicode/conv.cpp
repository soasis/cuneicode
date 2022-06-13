// =============================================================================
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
// ============================================================================ //

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/conv.h>
#include <ztd/cuneicode/mcchar.h>

#include <ztd/cuneicode/detail/conv_id.hpp>
#include <ztd/cuneicode/detail/align.hpp>
#include <ztd/cuneicode/detail/err.hpp>
#include <ztd/cuneicode/detail/heap_allocator.hpp>
#include <ztd/cuneicode/detail/conversion.hpp>
#include <ztd/cuneicode/detail/registry.hpp>

#include <ztd/idk/hash.hpp>
#include <ztd/idk/size.hpp>
#include <ztd/idk/encoding_name.hpp>

#include <string_view>
#include <unordered_map>
#include <vector>
#include <new>
#include <memory>
#include <cstring>

namespace {
	struct __typical_cnc_conversion {
		cnc_mcstate_t __state;
	};

	struct __intermediary_cnc_conversion {
		cnc_conversion __link0;
		cnc_conversion __link1;
		// from the end of this struct, AKA from "this + 1"!
		size_t __link1_state_byte_offset;
	};

	struct __intermediary_states {
		__intermediary_cnc_conversion* __intermediary_state;
		void* __link0_state;
		void* __link1_state;
	};

	static inline __typical_cnc_conversion* __typical_state(unsigned char* __state,
	     size_t __available_space
	     = sizeof(__typical_cnc_conversion) + (sizeof(__typical_cnc_conversion) - 1)) {
		void* __extra_start = static_cast<void*>(__state);
		void* __aligned_extra
		     = ::ztd::cnc::__cnc_detail::__align(sizeof(__typical_cnc_conversion),
		          sizeof(__typical_cnc_conversion), __extra_start, __available_space);
		if (__aligned_extra == nullptr) {
			return nullptr;
		}
		return static_cast<__typical_cnc_conversion*>(static_cast<void*>(__aligned_extra));
	}

	static inline __typical_cnc_conversion* __typical_state(void* __state,
	     size_t __available_space
	     = sizeof(__typical_cnc_conversion) + (sizeof(__typical_cnc_conversion) - 1)) noexcept {
		return __typical_state(static_cast<unsigned char*>(__state), __available_space);
	}

	static inline void __typical_close_function(void* __state) noexcept {
		__typical_cnc_conversion* __typical = __typical_state(__state);
		__typical->~__typical_cnc_conversion();
	}

	static inline cnc_open_error __typical_open_function(
	     [[maybe_unused]] cnc_conversion_registry* __registry, cnc_conversion* __conversion,
	     size_t* __p_available_space, size_t* __p_max_alignment, void** __p_space) noexcept {
		bool __is_counting = __conversion == nullptr;
		*__p_max_alignment = ::std::max(
		     *__p_max_alignment, static_cast<size_t>(alignof(__typical_cnc_conversion)));
		[[maybe_unused]] const size_t __starting_available_space = *__p_available_space;
		void* const __starting_p_space = __p_space == nullptr ? nullptr : *__p_space;
		__typical_cnc_conversion* __typical_aligned
		     = __typical_state(__starting_p_space, *__p_available_space);
		unsigned char* const __aligned_space
		     = static_cast<unsigned char*>(static_cast<void*>(__typical_aligned + 1));
		const size_t __used_space
		     = (__aligned_space - static_cast<unsigned char*>(__starting_p_space));
		if (__is_counting) {
			*__p_available_space -= (__used_space);
			return CNC_OPEN_ERROR_OKAY;
		}
		if (__typical_aligned == nullptr) {
			// Ffffffffflubberbuckets.
			return CNC_OPEN_ERROR_ALLOCATION_FAILURE;
		}
		__typical_cnc_conversion* __typical
		     = new (static_cast<void*>(__typical_aligned)) __typical_cnc_conversion();
		cnc_mcstate_t* __p_state = &(__typical->__state);
		::std::memset(__p_state, 0, sizeof(__typical_cnc_conversion));
		*__p_available_space -= __used_space;
		*__p_space = __aligned_space;
		return CNC_OPEN_ERROR_OKAY;
	}

	template <typename _SourceChar, typename _DestChar, typename _Func, _Func __func>
	static inline cnc_mcerror __typical_multi_conversion(
	     [[maybe_unused]] cnc_conversion* __base_conversion, size_t* __p_bytes_out_count,
	     unsigned char** __p_bytes_out, size_t* __p_bytes_in_count,
	     const unsigned char** __p_bytes_in, void* __state) noexcept {
		const bool __is_counting  = __p_bytes_out == nullptr || *__p_bytes_out == nullptr;
		const bool __is_unbounded = __p_bytes_out_count == nullptr;
		const size_t __initial_source_count = *__p_bytes_in_count / sizeof(_SourceChar);
		size_t __source_count               = __initial_source_count;
		const _SourceChar* __source_first   = reinterpret_cast<const _SourceChar*>(*__p_bytes_in);
		const _SourceChar* __source         = __source_first;
		const size_t __initial_dest_count
		     = __is_unbounded ? SIZE_MAX : *__p_bytes_out_count / sizeof(_DestChar);
		size_t __dest_count = __initial_dest_count;
		_DestChar* __dest_first
		     = __is_counting ? nullptr : reinterpret_cast<_DestChar*>(*__p_bytes_out);
		_DestChar* __dest                      = __dest_first;
		__typical_cnc_conversion* __conversion = __typical_state(__state);
		cnc_mcstate_t* __p_state               = &__conversion->__state;
		cnc_mcerror __err = __func(__is_unbounded ? nullptr : &__dest_count, &__dest,
		     &__source_count, &__source, __p_state);
		// always update all relevant counts
		// and pointers, whether or not there is an actual error
		size_t __bytes_written = (__initial_dest_count - __dest_count) * sizeof(_DestChar);
		size_t __bytes_read    = (__initial_source_count - __source_count) * sizeof(_SourceChar);
		*__p_bytes_in          = reinterpret_cast<const unsigned char*>(__source);
		*__p_bytes_in_count -= static_cast<size_t>(__bytes_read);
		if (!__is_counting) {
			*__p_bytes_out = reinterpret_cast<unsigned char*>(__dest);
		}
		if (!__is_unbounded) {
			*__p_bytes_out_count -= static_cast<size_t>(__bytes_written);
		}
		return __err;
	}

	template <typename _SourceChar, typename _DestChar, typename _Func, _Func __func>
	static inline cnc_mcerror __typical_single_conversion(
	     [[maybe_unused]] cnc_conversion* __base_conversion, size_t* __p_bytes_out_count,
	     unsigned char** __p_bytes_out, size_t* __p_bytes_in_count,
	     const unsigned char** __p_bytes_in, void* __state) noexcept {
		const bool __is_counting  = __p_bytes_out == nullptr || *__p_bytes_out == nullptr;
		const bool __is_unbounded = __p_bytes_out_count == nullptr;
		const size_t __initial_source_count = *__p_bytes_in_count / sizeof(_SourceChar);
		size_t __source_count               = __initial_source_count;
		const _SourceChar* __source_first   = reinterpret_cast<const _SourceChar*>(*__p_bytes_in);
		const _SourceChar* __source         = __source_first;
		[[maybe_unused]] const _SourceChar* __source_last = __source_first + __source_count;
		const size_t __initial_dest_count
		     = __is_unbounded ? SIZE_MAX : *__p_bytes_out_count / sizeof(_DestChar);
		size_t __dest_count = __initial_dest_count;
		_DestChar* __dest_first
		     = __is_counting ? nullptr : reinterpret_cast<_DestChar*>(*__p_bytes_out);
		_DestChar* __dest                      = __dest_first;
		__typical_cnc_conversion* __conversion = __typical_state(__state);
		cnc_mcstate_t* __p_state               = &__conversion->__state;
		cnc_mcerror __err = __func(__is_unbounded ? nullptr : &__dest_count, &__dest,
		     &__source_count, &__source, __p_state);
		// always update all relevant counts
		// and pointers, whether or not there is an actual error
		size_t __bytes_written = (__initial_dest_count - __dest_count) * sizeof(_DestChar);
		size_t __bytes_read    = (__initial_source_count - __source_count) * sizeof(_SourceChar);
		*__p_bytes_in          = reinterpret_cast<const unsigned char*>(__source);
		*__p_bytes_in_count -= static_cast<size_t>(__bytes_read);
		if (!__is_counting) {
			*__p_bytes_out = reinterpret_cast<unsigned char*>(__dest);
		}
		if (!__is_unbounded) {
			*__p_bytes_out_count -= static_cast<size_t>(__bytes_written);
		}
		return __err;
	}

	static inline __intermediary_cnc_conversion* __intermediary_align(
	     void* __state, [[maybe_unused]] size_t __available_space) {
		size_t __intermediary_extra_space = sizeof(__intermediary_cnc_conversion);
		void* __intermediary_extra_start  = __state;
		void* __intermediary_aligned      = ::ztd::cnc::__cnc_detail::__align(
		          sizeof(__intermediary_cnc_conversion), sizeof(__intermediary_cnc_conversion),
		          __intermediary_extra_start, __intermediary_extra_space);
		if (__intermediary_aligned == nullptr) {
			return nullptr;
		}
		__intermediary_cnc_conversion* __intermediate_state
		     = reinterpret_cast<__intermediary_cnc_conversion*>(__intermediary_aligned);
		return __intermediate_state;
	}

	static inline __intermediary_states __intermediary_state(unsigned char* __state,
	     size_t __available_space = sizeof(__intermediary_cnc_conversion)
	          + (sizeof(__intermediary_cnc_conversion) - 1)) noexcept {
		__intermediary_cnc_conversion* __intermediate_state
		     = __intermediary_align(__state, __available_space);
		if (__intermediate_state == nullptr) {
			return { nullptr, nullptr, nullptr };
		}
		unsigned char* __link0_byte_data = reinterpret_cast<unsigned char*>(__intermediate_state)
		     + sizeof(__intermediary_cnc_conversion);
		unsigned char* __link1_byte_data
		     = __link0_byte_data + __intermediate_state->__link1_state_byte_offset;
		return { __intermediate_state, static_cast<void*>(__link0_byte_data),
			static_cast<void*>(__link1_byte_data) };
	}

	static inline __intermediary_states __intermediary_state(void* __state,
	     size_t __available_space = sizeof(__intermediary_cnc_conversion)
	          + (sizeof(__intermediary_cnc_conversion) - 1)) noexcept {
		return __intermediary_state(static_cast<unsigned char*>(__state), __available_space);
	}

	static inline void __intermediary_close_function(void* __state) noexcept {
		__intermediary_states __intermediary = __intermediary_state(__state);
		__intermediary.__intermediary_state->__link0.__close_function(
		     __intermediary.__link0_state);
		__intermediary.__intermediary_state->__link1.__close_function(
		     __intermediary.__link1_state);
		__intermediary.__intermediary_state->~__intermediary_cnc_conversion();
	}

	static inline cnc_open_error __intermediary_open_function(cnc_conversion_registry* __registry,
	     cnc_conversion* __conversion, const __cnc_registry_entry* __from,
	     const __cnc_registry_entry* __to, size_t* __p_available_space, size_t* __p_max_alignment,
	     void** __p_space) noexcept {
		bool __is_counting = __conversion == nullptr;
		*__p_max_alignment = ::std::max(
		     *__p_max_alignment, static_cast<size_t>(alignof(__intermediary_cnc_conversion)));
		const size_t __starting_available_space = *__p_available_space;
		void* const __starting_space            = __p_space == nullptr ? nullptr : *__p_space;
		[[maybe_unused]] void* __space          = static_cast<unsigned char*>(*__p_space);
		__intermediary_cnc_conversion* __intermediary_aligned
		     = __intermediary_align(*__p_space, *__p_available_space);
		if (__is_counting) {
			void* __link0_space         = *__p_space;
			size_t __space_before_link0 = *__p_available_space;
			cnc_open_error __link0_err  = __from->__open_function(
			      __registry, nullptr, __p_available_space, __p_max_alignment, &__link0_space);
			if (__link0_err != CNC_OPEN_ERROR_OKAY) {
				*__p_available_space = __starting_available_space;
				*__p_space           = __starting_space;
				return __link0_err;
			}
			*__p_space = __link0_space;
			[[maybe_unused]] size_t __link1_byte_offset
			     = (__space_before_link0 - *__p_available_space);
			void* __link1_space         = *__p_space;
			size_t __space_before_link1 = *__p_available_space;
			cnc_open_error __link1_err  = __to->__open_function(
			      __registry, nullptr, __p_available_space, __p_max_alignment, &__link1_space);
			if (__link1_err != CNC_OPEN_ERROR_OKAY) {
				return __link1_err;
			}
			size_t __link1_last = (__space_before_link1 - *__p_available_space);
			size_t __intermediary_required_size
			     = (static_cast<unsigned char*>(__starting_space)
			            - reinterpret_cast<unsigned char*>(__intermediary_aligned))
			     + __link1_last;
			*__p_available_space -= __intermediary_required_size;
			return CNC_OPEN_ERROR_OKAY;
		}
		if (__intermediary_aligned == nullptr) {
			// Ffffffffflubberbuckets.
			return CNC_OPEN_ERROR_ALLOCATION_FAILURE;
		}
		__intermediary_cnc_conversion* __intermediary
		     = new (__intermediary_aligned) __intermediary_cnc_conversion();
		__intermediary->__link1_state_byte_offset = 0;
		*__p_available_space -= sizeof(__intermediary_cnc_conversion);
		*__p_space          = static_cast<void*>(reinterpret_cast<unsigned char*>(__intermediary)
               + sizeof(__intermediary_cnc_conversion));
		void* __link0_space = *__p_space;
		size_t __space_before_link0 = *__p_available_space;
		cnc_open_error __link0_err  = __from->__open_function(
		      __registry, __conversion, __p_available_space, __p_max_alignment, &__link0_space);
		if (__link0_err != CNC_OPEN_ERROR_OKAY) {
			__intermediary->~__intermediary_cnc_conversion();
			*__p_available_space = __starting_available_space;
			*__p_space           = __starting_space;
			return __link0_err;
		}
		*__p_space                 = __link0_space;
		size_t __link1_byte_offset = (__space_before_link0 - *__p_available_space);
		void* __link1_space        = static_cast<unsigned char*>(*__p_space)
		     + __intermediary->__link1_state_byte_offset;
		cnc_open_error __link1_err = __to->__open_function(
		     __registry, __conversion, __p_available_space, __p_max_alignment, &__link1_space);
		if (__link1_err != CNC_OPEN_ERROR_OKAY) {
			__intermediary->~__intermediary_cnc_conversion();
			__from->__close_function(__link0_space);
			*__p_available_space = __starting_available_space;
			*__p_space           = __starting_space;
			return __link1_err;
		}
		*__p_space = __link1_space;

		__intermediary->__link0.__multi_conversion_function = __from->__multi_conversion_function;
		__intermediary->__link0.__single_conversion_function
		     = __from->__single_conversion_function;
		__intermediary->__link0.__close_function             = __from->__close_function;
		__intermediary->__link1.__multi_conversion_function  = __to->__multi_conversion_function;
		__intermediary->__link1.__single_conversion_function = __to->__single_conversion_function;
		__intermediary->__link1.__close_function             = __to->__close_function;
		__intermediary->__link1_state_byte_offset            = __link1_byte_offset;
		return CNC_OPEN_ERROR_OKAY;
	}

	static inline cnc_mcerror __intermediary_multi_conversion(cnc_conversion* __base_conversion,
	     size_t* __p_bytes_out_count, unsigned char** __p_bytes_out, size_t* __p_bytes_in_count,
	     const unsigned char** __p_bytes_in, void* __state) noexcept {
		const size_t __intermediate_buffer_max = CNC_DEFAULT_CONVERSION_INTERMEDIATE_BUFFER_SIZE;
		unsigned char __intermediate_buffer[__intermediate_buffer_max] {};
		__intermediary_states __states = __intermediary_state(__state);
		for (; *__p_bytes_in_count != 0;) {
			size_t __start_bytes_in_count           = *__p_bytes_in_count;
			const unsigned char* __start_bytes_in   = *__p_bytes_in;
			size_t __intermediate_bytes_out_count   = __intermediate_buffer_max;
			unsigned char* __intermediate_bytes_out = __intermediate_buffer;
			cnc_mcerror __link0res
			     = __states.__intermediary_state->__link0.__multi_conversion_function(
			          __base_conversion, &__intermediate_bytes_out_count,
			          &__intermediate_bytes_out, __p_bytes_in_count, __p_bytes_in,
			          __states.__link0_state);
			if (__link0res != CNC_MCERROR_OKAY
			     && __link0res != CNC_MCERROR_INSUFFICIENT_OUTPUT) {
				// something bad happened: revert potential writes to the
				// progress variables
				*__p_bytes_in_count = __start_bytes_in_count;
				*__p_bytes_in       = __start_bytes_in;
				return __link0res;
			}
			size_t __intermediate_bytes_in_count
			     = __intermediate_buffer_max - __intermediate_bytes_out_count;
			const unsigned char* __intermediate_bytes_in = __intermediate_buffer;
			cnc_mcerror __link1res
			     = __states.__intermediary_state->__link1.__multi_conversion_function(
			          __base_conversion, __p_bytes_out_count, __p_bytes_out,
			          &__intermediate_bytes_in_count, &__intermediate_bytes_in,
			          __states.__link1_state);
			if (__link1res != CNC_MCERROR_OKAY) {
				return __link1res;
			}
		}
		return CNC_MCERROR_OKAY;
	}

	static inline cnc_mcerror __intermediary_single_conversion(cnc_conversion* __base_conversion,
	     size_t* __p_bytes_out_count, unsigned char** __p_bytes_out, size_t* __p_bytes_in_count,
	     const unsigned char** __p_bytes_in, void* __state) noexcept {
		const size_t __intermediate_buffer_max = CNC_DEFAULT_CONVERSION_INTERMEDIATE_BUFFER_SIZE;
		unsigned char __intermediate_buffer[__intermediate_buffer_max] {};
		__intermediary_states __states                         = __intermediary_state(__state);
		[[maybe_unused]] size_t __start_bytes_in_count         = *__p_bytes_in_count;
		[[maybe_unused]] const unsigned char* __start_bytes_in = *__p_bytes_in;
		size_t __intermediate_bytes_out_count                  = __intermediate_buffer_max;
		unsigned char* __intermediate_bytes_out                = __intermediate_buffer;
		cnc_mcerror __link0res
		     = __states.__intermediary_state->__link0.__single_conversion_function(
		          __base_conversion, &__intermediate_bytes_out_count, &__intermediate_bytes_out,
		          __p_bytes_in_count, __p_bytes_in, __states.__link0_state);
		if (__link0res != CNC_MCERROR_OKAY) {
			return __link0res;
		}
		size_t __intermediate_bytes_in_count
		     = __intermediate_buffer_max - __intermediate_bytes_out_count;
		const unsigned char* __intermediate_bytes_in = __intermediate_buffer;
		cnc_mcerror __link1res
		     = __states.__intermediary_state->__link1.__single_conversion_function(
		          __base_conversion, __p_bytes_out_count, __p_bytes_out,
		          &__intermediate_bytes_in_count, &__intermediate_bytes_in,
		          __states.__link1_state);
		return __link1res;
	}

	static inline cnc_open_error __cnc_open_with(cnc_conversion_registry* __registry,
	     const __cnc_registry_entry* __entry, cnc_conversion** __p_out_conversion,
	     size_t* __p_available_space, void** __p_space) {
		const size_t __starting_available_space = *__p_available_space;
		void* __target                          = *__p_space;
		void* __aligned_target                  = ::ztd::cnc::__cnc_detail::__align(
		                      alignof(cnc_conversion), sizeof(cnc_conversion), __target, *__p_available_space);
		if (__aligned_target == nullptr) {
			*__p_available_space = __starting_available_space;
			return CNC_OPEN_ERROR_INSUFFICIENT_OUTPUT;
		}
		cnc_conversion* __base_conv               = new (__aligned_target) cnc_conversion();
		__base_conv->__registry                   = __registry;
		__base_conv->__size                       = __starting_available_space;
		__base_conv->__multi_conversion_function  = __entry->__multi_conversion_function;
		__base_conv->__single_conversion_function = __entry->__single_conversion_function;
		__base_conv->__close_function             = __entry->__close_function;
		*__p_out_conversion                       = __base_conv;
		*__p_space = static_cast<unsigned char*>(static_cast<void*>(__base_conv + 1));
		*__p_available_space -= sizeof(cnc_conversion);
		size_t __max_alignment = alignof(cnc_conversion);
		cnc_open_error __err   = __entry->__open_function(
		       __registry, __base_conv, __p_available_space, &__max_alignment, __p_space);
		if (__err != CNC_OPEN_ERROR_OKAY) {
			__base_conv->~cnc_conversion();
			*__p_out_conversion  = nullptr;
			*__p_available_space = __starting_available_space;
			return __err;
		}
		return __err;
	}

	static inline cnc_open_error __cnc_open_intermediary_with(cnc_conversion_registry* __registry,
	     const __cnc_registry_entry* __from, const __cnc_registry_entry* __to,
	     cnc_conversion** __p_out_conversion, size_t* __p_available_space, void** __p_space) {
		const size_t __starting_available_space = *__p_available_space;
		void* __target                          = *__p_space;
		void* __aligned_target                  = ::ztd::cnc::__cnc_detail::__align(
		                      alignof(cnc_conversion), sizeof(cnc_conversion), __target, *__p_available_space);
		if (__aligned_target == nullptr) {
			*__p_available_space = __starting_available_space;
			return CNC_OPEN_ERROR_INSUFFICIENT_OUTPUT;
		}
		size_t __max_alignment                    = alignof(cnc_conversion);
		cnc_conversion* __base_conv               = new (__aligned_target) cnc_conversion();
		__base_conv->__registry                   = __registry;
		__base_conv->__size                       = __starting_available_space;
		__base_conv->__multi_conversion_function  = &__intermediary_multi_conversion;
		__base_conv->__single_conversion_function = &__intermediary_single_conversion;
		__base_conv->__close_function             = &__intermediary_close_function;
		*__p_available_space -= sizeof(cnc_conversion);
		*__p_space
		     = static_cast<void*>(static_cast<unsigned char*>(static_cast<void*>(__base_conv))
		          + sizeof(cnc_conversion));
		*__p_out_conversion  = __base_conv;
		cnc_open_error __err = ::__intermediary_open_function(__registry, __base_conv, __from,
		     __to, __p_available_space, &__max_alignment, __p_space);
		if (__err != CNC_OPEN_ERROR_OKAY) {
			__base_conv->~cnc_conversion();
			*__p_out_conversion  = nullptr;
			*__p_available_space = __starting_available_space;
			return __err;
		}

		return __err;
	}
} // namespace

extern cnc_mcerror __cnc_multi_from_single_conversion(cnc_conversion* __conversion,
     size_t* __p_output_bytes_size, unsigned char** __p_output_bytes, size_t* __p_input_bytes_size,
     const unsigned char** __p_input_bytes, void* __user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__p_input_bytes_size == nullptr || __p_input_bytes == nullptr) {
		return CNC_MCERROR_OKAY;
	}
	const unsigned char*& __input_bytes = *__p_input_bytes;
	size_t& __input_bytes_size          = *__p_input_bytes_size;
	if (__input_bytes == nullptr || __input_bytes_size == 0) {
		return CNC_MCERROR_OKAY;
	}
	for (; __input_bytes_size > 0;) {
		cnc_mcerror __err
		     = __conversion->__single_conversion_function(__conversion, __p_output_bytes_size,
		          __p_output_bytes, __p_input_bytes_size, __p_input_bytes, __user_data);
		switch (__err) {
		case CNC_MCERROR_OKAY:
			// loop around!
			continue;
		default:
			break;
		}
		return __err;
	}
	// if we reach here, we just simply do not have enough input: bail out
	return CNC_MCERROR_INCOMPLETE_INPUT;
}

extern cnc_mcerror __cnc_single_from_multi_conversion(cnc_conversion* __conversion,
     size_t* __p_output_bytes_size, unsigned char** __p_output_bytes, size_t* __p_input_bytes_size,
     const unsigned char** __p_input_bytes, void* __user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__p_input_bytes_size == nullptr || __p_input_bytes == nullptr) {
		return CNC_MCERROR_OKAY;
	}
	const unsigned char* __input_bytes = *__p_input_bytes;
	size_t __input_bytes_size          = *__p_input_bytes_size;
	if (__input_bytes == nullptr || __input_bytes_size == 0) {
		return CNC_MCERROR_OKAY;
	}
	for (size_t __len = 1; __len <= __input_bytes_size; ++__len) {
		cnc_mcerror __err = __conversion->__multi_conversion_function(__conversion,
		     __p_output_bytes_size, __p_output_bytes, &__len, &__input_bytes, __user_data);
		switch (__err) {
		case CNC_MCERROR_INCOMPLETE_INPUT:
			// alright, so we just need more input:
			// we go A G A N E !
			continue;
		default:
			break;
		}
		return __err;
	}
	// if we reach here, we just simply do not have enough input: bail out
	return CNC_MCERROR_INCOMPLETE_INPUT;
}

cnc_open_error __cnc_add_default_registry_entries(
     cnc_conversion_registry* __registry) ZTD_NOEXCEPT_IF_CXX_I_ {
	cnc_open_error __err;
#define _CHECK_ERR_AND_RETURN(...)                       \
	__err = __VA_ARGS__;                                \
	if (__err != cnc_open_error::CNC_OPEN_ERROR_OKAY) { \
		return __err;                                  \
	}                                                   \
	static_assert(true, "")

	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__exec_name(),
	     ::ztd::cnc::__cnc_detail::__exec_name(),
	     &__typical_multi_conversion<char, char, decltype(&::cnc_mcsnrtomcsn), &::cnc_mcsnrtomcsn>,
	     &__typical_single_conversion<char, char, decltype(&::cnc_mcnrtomcn), &::cnc_mcnrtomcn>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__exec_name(),
	     ::ztd::cnc::__cnc_detail::__wide_name(),
	     &__typical_multi_conversion<char, wchar_t, decltype(&::cnc_mcsnrtomwcsn),
	          &::cnc_mcsnrtomwcsn>,
	     &__typical_single_conversion<char, wchar_t, decltype(&::cnc_mcnrtomwcn),
	          &::cnc_mcnrtomwcn>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__exec_name(),
	     ::ztd::cnc::__cnc_detail::__utf8_name(),
	     &__typical_multi_conversion<char, ztd_char8_t, decltype(&::cnc_mcsnrtoc8sn),
	          &::cnc_mcsnrtoc8sn>,
	     &__typical_single_conversion<char, ztd_char8_t, decltype(&::cnc_mcnrtoc8n),
	          &::cnc_mcnrtoc8n>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__exec_name(),
	     ::ztd::cnc::__cnc_detail::__utf16_name(),
	     &__typical_multi_conversion<char, ztd_char16_t, decltype(&::cnc_mcsnrtoc16sn),
	          &::cnc_mcsnrtoc16sn>,
	     &__typical_single_conversion<char, ztd_char16_t, decltype(&::cnc_mcnrtoc16n),
	          &::cnc_mcnrtoc16n>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__exec_name(),
	     ::ztd::cnc::__cnc_detail::__utf32_name(),
	     &__typical_multi_conversion<char, ztd_char32_t, decltype(&::cnc_mcsnrtoc32sn),
	          &::cnc_mcsnrtoc32sn>,
	     &__typical_single_conversion<char, ztd_char32_t, decltype(&::cnc_mcnrtoc32n),
	          &::cnc_mcnrtoc32n>,
	     &::__typical_open_function, &::__typical_close_function));

	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__wide_name(),
	     ::ztd::cnc::__cnc_detail::__exec_name(),
	     &__typical_multi_conversion<wchar_t, char, decltype(&::cnc_mwcsnrtomcsn),
	          &::cnc_mwcsnrtomcsn>,
	     &__typical_single_conversion<wchar_t, char, decltype(&::cnc_mwcnrtomcn),
	          &::cnc_mwcnrtomcn>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__wide_name(),
	     ::ztd::cnc::__cnc_detail::__wide_name(),
	     &__typical_multi_conversion<wchar_t, wchar_t, decltype(&::cnc_mwcsnrtomwcsn),
	          &::cnc_mwcsnrtomwcsn>,
	     &__typical_single_conversion<wchar_t, wchar_t, decltype(&::cnc_mwcnrtomwcn),
	          &::cnc_mwcnrtomwcn>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__wide_name(),
	     ::ztd::cnc::__cnc_detail::__utf8_name(),
	     &__typical_multi_conversion<wchar_t, ztd_char8_t, decltype(&::cnc_mwcsnrtoc8sn),
	          &::cnc_mwcsnrtoc8sn>,
	     &__typical_single_conversion<wchar_t, ztd_char8_t, decltype(&::cnc_mwcnrtoc8n),
	          &::cnc_mwcnrtoc8n>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__wide_name(),
	     ::ztd::cnc::__cnc_detail::__utf16_name(),
	     &__typical_multi_conversion<wchar_t, ztd_char16_t, decltype(&::cnc_mwcsnrtoc16sn),
	          &::cnc_mwcsnrtoc16sn>,
	     &__typical_single_conversion<wchar_t, ztd_char16_t, decltype(&::cnc_mwcnrtoc16n),
	          &::cnc_mwcnrtoc16n>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__wide_name(),
	     ::ztd::cnc::__cnc_detail::__utf32_name(),
	     &__typical_multi_conversion<wchar_t, ztd_char32_t, decltype(&::cnc_mwcsnrtoc32sn),
	          &::cnc_mwcsnrtoc32sn>,
	     &__typical_single_conversion<wchar_t, ztd_char32_t, decltype(&::cnc_mwcnrtoc32n),
	          &::cnc_mwcnrtoc32n>,
	     &::__typical_open_function, &::__typical_close_function));

	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf8_name(),
	     ::ztd::cnc::__cnc_detail::__exec_name(),
	     &__typical_multi_conversion<ztd_char8_t, char, decltype(&::cnc_c8snrtomcsn),
	          &::cnc_c8snrtomcsn>,
	     &__typical_single_conversion<ztd_char8_t, char, decltype(&::cnc_c8nrtomcn),
	          &::cnc_c8nrtomcn>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf8_name(),
	     ::ztd::cnc::__cnc_detail::__wide_name(),
	     &__typical_multi_conversion<ztd_char8_t, wchar_t, decltype(&::cnc_c8snrtomwcsn),
	          &::cnc_c8snrtomwcsn>,
	     &__typical_single_conversion<ztd_char8_t, wchar_t, decltype(&::cnc_c8nrtomwcn),
	          &::cnc_c8nrtomwcn>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf8_name(),
	     ::ztd::cnc::__cnc_detail::__utf8_name(),
	     &__typical_multi_conversion<ztd_char8_t, ztd_char8_t, decltype(&::cnc_c8snrtoc8sn),
	          &::cnc_c8snrtoc8sn>,
	     &__typical_single_conversion<ztd_char8_t, ztd_char8_t, decltype(&::cnc_c8nrtoc8n),
	          &::cnc_c8nrtoc8n>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf8_name(),
	     ::ztd::cnc::__cnc_detail::__utf16_name(),
	     &__typical_multi_conversion<ztd_char8_t, ztd_char16_t, decltype(&::cnc_c8snrtoc16sn),
	          &::cnc_c8snrtoc16sn>,
	     &__typical_single_conversion<ztd_char8_t, ztd_char16_t, decltype(&::cnc_c8nrtoc16n),
	          &::cnc_c8nrtoc16n>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf8_name(),
	     ::ztd::cnc::__cnc_detail::__utf32_name(),
	     &__typical_multi_conversion<ztd_char8_t, ztd_char32_t, decltype(&::cnc_c8snrtoc32sn),
	          &::cnc_c8snrtoc32sn>,
	     &__typical_single_conversion<ztd_char8_t, ztd_char32_t, decltype(&::cnc_c8nrtoc32n),
	          &::cnc_c8nrtoc32n>,
	     &::__typical_open_function, &::__typical_close_function));

	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf16_name(),
	     ::ztd::cnc::__cnc_detail::__exec_name(),
	     &__typical_multi_conversion<ztd_char16_t, char, decltype(&::cnc_c16snrtomcsn),
	          &::cnc_c16snrtomcsn>,
	     &__typical_single_conversion<ztd_char16_t, char, decltype(&::cnc_c16nrtomcn),
	          &::cnc_c16nrtomcn>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf16_name(),
	     ::ztd::cnc::__cnc_detail::__wide_name(),
	     &__typical_multi_conversion<ztd_char16_t, wchar_t, decltype(&::cnc_c16snrtomwcsn),
	          &::cnc_c16snrtomwcsn>,
	     &__typical_single_conversion<ztd_char16_t, wchar_t, decltype(&::cnc_c16nrtomwcn),
	          &::cnc_c16nrtomwcn>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf16_name(),
	     ::ztd::cnc::__cnc_detail::__utf8_name(),
	     &__typical_multi_conversion<ztd_char16_t, ztd_char8_t, decltype(&::cnc_c16snrtoc8sn),
	          &::cnc_c16snrtoc8sn>,
	     &__typical_single_conversion<ztd_char16_t, ztd_char8_t, decltype(&::cnc_c16nrtoc8n),
	          &::cnc_c16nrtoc8n>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf16_name(),
	     ::ztd::cnc::__cnc_detail::__utf16_name(),
	     &__typical_multi_conversion<ztd_char16_t, ztd_char16_t, decltype(&::cnc_c16snrtoc16sn),
	          &::cnc_c16snrtoc16sn>,
	     &__typical_single_conversion<ztd_char16_t, ztd_char16_t, decltype(&::cnc_c16nrtoc16n),
	          &::cnc_c16nrtoc16n>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf16_name(),
	     ::ztd::cnc::__cnc_detail::__utf32_name(),
	     &__typical_multi_conversion<ztd_char16_t, ztd_char32_t, decltype(&::cnc_c16snrtoc32sn),
	          &::cnc_c16snrtoc32sn>,
	     &__typical_single_conversion<ztd_char16_t, ztd_char32_t, decltype(&::cnc_c16nrtoc32n),
	          &::cnc_c16nrtoc32n>,
	     &::__typical_open_function, &::__typical_close_function));

	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf32_name(),
	     ::ztd::cnc::__cnc_detail::__exec_name(),
	     &__typical_multi_conversion<ztd_char32_t, char, decltype(&::cnc_c32snrtomcsn),
	          &::cnc_c32snrtomcsn>,
	     &__typical_single_conversion<ztd_char32_t, char, decltype(&::cnc_c32nrtomcn),
	          &::cnc_c32nrtomcn>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf32_name(),
	     ::ztd::cnc::__cnc_detail::__wide_name(),
	     &__typical_multi_conversion<ztd_char32_t, wchar_t, decltype(&::cnc_c32snrtomwcsn),
	          &::cnc_c32snrtomwcsn>,
	     &__typical_single_conversion<ztd_char32_t, wchar_t, decltype(&::cnc_c32nrtomwcn),
	          &::cnc_c32nrtomwcn>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf32_name(),
	     ::ztd::cnc::__cnc_detail::__utf8_name(),
	     &__typical_multi_conversion<ztd_char32_t, ztd_char8_t, decltype(&::cnc_c32snrtoc8sn),
	          &::cnc_c32snrtoc8sn>,
	     &__typical_single_conversion<ztd_char32_t, ztd_char8_t, decltype(&::cnc_c32nrtoc8n),
	          &::cnc_c32nrtoc8n>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf32_name(),
	     ::ztd::cnc::__cnc_detail::__utf16_name(),
	     &__typical_multi_conversion<ztd_char32_t, ztd_char16_t, decltype(&::cnc_c32snrtoc16sn),
	          &::cnc_c32snrtoc16sn>,
	     &__typical_single_conversion<ztd_char32_t, ztd_char16_t, decltype(&::cnc_c32nrtoc16n),
	          &::cnc_c32nrtoc16n>,
	     &::__typical_open_function, &::__typical_close_function));
	_CHECK_ERR_AND_RETURN(cnc_add_to_registry(__registry, ::ztd::cnc::__cnc_detail::__utf32_name(),
	     ::ztd::cnc::__cnc_detail::__utf32_name(),
	     &__typical_multi_conversion<ztd_char32_t, ztd_char32_t, decltype(&::cnc_c32snrtoc32sn),
	          &::cnc_c32snrtoc32sn>,
	     &__typical_single_conversion<ztd_char32_t, ztd_char32_t, decltype(&::cnc_c32nrtoc32n),
	          &::cnc_c32nrtoc32n>,
	     &::__typical_open_function, &::__typical_close_function));

	return CNC_OPEN_ERROR_OKAY;
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_error cnc_conv_new(
     cnc_conversion_registry* __registry, const ztd_char8_t* __from, const ztd_char8_t* __to,
     cnc_conversion** __p_out_conversion, cnc_conversion_info* __p_info) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__from == nullptr) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	if (__to == nullptr) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	size_t __from_size = ::ztd::c_string_ptr_size(__from);
	size_t __to_size   = ::ztd::c_string_ptr_size(__to);
	return cnc_conv_new_n(
	     __registry, __from_size, __from, __to_size, __to, __p_out_conversion, __p_info);
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_error cnc_conv_new_n(
     cnc_conversion_registry* __registry, size_t __from_size, const ztd_char8_t* __from,
     size_t __to_size, const ztd_char8_t* __to, cnc_conversion** __p_out_conversion,
     cnc_conversion_info* __p_info) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__from_size == 0 || __from == nullptr) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	if (__to_size == 0 || __to == nullptr) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	::std::basic_string_view<ztd_char8_t> __from_view(__from, __from_size);
	::std::basic_string_view<ztd_char8_t> __to_view(__to, __to_size);
	const __cnc_registry_entry* __from_entry;
	const __cnc_registry_entry* __to_entry;
	cnc_open_error err = ::__cnc_find_entry(
	     __registry, __from_view, __to_view, &__from_entry, &__to_entry, __p_info);
	if (err != CNC_OPEN_ERROR_OKAY) {
		return err;
	}
	size_t __before_available_space = SIZE_MAX;
	size_t __after_available_space  = SIZE_MAX - sizeof(cnc_conversion);
	size_t __max_alignment          = alignof(cnc_conversion);
	unsigned char __faux_space_buffer[500 + (500 % alignof(cnc_conversion))];
	void* __faux_space = static_cast<unsigned char*>(__faux_space_buffer) + sizeof(cnc_conversion);
	if (!__p_info->is_indirect) {
		cnc_open_error __counting_err = __from_entry->__open_function(
		     __registry, nullptr, &__after_available_space, &__max_alignment, &__faux_space);
		if (__counting_err != CNC_OPEN_ERROR_OKAY) {
			return __counting_err;
		}
	}
	else {
		cnc_open_error __counting_err = ::__intermediary_open_function(__registry, nullptr,
		     __from_entry, __to_entry, &__after_available_space, &__max_alignment, &__faux_space);
		if (__counting_err != CNC_OPEN_ERROR_OKAY) {
			return __counting_err;
		}
	}
	size_t __available_space = __before_available_space - __after_available_space;
	void* __space            = __registry->__heap.allocate(
	                __available_space, __max_alignment, &__available_space, __registry->__heap.user_data);
	if (__space == nullptr) {
		return CNC_OPEN_ERROR_ALLOCATION_FAILURE;
	}
	if (!__p_info->is_indirect) {
		return ::__cnc_open_with(
		     __registry, __from_entry, __p_out_conversion, &__available_space, &__space);
	}
	else {
		return ::__cnc_open_intermediary_with(__registry, __from_entry, __to_entry,
		     __p_out_conversion, &__available_space, &__space);
	}
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_error cnc_conv_open(
     cnc_conversion_registry* __registry, const ztd_char8_t* __from, const ztd_char8_t* __to,
     cnc_conversion** __p_out_conversion, size_t* __p_available_space, unsigned char* __space,
     cnc_conversion_info* __p_info) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__from == nullptr) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	if (__to == nullptr) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	size_t __from_size = ::ztd::c_string_ptr_size(__from);
	size_t __to_size   = ::ztd::c_string_ptr_size(__to);
	return ::cnc_conv_open_n(__registry, __from_size, __from, __to_size, __to, __p_out_conversion,
	     __p_available_space, __space, __p_info);
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_error cnc_conv_open_n(
     cnc_conversion_registry* __registry, size_t __from_size, const ztd_char8_t* __from,
     size_t __to_size, const ztd_char8_t* __to, cnc_conversion** __p_out_conversion,
     size_t* __p_available_space, void* __space,
     cnc_conversion_info* __p_info) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__from == nullptr) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	if (__to == nullptr) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	if (__p_available_space == nullptr || __space == nullptr || __p_info == nullptr) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	::std::basic_string_view<ztd_char8_t> __from_view(__from, __from_size);
	::std::basic_string_view<ztd_char8_t> __to_view(__to, __to_size);
	if (__from_view.empty()) {
		__from_view = ::ztd::cnc::__cnc_detail::__exec_name();
	}
	else if (::ztd::is_encoding_name_equal(
	              __from_view, ::ztd::cnc::__cnc_detail::__exec_alias())) {
		__from_view = ::ztd::cnc::__cnc_detail::__exec_name();
	}
	else if (::ztd::is_encoding_name_equal(
	              __from_view, ::ztd::cnc::__cnc_detail::__wide_alias())) {
		__from_view = ::ztd::cnc::__cnc_detail::__wide_name();
	}
	if (__to_view.empty()) {
		__to_view = ::ztd::cnc::__cnc_detail::__utf8_name();
	}
	else if (::ztd::is_encoding_name_equal(__to_view, ::ztd::cnc::__cnc_detail::__exec_alias())) {
		__to_view = ::ztd::cnc::__cnc_detail::__exec_name();
	}
	else if (::ztd::is_encoding_name_equal(__to_view, ::ztd::cnc::__cnc_detail::__wide_alias())) {
		__to_view = ::ztd::cnc::__cnc_detail::__wide_name();
	}

	const __cnc_registry_entry* __from_entry;
	const __cnc_registry_entry* __to_entry;
	cnc_open_error err = ::__cnc_find_entry(
	     __registry, __from_view, __to_view, &__from_entry, &__to_entry, __p_info);
	if (err != CNC_OPEN_ERROR_OKAY) {
		return err;
	}
	if (!__p_info->is_indirect) {
		return ::__cnc_open_with(
		     __registry, __from_entry, __p_out_conversion, __p_available_space, &__space);
	}
	else {
		// we have matching entries: open an intermediary
		return ::__cnc_open_intermediary_with(__registry, __from_entry, __to_entry,
		     __p_out_conversion, __p_available_space, &__space);
	}
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_conv_close(
     cnc_conversion* __conversion) ZTD_NOEXCEPT_IF_CXX_I_ {
	__conversion->__close_function(__conversion);
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_conv_delete(
     cnc_conversion* __conversion) ZTD_NOEXCEPT_IF_CXX_I_ {
	::cnc_conv_close(__conversion);
	const cnc_conversion_heap& __heap = __conversion->__registry->__heap;
	__heap.deallocate(static_cast<unsigned char*>(static_cast<void*>(__conversion)),
	     __conversion->__size, alignof(cnc_conversion), __heap.user_data);
}
