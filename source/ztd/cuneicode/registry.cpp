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

#include <ztd/cuneicode/detail/registry.hpp>

#include <ztd/cuneicode/registry.h>
#include <ztd/cuneicode/detail/conv_id.hpp>
#include <ztd/cuneicode/detail/align.hpp>
#include <ztd/cuneicode/detail/conversion.hpp>

#include <ztd/idk/empty_string.hpp>
#include <ztd/idk/assert.hpp>

#include <limits>

namespace {
	static inline void* __cnc_default_allocate(size_t __requested_size, size_t __alignment,
	     size_t* __p_actual_size, void* __user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
		(void)__user_data;
		(void)__alignment;
		unsigned char* __ptr = static_cast<unsigned char*>(malloc(__requested_size));
		if (__ptr == nullptr) {
			return nullptr;
		}
		*__p_actual_size = __requested_size;
		return __ptr;
	}

	static inline void* __cnc_default_reallocate(void* __original, size_t __requested_size,
	     size_t __alignment, size_t* __p_actual_size, void* __user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
		(void)__user_data;
		(void)__alignment;
		unsigned char* __ptr = static_cast<unsigned char*>(realloc(__original, __requested_size));
		if (__ptr == nullptr) {
			*__p_actual_size = 0;
			return nullptr;
		}
		*__p_actual_size = __requested_size;
		return __ptr;
	}

	static inline void* __cnc_default_expand_allocation(void* __original, size_t __original_size,
	     size_t __alignment, size_t __expand_left, size_t __expand_right, size_t* __p_actual_size,
	     void* __user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
		(void)__original;
		(void)__original_size;
		(void)__alignment;
		(void)__expand_left;
		(void)__expand_right;
		(void)__user_data;
		*__p_actual_size = 0;
		return nullptr;
	}

	static inline void* __cnc_default_shrink_allocation(void* __original, size_t __original_size,
	     size_t __alignment, size_t __reduce_left, size_t __reduce_right, size_t* __p_actual_size,
	     void* __user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
		(void)__original;
		(void)__original_size;
		(void)__alignment;
		(void)__reduce_left;
		(void)__reduce_right;
		(void)__user_data;
		*__p_actual_size = 0;
		return nullptr;
	}

	static inline void __cnc_default_deallocate(void* __ptr, size_t __ptr_size, size_t __alignment,
	     void* __user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
		(void)__ptr_size;
		(void)__user_data;
		(void)__alignment;
		free(__ptr);
	}

	static inline void __default_close_function(void* __conversion) ZTD_NOEXCEPT_IF_CXX_I_ {
		// nothing to do with the extra space
		(void)__conversion;
	}

	static inline cnc_open_error __default_open_function(cnc_conversion_registry* __registry,
	     cnc_conversion* __conversion, size_t* __p_available_space, size_t* __p_max_alignment,
	     void** __p_space) ZTD_NOEXCEPT_IF_CXX_I_ {
		// nothing to do with the extra space
		(void)__registry;
		(void)__conversion;
		(void)__p_available_space;
		(void)__p_max_alignment;
		(void)__p_space;
		return CNC_OPEN_ERROR_OKAY;
	}
} // namespace

