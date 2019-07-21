#include <RazerCompiler/OverloadResolver.h>
#include <razer/utils/Logger.h>

#include <razer/frontend/AST/aeNodeBinaryOperator.h>

OperatorStrategy resolveBinaryOperationStrategy(aeNodeBinaryOperator& binaryOperation) {
    return OperatorStrategy();
}

OperatorStrategy resolveUnaryOperationStrategy(aeNodeUnaryOperator& unaryOperation) {
    return OperatorStrategy();
}
