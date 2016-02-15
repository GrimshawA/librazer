#include <AEON/aeQualType.h>
#include <AEON/aeType.h>

aeQualType aeQualType::fromString(const std::string& str)
{
	aeQualType t;
	return t;
}

aeQualType::aeQualType()
: m_type(nullptr)
, m_templated(false)
, m_const(false)
, m_handle(false)
{

}

aeQualType::aeQualType(aeType* type)
: m_type(type)
, m_templated(false)
, m_const(false)
, m_handle(false)
{

}

void aeQualType::parse(const std::string& str, aeContext* ctx)
{

}

uint32_t aeQualType::getSize()
{
	if (m_type)
	{
		if (m_handle)
		{
			return 4;
		}
		else
		{
			return m_type->getSize();
		}
	}
	else
		return 0;
}

std::string aeQualType::getTypeName() const
{
	if (m_type)
		return m_type->getName();
	else
		return std::string();
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

aeType* aeQualType::getType() const
{
	return m_type;
}

bool aeQualType::isConst() const
{
	return m_const;
}

bool aeQualType::isTemplated() const
{
	return !m_templateArgs.empty();
}

bool aeQualType::isPrimitive() const
{
	return false;
}

bool aeQualType::isVoid() const
{
	return m_type == nullptr;
}

bool aeQualType::isPod() const
{
	if (m_handle)
		return true;

	if (m_type->isPod())
	{
		return true;
	}

	return false;
}

AEPrimitive aeQualType::getPrimitive() const
{
	return AE_PFLOAT; //todo
}

std::string aeQualType::str() const
{
	std::string str = "nulltype";
	if (m_type)
	{
		str.clear();

		if (m_const)
			str = "const ";

		str += m_type->getSymbolName();

		if (m_templateArgs.size() > 0)
		{
			str += "<";
			for (auto& i : m_templateArgs)
			{
				str += i.str();
			}
			str += ">";
		}

		if (m_handle)
			str += "@";
	}
	return str;
}

const char* aeQualType::c_str()
{
	return str().c_str();
}

aeQualType::operator bool() const
{
	return m_type != nullptr;
}