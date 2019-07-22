#include "aeon_type_utils.h"
#include <razer/runtime/RzType.h>
#include <razer/utils/Logger.h>

void printTypeDetails(RzType* type)
{
	RZLOG("typename: %s\n", type->m_name.c_str());
}
