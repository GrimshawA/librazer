#ifndef RZTYPERESOLVER_H__
#define RZTYPERESOLVER_H__

#include <RazerParser/AST/aeNodeExpr.h>
#include <RazerRuntime/QualType.h>

/// Resolve the qualified type of an arbitrary expression
/// regarding a compilation context
RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeExpr& expr, RzQualType base = RzQualType());

#endif
