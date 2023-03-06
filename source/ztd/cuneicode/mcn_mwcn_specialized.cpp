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

#include <ztd/cuneicode.h>

#include <ztd/cuneicode/detail/defn_boilerplate.h>

CNC_TYPED_CONVERSION_DEFN_I_(mc, mc, c8, mc, _utf8_exec);
CNC_TYPED_CONVERSION_DEFN_I_(mc, mc, mc, c8, _exec_utf8);
CNC_TYPED_CONVERSION_DEFN_I_(mc, mc, c8, c8, _utf8_utf8);

CNC_TYPED_CONVERSION_DEFN_I_(mc, c32, c8, c32, _utf8);
CNC_TYPED_CONVERSION_DEFN_I_(c32, mc, c32, c8, _utf8);

CNC_TYPED_CONVERSION_DEFN_I_(mc, c16, c8, c16, _utf8);
CNC_TYPED_CONVERSION_DEFN_I_(c16, mc, c16, c8, _utf8);

CNC_TYPED_CONVERSION_DEFN_I_(mc, mwc, c8, mwc, _utf8_wide_exec);
CNC_TYPED_CONVERSION_DEFN_I_(mwc, mc, mwc, c8, _wide_exec_utf8);
CNC_TYPED_CONVERSION_DEFN_I_(mc, mwc, c8, c16, _utf8_utf16);
CNC_TYPED_CONVERSION_DEFN_I_(mwc, mc, c16, c8, _utf16_utf8);
CNC_TYPED_CONVERSION_DEFN_I_(mc, mwc, c8, c32, _utf8_utf32);
CNC_TYPED_CONVERSION_DEFN_I_(mwc, mc, c32, c8, _utf32_utf8);

CNC_TYPED_CONVERSION_DEFN_I_(mc, mwc, mc, c16, _exec_utf16);
CNC_TYPED_CONVERSION_DEFN_I_(mwc, mc, c16, mc, _utf16_exec);
CNC_TYPED_CONVERSION_DEFN_I_(mc, mwc, mc, c32, _exec_utf32);
CNC_TYPED_CONVERSION_DEFN_I_(mwc, mc, c32, mc, _utf32_exec);
