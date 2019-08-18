All references are never null, by default, so that:

obj: MyClass // obj will never be null!

In order to obtain a nullable obj:

obj: MyClass? // Can be null!

Which in terms of type system, the ? makes the type a Maybe<MyClass>
which is a see-through type that contains either the null or the
reference.



This also makes it possible for dependent typing to work on top:
Depending on the context being analyzed, it may be simple to type the Maybe<MyClass> into something else, during static analysis.

Sample:
main: class {
	obj: MyClass?
}

When used from here:

function f() {
	new main(); // we don't provide any MyClass
}

The static analyzer can safely convert the Maybe<MyClass> into a Never<MyClass>, which is usable to validate usage of obj in many places, making the compiler enter in error state easily.

Or here:
function f() {
	new main() {
		obj: new MyClass()
	}
}

Here, its obvious for the compiler the dependency will be satisfied,
so the type can be converted into a Single<MyClass>, which means the ref is always correct.

Marking the reference as MyClass? mutable, will break the dependent typing rules, as the reference may then change at runtime!