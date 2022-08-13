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

#include <ztd/idk/c_span.h>
#include <barrier/barrier.h>

#include <unicode/utypes.h>
#include <unicode/stringpiece.h>
#include <unicode/utf8.h>
#include <unicode/uchar.h>
#include <unicode/ustring.h>
#include <unicode/unistr.h>
#include <unicode/ucnv.h>

#include <memory>

struct uconv_close_converter {
	void operator()(UConverter* conv) noexcept {
		ucnv_close(conv);
	}
};

static void utf16_to_utf8_well_formed_icu_UnicodeString_toUTF8(benchmark::State& state) {
	std::vector<ztd_char8_t> output_data(c_span_char8_t_size(u8_data));
	icu::UnicodeString str(
	     c_span_char16_t_data(u16_data), (int32_t)c_span_char16_t_size(u16_data));
	const bool is_input_equal = !std::equal(str.getBuffer(), str.getBuffer() + str.length(),
	     c_span_char16_t_data(u16_data),
	     c_span_char16_t_data(u16_data) + c_span_char16_t_size(u16_data));
	if (is_input_equal) {
		state.SkipWithError("bad benchmark result");
		return;
	}
	bool result = true;
	for (auto _ : state) {
		icu::CheckedArrayByteSink sink((char*)output_data.data(), (int32_t)output_data.size());
		str.toUTF8(sink);
	}
	const bool is_equal
	     = std::equal(output_data.cbegin(), output_data.cend(), c_span_char8_t_data(u8_data),
	          c_span_char8_t_data(u8_data) + c_span_char8_t_size(u8_data));
	if (!result || !is_equal) {
		state.SkipWithError("bad benchmark result");
	}
}

static void utf16_to_utf8_well_formed_icu_u_strToUTF8(benchmark::State& state) {
	std::vector<ztd_char8_t> output_data(c_span_char8_t_size(u8_data) + 1);
	icu::UnicodeString str(
	     c_span_char16_t_data(u16_data), (int32_t)c_span_char16_t_size(u16_data));
	const bool is_input_equal = !std::equal(str.getBuffer(), str.getBuffer() + str.length(),
	     c_span_char16_t_data(u16_data),
	     c_span_char16_t_data(u16_data) + c_span_char16_t_size(u16_data));
	if (is_input_equal) {
		state.SkipWithError("bad benchmark result");
		return;
	}
	bool result = true;
	for (auto _ : state) {
		UErrorCode err     = U_ZERO_ERROR;
		int32_t output_len = (int32_t)output_data.size();
		u_strToUTF8((char*)output_data.data(), (int32_t)output_data.size(), &output_len,
		     str.getBuffer(), (int32_t)str.length(), &err);
		if (err != U_ZERO_ERROR) {
			result = false;
		}
	}
	const bool is_equal
	     = std::equal(output_data.cbegin(), output_data.cend() - 1, c_span_char8_t_data(u8_data),
	          c_span_char8_t_data(u8_data) + c_span_char8_t_size(u8_data));
	if (!result || !is_equal) {
		state.SkipWithError("bad benchmark result");
	}
}

static void utf16_to_utf8_well_formed_icu_ucnv_Converter(benchmark::State& state) {
	std::vector<ztd_char8_t> output_data(c_span_char8_t_size(u8_data) + 1);
	icu::UnicodeString str(
	     c_span_char16_t_data(u16_data), (int32_t)c_span_char16_t_size(u16_data));
	const bool is_input_equal = !std::equal(str.getBuffer(), str.getBuffer() + str.length(),
	     c_span_char16_t_data(u16_data),
	     c_span_char16_t_data(u16_data) + c_span_char16_t_size(u16_data));
	if (is_input_equal) {
		state.SkipWithError("bad benchmark result");
		return;
	}
	UErrorCode open_err;
	std::unique_ptr<UConverter, uconv_close_converter> conv = nullptr;
	{
		UConverter* raw_conv = ucnv_open("utf-8", &open_err);
		if (open_err != U_ZERO_ERROR) {
			state.SkipWithError("bad benchmark result");
			return;
		}
	}
	bool result = true;
	for (auto _ : state) {
		UErrorCode err     = U_ZERO_ERROR;
		int32_t output_len = (int32_t)output_data.size();
		int32_t len        = ucnv_fromUChars(conv.get(), (char*)output_data.data(), output_len,
		            str.getBuffer(), (int32_t)str.length(), &err);
		if (err != U_ZERO_ERROR) {
			result = false;
		}
	}
	const bool is_equal
	     = std::equal(output_data.cbegin(), output_data.cend() - 1, c_span_char8_t_data(u8_data),
	          c_span_char8_t_data(u8_data) + c_span_char8_t_size(u8_data));
	if (!result || !is_equal) {
		state.SkipWithError("bad benchmark result");
	}
}

BENCHMARK(utf16_to_utf8_well_formed_icu_ucnv_Converter);
BENCHMARK(utf16_to_utf8_well_formed_icu_u_strToUTF8);
BENCHMARK(utf16_to_utf8_well_formed_icu_UnicodeString_toUTF8);
