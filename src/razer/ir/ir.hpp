#ifndef IR_HPP
#define IR_HPP

#include <razer/runtime/RzType.h>
#include <razer/ir/ir_value.hpp>

#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>

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
    virtual ~IRInstruction() = default;

    virtual bool doesReadValue(IRValue* val) {
        return false;
    }


public:
    IR::Type type;

public:

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
        std::stringstream str;
        str << "'" << ty << "' <- "
            << "getelementptr "
            << "'" << ty << "'"
            << " " << std::to_string(fieldIndex)
            << " '" << basePtr << "'";

        return str.str();
    }

    int fieldIndex;
    IRValue* ty = nullptr;
    IRValue* basePtr = nullptr;
};

class IRInstructionReturn : public IRInstruction
{
public:
    explicit IRInstructionReturn()
    {
        type = IR::Return;
    }

    std::string prettyString() override {
        std::stringstream ss;
        ss << "ret ";

        if (value == nullptr)
            ss << "void";
        else
            ss << "'" << value << "'";

        return ss.str();
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
        std::stringstream ss;
        ss << "'" << value << "'";
        ss << " <- label";
        return ss.str();
	}

	IRValue* value = nullptr;
};

class IRInstructionCall : public IRInstruction
{
public:
    explicit IRInstructionCall(IRValue* func)
    {
        type = IR::CALL;
        funcValue = func;
    }

    bool doesReadValue(IRValue* val) override
    {
        for (auto& a : args)
        {
            if (a == val)
                return true;
        }
        return false;
    }

    std::string prettyString() override {
        std::stringstream ss;
        ss << "'" << retValue << "'";
        ss << " <- call '" << funcValue << "' ";
        ss << "args '";

        for (auto& a : args)
        {
            ss << a << ", ";
        }
        ss << "'";

        return ss.str();
    }

    std::vector<IRValue*> args;
    IRValue* funcValue = nullptr;
    IRValue* retValue = nullptr;
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
        std::stringstream ss;
        ss << "'" << value << "' <- ";
        ss << "malloc " << size;

        return ss.str();
    }

    enum KindType {
        Stack,
        Heap
    } kind;

    std::size_t size = 0;
    IRValue* value = nullptr;

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

    bool doesReadValue(IRValue* val) override
    {
        if (memory == val)
            return true;

        if (value == val)
            return true;

        return false;
    }

    std::string prettyString() override {
        std::stringstream ss;
        ss << "store ";
        ss << "'" << memory << "' ";
        ss << "'" << value << "'";
        return ss.str();
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
        std::stringstream ss;
        ss << "jmp '" << target << "'";
        return ss.str();
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
        //this->ty = ty;
    }

    bool is(RzType* type) const
    {
        if (!ty)
            return false;

        if (!ty->isType())
            return false;

        return static_cast<IRValueType*>(ty)->type == type;
    }

    std::string prettyString() {
        std::stringstream ss;

        ss << "'" << ty << "' = type  {\n";

        for (auto& f : fields)
        {
            ss << "\t" + f.name + "\n";
        }

        ss << "}\n\n";
        return ss.str();
    }


//private:
    std::vector<Field> fields;
    std::string name; // anonymous is viable
    IRValue* ty = nullptr;
};

class IRFunction
{
public:
    std::string prettyString() {
        std::stringstream ss;
        ss << "def " << path << " '";

        for (auto& a : args)
        {
            ss << a << ", ";
        }
        ss << "'";

        return ss.str();
    }

    std::string name;
    std::string path;
    std::vector<IRValue*> args;
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
    IRValue* createType(RzType* ty, const std::string& name, const std::vector<IRType::Field>& fields);

    void writeToFile(const std::string& filename);

public:
    std::vector<IRType> types;
    std::vector<IRFunction> functions;

    ScopeStack stk;
};

#endif // IR_HPP
