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

    void     createStore(IRValue* memory, IRValue* value);
    void     createJumpStmt(IRValue* target);
    IRValue* createCall(const std::vector<IRValue*>& args);
    IRValue* createLocalAlloc();
    IRValue* createHeapAlloc();
    IRValue* createBinaryOp(std::string op, IRValue* lhs, IRValue* rhs);
    IRValue* createDestructure(RzType* ty, int fieldIndex);
    IRValue* newObject(IRValue* typeValue);
    IRValue* createReturn();
    IRValue* createLabelStmt();

    IRValue* makeValue();
    IRValue* makeTempValue();

    IR::Type getLastStmtType();

public:

    IRValue* getValue(const std::string& name);

public:
    IRContext& ctx;
    IRFunction func;
    int tempId = 1;
};


#endif // IRBUILDER_HPP_
