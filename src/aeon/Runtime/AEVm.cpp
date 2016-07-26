#include <AEON/Runtime/aeVM.h>
#include <AEON/Runtime/aeByteCode.h>
#include <AEON/Runtime/AEContext.h>
#include <AEON/Runtime/AEObject.h>
#include <AEON/Runtime/AEGeneric.h>

#include <AEON/Runtime/AEVmCalls.h>

#define vm_start(x) case x:{
#define vm_end break;}

#define LOGVM(x, a, b, c) printf("[vm] %s %d %d %d\n", #x, a, b, c);

#define VMLOGLEVEL 5
#define vm_log(tag, level, STR) if(level > VMLOGLEVEL) printf("%s: %s\n", tag, STR);

aeVM::aeVM()
{
	memset(m_stk.stack.data(), 0, m_stk.stack.size());
}

aeVM::aeVM(AEContext* context)
{
	m_ctx = context;
}

AEModule* aeVM::get_current_mod()
{
	return m_stk.frames[m_stk.frames.size() - 1].module;
}

void aeVM::callMethod(AEObject* object, const std::string& prototype)
{
	m_stk.ebp = m_stk.esp;

	m_stk.pushThisPtr(object->addr);
	  
	//printf("callMethod: %s. pushing this, pointing to %x popped\n", prototype.c_str(), object->addr);

	call(*object->getType()->getModule(), prototype.c_str());
} 

void aeVM::callMethod(AEObject* object, uint32_t methodId)
{

}

void aeVM::prepare(aeFunctionId function)
{

}

void aeVM::pushThis(void* obj)
{
	m_stk.push_addr(obj);
}

void aeVM::setContext(AEContext* context)
{
	m_ctx = context;
} 

void printBits2(size_t const size, void const * const ptr)
{
		unsigned char *b = (unsigned char*)ptr;
		unsigned char byte;
		int i, j;

		for (i = size - 1; i >= 0; i--)
		{
			for (j = 7; j >= 0; j--)
			{
				byte = b[i] & (1 << j);
				byte >>= j;
				printf("%u", byte);
			}
		}
		puts("");
}

void aeVM::call(AEModule& module, const char* func)
{
	aeFunctionId functionId = 0;

	AEFunction* function = m_ctx->getFunctionByName(func);
	int i = 0;

	if (function)
	{
		call(function);
	}
	else
	{
		printf("Couldn't find the calling function\n");
	}
}

void aeVM::call(AEFunction* fn)
{
	if (!fn->m_compiled)
	{
		//printf("This function is not compiled '%s'.\n", function->getSymbolName().c_str());
		return;
	}

	prepare(fn->id);

	aeStackFrame callinfo;
	callinfo.name = fn->getName();
	callinfo.object = nullptr;
	callinfo.pc = fn->offset;
	callinfo.module = fn->m_module;
	callinfo.ebp = m_stk.esp;
	callinfo.function = fn;
	m_stk.frames.push_back(callinfo);

	// Launch the thread from this entry point
	execute(m_stk);
}

inline static void DoAdd(aeVM* vm, AeonPrimitiveType ptype)
{
	vm_value a = vm->m_stk.pop_value();
	vm_value b = vm->m_stk.pop_value();

	switch (ptype)
	{
	case AEP_DOUBLE: a.dp = a.dp + b.dp; break;
	case AEP_FLOAT: a.fp = a.fp + b.fp; break;
	case AEP_UINT8: a.u8 = a.u8 + b.u8; break;
	case AEP_INT8: a.i8 = a.i8 + b.i8; break;
	case AEP_UINT16: a.u16 = a.u16 + b.u16; break;
	case AEP_INT16: a.i16 = a.i16 + b.i16; break;
	case AEP_UINT32: a.u32 = a.u32 + b.u32; break;
	case AEP_INT32: a.i32 = a.i32 + b.i32; break;
	case AEP_UINT64: a.u64 = a.u64 + b.u64; break;
	case AEP_INT64: a.i64 = a.i64 + b.i64; break;
	}

	vm->m_stk.push_value(a);
}

