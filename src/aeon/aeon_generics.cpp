#include "aeon_generics.h"
#include "aeon_vm.h"

float atom_generic::getArgFloat(uint32_t index)
{
	return args[index]._fp;
}

std::string atom_generic::getarg_string(uint32_t index)
{
	uint32_t strIndex = args[index]._u32;
	return vm->get_current_mod()->getStringFromPool(strIndex);
}