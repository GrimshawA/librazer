At the first level, the language helps doing dependency injection at a class level.

class test
{
	logger l;

	function show()
	{
		l.log("Hello World");
	}
}

logger is a clear dependency, which may not be null by convention. The named construction
helps initialize this to a valid value:

var t = new test {l: myLogger}; 

It is never necessary to write dependency injectors in the constructors, even though if that is done, it will take precedence in handling the dependency.