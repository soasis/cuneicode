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

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/conv.h>
#include <ztd/cuneicode/mcchar.h>
#include <ztd/cuneicode/max_input_output.h>
#include <ztd/cuneicode/pivot_info.h>
#include <ztd/cuneicode/mcchar_named.h>

#include <ztd/cuneicode/detail/conv_id.hpp>
#include <ztd/cuneicode/detail/align.hpp>
#include <ztd/cuneicode/detail/err.hpp>
#include <ztd/cuneicode/detail/heap_allocator.hpp>
#include <ztd/cuneicode/detail/conversion.hpp>
#include <ztd/cuneicode/detail/registry.hpp>
#include <ztd/cuneicode/detail/buffer_size.h>
#include <ztd/cuneicode/detail/always_complete.hpp>

#include <ztd/idk/hash.hpp>
#include <ztd/idk/size.hpp>
#include <ztd/idk/encoding_name.hpp>
#include <ztd/idk/unreachable.hpp>

#include <string_view>
#include <unordered_map>
#include <vector>
#include <new>
#include <memory>
#include <cstring>

namespace {

	template <typename _State>
	static inline _State* __basic_state(::ztd::qualify_like_t<unsigned char, _State>* __state,
	     size_t __available_space = sizeof(_State) + (sizeof(_State) - 1)) {
		using _VoidTy            = ::ztd::qualify_like_t<void, _State>;
		_VoidTy* __extra_start   = static_cast<_VoidTy*>(__state);
		_VoidTy* __aligned_extra = ::cnc::__cnc_detail::__align(
		     alignof(_State), sizeof(_State), __extra_start, __available_space);
		if (__aligned_extra == nullptr) {
			return nullptr;
		}
		return static_cast<_State*>(static_cast<_VoidTy*>(__aligned_extra));
	}

	template <typename _State>
	static inline _State* __basic_state(::ztd::qualify_like_t<void, _State>* __state,
	     size_t __available_space = sizeof(_State) + (sizeof(_State) - 1)) noexcept {
		return __basic_state<_State>(
		     static_cast<::ztd::qualify_like_t<unsigned char, _State>*>(__state),
		     __available_space);
	}

	template <typename _State>
	static inline void __basic_close_function(void* __state) noexcept {
		_State* __basic = __basic_state<_State>(__state);
		__basic->~_State();
	}

	template <typename _State, typename _CompleteFunction,
	     _CompleteFunction __state_is_complete_function>
	static inline bool __basic_state_is_complete_function(
	     const cnc_conversion*, const void* __state) noexcept {
		const _State* __basic = __basic_state<const _State>(__state);
		return __state_is_complete_function(__basic);
	}

	template <typename _State, bool _SetAssumeValidToTrue = false>
	static inline ::std::pair<cnc_open_err, _State*> __core_open_function(cnc_conversion_registry*,
	     cnc_conversion* __conversion, size_t* __p_available_space, size_t* __p_max_alignment,
	     void** __p_space) noexcept {
		const bool __is_counting = __conversion == nullptr;
		*__p_max_alignment = ::std::max(*__p_max_alignment, static_cast<size_t>(alignof(_State)));
		//[[maybe_unused]] const size_t __starting_available_space = *__p_available_space;
		void* const __starting_p_space = __p_space == nullptr ? nullptr : *__p_space;
		_State* __basic_aligned = __basic_state<_State>(__starting_p_space, *__p_available_space);
		if (__basic_aligned == nullptr) {
			// Ffffffffflubberbuckets.
			return { cnc_open_err_allocation_failure, nullptr };
		}
		unsigned char* const __aligned_space
		     = static_cast<unsigned char*>(static_cast<void*>(__basic_aligned + 1));
		const size_t __used_space
		     = (__aligned_space - static_cast<unsigned char*>(__starting_p_space));
		if (__is_counting) {
			*__p_available_space -= (__used_space);
			return { cnc_open_err_ok, nullptr };
		}
		_State* __basic = new (static_cast<void*>(__basic_aligned)) _State();
		if constexpr (_SetAssumeValidToTrue) {
			__basic->header.__assume_valid = 1u;
		}
		*__p_available_space -= __used_space;
		*__p_space = __aligned_space;
		return { cnc_open_err_ok, __basic };
	}

	template <typename _State, bool _SetAssumeValidToTrue = false>
	static inline cnc_open_err __basic_open_function(cnc_conversion_registry* __registry,
	     cnc_conversion* __conversion, size_t* __p_available_space, size_t* __p_max_alignment,
	     void** __p_space) noexcept {
		auto __code_and_ptr = __core_open_function<_State, _SetAssumeValidToTrue>(
		     __registry, __conversion, __p_available_space, __p_max_alignment, __p_space);
		return __code_and_ptr.first;
	}

	template <uint32_t _CodePageId, bool _SetAssumeValidToTrue = false>
	static inline cnc_open_err __windows_code_page_open_function(
	     cnc_conversion_registry* __registry, cnc_conversion* __conversion,
	     size_t* __p_available_space, size_t* __p_max_alignment, void** __p_space) noexcept {
		auto __code_and_ptr = __core_open_function<cnc_mcstate_t, _SetAssumeValidToTrue>(
		     __registry, __conversion, __p_available_space, __p_max_alignment, __p_space);
		cnc_mcstate_set_win32_code_page(__code_and_ptr.second, _CodePageId);
		return __code_and_ptr.first;
	}

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

	template <typename _SourceChar, typename _DestChar, typename _Func, _Func __func,
	     typename _State>
	static inline cnc_mcerr __basic_single_conversion(cnc_conversion*, size_t* __p_bytes_out_count,
	     unsigned char** __p_bytes_out, size_t* __p_bytes_in_count,
	     const unsigned char** __p_bytes_in, cnc_pivot_info* __p_pivot_info,
	     void* __state) noexcept {
		const bool __using_provided_pivot_info = __p_pivot_info != nullptr;
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
		_DestChar* __dest    = __dest_first;
		_State* __conversion = __basic_state<_State>(__state);
		cnc_mcerr __err;
		__err = __func(__is_unbounded ? nullptr : &__dest_count, &__dest, &__source_count,
		     &__source, __conversion);
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
		if (__using_provided_pivot_info) {
			__p_pivot_info->error = cnc_mcerr_ok;
		}
		return __err;
	}

	template <typename _SourceChar, typename _DestChar, typename _Func, _Func __func,
	     typename _State>
	static inline cnc_mcerr __basic_multi_conversion(cnc_conversion*, size_t* __p_bytes_out_count,
	     unsigned char** __p_bytes_out, size_t* __p_bytes_in_count,
	     const unsigned char** __p_bytes_in, cnc_pivot_info* __p_pivot_info,
	     void* __state) noexcept {
		const bool __using_provided_pivot_info = __p_pivot_info != nullptr;
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
		_DestChar* __dest    = __dest_first;
		_State* __conversion = __basic_state<_State>(__state);
		cnc_mcerr __err;
		__err = __func(__is_unbounded ? nullptr : &__dest_count, &__dest, &__source_count,
		     &__source, __conversion);
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
		if (__using_provided_pivot_info) {
			__p_pivot_info->error = cnc_mcerr_ok;
		}
		return __err;
	}

	static inline bool __typical_state_is_complete(
	     const cnc_conversion* __conversion, const void* __state) noexcept {
		return __basic_state_is_complete_function<cnc_mcstate_t,
		     decltype(&::cnc_mcstate_is_complete), &::cnc_mcstate_is_complete>(
		     __conversion, __state);
	}

	template <typename _SourceChar, typename _DestChar, typename _Func, _Func __func>
	static inline cnc_mcerr __typical_multi_conversion(cnc_conversion* __base_conversion,
	     size_t* __p_bytes_out_count, unsigned char** __p_bytes_out, size_t* __p_bytes_in_count,
	     const unsigned char** __p_bytes_in, cnc_pivot_info* __p_pivot_info,
	     void* __state) noexcept {
		return __basic_multi_conversion<_SourceChar, _DestChar, _Func, __func, cnc_mcstate_t>(
		     __base_conversion, __p_bytes_out_count, __p_bytes_out, __p_bytes_in_count,
		     __p_bytes_in, __p_pivot_info, __state);
	}

