Reflectible trait
=================

By default, entities in razer are not dynamically reflectible and only inspectible at compile time.
Though, the official reflection API exists to access runtime information for types we choose to make available.

By adhering to the std::reflectible trait, such as in:

myType: class, std::reflectible

We mark myType as being reflectible and anyone with a Object ref to such an object will be able to inspect its contents.

auto typeInfo = std::reflection::get("myType");
typeInfo.create();

In order to avoid having to do this with the entire set of classes we can pattern match it or just add everything at the module level:

using trait std::reflectible on *;