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

#ifndef ZTD_CUNEICODE_HEAP_H
#define ZTD_CUNEICODE_HEAP_H

#pragma once

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/mcerr.h>
#include <ztd/idk/charN_t.h>

#if ZTD_IS_ON(ZTD_CXX)
#include <cstddef>
#else
#include <stddef.h>
#include <stdbool.h>
#endif

ZTD_USE(ZTD_C_LANGUAGE_LINKAGE) char __heap_magic_value;

//////
/// @addtogroup ztd_cuneicode_heap Heap Functions
///
/// @{

//////
/// @brief A heap allocate function type. For a description of the parameters, see
/// @ref cnc_heap_allocate.
typedef void*(cnc_heap_allocate_function)(size_t __requested_size, size_t __alignment,
     size_t* __p_actual_size, void* __user_data);

//////
/// @brief A heap deallocate function type. For a description of the parameters, see
/// @ref cnc_heap_deallocate.
typedef void(cnc_heap_deallocate_function)(
     void* __ptr, size_t __ptr_size, size_t __alignment, void* __user_data);

//////
/// @brief A heap reallocate function type. For a description of the parameters, see
/// @ref cnc_heap_allocate.
typedef void*(cnc_heap_reallocate_allocation_function)(void* __original, size_t __original_size,
     size_t __requested_size, size_t __alignment, size_t* __p_actual_size, void* __user_data);

//////
/// @brief A heap allocation expand function type. For a description of the parameters, see
/// @ref cnc_heap_expand_allocation.
typedef void*(cnc_heap_expand_allocation_function)(void* __original, size_t __original_size,
     size_t __alignment, size_t __expand_left, size_t __expand_right, size_t* __p_actual_size,
     void* __user_data);

//////
/// @brief A heap allocation shrink function type. For a description of the parameters, see
/// @ref cnc_heap_shrink_allocation.
typedef void*(cnc_heap_shrink_allocation_function)(void* __original, size_t __original_size,
     size_t __alignment, size_t __reduce_left, size_t __reduce_right, size_t* __p_actual_size,
     void* __user_data);

//////
/// @brief A heap allocate function type. For a description of the parameters, see
/// @ref cnc_heap_allocate.
typedef void* (*cnc_heap_allocate_function_ptr)(
     size_t __requested_size, size_t __alignment, size_t* __p_actual_size, void* __user_data);

//////
/// @brief A heap deallocate function type. For a description of the parameters, see
/// @ref cnc_heap_deallocate.
typedef void (*cnc_heap_deallocate_function_ptr)(void* __ptr, size_t __ptr_size, size_t __alignment,
     void* __user_data) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

//////
/// @brief A heap reallocate function type. For a description of the parameters, see
/// @ref cnc_heap_allocate.
typedef void* (*cnc_heap_reallocate_allocation_function_ptr)(void* __original,
     size_t __original_size, size_t __requested_size, size_t __alignment, size_t* __p_actual_size,
     void* __user_data);

//////
/// @brief A heap allocation expand function type. For a description of the parameters, see
/// @ref cnc_heap_expand_allocation.
typedef void* (*cnc_heap_expand_allocation_function_ptr)(void* __original, size_t __original_size,
     size_t __alignment, size_t __expand_left, size_t __expand_right, size_t* __p_actual_size,
     void* __user_data);

//////
/// @brief A heap allocation shrink function type. For a description of the parameters, see
/// @ref cnc_heap_shrink_allocation.
typedef void* (*cnc_heap_shrink_allocation_function_ptr)(void* __original, size_t __original_size,
     size_t __alignment, size_t __reduce_left, size_t __reduce_right, size_t* __p_actual_size,
     void* __user_data);

