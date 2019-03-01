Trait system
=============

Razer does not have the concept of class, as the natural encapsulating unit, instead it has the concept of "entity"
which is any grouping of non-imperative statements.

Traits then, are essentially extensions an entity can adhere to in order to obtain additional semantics, going from extra data and code
or even type system rules.

While classes and structs may be defined as traits, making such entities alike traditional classes, they are not first class citizens
in the grammar or program semantics.

Here are some proposed traits for the standard library:

std::syntax_introspection -> the entire entity may be inspected in terms of the original sources that composed it. With this one could get the AST
    for a function F and inspect all the expressions within it. Within the same module, this is always possible, within different modules compiled separately,
    this must be enabled so that the module binary actually contains this data.

std::reflection -> the entity becomes identifiable at runtime and based on that identification it is possible to inspect and control the object, even when type erased.


----------

Traits can be selected at will when declaring a new code entity type,

ent : trait1 | trait2 | trait3

Some traits are implementable by source code but others are compiler intrinsincs which cannot be replicated.