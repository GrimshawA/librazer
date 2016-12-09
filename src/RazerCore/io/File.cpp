#include "File.h"

#include <Rzr/RzModule.h>

void registerFile(RzModule* m, const std::string& name)
{
	m->registerType(name, sizeof(Rz::File));
	m->registerTypeConstructor(name, [](void* memory, RzVirtualMachine* vm)
	{
		new (memory) Rz::File();
	});

	m->registerMethod(name, "void open(int32, int32)", [](AEGeneric g){
		Rz::File* obj = (Rz::File*)g.unpack_ptr();
		std::string fileName = g.unpack_string();
		std::string openMode = g.unpack_string();
		
		obj->open(fileName, openMode);
		
		/*std::string str = g.unpack_string();
		printf("%s\n", str.c_str());*/
	});
	m->registerMethod(name, "void close()", [](AEGeneric g){
		Rz::File* obj = (Rz::File*)g.unpack_ptr();
		obj->close();
	});
	m->registerMethod(name, "void write(int32)", [](AEGeneric g){
		Rz::File* obj = (Rz::File*)g.unpack_ptr();
		std::string buff = g.unpack_string();
		obj->write(buff);
	});
	m->registerMethod(name, "string getline()", [](AEGeneric g){
		Rz::File* obj = (Rz::File*)g.unpack_ptr();	
		std::string str =  obj->getline();
		g.pack_string(str);

		printf("getline was called and got %s\n", str.c_str());
	});
	m->registerMethod(name, "int tell()", [](AEGeneric g){
		Rz::File* obj = (Rz::File*)g.unpack_ptr();
		obj->tell();
	});
}

namespace Rz{

	File::File()
		: m_file(nullptr)
	{
		printf("FILE INSTANCED %x\n", this);
	}

	File::~File()
	{
		if (m_file)
			fclose(m_file);
	}

	bool File::open(const std::string& filename, const std::string& openMode)
	{
		if (m_file)
			fclose(m_file);

		m_file = fopen(filename.c_str(), openMode.c_str());

		printf("OPENING FILE %s %s\n", filename.c_str(), openMode.c_str());

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

	std::string File::getline()
	{
		if (!m_file)
			return std::string();

		std::string str;
		char c = fgetc(m_file); 
		str += c;
		while (c != '\n'){
			str += c;
			c = fgetc(m_file);
		}
		return str;
	}

	void File::write(const std::string& buffer)
	{
		if (!m_file)
			return;

		printf("Writing data to file %s\n", buffer.c_str());

		fwrite(buffer.c_str(), buffer.size(), 1, m_file);
	}

	int File::tell()
	{
		printf("WAZAAAAAAAAAAAAAAAAAa\n");
		return m_file ? ftell(m_file) : -1;
	}

}