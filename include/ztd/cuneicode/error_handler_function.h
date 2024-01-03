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

#ifndef ZTD_CUNEICODE_ERROR_HANDLER_FUNCTION_H
#define ZTD_CUNEICODE_ERROR_HANDLER_FUNCTION_H

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/mcerr.h>
#include <ztd/cuneicode/detail/decl_boilerplate.h>

#include <ztd/idk/charN_t.h>
#include <ztd/idk/restrict.h>

#if ZTD_IS_ON(ZTD_CXX)
#include <cstddef>
#else
#include <stddef.h>
#endif

#define CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(_PREFIX_FROM, _PREFIX_TO)          \
	typedef cnc_mcerr(cnc_##_PREFIX_FROM##_PREFIX_TO##_error_handler_function)( \
	     cnc_mcerr __current_eror, size_t* restrict __p_maybe_dst_len,          \
	     TAG_TO_TYPE_I_(_PREFIX_FROM)* restrict* restrict __p_maybe_dst,        \
	     size_t* restrict __p_src_len,                                          \
	     const TAG_TO_TYPE_I_(_PREFIX_TO)* restrict* restrict __p_src, void* restrict __p_state)

CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(mc, mc);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(mc, mwc);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(mc, c8);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(mc, c16);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(mc, c32);

CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(mwc, mc);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(mwc, mwc);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(mwc, c8);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(mwc, c16);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(mwc, c32);

CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c8, mc);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c8, mwc);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c8, c8);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c8, c16);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c8, c32);

CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c16, mc);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c16, mwc);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c16, c8);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c16, c16);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c16, c32);

CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c32, mc);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c32, mwc);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c32, c8);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c32, c16);
CNC_ERROR_HANDLER_FUNCTION_TYPEDEF_I_(c32, c32);

#endif
