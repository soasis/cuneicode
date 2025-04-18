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

#include <ztd/cuneicode/heap.h>

#include <cstdlib>
#include <cstring>

char __heap_magic_value = 0;

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
void* __cnc_default_allocate(size_t __requested_size, size_t __alignment, size_t* __p_actual_size,
     void* __user_data) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	(void)__user_data;
	(void)__alignment;
	if (__requested_size == 0) {
		// return magic value;
		*__p_actual_size = 0;
		return static_cast<void*>(&__heap_magic_value);
	}
	void* __ptr = malloc(__requested_size);
	if (__ptr == nullptr) {
		*__p_actual_size = 0;
		return nullptr;
	}
	*__p_actual_size = __requested_size;
	return __ptr;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
void __cnc_default_deallocate(void* __ptr, size_t __ptr_size, size_t __alignment, void* __user_data)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	(void)__ptr_size;
	(void)__user_data;
	(void)__alignment;
	if (__ptr == static_cast<void*>(&__heap_magic_value)) {
		return;
	}
	free(__ptr);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
void* __cnc_default_reallocate(void* __original, size_t __original_size, size_t __requested_size,
     size_t __alignment, size_t* __p_actual_size, void* __user_data) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	(void)__user_data;
	(void)__alignment;
	if (__requested_size == 0) {
		// In C23, it's undefined behavior.
		// before that, it's some messed up nonsense.
		// We protect against it here: deallocate and return what our default allocate does for
		// size 0.
		__cnc_default_deallocate(__original, __original_size, __alignment, __user_data);
		return __cnc_default_allocate(0, __alignment, __p_actual_size, __user_data);
	}
	void* __ptr = realloc(__original, __requested_size);
	if (__ptr == nullptr) {
		*__p_actual_size = 0;
		return nullptr;
	}
	*__p_actual_size = __requested_size;
	return __ptr;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
void* __cnc_default_expand_allocation(void* __original, size_t __original_size, size_t __alignment,
     size_t __expand_left, size_t __expand_right, size_t* __p_actual_size, void* __user_data)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	(void)__original;
	(void)__original_size;
	(void)__alignment;
	(void)__expand_left;
	(void)__expand_right;
	(void)__user_data;
	*__p_actual_size = 0;
	return nullptr;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
void* __cnc_default_shrink_allocation(void* __original, size_t __original_size, size_t __alignment,
     size_t __reduce_left, size_t __reduce_right, size_t* __p_actual_size, void* __user_data)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	(void)__original;
	(void)__original_size;
	(void)__alignment;
	(void)__reduce_left;
	(void)__reduce_right;
	(void)__user_data;
	*__p_actual_size = 0;
	return nullptr;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
void* cnc_heap_allocate(cnc_conversion_heap* __heap, size_t __requested_size, size_t __alignment,
     size_t* __p_actual_size) {
	return __heap->allocate(__requested_size, __alignment, __p_actual_size, __heap->user_data);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
void* cnc_heap_reallocate_allocation(cnc_conversion_heap* __heap, void* __original,
     size_t __original_size, size_t __requested_size, size_t __alignment, size_t* __p_actual_size) {
	return __heap->reallocate(__original, __original_size, __requested_size, __alignment,
	     __p_actual_size, __heap->user_data);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
void* cnc_heap_expand_allocation(cnc_conversion_heap* __heap, void* __original,
     size_t __original_size, size_t __alignment, size_t __expand_left, size_t __expand_right,
     size_t* __p_actual_size) {
	return __heap->expand(__original, __original_size, __alignment, __expand_left, __expand_right,
	     __p_actual_size, __heap->user_data);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
void* cnc_heap_shrink_allocation(cnc_conversion_heap* __heap, void* __original,
     size_t __original_size, size_t __alignment, size_t __reduce_left, size_t __reduce_right,
     size_t* __p_actual_size) {
	return __heap->shrink(__original, __original_size, __alignment, __reduce_left, __reduce_right,
	     __p_actual_size, __heap->user_data);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
void cnc_heap_deallocate(cnc_conversion_heap* __heap, void* __ptr, size_t __ptr_size,
     size_t __alignment) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	__heap->deallocate(__ptr, __ptr_size, __alignment, __heap->user_data);
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
cnc_conversion_heap cnc_create_default_heap(void) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	cnc_conversion_heap __heap {
		nullptr,
		__cnc_default_allocate,
		__cnc_default_deallocate,
		__cnc_default_reallocate,
		__cnc_default_expand_allocation,
		__cnc_default_shrink_allocation,
	};
	return __heap;
}


ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
bool cnc_conversion_heap_equals(const cnc_conversion_heap* __left,
     const cnc_conversion_heap* __right) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return __left->allocate == __right->allocate && __left->reallocate == __right->reallocate
	     && __left->shrink == __right->shrink && __left->expand == __right->expand
	     && __left->deallocate == __right->deallocate && __left->user_data == __right->user_data;
}

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_USE(ZTD_CUNEICODE_API_LINKAGE)
bool cnc_conversion_heap_not_equals(const cnc_conversion_heap* __left,
     const cnc_conversion_heap* __right) ZTD_USE(ZTD_NOEXCEPT_IF_CXX) {
	return __left->allocate != __right->allocate || __left->reallocate != __right->reallocate
	     || __left->shrink != __right->shrink || __left->expand != __right->expand
	     || __left->deallocate != __right->deallocate || __left->user_data != __right->user_data;
}
