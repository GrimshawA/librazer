#ifndef FRONTEND_EMIT_FUNCTION_HPP_
#define FRONTEND_EMIT_FUNCTION_HPP_

#include <razer/frontend/AST/aeNodeFunction.h>
#include <razer/frontend/AST/Nodes.h>
#include <unordered_map>

/*
 * This class converts a function in AST form to IR
 */
class EmitFunction
{
public:
    explicit EmitFunction(aeNodeFunction& node, IRContext& ctx, AEStructNode* structNode);

    void compile();
	void compileBlock(aeNodeBlock& blk);

    IRValue* compileExpression(aeNodeExpr* expr);
    IRValue* compileNewExpression(aeNodeNew& newNode);

	void compileFor(aeNodeFor& forNode);
    void compileWhile(aeNodeWhile& whileNode);

    IRValue* locateIdentifier(aeNodeIdentifier& identifier);

private:
    aeNodeFunction& node;
    AEStructNode* structNode = nullptr;
    IRContext& ctx;
    IRBuilder builder;
    ScopeStack scopeResolver;
};

#endif // FRONTEND_EMIT_FUNCTION_HPP_
