#ifndef IR_HPP
#define IR_HPP

#include <vector>
#include <string>

namespace IR
{
    enum Type {
        ASSIGN,
        ALLOCATE
    };
}

class IRModule
{

};


class IRValue
{
public:
    int offset;
};

class IRInstruction
{
public:
    IR::Type type;

    virtual std::string prettyString() = 0;
};

class IRInstructionDestructure : public IRInstruction
{
public:
    std::string prettyString() override {
        return "GetElementPtr";
    }
};

class IRInstructionBinaryOp : public IRInstruction
{
public:
    std::string prettyString() override {
        return "A = B";
    }

    IRValue* lhs = nullptr;
    IRValue* rhs = nullptr;
};

class IRFunction
{
public:
    std::vector<IRInstruction*> instructions;
};

class IRBuilder
{
public:

    void beginBlock();
    void endBlock();
    void Assign() {}

public:

    IRValue* getValue(const std::string& name);

    void Dummy()
    {
        func.instructions.push_back(new IRInstructionDestructure());
    }

    void dumpToFile(const std::string& filename);

public:
    IRFunction func;
};

#endif // IR_HPP
