#ifndef SYMBOLTYPENAME_H__
#define SYMBOLTYPENAME_H__

#include <string>
#include <vector>

class SymbolTypename
{
public:

    void addPart(const std::string& part);
    std::string str();

    void parse(const std::string& fullString);

private:
    std::string m_fullString;
    std::vector<std::string> m_parts;
};

#endif
