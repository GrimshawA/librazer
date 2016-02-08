#include "aeCompilerConv.h"
#include "aeon_compiler.h"

void ICG_FloatToDouble(aeon_compiler* c)
{
	// In the vm operand stack, no action is required to convert a float to a double
}

void ICG_DoubleToFloat(aeon_compiler* c)
{
	// For operands, double would lose precision converting to float, but float is able to hold the double in the context of expression handling.
}

void ICG_DoubleToInt64(aeon_compiler* c)
{

}

void ICG_BoolToInt32(aeon_compiler* c)
{

}

void TypeSystemInformation::init()
{
//	ImplicitConversion floatToDouble(AE_PFLOAT, AE_PDOUBLE, &ICG_FloatToDouble);
	//m_table.push_back(floatToDouble);
}

bool TypeSystemInformation::canConvert(aeQualType T1, aeQualType T2)
{
	for (auto& ic : m_table)
	{
		if (ic.T1 == T1.getType() && ic.T2 == T2.getType())
		{
			return true;
		}
	}

	return false;
}

void TypeSystemInformation::performConversion(aeQualType T1, aeQualType T2, aeon_compiler* compiler)
{
	if (T1.getTypeName() == "int32" && T2.getTypeName() == "float")
	{
		compiler->emitInstruction(OP_CONV, AEP_INT32, AEP_FLOAT);
	}
	else if (T1.getTypeName() == "int32" && T2.getTypeName() == "double")
	{
		compiler->emitInstruction(OP_CONV, AEP_INT32, AEP_DOUBLE);
	}
	else if (T1.getTypeName() == "uint32" && T2.getTypeName() == "float")
	{
		compiler->emitInstruction(OP_CONV, AEP_UINT32, AEP_FLOAT);
	}
	else if (T1.getTypeName() == "uint32" && T2.getTypeName() == "double")
	{
		compiler->emitInstruction(OP_CONV, AEP_UINT32, AEP_DOUBLE);
	}
	else if (T1.getTypeName() == "float" && T2.getTypeName() == "int32")
	{
		compiler->emitInstruction(OP_CONV, AEP_FLOAT, AEP_INT32);
	} 
	else if (T1.getTypeName() == "double" && T2.getTypeName() == "int32")
	{
		compiler->emitInstruction(OP_CONV, AEP_DOUBLE, AEP_INT32);
	}
	else
	{
		compiler->throwError("I don't know how to convert " + T1.getTypeName() + " to " + T2.getTypeName());
	}
}