	template <typename _SourceChar, typename _DestChar, typename _Func, _Func __func>
	static inline cnc_mcerr __typical_single_conversion(
	     [[maybe_unused]] cnc_conversion* __base_conversion, size_t* __p_bytes_out_count,
	     unsigned char** __p_bytes_out, size_t* __p_bytes_in_count,
	     const unsigned char** __p_bytes_in, cnc_pivot_info* __p_pivot_info,
	     void* __state) noexcept {
		const bool __using_provided_pivot_info = __p_pivot_info != nullptr;
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
		_DestChar* __dest        = __dest_first;
		cnc_mcstate_t* __p_state = __basic_state<cnc_mcstate_t>(__state);
		cnc_mcerr __err          = __func(__is_unbounded ? nullptr : &__dest_count, &__dest,
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
		if (__using_provided_pivot_info) {
			__p_pivot_info->error = cnc_mcerr_ok;
		}
		return __err;
	}

	static inline __intermediary_cnc_conversion* __intermediary_align(
	     void* __state, [[maybe_unused]] size_t __available_space) {
		size_t __intermediary_extra_space = sizeof(__intermediary_cnc_conversion);
		void* __intermediary_extra_start  = __state;
		void* __intermediary_aligned      = ::cnc::__cnc_detail::__align(
               alignof(__intermediary_cnc_conversion), sizeof(__intermediary_cnc_conversion),
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
		     &__intermediary.__intermediary_state->__link0);
		__intermediary.__intermediary_state->__link1.__close_function(
		     &__intermediary.__intermediary_state->__link1);
		__intermediary.__intermediary_state->~__intermediary_cnc_conversion();
	}

	static inline cnc_open_err __intermediary_open_function(cnc_conversion_registry* __registry,
	     cnc_conversion* __conversion, const __cnc_registry_entry_value* __from,
	     const __cnc_registry_entry_value* __to, size_t* __p_available_space,
	     size_t* __p_max_alignment, void** __p_space) noexcept {
		if (__p_space == nullptr || *__p_space == nullptr) {
			return cnc_open_err_invalid_parameter;
		}
		const bool __is_counting = __conversion == nullptr;
		*__p_max_alignment       = ::std::max(
               *__p_max_alignment, static_cast<size_t>(alignof(__intermediary_cnc_conversion)));
		const size_t __starting_available_space = *__p_available_space;
		void* const __starting_space            = *__p_space;
		__intermediary_cnc_conversion* __intermediary_aligned
		     = __intermediary_align(*__p_space, *__p_available_space);
		const size_t __aligned_up_size
		     = (static_cast<unsigned char*>(__starting_space)
		            - reinterpret_cast<unsigned char*>(__intermediary_aligned))
		     + sizeof(__intermediary_cnc_conversion);
		if (__is_counting) {
			*__p_space = static_cast<void*>(
			     reinterpret_cast<unsigned char*>(*__p_space) + __aligned_up_size);
			*__p_available_space -= __aligned_up_size;
			cnc_open_err __link0_err = __from->__open_function(
			     __registry, nullptr, __p_available_space, __p_max_alignment, __p_space);
			if (__link0_err != cnc_open_err_ok) {
				*__p_available_space = __starting_available_space;
				*__p_space           = __starting_space;
				return __link0_err;
			}
			cnc_open_err __link1_err = __to->__open_function(
			     __registry, nullptr, __p_available_space, __p_max_alignment, __p_space);
			if (__link1_err != cnc_open_err_ok) {
				return __link1_err;
			}
			return cnc_open_err_ok;
		}
		if (__intermediary_aligned == nullptr) {
			// Ffffffffflubberbuckets.
			return cnc_open_err_allocation_failure;
		}
		__intermediary_cnc_conversion* __intermediary
		     = new (__intermediary_aligned) __intermediary_cnc_conversion();
		__intermediary->__link1_state_byte_offset = 0;
		*__p_available_space -= sizeof(__intermediary_cnc_conversion);
		*__p_space          = static_cast<void*>(reinterpret_cast<unsigned char*>(__intermediary)
		              + sizeof(__intermediary_cnc_conversion));
		void* __link0_space = *__p_space;
		size_t __space_before_link0 = *__p_available_space;
		cnc_open_err __link0_err    = __from->__open_function(
               __registry, __conversion, __p_available_space, __p_max_alignment, &__link0_space);
		if (__link0_err != cnc_open_err_ok) {
			__intermediary->~__intermediary_cnc_conversion();
			*__p_available_space = __starting_available_space;
			*__p_space           = __starting_space;
			return __link0_err;
		}
		const size_t __link0_size = static_cast<unsigned char*>(__link0_space)
		     - static_cast<unsigned char*>(*__p_space);
		*__p_space                 = __link0_space;
		size_t __link1_byte_offset = (__space_before_link0 - *__p_available_space);
		void* __link1_space        = static_cast<unsigned char*>(*__p_space)
		     + __intermediary->__link1_state_byte_offset;
		cnc_open_err __link1_err = __to->__open_function(
		     __registry, __conversion, __p_available_space, __p_max_alignment, &__link1_space);
		if (__link1_err != cnc_open_err_ok) {
			__intermediary->~__intermediary_cnc_conversion();
			__from->__close_function(__link0_space);
			*__p_available_space = __starting_available_space;
			*__p_space           = __starting_space;
			return __link1_err;
		}
		const size_t __link1_size = static_cast<unsigned char*>(__link1_space)
		     - static_cast<unsigned char*>(*__p_space);

		*__p_space                                          = __link1_space;
		__intermediary->__link0.__registry                  = __registry;
		__intermediary->__link0.__multi_conversion_function = __from->__multi_conversion_function;
		__intermediary->__link0.__single_conversion_function
		     = __from->__single_conversion_function;
		__intermediary->__link0.__state_is_complete_function
		     = __from->__state_is_complete_function;
		__intermediary->__link0.__close_function = __from->__close_function;
		__intermediary->__link0.__size           = __link0_size;
		__intermediary->__link0.__properties     = CNC_CONV_PROPS_NONE;
		__intermediary->__link0.__multi_conversion_minimum_byte_size
		     = __from->__multi_conversion_minimum_byte_size;
		__intermediary->__link1.__registry   = __registry;
		__intermediary->__link1.__properties = CNC_CONV_PROPS_NONE;
		__intermediary->__link1.__multi_conversion_minimum_byte_size
		     = __to->__multi_conversion_minimum_byte_size;
		__intermediary->__link1.__multi_conversion_function  = __to->__multi_conversion_function;
		__intermediary->__link1.__single_conversion_function = __to->__single_conversion_function;
		__intermediary->__link1.__state_is_complete_function = __to->__state_is_complete_function;
		__intermediary->__link1.__close_function             = __to->__close_function;
		__intermediary->__link1_state_byte_offset            = __link1_byte_offset;
		__intermediary->__link1.__size                       = __link1_size;
		return cnc_open_err_ok;
	}

	static inline cnc_mcerr __intermediary_multi_conversion(cnc_conversion* __conv,
	     size_t* __p_bytes_out_count, unsigned char** __p_bytes_out, size_t* __p_bytes_in_count,
	     const unsigned char** __p_bytes_in, cnc_pivot_info* __p_pivot_info,
	     void* __state) noexcept {
		const bool __using_provided_pivot_info = __p_pivot_info != nullptr;
		cnc_pivot_info __empty_pivot_info      = { 0, nullptr, cnc_mcerr_ok };
		__intermediary_states __states         = __intermediary_state(__state);
#if 0
		/* We cannot use this as-is; we'd need to do something more sophisticated to "drill down"    \
		and get an accurate conversion count for these default conversion styles after performing \
		a (failed) second half of the bulk conversion. We dot he single-style conversion here,    \
		instead. */
		constexpr const size_t __intermediate_pivot_buffer_max
		= CNC_DEFAULT_CONVERSION_INTERMEDIATE_BUFFER_SIZE;
		unsigned char __intermediate_pivot_buffer[__intermediate_pivot_buffer_max] {};
		cnc_pivot_info __backup_pivot_info
			= { __intermediate_pivot_buffer_max, __intermediate_pivot_buffer, cnc_mcerr_ok };
		cnc_pivot_info* __target_p_pivot_info
			= !__using_provided_pivot_info || __p_pivot_info->bytes == nullptr
			? &__backup_pivot_info
			: __p_pivot_info;
		
		for (;;) {
			size_t __start_bytes_in_count         = *__p_bytes_in_count;
			size_t __intermediate_bytes_out_count = __target_p_pivot_info->bytes_size;
			if (__start_bytes_in_count == 0) {
				const bool __is_link0_state_complete
				     = __states.__intermediary_state->__link0.__state_is_complete_function(
				          &__states.__intermediary_state->__link0, __states.__link0_state);
				if (__is_link0_state_complete) {
					const bool __is_link1_state_complete
					     = __states.__intermediary_state->__link1.__state_is_complete_function(
					          &__states.__intermediary_state->__link1, __states.__link1_state);
					if (__is_link1_state_complete) {
						break;
					}
					else {
						// we're only halfway done: skip to the second half of the loop!
					}
				}
				// otherwise, we are not completely done, so go through the whole setup again.
			}
			const unsigned char* __start_bytes_in   = *__p_bytes_in;
			unsigned char* __intermediate_bytes_out = __target_p_pivot_info->bytes;
			cnc_mcerr __link0res
			     = __states.__intermediary_state->__link0.__multi_conversion_function(
			          &__states.__intermediary_state->__link0, &__intermediate_bytes_out_count,
			          &__intermediate_bytes_out, __p_bytes_in_count, __p_bytes_in,
			          &__empty_pivot_info, __states.__link0_state);
			if (__link0res != cnc_mcerr_ok && __link0res != cnc_mcerr_insufficient_output) {
				// something bad happened: revert potential writes to the
				// progress variables
				__p_bytes_in_count[0] = __start_bytes_in_count;
				__p_bytes_in[0]       = __start_bytes_in;
				if (__using_provided_pivot_info) {
					__p_pivot_info->error = __link0res;
				}
				return __link0res;
			}
			size_t __intermediate_bytes_in_count
			     = __target_p_pivot_info->bytes_size - __intermediate_bytes_out_count;
			const unsigned char* __intermediate_bytes_in = __target_p_pivot_info->bytes;

			cnc_mcerr __link1res
			     = __states.__intermediary_state->__link1.__multi_conversion_function(
			          &__states.__intermediary_state->__link1, __p_bytes_out_count,
			          __p_bytes_out, &__intermediate_bytes_in_count, &__intermediate_bytes_in,
			          &__empty_pivot_info, __states.__link1_state);
			if (__link1res != cnc_mcerr_ok) {
				if (__using_provided_pivot_info) {
					__p_pivot_info->error = __link1res;
				}
				return __link1res;
			}
		}
		if (__using_provided_pivot_info) {
			__p_pivot_info->error = cnc_mcerr_ok;
		}
#else
		constexpr const size_t __intermediate_pivot_buffer_max = CNC_MC_MAX * 2;
		unsigned char __intermediate_pivot_buffer[__intermediate_pivot_buffer_max] {};
		cnc_pivot_info __backup_pivot_info
		     = { __intermediate_pivot_buffer_max, __intermediate_pivot_buffer, cnc_mcerr_ok };
		cnc_pivot_info* __target_p_pivot_info
		     = !__using_provided_pivot_info || __p_pivot_info->bytes == nullptr
		     ? &__backup_pivot_info
		     : __p_pivot_info;

		for (;;) {
			size_t __intermediate_bytes_out_count         = __target_p_pivot_info->bytes_size;
			unsigned char* __intermediate_bytes_out       = __target_p_pivot_info->bytes;
			const size_t __initial_bytes_in_count         = __p_bytes_in_count[0];
			const unsigned char* const __initial_bytes_in = __p_bytes_in[0];
			cnc_mcerr __link0res
			     = __states.__intermediary_state->__link0.__single_conversion_function(
			          &__states.__intermediary_state->__link0, &__intermediate_bytes_out_count,
			          &__intermediate_bytes_out, __p_bytes_in_count, __p_bytes_in,
			          &__empty_pivot_info, __states.__link0_state);
			if (__link0res != cnc_mcerr_ok) {
				if (__using_provided_pivot_info) {
					__p_pivot_info->error = __link0res;
				}
				return __link0res;
			}
			if (__using_provided_pivot_info) {
				__p_pivot_info->error = cnc_mcerr_ok;
			}
			size_t __intermediate_bytes_in_count
			     = __target_p_pivot_info->bytes_size - __intermediate_bytes_out_count;
			const unsigned char* __intermediate_bytes_in = __target_p_pivot_info->bytes;
			cnc_mcerr __link1res
			     = __states.__intermediary_state->__link1.__single_conversion_function(
			          &__states.__intermediary_state->__link1, __p_bytes_out_count,
			          __p_bytes_out, &__intermediate_bytes_in_count, &__intermediate_bytes_in,
			          &__empty_pivot_info, __states.__link1_state);
			if (__link1res != cnc_mcerr_ok) {
				// revert any changes
				__p_bytes_in_count[0] = __initial_bytes_in_count;
				__p_bytes_in[0]       = __initial_bytes_in;
				return __link1res;
			}
			if (*__p_bytes_in_count == 0) {
				if (!__states.__intermediary_state->__link0.__state_is_complete_function(
				         __conv, __states.__link0_state)) {
					continue;
				}
				if (!__states.__intermediary_state->__link1.__state_is_complete_function(
				         __conv, __states.__link1_state)) {
					continue;
				}
				// we are done, then
				break;
			}
		}
#endif
		return cnc_mcerr_ok;
	}

	static inline cnc_mcerr __intermediary_single_conversion(cnc_conversion*,
	     size_t* __p_bytes_out_count, unsigned char** __p_bytes_out, size_t* __p_bytes_in_count,
	     const unsigned char** __p_bytes_in, cnc_pivot_info* __p_pivot_info,
	     void* __state) noexcept {
		const bool __using_provided_pivot_info       = __p_pivot_info != nullptr;
		const size_t __intermediate_pivot_buffer_max = CNC_MC_MAX * 2;
		unsigned char __intermediate_pivot_buffer[__intermediate_pivot_buffer_max] {};
		cnc_pivot_info __backup_pivot_info
		     = { __intermediate_pivot_buffer_max, __intermediate_pivot_buffer, cnc_mcerr_ok };
		cnc_pivot_info* __target_p_pivot_info
		     = !__using_provided_pivot_info || __p_pivot_info->bytes == nullptr
		     ? &__backup_pivot_info
		     : __p_pivot_info;
		cnc_pivot_info __empty_pivot_info       = { 0, nullptr, cnc_mcerr_ok };
		__intermediary_states __states          = __intermediary_state(__state);
		size_t __intermediate_bytes_out_count   = __target_p_pivot_info->bytes_size;
		unsigned char* __intermediate_bytes_out = __target_p_pivot_info->bytes;
		cnc_mcerr __link0res
		     = __states.__intermediary_state->__link0.__single_conversion_function(
		          &__states.__intermediary_state->__link0, &__intermediate_bytes_out_count,
		          &__intermediate_bytes_out, __p_bytes_in_count, __p_bytes_in,
		          &__empty_pivot_info, __states.__link0_state);
		if (__link0res != cnc_mcerr_ok) {
			if (__using_provided_pivot_info) {
				__p_pivot_info->error = __link0res;
			}
			return __link0res;
		}
		if (__using_provided_pivot_info) {
			__p_pivot_info->error = cnc_mcerr_ok;
		}
		size_t __intermediate_bytes_in_count
		     = __target_p_pivot_info->bytes_size - __intermediate_bytes_out_count;
		const unsigned char* __intermediate_bytes_in = __target_p_pivot_info->bytes;
		cnc_mcerr __link1res
		     = __states.__intermediary_state->__link1.__single_conversion_function(
		          &__states.__intermediary_state->__link1, __p_bytes_out_count, __p_bytes_out,
		          &__intermediate_bytes_in_count, &__intermediate_bytes_in, &__empty_pivot_info,
		          __states.__link1_state);
		return __link1res;
	}

	static inline cnc_open_err __cnc_open_with(cnc_conversion_registry* __registry,
	     const __cnc_registry_entry_value* __entry, cnc_conversion** __out_p_conversion,
	     size_t* __p_available_space, void** __p_space) {
		const size_t __starting_available_space = *__p_available_space;
		void* __target                          = *__p_space;
		void* __aligned_target                  = ::cnc::__cnc_detail::__align(
               alignof(cnc_conversion), sizeof(cnc_conversion), __target, *__p_available_space);
		if (__aligned_target == nullptr) {
			*__p_available_space = __starting_available_space;
			return cnc_open_err_insufficient_output;
		}
		cnc_conversion* __base_conv = new (__aligned_target) cnc_conversion();
		__base_conv->__registry     = __registry;
		__base_conv->__size         = __starting_available_space;
		__base_conv->__properties   = CNC_CONV_PROPS_NONE;
		__base_conv->__multi_conversion_minimum_byte_size
		     = __entry->__multi_conversion_minimum_byte_size;
		__base_conv->__state_is_complete_function = __entry->__state_is_complete_function;
		__base_conv->__multi_conversion_function  = __entry->__multi_conversion_function;
		__base_conv->__single_conversion_function = __entry->__single_conversion_function;
		__base_conv->__close_function             = __entry->__close_function;
		*__out_p_conversion                       = __base_conv;
		*__p_space = static_cast<unsigned char*>(static_cast<void*>(__base_conv + 1));
		*__p_available_space -= sizeof(cnc_conversion);
		size_t __max_alignment = alignof(cnc_conversion);
		cnc_open_err __err     = __entry->__open_function(
               __registry, __base_conv, __p_available_space, &__max_alignment, __p_space);
		if (__err != cnc_open_err_ok) {
			__base_conv->~cnc_conversion();
			*__out_p_conversion  = nullptr;
			*__p_available_space = __starting_available_space;
			return __err;
		}
		return __err;
	}

	static inline cnc_open_err __cnc_open_intermediary_with(cnc_conversion_registry* __registry,
	     const __cnc_registry_entry_value* __from, const __cnc_registry_entry_value* __to,
	     cnc_conversion** __out_p_conversion, size_t* __p_available_space, void** __p_space) {
		const size_t __starting_available_space = *__p_available_space;
		void* __target                          = *__p_space;
		void* __aligned_target                  = ::cnc::__cnc_detail::__align(
               alignof(cnc_conversion), sizeof(cnc_conversion), __target, *__p_available_space);
		if (__aligned_target == nullptr) {
			*__p_available_space = __starting_available_space;
			return cnc_open_err_insufficient_output;
		}
		size_t __max_alignment                    = alignof(cnc_conversion);
		cnc_conversion* __base_conv               = new (__aligned_target) cnc_conversion();
		__base_conv->__registry                   = __registry;
		__base_conv->__size                       = __starting_available_space;
		__base_conv->__properties                 = CNC_CONV_PROPS_INDIRECT;
		__base_conv->__state_is_complete_function = nullptr;
		__base_conv->__multi_conversion_function  = &__intermediary_multi_conversion;
		__base_conv->__single_conversion_function = &__intermediary_single_conversion;
		__base_conv->__close_function             = &__intermediary_close_function;
		*__p_available_space -= sizeof(cnc_conversion);
		*__p_space
		     = static_cast<void*>(static_cast<unsigned char*>(static_cast<void*>(__base_conv))
		          + sizeof(cnc_conversion));
		*__out_p_conversion = __base_conv;
		cnc_open_err __err = ::__intermediary_open_function(__registry, __base_conv, __from, __to,
		     __p_available_space, &__max_alignment, __p_space);
		if (__err != cnc_open_err_ok) {
			__base_conv->~cnc_conversion();
			*__out_p_conversion  = nullptr;
			*__p_available_space = __starting_available_space;
			return __err;
		}

		return __err;
	}
} // namespace

//////
/// @brief This function must be marked extern because it must have a stable external symbol w.r.t
/// this library.
extern cnc_mcerr __cnc_multi_from_single_conversion(cnc_conversion* __conversion,
     size_t* __p_output_bytes_size, unsigned char** __p_output_bytes, size_t* __p_input_bytes_size,
     const unsigned char** __p_input_bytes, cnc_pivot_info* __p_pivot_info, void* __user_data)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if (__p_input_bytes_size == nullptr || __p_input_bytes == nullptr) {
		return cnc_mcerr_ok;
	}
	const unsigned char*& __input_bytes = *__p_input_bytes;
	size_t& __input_bytes_size          = *__p_input_bytes_size;
	if (__input_bytes == nullptr || __input_bytes_size == 0) {
		return cnc_mcerr_ok;
	}
	for (;;) {
		cnc_mcerr __err = __conversion->__single_conversion_function(__conversion,
		     __p_output_bytes_size, __p_output_bytes, __p_input_bytes_size, __p_input_bytes,
		     __p_pivot_info, __user_data);
		switch (__err) {
		case cnc_mcerr_ok:
			if (__input_bytes_size > 0) {
				// loop around!
				continue;
			}
			if (__conversion->__state_is_complete_function(__conversion, __user_data)) {
				// we're done!
				return cnc_mcerr_ok;
			}
		default:
			break;
		}
		return __err;
	}
}

