Templates as a base, are somewhat like C++.

template<T> final
class x
{

}

The final keyword optionally marks the type as not instantiable outside the module, reducing the burden of carrying the AST over for instantion from dependant modules.

