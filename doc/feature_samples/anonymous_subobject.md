Within a class, it is possible to instantiate a sub object of a given type, with an optional name, whose lifetime is directly tied to its parent.

class test
{
	test2
	{
		// Instantiated and allowed to access test data
		// No way to access it from parent scope directly
	}

	named: test2 
	{
		// Identical to above, but the "named" identifier relates to it
	}
}
