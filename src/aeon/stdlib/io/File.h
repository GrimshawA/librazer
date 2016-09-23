#ifndef AEFILE_H__
#define AEFILE_H__

#include <string>

class AEModule;

void registerFile(AEModule* m, const std::string& name);

class File
{
public:

	bool open(const std::string& filename);

};

#endif // AEFILEINFO_H__