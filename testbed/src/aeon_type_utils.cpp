#include "aeon_type_utils.h"
#include <AEON/Runtime/AEType.h>

void printTypeDetails(AEType* type)
{
	printf("typename: %s\n", type->m_name.c_str());
}