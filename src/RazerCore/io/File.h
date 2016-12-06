#ifndef AEFILE_H__
#define AEFILE_H__

#include <string>

class RzModule;

void registerFile(RzModule* m, const std::string& name);

namespace aiStd{


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

};
#endif // AEFILEINFO_H__
