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

#ifndef ZTD_CUNEICODE_ISO_8859_15_H
#define ZTD_CUNEICODE_ISO_8859_15_H

#pragma once

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/mcerr.h>
#include <ztd/cuneicode/mcstate.h>
#include <ztd/idk/charN_t.h>

#if ZTD_IS_ON(ZTD_CXX)
#include <cstddef>
#else
#include <stddef.h>
#include <stdbool.h>
#endif

//////
/// @addtogroup ztd_cuneicode_conversion Conversion Functions
///
/// @{

//////
/// @see cnc_c32ntomcn
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32ntomcn_iso_8859_15(size_t* __p_maybe_dst_len,
     ztd_char_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

//////
/// @see cnc_c32nrtomcn
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32nrtomcn_iso_8859_15(size_t* __p_maybe_dst_len,
     ztd_char_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src,
     cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

//////
/// @see cnc_mcntoc32n
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcntoc32n_iso_8859_15(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char_t** __p_src)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

//////
/// @see cnc_mcnrtoc32n
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcnrtoc32n_iso_8859_15(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char_t** __p_src,
     cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);



//////
/// @see cnc_c32sntomcsn
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32sntomcsn_iso_8859_15(size_t* __p_maybe_dst_len,
     ztd_char_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

//////
/// @see cnc_c32snrtomcsn
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_c32snrtomcsn_iso_8859_15(size_t* __p_maybe_dst_len,
     ztd_char_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char32_t** __p_src,
     cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

//////
/// @see cnc_mcsntoc32sn
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcsntoc32sn_iso_8859_15(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char_t** __p_src)
     ZTD_USE(ZTD_NOEXCEPT_IF_CXX);

//////
/// @see cnc_mcsnrtoc32sn
ZTD_USE(ZTD_C_LANGUAGE_LINKAGE)
ZTD_CUNEICODE_API_LINKAGE_I_ cnc_mcerr cnc_mcsnrtoc32sn_iso_8859_15(size_t* __p_maybe_dst_len,
     ztd_char32_t** __p_maybe_dst, size_t* __p_src_len, const ztd_char_t** __p_src,
     cnc_mcstate_t* __p_state) ZTD_USE(ZTD_NOEXCEPT_IF_CXX);


//////
/// @}

#endif
