#include "emit_function.hpp"

#include <razer/frontend/AST/aeNodeBlock.h>
#include <razer/frontend/AST/Nodes.h>

EmitFunction::EmitFunction(aeNodeFunction& node, IRContext& ctx)
: node(node)
, ctx(ctx)
, builder(ctx)
{
    builder.func.name = node.m_name;
    builder.func.path = node.m_name;
}

void EmitFunction::compile()
{
    compileBlock(*node.m_block.get());
}

void EmitFunction::compileBlock(aeNodeBlock& blk)
{
    scopeResolver.push();

    for (auto& stmt : blk.m_items)
    {
        switch(stmt->m_nodeType)
        {

        case AEN_VARDECL: {
            auto& decl = static_cast<aeNodeVarDecl&>(*stmt);

            auto* allocValue = builder.createLocalAlloc();

            auto& initAssign = static_cast<aeNodeBinaryOperator&>(*decl.m_decls[0].m_init);

            auto* rhsValue = compileExpression(initAssign.operandB);

            builder.createStore(allocValue, rhsValue);

            scopeResolver.set(decl.m_decls[0].m_name, allocValue);
            break;
        }

        case AEN_BINARYOP: {
            auto* result = compileExpression((aeNodeExpr*)stmt);
            break;
        }

        case AEN_ACCESSOPERATOR: {
            auto* result = compileExpression((aeNodeExpr*)stmt);
            break;
        }


        case AEN_FOR: {
			compileFor((aeNodeFor&)*stmt);
			break;
		}

        default:
            break;

        }
    }

    if (builder.getLastStmtType() != IR::Return)
    {
        builder.createReturn();
    }

    scopeResolver.pop();
}

IRValue* EmitFunction::compileExpression(aeNodeExpr* expr)
{
    switch(expr->m_nodeType)
    {
    case AEN_BINARYOP: {
        return nullptr;
    }

    case AEN_ACCESSOPERATOR: {
        aeNodeAccessOperator& dot = static_cast<aeNodeAccessOperator&>(*expr);

        if (dot.m_b->m_nodeType == AEN_FUNCTIONCALL)
        {
            std::vector<IRValue*> args;
            args.push_back(compileExpression(dot.m_a));
            return builder.createCall(args);
        }
        else
        {

        }

        return nullptr;
        //return builder.call();
    }

    case AEN_IDENTIFIER: {
        auto& ident = static_cast<aeNodeIdentifier&>(*expr);
        return locateIdentifier(ident);
    }

    case AEN_NEW: {
        auto& ident = static_cast<aeNodeNew&>(*expr);
        return builder.newObject(nullptr);
    }


    }

    return nullptr;
}

void EmitFunction::compileFor(aeNodeFor& forNode)
{
	// init expression

	// for label
	IRValue* label = builder.createLabelStmt();

	compileBlock(*forNode.block.get());

	IRValue* updateExprValue = compileExpression(forNode.incrExpr.get());

	// Finally
	builder.createJumpStmt(label);
}

IRValue* EmitFunction::locateIdentifier(aeNodeIdentifier& identifier)
{
    return scopeResolver.locate(identifier.m_name);
}
