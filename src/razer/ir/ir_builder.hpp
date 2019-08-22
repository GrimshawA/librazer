#ifndef IRBUILDER_HPP_
#define IRBUILDER_HPP_

#include <razer/ir/ir.hpp>

class IRBuilder
{
public:
    IRBuilder(IRContext& ctx);
    ~IRBuilder();

    void beginBlock();
    void endBlock();
    void call();
    IRValue* binaryOp(std::string op, IRValue* lhs, IRValue* rhs);
    IRValue* newObject();
    IRValue* newType(const std::string& name);

    void Assign() {}

    IRValue* makeValue();
    IRValue* makeTempValue();

public:

    IRValue* getValue(const std::string& name);

public:
    IRContext& ctx;
    IRFunction func;
    int tempId = 1;
};


#endif // IRBUILDER_HPP_
