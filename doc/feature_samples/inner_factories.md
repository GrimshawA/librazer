Inner factories are a way of tying instantiation to a certain object with a well known property binding, so that it is easy to track them.

class test
{
	int32 apple_count = 5;

	list: factory => apple_count -> apple
	{

	}
}

Which instantiates 5 apple subobjects, indexable from "list".