inline static void DoSub(aeVM* vm, AeonPrimitiveType ptype)
{
	vm_value b = vm->m_stk.pop_value();
	vm_value a = vm->m_stk.pop_value();

	switch (ptype)
	{
	case AEP_DOUBLE: a.dp = a.dp - b.dp; break;
	case AEP_FLOAT: a.fp = a.fp - b.fp; break;
	case AEP_UINT8: a.u8 = a.u8 - b.u8; break;
	case AEP_INT8: a.i8 = a.i8 - b.i8; break;
	case AEP_UINT16: a.u16 = a.u16 - b.u16; break;
	case AEP_INT16: a.i16 = a.i16 - b.i16; break;
	case AEP_UINT32: a.u32 = a.u32 - b.u32; break;
	case AEP_INT32: a.i32 = a.i32 - b.i32; break;
	case AEP_UINT64: a.u64 = a.u64 - b.u64; break;
	case AEP_INT64: a.i64 = a.i64 - b.i64; break;
	}

	vm->m_stk.push_value(a);
}

inline static void DoMul(aeVM* vm, AeonPrimitiveType ptype)
{
	vm_value a = vm->m_stk.pop_value();
	vm_value b = vm->m_stk.pop_value();

	switch (ptype)
	{
	case AEP_DOUBLE: a.dp = a.dp * b.dp; break;
	case AEP_FLOAT: a.fp = a.fp * b.fp; break;
	case AEP_UINT8: a.u8 = a.u8 * b.u8; break;
	case AEP_INT8: a.i8 = a.i8 * b.i8; break;
	case AEP_UINT16: a.u16 = a.u16 * b.u16; break;
	case AEP_INT16: a.i16 = a.i16 * b.i16; break;
	case AEP_UINT32: a.u32 = a.u32 * b.u32; break;
	case AEP_INT32: a.i32 = a.i32 * b.i32; break;
	case AEP_UINT64: a.u64 = a.u64 * b.u64; break;
	case AEP_INT64: a.i64 = a.i64 * b.i64; break;
	}

	vm->m_stk.push_value(a);
}

inline static void DoDiv(aeVM* vm, AeonPrimitiveType ptype)
{
	vm_value b = vm->m_stk.pop_value();
	vm_value a = vm->m_stk.pop_value();

	//printf("DIV %d %d\n", a.i32, b.i32);

	switch (ptype)
	{
	case AEP_DOUBLE: a.dp = a.dp / b.dp; break;
	case AEP_FLOAT: a.fp = a.fp / b.fp; break;
	case AEP_UINT8: a.u8 = a.u8 / b.u8; break;
	case AEP_INT8: a.i8 = a.i8 / b.i8; break;
	case AEP_UINT16: a.u16 = a.u16 / b.u16; break;
	case AEP_INT16: a.i16 = a.i16 / b.i16; break;
	case AEP_UINT32: a.u32 = a.u32 / b.u32; break;
	case AEP_INT32: a.i32 = a.i32 / b.i32; break;
	case AEP_UINT64: a.u64 = a.u64 / b.u64; break;
	case AEP_INT64: a.i64 = a.i64 / b.i64; break;
	}

	vm->m_stk.push_value(a);
}

inline static void DoMod(aeVM* vm, AeonPrimitiveType ptype)
{
	vm_value a = vm->m_stk.pop_value();
	vm_value b = vm->m_stk.pop_value();

	switch (ptype)
	{
	//case AEP_DOUBLE: a.dp = a.dp % b.dp; break;
	//case AEP_FLOAT: a.fp = a.fp % b.fp; break;
	case AEP_UINT8: a.u8 = a.u8 % b.u8; break;
	case AEP_INT8: a.i8 = a.i8 % b.i8; break;
	case AEP_UINT16: a.u16 = a.u16 % b.u16; break;
	case AEP_INT16: a.i16 = a.i16 % b.i16; break;
	case AEP_UINT32: a.u32 = a.u32 % b.u32; break;
	case AEP_INT32: a.i32 = a.i32 % b.i32; break;
	case AEP_UINT64: a.u64 = a.u64 % b.u64; break;
	case AEP_INT64: a.i64 = a.i64 % b.i64; break;
	}

	vm->m_stk.push_value(a);
}

