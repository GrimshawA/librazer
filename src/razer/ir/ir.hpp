#ifndef IR_HPP
#define IR_HPP

#include <razer/runtime/RzType.h>
#include <razer/ir/ir_value.hpp>

#include <vector>
#include <string>
#include <unordered_map>

class RzType;
class IRValue;

struct LexicalScope
{
    std::unordered_map<std::string, IRValue*> variables;
};

struct ScopeStack
{
    void pop()
    {
        scopes.pop_back();
    }

    void push()
    {
        scopes.push_back({});
    }

    void set(const std::string& name, IRValue* val)
    {
        scopes.back().variables[name] = val;
    }

    IRValue* locate(const std::string& name)
    {
        for (int i = scopes.size() - 1; i >= 0; --i)
        {
            auto& s = scopes[i];

            if (s.variables.find(name) != s.variables.end())
            {
                return s.variables[name];
            }
        }

        return nullptr;
    }

    std::vector<LexicalScope> scopes;
};


namespace IR
{
    enum Type {
        ASSIGN,
        StackAlloc,
        CALL,
        New,
        Return,
        Jump,
        Store,
        Destructure,
        Label,
        Undefined
    };
}

class IRInstruction
{
public:
    IR::Type type;

    virtual std::string prettyString() = 0;
};

class IRInstructionDestructure : public IRInstruction
{
public:
    explicit IRInstructionDestructure()
    {
        type = IR::Destructure;
    }

    std::string prettyString() override {
        return "getelementptr " + std::to_string(fieldIndex);
    }

    int fieldIndex;
    IRValue* ty = nullptr;
};

class IRInstructionReturn : public IRInstruction
{
public:
    explicit IRInstructionReturn()
    {
        type = IR::Return;
    }

    std::string prettyString() override {
        return "ret";
    }

    IRValue* value = nullptr;
};

class IRInstructionLabel : public IRInstruction
{
public:
	explicit IRInstructionLabel()
	{
		type = IR::Label;
	}

	std::string prettyString() override {
		return "label";
	}

	IRValue* value = nullptr;
};

class IRInstructionCall : public IRInstruction
{
public:
    explicit IRInstructionCall()
    {
        type = IR::CALL;
    }

    std::string prettyString() override {
        std::string str = "x <- call ";
        for (auto& a : args)
        {
            str += a->name + ", ";
        }
        return str;
    }

    std::vector<IRValue*> args;
};

class IRInstructionStackAlloc : public IRInstruction
{
public:
    explicit IRInstructionStackAlloc()
    {
        type = IR::StackAlloc;
    }

    std::string prettyString() override
    {
        return "x <- alloc";
    }

    IRValue* lhs = nullptr;
    IRValue* ty = nullptr;
};

class IRInstructionStore : public IRInstruction
{
public:
    explicit IRInstructionStore(IRValue* memory, IRValue* value)
        : memory(memory)
        , value(value)
    {
        type = IR::Store;
    }

    std::string prettyString() override {
        return "store x y";
    }

    IRValue* memory = nullptr;
    IRValue* value = nullptr;
};

class IRInstructionJump : public IRInstruction
{
public:
	explicit IRInstructionJump(IRValue* target)
			: target(target)
	{
		type = IR::Jump;
	}

	std::string prettyString() override {
		return "jmp";
	}

	IRValue* target = nullptr;
};

class IRInstructionNew : public IRInstruction
{
public:
    explicit IRInstructionNew()
    {
        type = IR::New;
    }

    std::string prettyString() override {
        return "New";
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
        if (!lhs || !rhs)
            return {};

        std::string res;
        res += result->name + " <- ";
        res += lhs->name + " " + op + " " + rhs->name;
        return res;
    }

    std::string op;
    IRValue* lhs = nullptr;
    IRValue* rhs = nullptr;
    IRValue* result = nullptr;
};

/* Defining a type */
class IRType
{
public:

    struct Field
    {
        std::string name;
    };

    explicit IRType(const std::string& name)
    {
        this->name = name;
    }

    explicit IRType(RzType* ty)
    {
        this->ty = ty;
    }

    std::string prettyString() {
        if (ty && ty->isNative())
        {
            return "%var = extern type " + ty->getName();
        }

        std::string str = "\%" + name + " = type  {\n";

        for (auto& f : fields)
        {
            str += "\t" + f.name + "\n";
        }

        return str += "}\n\n";
    }


//private:
    std::vector<Field> fields;
    std::string name; // anonymous is viable
    RzType* ty = nullptr;
};

class IRFunction
{
public:
    std::string name;
    std::string path;
    std::vector<IRInstruction*> instructions;
};

class RzType;

/*
 * An IR context holds a set of functions and additional metadata
 * I.e One entire module is usually compiled into one IRContext
 */
class IRContext
{
public:

    void createExternalType(RzType* ty);
    void createType(const std::string& name, const std::vector<IRType::Field>& fields);

    void writeToFile(const std::string& filename);

public:
    std::vector<IRType> types;
    std::vector<IRFunction> functions;

    ScopeStack stk;
};

#endif // IR_HPP
