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

Quick 'n' Dirty Tutorial
========================



Setup
-----

Use of this library is officially supported through the use of `CMake <https://cmake.org/>`_. Getting an updated CMake is difficult on non-Windows machines, especially if they come from your system's package manager distribution which tends to be several (dozen?) minor revisions out of date, or an entire major revision behind on CMake. To get a very close to up-to-date CMake, Python maintains an version that works across all systems. You can get it (and the ninja build system) by using the following command in your favorite command line application (assuming Python is already installed):

.. code-block:: sh
	:linenos:

	python -m pip install --user --update cmake ninja

If you depend on calling these executables using shorthand and not their full path, make sure that the Python "downloaded binaries" folder is contained with the ``PATH`` environment variable. Usually this is already done, but if you have trouble invoking ``cmake --version`` on your typical command line, please see the `Python pip install documentation for more details <https://pip.pypa.io/en/stable/cli/pip_install/>`_ for more information, in particular about the ``--user`` `option <https://pip.pypa.io/en/stable/cli/pip_install/#cmdoption-user>`_.

If you do not have Python or CMake/ninja, you must get a recent enough version `directly from CMake <https://cmake.org/download/>`_ and build/install it and have a suitable build system around for CMake to pick up on (MSBuild from installing `Visual Studio <https://visualstudio.microsoft.com/downloads>`_, make in most GNU distributions / MinGW on Windows on your PATH environment variable, and/or a personal installation of `ninja <https://ninja-build.org/>`_).



Using CMake
-----------

Here's a sample of the ``CMakeLists.txt`` to create a new project and pull in ztd.text in the simplest possible way:

.. literalinclude:: ../../examples/documentation/quick/setup/CMakeLists.txt
	:language: cmake
	:linenos:
	:start-after: # === snippet-project-declaration-start
	:end-before: # === snippet-project-declaration-end


This will automatically download and set up all the dependencies ztd.cuneicode needs (in this case, simply ztd.cmake, ztd.platform, and `ztd.idk <https://ztdidk.readthedocs.io/en/latest/>`_ ). You can override how ztd.cuneicode gets these dependencies using the standard FetchContent described in the `CMake FetchContent Documentation <https://cmake.org/cmake/help/latest/module/FetchContent.html#command:fetchcontent_declare>`_. When done configuring, simply use CMake's ``target_link_libraries(…)`` to add it to the code:

.. literalinclude:: ../../examples/documentation/quick/setup/CMakeLists.txt
	:language: cmake
	:linenos:
	:start-after: # === snippet-library-start
	:end-before: # === snippet-library-end

Once you have everything configured and set up the way you like, you can then use ztd.cuneicode in your code, as shown below:

.. literalinclude:: ../../examples/documentation/quick/setup/source/main.c
	:language: cpp
	:linenos:
	:start-after: // ============================================================================ //

Let's get started by digging into some examples!

.. note::
	
	If you would like to see more examples and additional changes besides what is covered below, please do feel free to make requests for them `here <https://github.com/soasis/cuneicode/issues>`_! This is not a very full-on tutorial and there is a lot of functionality that, still, needs explanation!



Simple Conversions
------------------

Simple conversions are provided for UTF-8, UTF-16, UTF-32, :term:`execution encoding`, and :term:`wide execution encoding`. They allow an end-user to use bit-based types.

To convert from UTF-16 to UTF-8, use the appropriately ``c8`` and ``c16``-marked free functions in the library:

.. literalinclude:: ../../examples/documentation/quick/basic/source/utf16_to_utf8.c
	:language: c
	:start-after: // ========================================================================= //
	:linenos:

We use raw ``printf`` to print the UTF-8 text. It may not appear correctly on a terminal whose encoding which is not UTF-8, which may be the case for older Microsoft terminals, some Linux kernel configurations, and deliberately misconfigured Mac OSX terminals. There are also some other properties that can be gained from the use of the function:

- the amount of data read (using ``initial_input_size`` - ``input_size``);
- the amount of data written out (using ``initial_output_size`` - ``output_size``);
- a pointer to any extra input after the operation (``p_input``);
- and, a pointer to any extra output that was not written to after the operation (``p_output``).

