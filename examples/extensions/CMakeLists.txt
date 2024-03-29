# =============================================================================
#
# ztd.cuneicode
# Copyright © JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
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
# Version 2.0 (the "License"); you may not use this file except in compliance
# with the License. You may obtain a copy of the License at
#
# https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# =========================================================================== #

FILE(GLOB_RECURSE ztd.cuneicode.examples.extensions.stmt-exprs.sources
	LIST_DIRECTORIES FALSE
	CONFIGURE_DEPENDS
	source/**stmt_exprs**
)

foreach(extensions_source_file ${ztd.cuneicode.examples.extensions.stmt-exprs.sources})
		ztd_cuneicode_example_simple(${extensions_source_file} "ztd.cuneicode.examples.extensions" extensions_target_name)
		target_compile_options(${extensions_target_name}
			PRIVATE
			${--no-pedantic}
			${--no-warn-pedantic}
			${--allow-gnu-auto-type}
			${--allow-gnu-statement-expressions}
			${--allow-gnu-statement-expression-from-macro-expansion}
		)
	if (NOT MSVC)
		target_compile_features(${extensions_target_name}
			PRIVATE
			c_std_23)
		set_property(TARGET ${extensions_target_name}
			PROPERTY
			C_EXTENSIONS ON)
	endif()
endforeach()