inline static void DoConversion(aeVM* vm, AeonPrimitiveType t1, AeonPrimitiveType t2)
{
	vm_value v = vm->m_stk.pop_value();
	
	if (t1 == AEP_INT32 && t2 == AEP_FLOAT)
		v.fp = static_cast<float>(v.i32);
	else if (t1 == AEP_INT32 && t2 == AEP_DOUBLE)
		v.dp = static_cast<double>(v.i32);
	else if (t1 == AEP_UINT32 && t2 == AEP_FLOAT)
		v.fp = static_cast<float>(v.u32);
	else if (t1 == AEP_UINT32 && t2 == AEP_DOUBLE)
		v.dp = static_cast<double>(v.u32);
	else if (t1 == AEP_FLOAT && t2 == AEP_INT32)
		v.i32 = static_cast<int32_t>(v.fp);
	else if (t1 == AEP_DOUBLE && t2 == AEP_INT32)
		v.i32 = static_cast<int32_t>(v.dp);

	vm->m_stk.push_value(v);
}

inline static void DoEquals(aeVM* vm, AeonPrimitiveType ptype)
{
	vm_value a = vm->m_stk.pop_value();
	vm_value b = vm->m_stk.pop_value();

	switch (ptype)
	{
	case AEP_DOUBLE: a.dp = a.dp == b.dp; break;
	case AEP_FLOAT: a.fp = a.fp == b.fp; break;
	case AEP_UINT8: a.u8 = a.u8 == b.u8; break;
	case AEP_INT8: a.i8 = a.i8 == b.i8; break;
	case AEP_UINT16: a.u16 = a.u16 == b.u16; break;
	case AEP_INT16: a.i16 = a.i16 == b.i16; break;
	case AEP_UINT32: a.u32 = a.u32 == b.u32; break;
	case AEP_INT32: a.i32 = a.i32 == b.i32; break;
	case AEP_UINT64: a.u64 = a.u64 == b.u64; break;
	case AEP_INT64: a.i64 = a.i64 == b.i64; break;
	}

	vm->m_stk.push_value(a);
}

inline static void DoNotEquals(aeVM* vm, AeonPrimitiveType ptype)
{
	vm_value a = vm->m_stk.pop_value();
	vm_value b = vm->m_stk.pop_value();

	switch (ptype)
	{
	case AEP_DOUBLE: a.dp = a.dp != b.dp; break;
	case AEP_FLOAT: a.fp = a.fp != b.fp; break;
	case AEP_UINT8: a.u8 = a.u8 != b.u8; break;
	case AEP_INT8: a.i8 = a.i8 != b.i8; break;
	case AEP_UINT16: a.u16 = a.u16 != b.u16; break;
	case AEP_INT16: a.i16 = a.i16 != b.i16; break;
	case AEP_UINT32: a.u32 = a.u32 != b.u32; break;
	case AEP_INT32: a.i32 = a.i32 != b.i32; break;
	case AEP_UINT64: a.u64 = a.u64 != b.u64; break;
	case AEP_INT64: a.i64 = a.i64 != b.i64; break;
	}

	vm->m_stk.push_value(a);
}

inline static void DoAssign(aeVM* vm, int mode, int offset, int type)
{
	// Assign modes:
	// pod_copy - simply copy N bytes from the loaded value into the target address

	vm_value operand = vm->m_stk.pop_value();
	vm_value addr = vm->m_stk.pop_value();
	if (type == AEP_PTR)
	{
		memcpy(addr.ptr, &operand.ptr, 4);
		//printf("OP_SET ptr [%x now %x]\n", addr.ptr, operand.ptr);
	}
	else
	{
		memcpy(addr.ptr, &operand.i32, 4);
		//printf("OP_SET [%x now %d]\n", addr.ptr, operand.i32);
	}
}

