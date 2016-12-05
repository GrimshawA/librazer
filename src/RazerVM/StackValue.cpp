#include <RazerVM/StackValue.h>

RzStackValue RzStackValue::from_int32(int32_t v)
{
	RzStackValue vmv; vmv.i32 = v; return vmv;
}

RzStackValue RzStackValue::make_uint32(uint32_t v)
{
	RzStackValue vmv; vmv.u32 = v; return vmv;
}

RzStackValue RzStackValue::make_float(float v)
{
	RzStackValue vmv; vmv.fp = v; return vmv;
}

RzStackValue RzStackValue::make_double(double v)
{
	RzStackValue vmv; vmv.dp = v; return vmv;
}

RzStackValue RzStackValue::make_ptr(void* v)
{
	RzStackValue vmv; vmv.ptr = v; return vmv;
}