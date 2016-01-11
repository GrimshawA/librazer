#ifndef aeon_project_h__
#define aeon_project_h__

#include <vector>
#include <array>
#include <stdint.h>

class aeon_project
{
public:
	struct file
	{
		std::string name;
	};

	struct target
	{
		std::string name;
	};

	struct dep
	{
		std::string moduleLoc;
	};

	struct build_setting
	{
		std::string name;
		std::string val;
	};

	std::vector<file>          files;
	std::vector<target>        targets;
	std::vector<build_setting> settings;
	std::string                name;
	std::string                projectFileAbsolutePath;
	std::string                projectVersion;

public:
	void writeProjectFile();
	void readProjectFile(const std::string absolutePath);
};

aeon_project generateFromDirectory(const std::string& dir);

#endif // aeon_project_h__