//////
/// @brief This function must be marked extern because it must have a stable external symbol w.r.t
/// this library.
extern cnc_mcerr __cnc_single_from_multi_conversion(cnc_conversion* __conversion,
     size_t* __p_output_bytes_size, unsigned char** __p_output_bytes, size_t* __p_input_bytes_size,
     const unsigned char** __p_input_bytes, cnc_pivot_info* __p_pivot_info, void* __user_data)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if (__p_input_bytes_size == nullptr || __p_input_bytes == nullptr) {
		return cnc_mcerr_ok;
	}
	const unsigned char*& __input_bytes = *__p_input_bytes;
	size_t& __input_bytes_size          = *__p_input_bytes_size;
	if (__input_bytes == nullptr || __input_bytes_size == 0) {
		return cnc_mcerr_ok;
	}
	for (size_t __input_bytes_limit = __conversion->__multi_conversion_minimum_byte_size;;
	     __input_bytes_limit += __conversion->__multi_conversion_minimum_byte_size) {
		size_t __len = __input_bytes_limit;
		cnc_mcerr __err
		     = __conversion->__multi_conversion_function(__conversion, __p_output_bytes_size,
		          __p_output_bytes, &__len, &__input_bytes, __p_pivot_info, __user_data);
		switch (__err) {
		case cnc_mcerr_incomplete_input:
			// alright, so we just need more input:
			// we go A G A N E !
			if (__input_bytes_limit < __input_bytes_size) {
				continue;
			}
			break;
		default:
			break;
		}
		__input_bytes_size -= (__input_bytes_limit - __len);
		return __err;
	}
	// if we reach here, we just simply do not have enough input: bail out
	ZTD_UNREACHABLE();
}

