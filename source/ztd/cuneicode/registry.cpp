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

#include <ztd/cuneicode/detail/registry.hpp>

#include <ztd/cuneicode/registry.h>
#include <ztd/cuneicode/detail/conv_id.hpp>
#include <ztd/cuneicode/detail/align.hpp>
#include <ztd/cuneicode/detail/conversion.hpp>

#include <ztd/idk/empty_string.hpp>
#include <ztd/idk/assert.hpp>

#include <limits>

namespace {
	static inline void __cnc_default_close_function(void* __conversion) ZTD_NOEXCEPT_IF_CXX_I_ {
		// nothing to do with the extra space
		(void)__conversion;
	}

	static inline cnc_open_err __cnc_default_open_function(cnc_conversion_registry* __registry,
	     cnc_conversion* __conversion, size_t* __p_available_space, size_t* __p_max_alignment,
	     void** __p_space) ZTD_NOEXCEPT_IF_CXX_I_ {
		// nothing to do with the extra space
		(void)__registry;
		(void)__conversion;
		(void)__p_available_space;
		(void)__p_max_alignment;
		(void)__p_space;
		return cnc_open_err_ok;
	}

	static inline bool __cnc_default_state_is_complete_function(
	     const cnc_conversion* __conversion, const void* __p_state) ZTD_NOEXCEPT_IF_CXX_I_ {
		(void)__conversion;
		(void)__p_state;
		return true;
	}
} // namespace

__cnc_u8string_view __cnc_resolve_alias(
     cnc_conversion_registry* __registry, __cnc_u8string_view __name) {
	const auto __alias_last = __registry->__aliases.cend();
	for (auto __alias_it = __registry->__aliases.find(__name); __alias_it != __alias_last;
	     __alias_it      = __registry->__aliases.find(__name)) {
		__name = __alias_it->second;
	}
	return __name;
}