cnc_open_error __cnc_find_entry(cnc_conversion_registry* __registry,
     ::std::basic_string_view<ztd_char8_t> __from_view,
     ::std::basic_string_view<ztd_char8_t> __to_view, const __cnc_registry_entry** __p_from_entry,
     const __cnc_registry_entry** __p_to_entry,
     cnc_conversion_info* __p_info) ZTD_NOEXCEPT_IF_CXX_I_ {
	__cnc_registry_entry_key __key { __from_view, __to_view };
	auto __direct_it       = __registry->__paths.find(__key);
	bool __has_direct_path = __direct_it != __registry->__paths.cend();
	if (__has_direct_path) {
		__p_info->is_indirect        = false;
		__p_info->from_code_data     = __direct_it->first.__from.data();
		__p_info->from_code_size     = __direct_it->first.__from.size();
		__p_info->to_code_data       = __direct_it->first.__to.data();
		__p_info->to_code_size       = __direct_it->first.__to.size();
		__p_info->indirect_code_data = ::ztd::empty_string<ztd_char8_t>();
		__p_info->indirect_code_size = 0;
		*__p_from_entry              = &__direct_it->second;
		*__p_to_entry                = nullptr;
		return CNC_OPEN_ERROR_OKAY;
	}

	// no direct path: search indirect paths
	constexpr const size_t __from_index = 0;
	constexpr const size_t __to_index   = 1;
	const ::ztd::cnc::__cnc_detail::__cnc_defined_slot __valid_indirect_paths[][2]
	     = { { ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf8,
		         ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf8 },
		       { ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf8,
		            ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf8_unchecked },
		       { ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf8_unchecked,
		            ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf8_unchecked },
		       { ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf16,
		            ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf16 },
		       { ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf16,
		            ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf16_unchecked },
		       { ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf16_unchecked,
		            ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf16_unchecked },
		       { ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf32,
		            ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf32 },
		       { ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf32,
		            ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf32_unchecked },
		       { ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf32_unchecked,
		            ::ztd::cnc::__cnc_detail::__cnc_defined_slot::__utf32_unchecked } };
	constexpr const size_t __valid_indirect_paths_count
	     = sizeof(__valid_indirect_paths) / sizeof(__valid_indirect_paths[0]);
	const ::std::pair<const __cnc_registry_entry_key, __cnc_registry_entry>*
	     __path_entries[__valid_indirect_paths_count][2]
	     = { { nullptr, nullptr }, { nullptr, nullptr }, { nullptr, nullptr },
		       { nullptr, nullptr }, { nullptr, nullptr }, { nullptr, nullptr },
		       { nullptr, nullptr }, { nullptr, nullptr }, { nullptr, nullptr } };
	for (const auto& __entry : __registry->__paths) {
		const __cnc_registry_entry_key& __key = __entry.first;
		if (__key.__from != __from_view)
			continue;
		::ztd::cnc::__cnc_detail::__cnc_defined_slot __slot;
		if (!::ztd::cnc::__cnc_detail::__is_unicode_identifier(__key.__to, __slot))
			continue;
		__path_entries[static_cast<size_t>(__slot)][__from_index] = &__entry;
	}
	for (size_t __slot_index = 0; __slot_index < __valid_indirect_paths_count; ++__slot_index) {
		auto& __entry_pair_array = __path_entries[__slot_index];
		const auto* __from_entry = __entry_pair_array[__from_index];
		if (__from_entry == nullptr)
			continue;
		const ::ztd::cnc::__cnc_detail::__cnc_defined_slot __slot
		     = __valid_indirect_paths[__slot_index][__to_index];
		const auto& __slot_id_view = __to_name(__slot);
		for (const auto& __entry : __registry->__paths) {
			if (__key.__from != __slot_id_view)
				continue;
			if (__key.__to != __to_view)
				continue;
			// we found a good one,
			// but keep looping!
			// we want to prefer the utf32 intermediary over all other
			// intermediaries
			__entry_pair_array[__to_index] = &__entry;
			break;
		}
	}
	for (size_t __slot_index = __valid_indirect_paths_count; __slot_index-- > 0;) {
		const auto& __entry_pair_array = __path_entries[__slot_index];
		const auto* __from_entry       = __entry_pair_array[__from_index];
		const auto* __to_entry         = __entry_pair_array[__to_index];
		// do we have matching entries?
		if (__from_entry == nullptr || __to_entry == nullptr)
			continue;
		__p_info->is_indirect        = true;
		__p_info->from_code_data     = __from_entry->first.__from.data();
		__p_info->from_code_size     = __from_entry->first.__from.size();
		__p_info->to_code_data       = __to_entry->first.__to.data();
		__p_info->to_code_size       = __to_entry->first.__to.size();
		__p_info->indirect_code_data = __to_entry->first.__from.data();
		__p_info->indirect_code_size = __to_entry->first.__from.size();
		*__p_from_entry              = &__from_entry->second;
		*__p_to_entry                = &__to_entry->second;
		return CNC_OPEN_ERROR_OKAY;
	}
	return CNC_OPEN_ERROR_NO_CONVERSION_PATH;
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_error cnc_new_registry(
     cnc_conversion_registry** __p_out_registry,
     cnc_registry_options __registry_options) ZTD_NOEXCEPT_IF_CXX_I_ {
	try {
		cnc_conversion_registry* __registry = nullptr;
		cnc_conversion_heap __heap { nullptr, __cnc_default_allocate, __cnc_default_reallocate,
			__cnc_default_expand_allocation, __cnc_default_shrink_allocation,
			__cnc_default_deallocate };
		cnc_open_error err = ::cnc_open_registry(&__registry, &__heap, __registry_options);
		if (err != CNC_OPEN_ERROR_OKAY) {
			return err;
		}
		*__p_out_registry = __registry;
		return CNC_OPEN_ERROR_OKAY;
	}
	catch (...) {
		*__p_out_registry = nullptr;
	}
	return CNC_OPEN_ERROR_ALLOCATION_FAILURE;
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_error cnc_open_registry(
     cnc_conversion_registry** __p_out_registry, cnc_conversion_heap* __p_original_heap,
     cnc_registry_options __registry_options) ZTD_NOEXCEPT_IF_CXX_I_ {
	size_t __available_space      = (::std::numeric_limits<size_t>::max)();
	size_t __necessary_space      = 0;
	size_t __used_space           = 0;
	void* __target                = nullptr;
	cnc_conversion_heap* __p_heap = __p_original_heap;
	cnc_conversion_heap __default_heap { nullptr, __cnc_default_allocate, __cnc_default_reallocate,
		__cnc_default_expand_allocation, __cnc_default_shrink_allocation,
		__cnc_default_deallocate };
	if (__p_heap == nullptr) {
		__p_heap = &__default_heap;
	}
	::ztd::cnc::__cnc_detail::__align(alignof(cnc_conversion_registry),
	     sizeof(cnc_conversion_registry), __target, __available_space, __necessary_space);
	__target = static_cast<void*>(__p_heap->allocate(
	     __necessary_space, alignof(cnc_conversion_registry), &__used_space, __p_heap->user_data));
	if (__target == nullptr) {
		return CNC_OPEN_ERROR_ALLOCATION_FAILURE;
	}
	void* __aligned_target = ::ztd::cnc::__cnc_detail::__align(alignof(cnc_conversion_registry),
	     sizeof(cnc_conversion_registry), __target, __available_space);
	if (__aligned_target == nullptr) {
		return CNC_OPEN_ERROR_ALLOCATION_FAILURE;
	}
	cnc_conversion_registry* __aligned_registry
	     = new (__aligned_target) cnc_conversion_registry(*__p_heap);
	*__p_out_registry = __aligned_registry;
	if ((__registry_options & CNC_REGISTRY_OPTIONS_EMPTY) != CNC_REGISTRY_OPTIONS_EMPTY) {
		// if it does not need to be empty, pre-fill with system entities
		cnc_open_error __err = ::__cnc_add_default_registry_entries(*__p_out_registry);
		if (__err != CNC_OPEN_ERROR_OKAY) {
			return __err;
		}
	}
	return CNC_OPEN_ERROR_OKAY;
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_error cnc_add_to_registry(
     cnc_conversion_registry* __registry, const ztd_char8_t* __from, const ztd_char8_t* __to,
     cnc_conversion_function* __multi_conversion_function,
     cnc_conversion_function* __single_conversion_function, cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	size_t __from_size = ::ztd::c_string_ptr_size(__from);
	size_t __to_size   = ::ztd::c_string_ptr_size(__to);
	return ::cnc_add_to_registry_n(__registry, __from_size, __from, __to_size, __to,
	     __multi_conversion_function, __single_conversion_function, __open_function,
	     __close_function);
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_error
cnc_add_to_registry_n(cnc_conversion_registry* __registry, size_t __from_size,
     const ztd_char8_t* __from, size_t __to_size, const ztd_char8_t* __to,
     cnc_conversion_function* __multi_conversion_function,
     cnc_conversion_function* __single_conversion_function, cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__from_size == 0 || __from == nullptr) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	if (__to_size == 0 || __to == nullptr) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	if (__multi_conversion_function == nullptr && __single_conversion_function == nullptr) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	if (__multi_conversion_function == nullptr) {
		__multi_conversion_function = &__cnc_multi_from_single_conversion;
	}
	else if (__single_conversion_function == nullptr) {
		__single_conversion_function = &__cnc_single_from_multi_conversion;
	}

	// must specify both, or neither
	bool __open_is_null  = __open_function == nullptr;
	bool __close_is_null = __close_function == nullptr;
	if (__open_is_null != __close_is_null) {
		return CNC_OPEN_ERROR_INVALID_PARAMETER;
	}
	if (__open_is_null) {
		__open_function = &::__default_open_function;
	}
	if (__close_is_null) {
		__close_function = &::__default_close_function;
	}

	try {
		__path_key& __from_key = *std::prev(__registry->__path_keys.end(), 2);
		__path_key& __to_key   = *std::prev(__registry->__path_keys.end(), 1);
		__from_key.assign(__from, __from + __from_size);
		__to_key.assign(__to, __to + __to_size);
		__from_key.push_back(static_cast<ztd_char8_t>(0));
		__to_key.push_back(static_cast<ztd_char8_t>(0));
		__cnc_registry_entry_key __registry_entry_key {
			::std::basic_string_view<ztd_char8_t>(__from_key.data(), __from_key.size() - 1),
			::std::basic_string_view<ztd_char8_t>(__to_key.data(), __to_key.size() - 1)
		};
		__cnc_registry_entry __registry_entry { __multi_conversion_function,
			__single_conversion_function, __open_function, __close_function };
		auto __path_it = __registry->__paths.find(__registry_entry_key);
		if (__path_it != __registry->__paths.cend()) {
			// update existing conversion
			__path_it->second = __registry_entry;
		}
		else {
			__registry->__path_keys.emplace_back(
			     __inline_size_guess, __path_key_allocator(__registry->__heap));
			__registry->__path_keys.emplace_back(
			     __inline_size_guess, __path_key_allocator(__registry->__heap));
			__registry->__paths.emplace_hint(::std::move(__path_it),
			     ::std::move(__registry_entry_key), ::std::move(__registry_entry));
		}
	}
	catch (...) {
		return CNC_OPEN_ERROR_ALLOCATION_FAILURE;
	}

	return CNC_OPEN_ERROR_OKAY;
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_error
cnc_add_to_registry_single(cnc_conversion_registry* __registry, const ztd_char8_t* __from,
     const ztd_char8_t* __to, cnc_conversion_function* __single_conversion_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_add_to_registry(__registry, __from, __to, nullptr, __single_conversion_function,
	     __open_function, __close_function);
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_error
cnc_add_to_registry_n_single(cnc_conversion_registry* __registry, size_t __from_size,
     const ztd_char8_t* __from, size_t __to_size, const ztd_char8_t* __to,
     cnc_conversion_function* __single_conversion_function, cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_add_to_registry_n(__registry, __from_size, __from, __to_size, __to, nullptr,
	     __single_conversion_function, __open_function, __close_function);
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_error
cnc_add_to_registry_multi(cnc_conversion_registry* __registry, const ztd_char8_t* __from,
     const ztd_char8_t* __to, cnc_conversion_function* __multi_conversion_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_add_to_registry(__registry, __from, __to, __multi_conversion_function, nullptr,
	     __open_function, __close_function);
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_error
cnc_add_to_registry_n_multi(cnc_conversion_registry* __registry, size_t __from_size,
     const ztd_char8_t* __from, size_t __to_size, const ztd_char8_t* __to,
     cnc_conversion_function* __multi_conversion_function, cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_add_to_registry_n(__registry, __from_size, __from, __to_size, __to,
	     __multi_conversion_function, nullptr, __open_function, __close_function);
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_close_registry(
     cnc_conversion_registry* __registry) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__registry == nullptr)
		return;
	__registry->~cnc_conversion_registry();
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_delete_registry(
     cnc_conversion_registry* __registry) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__registry == nullptr)
		return;
	cnc_conversion_heap __heap = __registry->__heap;
	cnc_close_registry(__registry);
	__heap.deallocate(static_cast<unsigned char*>(static_cast<void*>(__registry)),
	     sizeof(cnc_conversion_registry), alignof(cnc_conversion_registry), __heap.user_data);
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_pairs_list(
     const cnc_conversion_registry* __registry,
     cnc_conversion_registry_pair_function __callback_function,
     void* __user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__registry == nullptr || __callback_function == nullptr)
		return;
	for (const auto& __key_value : __registry->__paths) {
		const __cnc_registry_entry_key& __key = __key_value.first;
		__callback_function(__key.__from.size(), __key.__from.data(), __key.__to.size(),
		     __key.__to.data(), __user_data);
	}
}
