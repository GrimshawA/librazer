
This document informs about the language used within the semantic analyzer, to ensure its unambiguous how to translate to real code.

Control Transfer
=================

Direct - Similar to synchronous but used more specifically to symbolize (within code flow) that something should be called directly and not through ANY form of indirection.

DirectProxy - Similar to the above, the control is transferred synchrounously and predictably but through an obscuring mechanism like a function pointer (Active) or a
delegate (Passive)

Indirect - The control is not passed now and instead is posted somewhere for handling later.


Data Layout
================

Contiguous - Regards data storage, data will live together and be friendly to serial consumption, threaded logic may consume portions of the data independently only if said access is deemed to be valid and depending on actual sizes of the data.

ContiguousPaged - Lends itself well to partitioning contiguous data into different chunks which may or may not live close together in the form of linked list. Also friendly to threading.

Sparse - The data is scattered by lack of knowledge and therefore we can make no predictions and just access it from random locations (ptrs)


Allocation patterns
==========================

Direct - The memory should be mapped for the process NOW using system allocators, we don't know anything about allocation of this data and there is no better heuristic; akin to calling new()

DirectInternal - These allocations must NOT be done directly to system memory, they need to go through a contextual allocator which will have the full control over the allocation of this object. These allocators may be process-wide, type-wide, or actual data domain wide, but thats the scope of another decision maker.

ValueCached - Does not go to any allocator and a stateful cache of objects must be used. Value semantics are associated here, meaning that we will just be dealing with values and not references.

RefCached - Same as above, but we reuse memory addresses.