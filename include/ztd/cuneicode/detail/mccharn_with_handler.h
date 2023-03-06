// ============================================================================
//
// ztd.cuneicode
// Copyright © 2022-2023 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
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
// Version 2.0 _with_handler(the "License"); you may not use this file except in compliance
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

#ifndef ZTD_CUNEICODE_DETAIL_MCCHARN_WITH_HANDLER_H
#define ZTD_CUNEICODE_DETAIL_MCCHARN_WITH_HANDLER_H

#pragma once

#include <ztd/cuneicode/version.h>

#include <ztd/cuneicode/detail/decl_boilerplate.h>

CNC_TYPED_CONVERSION_ERROR_DECL_I_(mc, mc, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mc, mwc, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mc, c8, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mc, c16, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mc, c32, );

CNC_TYPED_CONVERSION_ERROR_DECL_I_(mwc, mc, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mwc, mwc, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mwc, c8, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mwc, c16, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mwc, c32, );

CNC_TYPED_CONVERSION_ERROR_DECL_I_(c8, mc, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c8, mwc, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c8, c8, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c8, c16, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c8, c32, );

CNC_TYPED_CONVERSION_ERROR_DECL_I_(c16, mc, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c16, mwc, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c16, c8, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c16, c16, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c16, c32, );

CNC_TYPED_CONVERSION_ERROR_DECL_I_(c32, mc, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c32, mwc, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c32, c8, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c32, c16, );
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c32, c32, );

CNC_TYPED_CONVERSION_ERROR_DECL_I_(c32, mc, _utf8);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c32, mwc, _utf16);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c32, mwc, _utf32);

CNC_TYPED_CONVERSION_ERROR_DECL_I_(c16, mc, _utf8);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c16, mwc, _utf16);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c16, mwc, _utf32);

CNC_TYPED_CONVERSION_ERROR_DECL_I_(c8, mc, _utf8);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c8, mwc, _utf16);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(c8, mwc, _utf32);

CNC_TYPED_CONVERSION_ERROR_DECL_I_(mc, mc, _exec_utf8);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mc, mwc, _exec_utf16);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mc, mwc, _exec_utf32);

CNC_TYPED_CONVERSION_ERROR_DECL_I_(mwc, mc, _wide_exec_utf8);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mwc, mc, _utf16_utf8);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mwc, mc, _utf32_utf8);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mwc, mwc, _wide_exec_utf16);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mwc, mwc, _wide_exec_utf32);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mwc, mwc, _utf16_wide_exec);
CNC_TYPED_CONVERSION_ERROR_DECL_I_(mwc, mwc, _utf32_wide_exec);


//////
/// @addtogroup ztd_cuneicode_single_conversion_functions Single Conversion Functions
/// @{

//////
/// @}

#endif