extern cnc_open_err __cnc_add_default_registry_entries(cnc_conversion_registry* __registry)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_open_err __err;
#define _CHECK_ERR_AND_RETURN(...)                 \
	__err = __VA_ARGS__;                          \
	if (__err != cnc_open_err::cnc_open_err_ok) { \
		return __err;                            \
	}                                             \
	static_assert(true, "")

	// Basic core functionality
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__exec_name(),
	     ::cnc::__cnc_detail::__exec_name(),
	     &__typical_multi_conversion<char, char, decltype(&::cnc_mcsnrtomcsn), &::cnc_mcsnrtomcsn>,
	     &__typical_single_conversion<char, char, decltype(&::cnc_mcnrtomcn), &::cnc_mcnrtomcn>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__exec_name(),
	     ::cnc::__cnc_detail::__wide_name(),
	     &__typical_multi_conversion<char, wchar_t, decltype(&::cnc_mcsnrtomwcsn),
	          &::cnc_mcsnrtomwcsn>,
	     &__typical_single_conversion<char, wchar_t, decltype(&::cnc_mcnrtomwcn),
	          &::cnc_mcnrtomwcn>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__exec_name(),
	     ::cnc::__cnc_detail::__utf8_name(),
	     &__typical_multi_conversion<char, ztd_char8_t, decltype(&::cnc_mcsnrtoc8sn),
	          &::cnc_mcsnrtoc8sn>,
	     &__typical_single_conversion<char, ztd_char8_t, decltype(&::cnc_mcnrtoc8n),
	          &::cnc_mcnrtoc8n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__exec_name(),
	     ::cnc::__cnc_detail::__utf16_name(),
	     &__typical_multi_conversion<char, ztd_char16_t, decltype(&::cnc_mcsnrtoc16sn),
	          &::cnc_mcsnrtoc16sn>,
	     &__typical_single_conversion<char, ztd_char16_t, decltype(&::cnc_mcnrtoc16n),
	          &::cnc_mcnrtoc16n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__exec_name(),
	     ::cnc::__cnc_detail::__utf32_name(),
	     &__typical_multi_conversion<char, ztd_char32_t, decltype(&::cnc_mcsnrtoc32sn),
	          &::cnc_mcsnrtoc32sn>,
	     &__typical_single_conversion<char, ztd_char32_t, decltype(&::cnc_mcnrtoc32n),
	          &::cnc_mcnrtoc32n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));

	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__wide_name(),
	     ::cnc::__cnc_detail::__exec_name(),
	     &__typical_multi_conversion<wchar_t, char, decltype(&::cnc_mwcsnrtomcsn),
	          &::cnc_mwcsnrtomcsn>,
	     &__typical_single_conversion<wchar_t, char, decltype(&::cnc_mwcnrtomcn),
	          &::cnc_mwcnrtomcn>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__wide_name(),
	     ::cnc::__cnc_detail::__wide_name(),
	     &__typical_multi_conversion<wchar_t, wchar_t, decltype(&::cnc_mwcsnrtomwcsn),
	          &::cnc_mwcsnrtomwcsn>,
	     &__typical_single_conversion<wchar_t, wchar_t, decltype(&::cnc_mwcnrtomwcn),
	          &::cnc_mwcnrtomwcn>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__wide_name(),
	     ::cnc::__cnc_detail::__utf8_name(),
	     &__typical_multi_conversion<wchar_t, ztd_char8_t, decltype(&::cnc_mwcsnrtoc8sn),
	          &::cnc_mwcsnrtoc8sn>,
	     &__typical_single_conversion<wchar_t, ztd_char8_t, decltype(&::cnc_mwcnrtoc8n),
	          &::cnc_mwcnrtoc8n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__wide_name(),
	     ::cnc::__cnc_detail::__utf16_name(),
	     &__typical_multi_conversion<wchar_t, ztd_char16_t, decltype(&::cnc_mwcsnrtoc16sn),
	          &::cnc_mwcsnrtoc16sn>,
	     &__typical_single_conversion<wchar_t, ztd_char16_t, decltype(&::cnc_mwcnrtoc16n),
	          &::cnc_mwcnrtoc16n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__wide_name(),
	     ::cnc::__cnc_detail::__utf32_name(),
	     &__typical_multi_conversion<wchar_t, ztd_char32_t, decltype(&::cnc_mwcsnrtoc32sn),
	          &::cnc_mwcsnrtoc32sn>,
	     &__typical_single_conversion<wchar_t, ztd_char32_t, decltype(&::cnc_mwcnrtoc32n),
	          &::cnc_mwcnrtoc32n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));

	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf8_name(),
	     ::cnc::__cnc_detail::__exec_name(),
	     &__typical_multi_conversion<ztd_char8_t, char, decltype(&::cnc_c8snrtomcsn),
	          &::cnc_c8snrtomcsn>,
	     &__typical_single_conversion<ztd_char8_t, char, decltype(&::cnc_c8nrtomcn),
	          &::cnc_c8nrtomcn>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf8_name(),
	     ::cnc::__cnc_detail::__wide_name(),
	     &__typical_multi_conversion<ztd_char8_t, wchar_t, decltype(&::cnc_c8snrtomwcsn),
	          &::cnc_c8snrtomwcsn>,
	     &__typical_single_conversion<ztd_char8_t, wchar_t, decltype(&::cnc_c8nrtomwcn),
	          &::cnc_c8nrtomwcn>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf8_name(),
	     ::cnc::__cnc_detail::__utf8_name(),
	     &__typical_multi_conversion<ztd_char8_t, ztd_char8_t, decltype(&::cnc_c8snrtoc8sn),
	          &::cnc_c8snrtoc8sn>,
	     &__typical_single_conversion<ztd_char8_t, ztd_char8_t, decltype(&::cnc_c8nrtoc8n),
	          &::cnc_c8nrtoc8n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf8_name(),
	     ::cnc::__cnc_detail::__utf16_name(),
	     &__typical_multi_conversion<ztd_char8_t, ztd_char16_t, decltype(&::cnc_c8snrtoc16sn),
	          &::cnc_c8snrtoc16sn>,
	     &__typical_single_conversion<ztd_char8_t, ztd_char16_t, decltype(&::cnc_c8nrtoc16n),
	          &::cnc_c8nrtoc16n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf8_name(),
	     ::cnc::__cnc_detail::__utf32_name(),
	     &__typical_multi_conversion<ztd_char8_t, ztd_char32_t, decltype(&::cnc_c8snrtoc32sn),
	          &::cnc_c8snrtoc32sn>,
	     &__typical_single_conversion<ztd_char8_t, ztd_char32_t, decltype(&::cnc_c8nrtoc32n),
	          &::cnc_c8nrtoc32n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));

	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf16_name(),
	     ::cnc::__cnc_detail::__exec_name(),
	     &__typical_multi_conversion<ztd_char16_t, char, decltype(&::cnc_c16snrtomcsn),
	          &::cnc_c16snrtomcsn>,
	     &__typical_single_conversion<ztd_char16_t, char, decltype(&::cnc_c16nrtomcn),
	          &::cnc_c16nrtomcn>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf16_name(),
	     ::cnc::__cnc_detail::__wide_name(),
	     &__typical_multi_conversion<ztd_char16_t, wchar_t, decltype(&::cnc_c16snrtomwcsn),
	          &::cnc_c16snrtomwcsn>,
	     &__typical_single_conversion<ztd_char16_t, wchar_t, decltype(&::cnc_c16nrtomwcn),
	          &::cnc_c16nrtomwcn>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf16_name(),
	     ::cnc::__cnc_detail::__utf8_name(),
	     &__typical_multi_conversion<ztd_char16_t, ztd_char8_t, decltype(&::cnc_c16snrtoc8sn),
	          &::cnc_c16snrtoc8sn>,
	     &__typical_single_conversion<ztd_char16_t, ztd_char8_t, decltype(&::cnc_c16nrtoc8n),
	          &::cnc_c16nrtoc8n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf16_name(),
	     ::cnc::__cnc_detail::__utf16_name(),
	     &__typical_multi_conversion<ztd_char16_t, ztd_char16_t, decltype(&::cnc_c16snrtoc16sn),
	          &::cnc_c16snrtoc16sn>,
	     &__typical_single_conversion<ztd_char16_t, ztd_char16_t, decltype(&::cnc_c16nrtoc16n),
	          &::cnc_c16nrtoc16n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf16_name(),
	     ::cnc::__cnc_detail::__utf32_name(),
	     &__typical_multi_conversion<ztd_char16_t, ztd_char32_t, decltype(&::cnc_c16snrtoc32sn),
	          &::cnc_c16snrtoc32sn>,
	     &__typical_single_conversion<ztd_char16_t, ztd_char32_t, decltype(&::cnc_c16nrtoc32n),
	          &::cnc_c16nrtoc32n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));

	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf32_name(),
	     ::cnc::__cnc_detail::__exec_name(),
	     &__typical_multi_conversion<ztd_char32_t, char, decltype(&::cnc_c32snrtomcsn),
	          &::cnc_c32snrtomcsn>,
	     &__typical_single_conversion<ztd_char32_t, char, decltype(&::cnc_c32nrtomcn),
	          &::cnc_c32nrtomcn>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf32_name(),
	     ::cnc::__cnc_detail::__wide_name(),
	     &__typical_multi_conversion<ztd_char32_t, wchar_t, decltype(&::cnc_c32snrtomwcsn),
	          &::cnc_c32snrtomwcsn>,
	     &__typical_single_conversion<ztd_char32_t, wchar_t, decltype(&::cnc_c32nrtomwcn),
	          &::cnc_c32nrtomwcn>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf32_name(),
	     ::cnc::__cnc_detail::__utf8_name(),
	     &__typical_multi_conversion<ztd_char32_t, ztd_char8_t, decltype(&::cnc_c32snrtoc8sn),
	          &::cnc_c32snrtoc8sn>,
	     &__typical_single_conversion<ztd_char32_t, ztd_char8_t, decltype(&::cnc_c32nrtoc8n),
	          &::cnc_c32nrtoc8n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf32_name(),
	     ::cnc::__cnc_detail::__utf16_name(),
	     &__typical_multi_conversion<ztd_char32_t, ztd_char16_t, decltype(&::cnc_c32snrtoc16sn),
	          &::cnc_c32snrtoc16sn>,
	     &__typical_single_conversion<ztd_char32_t, ztd_char16_t, decltype(&::cnc_c32nrtoc16n),
	          &::cnc_c32nrtoc16n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf32_name(),
	     ::cnc::__cnc_detail::__utf32_name(),
	     &__typical_multi_conversion<ztd_char32_t, ztd_char32_t, decltype(&::cnc_c32snrtoc32sn),
	          &::cnc_c32snrtoc32sn>,
	     &__typical_single_conversion<ztd_char32_t, ztd_char32_t, decltype(&::cnc_c32nrtoc32n),
	          &::cnc_c32nrtoc32n>,
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t>,
	     &::__basic_close_function<cnc_mcstate_t>));

	// Basic unchecked functionality (UTF only)
	_CHECK_ERR_AND_RETURN(
	     cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf8_unchecked_name(),
	          ::cnc::__cnc_detail::__utf8_unchecked_name(),
	          &__typical_multi_conversion<ztd_char8_t, ztd_char8_t, decltype(&::cnc_c8snrtoc8sn),
	               &::cnc_c8snrtoc8sn>,
	          &__typical_single_conversion<ztd_char8_t, ztd_char8_t, decltype(&::cnc_c8nrtoc8n),
	               &::cnc_c8nrtoc8n>,
	          &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t, true>,
	          &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(
	     cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf8_unchecked_name(),
	          ::cnc::__cnc_detail::__utf16_unchecked_name(),
	          &__typical_multi_conversion<ztd_char8_t, ztd_char16_t, decltype(&::cnc_c8snrtoc16sn),
	               &::cnc_c8snrtoc16sn>,
	          &__typical_single_conversion<ztd_char8_t, ztd_char16_t, decltype(&::cnc_c8nrtoc16n),
	               &::cnc_c8nrtoc16n>,
	          &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t, true>,
	          &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(
	     cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf8_unchecked_name(),
	          ::cnc::__cnc_detail::__utf32_unchecked_name(),
	          &__typical_multi_conversion<ztd_char8_t, ztd_char32_t, decltype(&::cnc_c8snrtoc32sn),
	               &::cnc_c8snrtoc32sn>,
	          &__typical_single_conversion<ztd_char8_t, ztd_char32_t, decltype(&::cnc_c8nrtoc32n),
	               &::cnc_c8nrtoc32n>,
	          &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t, true>,
	          &::__basic_close_function<cnc_mcstate_t>));

	_CHECK_ERR_AND_RETURN(
	     cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf16_unchecked_name(),
	          ::cnc::__cnc_detail::__utf8_unchecked_name(),
	          &__typical_multi_conversion<ztd_char16_t, ztd_char8_t, decltype(&::cnc_c16snrtoc8sn),
	               &::cnc_c16snrtoc8sn>,
	          &__typical_single_conversion<ztd_char16_t, ztd_char8_t, decltype(&::cnc_c16nrtoc8n),
	               &::cnc_c16nrtoc8n>,
	          &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t, true>,
	          &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(
	     cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf16_unchecked_name(),
	          ::cnc::__cnc_detail::__utf16_unchecked_name(),
	          &__typical_multi_conversion<ztd_char16_t, ztd_char16_t,
	               decltype(&::cnc_c16snrtoc16sn), &::cnc_c16snrtoc16sn>,
	          &__typical_single_conversion<ztd_char16_t, ztd_char16_t,
	               decltype(&::cnc_c16nrtoc16n), &::cnc_c16nrtoc16n>,
	          &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t, true>,
	          &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(
	     cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf16_unchecked_name(),
	          ::cnc::__cnc_detail::__utf32_unchecked_name(),
	          &__typical_multi_conversion<ztd_char16_t, ztd_char32_t,
	               decltype(&::cnc_c16snrtoc32sn), &::cnc_c16snrtoc32sn>,
	          &__typical_single_conversion<ztd_char16_t, ztd_char32_t,
	               decltype(&::cnc_c16nrtoc32n), &::cnc_c16nrtoc32n>,
	          &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t, true>,
	          &::__basic_close_function<cnc_mcstate_t>));

	_CHECK_ERR_AND_RETURN(
	     cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf32_unchecked_name(),
	          ::cnc::__cnc_detail::__utf8_unchecked_name(),
	          &__typical_multi_conversion<ztd_char32_t, ztd_char8_t, decltype(&::cnc_c32snrtoc8sn),
	               &::cnc_c32snrtoc8sn>,
	          &__typical_single_conversion<ztd_char32_t, ztd_char8_t, decltype(&::cnc_c32nrtoc8n),
	               &::cnc_c32nrtoc8n>,
	          &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t, true>,
	          &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(
	     cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf32_unchecked_name(),
	          ::cnc::__cnc_detail::__utf16_unchecked_name(),
	          &__typical_multi_conversion<ztd_char32_t, ztd_char16_t,
	               decltype(&::cnc_c32snrtoc16sn), &::cnc_c32snrtoc16sn>,
	          &__typical_single_conversion<ztd_char32_t, ztd_char16_t,
	               decltype(&::cnc_c32nrtoc16n), &::cnc_c32nrtoc16n>,
	          &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t, true>,
	          &::__basic_close_function<cnc_mcstate_t>));
	_CHECK_ERR_AND_RETURN(
	     cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf32_unchecked_name(),
	          ::cnc::__cnc_detail::__utf32_unchecked_name(),
	          &__typical_multi_conversion<ztd_char32_t, ztd_char32_t,
	               decltype(&::cnc_c32snrtoc32sn), &::cnc_c32snrtoc32sn>,
	          &__typical_single_conversion<ztd_char32_t, ztd_char32_t,
	               decltype(&::cnc_c32nrtoc32n), &::cnc_c32nrtoc32n>,
	          &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t, true>,
	          &::__basic_close_function<cnc_mcstate_t>));

#define _ADD_MCN_NAMED_ENCODING_BASIC(                                                             \
     _NAME, _SUFFIX, _DECODE_STATE, _DECODE_COMPLETE_FN, _ENCODE_STATE, _ENCODE_COMPLETE_FN)       \
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, (const ztd_char8_t*)(_NAME),            \
	     ::cnc::__cnc_detail::__utf32_name(),                                                     \
	     &__basic_multi_conversion<ztd_char_t, ztd_char32_t,                                      \
	          decltype(&::cnc_mcsnrtoc32sn_##_SUFFIX), &::cnc_mcsnrtoc32sn_##_SUFFIX,             \
	          _DECODE_STATE>,                                                                     \
	     &__basic_single_conversion<ztd_char_t, ztd_char32_t,                                     \
	          decltype(&::cnc_mcnrtoc32n_##_SUFFIX), &::cnc_mcnrtoc32n_##_SUFFIX, _DECODE_STATE>, \
	     &::__basic_state_is_complete_function<_DECODE_STATE, decltype(_DECODE_COMPLETE_FN),      \
	          _DECODE_COMPLETE_FN>,                                                               \
	     &::__basic_open_function<_DECODE_STATE>, &::__basic_close_function<_DECODE_STATE>));     \
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf32_name(),    \
	     (const ztd_char8_t*)(_NAME),                                                             \
	     &__basic_multi_conversion<ztd_char32_t, ztd_char_t,                                      \
	          decltype(&::cnc_c32snrtomcsn_##_SUFFIX), &::cnc_c32snrtomcsn_##_SUFFIX,             \
	          _ENCODE_STATE>,                                                                     \
	     &__basic_single_conversion<ztd_char32_t, ztd_char_t,                                     \
	          decltype(&::cnc_c32nrtomcn_##_SUFFIX), &::cnc_c32nrtomcn_##_SUFFIX, _ENCODE_STATE>, \
	     &::__basic_state_is_complete_function<_ENCODE_STATE, decltype(_ENCODE_COMPLETE_FN),      \
	          _ENCODE_COMPLETE_FN>,                                                               \
	     &::__basic_open_function<_ENCODE_STATE>, &::__basic_close_function<_ENCODE_STATE>))

