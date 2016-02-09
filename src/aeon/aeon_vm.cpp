#include "aeon_vm.h"
#include "aeon_bytecode.h"
#include "aeon_context.h"
#include "aeon_object.h"

#define vm_start(x) case x:{
#define vm_end break;}

#define LOGVM(x, a, b, c) printf("[vm] %s %d %d %d\n", #x, a, b, c);


aeon_vm::aeon_vm()
{
	
}

aeon_module* aeon_vm::get_current_mod()
{
	return m_stk.frames[m_stk.frames.size() - 1].module;
}

void aeon_vm::callMethod(aeon_object* object, const std::string& prototype)
{
	m_stk.ebp = m_stk.esp;

	m_stk.pushThisPtr(object->addr);
	  
	printf("callMethod: %s. pushing this, pointing to %x popped\n", prototype.c_str(), object->addr);

	call(*object->getType()->getModule(), prototype.c_str());
} 

void aeon_vm::callMethod(aeon_object* object, uint32_t methodId)
{

}

void aeon_vm::prepare(aeFunctionId function)
{

}

void aeon_vm::pushThis(void* obj)
{
	m_stk.push_addr(obj);
}

void aeon_vm::setContext(aeon_context* context)
{
	ctx = context;
} 

void aeon_vm::setArgFloat(uint32_t index, float v)
{
	*(m_stk.esp - sizeof(aeDynamicType)* index) = v;
}

float aeon_vm::getArgFloat(uint32_t index)
{
	return *(m_stk.esp - index * sizeof(aeDynamicType));
}

void aeon_vm::setArg(uint32_t index, aeDynamicType v)
{
	uint64_t* argAddress = reinterpret_cast<uint64_t*>((m_stk.esp - sizeof(uint64_t)* index));
	*argAddress = v._u64;
}

