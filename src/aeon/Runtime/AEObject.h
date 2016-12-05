#ifndef AEOBJECT_H__
#define AEOBJECT_H__

#include <vector>
#include <string>
#include <map>
#include <stdint.h>

#include <Rzr/RzValue.h>

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
	AEObject();

	RzValue* getValueRef(const std::string& name);
	
	void setProperty(const std::string& name, const std::string& value);

	void setProperty(const std::string& name, RzValue value);

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
	friend class RzVirtualMachine;
	friend class RzEngine;

	std::vector<std::string> m_names;
	std::vector<RzValue> m_properties;

public:
	void*   m_obj;
	AEType* m_type;
	int     m_ref;
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

	void setValue(const std::string& name, const RzValue& value);

private:
	std::map<std::string, RzValue> m_properties;
};

#endif // AEOBJECT_H__
