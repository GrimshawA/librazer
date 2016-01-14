#include "aeon_type_utils.h"
#include <aeon_type.h>

void printTypeDetails(aeon_type* type)
{
	printf("typename: %s\n", type->m_name.c_str());
}