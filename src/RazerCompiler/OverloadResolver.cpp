#include <RazerCompiler/OverloadResolver.h>
#include <Logger.h>

#include <RazerParser/AST/aeNodeBinaryOperator.h>

OperatorStrategy resolveBinaryOperationStrategy(aeNodeBinaryOperator& binaryOperation) {
    return OperatorStrategy();
}

OperatorStrategy resolveUnaryOperationStrategy(aeNodeUnaryOperator& unaryOperation) {
    return OperatorStrategy();
}