//////
/// @brief The conversion heap through which all allocating and deallocating happens, as well as any
/// related actions that require dynamic allocation.
typedef struct cnc_conversion_heap {
	//////
	/// @brief The userdata to be passed alng to the heap functions.
	void* user_data;
	//////
	/// @brief The allocation function. It takes a userdata passed in when creating the heap, and
	/// writes out the actual size of the returned allocated pointer. See @ref cnc_heap_allocate
	/// for more information.
	cnc_heap_allocate_function_ptr allocate;
	//////
	/// @brief The allocation deallocate function. It takes the original pointer, its size, its
	/// alignment, and a userdata passed in during heap creation. See @ref cnc_heap_deallocate for
	/// more information. See cnc_heap_deallocate for more information.
	cnc_heap_deallocate_function_ptr deallocate;
	//////
	/// @brief The reallocation function. It takes the original pointer and the requested size
	/// alongside a userdata passed in when creating the heap, and writes out the actual size of
	/// the returned allocated pointer. The original pointer cannot be used, if successful. See
	/// @ref cnc_heap_reallocate_allocation for more information.
	cnc_heap_reallocate_allocation_function_ptr reallocate;
	//////
	/// @brief The allocation expanding function. It takes the original allocation, the amount to
	/// expand the allocation by to its left (descending, lowest memory order) and right
	/// (ascending, highest memory order), the original pointer, and the original size alongside a
	/// userdata pointer. It returns the new pointer and writes out the new size, if successful.
	/// See @ref cnc_heap_expand_allocation for more information.
	cnc_heap_expand_allocation_function_ptr expand;
	//////
	/// @brief The allocation shrink function. It takes the original allocation, the amount to
	/// shrink the allocation by to its left (descending, lowest memory order) and right
	/// (ascending, highest memory order), the original pointer, and the original size alongside a
	/// userdata pointer. It returns the new pointer and writes out the new size, if successful.
	/// See @ref cnc_heap_shrink_allocation for more information.
	cnc_heap_shrink_allocation_function_ptr shrink;
} cnc_conversion_heap;

//////
/// @brief Attempts to allocate memory from the given `__heap`.  Calls the `__heap`'s `allocate`
/// function, using the `user_data` member from the given `__heap` for the last `void*` parameter.
///
/// @param[in, out] __heap The heap to allocate from.
/// @param[in] __requested_size The newly requested size for the given pointer.
/// @param[in] __alignment The alignment of the given pointer.
/// @param[in, out] __p_actual_size An output parameter for the size of the allocated pointer. If
/// this function cannot allocate, this will be set to 0. Otherwise, it will be the size of the
/// new memory region in bytes.
///
/// @returns A pointer to the newly created space of at least `__requested_size` bytes. It may be
/// larger than request, but it must not be smaller than requested. The real size is stored in
/// `*__p_actual_size`. If the allocation failed, or the allocator does not support 0-sized objects
/// (in the case of a `__requested_size` of 0), then the returned pointer is shall be `nullptr`.
///
/// @remarks `0` is a valid value for the `__requested_size`. If `__requested_size` is zero, then a
/// so-called "zero sized object" may be allocated that may also be "freed" (in whatever fashion,
/// including a potential no-op) using a matching call to `cnc_heap_deallocate`. Every allocation
/// must have a separate and distinct address, except for zero-sized objects and zero-sized
/// allocations. They may not have distinct addresses, even within the same address space (e.g.,
/// using a so-called sentinel object that marks zero-sized allocations).
///
/// If `0` is used for the `__requested_size` and `nullptr` is returned, then the implementation
/// does not support zero-sized objects and the user of this functinon shall assume that this is an
/// error case and respond identically as if a non-zero `__requested_size` is used and `nullptr` was
/// returned.
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ void* cnc_heap_allocate(cnc_conversion_heap* __heap,
     size_t __requested_size, size_t __alignment, size_t* __p_actual_size);