aeDynamicType aeon_vm::getArg(uint32_t index)
{
	uint64_t* argAddress = reinterpret_cast<uint64_t*>((m_stk.esp - sizeof(uint64_t)* index));
	aeDynamicType v;
	v._u64 = *argAddress;
	return v;
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

void aeon_vm::push(uint64_t n)
{
	m_stk.esp -= 8;
	memcpy(m_stk.esp, &n, sizeof(n));
	//Log("Pushed to stack: %d", n);
}

void aeon_vm::push_float(float value)
{
	m_stk.esp -= sizeof(value);
	memcpy(m_stk.esp, &value, sizeof(value));
}

void aeon_vm::push_double(double value)
{
	m_stk.esp -= sizeof(value);
	memcpy(m_stk.esp, &value, sizeof(value));
}

void aeon_vm::push_int32(int32_t value)
{
	m_stk.esp -= sizeof(value);
	memcpy(m_stk.esp, &value, sizeof(value));
}

void aeon_vm::push_bytes(uint32_t bytes)
{
	m_stk.esp -= bytes;
}

void aeon_vm::push_objectref(atom_objectref ref)
{
	m_stk.esp -= sizeof(ref);
	memcpy(m_stk.esp, &ref, sizeof(ref));
}

uint64_t aeon_vm::pop()
{
	uint64_t stacktop;
	memcpy(&stacktop, m_stk.esp, sizeof(uint64_t));
	m_stk.esp += 8;
	return stacktop;
}

float aeon_vm::pop_float()
{
	float value;
	memcpy(&value, m_stk.esp, sizeof(float));
	m_stk.esp += sizeof(float);
	return value;
}

double aeon_vm::pop_double()
{
	double value;
	memcpy(&value, m_stk.esp, sizeof(double));
	m_stk.esp += sizeof(double);
	return value;
}

int32_t aeon_vm::pop_int32()
{
	int32_t value;
	memcpy(&value, m_stk.esp, sizeof(int32_t));
	m_stk.esp += sizeof(int32_t);
	return value;
}

void aeon_vm::call(aeon_module& module, const char* func)
{
	aeStackFrame callinfo;
	callinfo.name = func;
	callinfo.object = nullptr;

	aeFunctionId functionId = 0;

	aeFunction* function = ctx->getFunctionByName(func);
	int i = 0;

	if (function)
	{
		if (!function->m_compiled)
		{
			printf("This function is not compiled '%s'.\n", function->getSymbolName().c_str());
			//return;
		}

		prepare(functionId);


		callinfo.pc = function->offset;
		callinfo.module = &module;
		callinfo.ebp = m_stk.esp;
		m_stk.frames.push_back(callinfo);

		// Launch the thread from this entry point
		execute(m_stk);
	}
	else
	{
		printf("Couldn't find the calling function\n");
	}
}

inline static void DoAdd(aeon_vm* vm, AeonPrimitiveType ptype)
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

inline static void DoSub(aeon_vm* vm, AeonPrimitiveType ptype)
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

inline static void DoMul(aeon_vm* vm, AeonPrimitiveType ptype)
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

inline static void DoDiv(aeon_vm* vm, AeonPrimitiveType ptype)
{
	vm_value b = vm->m_stk.pop_value();
	vm_value a = vm->m_stk.pop_value();

	printf("DIV %d %d\n", a.i32, b.i32);

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

inline static void DoMod(aeon_vm* vm, AeonPrimitiveType ptype)
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

inline static void DoConversion(aeon_vm* vm, AeonPrimitiveType t1, AeonPrimitiveType t2)
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

inline static void DoEquals(aeon_vm* vm, AeonPrimitiveType ptype)
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

inline static void DoNotEquals(aeon_vm* vm, AeonPrimitiveType ptype)
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

inline static void DoAssign(aeon_vm* vm, int mode, int offset, int type)
{
	// Assign modes:
	// pod_copy - simply copy N bytes from the loaded value into the target address

	vm_value operand = vm->m_stk.pop_value();
	vm_value addr = vm->m_stk.pop_value();
	if (type == AEP_PTR)
	{
		memcpy(addr.ptr, &operand.ptr, 4);
		printf("OP_SET ptr [%x now %x]\n", addr.ptr, operand.ptr);
	}
	else
	{
		memcpy(addr.ptr, &operand.i32, 4);
		printf("OP_SET [%x now %d]\n", addr.ptr, operand.i32);
	}
}

inline static void DoLoad(aeon_vm* vm, int addressMode, int offset, int kind)
{
	void* dataPtr = nullptr;
	if (addressMode == AEK_THIS)
	{
		
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
			printf("Loading ptr %x\n", v.ptr);
		}
		else
		{
			vm_value v;
			v.i32 = *static_cast<int32_t*>(dataPtr);
			vm->m_stk.push_value(v);
			printf("Loading %d\n", v.i32);
		}
	}
}

inline static void DoLoadAddr(aeon_vm* vm, int addressMode, int offset, int x)
{
	if (addressMode == AEK_THIS)
	{
		vm_value thisPtr = vm->m_stk.getThisPtr();
		thisPtr.ptr = (unsigned char*)thisPtr.ptr + offset;
		/*
		vm_value offsetedAddr = *thisObjAddr;
		offsetedAddr.ptr = (char*)offsetedAddr.ptr + inst.arg1;*/
		vm->m_stk.push_value(thisPtr);
		printf("OP_LOAD THIS = %x (offset %d)\n", vm->m_stk.getThisPtr().ptr, offset);
	}
	else if (addressMode == AEK_EBP)
	{
		vm_value val;
		val.ptr = vm->m_stk.ebp - offset;
		vm->m_stk.push_value(val);
		printf("OP_LOADADDR EBP address %x (offset %d)\n", val.ptr, offset);
	}
	else if (addressMode == AEK_ESP)
	{
		vm_value v;
		v.ptr = vm->m_stk.esp;
		vm->m_stk.push_value(v);
		printf("OP_LOAD ESP address %x\n", v.ptr);
	}
}

inline static void DoLoadConstant(aeon_vm* vm, int primType, int index, int x)
{
	vm_value kVal;
	if (primType == AEK_FLOAT)
	{
		kVal.fp = vm->ctx->m_floatTable[index];
		printf("OP_LOADK FLOAT %f constant\n", kVal.fp);
	}
	else if (primType == AEK_INT)
	{
		kVal.i32 = vm->ctx->int_literals[index];
		printf("OP_LOADK INT32 %d constant\n", kVal.i32);
	}
	else if (primType == AEK_STRING)
	{
		kVal.i32 = index;
		printf("OP_LOADK string %d constant\n", kVal.i32);
	}
	vm->m_stk.push_value(kVal);
}

inline static void DoLogicalNot(aeon_vm* vm)
{
	vm_value a = vm->m_stk.pop_value();
	a.i64 = !a.i64;
	vm->m_stk.push_value(a);
}

inline static void DoNewObject(aeon_vm* vm, int type)
{
	vm_value obj;
	obj.ptr = nullptr;
	vm->m_stk.push_value(obj);
}

inline static void DoLessThan(aeon_vm* vm, AeonPrimitiveType ptype)
{
	vm_value a = vm->m_stk.pop_value();
	vm_value b = vm->m_stk.pop_value();
	 

	switch (ptype)
	{
	case AEP_DOUBLE: a.dp = a.dp < b.dp; break;
	case AEP_FLOAT: a.fp = a.fp < b.fp; break;
	case AEP_UINT8: a.u8 = a.u8 < b.u8; break;
	case AEP_INT8: a.i8 = a.i8 < b.i8; break;
	case AEP_UINT16: a.u16 = a.u16 < b.u16; break;
	case AEP_INT16: a.i16 = a.i16 < b.i16; break;
	case AEP_UINT32: a.u32 = a.u32 < b.u32; break;
	case AEP_INT32: a.i32 = a.i32 < b.i32; break;
	case AEP_UINT64: a.u64 = a.u64 < b.u64; break;
	case AEP_INT64: a.i64 = a.i64 < b.i64; break;
	}

	vm->m_stk.push_value(a);
}

inline static void DoGreaterThan(aeon_vm* vm, AeonPrimitiveType ptype)
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

void aeon_vm::execute(const aeThreadState& thread)
{
	aeStackFrame* cl = &m_stk.frames[m_stk.frames.size() - 1];

	for (; cl->pc < cl->module->instructions.size(); ++cl->pc)
	{
		aeon_instruction& inst = cl->module->instructions[cl->pc];
		switch (inst.opcode)
		{
			vm_start(OP_PREPARE)
				aeFunction* functionData = ctx->m_functionTable[inst.arg0];
				m_stk.esp -= functionData->returnValueSize;
			vm_end

			vm_start(OP_CALL)
				aeFunction* functionData = ctx->m_functionTable[inst.arg0];
				
				aeStackFrame callinfo;
				callinfo.name = "unknown";
				callinfo.pc = functionData->offset - 1;
				callinfo.module = functionData->m_module;
				callinfo.ebp = m_stk.ebp;
				callinfo.function = functionData;
				m_stk.frames.push_back(callinfo);
				cl = &m_stk.frames[m_stk.frames.size() - 1];
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
				ctx->m_functionTable[inst.arg0]->fn(this);
			vm_end

			vm_start(OP_RETURN)
				m_stk.frames.pop_back();
				if (m_stk.frames.size() == 0)
				{
					return;
				}
				else
				{
					if (m_stk.ebp != m_stk.esp)
						printf("Script function returned without popping all stack memory!\n");
					else
						printf("All memory is as expected\n");

					// restore underlying function
					cl = &m_stk.frames[m_stk.frames.size() - 1];
					m_stk.ebp = cl->ebp;
				}
			vm_end

			vm_start(OP_JZ)
				int pcoffset = getinst_a(inst);
				int cond = pop();
				if (cond == 0)
				{
					 cl->pc += pcoffset;
					 //Log("the if condition was false. eax %d", eax);
				}
				else
				{
					 //Log("the if condition was true. eax %d", eax);
				}
				//Log("[jz] test %d %d", cond, pcoffset);
			vm_end

			vm_start(OP_JMP)
				int newpc = getinst_a(inst);
				cl->pc = newpc;
			vm_end

			vm_start(OP_PUSHARG)
				uint32_t argIndex = getinst_a(inst);
				uint32_t argType = getinst_b(inst);
				// uint64_t argValue = cl->module->instructions[++cl->pc];
				uint64_t argValue = 0;
				aeDynamicType  argVariant; argVariant._u64 = argValue; argVariant.type = argType;

				setArg(argIndex, argVariant);
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

			vm_start(OP_LT)
				DoLessThan(this, (AeonPrimitiveType)inst.arg0);
			vm_end

			vm_start(OP_LTE)
				int opb = pop();
				int opa = pop();
				push(static_cast<int>(opa < opb));
				//Log("[lt] %d < %d = %d", opa, opb, static_cast<int>(opa < opb));
			vm_end

			vm_start(OP_GT)
				DoGreaterThan(this, (AeonPrimitiveType)inst.arg0);
			vm_end

			vm_start(OP_GTE)
				int opb = pop();
				int opa = pop();
				push(static_cast<int>(opa > opb));
				// Log("[gt] %d > %d = %d", opa, opb, static_cast<int>(opa < opb));
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
				uint32_t type_token = getinst_a(inst);
				push(ctx->typedb[type_token]->getSize());
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
				printf("DEBUG: %s\n", ctx->string_literals[inst.arg1].c_str());
			vm_end

		} // end of switch
	}
}