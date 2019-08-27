#include "ir_value.hpp"
#include <iostream>

bool IRValue::isType()
{
    return dynamic_cast<IRValueType*>(this);
}