cnc_open_err __cnc_find_entry(cnc_conversion_registry* __registry, __cnc_u8string_view __from_view,
     __cnc_u8string_view __to_view, cnc_indirect_selection_c8_function* __selection,
     const __cnc_registry_entry_value** __p_from_entry,
     const __cnc_registry_entry_value** __p_to_entry,
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
		return cnc_open_err_ok;
	}

	// no direct path: search indirect paths
	{
		// First: Unicode-based indirect paths for UTF-8/16/32, and their checked/unchecked
		// variants
		constexpr const size_t __from_index = 0;
		constexpr const size_t __to_index   = 1;
		const ::cnc::__cnc_detail::__cnc_defined_slot __valid_indirect_paths[][2]
		     = { { ::cnc::__cnc_detail::__cnc_defined_slot::__utf8,
			         ::cnc::__cnc_detail::__cnc_defined_slot::__utf8 },
			       { ::cnc::__cnc_detail::__cnc_defined_slot::__utf8,
			            ::cnc::__cnc_detail::__cnc_defined_slot::__utf8_unchecked },
			       { ::cnc::__cnc_detail::__cnc_defined_slot::__utf8_unchecked,
			            ::cnc::__cnc_detail::__cnc_defined_slot::__utf8_unchecked },
			       { ::cnc::__cnc_detail::__cnc_defined_slot::__utf16,
			            ::cnc::__cnc_detail::__cnc_defined_slot::__utf16 },
			       { ::cnc::__cnc_detail::__cnc_defined_slot::__utf16,
			            ::cnc::__cnc_detail::__cnc_defined_slot::__utf16_unchecked },
			       { ::cnc::__cnc_detail::__cnc_defined_slot::__utf16_unchecked,
			            ::cnc::__cnc_detail::__cnc_defined_slot::__utf16_unchecked },
			       { ::cnc::__cnc_detail::__cnc_defined_slot::__utf32,
			            ::cnc::__cnc_detail::__cnc_defined_slot::__utf32 },
			       { ::cnc::__cnc_detail::__cnc_defined_slot::__utf32,
			            ::cnc::__cnc_detail::__cnc_defined_slot::__utf32_unchecked },
			       { ::cnc::__cnc_detail::__cnc_defined_slot::__utf32_unchecked,
			            ::cnc::__cnc_detail::__cnc_defined_slot::__utf32_unchecked } };
		constexpr const size_t __valid_indirect_paths_count
		     = ztdc_c_array_size(__valid_indirect_paths);
		const ::std::pair<const __cnc_registry_entry_key, __cnc_registry_entry_value>*
		     __path_entries[__valid_indirect_paths_count][2]
		     = { { nullptr, nullptr }, { nullptr, nullptr }, { nullptr, nullptr },
			       { nullptr, nullptr }, { nullptr, nullptr }, { nullptr, nullptr },
			       { nullptr, nullptr }, { nullptr, nullptr }, { nullptr, nullptr } };
		for (const auto& __entry : __registry->__paths) {
			const __cnc_registry_entry_key& __key = __entry.first;
			if (!::ztd::is_encoding_name_equal_for(__key.__from, __from_view))
				continue;
			::cnc::__cnc_detail::__cnc_defined_slot __slot;
			if (!::cnc::__cnc_detail::__is_unicode_identifier(__key.__to, __slot))
				continue;
			__path_entries[static_cast<size_t>(__slot)][__from_index] = &__entry;
		}
		for (size_t __slot_index = 0; __slot_index < __valid_indirect_paths_count;
		     ++__slot_index) {
			auto& __entry_pair_array = __path_entries[__slot_index];
			const auto* __from_entry = __entry_pair_array[__from_index];
			if (__from_entry == nullptr)
				continue;
			const ::cnc::__cnc_detail::__cnc_defined_slot __slot
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
			if (__from_entry == nullptr || __to_entry == nullptr) {
				continue;
			}
			if (!__selection(__from_entry->first.__from.size(),
			         __from_entry->first.__from.data(), __to_entry->first.__to.size(),
			         __to_entry->first.__to.data(), __to_entry->first.__from.size(),
			         __to_entry->first.__from.data())) {
				continue;
			}
			__p_info->is_indirect        = true;
			__p_info->from_code_data     = __from_entry->first.__from.data();
			__p_info->from_code_size     = __from_entry->first.__from.size();
			__p_info->to_code_data       = __to_entry->first.__to.data();
			__p_info->to_code_size       = __to_entry->first.__to.size();
			__p_info->indirect_code_data = __to_entry->first.__from.data();
			__p_info->indirect_code_size = __to_entry->first.__from.size();
			*__p_from_entry              = &__from_entry->second;
			*__p_to_entry                = &__to_entry->second;
			return cnc_open_err_ok;
		}
	}
	// If the above failed, then we need to do a full-scan search over all entries...
	{
		for (const auto& __from_entry : __registry->__paths) {
			// find the from -> indirect path, if present
			const __cnc_registry_entry_key& __from_key = __from_entry.first;
			if (!::ztd::is_encoding_name_equal_for(__from_key.__from, __from_view))
				continue;
			__cnc_registry_entry_key __indirect_from_to_lookup = { __from_key.__to, __to_view };
			auto __indirect_it = __registry->__paths.find(__indirect_from_to_lookup);
			if (__indirect_it == __registry->__paths.cend()) {
				continue;
			}
			const __cnc_registry_entry_key& __indirect_key = __indirect_it->first;
			if (!__selection(__from_key.__from.size(), __from_key.__from.data(),
			         __indirect_key.__to.size(), __indirect_key.__to.data(),
			         __indirect_key.__from.size(), __indirect_key.__from.data())) {
				continue;
			}
			// we have a full indirect path now: go with it
			__p_info->is_indirect        = true;
			__p_info->from_code_data     = __from_key.__from.data();
			__p_info->from_code_size     = __from_key.__from.size();
			__p_info->to_code_data       = __indirect_key.__to.data();
			__p_info->to_code_size       = __indirect_key.__to.size();
			__p_info->indirect_code_data = __indirect_key.__from.data();
			__p_info->indirect_code_size = __indirect_key.__from.size();
			*__p_from_entry              = &__from_entry.second;
			*__p_to_entry                = &__indirect_it->second;
			return cnc_open_err_ok;
		}
	}
	return cnc_open_err_no_conversion_path;
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_new(
     cnc_conversion_registry** __out_p_registry,
     cnc_registry_options __registry_options) ZTD_NOEXCEPT_IF_CXX_I_ {
	try {
		cnc_conversion_registry* __registry = nullptr;
		cnc_conversion_heap __heap          = cnc_create_default_heap();
		cnc_open_err err = ::cnc_registry_open(&__registry, &__heap, __registry_options);
		if (err != cnc_open_err_ok) {
			return err;
		}
		*__out_p_registry = __registry;
		return cnc_open_err_ok;
	}
	catch (...) {
		*__out_p_registry = nullptr;
	}
	return cnc_open_err_allocation_failure;
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_open(
     cnc_conversion_registry** __out_p_registry, cnc_conversion_heap* __p_original_heap,
     cnc_registry_options __registry_options) ZTD_NOEXCEPT_IF_CXX_I_ {
	size_t __available_space           = (::std::numeric_limits<size_t>::max)();
	size_t __necessary_space           = 0;
	size_t __used_space                = 0;
	void* __target                     = nullptr;
	cnc_conversion_heap __default_heap = cnc_create_default_heap();
	cnc_conversion_heap* __p_heap      = __p_original_heap;
	if (__p_heap == nullptr) {
		__p_heap = &__default_heap;
	}
	::cnc::__cnc_detail::__align(alignof(cnc_conversion_registry), sizeof(cnc_conversion_registry),
	     __target, __available_space, __necessary_space);
	__target = static_cast<void*>(cnc_heap_allocate(
	     __p_heap, __necessary_space, alignof(cnc_conversion_registry), &__used_space));
	if (__target == nullptr) {
		return cnc_open_err_allocation_failure;
	}
	void* __aligned_target = ::cnc::__cnc_detail::__align(alignof(cnc_conversion_registry),
	     sizeof(cnc_conversion_registry), __target, __available_space);
	if (__aligned_target == nullptr) {
		return cnc_open_err_allocation_failure;
	}
	cnc_conversion_registry* __aligned_registry
	     = new (__aligned_target) cnc_conversion_registry(*__p_heap);
	*__out_p_registry = __aligned_registry;
	if ((__registry_options & cnc_registry_options_empty) != cnc_registry_options_empty) {
		// if it does not need to be empty, pre-fill with system entities
		cnc_open_err __err = ::__cnc_add_default_registry_entries(*__out_p_registry);
		if (__err != cnc_open_err_ok) {
			return __err;
		}
	}
	return cnc_open_err_ok;
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_c8n(
     cnc_conversion_registry* __registry, size_t __from_size,
     const ztd_char8_t __from[ZTD_PTR_EXTENT(__from_size)], size_t __to_size,
     const ztd_char8_t __to[ZTD_PTR_EXTENT(__to_size)],
     cnc_conversion_function* __multi_conversion_function,
     cnc_conversion_function* __single_conversion_function,
     cnc_state_is_complete_function* __state_is_complete_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__from_size == 0 || __from == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	if (__to_size == 0 || __to == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	if (__multi_conversion_function == nullptr && __single_conversion_function == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	if (__multi_conversion_function == nullptr) {
		__multi_conversion_function = &__cnc_multi_from_single_conversion;
	}
	else if (__single_conversion_function == nullptr) {
		__single_conversion_function = &__cnc_single_from_multi_conversion;
	}
	if (__state_is_complete_function == nullptr) {
		__state_is_complete_function = &__cnc_default_state_is_complete_function;
	}

	// must specify both, or neither
	bool __open_is_null  = __open_function == nullptr;
	bool __close_is_null = __close_function == nullptr;
	if (__open_is_null != __close_is_null) {
		return cnc_open_err_invalid_parameter;
	}
	if (__open_is_null) {
		__open_function = &::__cnc_default_open_function;
	}
	if (__close_is_null) {
		__close_function = &::__cnc_default_close_function;
	}

	const ::std::size_t __before_path_keys_size = __registry->__path_keys.size();
	try {
		__path_key& __from_key = *std::prev(__registry->__path_keys.end(), 2);
		__path_key& __to_key   = *std::prev(__registry->__path_keys.end(), 1);
		__from_key.assign(__from, __from + __from_size);
		__to_key.assign(__to, __to + __to_size);
		__from_key.push_back(static_cast<ztd_char8_t>(0));
		__to_key.push_back(static_cast<ztd_char8_t>(0));
		__cnc_registry_entry_key __registry_entry_key { __cnc_u8string_view(__from_key.data(),
			                                                __from_key.size() - 1),
			__cnc_u8string_view(__to_key.data(), __to_key.size() - 1) };
		__cnc_registry_entry_value __registry_entry { __multi_conversion_function,
			__single_conversion_function, __state_is_complete_function, __open_function,
			__close_function };
		auto __path_it = __registry->__paths.find(__registry_entry_key);
		if (__path_it != __registry->__paths.cend()) {
			// update existing conversion
			__path_it->second = __registry_entry;
		}
		else {
			__registry->__path_keys.emplace_back(
			     __cnc_detail_inline_size_guess, __path_key_allocator(__registry->__heap));
			__registry->__path_keys.emplace_back(
			     __cnc_detail_inline_size_guess, __path_key_allocator(__registry->__heap));
			__registry->__paths.emplace_hint(::std::move(__path_it),
			     ::std::move(__registry_entry_key), ::std::move(__registry_entry));
		}
	}
	catch (...) {
		while (__registry->__path_keys.size() != __before_path_keys_size) {
			__registry->__path_keys.pop_back();
		}
		return cnc_open_err_allocation_failure;
	}

	return cnc_open_err_ok;
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_c8(
     cnc_conversion_registry* __registry, const ztd_char8_t* __from, const ztd_char8_t* __to,
     cnc_conversion_function* __multi_conversion_function,
     cnc_conversion_function* __single_conversion_function,
     cnc_state_is_complete_function* __state_is_complete_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	size_t __from_size = ztdc_c_string_ptr_size_c8(__from);
	size_t __to_size   = ztdc_c_string_ptr_size_c8(__to);
	return ::cnc_registry_add_c8n(__registry, __from_size, __from, __to_size, __to,
	     __multi_conversion_function, __single_conversion_function, __state_is_complete_function,
	     __open_function, __close_function);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add(
     cnc_conversion_registry* __registry, const char* __from, const char* __to,
     cnc_conversion_function* __multi_conversion_function,
     cnc_conversion_function* __single_conversion_function,
     cnc_state_is_complete_function* __state_is_complete_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	size_t __from_size = ztdc_c_string_ptr_size_c(__from);
	size_t __to_size   = ztdc_c_string_ptr_size_c(__to);
	return ::cnc_registry_add_c8n(__registry, __from_size,
	     reinterpret_cast<const ztd_char8_t*>(__from), __to_size,
	     reinterpret_cast<const ztd_char8_t*>(__to), __multi_conversion_function,
	     __single_conversion_function, __state_is_complete_function, __open_function,
	     __close_function);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_single_c8(
     cnc_conversion_registry* __registry, const ztd_char8_t* __from, const ztd_char8_t* __to,
     cnc_conversion_function* __single_conversion_function,
     cnc_state_is_complete_function* __state_is_complete_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_registry_add_c8(__registry, __from, __to, nullptr, __single_conversion_function,
	     __state_is_complete_function, __open_function, __close_function);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_single_c8n(
     cnc_conversion_registry* __registry, size_t __from_size,
     const ztd_char8_t __from[ZTD_PTR_EXTENT(__from_size)], size_t __to_size,
     const ztd_char8_t __to[ZTD_PTR_EXTENT(__to_size)],
     cnc_conversion_function* __single_conversion_function,
     cnc_state_is_complete_function* __state_is_complete_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_registry_add_c8n(__registry, __from_size, __from, __to_size, __to, nullptr,
	     __single_conversion_function, __state_is_complete_function, __open_function,
	     __close_function);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_multi_c8(
     cnc_conversion_registry* __registry, const ztd_char8_t* __from, const ztd_char8_t* __to,
     cnc_conversion_function* __multi_conversion_function,
     cnc_state_is_complete_function* __state_is_complete_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_registry_add_c8(__registry, __from, __to, __multi_conversion_function, nullptr,
	     __state_is_complete_function, __open_function, __close_function);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_multi_c8n(
     cnc_conversion_registry* __registry, size_t __from_size,
     const ztd_char8_t __from[ZTD_PTR_EXTENT(__from_size)], size_t __to_size,
     const ztd_char8_t __to[ZTD_PTR_EXTENT(__to_size)],
     cnc_conversion_function* __multi_conversion_function,
     cnc_state_is_complete_function* __state_is_complete_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_registry_add_c8n(__registry, __from_size, __from, __to_size, __to,
	     __multi_conversion_function, nullptr, __state_is_complete_function, __open_function,
	     __close_function);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_single(
     cnc_conversion_registry* __registry, const char* __from, const char* __to,
     cnc_conversion_function* __single_conversion_function,
     cnc_state_is_complete_function* __state_is_complete_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_registry_add_single_c8(__registry, reinterpret_cast<const ztd_char8_t*>(__from),
	     reinterpret_cast<const ztd_char8_t*>(__to), __single_conversion_function,
	     __state_is_complete_function, __open_function, __close_function);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_single_n(
     cnc_conversion_registry* __registry, size_t __from_size,
     const char __from[ZTD_PTR_EXTENT(__from_size)], size_t __to_size,
     const char __to[ZTD_PTR_EXTENT(__to_size)],
     cnc_conversion_function* __single_conversion_function,
     cnc_state_is_complete_function* __state_is_complete_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_registry_add_single_c8n(__registry, __from_size,
	     reinterpret_cast<const ztd_char8_t*>(__from), __to_size,
	     reinterpret_cast<const ztd_char8_t*>(__to), __single_conversion_function,
	     __state_is_complete_function, __open_function, __close_function);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_multi(
     cnc_conversion_registry* __registry, const char* __from, const char* __to,
     cnc_conversion_function* __multi_conversion_function,
     cnc_state_is_complete_function* __state_is_complete_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_registry_add_multi_c8(__registry, reinterpret_cast<const ztd_char8_t*>(__from),
	     reinterpret_cast<const ztd_char8_t*>(__to), __multi_conversion_function,
	     __state_is_complete_function, __open_function, __close_function);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_multi_n(
     cnc_conversion_registry* __registry, size_t __from_size,
     const char __from[ZTD_PTR_EXTENT(__from_size)], size_t __to_size,
     const char __to[ZTD_PTR_EXTENT(__to_size)],
     cnc_conversion_function* __multi_conversion_function,
     cnc_state_is_complete_function* __state_is_complete_function,
     cnc_open_function* __open_function,
     cnc_close_function* __close_function) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_registry_add_multi_c8n(__registry, __from_size,
	     reinterpret_cast<const ztd_char8_t*>(__from), __to_size,
	     reinterpret_cast<const ztd_char8_t*>(__to), __multi_conversion_function,
	     __state_is_complete_function, __open_function, __close_function);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_alias_c8(
     cnc_conversion_registry* __registry, const ztd_char8_t* __alias,
     const ztd_char8_t* __original) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_registry_add_alias_c8n(__registry, ztdc_c_string_ptr_size_c8(__alias), __alias,
	     ztdc_c_string_ptr_size_c8(__original), __original);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_alias(
     cnc_conversion_registry* __registry, const char* __alias,
     const char* __original) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_registry_add_alias_n(__registry, ztdc_c_string_ptr_size(__alias), __alias,
	     ztdc_c_string_ptr_size(__original), __original);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_alias_c8n(
     cnc_conversion_registry* __registry, size_t __alias_size,
     const ztd_char8_t __alias[ZTD_PTR_EXTENT(__alias_size)], size_t __original_size,
     const ztd_char8_t __original[ZTD_PTR_EXTENT(__original_size)]) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__alias_size == 0 || __alias == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	if (__alias_size == 0 || __alias == nullptr) {
		return cnc_open_err_invalid_parameter;
	}
	const ::std::size_t __before_path_keys_size = __registry->__path_keys.size();
	// first, verify that the original exists before we do anything
	try {
		__path_key& __alias_key    = *std::prev(__registry->__path_keys.end(), 2);
		__path_key& __original_key = *std::prev(__registry->__path_keys.end(), 1);
		__alias_key.assign(__alias, __alias + __alias_size);
		__original_key.assign(__original, __original + __original_size);
		__alias_key.push_back(static_cast<ztd_char8_t>(0));
		__original_key.push_back(static_cast<ztd_char8_t>(0));
		__cnc_u8string_view __alias_view(__alias_key.data(), __alias_key.size() - 1);
		__cnc_u8string_view __original_view(__original_key.data(), __original_key.size() - 1);
		auto __path_it = __registry->__aliases.find(__alias_view);
		if (__path_it != __registry->__aliases.cend()) {
			// update existing conversion
			__path_it->second = __alias_view;
		}
		else {
			__registry->__path_keys.emplace_back(
			     __cnc_detail_inline_size_guess, __path_key_allocator(__registry->__heap));
			__registry->__path_keys.emplace_back(
			     __cnc_detail_inline_size_guess, __path_key_allocator(__registry->__heap));
			__registry->__aliases.emplace_hint(::std::move(__path_it), ::std::move(__alias_view),
			     ::std::move(__original_view));
		}
	}
	catch (...) {
		while (__registry->__path_keys.size() != __before_path_keys_size) {
			__registry->__path_keys.pop_back();
		}
		return cnc_open_err_allocation_failure;
	}
	return cnc_open_err_ok;
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ cnc_open_err cnc_registry_add_alias_n(
     cnc_conversion_registry* __registry, size_t __alias_size,
     const char __alias[ZTD_PTR_EXTENT(__alias_size)], size_t __original_size,
     const char __original[ZTD_PTR_EXTENT(__original_size)]) ZTD_NOEXCEPT_IF_CXX_I_ {
	return cnc_registry_add_alias_c8n(__registry, __alias_size,
	     reinterpret_cast<const ztd_char8_t*>(__alias), __original_size,
	     reinterpret_cast<const ztd_char8_t*>(__original));
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_registry_close(
     cnc_conversion_registry* __registry) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__registry == nullptr)
		return;
	__registry->~cnc_conversion_registry();
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_registry_delete(
     cnc_conversion_registry* __registry) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__registry == nullptr)
		return;
	const cnc_conversion_heap& __heap = __registry->__heap;
	cnc_registry_close(__registry);
	__heap.deallocate(static_cast<unsigned char*>(static_cast<void*>(__registry)),
	     sizeof(cnc_conversion_registry), alignof(cnc_conversion_registry), __heap.user_data);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ const cnc_conversion_heap* cnc_registry_heap(
     const cnc_conversion_registry* __registry) ZTD_NOEXCEPT_IF_CXX_I_ {
	return ::std::addressof(__registry->__heap);
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_registry_pairs_list_c8n(
     const cnc_conversion_registry* __registry,
     cnc_conversion_registry_pair_c8_function __callback_function,
     void* __user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__registry == nullptr || __callback_function == nullptr)
		return;
	for (const auto& __key_value : __registry->__paths) {
		const __cnc_registry_entry_key& __key = __key_value.first;
		const auto& __from                    = __key.__from;
		const auto& __to                      = __key.__to;
		__callback_function(
		     false, __from.size(), __from.data(), __to.size(), __to.data(), __user_data);
	}
	for (const auto& __key_value : __registry->__aliases) {
		const auto& __from = __key_value.first;
		const auto& __to   = __key_value.second;
		__callback_function(
		     true, __from.size(), __from.data(), __to.size(), __to.data(), __user_data);
	}
}

ZTD_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_registry_pairs_list_n(
     const cnc_conversion_registry* __registry,
     cnc_conversion_registry_pair_function __callback_function,
     void* __user_data) ZTD_NOEXCEPT_IF_CXX_I_ {
	if (__registry == nullptr || __callback_function == nullptr)
		return;
	for (const auto& __key_value : __registry->__paths) {
		const __cnc_registry_entry_key& __key = __key_value.first;
		const auto& __from                    = __key.__from;
		const auto& __to                      = __key.__to;
		__callback_function(false, __from.size(), reinterpret_cast<const char*>(__from.data()),
		     __to.size(), reinterpret_cast<const char*>(__to.data()), __user_data);
	}
	for (const auto& __key_value : __registry->__aliases) {
		const auto& __from = __key_value.first;
		const auto& __to   = __key_value.second;
		__callback_function(true, __from.size(), reinterpret_cast<const char*>(__from.data()),
		     __to.size(), reinterpret_cast<const char*>(__to.data()), __user_data);
	}
}
