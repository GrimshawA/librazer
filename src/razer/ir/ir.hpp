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
    std::string name;
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

class IRInstructionCall : public IRInstruction
{
public:
    std::string prettyString() override {
        return "Call";
    }
};

class IRInstructionBinaryOp : public IRInstruction
{
public:
    IRInstructionBinaryOp(std::string op, IRValue* lhs, IRValue* rhs, IRValue* result)
    {
        this->lhs = lhs;
        this->rhs = rhs;
        this->result = result;
        this->op = op;
    }

    std::string prettyString() override {
        std::string res;
        res += result->name + " <- ";
        res += op + " " + lhs->name + " " + rhs->name;
        return res;
    }

    std::string op;
    IRValue* lhs = nullptr;
    IRValue* rhs = nullptr;
    IRValue* result = nullptr;
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
    void call();
    IRValue* binaryOp(std::string op, IRValue* lhs, IRValue* rhs);
    void Assign() {}
    IRValue* makeValue();

public:

    IRValue* getValue(const std::string& name);

    void Dummy()
    {
        func.instructions.push_back(new IRInstructionDestructure());
    }

    void dumpToFile(const std::string& filename);

public:
    IRFunction func;
    int tempId = 1;
};

#endif // IR_HPP
