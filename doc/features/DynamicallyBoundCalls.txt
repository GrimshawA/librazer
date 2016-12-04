The language supports dynamic typing in the sense there is a built-in variant type which
emulates dynamic typing in a static typed language.

On function calling, the dynamic typing works automatically and seamlessly. 
-> Whenever a function is called on an object, if the object type is static,
known and the said function exists, it will be a static call, equivalent to a C call

-> Whenever the object type is variant or the function couldn't be found, it will perform a dynamic binding.
The dynamic binding will attempt, on runtime, to find the address of the desired function withing the object,
via a special operator. If through the operator, a non null function pointer is returned, the call will be done on that
function pointer.

This adds a lookup to every dynamic call, which may affect performance slightly. Declaring functions as part of the class
will skip this cost entirely.

(In order for this to work, the class needs to adhere to the std::proto trait, which is made
available for assuring some types are static and the compiler fails to build dynamic calls on that type)

The variant type will always support dynamic calls.