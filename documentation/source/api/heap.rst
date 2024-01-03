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

Conversion Heap and Functions
=============================

For the type-erased, registry-based conversions, a heap is used to perform all allocations to ensure the end-user is in control of how much memory is ultimately used, even if they do not have full control over the type-erased data structures. To this end, the :cpp:class:`cnc_conversion_heap` is used, and it is described below in the convenience functions which will pass through the designated ``user_data`` and similar to the specified heap's given functionality.

The default heap uses ``malloc``, ``free``, and ``realloc`` for its implementation, ignoring related alignment and user data parameters. It also performs no attempted shrinking or expanding of any given allocation, single ignoring all parameters and following the specification for performing no action.



Function Calls
--------------

.. doxygenfunction:: cnc_heap_allocate

.. doxygenfunction:: cnc_heap_deallocate

.. doxygenfunction:: cnc_heap_reallocate_allocation

.. doxygenfunction:: cnc_heap_expand_allocation

.. doxygenfunction:: cnc_heap_shrink_allocation



Heap Object
-----------

.. doxygenstruct:: cnc_conversion_heap
	:members:



Types
-----

.. doxygentypedef:: cnc_heap_allocate_function_ptr

.. doxygentypedef:: cnc_heap_deallocate_function_ptr

.. doxygentypedef:: cnc_heap_reallocate_allocation_function_ptr

.. doxygentypedef:: cnc_heap_expand_allocation_function_ptr

.. doxygentypedef:: cnc_heap_shrink_allocation_function_ptr
