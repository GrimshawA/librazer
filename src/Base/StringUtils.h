#ifndef RZSTRINGUTILS_H__
#define RZSTRINGUTILS_H__

#include <string>
#include <locale>

void myReplace(std::string& str, const std::string& oldStr, const std::string& newStr);
void EscapeString(std::string& in);

#endif
