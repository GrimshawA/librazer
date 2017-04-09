#ifndef AEFILE_H__
#define AEFILE_H__

#include <string>

class RzModule;

void registerFile(RzModule* m, const std::string& name);

namespace Rz{

	/**
		\class File
		\brief Provides file manipulation abilities
	*/
	class File
	{
	public:

		File();
		~File();

		bool open(const std::string& filename, const std::string& openMode);
		void close();

		std::string getline();

		void write(const std::string& buffer);

		int tell();

        bool atEnd();


	private:
		FILE* m_file;
	};
};
#endif // AEFILEINFO_H__
