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

#ifndef ZTD_CUNEICODE_EXAMPLES_MY_CONVERSION_H
#define ZTD_CUNEICODE_EXAMPLES_MY_CONVERSION_H

#include <ztd/cuneicode.h>

#include <ztd/idk/size.h>

#include <string.h>
#include <stddef.h>
#include <assert.h>

typedef struct my_state {
	char accumulation;
	unsigned char accumulation_count;
	bool assume_valid_input;
} my_state;

#define MC_MAX_MY 4
#define C32_MAX_MY 2

extern inline bool my_state_is_complete(const my_state* p_state) {
	return p_state->accumulation_count == 0 && p_state->accumulation == '\0';
}

extern inline bool my_internal_my_to_c32_dump_state(const bool counting_only,
     const bool assume_enough_output, size_t* p_output_size,
     ztd_char32_t** p_output, my_state* p_state, cnc_mcerr* p_dump_state_err) {
	switch (p_state->accumulation) {
	case '\'':
		// serialize prime according to # in accumulation length
		switch (p_state->accumulation_count) {
		case 1:
			if (!assume_enough_output) {
				if (p_output_size[0] < 0) {
					*p_dump_state_err = cnc_mcerr_insufficient_output;
					return true;
				}
				p_output_size[0] -= 1;
			}
			if (!counting_only) {
				p_output[0][0] = U'\'';
				p_output[0] += 1;
			}
			p_state->accumulation       = '\0';
			p_state->accumulation_count = 0;
			*p_dump_state_err           = cnc_mcerr_ok;
			return true;
		case 2:
		case 3:
			if (!assume_enough_output) {
				if (p_output_size[0] < 0) {
					*p_dump_state_err = cnc_mcerr_insufficient_output;
					return true;
				}
				p_output_size[0] -= 1;
			}
			if (!counting_only) {
				p_output[0][0] = (ztd_char32_t)(U'\u2034'
				     - (3 - p_state->accumulation_count));
				p_output[0] += 1;
			}
			p_state->accumulation       = '\0';
			p_state->accumulation_count = 0;
			*p_dump_state_err           = cnc_mcerr_ok;
			return true;
		case 4:
			if (!assume_enough_output) {
				if (p_output_size[0] < 0) {
					*p_dump_state_err = cnc_mcerr_insufficient_output;
					return true;
				}
				p_output_size[0] -= 1;
			}
			if (!counting_only) {
				p_output[0][0] = U'\u2057';
				p_output[0] += 1;
			}
			p_state->accumulation       = '\0';
			p_state->accumulation_count = 0;
			*p_dump_state_err           = cnc_mcerr_ok;
			return true;
		default:
			// we should never end up here, so something has gone
			// CATASTROPHICALLY WRONG
			*p_dump_state_err = cnc_mcerr_invalid_sequence;
			return true;
		}
	case '.': {
		// just write out '.' if they are present
		// save current progress and only change output if
		// we have a successful write!
		ztd_char32_t unused_buffer[C32_MAX_MY];
		ztd_char32_t* output = counting_only ? &unused_buffer[0] : p_output[0];
		size_t output_size
		     = assume_enough_output ? C32_MAX_MY : p_output_size[0];
		for (; p_state->accumulation_count > 0; --p_state->accumulation_count) {
			if (output_size < 0) {
				*p_dump_state_err = cnc_mcerr_insufficient_output;
				return true;
			}
			output_size -= 1;
			output[0] = U'.';
			output += 1;
		}
		if (!assume_enough_output) {
			p_output_size[0] = output_size;
		}
		if (!counting_only) {
			p_output[0] = output;
		}
		p_state->accumulation       = '\0';
		p_state->accumulation_count = 0;
		*p_dump_state_err           = cnc_mcerr_ok;
		return true;
	}
	default:
		return false;
	}
}

