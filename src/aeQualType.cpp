#include "aeQualType.h"
#include "aeon_type.h"

aeQualType aeQualType::fromString(const std::string& str)
{
	aeQualType t;
	return t;
}

aeQualType::aeQualType()
: m_type(nullptr)
, m_templated(false)
, m_const(false)
{

}

uint32_t aeQualType::getSize()
{
	if (m_type)
		return m_type->getSize();
	else
		return 0;
}

uint32_t aeQualType::getNumTemplateArgs() const
{
	return m_templateArgs.size();
}

aeQualType aeQualType::getTemplateArg(uint32_t index) const
{
	return m_templateArgs[index];
}

std::string aeQualType::getName() const
{
	return m_type ? m_type->getName() : "void";
}

aeon_type* aeQualType::getType() const
{
	return m_type;
}

bool aeQualType::isConst() const
{
	return m_const;
}

bool aeQualType::isTemplated() const
{
	return m_templated;
}

bool aeQualType::isPrimitive() const
{
	return false;
}

bool aeQualType::isVoid() const
{
	return m_type == nullptr;
}

AEPrimitive aeQualType::getPrimitive() const
{
	return AE_PFLOAT; //todo
}

std::string aeQualType::str() const
{
	if (isVoid())
		return "void";
	else
		return m_type->getName();
}

aeQualType::operator bool() const
{
	return m_type != nullptr;
}