#define _ADD_MCN_NAMED_ENCODING(_NAME, _SUFFIX)                                                 \
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, (const ztd_char8_t*)(_NAME),         \
	     ::cnc::__cnc_detail::__utf32_name(),                                                  \
	     &__typical_multi_conversion<ztd_char_t, ztd_char32_t,                                 \
	          decltype(&::cnc_mcsnrtoc32sn_##_SUFFIX), &::cnc_mcsnrtoc32sn_##_SUFFIX>,         \
	     &__typical_single_conversion<ztd_char_t, ztd_char32_t,                                \
	          decltype(&::cnc_mcnrtoc32n_##_SUFFIX), &::cnc_mcnrtoc32n_##_SUFFIX>,             \
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t, true>,        \
	     &::__basic_close_function<cnc_mcstate_t>));                                           \
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf32_name(), \
	     (const ztd_char8_t*)(_NAME),                                                          \
	     &__typical_multi_conversion<ztd_char32_t, ztd_char_t,                                 \
	          decltype(&::cnc_c32snrtomcsn_##_SUFFIX), &::cnc_c32snrtomcsn_##_SUFFIX>,         \
	     &__typical_single_conversion<ztd_char32_t, ztd_char_t,                                \
	          decltype(&::cnc_c32nrtomcn_##_SUFFIX), &::cnc_c32nrtomcn_##_SUFFIX>,             \
	     &::__typical_state_is_complete, &::__basic_open_function<cnc_mcstate_t, true>,        \
	     &::__basic_close_function<cnc_mcstate_t>))

	_ADD_MCN_NAMED_ENCODING("ascii", ascii);
	_CHECK_ERR_AND_RETURN(cnc_registry_add_alias(__registry, "ANSI_X3.4-1968", "ascii"));
	_ADD_MCN_NAMED_ENCODING("atari st", atari_st);
	_ADD_MCN_NAMED_ENCODING("atascii", atascii);

	_ADD_MCN_NAMED_ENCODING("gbk", gbk);
	_ADD_MCN_NAMED_ENCODING("big5-hkscs", big5_hkscs);
	_CHECK_ERR_AND_RETURN(cnc_registry_add_alias(__registry, "big5", "big5-hkscs"));
	_ADD_MCN_NAMED_ENCODING("gb18030", gb18030);

	_ADD_MCN_NAMED_ENCODING("kamenicky", kamenicky);
	_ADD_MCN_NAMED_ENCODING("kazakh strk1048", kazakh_strk1048);
	_ADD_MCN_NAMED_ENCODING("koi8-u", koi8_u);
	_ADD_MCN_NAMED_ENCODING("koi8-r", koi8_r);

	_ADD_MCN_NAMED_ENCODING("ibm-424", ibm_424_hebrew_bulletin);
	_ADD_MCN_NAMED_ENCODING("ibm-856", ibm_856_hebrew);
	_ADD_MCN_NAMED_ENCODING("ibm-866", ibm_866_cyrillic);
	_ADD_MCN_NAMED_ENCODING("ibm-1006", ibm_1006_urdu);

	_ADD_MCN_NAMED_ENCODING("iso-8859-1-1985", iso_8859_1_1985);
	_ADD_MCN_NAMED_ENCODING("iso-8859-1-1998", iso_8859_1_1998);
	_ADD_MCN_NAMED_ENCODING("iso-8859-1", iso_8859_1);
	_ADD_MCN_NAMED_ENCODING("iso-8859-2", iso_8859_2);
	_ADD_MCN_NAMED_ENCODING("iso-8859-3", iso_8859_3);
	_ADD_MCN_NAMED_ENCODING("iso-8859-4", iso_8859_4);
	_ADD_MCN_NAMED_ENCODING("iso-8859-5", iso_8859_5);
	_ADD_MCN_NAMED_ENCODING("iso-8859-6", iso_8859_6);
	_ADD_MCN_NAMED_ENCODING("iso-8859-7", iso_8859_7);
	_ADD_MCN_NAMED_ENCODING("iso-8859-8", iso_8859_8);
	_ADD_MCN_NAMED_ENCODING("iso-8859-10", iso_8859_10);
	_ADD_MCN_NAMED_ENCODING("iso-8859-13", iso_8859_13);
	_ADD_MCN_NAMED_ENCODING("iso-8859-14", iso_8859_14);
	_ADD_MCN_NAMED_ENCODING("iso-8859-15", iso_8859_15);
	_ADD_MCN_NAMED_ENCODING("iso-8859-16", iso_8859_16);

	_ADD_MCN_NAMED_ENCODING("mac_roman", mac_roman);
	_CHECK_ERR_AND_RETURN(cnc_registry_add_alias(__registry, "x-mac-roman", "mac-roman"));
	_ADD_MCN_NAMED_ENCODING("mac-cyrillic", mac_cyrillic);
	_CHECK_ERR_AND_RETURN(cnc_registry_add_alias(__registry, "x-mac-cyrillic", "mac-cyrillic"));
	_ADD_MCN_NAMED_ENCODING("shift-jis-x0208", shift_jis_x0208);
	// Shift_JIS aliases to the one available in the WHATWG datbase first and foremost
	_CHECK_ERR_AND_RETURN(cnc_registry_add_alias(__registry, "shift-jis", "shift-jis-x0208"));

	_ADD_MCN_NAMED_ENCODING("tatar-ansi", tatar_ansi);
	// Tatar aliases to the Windows/ANSI version by-default
	_CHECK_ERR_AND_RETURN(cnc_registry_add_alias(__registry, "tatar", "tatar-ansi"));
	_ADD_MCN_NAMED_ENCODING("tatar-ascii", tatar_ascii);

	_ADD_MCN_NAMED_ENCODING("windows-473", windows_437_dos_latin_us);
	_CHECK_ERR_AND_RETURN(
	     cnc_registry_add_alias(__registry, "windows-473-dos-latin-us", "windows-473"));
	_CHECK_ERR_AND_RETURN(
	     cnc_registry_add_alias(__registry, "windows-473-dos-latin", "windows-473"));
	_ADD_MCN_NAMED_ENCODING("windows-865", windows_865_dos_nordic);
	_CHECK_ERR_AND_RETURN(
	     cnc_registry_add_alias(__registry, "windows-865-dos-nordic", "windows-865"));
	_ADD_MCN_NAMED_ENCODING("windows-874", windows_874);
	_ADD_MCN_NAMED_ENCODING("windows-1251", windows_1251);
	_ADD_MCN_NAMED_ENCODING("windows-1252", windows_1252);
	_ADD_MCN_NAMED_ENCODING("windows-1253", windows_1253);
	_ADD_MCN_NAMED_ENCODING("windows-1254", windows_1254);
	_ADD_MCN_NAMED_ENCODING("windows-1255", windows_1255);
	_ADD_MCN_NAMED_ENCODING("windows-1256", windows_1257);
	_ADD_MCN_NAMED_ENCODING("windows-1257", windows_1257);
	_ADD_MCN_NAMED_ENCODING("windows-1258", windows_1258);

	_ADD_MCN_NAMED_ENCODING_BASIC("petscii", petscii, cnc_petscii_state_t,
	     &::cnc::__cnc_detail::__always_complete<cnc_petscii_state_t>, cnc_petscii_state_t,
	     &::cnc::__cnc_detail::__always_complete<cnc_petscii_state_t>);
	_ADD_MCN_NAMED_ENCODING_BASIC("punycode", punycode, cnc_pny_decode_state_t,
	     &cnc_pny_decode_state_is_complete, cnc_pny_encode_state_t,
	     &cnc_pny_encode_state_is_complete);
	_ADD_MCN_NAMED_ENCODING_BASIC("punycode idna", punycode_idna, cnc_pny_decode_state_t,
	     &cnc_pny_decode_state_is_complete, cnc_pny_encode_state_t,
	     &cnc_pny_encode_state_is_complete);

	// Aliases
	// execution / wide execution aliases
	_CHECK_ERR_AND_RETURN(cnc_registry_add_alias_c8(
	     __registry, ::cnc::__cnc_detail::__wide_alias(), ::cnc::__cnc_detail::__wide_name()));
	_CHECK_ERR_AND_RETURN(cnc_registry_add_alias_c8(
	     __registry, ::cnc::__cnc_detail::__exec_alias(), ::cnc::__cnc_detail::__exec_name()));

	// Windows conversions
