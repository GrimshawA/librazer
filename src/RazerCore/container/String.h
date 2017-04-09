#ifndef RZSTRING_H__
#define RZSTRING_H__

#include <string>

class RzModule;

class RzString
{
public:
    RzString();

    int size();

private:
    std::string m_str;
};

void registerString(RzModule* mod);

#endif
