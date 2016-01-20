#include "aeQualType.h"
#include "../aeon_type.h"

aeQualType::aeQualType()
: m_type(nullptr)
{

}

bool aeQualType::isPrimitive() const
{
	return m_type->
}

bool aeQualType::isVoid() const
{
	return m_type == nullptr;
}

std::string aeQualType::str() const
{
	if (isVoid())
		return "void";
	else
		return "Type " + m_type->getName();
}
