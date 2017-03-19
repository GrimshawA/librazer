#include "aeon_type_utils.h"
#include <RazerRuntime/RzType.h>
#include <Logger.h>

void printTypeDetails(RzType* type)
{
	RZLOG("typename: %s\n", type->m_name.c_str());
}
