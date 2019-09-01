#ifndef IR_VALUE_HPP
#define IR_VALUE_HPP

#include <razer/runtime/RzType.h>

#include <vector>
#include <string>
#include <unordered_map>

class IRValueType;

class IRValue
{
public:
    virtual ~IRValue() = default;

    bool isType();

public:
    int offset;
    std::string name;
};

class IRValueType : public IRValue
{
public:

    RzType* type = nullptr;
};

class IRValueFunc : public IRValue
{
public:

    std::string fullName;

    RzFunction* func = nullptr;
};

class IRValueLabel : public IRValue
{
public:

    RzType* type = nullptr;
};

#endif // IR_VALUE_HPP