inline static void DoLoad(aeVM* vm, int addressMode, int offset, int kind)
{
	void* dataPtr = nullptr;

	if (addressMode == AEK_THIS)
	{
		vm_value thisPtr = vm->m_stk.pop_value();
		dataPtr = static_cast<unsigned char*>(thisPtr.ptr) + offset;
		//printf("LOAD FROM POPPED THIS|\n");
	}
	else if (addressMode == AEK_EBP)
	{
		dataPtr = vm->m_stk.ebp - offset;
	}
	else if (addressMode == AEK_ESP)
	{
		
	}

	if (dataPtr)
	{
		if (kind == AEP_PTR)
		{
			vm_value v;
			memcpy(&v.ptr, dataPtr, sizeof(void*));
			vm->m_stk.push_value(v);
			//printf("Loading ptr %x\n", v.ptr);
		}
		else
		{
			vm_value v;
			v.i32 = *static_cast<int32_t*>(dataPtr);
			vm->m_stk.push_value(v);
			//printf("Loading %d\n", v.i32);
		}
	}
}

inline static void DoLoadAddr(aeVM* vm, int addressMode, int offset, int x)
{
	if (addressMode == AEK_THIS)
	{
		vm_value thisPtr = vm->m_stk.pop_value();
		thisPtr.ptr = (unsigned char*)thisPtr.ptr + offset;
		vm->m_stk.push_value(thisPtr);
		printf("OP_LOADADDR THIS = %x (offset %d)\n", vm->m_stk.getThisPtr().ptr, offset);
	}
	else if (addressMode == AEK_EBP)
	{
		vm_value val;
		val.ptr = vm->m_stk.ebp - offset;
		vm->m_stk.push_value(val);
		//printf("OP_LOADADDR EBP address %x (offset %d)\n", val.ptr, offset);
	}
	else if (addressMode == AEK_ESP)
	{
		vm_value v;
		v.ptr = vm->m_stk.esp;
		vm->m_stk.push_value(v);
		//printf("OP_LOAD ESP address %x\n", v.ptr);
	}
}

inline static void DoLoadConstant(aeVM* vm, int primType, int index, int x)
{
	vm_value kVal;
	if (primType == AEK_FLOAT)
	{
		kVal.fp = vm->m_ctx->m_floatTable[index];
		//printf("OP_LOADK FLOAT %f constant\n", kVal.fp);
	}
	else if (primType == AEK_INT)
	{
		kVal.i32 = vm->m_ctx->int_literals[index];
		//printf("OP_LOADK INT32 %d constant\n", kVal.i32);
	}
	else if (primType == AEK_STRING)
	{
		kVal.i32 = index;
		//printf("OP_LOADK string %d constant\n", kVal.i32);
	}
	vm->m_stk.push_value(kVal);
}

inline static void DoLogicalNot(aeVM* vm)
{
	vm_value a = vm->m_stk.pop_value();
	a.i64 = !a.i64;
	vm->m_stk.push_value(a);
}

inline static void DoNewObject(aeVM* vm, int type)
{
	vm_value obj;
	obj.ptr = nullptr;
	vm->m_stk.push_value(obj);
}

inline static void DoLessThan(aeVM* vm, AeonPrimitiveType ptype)
{
	vm_value b = vm->m_stk.pop_value();
	vm_value a = vm->m_stk.pop_value();
	//printf("LT OP %d < %d = %d\n", a.i32, b.i32, a.i32 < b.i32);

	switch (ptype)
	{
	case AEP_DOUBLE: a.dp = a.dp < b.dp; break;
	case AEP_FLOAT: a.fp = a.fp < b.fp; break;
	case AEP_UINT8: a.u8 = a.u8 < b.u8; break;
	case AEP_INT8: a.i8 = a.i8 < b.i8; break;
	case AEP_UINT16: a.u16 = a.u16 < b.u16; break;
	case AEP_INT16: a.i16 = a.i16 < b.i16; break;
	case AEP_UINT32: a.u32 = a.u32 < b.u32; break;
	case AEP_INT32: a.i32 = (bool)(a.i32 < b.i32); break;
	case AEP_UINT64: a.u64 = a.u64 < b.u64; break;
	case AEP_INT64: a.i64 = a.i64 < b.i64; break;
	}

	vm->m_stk.push_value(a);
}

