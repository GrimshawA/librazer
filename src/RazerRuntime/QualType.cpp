#include <RazerRuntime/QualType.h>
#include <RazerRuntime/RzType.h>

RzQualType RzQualType::fromString(const std::string& str)
{
    RzQualType t;
	return t;
}

RzQualType::RzQualType()
: m_type(nullptr)
, m_templated(false)
, m_const(false)
, m_handle(false) {

}

RzQualType::RzQualType(RzType* type)
: m_type(type)
, m_templated(false)
, m_const(false)
, m_handle(false) {

}

bool RzQualType::sameTypeAs(const RzQualType& b) const {
    if (!m_type || !b.m_type)
        return false;

    return m_type == b.m_type;
}

void RzQualType::parse(const std::string& str, RzEngine* ctx)
{

}

uint32_t RzQualType::getSize()
{
	if (m_type)
	{
		if (m_handle)
		{
			return sizeof(void*);
		}
		else
		{
			return m_type->getSize();
		}
	}
	else
		return 0;
}

std::string RzQualType::getTypeName() const
{
	if (m_type)
		return m_type->getName();
	else
		return std::string();
}

uint32_t RzQualType::getNumTemplateArgs() const
{
	return m_templateArgs.size();
}

RzQualType RzQualType::getTemplateArg(uint32_t index) const
{
	return m_templateArgs[index];
}

std::string RzQualType::getName() const
{
	return m_type ? m_type->getName() : "void";
}

RzType* RzQualType::getType() const
{
	return m_type;
}

bool RzQualType::isBroken() const
{
	return !m_typeString.empty() && !m_type;
}

bool RzQualType::isConst() const
{
	return m_const;
}

bool RzQualType::isTemplated() const
{
	return !m_templateArgs.empty();
}

bool RzQualType::isPrimitive() const
{
    if (!m_type)
        return false;

    return m_type->isPrimitive();
}

bool RzQualType::isVoid() const
{
	return m_type == nullptr;
}

bool RzQualType::isPod() const
{
	if (m_handle)
		return true;

	if (m_type->isPod())
	{
		return true;
	}

	return false;
}

bool RzQualType::isVariant() const
{
    if (!m_type)
        return false;

	return m_type->getSymbolName() == "var";
}

AEPrimitive RzQualType::getPrimitive() const
{
	return AE_PFLOAT; //todo
}

std::string RzQualType::str() const
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

RzQualType::operator bool() const {
	return m_type != nullptr;
}

bool RzQualType::operator!=(const RzQualType& b) const {
    if (!m_type || !b.m_type)
        return false;

    return m_type != b.m_type;
}

bool RzQualType::operator==(const RzQualType& b) const {
    if (!m_type || !b.m_type)
        return false;

    return m_type == b.m_type;
}
