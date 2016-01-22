#include "aeCompilerConv.h"

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

void ConversionProcedureTable::init()
{
//	ImplicitConversion floatToDouble(AE_PFLOAT, AE_PDOUBLE, &ICG_FloatToDouble);
	//m_table.push_back(floatToDouble);
}

bool ConversionProcedureTable::canConvert(aeQualType T1, aeQualType T2)
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