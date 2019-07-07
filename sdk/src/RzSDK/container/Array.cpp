#include <RzSDK/container/Array.h>
#include <RazerRuntime/Module.h>

#include <iostream>

void VariantArray::registerApi(RzModule* m)
{
	m->registerType("array", sizeof(VariantArray));
	m->registerTypeConstructor("array", [](void* memory, RzVirtualMachine* vm)
	{
		new (memory) VariantArray();
	});

	m->registerMethod("array", "void push(var)", [](RzGeneric g)
	{
		VariantArray* obj = (VariantArray*)g.popObject();
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

//////////////////////////////////

void ArrayT::registerApi(RzModule* m)
{
    m->registerType("array", sizeof(ArrayT));

    m->registerTypeConstructor("array", [](void* memory, RzVirtualMachine* vm, int objSize)
    {
        new (memory) ArrayT(objSize);
    });

    m->registerMethod("array", "void push(var)", [](RzGeneric g)
    {
        VariantArray* obj = (VariantArray*)g.popObject();
        RzValue var;
        obj->push(var);
    });

    m->registerMethod("array", "void resize(int32)", [](RzGeneric g)
    {
        ArrayT* obj = (ArrayT*)g.popObject();
        obj->resize(g.popInt32());
    });

    m->registerMethod("array", "int32 size()", [](RzGeneric g)
    {
        ArrayT* obj = (ArrayT*)g.popObject();
        std::size_t size = obj->size();
        g.pushInt32(size);
    });
}

ArrayT::ArrayT(int typeSize)
{
    m_typeSize = typeSize;
    std::cout << "ArrayT() "<<this << "," << typeSize << std::endl;
}

void ArrayT::clear()
{
    m_data.clear();
}

std::size_t ArrayT::size()
{
    std::cout << "ArrayT.:size() "<<this << std::endl;
    return m_data.size();
}

void ArrayT::resize(int size)
{
    if (m_data.size() == size)
        return;

    std::cout << "ArrayT::resize() "<<this << "," << size << std::endl;
    m_data.resize(size);
}

void ArrayT::push(void* obj)
{
    m_data.push_back({});
}

void ArrayT::remove(void* obj)
{

}