One can convert from other forms of UTF-8/16/32 encodings, and from the :term:`wide execution encodings <wide execution encoding>`/:term:`execution encoding <execution encoding>` (encodings used by default for ``const char[]`` and ``const wchar_t[]`` strings) using the various different :ref:`prefixed-based <design-naming-encoding.table>` functions.


Counting 
++++++++

More often than not, the exact amount of input and output might not be known before-hand. Therefore, it may be useful to count how many elements of output would be required before allocating exactly that much space to hold the result. In this case, simply passing ``NULL`` for the data output pointer instead of a real pointer, while providing a non-``NULL`` pointer for the output size argument, will give an accurate reading for the amount of space that is necessary: 

.. literalinclude:: ../../examples/documentation/quick/basic/source/count.utf16_to_utf8.c
	:language: c
	:start-after: // ========================================================================= //
	:linenos:

Here, we find out the ``output_size_needed`` by taking the size before the call, then subtracting it by the decremented size from after the function call. Then, after checking for errors, we do the actual conversion with a properly sized buffer that includes a null terminator so the conversion result is suitable for printing to a (UTF-8 capable) terminal. Finally, after completing our task, we free the memory and return a proper error code.


Unbounded Output Writing 
++++++++++++++++++++++++

Sometimes, it is know ahead of time that there is enough space in a given buffer for a given conversion result because the inputs are not at all associated with user input or user-facing anything (e.g., static storage duration string literals with known sizes and elements). If that is the case, then a ``NULL`` value can be passed in for the output size argument, and the function will assume that there is enough space for writing:

.. literalinclude:: ../../examples/documentation/quick/basic/source/count.utf16_to_utf8.c
	:language: c
	:start-after: // ========================================================================= //
	:linenos:

This can be useful for performance-oriented scenarios, where writing without doing any bounds checking may result in deeply improved speed.




Validating
++++++++++

Validation is similar to counting, except that the output size argument is ``NULL``. This effectively allows someone to check if the input is not only valid for that encoding, but also if it can be transcoded to the output assuming enough size.

.. literalinclude:: ../../examples/documentation/quick/basic/source/validate.utf16_to_utf8.c
	:language: c
	:start-after: // ========================================================================= //
	:linenos:

In many instances, simply validating that text can be converted rather than attempting the conversion can provide :term:`a far greater degree of speed using specialized algorithms and instruction sets<simdutf>`.




Registry-Based Conversions
--------------------------

Conversion registries in cuneicode provide a way to obtain potentially runtime-defined encodings. It can be added to and removed from by a user, and all access to data (save for those which are defined to access global state such as the ``char``/``execution`` and ``wchar_t``/``wide execution`` encodings) is referenced straight from the objects created and involved and should involve no global, mutable state. This should enable users to create, use, and pass around registry objects freely without the burden of pre-allocated or statically-shared state, resulting in programs that are easier to reason about. Here is an example of converting between the Windows-1251 encoding and the UTF-8 encoding by passing both names to ```cnc_conv_new(…)```:

.. literalinclude:: ../../examples/documentation/quick/basic/source/registry_conversion.c
	:language: c
	:start-after: // ========================================================================= //
	:linenos:

Care must be taken that, upon allocating one of these types, it is deallocated with care. A large number of additional registry functionality is described in the :doc:`registry design documentation </design/registry>` and the :doc:`registry API documentation </api/registry conversions>`, including an example of registering an encoding which contains its own state and must be stored in a ``cnc_conv*`` handle.

Most importantly in this short example is that there is no direct conversion between Windows-1251 and UTF-8 in the default offerings of cuneicode. Instead, the registry knows how to negotiate a pathway between the registered Windows-1251 encoding (which goes from itself to UTF-32 and back) to UTF-8 (which goes from itself to UTF-32 and back). This automatic handling of indirection is provided by-default and is described in the :doc:`registry design for indirections </design/registry/indirect>`.
