#ifndef RZOVERLOADRESOLVER_H__
#define RZOVERLOADRESOLVER_H__

/*
 * Utility functions for resolving overloads.
 * This includes method overloads, operator overloads and other utils.
 */

class aeNodeBinaryOperator;
class aeNodeUnaryOperator;

/// Information on how to compile a binary operator
struct OperatorStrategy {
    enum Technique {
        NATIVE_CALL,
        REGULAR_CALL,
        PRIMITIVE_OPERATION
    };

    Technique technique;

};

/// Calculate the strategy to use for a given operation, which can vary from native method calls,
/// calling function overloads or resourcing to the built in primitive type instructions
OperatorStrategy resolveBinaryOperationStrategy(aeNodeBinaryOperator& binaryOperation);

/// Calculate the strategy to use for a given operation, which can vary from native method calls,
/// calling function overloads or resourcing to the built in primitive type instructions
OperatorStrategy resolveUnaryOperationStrategy(aeNodeUnaryOperator& unaryOperation);

#endif
