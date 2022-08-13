# =============================================================================
#
# ztd.cuneicode
# Copyright © 2022 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
# Contact: opensource@soasis.org
#
# Commercial License Usage
# Licensees holding valid commercial ztd.idk licenses may use this file in
# accordance with the commercial license agreement provided with the
# Software or, alternatively, in accordance with the terms contained in
# a written agreement between you and Shepherd's Oasis, LLC.
# For licensing terms and conditions see your agreement. For
# further information contact opensource@soasis.org.
#
# Apache License Version 2 Usage
# Alternatively, this file may be used under the terms of Apache License
# Version 2.0 (the "License") for non-commercial use; you may not use this
# file except in compliance with the License. You may obtain a copy of the
# License at
#
#		http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# ============================================================================>

# # Benchmark Executable
file(GLOB_RECURSE ztd.cuneicode.benchmarks.speed.utf16_to_utf8.sources
	CONFIGURE_DEPENDS
	LIST_DIRECTORIES NO
	source/**)

# libraries and source file culling
if (NOT ICU_I18N_FOUND OR NOT ICU_UC_FOUND)
	# we don't have ICU: cull any ICU-related sources
	list(FILTER ztd.cuneicode.benchmarks.speed.utf16_to_utf8.sources
		EXCLUDE
		REGEX
		"icu")
endif()
string(CONCAT --icu-libraries-i18n
	$<
		$<BOOL:${ICU_I18N_FOUND}>:
		ICU::i18n
	>
)
string(CONCAT --icu-libraries-uc
	$<
		$<BOOL:${ICU_UC_FOUND}>:
		ICU::uc
	>
)

if (NOT TARGET Iconv::Iconv)
	# we don't have libiconv: cull any iconv-related sources
	list(FILTER ztd.cuneicode.benchmarks.speed.utf16_to_utf8.sources
		EXCLUDE
		REGEX
		"iconv")
endif()
string(CONCAT --iconv-libraries
	$<
		$<TARGET_EXISTS:Iconv::Iconv>:
		Iconv::Iconv
	>
)
if (NOT TARGET boost::text)
	# we don't have boost.text - cull any boost.text-related sources
	list(FILTER ztd.cuneicode.benchmarks.speed.utf16_to_utf8.sources
		EXCLUDE
		REGEX
		"boost.text")
endif()
string(CONCAT --boost.text-libraries
	$<
		$<TARGET_EXISTS:boost::text>:
		boost::text
	>
)

if (NOT TARGET simdutf::simdutf)
	# we don't have boost.text - cull any boost.text-related sources
	list(FILTER ztd.cuneicode.benchmarks.speed.utf16_to_utf8.sources
		EXCLUDE
		REGEX
		"simdutf")
endif()
string(CONCAT --simdutf-libraries
	$<
		$<TARGET_EXISTS:simdutf::simdutf>:
		ztd::cuneicode::shared::simdutf
	>
)

add_executable(ztd.cuneicode.benchmarks.speed.utf16_to_utf8 ${ztd.cuneicode.benchmarks.speed.utf16_to_utf8.sources})
target_include_directories(ztd.cuneicode.benchmarks.speed.utf16_to_utf8 PRIVATE
	include
)
target_link_libraries(ztd.cuneicode.benchmarks.speed.utf16_to_utf8
	PRIVATE
	ztd::cuneicode
	ztd::cuneicode::shared
	ztd::cuneicode::benchmarks::barrier
	${--simdutf-libraries} # include simdutf library, if they exist
	${--icu-libraries-i18n} # include ICU components, if they exist
	${--icu-libraries-uc} # include ICU components, if they exist
	${--iconv-libraries} # include iconv libraries, if it exists
	Boost::boost
	${--boost.text-libraries} # include boost.text library, if it exists
	${CMAKE_DL_LIBS}
	benchmark::benchmark
)
target_compile_options(ztd.cuneicode.benchmarks.speed.utf16_to_utf8
	PRIVATE
	${--utf8-literal-encoding}
	${--utf8-source-encoding}
	${--disable-permissive}
	${--updated-cpp-version-flag}
	${--warn-pedantic}
	${--warn-default}
	${--warn-extra}
	${--warn-errors}
	${--allow-microsoft-enum-value}
)
get_target_property(prop_value ICU::uc TYPE)
message(STATUS TYPE: ${prop_value})
get_target_property(prop_value ICU::uc IMPORTED_LOCATION)
message(STATUS IMPORTED_LOCATION: ${prop_value})
generate_config_and_manifest_for(ztd.cuneicode.benchmarks.speed.utf16_to_utf8)
