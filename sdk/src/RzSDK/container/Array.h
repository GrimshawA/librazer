#ifndef ARRAY_H__
#define ARRAY_H__

#include <RazerRuntime/RzValue.h>
#include <vector>

class VariantArray
{
public:

	static void registerApi(RzModule* mod);

	void push(const RzValue& value);
	void pop();
	void clear();
	RzValue at(int index);

	std::vector<RzValue> m_data;
};

/*
 * Initial array implementation for the core language,
 * used as type [] or array<type>
 *
 * This is templated from within the lang!
 */
class ArrayT
{
public:

    static void registerApi(RzModule* mod);

public:
    explicit ArrayT(int typeSize);

    void clear();
    std::size_t size();
    void resize(int size);
    void push(void* obj);
    void remove(void* obj);
    void* at(int index);

private:
    using object_t = void*;
    std::vector<object_t> m_data;
    std::size_t           m_typeSize;
};

#endif
