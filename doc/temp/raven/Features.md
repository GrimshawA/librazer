
# Modules
All code of a given class is put together within the same file without headers and source files. A set of source files composes a module. All things know each other automatically from within a module. Modules have an optional definition block which allows to do certain things.
For example, in the module definition, one can which namespace will be encapsulating all module things when accessed from the outside, without the need for any of the source files to specify it.

# Non-nullable by default
When writing a function that takes for example Car x as a parameter, its implicit that x can never be null and will always be a valid Car.
To opt-in allowing null to be passed, then Car? must be written instead, which ideally would never ever happen.

# Bindings
Within class context, the colon : sign will allow to create bindings to constants or other variables, as in, a: b + 10. This relationship will always remain true. Setting b from anywhere will in fact update a as well.

# Templates just like C++
Containers are implemented using a similar templated approach, so the target module using the container may have to instantiate the final code for the type being used.