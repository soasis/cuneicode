.. ============================================================================
..
.. ztd.cuneicode
.. Copyright © JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
.. Contact: opensource@soasis.org
..
.. Commercial License Usage
.. Licensees holding valid commercial ztd.cuneicode licenses may use this file in
.. accordance with the commercial license agreement provided with the
.. Software or, alternatively, in accordance with the terms contained in
.. a written agreement between you and Shepherd's Oasis, LLC.
.. For licensing terms and conditions see your agreement. For
.. further information contact opensource@soasis.org.
..
.. Apache License Version 2 Usage
.. Alternatively, this file may be used under the terms of Apache License
.. Version 2.0 (the "License") for non-commercial use; you may not use this
.. file except in compliance with the License. You may obtain a copy of the
.. License at
..
.. https://www.apache.org/licenses/LICENSE-2.0
..
.. Unless required by applicable law or agreed to in writing, software
.. distributed under the License is distributed on an "AS IS" BASIS,
.. WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
.. See the License for the specific language governing permissions and
.. limitations under the License.
..
.. ========================================================================= ..

State
=====

Occasionally, state objects are passed to the routines that perform conversions. These can do things such as hold onto shift state, have special flags for special processing (e.g., for :cpp:func:`assuming input for a given function is valid <cnc_mcstate_set_assume_valid>`). In general, the state parameter comes last and is a pointer to a state the user creates (or, if using the :doc:`typed conversion functions </api/typed conversions>`, may be created for you and used.) The function therefore usually ends up of the form:

.. code-block:: cpp

	#include <uchar.h>
	
	typedef struct my_state {
		bool assume_valid_input;
		char accumulation[2];
	} my_state;

	cnc_mcerr my_single_conversion_to_utf32(size_t* p_output_size,
		const char32_t** p_output,
		size_t* p_input_size,
		const char** p_input,
		my_state* p_state);

As described by the :doc:`indivisible unit of work </design/indivisible>`, reading input can change the state internally, even accumulate data in the state while outputting 0 characters. Because state may influence whether or not a conversion is complete (e.g., it has accumulated data inside of it and must empty it out), complex state objects should add a visible inspection function to check if the state is "complete" (has nothing left to output **and** is awaiting no further characters):

.. code-block:: cpp

	#include <uchar.h>
	
	typedef struct my_conversion_state {
		char accumulation[2];
		unsigned char accumulation_count;
		bool assume_valid_input;
	} my_conversion_state;

	bool my_state_is_complete(const my_conversion_state* p_state);

	cnc_mcerr my_single_conversion_to_utf32(size_t* p_output_size,
		const char32_t** p_output,
		size_t* p_input_size,
		const char** p_input,
		my_conversion_state* p_state);

The :cpp:union:`cnc_mcstate_t` type has a similar function named :cpp:func:`cnc_mcstate_is_complete`, used for this purpose. As a completely made up example for a complete made up encoding,

.. literalinclude:: ../../../examples/documentation/design/source/single_with_state_loop_example.c
	:language: cpp
	:linenos:
	:start-after: // ========================================================================= //
	:emphasize-lines: 29-32
