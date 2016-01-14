#include "aeon_object.h"
#include "aeon_type.h"

aeon_type* aeon_object::getType()
{
	return m_type;
}

void aeon_object::setField(const std::string& field, int32_t value)
{
	aeon_type::FieldInfo* fieldInfo = m_type->getField(field);
	if (fieldInfo)
	{
		memcpy((char*)addr + fieldInfo->offset, &value, sizeof(value));
	}
	else
	{
		printf("[setField] Type '%s' has no field named '%s'\n", m_type->getName().c_str(), field.c_str());
	}
}

void aeon_object::getField(const std::string& field, int32_t& value) const
{
	aeon_type::FieldInfo* fieldInfo = m_type->getField(field);
	if (fieldInfo)
	{
		memcpy(&value, (char*)addr + fieldInfo->offset, sizeof(value));
	}
	else
	{
		printf("[getField] Type '%s' has no field named '%s'\n", m_type->getName().c_str(), field.c_str());
	}
}

void aeon_object::log()
{
	printf("Object %s@%x\n", m_type->m_name.c_str(), addr);
}