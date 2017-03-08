#ifndef FILEUTILS_H__
#define FILEUTILS_H__

#include <string>

extern std::string getFileSource(const std::string& filename);

extern void rzCreateDir(const std::string& name);

std::string rzGetFileName(const std::string& fullPath);

#endif
