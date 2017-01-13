#include "aeon_type_utils.h"
#include <RazerRuntime/RzType.h>

void printTypeDetails(RzType* type)
{
	printf("typename: %s\n", type->m_name.c_str());
}