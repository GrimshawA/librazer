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

#endif