#if ZTD_IS_ON(ZTD_PLATFORM_WINDOWS)
#define _ADD_WINDOWS_CODE_PAGE_MCN(_NAME, _ALIAS, _CODE_PAGE_ID)                                \
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, (const ztd_char8_t*)(_NAME),         \
	     ::cnc::__cnc_detail::__utf32_name(),                                                  \
	     &__typical_multi_conversion<char, ztd_char32_t,                                       \
	          decltype(&::cnc_mcsnrtoc32sn_windows_code_page),                                 \
	          &::cnc_mcsnrtoc32sn_windows_code_page>,                                          \
	     &__typical_single_conversion<char, ztd_char32_t,                                      \
	          decltype(&::cnc_mcnrtoc32n_windows_code_page),                                   \
	          &::cnc_mcnrtoc32n_windows_code_page>,                                            \
	     &::__typical_state_is_complete, &::__windows_code_page_open_function<_CODE_PAGE_ID>,  \
	     &::__basic_close_function<cnc_mcstate_t>));                                           \
	_CHECK_ERR_AND_RETURN(cnc_registry_add_c8(__registry, ::cnc::__cnc_detail::__utf32_name(), \
	     (const ztd_char8_t*)(_NAME),                                                          \
	     &__typical_multi_conversion<ztd_char32_t, char,                                       \
	          decltype(&::cnc_c32snrtomcsn_windows_code_page),                                 \
	          &::cnc_c32snrtomcsn_windows_code_page>,                                          \
	     &__typical_single_conversion<ztd_char32_t, char,                                      \
	          decltype(&::cnc_c32nrtomcn_windows_code_page),                                   \
	          &::cnc_c32nrtomcn_windows_code_page>,                                            \
	     &::__typical_state_is_complete, &::__windows_code_page_open_function<_CODE_PAGE_ID>,  \
	     &::__basic_close_function<cnc_mcstate_t>));                                           \
	_CHECK_ERR_AND_RETURN(cnc_registry_add_alias_c8(                                           \
	     __registry, (const ztd_char8_t*)(_ALIAS), (const ztd_char8_t*)(_NAME)));

	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-037", "win32-037", 037);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-437", "win32-437", 437);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-500", "win32-500", 500);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-708", "win32-708", 708);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-709", "win32-709", 709);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-710", "win32-710", 710);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-720", "win32-720", 720);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-737", "win32-737", 737);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-775", "win32-775", 775);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-850", "win32-850", 850);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-852", "win32-852", 852);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-855", "win32-855", 855);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-857", "win32-857", 857);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-858", "win32-858", 858);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-860", "win32-860", 860);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-861", "win32-861", 861);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-862", "win32-862", 862);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-863", "win32-863", 863);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-864", "win32-864", 864);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-865", "win32-865", 865);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-866", "win32-866", 866);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-869", "win32-869", 869);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-870", "win32-870", 870);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-874", "win32-874", 874);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-875", "win32-875", 875);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-932", "win32-932", 932);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-936", "win32-936", 936);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-949", "win32-949", 949);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-950", "win32-950", 950);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1026", "win32-1026", 1026);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1047", "win32-1047", 1047);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1140", "win32-1140", 1140);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1141", "win32-1141", 1141);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1142", "win32-1142", 1142);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1143", "win32-1143", 1143);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1144", "win32-1144", 1144);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1145", "win32-1145", 1145);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1146", "win32-1146", 1146);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1147", "win32-1147", 1147);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1148", "win32-1148", 1148);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1149", "win32-1149", 1149);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1200", "win32-1200", 1200);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1201", "win32-1201", 1201);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1250", "win32-1250", 1250);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1251", "win32-1251", 1251);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1252", "win32-1252", 1252);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1253", "win32-1253", 1253);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1254", "win32-1254", 1254);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1255", "win32-1255", 1255);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1256", "win32-1256", 1256);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1257", "win32-1257", 1257);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1258", "win32-1258", 1258);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-1361", "win32-1361", 1361);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10000", "win32-10000", 10000);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10001", "win32-10001", 10001);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10002", "win32-10002", 10002);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10003", "win32-10003", 10003);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10004", "win32-10004", 10004);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10005", "win32-10005", 10005);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10006", "win32-10006", 10006);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10007", "win32-10007", 10007);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10008", "win32-10008", 10008);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10010", "win32-10010", 10010);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10017", "win32-10017", 10017);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10021", "win32-10021", 10021);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10029", "win32-10029", 10029);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10079", "win32-10079", 10079);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10081", "win32-10081", 10081);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-10082", "win32-10082", 10082);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-12000", "win32-12000", 12000);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-12001", "win32-12001", 12001);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20000", "win32-20000", 20000);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20001", "win32-20001", 20001);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20002", "win32-20002", 20002);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20003", "win32-20003", 20003);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20004", "win32-20004", 20004);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20005", "win32-20005", 20005);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20105", "win32-20105", 20105);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20106", "win32-20106", 20106);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20107", "win32-20107", 20107);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20108", "win32-20108", 20108);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20127", "win32-20127", 20127);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20261", "win32-20261", 20261);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20269", "win32-20269", 20269);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20273", "win32-20273", 20273);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20277", "win32-20277", 20277);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20278", "win32-20278", 20278);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20280", "win32-20280", 20280);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20284", "win32-20284", 20284);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20285", "win32-20285", 20285);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20290", "win32-20290", 20290);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20297", "win32-20297", 20297);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20420", "win32-20420", 20420);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20423", "win32-20423", 20423);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20424", "win32-20424", 20424);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20833", "win32-20833", 20833);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20838", "win32-20838", 20838);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20866", "win32-20866", 20866);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20871", "win32-20871", 20871);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20880", "win32-20880", 20880);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20905", "win32-20905", 20905);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20924", "win32-20924", 20924);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20932", "win32-20932", 20932);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20936", "win32-20936", 20936);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-20949", "win32-20949", 20949);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-21025", "win32-21025", 21025);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-21866", "win32-21866", 21866);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-28591", "win32-28591", 28591);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-28592", "win32-28592", 28592);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-28593", "win32-28593", 28593);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-28594", "win32-28594", 28594);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-28595", "win32-28595", 28595);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-28596", "win32-28596", 28596);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-28597", "win32-28597", 28597);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-28598", "win32-28598", 28598);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-28599", "win32-28599", 28599);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-28603", "win32-28603", 28603);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-28605", "win32-28605", 28605);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-29001", "win32-29001", 29001);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-38598", "win32-38598", 38598);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50220", "win32-50220", 50220);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50221", "win32-50221", 50221);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50222", "win32-50222", 50222);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50225", "win32-50225", 50225);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50227", "win32-50227", 50227);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50229", "win32-50229", 50229);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50930", "win32-50930", 50930);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50931", "win32-50931", 50931);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50933", "win32-50933", 50933);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50935", "win32-50935", 50935);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50936", "win32-50936", 50936);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50937", "win32-50937", 50937);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-50939", "win32-50939", 50939);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-51932", "win32-51932", 51932);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-51936", "win32-51936", 51936);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-51949", "win32-51949", 51949);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-51950", "win32-51950", 51950);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-52936", "win32-52936", 52936);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-54936", "win32-54936", 54936);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-57002", "win32-57002", 57002);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-57003", "win32-57003", 57003);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-57004", "win32-57004", 57004);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-57005", "win32-57005", 57005);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-57006", "win32-57006", 57006);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-57007", "win32-57007", 57007);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-57008", "win32-57008", 57008);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-57009", "win32-57009", 57009);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-57010", "win32-57010", 57010);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-57011", "win32-57011", 57011);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-65000", "win32-65000", 65000);
	_ADD_WINDOWS_CODE_PAGE_MCN("win32-code_page-65001", "win32-65001", 65001);
