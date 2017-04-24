#include "aeNodeBinaryOperator.h"
#include <RazerCompiler/RzCompiler.h>
#include <RazerRuntime/RzEngine.h>
#include <Logger.h>

aeNodeBinaryOperator::aeNodeBinaryOperator(aeNodeExpr* opA, aeNodeExpr* opB, std::string _oper)
{
	m_nodeType = AEN_BINARYOP;

	operandA = opA;
	operandB = opB;
	oper = _oper;
}

bool aeNodeBinaryOperator::isRelational()
{
	return (oper == ">") || (oper == ">=") || (oper == "<") || (oper == "<=")
		|| (oper == "==") || (oper == "!=");
}

bool aeNodeBinaryOperator::isArithmetic()
{
	return oper == "+" || oper == "-" || oper == "*" || oper == "/";
}

RzQualType aeNodeBinaryOperator::getQualifiedType(RzCompiler* c, RzQualType base)
{
    RzQualType qt;
	if (isRelational())
	{
		qt.m_type = c->m_env->getTypeInfo("bool");
	}
	else if (isArithmetic())
	{
        RzQualType T1 = operandA->getQualifiedType(c);
        RzQualType T2 = operandB->getQualifiedType(c);

        if (T1 == T2) {
            qt = T1;
        }
        else {
            RZLOG("error: implicit primitive promotion not ready\n");
        }
	}

	return qt;
}

std::string aeNodeBinaryOperator::str() const
{
	if (!operandA || !operandB)
		return std::string();

	return std::string("(") + operandA->str() + " " + oper + " " + operandB->str() + ")";
}
