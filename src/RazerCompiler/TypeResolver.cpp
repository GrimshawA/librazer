#include <RazerCompiler/TypeResolver.h>
#include <RazerCompiler/RzCompiler.h>
#include <Logger.h>

#include <RazerParser/AST/aeNodeBinaryOperator.h>
#include <RazerParser/AST/aeNodeAccessOperator.h>
#include <RazerParser/AST/aeNodeFunctionCall.h>
#include <RazerParser/AST/aeNodeIdentifier.h>
#include <RazerParser/AST/aeNodeLiterals.h>
#include <RazerParser/AST/aeNodeNew.h>
#include <RazerParser/AST/aeNodeTernaryOperator.h>
#include <RazerParser/AST/aeNodeUnaryOperator.h>

#include <cassert>

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeBinaryOperator& binOp, RzQualType base) {
    RzQualType qt;
    if (binOp.isRelational())
    {
        qt.m_type = ctx.m_env->getTypeInfo("bool");
    }
    else if (binOp.isArithmetic())
    {
        RzQualType T1 = resolveQualifiedType(ctx, *binOp.operandA, base);
        RzQualType T2 = resolveQualifiedType(ctx, *binOp.operandB, base);

        if (T1 == T2) {
            qt = T1;
        }
        else {
            RZLOG("error: implicit primitive promotion not ready\n");
        }
    }

    return qt;
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeAccessOperator& accessOp, RzQualType base) {
    // When the left side in a.b is a var, it can only yield another var as result
    RzQualType leftType = resolveQualifiedType(ctx, *accessOp.m_a, base);
    if (leftType.isVariant())
    {
        return leftType;
    }

    // Determining the right side is dependent on the left type
    RzQualType lhsType = resolveQualifiedType(ctx, *accessOp.m_a, base);

    return resolveQualifiedType(ctx, *accessOp.m_b, lhsType);
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeFunctionCall& funcCall, RzQualType base) {
    if (base) {
        // This is a call on a specific expression type
        RzFunction* fn = base.m_type->getFunction(funcCall.m_name);
        if (fn) {
            return fn->returnType;
        }
    }

    if (!funcCall.m_fn)
    {
        // Derives which function this call actually wants to call, to find the return type
        funcCall.m_fn = ctx.selectFunction(&funcCall);
    }

    if (funcCall.m_fn)
    {
        return funcCall.m_fn->returnType;
    }

    return RzQualType();
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeString& stringLiteral, RzQualType base) {
    return RzQualType(ctx.m_env->getTypeInfo("string"));
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeInteger& integerLiteral, RzQualType base) {
    return RzQualType(ctx.m_env->getTypeInfo("int32"));
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeFloat& floatLiteral, RzQualType base) {
    return RzQualType(ctx.m_env->getTypeInfo("float"));
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeNew& newExpr, RzQualType base) {
    newExpr.m_instanceType = RzQualType(ctx.m_env->getTypeInfo(newExpr.type));
    newExpr.m_instanceType.m_typeString = newExpr.type;
    return newExpr.m_instanceType;
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeIdentifier& identExpr, RzQualType base) {
    RzType* typeInfo = ctx.m_env->getTypeInfo(identExpr.m_name);
    if (typeInfo)
    {
        // This identifier is actually a type (enum, class)
        RzQualType qt;
        qt.m_type = typeInfo;
        return qt;
    }

    RzQualType qt = ctx.getVariable(identExpr.m_name).type;
    if (!qt.m_type) {
        qt.m_type = ctx.m_env->getTypeInfo(qt.m_typeString);
    }
    return qt;
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeUnaryOperator& unaryOp, RzQualType base) {
    return resolveQualifiedType(ctx, *unaryOp.Operand, base);
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeExpr& expr, RzQualType base) {

    switch (expr.m_nodeType) {
    case AEN_BINARYOP: {
        return resolveQualifiedType(ctx, (aeNodeBinaryOperator&)expr, base);
    }
    case AEN_ACCESSOPERATOR: {
        return resolveQualifiedType(ctx, (aeNodeAccessOperator&)expr, base);
    }
    case AEN_FUNCTIONCALL: {
        return resolveQualifiedType(ctx, (aeNodeFunctionCall&)expr, base);
    }
    case AEN_STRING: {
        return resolveQualifiedType(ctx, (aeNodeString&)expr, base);
    }
    case AEN_INTEGER: {
        return resolveQualifiedType(ctx, (aeNodeInteger&)expr, base);
    }
    case AEN_FLOAT: {
        return resolveQualifiedType(ctx, (aeNodeFloat&)expr, base);
    }
    case AEN_NEW: {
        return resolveQualifiedType(ctx, (aeNodeNew&)expr, base);
    }
    case AEN_IDENTIFIER: {
        return resolveQualifiedType(ctx, (aeNodeIdentifier&)expr, base);
    }
    case AEN_UNARYOP: {
        return resolveQualifiedType(ctx, (aeNodeUnaryOperator&)expr, base);
    }

    default: {
        assert(false);
    }

    }
}

RzQualType resolvePromotedType(RzQualType t1, RzQualType t2) {
    if (!t1 || !t2)
        return RzQualType();

    if (t1.sameTypeAs(t2))
        return t1;

    if (t1.getName() == "int32" && t2.getName() == "float") {
        return t2;
    }

    if (t1.getName() == "float" && t2.getName() == "int32") {
        return t1;
    }

    return RzQualType();
}

int resolvePrimitiveConstantFromType(const RzQualType& t) {
    if (t.getName() == "int32")
        return AEP_INT32;
    else if (t.getName() == "float")
        return AEP_FLOAT;

    return AEP_INVALID;
}

bool resolveUnlinkedType(RzCompiler& ctx, RzQualType& t) {
    RzType* typeToLink = ctx.m_env->getTypeInfo(t.m_typeString);
    if (!typeToLink) {
        RZLOG("Failed to link type '%s'\n", t.m_typeString.c_str());
        return false;
    }

    t.m_type = typeToLink;
    return true;
}
