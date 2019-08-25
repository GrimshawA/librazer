#ifndef FRONTEND_EMIT_FUNCTION_HPP_
#define FRONTEND_EMIT_FUNCTION_HPP_

#include <razer/frontend/AST/aeNodeFunction.h>
#include <razer/frontend/AST/Nodes.h>
#include <unordered_map>

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

/*
 * This class converts a function in AST form to IR
 */
class EmitFunction
{
public:
    explicit EmitFunction(aeNodeFunction& node, IRContext& ctx);

    void compile();
    void compileBlock(aeNodeBlock& blk);
    IRValue* compileExpression(aeNodeExpr* expr);

    IRValue* locateIdentifier(aeNodeIdentifier& identifier);

private:
    aeNodeFunction& node;
    IRContext& ctx;
    IRBuilder builder;
    ScopeStack scopeResolver;
};

#endif // FRONTEND_EMIT_FUNCTION_HPP_
