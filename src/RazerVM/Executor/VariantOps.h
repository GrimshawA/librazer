#ifndef VariantOps_h__
#define VariantOps_h__

inline static void DoVarStore(RzVirtualMachine* vm, int storeType, int typeIndex, int c)
{
	RzStackValue operand = vm->m_mainContext.pop_value();
	AEValueRef ref = vm->m_mainContext.popVariantRef();

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

inline void DoVarLoad(RzVirtualMachine* vm, int memberIndex)
{
	RzValue varObj;
	vm->m_mainContext.popVariant(varObj);
	std::string fieldName = vm->m_mainContext.cl->module->m_identifierPool[memberIndex];

	//if (varObj)
	//{
		RzValue memberField = varObj.property(fieldName);
		vm->m_mainContext.pushVariant(memberField);
	//}
}

inline void DoVarLoadRef(RzVirtualMachine* vm, int op, int data0)
{
	if (op == 0)
	{
		// There is already a ref in the stack, now get a subref out of it
		AEValueRef ref = vm->m_mainContext.popVariantRef();
		if (ref.type == AEValueRef::REF_VALUE)
		{

			AEValueRef subRef = (*ref.value).makeRefForChild(vm->get_current_mod()->m_identifierPool[data0]);			
			vm->m_mainContext.pushVariantRef(subRef);
		}
	}

	if (op == 1)
	{
		// Load a local variable as a ref
		RzValue* referredValue = reinterpret_cast<RzValue*>(vm->m_mainContext.ebp - data0 - sizeof(RzValue));
		AEValueRef ref;
		ref.type = AEValueRef::REF_VALUE;
		ref.value = referredValue;
		vm->m_mainContext.pushVariantRef(ref);
	}
}

inline void DoAddV(RzVirtualMachine* vm)
{
	RzValue a,b;
	vm->m_mainContext.popVariant(b);
	vm->m_mainContext.popVariant(a);
	vm->m_mainContext.pushVariant(a+b);
}

inline void DoSubV(RzVirtualMachine* vm)
{
	RzValue a,b;
	vm->m_mainContext.popVariant(b);
	vm->m_mainContext.popVariant(a);
	vm->m_mainContext.pushVariant(a-b);
}

inline void DoMultV(RzVirtualMachine* vm)
{
	RzValue a,b;
	vm->m_mainContext.popVariant(b);
	vm->m_mainContext.popVariant(a);
	vm->m_mainContext.pushVariant(a*b);
}

inline void DoDivV(RzVirtualMachine* vm)
{
	RzValue a,b;
	vm->m_mainContext.popVariant(b);
	vm->m_mainContext.popVariant(a);
	vm->m_mainContext.pushVariant(a/b);
}

inline void DoCompareV(RzVirtualMachine* vm)
{
	RzValue a,b;
	vm->m_mainContext.popVariant(b);
	vm->m_mainContext.popVariant(a);
	vm->m_mainContext.pushVariant(a==b);
}

#endif // VariantOps_h__
