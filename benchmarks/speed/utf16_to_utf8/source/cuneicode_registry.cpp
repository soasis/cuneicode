// ============================================================================
//
// ztd.cuneicode
// Copyright © 2022-2022 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
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
// 		http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// ========================================================================= //

#include <benchmark/benchmark.h>

#include <ztd/cuneicode.h>
#include <ztd/cuneicode/shared/simdutf/registry.hpp>
#include <barrier/barrier.h>

#include <memory>
#include <vector>

struct conversion_deleter {
	void operator()(cnc_conversion* conv) const noexcept {
		cnc_conv_delete(conv);
	}
};

struct registry_deleter {
	void operator()(cnc_conversion_registry* registry) const noexcept {
		cnc_delete_registry(registry);
	}
};

template <bool Fast>
static void utf16_to_utf8_well_formed_cuneicode_registry_core(benchmark::State& state) {
	const std::vector<char16_t> input_data(c_span_char16_t_data(u16_data),
	     c_span_char16_t_data(u16_data) + c_span_char16_t_size(u16_data));
	std::vector<unsigned char> output_data(c_span_char8_t_size(u8_data));
	// create registry to use, and conversion descriptor too
	std::unique_ptr<cnc_conversion_registry, registry_deleter> registry = nullptr;
	std::unique_ptr<cnc_conversion, conversion_deleter> conversion      = nullptr;
	{
		cnc_conversion_registry* raw_registry = nullptr;
		cnc_conversion* raw_conversion        = nullptr;
		cnc_conversion_info info              = {};
		const cnc_open_error err = cnc_new_registry(&raw_registry, CNC_REGISTRY_OPTIONS_NONE);
		if (err != CNC_OPEN_ERROR_OKAY) {
			// something went wrong, get out of here quick!
			state.SkipWithError("bad benchmark result");
			return;
		}
		registry.reset(raw_registry);
		if constexpr (Fast) {
			if (!add_simdutf_to_registry(registry.get())) {
				// something went wrong, get out of here quick!
				state.SkipWithError("bad benchmark result");
				return;
			}
		}
		const cnc_open_error conv_err
		     = cnc_conv_new(registry.get(), (const ztd_char8_t*)u8"UTF-16",
		          (const ztd_char8_t*)u8"UTF-8", &raw_conversion, &info);
		if (conv_err != CNC_OPEN_ERROR_OKAY) {
			// something went wrong, get out of here quick!
			state.SkipWithError("bad benchmark result");
			return;
		}
		conversion.reset(raw_conversion);
	}
	bool result = true;
	for (auto _ : state) {
		size_t input_size          = input_data.size() * sizeof(*input_data.data());
		const unsigned char* input = (const unsigned char*)input_data.data();
		size_t output_size         = output_data.size() * sizeof(*output_data.data());
		unsigned char* output      = (unsigned char*)output_data.data();
		cnc_mcerror err = cnc_conv(conversion.get(), &output_size, &output, &input_size, &input);
		if (err != CNC_MCERROR_OKAY) {
			result = false;
		}
	}
	const bool is_equal
	     = std::equal(output_data.cbegin(), output_data.cend(), c_span_char8_t_data(u8_data),
	          c_span_char8_t_data(u8_data) + c_span_char8_t_size(u8_data));
	if (!result || !is_equal) {
		state.SkipWithError("bad benchmark result");
	}
}

static void utf16_to_utf8_well_formed_cuneicode_registry(benchmark::State& state) {
	utf16_to_utf8_well_formed_cuneicode_registry_core<false>(state);
}

static void utf16_to_utf8_well_formed_cuneicode_registry_fast(benchmark::State& state) {
	utf16_to_utf8_well_formed_cuneicode_registry_core<true>(state);
}

BENCHMARK(utf16_to_utf8_well_formed_cuneicode_registry);
BENCHMARK(utf16_to_utf8_well_formed_cuneicode_registry_fast);
