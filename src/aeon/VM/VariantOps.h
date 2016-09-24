#ifndef VariantOps_h__
#define VariantOps_h__

inline static void DoVarStore(AEVirtualMachine* vm, int storeType, int b, int c)
{
	vm_value operand = vm->m_stk.pop_value();
	AEValue* varObj = vm->m_stk.pop_value().ptr;

	switch (storeType)
	{
	case 0: varObj ? *varObj = operand.i64; break;
	case 1: varObj ? *varObj = operand.i64; break;
	case 2: varObj ? *varObj = operand.i64; break;
	case 3: varObj ? *varObj = operand.i64; break;
	case 4: varObj ? *varObj = operand.i64; break;
	}
}


#endif // VariantOps_h__
