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

#include <ztd/cuneicode/heap.h>

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ bool cnc_conversion_heap_equals(
     const cnc_conversion_heap* __left, const cnc_conversion_heap* __right) ZTD_NOEXCEPT_IF_CXX_I_ {
	return __left->allocate == __right->allocate && __left->reallocate == __right->reallocate
	     && __left->shrink == __right->shrink && __left->expand == __right->expand
	     && __left->deallocate == __right->deallocate && __left->user_data == __right->user_data;
}

ZTD_CUNEICODE_C_LANGUAGE_LINKAGE_I_ ZTD_CUNEICODE_API_LINKAGE_I_ bool
cnc_conversion_heap_not_equals(
     const cnc_conversion_heap* __left, const cnc_conversion_heap* __right) ZTD_NOEXCEPT_IF_CXX_I_ {
	return __left->allocate != __right->allocate || __left->reallocate != __right->reallocate
	     || __left->shrink != __right->shrink || __left->expand != __right->expand
	     || __left->deallocate != __right->deallocate || __left->user_data != __right->user_data;
}
