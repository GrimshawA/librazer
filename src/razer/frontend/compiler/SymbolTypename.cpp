#include <razer/frontend/compiler/SymbolTypename.h>

void SymbolTypename::addPart(const std::string& part)
{
    if (m_fullString.empty())
        m_fullString += part;
    else
        m_fullString += "." + part;

    m_parts.push_back(part);
}

std::string SymbolTypename::str()
{
    return m_fullString;
}

void SymbolTypename::parse(const std::string& fullString)
{
    std::string cpy = fullString;
    while(cpy.find_first_of('.') != cpy.npos) {
        std::string part = cpy.substr(0, cpy.find_first_of('.'));
        cpy.erase(0, cpy.find_first_of('.')+1);

        addPart(part);
    }

    addPart(cpy);
}
