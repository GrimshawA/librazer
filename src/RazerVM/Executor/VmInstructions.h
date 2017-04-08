
inline static void DoAdd(RzThreadContext& ctx, AeonPrimitiveType ptype)
{
	RzStackValue a = ctx.pop_value();
	RzStackValue b = ctx.pop_value();

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

	ctx.push_value(a);
}

inline static void DoSub(RzThreadContext& ctx, AeonPrimitiveType ptype)
{
	RzStackValue b = ctx.pop_value();
	RzStackValue a = ctx.pop_value();

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

	ctx.push_value(a);
}

inline static void DoMul(RzThreadContext& ctx, AeonPrimitiveType ptype)
{
	RzStackValue a = ctx.pop_value();
	RzStackValue b = ctx.pop_value();

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

	ctx.push_value(a);
}

inline static void DoDiv(RzThreadContext& ctx, AeonPrimitiveType ptype)
{
	RzStackValue b = ctx.pop_value();
	RzStackValue a = ctx.pop_value();

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

	ctx.push_value(a);
}

inline static void DoMod(RzThreadContext& ctx, AeonPrimitiveType ptype)
{
	RzStackValue a = ctx.pop_value();
	RzStackValue b = ctx.pop_value();

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

	ctx.push_value(a);
}

inline static void DoConversion(RzThreadContext& ctx, AeonPrimitiveType t1, AeonPrimitiveType t2)
{
	RzStackValue v = ctx.pop_value();

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

	ctx.push_value(v);
}

inline static void DoEquals(RzThreadContext& ctx, AeonPrimitiveType ptype)
{
	RzStackValue a = ctx.pop_value();
	RzStackValue b = ctx.pop_value();

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

	ctx.push_value(a);
}

inline static void DoNotEquals(RzThreadContext& ctx, AeonPrimitiveType ptype)
{
	RzStackValue a = ctx.pop_value();
	RzStackValue b = ctx.pop_value();

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

	ctx.push_value(a);
}

inline static void DoAssign(RzThreadContext& ctx, int mode, int offset, int type)
{
	// Assign modes:
	// pod_copy - simply copy N bytes from the loaded value into the target address

	RzStackValue operand = ctx.pop_value();
    RzStackValue dest_addr = ctx.pop_value();
	if (type == AEP_PTR)
	{
        RZLOG("Copied ptr %x to %x\n", operand.ptr, dest_addr.ptr);
        memcpy(dest_addr.ptr, &operand.ptr, 4);
	}
	else
	{
        memcpy(dest_addr.ptr, &operand.i32, 4);
	}
}

inline static void DoLoad(RzThreadContext& ctx, int addressMode, int offset, int kind)
{
	void* dataPtr = nullptr;

	if (addressMode == AEK_THIS)
	{
		RzStackValue thisPtr = ctx.pop_value();
		dataPtr = static_cast<unsigned char*>(thisPtr.ptr) + offset;
	}
	else if (addressMode == AEK_EBP)
	{
		dataPtr = ctx.ebp - offset;
	}
	else if (addressMode == AEK_ESP)
	{

	}

	if (dataPtr)
	{
		if (kind == AEP_PTR)
		{
			RzStackValue v;
			memcpy(&v.ptr, dataPtr, sizeof(void*));
			ctx.push_value(v);
		}
		else
		{
			RzStackValue v;
			v.i32 = *static_cast<int32_t*>(dataPtr);
			ctx.push_value(v);
		}
	}
}

inline static void DoLoadAddr(RzThreadContext& ctx, int addressMode, int offset, int x)
{
	if (addressMode == AEK_THIS)
	{
		RzStackValue thisPtr = ctx.pop_value();
		thisPtr.ptr = (unsigned char*)thisPtr.ptr + offset;
		ctx.push_value(thisPtr);
		RZLOG("OP_LOADADDR THIS = %x (offset %d)\n", ctx.getThisPtr().ptr, offset);
	}
	else if (addressMode == AEK_EBP)
	{
		RzStackValue val;
		val.ptr = ctx.ebp - offset;
		ctx.push_value(val);

        RZLOG("Loaded ebp relative address (local) %x\n", val.ptr);
	}
	else if (addressMode == AEK_ESP)
	{
		RzStackValue v;
		v.ptr = ctx.esp;
		ctx.push_value(v);
	}
}

inline static void DoLoadConstant(RzThreadContext& ctx, int primType, int index, int x)
{
	RzStackValue kVal;
	if (primType == AEK_FLOAT)
	{
		kVal.fp = ctx.engine->m_floatTable[index];
	}
	else if (primType == AEK_INT)
	{
		kVal.i32 = ctx.engine->int_literals[index];
	}
	else if (primType == AEK_STRING)
	{
		kVal.i32 = index;
	}
	ctx.push_value(kVal);
}

inline static void DoLogicalNot(RzThreadContext& ctx)
{
	RzStackValue a = ctx.pop_value();
	a.i64 = !a.i64;
	ctx.push_value(a);
}

inline static void DoNewObject(RzThreadContext& ctx, int module_id, int type)
{
	RzType* typeInfo = ctx.getModule().resolveType(module_id, type);
	void* obj = typeInfo->construct();

	RzStackValue v;
	v.ptr = obj;
	ctx.push_value(v);

	RZLOG("Constructed a new %s\n", typeInfo->getName().c_str());
}

inline static void DoLessThan(RzThreadContext& ctx, AeonPrimitiveType ptype)
{
	RzStackValue b = ctx.pop_value();
	RzStackValue a = ctx.pop_value();

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

	ctx.push_value(a);
}

inline static void DoGreaterThan(RzThreadContext& ctx, AeonPrimitiveType ptype)
{
	RzStackValue a = ctx.pop_value();
	RzStackValue b = ctx.pop_value();

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

	ctx.push_value(a);
}

static inline void DoJumpIfZero(RzThreadContext& ctx, int jumpOffset)
{
	RzStackFrame* cl = &ctx.frames[ctx.frames.size() - 1];

	int value = ctx.pop_value().i32;
	if (value == 0)
	{
		cl->pc += jumpOffset;
		
		RZLOG("the if condition was false\n");
	}
	else
	{
		RZLOG("the if condition was true\n");
	}
}

static inline void DoJump(RzThreadContext& ctx, int address)
{
	RzStackFrame* cl = &ctx.frames[ctx.frames.size() - 1];
	cl->pc += address;

	RZLOG("Jumping to %d %d\n", cl->pc+1, cl->module->m_code[cl->pc+1].opcode);
}

static inline bool DoReturn(RzThreadContext& ctx)
{
	ctx.frames.pop_back();
	if (ctx.frames.size() == 0)
	{
		return true;
	}
	else
	{
		if (ctx.ebp != ctx.esp){
			RZLOG("Script function returned without popping all stack memory!\n");
		}			
		else
			RZLOG("All memory is as expected\n");

		// restore underlying function
		ctx.cl = &ctx.frames[ctx.frames.size() - 1];
		ctx.ebp = ctx.cl->ebp;
	}

	return false;
}
