The language comes with the capability to introspect itself, so that it becomes possible to browse the AST of any node within the program itself, but also by parsing a source file on runtime.

std::st:: is the namespace for the syntax tree utilities.

For example, this would allow to write a shader within the language itself like:

function vertex_main()
{
	return vec4(1.0, 1.0, 1.0, 0.0);
}

Then, we could build our graphics lib on top of this fact, so that:

auto vs = loadVertexShader(vertex_main);

.. where the shader is compiled from GLSL, for example, by traversing the AST for that function. At the type system level, if a function argument is std::st::function,
the actual method or function will be implicitly converted to its AST representation.

Otherwise we could just load a source file:

auto sources = std::st::parse(mysources);