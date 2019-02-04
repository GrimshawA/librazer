To make the clear distinction between the language auto data types and the technical data types:

Technical
===

The type behaves according to a technical specification as it would in C++.

Auto
===

The type behaves within certain rules but the compiler is free to change its dynamics in any way necessary, without breaking specification.


For example:

number -> primitive that can be any real number, automatic determination of technical details.

int32 -> technical int of 32 bits

rz::array -> auto array that contains a type T but internal rules are compiler guided
rz::vector -> technical vector with specific memory rules