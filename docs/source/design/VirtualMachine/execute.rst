Execute Function
==============================

What does execute() do?
------------------------------
The execute function handles instruction execution.
This function hosts the instruction loop, and redirects the instruction on line to the appropriate execution case of the opcode.

The dispatch mechanism uses switch-case, which the cases are located in ``virtual-machine/src/partials/executor_cases.h``.

Execution Flow
-------------------------------
- The VirtualMachine class is instantiated and loaded with the bytecode instructions.
- Starting from the first instruction, instruction opcodes are dispatched to the appropriate executor case.
- The executor case then performs the opcode operation, where multiple safety checks are done.


Why use std::variant and std::visit inside the executor cases in the interpreter? 
----------------------------------------------------------------------------------
These are used to maintain a maximum balance of type-safety, usability, and performance.

As cppreference puts it: “std::variant represents a type-safe union.” [0]_. I utilize this type safety feature through multiple implementations in my project. 
Main implementations include::

	1. std::visit
	2. std::get through switch-case

According to cppreference.com [1]_:
	
    “Implementations may also generate a switch statement with n branches for std::visit (e.g., the MSVC STL implementation uses a switch statement when n is not greater than 256).”.

Alternatively, the compiler may:
    
    “Generate a table equivalent to an (possibly multidimensional) array of n function pointers for every specialization of std::visit, … similar to the implementation of virtual functions.”.

Therefore, std::visit’s which the compiler does not force switch-case optimization are identified and are replaced with switch-case statements that utilize std::get to ensure that a similar switch-case optimization is applied. 


In such case, of course, the implementation is abstracted into a visitor-like implementation. 

The files that contain these optimizations will also explicitly make note of this and reference the benchmark and/or binary analysis results and this page.

It is also important to note the std::get’s functionality and performance, which cppreference states that it is:
    
    “[either an] index-based value accessor: ... [which] returns a reference to the value stored in `v`… [or] type-based value accessor: … [which] returns a reference to the value stored in `v`.” [2]_.

For more context, you can check this `reddit post <https://www.reddit.com/r/cpp_questions/comments/1nl0r13/stdvisit_vs_switchcase_for_interpreter_performance/>`_.

References
------------------------------
.. [0] https://en.cppreference.com/w/cpp/utility/variant.html, Accessed on Sep 18, 2025
.. [1]  https://en.cppreference.com/w/cpp/utility/variant/visit2, Accessed on Sep 18, 2025
.. [2] https://en.cppreference.com/w/cpp/utility/variant/get, Accessed on Sep 18, 2025