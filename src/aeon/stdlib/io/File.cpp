#include "File.h"

#include <Rzr/RzModule.h>

void registerFile(RzModule* m, const std::string& name)
{
	m->registerType(name, sizeof(aiStd::File));
	m->registerMethod(name, "void open(string)", [](AEGeneric g){
		std::string str = g.unpack_string();
		printf("%s\n", str.c_str());
	});
	m->registerMethod(name, "void close()", [](AEGeneric g){

	});
	m->registerMethod(name, "int tell()", [](AEGeneric g){

	});
}

namespace aiStd{

	File::File()
		: m_file(nullptr)
	{

	}

	File::~File()
	{
		if (m_file)
			fclose(m_file);
	}

	bool File::open(const std::string& filename)
	{
		if (m_file)
			fclose(m_file);

		m_file = fopen(filename.c_str(), "rb");

		return m_file != nullptr;
	}

	void File::close()
	{
		if (m_file)
		{
			fclose(m_file);
			m_file = nullptr;
		}
	}

	int File::tell()
	{
		return m_file ? ftell(m_file) : -1;
	}

}