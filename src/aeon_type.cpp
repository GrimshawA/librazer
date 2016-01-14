#include "aeon_type.h"
#include "aeon_vm.h"

aeon_type::aeon_type()
{
	m_size = 1;
	m_name = "null";
}

aeon_type::aeon_type(const std::string& _name, uint32_t _size)
{
	m_name = _name;
	m_size = _size;
}

uint32_t aeon_type::getSize()
{
	return m_size;
}

uint32_t aeon_type::getNumMethods()
{
	return m_methods.size();
}

uint32_t aeon_type::getNumFields()
{
	return m_fields.size();
}

uint32_t aeon_type::getNumProtocols()
{
	return 0;
}

std::string aeon_type::getName()
{
	return m_name;
}

aeon_module* aeon_type::getModule()
{
	return m_module;
}

bool aeon_type::isScriptSide()
{
	return true;
}

bool aeon_type::isHostSide()
{
	return true;
}

bool aeon_type::isHybrid()
{
	return true;
}

aeon_type::FieldInfo* aeon_type::getField(std::string name)
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

void aeon_type::createField(FieldInfo fieldInfo)
{
	if (m_fields.size() == 0)
		m_size = 0;

	fieldInfo.offset = getSize();
	m_fields.push_back(fieldInfo);

	m_size += fieldInfo.size;
}

void aeon_type::registerMethod(const std::string& name, void* funptr)
{
	MethodInfo methodInfo;
	methodInfo.type = 0;
	methodInfo.name = name;
	m_methods.push_back(methodInfo);
}

void aeon_type::registerField(const std::string& name, int offset)
{
	FieldInfo fieldInfo;
	fieldInfo.name = name;
	fieldInfo.offset = offset;
	m_fields.push_back(fieldInfo);
}

void aeon_type::registerEnum(const std::string& name)
{
	EnumInfo enumInfo;
	enumInfo.name = name;
	m_enums.push_back(enumInfo);
}

void aeon_type::registerEnumProperty(const std::string& name, const std::string& property, int value)
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