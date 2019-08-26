#ifndef IR_HPP
#define IR_HPP

#include <vector>
#include <string>

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
        Label,
        Undefined
    };
}

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
        res += op + " " + lhs->name + " " + rhs->name;
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

    std::string prettyString() {
        std::string str = "type " + name + " {\n";

        for (auto& f : fields)
        {
            str += "\t" + f.name + "\n";
        }

        return str += "}\n\n";
    }


//private:
    std::vector<Field> fields;
    std::string name; // anonymous is viable
};

class IRFunction
{
public:
    std::string name;
    std::string path;
    std::vector<IRInstruction*> instructions;
};

/*
 * An IR context holds a set of functions and additional metadata
 * I.e One entire module is usually compiled into one IRContext
 */
class IRContext
{
public:

    void createType(const std::string& name, const std::vector<IRType::Field>& fields);

    void writeToFile(const std::string& filename);

public:
    std::vector<IRType> types;
    std::vector<IRFunction> functions;
};

#endif // IR_HPP
