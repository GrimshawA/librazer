DESTRUCTURING INVOCATION
========================

A common thing to do is to want to join a list of strings, something like:

x: string[]
x.join(", ");

But what happens when the strings are contained in some other struct?

b: struct {
	x: string
}
x: b[]

How do we join b.x for all b just the same?

1. This is the first option, join using ranges and predicates to select the strings

join (b.full(), b => b.x)

2. Maybe with selection queries! Technically, b is a domain
and therefore select will be able to iterate all of b, destructuring x in the process. Since select is not filtered, it just a functor, it converts N 'b structs' to N 'string', yielding string[], which can be joined!!

(select x from b).join(", ");