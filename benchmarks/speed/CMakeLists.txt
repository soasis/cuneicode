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
file(GLOB_RECURSE ztd.cuneicode.benchmarks.speed.sources
	CONFIGURE_DEPENDS
	LIST_DIRECTORIES NO
	source/**)

add_executable(ztd.cuneicode.benchmarks.speed ${ztd.cuneicode.benchmarks.speed.sources})
target_include_directories(ztd.cuneicode.benchmarks.speed
	PRIVATE
		include
)
target_link_libraries(ztd.cuneicode.benchmarks.speed
	PRIVATE
		ztd::platform
		ztd::cuneicode
		ztd::cuneicode::shared
		ztd::cuneicode::benchmarks::barrier
		ztd::cuneicode::shared::simdutf
		ctre::ctre
		utf8::cpp
		Boost::text
		benchmark::benchmark
		${CMAKE_DL_LIBS}
)
target_compile_options(ztd.cuneicode.benchmarks.speed
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
generate_config_and_manifest_for(ztd.cuneicode.benchmarks.speed)
configure_graph_benchmark_targets(
	NAME
		speed
	CONFIG
		"${CMAKE_CURRENT_SOURCE_DIR}/graph_config.json"
	TARGETS
		ztd.cuneicode.benchmarks.speed
	REPETITIONS
		${ZTD_CUNEICODE_BENCHMARKS_REPETITIONS}
)
