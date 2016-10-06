#ifndef VariantOps_h__
#define VariantOps_h__

inline static void DoVarStore(AEVirtualMachine* vm, int storeType, int typeIndex, int c)
{
	vm_value operand = vm->m_stk.pop_value();
	AEValueRef ref = vm->m_stk.popVariantRef();

	if (!ref.data && !ref.createOnAssign())
	{
		printf("RUNTIME EXCEPTION: Cannot assign to undefined\n");
		return;
	}

	printf("store type: %d\n", storeType);

	if (ref.type == AEValueRef::REF_RAW)
	{
		switch (storeType)
		{
		case AE_VARIANTTYPE_INT: {
									 memcpy(ref.data, &operand.i32, sizeof(int));
									 printf("Setting var %x to int %d\n", ref.data, operand.i64);
									 break;

		}
			/*case AE_VARIANTTYPE_FLOAT: varObj ? *varObj = operand.i64; break;
			case AE_VARIANTTYPE_STRING: varObj ? *varObj = operand.i64; break;
			case AE_VARIANTTYPE_OBJECT: varObj ? *varObj = operand.i64; break;*/
		}
	}
	else if (ref.type == AEValueRef::REF_VALUE)
	{
		switch (storeType)
		{
		case AE_VARIANTTYPE_INT: {
									 *ref.value = operand.i32;
									 break;
		}

		case AE_VARIANTTYPE_OBJECT: {
			AEType* typeInfo;// = vm->m_module;
			(*ref.value).setFromObject(operand.ptr, typeInfo);
			break;
		}
			/*case AE_VARIANTTYPE_FLOAT: varObj ? *varObj = operand.i64; break;
			case AE_VARIANTTYPE_STRING: varObj ? *varObj = operand.i64; break;
			case AE_VARIANTTYPE_OBJECT: varObj ? *varObj = operand.i64; break;*/
		}
	}
}

inline void DoVarLoad(AEVirtualMachine* vm, int memberIndex)
{
	AEValue varObj;
	vm->m_stk.popVariant(varObj);
	std::string fieldName = vm->m_stk.cl->module->m_identifierPool[memberIndex];

	//if (varObj)
	//{
		AEValue memberField = varObj.property(fieldName);
		vm->m_stk.pushVariant(memberField);
	//}
}

inline void DoVarLoadRef(AEVirtualMachine* vm, int op, int data0)
{
	if (op == 0)
	{
		// There is already a ref in the stack, now get a subref out of it
		AEValueRef ref = vm->m_stk.popVariantRef();
		if (ref.type == AEValueRef::REF_VALUE)
		{

			AEValueRef subRef = (*ref.value).makeRefForChild(vm->get_current_mod()->m_identifierPool[data0]);			
			vm->m_stk.pushVariantRef(subRef);
		}
	}

	if (op == 1)
	{
		// Load a local variable as a ref
		AEValue* referredValue = reinterpret_cast<AEValue*>(vm->m_stk.ebp - data0 - sizeof(AEValue));
		AEValueRef ref;
		ref.type = AEValueRef::REF_VALUE;
		ref.value = referredValue;
		vm->m_stk.pushVariantRef(ref);
	}
}

inline void DoAddV(AEVirtualMachine* vm)
{
	AEValue a,b;
	vm->m_stk.popVariant(b);
	vm->m_stk.popVariant(a);
	vm->m_stk.pushVariant(a+b);
}

inline void DoSubV(AEVirtualMachine* vm)
{
	AEValue a,b;
	vm->m_stk.popVariant(b);
	vm->m_stk.popVariant(a);
	vm->m_stk.pushVariant(a-b);
}

inline void DoMultV(AEVirtualMachine* vm)
{
	AEValue a,b;
	vm->m_stk.popVariant(b);
	vm->m_stk.popVariant(a);
	vm->m_stk.pushVariant(a*b);
}

inline void DoDivV(AEVirtualMachine* vm)
{
	AEValue a,b;
	vm->m_stk.popVariant(b);
	vm->m_stk.popVariant(a);
	vm->m_stk.pushVariant(a/b);
}

inline void DoCompareV(AEVirtualMachine* vm)
{
	AEValue a,b;
	vm->m_stk.popVariant(b);
	vm->m_stk.popVariant(a);
	vm->m_stk.pushVariant(a==b);
}

#endif // VariantOps_h__
