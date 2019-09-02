#ifndef FRONTEND_EMIT_FUNCTION_HPP_
#define FRONTEND_EMIT_FUNCTION_HPP_

#include <razer/frontend/AST/aeNodeFunction.h>
#include <razer/frontend/AST/Nodes.h>
#include <unordered_map>

struct ExpressionIntent
{
    enum Intents {
        Read,
        Write
    };

    ExpressionIntent() {}

    ExpressionIntent(Intents intent)
    {
        intents = intent;
    }

    Intents intents = Write;
};

/*
 * This class converts a function in AST form to IR
 */
class EmitFunction
{
public:
    explicit EmitFunction(aeNodeFunction& node, IRContext& ctx, AEStructNode* structNode);

    void compile();
	void compileBlock(aeNodeBlock& blk);

    IRValue* compileExpression(aeNodeExpr* expr, ExpressionIntent intent = {});
    IRValue* compileNewExpression(aeNodeNew& newNode);

	void compileFor(aeNodeFor& forNode);
    void compileWhile(aeNodeWhile& whileNode);

    IRValue* locateIdentifier(aeNodeIdentifier& identifier, ExpressionIntent intent = {});

private:
    aeNodeFunction& node;
    AEStructNode* structNode = nullptr;
    IRContext& ctx;
    IRBuilder builder;
    ScopeStack scopeResolver;
};

#endif // FRONTEND_EMIT_FUNCTION_HPP_
