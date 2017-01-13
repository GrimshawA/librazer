#include <RazerCore/container/Array.h>
#include <Rzr/RzModule.h>

void VariantArray::registerApi(RzModule* m)
{
	m->registerType("array", sizeof(VariantArray));
	m->registerTypeConstructor("array", [](void* memory, RzVirtualMachine* vm)
	{
		new (memory) VariantArray();
	});

	m->registerMethod("array", "void push(var)", [](AEGeneric g)
	{
		VariantArray* obj = (VariantArray*)g.unpack_ptr();
		RzValue var;
		obj->push(var);
	});
}

void VariantArray::push(const RzValue& value)
{
	m_data.push_back(value);
}

void VariantArray::pop()
{
	m_data.pop_back();
}

void VariantArray::clear()
{
	m_data.clear();
}

RzValue VariantArray::at(int index)
{
	return m_data[index];
}