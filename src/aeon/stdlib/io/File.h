#ifndef AEFILE_H__
#define AEFILE_H__

#include <string>

class AEModule;

void registerFile(AEModule* m, const std::string& name);

class File
{
public:

	File();
	~File();

	bool open(const std::string& filename);
	void close();

	int tell();


private:
	FILE* m_file;
};

#endif // AEFILEINFO_H__
