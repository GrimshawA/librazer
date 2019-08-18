Static Type Introspection
=========================

It is possible to obtain all information about any code entity by inspecting it, without need for any kind of reflection usage.

All introspection information is a compile time expression which means computations done with them, including code generation, may run at compile time as well.

Regardless of any shorthand notation, all compiler built ins are exposed as full verbs within the standard library, so:

std::type_info<my_type>::methods -> std::array<std::method>
std::type_info<my_type>::properties -> std::array<std::property>

Which can be inspected at compile time and runtime without issue.