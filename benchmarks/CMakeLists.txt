# =============================================================================
#
# ztd.cuneicode
# Copyright © 2022-2022 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
# Contact: opensource@soasis.org
#
# Commercial License Usage
# Licensees holding valid commercial ztd.cuneicode licenses may use this file
# in accordance with the commercial license agreement provided with the
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
# 		https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# ============================================================================>

include(FetchContent)

# # Fetch dependencies
# Google Benchmark
FetchContent_Declare(
	googlebenchmark
	GIT_REPOSITORY https://github.com/google/benchmark.git
)
FetchContent_GetProperties(googlebenchmark)
if(NOT googlebenchmark_POPULATED)
	FetchContent_Populate(googlebenchmark)
	set(BENCHMARK_ENABLE_INSTALL OFF CACHE INTERNAL "")
	set(BENCHMARK_ENABLE_TESTING OFF CACHE INTERNAL "")
	set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE INTERNAL "")
	set(BENCHMARK_ENABLE_WERROR OFF CACHE INTERNAL "")
	set(BENCHMARK_FORCE_WERROR OFF CACHE INTERNAL "")
	add_subdirectory(${googlebenchmark_SOURCE_DIR} ${googlebenchmark_BINARY_DIR})
endif()

add_subdirectory(barrier_conversion)
add_subdirectory(function_form)
