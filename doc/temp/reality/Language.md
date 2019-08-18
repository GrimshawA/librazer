In the reality model we must create usage patterns for our data that are human friendly,
the technical model this is converted to should handle optimal usage of this information.

This model is usually defined in its own little world of source files in a DSL within the main language usable to define these rules.

-------------------------------------------------

DISCRETE
========

between - state a range for any given property

X.health between 0 and 1000

--

lifespan keyword helps denote lifetimes of objects

bullet lifespan between 0ms -> 10ms // denoting shorted lived objects help immensely creating caching mechanisms that are very cheap and specific.

--

TRANSIENCE
==========

Some objects are temporary by nature and just won't be persistent in memory. There is a big distinction of extremely short lived objects and transient objects. Transient objects are not going to outlive a event loop being triggered, while short lived objects will still persist in memory over a span of time, making them continuous objects.

Transience is applied at code generation when the event is sourced in a way we know the object won't need to be persisted in any way or because the user stated the object should be transient.

Transient objects can be reasoned about as being inserted somewhere in a well planned call stack and living only as much as needed.

Great example for this is a order in a trading platform which was just sourced from a public protocol into our domain entity and logic runs as a sequence of the birth of this entity.

TEMPORAL
========

Time based requirements, either time or calendar days, ranges, patterns anything, or a conjunction of any elemental operations like:

A used at 200% within 5am to 7am AND within 15th to 30th of August.

REQUIREMENT PROPAGATION
=============

The term still needs work but this refers to stating about a given entity what is a requirement that should extend from it.

For example we have a transient order and we mark it as low latency entity, which causes all the code referring to this entity be hyper costly when weighting it, making all the call stack related to that entity aggressively optimized. 

This mechanism is just a way to emphatize how central certain entities are to performance artificially. This is because even a scenario with equivalent requirements of two domains, we may want to give more processing power to one of the problems at will.

CONDITIONAL REQUIREMENTS
======================

Very special requirements that are only active when a certain condition is true like Author.name == "Arthur". This, as for example temporal requirements generate competing semantical analysis for the problems related to Author;

If the SEMA decides there should be two entire domain implementations for this author versus the remaining authors, it will do so, based on how disparate the costs are for running data of that Author versus the others.