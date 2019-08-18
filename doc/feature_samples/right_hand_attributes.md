As in the RightHandedAttributes.aeon technical preview source, we can observe the language promotes
right hand sided attributes to function declaration on classes, to promote readability and make it as little
verbose as possible. It also promotes alignment and code organization, as opposed to showing return type first in the function declaration.


class RightHandedAttributes
{
public:
	setPosition() -> Vector3
	setSize() -> Vector2
	setSomethingElse() -> auto

	position() [InlineMetadata=Stuff]
	size() override
	x()
	y()
	z()

	evaluate()
	test()
	do()

	[ContextMetaData=Nice Way to define meta data for the function]
	runScript()
}

// Anywhere metadata
[@RightHandedAttributes::evaluate AdditionalProperty=Lel]

function Hello() {
	
	//Runtime metadata, adds metadata on runtime for that type
	[@RightHandedAttributes Property=Value]
}