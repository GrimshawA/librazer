#include <RazerCompiler/TypeResolver.h>
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
    return binOp.getQualifiedType(&ctx, base);
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeAccessOperator& accessOp, RzQualType base) {
    return accessOp.getQualifiedType(&ctx, base);
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeFunctionCall& funcCall, RzQualType base) {
    return funcCall.getQualifiedType(&ctx, base);
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeString& stringLiteral, RzQualType base) {
    return stringLiteral.getQualifiedType(&ctx, base);
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeInteger& integerLiteral, RzQualType base) {
    return integerLiteral.getQualifiedType(&ctx, base);
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeFloat& floatLiteral, RzQualType base) {
    return floatLiteral.getQualifiedType(&ctx, base);
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeNew& newExpr, RzQualType base) {
    return newExpr.getQualifiedType(&ctx, base);
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeIdentifier& identExpr, RzQualType base) {
    return identExpr.getQualifiedType(&ctx, base);
}

RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeUnaryOperator& unaryOp, RzQualType base) {
    return unaryOp.getQualifiedType(&ctx, base);
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
