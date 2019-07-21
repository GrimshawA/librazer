#ifndef RZTYPERESOLVER_H__
#define RZTYPERESOLVER_H__

#include <razer/frontend/AST/aeNodeExpr.h>
#include <RazerRuntime/QualType.h>

/// Resolve the qualified type of an arbitrary expression
/// regarding a compilation context
RzQualType resolveQualifiedType(RzCompiler& ctx, aeNodeExpr& expr, RzQualType base = RzQualType());

/// Resolves the final type each operator should have for a numerical operation between the two
RzQualType resolvePromotedType(RzQualType t1, RzQualType t2);

/// Resolves the primitive enumerator for the VM from a qualified type
int resolvePrimitiveConstantFromType(const RzQualType& t);

/// Resolves an unlinked type, which is in a string based form yet
/// and looking to become fully linked with a real RzType
bool resolveUnlinkedType(RzCompiler& ctx, RzQualType& t);

#endif
