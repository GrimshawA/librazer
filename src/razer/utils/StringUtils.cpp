#include <razer/utils/StringUtils.h>

void myReplace(std::string& str, const std::string& oldStr, const std::string& newStr) {
    std::string::size_type pos = 0u;
    while ((pos = str.find(oldStr, pos)) != std::string::npos) {
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}

void EscapeString(std::string& in) {
    myReplace(in, "\\n", "\n");
}
