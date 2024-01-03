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

#ifndef ZTD_CUNEICODE_SOURCE_DETAIL_STRING_VIEW_HPP
#define ZTD_CUNEICODE_SOURCE_DETAIL_STRING_VIEW_HPP

#include <ztd/cuneicode/version.h>

#include <ztd/idk/char_traits.hpp>

#include <string_view>

using __cnc_u8string_view = ::std::basic_string_view<::ztd::uchar8_t, ::ztd::uchar8_traits>;

#endif
