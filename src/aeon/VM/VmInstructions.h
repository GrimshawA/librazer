
inline static void DoAdd(AEVirtualMachine* vm, AeonPrimitiveType ptype)
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

inline static void DoSub(AEVirtualMachine* vm, AeonPrimitiveType ptype)
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

inline static void DoMul(AEVirtualMachine* vm, AeonPrimitiveType ptype)
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

inline static void DoDiv(AEVirtualMachine* vm, AeonPrimitiveType ptype)
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

inline static void DoMod(AEVirtualMachine* vm, AeonPrimitiveType ptype)
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

inline static void DoConversion(AEVirtualMachine* vm, AeonPrimitiveType t1, AeonPrimitiveType t2)
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

inline static void DoEquals(AEVirtualMachine* vm, AeonPrimitiveType ptype)
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

inline static void DoNotEquals(AEVirtualMachine* vm, AeonPrimitiveType ptype)
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

inline static void DoAssign(AEVirtualMachine* vm, int mode, int offset, int type)
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

inline static void DoLoad(AEVirtualMachine* vm, int addressMode, int offset, int kind)
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

inline static void DoLoadAddr(AEVirtualMachine* vm, int addressMode, int offset, int x)
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

inline static void DoLoadConstant(AEVirtualMachine* vm, int primType, int index, int x)
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

inline static void DoLogicalNot(AEVirtualMachine* vm)
{
	vm_value a = vm->m_stk.pop_value();
	a.i64 = !a.i64;
	vm->m_stk.push_value(a);
}

inline static void DoNewObject(AEVirtualMachine* vm, int module_id, int type)
{
	void* objectMem = malloc(vm->get_current_mod()->m_types[type]->getSize());

	vm_value obj;
	obj.ptr = objectMem;
	vm->m_stk.push_value(obj);
}

inline static void DoLessThan(AEVirtualMachine* vm, AeonPrimitiveType ptype)
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

inline static void DoGreaterThan(AEVirtualMachine* vm, AeonPrimitiveType ptype)
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

static inline void DoJumpIfZero(AEVirtualMachine* vm, int jumpOffset)
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

static inline void DoJump(AEVirtualMachine* vm, int address)
{
	aeStackFrame* cl = &vm->m_stk.frames[vm->m_stk.frames.size() - 1];
	cl->pc = address;
}

static inline bool DoReturn(AEVirtualMachine* vm)
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
