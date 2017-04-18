#include <RazerVM/ThreadContext.h>
#include <RazerRuntime/Module.h>
#include <Logger.h>

#define TRACE_VM

/*
	x.f()

	Calls function named f on object x (variant type)
*/
inline static void ExecVariantCall(RzThreadContext& ctx, int identifierIndex)
{
	RzValue thisVar;
	ctx.popVariant(thisVar);

	if (thisVar.m_valueType != RzValue::VALUE_OBJECT)
	{
		RZLOG("Exception: Calling a method on undefined\n");
		return;
	}

	std::string methodName = ctx.cl->module->m_identifierPool[identifierIndex];
	
#if defined TRACE_VM
	RZLOG("Attempting to call method %s\n", methodName.c_str());
#endif

	if (thisVar._object->m_obj)
	{
		RzType* wrappedObjectType = thisVar._object->m_type;
		RzType::MethodInfo* method = wrappedObjectType->getMethod(methodName);

		if (!method)
		{
            RZEXCEPTION("EXCEPTION: No such method in the object %s\n", methodName.c_str());
			return;
		}

		if (method->methodCallback)
		{
			// Must push the native object pointer to call the method on
			ctx.push_addr(thisVar._object->m_obj);

			RzGeneric g; g.m_threadCtx = &ctx;
			g.m_variantCall = true;
			aeBindMethod funPtr = thisVar._object->m_type->getNativeFunction(methodName);
			if (funPtr){
				RZLOG("About to call an actual function\n");
				funPtr(g);
			}
		}
		else
		{
			RZLOG("Calling script function %s offset %d\n", methodName.c_str(), method->offset);

			ctx.pc = ctx.cl->pc;
			ctx.ebp = ctx.cl->ebp;			

			RzStackFrame stackFrame;
			stackFrame.name = methodName;
			stackFrame.pc = method->offset - 1;
			stackFrame.module = ctx.cl->module;
			stackFrame.ebp = ctx.ebp;
			//stackFrame.function = functionData;
			ctx.frames.push_back(stackFrame);
			ctx.cl = &ctx.frames[ctx.frames.size() - 1];			
		}
	}
}

inline static void DoVarStore(RzThreadContext& ctx, int storeType, int moduleIndex, int typeIndex)
{
	if (storeType == AE_VARIANTTYPE_VAR)
	{
		// RHS is another var
		RzValue rhs;
		ctx.popVariant(rhs);

		AEValueRef ref = ctx.popVariantRef();

		(*ref.value) = rhs;

		return;
	}


	RzStackValue operand = ctx.pop_value();
	AEValueRef ref = ctx.popVariantRef();

	if (!ref.data && !ref.createOnAssign())
	{
		RZLOG("RUNTIME EXCEPTION: Cannot assign to undefined\n");
		return;
	}

	RZLOG("store type: %d\n", storeType);

	if (ref.type == AEValueRef::REF_RAW)
	{
		switch (storeType)
		{
		case AE_VARIANTTYPE_INT: {
									 memcpy(ref.data, &operand.i32, sizeof(int));
									 RZLOG("Setting var %x to int %d\n", ref.data, operand.i64);
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
			RzType* typeInfo = ctx.getModule().resolveType(moduleIndex, typeIndex);
			(*ref.value).setFromObject(operand.ptr, typeInfo);
			break;
		}
			/*case AE_VARIANTTYPE_FLOAT: varObj ? *varObj = operand.i64; break;
			case AE_VARIANTTYPE_STRING: varObj ? *varObj = operand.i64; break;
			case AE_VARIANTTYPE_OBJECT: varObj ? *varObj = operand.i64; break;*/
		}
	}
}

inline void DoVarLoad(RzThreadContext& ctx, int memberIndex)
{
	RzValue varObj;
	ctx.popVariant(varObj);
	std::string fieldName = ctx.cl->module->m_identifierPool[memberIndex];

	//if (varObj)
	//{
		RzValue memberField = varObj.property(fieldName);
		ctx.pushVariant(memberField);
	//}
}

inline void DoVarLoadRef(RzThreadContext& ctx, int op, int data0)
{
	if (op == 0)
	{
		// There is already a ref in the stack, now get a subref out of it
		AEValueRef ref = ctx.popVariantRef();
		if (ref.type == AEValueRef::REF_VALUE)
		{

			AEValueRef subRef = (*ref.value).makeRefForChild(ctx.cl->module->m_identifierPool[data0]);			
			ctx.pushVariantRef(subRef);
		}
	}

	if (op == 1)
	{
		// Load a local variable as a ref
		RzValue* referredValue = reinterpret_cast<RzValue*>(ctx.ebp - data0 - sizeof(RzValue));
		AEValueRef ref;
		ref.type = AEValueRef::REF_VALUE;
		ref.value = referredValue;
		ctx.pushVariantRef(ref);
	}
}

inline void DoAddV(RzThreadContext& ctx)
{
	RzValue a,b;
	ctx.popVariant(b);
	ctx.popVariant(a);
	ctx.pushVariant(a + b);
}

inline void DoSubV(RzThreadContext& ctx)
{
	RzValue a,b;
	ctx.popVariant(b);
	ctx.popVariant(a);
	ctx.pushVariant(a - b);
}

inline void DoMultV(RzThreadContext& ctx)
{
	RzValue a,b;
	ctx.popVariant(b);
	ctx.popVariant(a);
	ctx.pushVariant(a*b);
}

inline void DoDivV(RzThreadContext& ctx)
{
	RzValue a,b;
	ctx.popVariant(b);
	ctx.popVariant(a);
	ctx.pushVariant(a / b);
}

inline void DoCompareV(RzThreadContext& ctx)
{
	RzValue a,b;
	ctx.popVariant(b);
	ctx.popVariant(a);
	ctx.pushVariant(a == b);
}
