.. ============================================================================
..
.. ztd.cuneicode
.. Copyright © 2022-2023 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
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

Encodings
=========

This is the list of encodings which have named functions available in cuneicode.


.. =============================================================================
..
.. ztd.text
.. Copyright © 2022-2023 JeanHeyd "ThePhD" Meneide and Shepherd's Oasis, LLC
.. Contact: opensource@soasis.org
..
.. Commercial License Usage
.. Licensees holding valid commercial ztd.text licenses may use this file in
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
.. =============================================================================>

Available Encodings
===================

All of the encodings available here are identical to the ones available in `ztd.text, and the documentation there gives a long list <https://ztdtext.readthedocs.io/en/latest/encodings.html>`_. The only thing that is different is that this uses function calls and, perhaps, state objects to achieve a similar effect. The ones with interesting consequences for the API shape are linked below; the rest follow a more or less identical pattern to preexisting functionality.



Special, Stateful APIs
----------------------

.. toctree::
	:maxdepth: 1
	
	encodings/punycode



Single and Multibyte Encoding APIs
----------------------------------

These APIs use :doc:`the typical cnc_mcstate_t </api/mcstate_t>` state (and may not do anything to it internally in the functions, if the API is stateless). They do not have any higher-level or special functionality associated with them.

.. toctree::
	:maxdepth: 1
	
	encodings/multi_byte_encodings
	encodings/single_byte_encodings
