#ifndef aeon_type_h__
#define aeon_type_h__

#include "aeon_generics.h"
#include <vector>
#include <string>
#include <stdint.h>

/**
	\class aeon_type
	\brief Each script class is represented by one of these
*/
class aeon_type
{
	public:
		std::string name;
		int id;
		uint32_t size;
		
		struct MethodInfo
		{

		};

		struct FieldInfo
		{
			uint16_t offset;
			uint16_t size;
		};

		/// Every callable method in the type, indexable directly.
		std::vector<MethodInfo> methods;
		std::vector<FieldInfo>  fields;

	public:

		aeon_type()
		{

		}

		aeon_type(const std::string& _name, uint32_t _size)
		{
			name = _name;
			size = _size;
		}
};

#endif // aeon_type_h__
