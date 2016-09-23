#include "File.h"

#include <AEON/Runtime/AEModule.h>

void registerFile(AEModule* m, const std::string& name)
{
	m->exportType(name, sizeof(File));
	m->exportMethod(name, "void open(int32)", [](AEGeneric g){
		std::string str = g.unpack_string();
		printf("%s\n", str.c_str());
	});
}

bool File::open(const std::string& filename)
{
	return true;
}