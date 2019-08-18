This language has zero tolerance for unnecessary verbosity. It is stripped down of all unnecessary complexities just so the lexical symbols and grammar are free to use in more interesting scenarios.

For example, in global scope of a file, anything that is not a top level statement like import, namespace, function or using, is automatically a "thing".

All "things" are based on meta entities, and are usually a collection of multiple meta entities or traits. Instead of the grammar having special meaning for things like class or primitives, it is the trait system which determines what compiles and what doesn't.

thing: class 
{

}

Or..

thing: rz::view
{

}

Where thing just becomes the set of traits that rz::view has, allowing to omit class, which rz::view already carries over!!

## No include guards!!

## No unnecessary includes and imports kept to a minimum (can be done once in module spec)

## The entire class is defined in a single file, except for partials