extern inline cnc_mcerr my_to_c32(size_t* p_output_size, ztd_char32_t** p_output,
     size_t* p_input_size, const char** p_input, my_state* p_state) {
	// Converts from a "my" - encoded string to UTF-32.

	// create a default state if we need it
	my_state automatic_default = { 0 };
	if (p_state == NULL) {
		p_state = &automatic_default;
	}

	if (p_input == NULL || p_input[0] == NULL) {
		// input is null; zero the state and leave
		*p_state = (my_state) { 0 };
		return cnc_mcerr_ok;
	}

	// if the input is null, then the size must ALSO be a null pointer,
	// or else this is a constraint violation.
	assert(p_input_size);

	// helper variables to determine whether we should be writing to the output
	// values at all or updating the output count
	const bool counting_only        = (p_output == NULL || p_output[0] == NULL);
	const bool assume_enough_output = (p_output_size == NULL);

	if (p_input_size[0] < 1) {
		// empty input, check if there's state left
		if (!my_state_is_complete(p_state)) {
			cnc_mcerr dump_state_err;
			if (my_internal_my_to_c32_dump_state(counting_only,
			         assume_enough_output, p_output_size, p_output, p_state,
			         &dump_state_err)) {
				return dump_state_err;
			}
		}
		// otherwise, we are done.
		return cnc_mcerr_ok;
	}

	// get first code unit
	const unsigned char code_unit = (unsigned char)(p_input[0][0]);

	// if we cannot assume the input is valid...
	if (!p_state->assume_valid_input) {
		// then we check for the ASCII-only code points we want
		if (code_unit > 0x7F) {
			return cnc_mcerr_invalid_sequence;
		}
	}

	// time to decode to UTF-32
	switch (code_unit) {
	case '.':
		if (p_state->accumulation != '.' && p_state->accumulation != '\0') {
			// we have something we may need to serialize...
			cnc_mcerr dump_state_err;
			if (my_internal_my_to_c32_dump_state(counting_only,
			         assume_enough_output, p_output_size, p_output, p_state,
			         &dump_state_err)) {
				return dump_state_err;
			}
		}
		// consecutive "."s must be serialized into the special Unicode Code
		// Point U+2026 HORIZONTAL ELLIPSIS.
		switch (p_state->accumulation_count) {
		case 0:
			// this is the first "." we encountered
			p_state->accumulation = '.';
			p_state->accumulation_count += 1;
			break;
		case 1:
			// this is the second "." in a row we encountered
			p_state->accumulation_count += 1;
			break;
		case 2:
			if (!assume_enough_output) {
				// must check size
				if (p_output_size[0] < 1) {
					return cnc_mcerr_insufficient_output;
				}
				p_output_size[0] -= 1;
			}
			if (!counting_only) {
				// write out data
				p_output[0][0] = U'\u2026';
				p_output[0] += 1;
			}
			// advance the input
			p_input[0] += 1;
			p_input_size[0] -= 1;
			p_state->accumulation       = '\0';
			p_state->accumulation_count = 0;
			return cnc_mcerr_ok;
		default:
			// if this happens, something is broken; signal an error and cry in
			// a corner.
			return cnc_mcerr_invalid_sequence;
		}
		// advance input since we are currently okay
		p_input[0] += 1;
		p_input_size[0] -= 1;
		return cnc_mcerr_ok;
	case '\'':
		if (p_state->accumulation != '\'' && p_state->accumulation != '\0') {
			// we have something we may need to serialize...
			cnc_mcerr dump_state_err;
			if (my_internal_my_to_c32_dump_state(counting_only,
			         assume_enough_output, p_output_size, p_output, p_state,
			         &dump_state_err)) {
				return dump_state_err;
			}
		}
		// consecutive "'"s are interpreted as primes for any beyond 2
		// if it is just one, serialize only 1.
		switch (p_state->accumulation_count) {
		case 0:
			// this is the first "'" we encountered
			p_state->accumulation = '\'';
			p_state->accumulation_count += 1;
			break;
		case 1:
			// this is the second "." in a row we encountered
			p_state->accumulation_count += 1;
			break;
		case 2:
			// this is the second "." in a row we encountered
			p_state->accumulation_count += 1;
			break;
		case 3:
			if (!assume_enough_output) {
				// must check size
				if (p_output_size[0] < 1) {
					return cnc_mcerr_insufficient_output;
				}
				p_output_size[0] -= 1;
			}
			if (!counting_only) {
				// write out data
				p_output[0][0] = U'\u2057';
				p_output[0] += 1;
			}
			// advance the input
			p_input[0] += 1;
			p_input_size[0] -= 1;
			p_state->accumulation       = '\0';
			p_state->accumulation_count = 0;
			return cnc_mcerr_ok;
		default:
			// if this happens, something is broken; signal an error and cry in
			// a corner.
			return cnc_mcerr_invalid_sequence;
		}
		// advance input since we are currently okay
		p_input[0] += 1;
		p_input_size[0] -= 1;
		return cnc_mcerr_ok;
	default:
		// normal character: keep going.
		break;
	}

	if (p_state->accumulation != '\0') {
		// we have something we may need to serialize...
		cnc_mcerr dump_state_err;
		if (my_internal_my_to_c32_dump_state(counting_only, assume_enough_output,
		         p_output_size, p_output, p_state, &dump_state_err)) {
			return dump_state_err;
		}
	}

	// just a normal ASCII character, then
	if (!assume_enough_output) {
		// must check size
		if (p_output_size[0] < 1) {
			return cnc_mcerr_insufficient_output;
		}
		p_output_size[0] -= 1;
	}
	if (!counting_only) {
		// write out data
		p_output[0][0] = (ztd_char32_t)code_unit;
		p_output[0] += 1;
	}
	// advance the input
	p_input[0] += 1;
	p_input_size[0] -= 1;
	return cnc_mcerr_ok;
}