//////
/// @brief Attempts to reallocate an allocation from a given `__heap`, replacing it. Calls the
/// `__heap`'s `reallocate` function, using the `user_data` member from the given `__heap` for the
/// last `void*` parameter.
///
/// @param[in, out] __heap The heap from which `__original` was allocated from and manipulated with.
/// @param[in] __original The original pointer to deallocate.
/// @param[in] __original_size The size of the allocation for the original pointer.
/// @param[in] __requested_size The newly requested size for the given pointer.
/// @param[in] __alignment The alignment of the given pointer.
/// cases.
/// @param[in, out] __p_actual_size An output parameter for the size of the reallocated pointer. If
/// this function cannot reallocate, this will be set to 0. Otherwise, it will be the size of the
/// new memory region in bytes.
///
/// @returns A new pointer whose value may be moved and may not be identical to `__original`. If
/// this function returns `nullptr`, then the block pointer to be `__original` may still be used to
/// perform work and may be dereferenced, accessed, compared, and similar within the valid region of
/// [`__original`, `__original + __original_size`). Otherwise, the value may not be used in any way,
/// shape, or form and all work must be done with the returned pointer with its byte size indicated
/// by the value of `*__p_actual_size`.
///
/// @remarks If the return value is nullptr, that means that reallocation failed and the original
/// pointer was not deallocated.
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ void* cnc_heap_reallocate_allocation(cnc_conversion_heap* __heap,
     void* __original, size_t __original_size, size_t __requested_size, size_t __alignment,
     size_t* __p_actual_size);

//////
/// @brief Attempts to expand the given allocation from the provided `__heap`. Calls the `__heap`'s
/// `expand` function, using the `user_data` member from the given `__heap` for the last `void*`
/// parameter.
///
/// @param[in, out] __heap The heap from which `__original` was allocated from and manipulated with.
/// @param[in] __original The original pointer to expand.
/// @param[in] __original_size The size of the allocation for the original pointer, in bytes.
/// @param[in] __alignment The alignment for `__original`.
/// @param[in] __expand_left The amount to grow the "left" (start base `__original` value, extending
/// outwards by further decrementing the `__original` start pointer value) by, in bytes.
/// @param[in] __expand_right The amount to grow the "right" (end base `__original +
/// __original_size` value, extending outwards by further incrementing `__original +
/// __original_size` end pointer value) by, in bytes.
/// @param[in, out] __p_actual_size An output parameter for the size of the expanded pointer. If
/// this function cannot expand, this will be set to 0. Otherwise, it will be the size of the new
/// memory region in bytes.
///
/// @returns A new pointer of the shrank memory region. If this is `nullptr`, then no expansion has
/// occurred.
///
/// @remarks If the return is `nullptr`, then the memory region of the bytes spanning [`__original`,
/// `__original + __original_size`) may be accessed. If the return is not `nullptr`, then the memory
/// region of the bytes spanning [`__original`, `__original + __original_size`) may not be accessed
/// in any way, shape, or form, and its new size is stored in `*__p_actual_size` at an address
/// specified by the return value.
///
/// Commonly, `__expand_left` is set to zero while `__expand_right` contains the amount to expand
/// the allocation by. If `__expand_left` is not set to zero, then this function may move memory
/// from its original starting location to the new starting location VIA a careful overlapping copy.
/// Expansion is a request that allows an implementation to:
///
/// - expand to the exact specified region;
/// - or, expand to a larger region but that is at least as large as what is requested.
///
/// An implementation may expand any further than what was requested.
///
/// If `__expand_left` is zero and `__expand_right` is zero, then the function sets
/// `*__p_actual_size` to 0, returns `nullptr`, and takes no other heap-related action.
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ void* cnc_heap_expand_allocation(cnc_conversion_heap* __heap,
     void* __original, size_t __original_size, size_t __alignment, size_t __expand_left,
     size_t __expand_right, size_t* __p_actual_size);