inline static void DoGreaterThan(aeVM* vm, AeonPrimitiveType ptype)
{
	vm_value a = vm->m_stk.pop_value();
	vm_value b = vm->m_stk.pop_value();

	switch (ptype)
	{
	case AEP_DOUBLE: a.dp = a.dp > b.dp; break;
	case AEP_FLOAT: a.fp = a.fp > b.fp; break;
	case AEP_UINT8: a.u8 = a.u8 > b.u8; break;
	case AEP_INT8: a.i8 = a.i8 > b.i8; break;
	case AEP_UINT16: a.u16 = a.u16 > b.u16; break;
	case AEP_INT16: a.i16 = a.i16 > b.i16; break;
	case AEP_UINT32: a.u32 = a.u32 > b.u32; break;
	case AEP_INT32: a.i32 = a.i32 > b.i32; break;
	case AEP_UINT64: a.u64 = a.u64 > b.u64; break;
	case AEP_INT64: a.i64 = a.i64 > b.i64; break;
	}

	vm->m_stk.push_value(a);
}

static inline void DoJumpIfZero(aeVM* vm, int jumpOffset)
{
	aeStackFrame* cl = &vm->m_stk.frames[vm->m_stk.frames.size() - 1];

	int value = vm->m_stk.pop_value().i32;
	if (value == 0)
	{
		cl->pc += jumpOffset;
		//Log("the if condition was false. eax %d", eax);
	}
	else
	{
		//Log("the if condition was true. eax %d", eax);
	}
	//Log("[jz] test %d %d", cond, pcoffset);
}

static inline void DoJump(aeVM* vm, int address)
{
	aeStackFrame* cl = &vm->m_stk.frames[vm->m_stk.frames.size() - 1];
	cl->pc = address;
}

static inline bool DoReturn(aeVM* vm)
{
	vm->m_stk.frames.pop_back();
	if (vm->m_stk.frames.size() == 0)
	{
		return true;
	}
	else
	{
		if (vm->m_stk.ebp != vm->m_stk.esp)
			printf("Script function returned without popping all stack memory!\n");
		else
			printf("All memory is as expected\n");

		// restore underlying function
		vm->m_stk.cl = &vm->m_stk.frames[vm->m_stk.frames.size() - 1];
		vm->m_stk.ebp = vm->m_stk.cl->ebp;
	}

	return false;
}

