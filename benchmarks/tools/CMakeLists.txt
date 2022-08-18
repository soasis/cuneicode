# =============================================================================
#
# ztd.idk
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

function (ztd_tools_make_benchmark_graphing_targets benchmark_target json_graph_config_file repetition_count)
	if (NOT repetition_count)
		if (ZTD_TOOLS_BENCHMARKS_REPETITIONS)
			set(repetition_count ${ZTD_TOOLS_BENCHMARKS_REPETITIONS})
		else()
			set(repetition_count 50)
		endif()
	endif()

	cmake_path(GET json_graph_config_file FILENAME json_graph_config_file_filename)
	set(ZTD_TOOLS_BENCHMARKS_CONFIG_SOURCEFILE "${json_graph_config_file}")
	set(ZTD_TOOLS_BENCHMARKS_CONFIG_INPUTFILE "${CMAKE_BINARY_DIR}/benchmark_results/${benchmark_target}.${json_graph_config_file_filename}")
	set(ZTD_TOOLS_BENCHMARKS_RESULTS_OUTFILE "${CMAKE_BINARY_DIR}/benchmark_results/${benchmark_target}.json")
	set(ZTD_TOOLS_BENCHMARKS_RESULTS_OUTDIR "${CMAKE_BINARY_DIR}/benchmark_results/")

	# # Commands and Targets
	add_custom_command(
		OUTPUT ${ZTD_TOOLS_BENCHMARKS_CONFIG_INPUTFILE}
		COMMAND ${CMAKE_COMMAND} -E copy
			${ZTD_TOOLS_BENCHMARKS_CONFIG_SOURCEFILE}
			${ZTD_TOOLS_BENCHMARKS_CONFIG_INPUTFILE}
		DEPENDS ${ZTD_TOOLS_BENCHMARKS_CONFIG_SOURCEFILE}
		COMMENT "[ztd.tools] Copying configuration file from '${ZTD_TOOLS_BENCHMARKS_CONFIG_SOURCEFILE}' to '${ZTD_TOOLS_BENCHMARKS_CONFIG_INPUTFILE}'."
	)

	add_custom_target(ztd.tools.benchmarks_config_generator.${benchmark_target}
		DEPENDS "${ZTD_TOOLS_BENCHMARKS_CONFIG_SOURCEFILE}" "${ZTD_TOOLS_BENCHMARKS_CONFIG_INPUTFILE}"
	)

	add_custom_command(
		OUTPUT "${ZTD_TOOLS_BENCHMARKS_RESULTS_OUTFILE}"
		COMMAND ${benchmark_target}
			--benchmark_out=${ZTD_TOOLS_BENCHMARKS_RESULTS_OUTFILE}
			--benchmark_out_format=json
			--benchmark_repetitions=${repetition_count}
		DEPENDS ${benchmark_target}
		COMMENT "[ztd.tools] Executing Benchmarks and outputting to '${ZTD_TOOLS_BENCHMARKS_RESULTS_OUTFILE}'"
	)

	add_custom_target(ztd.tools.benchmarks_data_generator.${benchmark_target}
		DEPENDS "${ZTD_TOOLS_BENCHMARKS_RESULTS_OUTFILE}"
	)

	if (Python3_Interpreter_FOUND)
		add_custom_target(ztd.tools.benchmarks_graphs_generator.${benchmark_target}
			COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_FUNCTION_LIST_DIR}/generate_graphs/__main__.py
				-c "${ZTD_TOOLS_BENCHMARKS_CONFIG_SOURCEFILE}"
				-i "${ZTD_TOOLS_BENCHMARKS_RESULTS_OUTFILE}"
				-o "${ZTD_TOOLS_BENCHMARKS_RESULTS_OUTDIR}"
			DEPENDS ztd.tools.benchmarks_data_generator.${benchmark_target} ztd.tools.benchmarks_config_generator.${benchmark_target}
			COMMENT "[ztd.tools] Graphing '${ZTD_TOOLS_BENCHMARKS_RESULTS_OUTFILE}' data to '${ZTD_TOOLS_BENCHMARKS_RESULTS_OUTDIR}'"
		)
	endif()
endfunction()
