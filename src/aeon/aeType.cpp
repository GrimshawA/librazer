#include <AEON/aeType.h>
#include <aeon/aeVM.h>

aeType::aeType()
: is_native(false)
, m_userData(nullptr)
{
	m_size = 1;
	m_name = "null";
}

aeType::aeType(const std::string& _name, uint32_t _size)
: is_native(false)
, m_userData(nullptr)
{
	m_name = _name;
	m_size = _size;
}

bool aeType::isEnum()
{
	return m_typeKind == KindEnum;
}

bool aeType::isTemplated()
{
	return !m_templateParams.empty();
}

bool aeType::isClass()
{
	return m_typeKind == KindClass;
}

bool aeType::isNative()
{
	return is_native;
}

bool aeType::isPod()
{
	for (auto& field : m_fields)
	{
		if (!field.type.getType()->isPod())
			return false;
	}

	return m_pod;
}

uint32_t aeType::getSize()
{
	return m_size;
}

uint32_t aeType::getNumMethods()
{
	return m_methods.size();
}

uint32_t aeType::getNumFields()
{
	return m_fields.size();
}

uint32_t aeType::getNumProtocols()
{
	return 0;
}

std::string aeType::getName()
{
	return m_name;
}

aeon_module* aeType::getModule()
{
	return m_module;
}

bool aeType::isScriptSide()
{
	return true;
}

bool aeType::isHostSide()
{
	return true;
}

bool aeType::isHybrid()
{
	return true;
}

aeField* aeType::getField(std::string name)
{
	for (auto& field : m_fields)
	{
		if (field.name == name)
		{
			return &field;
		}
	}

	return nullptr;
}

void aeType::createField(aeField fieldInfo)
{
	if (m_fields.size() == 0)
		m_size = 0;

	fieldInfo.offset = getSize();
	m_fields.push_back(fieldInfo);

	m_size += fieldInfo.size;
}

void aeType::registerMethod(const std::string& name, void* funptr)
{
	MethodInfo methodInfo;
	methodInfo.type = 0;
	methodInfo.name = name;
	m_methods.push_back(methodInfo);
}

void aeType::registerField(const std::string& name, int offset)
{
	aeField fieldInfo;
	fieldInfo.name = name;
	fieldInfo.offset = offset;
	m_fields.push_back(fieldInfo);
}

void aeType::registerEnum(const std::string& name)
{
	EnumInfo enumInfo;
	enumInfo.name = name;
	m_enums.push_back(enumInfo);
}

void aeType::registerEnumProperty(const std::string& name, const std::string& property, int value)
{
	for (auto& enumm : m_enums)
	{
		if (enumm.name == name)
		{
			EnumEntryInfo enumEntryInfo;
			enumEntryInfo.name = property;
			enumEntryInfo.value = value;
			enumm.entries.push_back(enumEntryInfo);
			return;
		}
	}
	// throw api error
}