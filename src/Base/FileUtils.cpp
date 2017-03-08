#include <Base/FileUtils.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <sstream>
#include <vector>
#include <iterator>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}

std::string getFileSource(const std::string& filename)
{
	FILE* fp = fopen(filename.c_str(), "r");
	std::string s;
	if (fp)
	{
		while (!feof(fp))
		{
			s += fgetc(fp);
		}
	}
	return s;
}

void rzCreateDir(const std::string& name)
{
#ifdef _WIN32
	CreateDirectoryA(name.c_str(), 0);
#else
#error Unimplemented func
#endif
}

std::string rzGetFileName(const std::string& fullPath)
{
	auto elems = split(fullPath, '/');
	auto frags = split(elems.back(), '.');

	return frags.at(0);
}