extern inline cnc_mcerr c32_to_my(size_t* p_output_size, char** p_output,
     size_t* p_input_size, const ztd_char32_t** p_input, my_state* p_state) {
	// Converts from a UTF-32 encoded string to "my".

	// create a default state if we need it
	my_state automatic_default = { 0 };
	if (p_state == NULL) {
		p_state = &automatic_default;
	}

	if (p_input == NULL || p_input[0] == NULL) {
		// input is null; zero the state and leave
		*p_state = (my_state) { 0 };
		return cnc_mcerr_ok;
	}

	// if the input is null, then the size must ALSO be a null pointer,
	// or else this is a constraint violation.
	assert(p_input_size);

	// helper variables to determine whether we should be writing to the output
	// values at all or updating the output count
	const bool counting_only        = (p_output == NULL || p_output[0] == NULL);
	const bool assume_enough_output = (p_output_size == NULL);
	bool input_read                 = false;

	if (p_state->accumulation != '\0') {
	accumulation_writeout:;
		// we have extra writeouts to do based on what we got!
		char unused_buffer[MC_MAX_MY];
		char* output       = counting_only ? &unused_buffer[0] : p_output[0];
		size_t output_size = assume_enough_output ? MC_MAX_MY : p_output_size[0];
		for (; p_state->accumulation_count > 0; --p_state->accumulation_count) {
			if (output_size < 1) {
				if (input_read) {
					p_state->accumulation       = '\0';
					p_state->accumulation_count = 0;
				}
				return cnc_mcerr_insufficient_output;
			}
			output_size -= 1;
			output[0] = (char)p_state->accumulation;
			output += 1;
		}
		// only update the parameters if we have
		// reached the end and successfully serialized everything!
		if (input_read) {
			p_input_size[0] -= 1;
			p_input[0] += 1;
		}
		if (!assume_enough_output) {
			p_output[0] = output;
		}
		if (!counting_only) {
			p_output_size[0] = output_size;
		}
		p_state->accumulation       = '\0';
		p_state->accumulation_count = 0;
		return cnc_mcerr_ok;
	}

	if (p_input_size[0] < 1) {
		// empty input, check if there's state left
		return cnc_mcerr_ok;
	}

	const ztd_char32_t code_point = p_input[0][0];
	switch (code_point) {
	case U'\u2026':
		// set up triple-dot writeout
		p_state->accumulation       = '.';
		p_state->accumulation_count = 3;
		input_read                  = true;
		goto accumulation_writeout;
	case U'\u2057':
		p_state->accumulation       = '\'';
		p_state->accumulation_count = 4;
		input_read                  = true;
		goto accumulation_writeout;
	case U'\u2034':
	case U'\u2033':
		// set up one or more apostrophe for (single, double, triple) prime
		// writeout
		p_state->accumulation       = '\'';
		p_state->accumulation_count = (unsigned char)(code_point - U'\u2031');
		input_read                  = true;
		goto accumulation_writeout;
	default:
		if (!p_state->assume_valid_input) {
			// if it's a non-ascii code point, error
			if (code_point > U'\x7F') {
				return cnc_mcerr_invalid_sequence;
			}
		}
		break;
	}
	// otherwise, we are just writing out some normal, wholesome ASCII
	if (!assume_enough_output) {
		if (p_output_size[0] < 1) {
			return cnc_mcerr_insufficient_output;
		}
		p_output_size[0] -= 1;
	}
	if (!counting_only) {
		p_output[0][0] = (char)code_point;
		p_output[0] += 1;
	}
	p_input_size[0] -= 1;
	p_input[0] += 1;

	return cnc_mcerr_ok;
}

extern inline cnc_mcerr my_str_to_c32_str(size_t* p_output_size,
     ztd_char32_t** p_output, size_t* p_input_size, const char** p_input,
     my_state* p_state) {
	my_state automatic_default = { 0 };
	if (p_state == NULL) {
		p_state = &automatic_default;
	}
	if (p_state == NULL) {
		automatic_default = (my_state) { 0 };
		p_state           = &automatic_default;
	}
	if (p_input == NULL || p_input[0] == NULL) {
		return my_to_c32(
		     p_output_size, p_output, p_input_size, p_input, p_state);
	}
	for (;;) {
		cnc_mcerr err = my_to_c32(
		     p_output_size, p_output, p_input_size, p_input, p_state);
		if (err != cnc_mcerr_ok) {
			return err;
		}
		if (p_input_size[0] > 0) {
			continue;
		}
		bool state_finished = my_state_is_complete(p_state);
		if (!state_finished) {
			continue;
		}
		return err;
	}
}

extern inline cnc_mcerr c32_str_to_my_str(size_t* p_output_size, char** p_output,
     size_t* p_input_size, const ztd_char32_t** p_input, my_state* p_state) {
	my_state automatic_default = { 0 };
	if (p_state == NULL) {
		p_state = &automatic_default;
	}
	if (p_state == NULL) {
		automatic_default = (my_state) { 0 };
		p_state           = &automatic_default;
	}
	if (p_input == NULL || p_input[0] == NULL) {
		return c32_to_my(
		     p_output_size, p_output, p_input_size, p_input, p_state);
	}
	for (;;) {
		cnc_mcerr err = c32_to_my(
		     p_output_size, p_output, p_input_size, p_input, p_state);
		if (err != cnc_mcerr_ok) {
			return err;
		}
		if (p_input_size[0] > 0) {
			continue;
		}
		bool state_finished = my_state_is_complete(p_state);
		if (!state_finished) {
			continue;
		}
		return err;
	}
}

#endif
