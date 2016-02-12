#ifndef aeon_object_h__
#define aeon_object_h__

#include <vector>
#include <string>
#include <stdint.h>

class aeType;

/**
	\class aeon_object
	\brief Instances of script classes

	Since at native compile time, we cannot know the specific script side types,
	we need to use an interface to represent and interact them.
	
	How big an aeon_object is, is highly dependent on the size of the
	aeon type and its features.

	This type can represent any type available to scripts, it acts as an interface to interact
	with that object's memory. The address of these objects is NOT the same as the objects themselves.
	It is just a wrapper.
*/
class aeon_object
{
	public:
		void* addr;
		aeType* m_type;

public:
		aeType* getType();

		void setField(const std::string& field, int32_t value);

		void getField(const std::string& field, int32_t& value) const;

		void* getFieldAddress(const std::string& field) const;

		void log();
};

#endif // aeon_object_h__
