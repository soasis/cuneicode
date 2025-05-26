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

#include <catch2/catch_all.hpp>

#include <ztd/cuneicode.h>

#include <ztd/tests/basic_unicode_strings.hpp>
#include <ztd/idk/detail/unicode.hpp>

#include <iterator>
#include <algorithm>
#include <cstring>

TEST_CASE(
     "check if the a larger unicode code point going through a windows code page works well enough",
     "[cuneicode][direct][corner-case][large-single-code-point]") {
#if ZTD_IS_ON(ZTD_PLATFORM_WINDOWS)
	constexpr std::wstring_view sourcemwc   = L"\U0001F377";
	constexpr std::u32string_view sourcec32 = U"\U0001F377";
	constexpr std::string_view sourcemc     = "\xF0\x9F\x8D\xB7";
	constexpr uint32_t utf8_codepage        = 65001;
	SECTION("from mwc") {
		SECTION("undersized output") {
			char destination[1]                = {};
			const std::size_t source_size      = sourcemwc.size();
			const wchar_t* const source_input  = sourcemwc.data();
			const std::size_t destination_size = ztdc_c_array_size(destination);
			const wchar_t* input               = source_input;
			std::size_t input_size             = source_size;
			char* output                       = destination;
			std::size_t output_size            = destination_size;
			cnc_mcstate_t state                = {};
			cnc_mcstate_set_win32_code_page(&state, utf8_codepage);
			cnc_mcerr err = cnc_mwcnrtomcn_windows_code_page(
			     &output_size, &output, &input_size, &input, &state);
			REQUIRE(err == cnc_mcerr_insufficient_output);
			REQUIRE(input == source_input);
			REQUIRE(input_size == source_size);
			REQUIRE(output == &destination[0]);
			REQUIRE(output_size == destination_size);
		}
		SECTION("approrpiately sized output") {
			char destination[4]                = {};
			const std::size_t source_size      = sourcemwc.size();
			const wchar_t* const source_input  = sourcemwc.data();
			const std::size_t destination_size = ztdc_c_array_size(destination);
			const wchar_t* input               = source_input;
			std::size_t input_size             = source_size;
			char* output                       = destination;
			std::size_t output_size            = destination_size;
			cnc_mcstate_t state                = {};
			cnc_mcstate_set_win32_code_page(&state, utf8_codepage);
			cnc_mcerr err = cnc_mwcnrtomcn_windows_code_page(
			     &output_size, &output, &input_size, &input, &state);
			REQUIRE(err == cnc_mcerr_ok);
			REQUIRE(input == (source_input + source_size));
			REQUIRE(input_size == 0);
			REQUIRE(output == (&destination[0] + destination_size));
			REQUIRE(output_size == 0);
			REQUIRE(std::string_view(destination, 4) == sourcemc);
		}
	}
	SECTION("from c32") {
		SECTION("undersized output") {
			char destination[1]                = {};
			const std::size_t source_size      = sourcec32.size();
			const char32_t* const source_input = sourcec32.data();
			const std::size_t destination_size = ztdc_c_array_size(destination);
			const char32_t* input              = source_input;
			std::size_t input_size             = source_size;
			char* output                       = destination;
			std::size_t output_size            = destination_size;
			cnc_mcstate_t state                = {};
			cnc_mcstate_set_win32_code_page(&state, utf8_codepage);
			cnc_mcerr err = cnc_c32nrtomcn_windows_code_page(
			     &output_size, &output, &input_size, &input, &state);
			REQUIRE(err == cnc_mcerr_insufficient_output);
			REQUIRE(input == source_input);
			REQUIRE(input_size == source_size);
			REQUIRE(output == &destination[0]);
			REQUIRE(output_size == destination_size);
		}
		SECTION("approrpiately sized output") {
			char destination[4]                = {};
			const std::size_t source_size      = sourcec32.size();
			const char32_t* const source_input = sourcec32.data();
			const std::size_t destination_size = ztdc_c_array_size(destination);
			const char32_t* input              = source_input;
			std::size_t input_size             = source_size;
			char* output                       = destination;
			std::size_t output_size            = destination_size;
			cnc_mcstate_t state                = {};
			cnc_mcstate_set_win32_code_page(&state, utf8_codepage);
			cnc_mcerr err = cnc_c32nrtomcn_windows_code_page(
			     &output_size, &output, &input_size, &input, &state);
			REQUIRE(err == cnc_mcerr_ok);
			REQUIRE(input == (source_input + source_size));
			REQUIRE(input_size == 0);
			REQUIRE(output == (&destination[0] + destination_size));
			REQUIRE(output_size == 0);
			REQUIRE(std::string_view(destination, 4) == sourcemc);
		}
	}
	SECTION("to mwc") {
		SECTION("undersized output") {
			wchar_t destination[1]             = {};
			const std::size_t source_size      = sourcemc.size();
			const char* const source_input     = sourcemc.data();
			const std::size_t destination_size = ztdc_c_array_size(destination);
			const char* input                  = source_input;
			std::size_t input_size             = source_size;
			wchar_t* output                    = destination;
			std::size_t output_size            = destination_size;
			cnc_mcstate_t state                = {};
			cnc_mcstate_set_win32_code_page(&state, utf8_codepage);
			cnc_mcerr err = cnc_mcnrtomwcn_windows_code_page(
			     &output_size, &output, &input_size, &input, &state);
			REQUIRE(err == cnc_mcerr_insufficient_output);
			REQUIRE(input == source_input);
			REQUIRE(input_size == source_size);
			REQUIRE(output == &destination[0]);
			REQUIRE(output_size == destination_size);
		}
		SECTION("appropriately sized output") {
			wchar_t destination[2]             = {};
			const std::size_t source_size      = sourcemc.size();
			const char* const source_input     = sourcemc.data();
			const std::size_t destination_size = ztdc_c_array_size(destination);
			const char* input                  = source_input;
			std::size_t input_size             = source_size;
			wchar_t* output                    = destination;
			std::size_t output_size            = destination_size;
			cnc_mcstate_t state                = {};
			cnc_mcstate_set_win32_code_page(&state, utf8_codepage);
			cnc_mcerr err = cnc_mcnrtomwcn_windows_code_page(
			     &output_size, &output, &input_size, &input, &state);
			REQUIRE(err == cnc_mcerr_ok);
			REQUIRE(input == (source_input + source_size));
			REQUIRE(input_size == 0);
			REQUIRE(output == (&destination[0] + destination_size));
			REQUIRE(output_size == 0);
			REQUIRE(std::wstring_view(destination, 2) == sourcemwc);
		}
	}
#else
	REQUIRE(true); // all fine
#endif
}