void aeVM::execute(aeThreadState& threadInfo)
{
	threadInfo.cl = &m_stk.frames[m_stk.frames.size() - 1];

	for (; threadInfo.cl->pc < threadInfo.cl->module->instructions.size(); ++threadInfo.cl->pc)
	{
		AEInstruction& inst = threadInfo.cl->module->instructions[threadInfo.cl->pc];
		switch (inst.opcode)
		{
			vm_start(OP_PREPARE)
				AEFunction* functionData = m_ctx->m_functionTable[inst.arg0];
				m_stk.esp -= functionData->returnValueSize;
			vm_end

			vm_start(OP_CALL)
				DoCall(this, inst.arg0);
			vm_end

			vm_start(OP_DCALL)
				DoDynamicCall(this, inst.arg0);
			vm_end

				vm_start(OP_CALLVIRTUAL)
				// todo
				vm_end

				vm_start(OP_CALLNATIVE)
				/*uint32_t funcid = getinst_a(inst);
				uint32_t params = getinst_b(inst);

				atom_generic* gen = new atom_generic();
				gen->vm = this;
				// forward to generic
				gen->args.resize(params);
				for (int k = 0; k < params; ++k)
				gen->args[k]._u64 = pop();
				//Log("Stored %d in the generic before calling", params);

				// C Call
				ctx->native_functions[funcid].f(gen);

				// always push the return type
				if (gen->hasRet)
				push(gen->retarg._u64);

				delete gen;*/
			vm_end

			vm_start(OP_CALLMETHOD_NAT)
				AEGeneric g; g.m_vm = this;
				m_ctx->m_functionTable[inst.arg0]->fn(g);
			vm_end

			vm_start(OP_RETURN)
				if (DoReturn(this))
					return;
			vm_end

			vm_start(OP_JZ)
				DoJumpIfZero(this, inst.arg0);
			vm_end

			vm_start(OP_JMP)
				DoJump(this, inst.arg0);
			vm_end

			vm_start(OP_LOAD)
				DoLoad(this, inst.arg0, inst.arg1, inst.arg2);
			vm_end

			vm_start(OP_LOADADDR)
				DoLoadAddr(this, inst.arg0, inst.arg1, inst.arg2);
			vm_end

			vm_start(OP_LOADK)
				DoLoadConstant(this, inst.arg0, inst.arg1, inst.arg2);				
			vm_end

			vm_start(OP_LOADENUM)
				vm_value v;
				v.i32 = inst.arg0;
				m_stk.push_value(v);
			vm_end

			vm_start(OP_LT)
				DoLessThan(this, (AeonPrimitiveType)inst.arg0);
			vm_end

			vm_start(OP_LTE)
				
			vm_end

			vm_start(OP_GT)
				DoGreaterThan(this, (AeonPrimitiveType)inst.arg0);
			vm_end

			vm_start(OP_GTE)
				
			vm_end

			vm_start(OP_EQ)
				DoEquals(this, (AeonPrimitiveType)inst.arg0);
			vm_end

			vm_start(OP_NEQ)
				DoNotEquals(this, (AeonPrimitiveType)inst.arg0);
			vm_end

			vm_start(OP_NOT)
				DoLogicalNot(this);
			vm_end

			vm_start(OP_SET)
				DoAssign(this, inst.arg0, inst.arg1, inst.arg2);
			vm_end

			vm_start(OP_MUL)
				DoMul(this, (AeonPrimitiveType)inst.arg0);
			vm_end

			vm_start(OP_DIV)
				DoDiv(this, (AeonPrimitiveType)inst.arg0);
			vm_end

			vm_start(OP_MOD)
				DoMod(this, (AeonPrimitiveType)inst.arg0);
			vm_end

			vm_start(OP_ADD)
				DoAdd(this, (AeonPrimitiveType)inst.arg0);
			vm_end

			vm_start(OP_SUB)
				DoSub(this, (AeonPrimitiveType)inst.arg0);
			vm_end

			vm_start(OP_CONV)
				DoConversion(this, (AeonPrimitiveType)inst.arg0, (AeonPrimitiveType)inst.arg1);
			vm_end

			vm_start(OP_PUSHTHIS)
				vm_value v = m_stk.getThisPtr();
				m_stk.push_value(v);
				vm_end

					vm_start(OP_SIZEOF)

					vm_end

			vm_start(OP_TYPEINFO)
				int index = inst.arg0;
				m_stk.push_value(vm_value::make_ptr(m_ctx->typedb[index]));
				printf("LOADED TYPE INFO %s\n", m_ctx->typedb[index]->getName().c_str());
			vm_end

			vm_start(OP_NEWOBJECT)
				DoNewObject(this, inst.arg0);				
			vm_end

			vm_start(OP_DELETEOBJECT)
				
			vm_end

			vm_start(OP_MOV)
				m_stk.esp += inst.arg1;
			vm_end

			vm_start(OP_BREAKPOINT)
				__asm int 3;
			vm_end

			vm_start(OP_DTEST)
				printf("TEST VALUE %d\n", inst.arg0);
			vm_end

			vm_start(OP_DEBUG)
				vm_log("DEBUG", 0, m_ctx->string_literals[inst.arg1].c_str());
			vm_end

		} // end of switch
	}
}