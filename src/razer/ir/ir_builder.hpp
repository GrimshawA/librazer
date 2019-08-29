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
    IRValue* createCall(IRValue* func, const std::vector<IRValue*>& args);
    IRValue* createLocalAlloc();

    // Allocate N bytes in heap, returning value is a pointer to it
    IRValue* createHeapAlloc(std::size_t bytes);
    IRValue* createBinaryOp(std::string op, IRValue* lhs, IRValue* rhs);
    IRValue* createDestructure(RzType* ty, int fieldIndex, IRValue* base);
    IRValue* newObject(IRValue* typeValue);
    IRValue* createReturn();
    IRValue* createLabelStmt();

    IRValue* makeValue();
    IRValue* makeTempValue();
    IRValue* makeFuncValue(RzFunction* func);
    IRValue* makeFuncValue(const std::string& fullName);
    IRValue* makeNativeFuncValue(RzType::MethodInfo* method);

    IR::Type getLastStmtType();

public:

    IRValue* getValue(const std::string& name);

public:
    IRContext& ctx;
    IRFunction func;
    int tempId = 1;
};


#endif // IRBUILDER_HPP_
