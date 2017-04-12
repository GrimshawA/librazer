#ifndef RZSTRING_H__
#define RZSTRING_H__

#include <string>

class RzModule;

class RzString
{
public:
    RzString();

    void set(const std::string& content);

    std::string toStdString();

    void print();

    int size();

private:
    std::string m_str;
};

void registerString(RzModule* mod);

#endif
