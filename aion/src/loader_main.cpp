/**
	This program is a launcher of applications generated by the aeon official runtime and compiler.
	Designed specifically for bytecode based builds, which require a host native application to function.
*/

#include "testbed.h"
#include <AEON/AEContext.h>
#include <AEON/VM/AEVm.h>
#include <AEON/Runtime/AEObject.h>
#include <AEON/AEDocument.h>
#include <AEON/AEBuilder.h>

void main(int argc, char** argv)
{
	printf("aion interpreter\n");

	if (argc > 1)
	{
		std::string appPath = argv[1];
		printf("%s was successfully executed.\n", appPath.c_str());
		system("pause");
	}
}