//////
/// @brief Attempts to shrink the given allocation from the provided `__heap`. Calls the `__heap`'s
/// `shink` function, using the `user_data` member from the given `__heap` for the last `void*`
/// parameter.
///
/// @param[in, out] __heap The heap from which `__original` was allocated from and manipulated with.
/// @param[in] __original The original pointer to shrink.
/// @param[in] __original_size The size of the allocation for the original pointer, in bytes.
/// @param[in] __alignment The alignment for `__original`.
/// @param[in] __reduce_left The amount to reduce the "left" (start base `__original` value, pulling
/// inwards by further incrementing the `__original` start pointer value) by, in bytes.
/// @param[in] __reduce_right The amount to reduce the "right" (end base `__original +
/// __original_size` value, pulling inwards by further decrementing the `__original +
/// __original_size` end pointer value) by, in bytes.
/// @param[in, out] __p_actual_size An output parameter for the size of the shrunk pointer. If this
/// function cannot shrink, this will be set to 0. Otherwise, it will be the size of the new memory
/// region in bytes.
///
/// @returns A new pointer of the shrank memory region. If this is `nullptr`, then no shrinking has
/// occurred and `*__p_actual_size` is set to `0`.
///
/// @remarks If the return is `nullptr`, then the memory region of the bytes spanning [`__original`,
/// `__original + __original_size`) may be accessed. If the return is not `nullptr`, then the memory
/// region of the bytes spanning [`__original`, `__original + __original_size`) may not be accessed
/// in any way, shape, or form, and its new size is stored in `*__p_actual_size` at an address
/// specified by the return value.
///
/// Commonly, `__reduce_left` is set to zero while `__reduce_right` contains the amount to shrink
/// the allocation by. If `__reduce_left` is not set to zero, then this function may move memory
/// from its original starting location to the new starting location VIA a careful overlapping copy.
/// Shrinking is a request that allows an implementation to:
///
/// - shrink to the exact specified region;
/// - or, shrink to a smaller region that at least as large as what is requested.
///
/// An implementation must not shrink any further than what was requested and it is a violation of
/// this API's conditions if it does.
///
/// If `__reduce_left` is zero and `__reduce_right` is non-zero, is it not allowed for an
/// implementation to do nothing but then tell the implementation that the new size is smaller than
/// the original without affecting the allocation in some fashion. An implementation that does
/// nothing must return `nullptr` and set
/// `*__p_actual_size` to `0`.
///
/// If `__reduce_left` is zero and `__reduce_right` is zero, then the function sets
/// `*__p_actual_size` to 0, returns `nullptr`, and takes no other heap-related action.
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ void* cnc_heap_shrink_allocation(cnc_conversion_heap* __heap,
     void* __original, size_t __original_size, size_t __alignment, size_t __reduce_left,
     size_t __reduce_right, size_t* __p_actual_size);

//////
/// @brief Deallocates the given memory using the provided heap. Calls the `__heap`'s `deallocate`
/// function, using the `user_data` member from the given `__heap` for the last `void*` parameter.
///
/// @param[in, out] __heap The heap from which `__ptr` was allocated from and manipulated with.
/// @param[in] __ptr The original pointer to deallocate.
/// @param[in] __ptr_size The size of the allocation for the original pointer.
/// @param[in] __alignment The alignment of the given pointer. Not necessarily required in all cases
/// for all allocators, but must be provided by the user.
///
/// @remarks The allocation must have come from this heap. If it does, then it is an error and
/// the behavior after such an error is committed is undefined. The memory region of the bytes
/// denoted by [`__ptr`, `__ptr + __ptr_size`) can no longer be accessed after this function
/// returns; doing so is an error and the behavior after such an error is committed is undefined.
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ void cnc_heap_deallocate(cnc_conversion_heap* __heap, void* __ptr,
     size_t __ptr_size, size_t __alignment) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);


//////
/// @brief Creates a default heap to be used. Goes through the globally-available allocator (malloc,
/// free. etc. or similar known provided free stores).
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_conversion_heap cnc_create_default_heap(void)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

//////
/// @brief Compares whether two cnc_conversion_heaps are the same.
///
/// @param[in] __left The first heap.
/// @param[in] __right The second heap.
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ bool cnc_conversion_heap_equals(const cnc_conversion_heap* __left,
     const cnc_conversion_heap* __right) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

//////
/// @brief Compares whether two cnc_conversion_heaps are not the same.
///
/// @param[in] __left The first heap.
/// @param[in] __right The second heap.
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ bool cnc_conversion_heap_not_equals(const cnc_conversion_heap* __left,
     const cnc_conversion_heap* __right) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

//////
/// @}

#endif