#endif

#undef _ADD_WINDOWS_CODE_PAGE_MCN
#undef _ADD_MCN_NAMED_ENCODING
#undef _ADD_MCN_NAMED_ENCODING_BASIC
#undef _CHECK_ERR_AND_RETURN

	return cnc_open_err_ok;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_open_err cnc_conv_new_c8(cnc_conversion_registry* __registry, const ztd_char8_t* __from,
     const ztd_char8_t* __to, cnc_conversion** __out_p_conversion, cnc_conversion_info* __p_info)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if (__from == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	if (__to == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	size_t __from_size = ztdc_c_string_ptr_size_c8(__from);
	size_t __to_size   = ztdc_c_string_ptr_size_c8(__to);
	return cnc_conv_new_c8n(
	     __registry, __from_size, __from, __to_size, __to, __out_p_conversion, __p_info);
}



ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_open_err cnc_conv_new(cnc_conversion_registry* __registry, const char* __from, const char* __to,
     cnc_conversion** __out_p_conversion, cnc_conversion_info* __p_info)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_conv_new_c8(__registry, reinterpret_cast<const ztd_char8_t*>(__from),
	     reinterpret_cast<const ztd_char8_t*>(__to), __out_p_conversion, __p_info);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_open_err cnc_conv_new_select_c8n(cnc_conversion_registry* __registry, size_t __from_size,
     const ztd_char8_t __from[ZTD_PTR_EXTENT(__from_size)], size_t __to_size,
     const ztd_char8_t __to[ZTD_PTR_EXTENT(__to_size)],
     cnc_indirect_selection_c8_function __selection, cnc_conversion** __out_p_conversion,
     cnc_conversion_info* __p_info) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if (__from_size == 0 || __from == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	if (__to_size == 0 || __to == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	if (__selection == nullptr) {
		__selection = reinterpret_cast<cnc_indirect_selection_c8_function*>(
		     &__cnc_detail_select_everything_okay);
	}
	__cnc_u8string_view __from_view(__from, __from_size);
	__cnc_u8string_view __to_view(__to, __to_size);
	__from_view = ::__cnc_resolve_alias(__registry, __from_view);
	__to_view   = ::__cnc_resolve_alias(__registry, __to_view);

	const __cnc_registry_entry_value* __from_entry;
	const __cnc_registry_entry_value* __to_entry;
	cnc_open_err __err = ::__cnc_find_entry(
	     __registry, __from_view, __to_view, __selection, &__from_entry, &__to_entry, __p_info);
	if (__err != cnc_open_err_ok) {
		return __err;
	}
	constexpr const size_t __default_guess = (sizeof(cnc_conversion) + 128)
	     + ((sizeof(cnc_conversion) + 128) % alignof(cnc_conversion));
	constexpr const size_t __buffer_space
	     = (ZTD_CUNEICODE_INTERMEDIATE_BUFFER_SUGGESTED_BYTE_SIZE_I_ / 2) > __default_guess
	     ? (ZTD_CUNEICODE_INTERMEDIATE_BUFFER_SUGGESTED_BYTE_SIZE_I_ / 2)
	     : __default_guess;
	size_t __before_available_space = __buffer_space;
	size_t __after_available_space  = __buffer_space - sizeof(cnc_conversion);
	size_t __max_alignment          = alignof(cnc_conversion);
	alignas(cnc_conversion) unsigned char __faux_space_buffer[__buffer_space];
	void* __faux_space = static_cast<unsigned char*>(__faux_space_buffer) + sizeof(cnc_conversion);
	if (!__p_info->is_indirect) {
		cnc_open_err __counting_err = __from_entry->__open_function(
		     __registry, nullptr, &__after_available_space, &__max_alignment, &__faux_space);
		if (__counting_err != cnc_open_err_ok) {
			return __counting_err;
		}
	}
	else {
		cnc_open_err __counting_err = ::__intermediary_open_function(__registry, nullptr,
		     __from_entry, __to_entry, &__after_available_space, &__max_alignment, &__faux_space);
		if (__counting_err != cnc_open_err_ok) {
			return __counting_err;
		}
	}
	size_t __available_space = __before_available_space - __after_available_space;
	void* __space            = __registry->__heap.allocate(
          __available_space, __max_alignment, &__available_space, __registry->__heap.user_data);
	if (__space == nullptr) {
		return cnc_open_err_allocation_failure;
	}
	if (!__p_info->is_indirect) {
		return ::__cnc_open_with(
		     __registry, __from_entry, __out_p_conversion, &__available_space, &__space);
	}
	else {
		return ::__cnc_open_intermediary_with(__registry, __from_entry, __to_entry,
		     __out_p_conversion, &__available_space, &__space);
	}
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_open_err cnc_conv_new_c8n(cnc_conversion_registry* __registry, size_t __from_size,
     const ztd_char8_t __from[ZTD_PTR_EXTENT(__from_size)], size_t __to_size,
     const ztd_char8_t __to[ZTD_PTR_EXTENT(__to_size)], cnc_conversion** __out_p_conversion,
     cnc_conversion_info* __p_info) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_conv_new_select_c8n(
	     __registry, __from_size, __from, __to_size, __to, nullptr, __out_p_conversion, __p_info);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_open_err cnc_conv_new_n(cnc_conversion_registry* __registry, size_t __from_size,
     const char __from[ZTD_PTR_EXTENT(__from_size)], size_t __to_size,
     const char __to[ZTD_PTR_EXTENT(__to_size)], cnc_conversion** __out_p_conversion,
     cnc_conversion_info* __p_info) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_conv_new_c8n(__registry, __from_size, reinterpret_cast<const ztd_char8_t*>(__from),
	     __to_size, reinterpret_cast<const ztd_char8_t*>(__to), __out_p_conversion, __p_info);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_open_err cnc_conv_open_c8(cnc_conversion_registry* __registry, const ztd_char8_t* __from,
     const ztd_char8_t* __to, cnc_conversion** __out_p_conversion, size_t* __p_available_space,
     unsigned char* __space, cnc_conversion_info* __p_info) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if (__from == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	if (__to == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	size_t __from_size = ztdc_c_string_ptr_size_c8(__from);
	size_t __to_size   = ztdc_c_string_ptr_size_c8(__to);
	return ::cnc_conv_open_c8n(__registry, __from_size, __from, __to_size, __to,
	     __out_p_conversion, __p_available_space, __space, __p_info);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_open_err cnc_conv_open_c8_select(cnc_conversion_registry* __registry, const ztd_char8_t* __from,
     const ztd_char8_t* __to, cnc_indirect_selection_c8_function* __selection,
     cnc_conversion** __out_p_conversion, size_t* __p_available_space, void* __space,
     cnc_conversion_info* __p_info) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if (__from == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	if (__to == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	size_t __from_size = ztdc_c_string_ptr_size_c8(__from);
	size_t __to_size   = ztdc_c_string_ptr_size_c8(__to);
	return ::cnc_conv_open_select_c8n(__registry, __from_size, __from, __to_size, __to,
	     __selection, __out_p_conversion, __p_available_space, __space, __p_info);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_open_err cnc_conv_open_select_c8n(cnc_conversion_registry* __registry, size_t __from_size,
     const ztd_char8_t __from[ZTD_PTR_EXTENT(__from_size)], size_t __to_size,
     const ztd_char8_t __to[ZTD_PTR_EXTENT(__to_size)],
     cnc_indirect_selection_c8_function* __selection, cnc_conversion** __out_p_conversion,
     size_t* __p_available_space, void* __space, cnc_conversion_info* __p_info)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if (__from == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	if (__to == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	if (__p_available_space == nullptr || __space == nullptr || __p_info == nullptr) {
		return cnc_open_err_invalid_parameter;
	}

	__cnc_u8string_view __from_view(__from, __from_size);
	__cnc_u8string_view __to_view(__to, __to_size);
	if (__from_view.empty()) {
		__from_view = ::cnc::__cnc_detail::__exec_name();
	}
	if (__to_view.empty()) {
		__to_view = ::cnc::__cnc_detail::__utf8_name();
	}
	__from_view = ::__cnc_resolve_alias(__registry, __from_view);
	__to_view   = ::__cnc_resolve_alias(__registry, __to_view);

	const __cnc_registry_entry_value* __from_entry;
	const __cnc_registry_entry_value* __to_entry;
	cnc_open_err err = ::__cnc_find_entry(
	     __registry, __from_view, __to_view, __selection, &__from_entry, &__to_entry, __p_info);
	if (err != cnc_open_err_ok) {
		return err;
	}
	if (!__p_info->is_indirect) {
		return ::__cnc_open_with(
		     __registry, __from_entry, __out_p_conversion, __p_available_space, &__space);
	}
	else {
		// we have matching entries: open an intermediary
		return ::__cnc_open_intermediary_with(__registry, __from_entry, __to_entry,
		     __out_p_conversion, __p_available_space, &__space);
	}
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_open_err cnc_conv_open_select_n(cnc_conversion_registry* __registry, size_t __from_size,
     const char __from[ZTD_PTR_EXTENT(__from_size)], size_t __to_size,
     const char __to[ZTD_PTR_EXTENT(__to_size)], cnc_indirect_selection_function* __selection,
     cnc_conversion** __out_p_conversion, size_t* __p_available_space, void* __space,
     cnc_conversion_info* __p_info) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_conv_open_select_c8n(__registry, __from_size,
	     reinterpret_cast<const ztd_char8_t*>(__from), __to_size,
	     reinterpret_cast<const ztd_char8_t*>(__to),
	     reinterpret_cast<cnc_indirect_selection_c8_function*>(__selection), __out_p_conversion,
	     __p_available_space, __space, __p_info);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_open_err cnc_conv_open_c8n(cnc_conversion_registry* __registry, size_t __from_size,
     const ztd_char8_t __from[ZTD_PTR_EXTENT(__from_size)], size_t __to_size,
     const ztd_char8_t __to[ZTD_PTR_EXTENT(__to_size)], cnc_conversion** __out_p_conversion,
     size_t* __p_available_space, void* __space, cnc_conversion_info* __p_info)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_conv_open_select_c8n(__registry, __from_size, __from, __to_size, __to, nullptr,
	     __out_p_conversion, __p_available_space, __space, __p_info);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_open_err cnc_conv_open_n(cnc_conversion_registry* __registry, size_t __from_size,
     const char __from[ZTD_PTR_EXTENT(__from_size)], size_t __to_size,
     const char __to[ZTD_PTR_EXTENT(__to_size)], cnc_conversion** __out_p_conversion,
     size_t* __p_available_space, void* __space, cnc_conversion_info* __p_info)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return cnc_conv_open_c8n(__registry, __from_size, reinterpret_cast<const ztd_char8_t*>(__from),
	     __to_size, reinterpret_cast<const ztd_char8_t*>(__to), __out_p_conversion,
	     __p_available_space, __space, __p_info);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
void cnc_conv_close(cnc_conversion* __conversion) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if (__conversion == nullptr)
		return;
	__conversion->__close_function(__conversion + 1);
	__conversion->~cnc_conversion();
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
void cnc_conv_delete(cnc_conversion* __conversion) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	if (__conversion == nullptr)
		return;
	cnc_conversion_registry* __registry                   = __conversion->__registry;
	const ::std::size_t __cnc_size                        = __conversion->__size;
	cnc_heap_deallocate_function* const __heap_deallocate = __registry->__heap.deallocate;
	void* const __heap_user_data                          = __registry->__heap.user_data;
	::cnc_conv_close(__conversion);
	__heap_deallocate(static_cast<unsigned char*>(static_cast<void*>(__conversion)), __cnc_size,
	     alignof(cnc_conversion), __heap_user_data);
}
