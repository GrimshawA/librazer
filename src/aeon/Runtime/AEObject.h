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
	\brief Instances of script classes

	Since at native compile time, we cannot know the specific script side types,
	we need to use an interface to represent and interact them.
	
	How big an aeon_object is, is highly dependent on the size of the
	aeon type and its features.

	This type can represent any type available to scripts, it acts as an interface to interact
	with that object's memory. The address of these objects is NOT the same as the objects themselves.
	It is just a wrapper.
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
	void* addr;
	AEType* m_type;

};
#endif // AEOBJECT_H__
