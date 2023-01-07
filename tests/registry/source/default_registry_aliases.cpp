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

#include <catch2/catch_all.hpp>

#include <ztd/cuneicode.h>

#include <ztd/idk/size.hpp>
#include <ztd/idk/encoding_name.hpp>

#include <iterator>
#include <algorithm>
#include <cstring>
#include <string_view>
#include <tuple>

inline namespace cnc_tests_registry_default_registry_aliases {

	struct cnc_conversion_deleter {
		void operator()(cnc_conversion* handle) const {
			cnc_conv_delete(handle);
		}
	};

	struct cnc_registry_deleter {
		void operator()(cnc_conversion_registry* handle) const {
			cnc_registry_delete(handle);
		}
	};

} // namespace cnc_tests_registry_default_registry_aliases


TEST_CASE(
     "conversion from a alias encoding to UTF-32, which will give the original name in the "
     "cnc_conversion_info structure",
     "[cuneicode][registry][default registry aliases]") {
	// open the registry
	std::unique_ptr<cnc_conversion_registry, cnc_registry_deleter> registry = nullptr;
	{
		cnc_conversion_registry* raw_registry = registry.get();
		cnc_registry_options registry_options = CNC_REGISTRY_OPTIONS_DEFAULT;
		cnc_open_error err                    = cnc_registry_new(&raw_registry, registry_options);
		REQUIRE(err == CNC_OPEN_ERROR_OK);
		registry.reset(raw_registry);
	}
	// open the conversions with alias names
	SECTION("execution") {
		std::unique_ptr<cnc_conversion, cnc_conversion_deleter> conversion = nullptr;
		cnc_conversion_info info                                           = {};
		{
			std::string_view alias_name("char");
			std::string_view original_name("execution");
			std::string_view utf32_name("UTF-32");
			cnc_conversion* raw_conversion      = conversion.get();
			std::size_t from_size               = alias_name.size();
			const ztd_char8_t* from_data        = (const ztd_char8_t*)alias_name.data();
			std::size_t to_size                 = utf32_name.size();
			const ztd_char8_t* to_data          = (const ztd_char8_t*)utf32_name.data();
			cnc_conversion_registry* __registry = registry.get();
			cnc_open_error err                  = cnc_conv_new_c8n(
                    __registry, from_size, from_data, to_size, to_data, &raw_conversion, &info);
			REQUIRE(err == CNC_OPEN_ERROR_OK);
			std::string_view from_name((const char*)info.from_code_data, info.from_code_size);
			std::string_view to_name((const char*)info.to_code_data, info.to_code_size);
			std::string_view indirect_name(
			     (const char*)info.indirect_code_data, info.indirect_code_size);
			REQUIRE(ztd::is_encoding_name_equal(original_name, from_name));
			REQUIRE(ztd::is_encoding_name_equal(utf32_name, to_name));
			conversion.reset(raw_conversion);
		}
	}
	SECTION("wide execution") {
		std::unique_ptr<cnc_conversion, cnc_conversion_deleter> conversion = nullptr;
		cnc_conversion_info info                                           = {};
		{
			std::string_view alias_name("wchar_t");
			std::string_view original_name("wide-execution");
			std::string_view utf32_name("UTF-32");
			cnc_conversion* raw_conversion      = conversion.get();
			std::size_t from_size               = alias_name.size();
			const ztd_char8_t* from_data        = (const ztd_char8_t*)alias_name.data();
			std::size_t to_size                 = utf32_name.size();
			const ztd_char8_t* to_data          = (const ztd_char8_t*)utf32_name.data();
			cnc_conversion_registry* __registry = registry.get();
			cnc_open_error err                  = cnc_conv_new_c8n(
                    __registry, from_size, from_data, to_size, to_data, &raw_conversion, &info);
			REQUIRE(err == CNC_OPEN_ERROR_OK);
			std::string_view from_name((const char*)info.from_code_data, info.from_code_size);
			std::string_view to_name((const char*)info.to_code_data, info.to_code_size);
			std::string_view indirect_name(
			     (const char*)info.indirect_code_data, info.indirect_code_size);
			REQUIRE(ztd::is_encoding_name_equal(original_name, from_name));
			REQUIRE(ztd::is_encoding_name_equal(utf32_name, to_name));
			conversion.reset(raw_conversion);
		}
	}
}
