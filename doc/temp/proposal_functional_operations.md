Functional style API
=====================

In order to better express data transformations, there is a standardized way of doing operations, which might have differing implementations in differing contexts!

Operations:
    1. .map() use a predicate to convert a data set into a data set of the same features, but a mapped content, like x.map(y => y * 2), to duplicate all elements.
    2. .select() use a predicate to get N elements out of M elements
    3. .zip() use a predicate to take elements from two collections with a transformation or no predicate, {x, y}.zip()
    4. .reduce(), combine elements into a final value, accumulation: x.reduce(acc, n => )