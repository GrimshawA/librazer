#include "emit_function.hpp"

#include <razer/frontend/AST/aeNodeBlock.h>
#include <razer/frontend/AST/Nodes.h>

EmitFunction::EmitFunction(aeNodeFunction& node, IRContext& ctx, AEStructNode* structNode)
: node(node)
, structNode(structNode)
, ctx(ctx)
, builder(ctx)
{
    builder.func.args.resize(1);
    builder.func.args[0] = builder.makeValue();

    builder.func.name = node.m_name;
    builder.func.path = node.m_name;
}

void EmitFunction::compile()
{
    compileBlock(*node.m_block.get());

    if (builder.getLastStmtType() != IR::Return)
    {
        builder.createReturn();
    }
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

        case AEN_WHILE: {
            compileWhile((aeNodeWhile&)*stmt);
            break;
        }

        default:
            break;

        }
    }

    scopeResolver.pop();
}

IRValue* EmitFunction::compileExpression(aeNodeExpr* expr)
{
    switch(expr->m_nodeType)
    {
    case AEN_BINARYOP: {
        aeNodeBinaryOperator& op = static_cast<aeNodeBinaryOperator&>(*expr);

        auto* lhs = compileExpression(op.operandA);
        auto* rhs = compileExpression(op.operandB);

        if (op.oper == "=")
        {
            // Store operations don't have an output
            builder.createStore(lhs, rhs);
            return nullptr;
        }

        return builder.createBinaryOp(op.oper, lhs, rhs);
    }

    case AEN_ACCESSOPERATOR: {
        aeNodeAccessOperator& dot = static_cast<aeNodeAccessOperator&>(*expr);

        if (dot.m_b->m_nodeType == AEN_FUNCTIONCALL)
        {
            std::vector<IRValue*> args;
            args.push_back(compileExpression(dot.m_a));
            return builder.createCall(nullptr, args);
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
        return compileNewExpression(ident);
    }


    }

    return nullptr;
}

IRValue* EmitFunction::compileNewExpression(aeNodeNew& newNode)
{
    // Create the object in the heap and get a ptr to it
    auto memSize = newNode.m_instanceType.m_type->getSize();

    auto* mem = builder.createHeapAlloc(memSize);

    auto* newTypeModule = newNode.m_instanceType.m_type->getModule();
    auto* newType = newNode.m_instanceType.m_type;

    std::string constructorName = newType->getName() + "." + newType->getName();
    IRValue* constructor = builder.makeFuncValue(constructorName);

    // Call the constructor
    auto* thisValue = builder.func.args[0];
    builder.createCall(constructor, {mem});

    return mem;
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

void EmitFunction::compileWhile(aeNodeWhile& whileNode)
{
    IRValue* label = builder.createLabelStmt();

    compileBlock(*whileNode.block.get());

    builder.createJumpStmt(label);
}

IRValue* EmitFunction::locateIdentifier(aeNodeIdentifier& identifier)
{
    auto* local = scopeResolver.locate(identifier.m_name);

    if (local)
        return local;

    // At class scope?
    if (structNode && structNode->hasField(identifier.m_name))
    {
        int fieldIndex = structNode->getFieldIndex(identifier.m_name);
        //auto* fieldType = structNode->getFieldType(identifier.m_name);
        return builder.createDestructure(structNode->m_typeInfo, fieldIndex, builder.func.args[0]);
    }

    return nullptr;
}
