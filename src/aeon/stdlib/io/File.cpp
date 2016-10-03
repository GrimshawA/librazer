#include "File.h"

#include <AEON/AEModule.h>

void registerFile(AEModule* m, const std::string& name)
{
	m->registerType(name, sizeof(File));
	m->registerMethod(name, "void open(int32)", [](AEGeneric g){
		std::string str = g.unpack_string();
		printf("%s\n", str.c_str());
	});
}

bool File::open(const std::string& filename)
{
	return true;
}
