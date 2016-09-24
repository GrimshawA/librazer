#ifndef AEOBJECT_H__
#define AEOBJECT_H__

#include <vector>
#include <string>
#include <stdint.h>

#include <AEON/Runtime/AEValue.h>

class AEVm;
class AEType;

/**
	\class AEObject
	\brief Wraps a statically typed object

	This object is used to represent a native or script language object at runtime.
*/
class AEObject
{
public:

	void setProperty(const std::string& name, const std::string& value);

	void setProperty(const std::string& name, AEValue value);

	void call(const std::string& name);

	int count() const;

	std::string propertyName(int index);

	AEType* getType();

	void setField(const std::string& field, int32_t value);

	void getField(const std::string& field, int32_t& value) const;

	void* getFieldAddress(const std::string& field) const;

	void log();

	bool contains(const std::string& key) const{
		for (auto& m : m_names)
		{
			if (m == key)
				return true;
		}
		return false;
	}

	//private:
	friend class AEVirtualMachine;
	friend class AEContext;

	std::vector<std::string> m_names;
	std::vector<AEValue> m_properties;
	void* m_obj;
	AEType* m_type;
};

/**
	\class AEDynamicObject
	\brief Typeless object that can take arbitrary properties

	In the language type system, one can use the dynamic type for storing any value.
	It can hold actual typed objects as well as anonymous objects that are fully prototype based.
*/
class AEDynamicObject
{
public:


private:
	std::vector<std::string> m_names;
	std::vector<AEValue> m_properties;
};

#endif // AEOBJECT_H__
