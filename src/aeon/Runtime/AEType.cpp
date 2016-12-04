#include <AEON/Runtime/AEType.h>
#include <RazerVM/AEVm.h>

AEType::AEType()
: is_native(false)
, m_userData(nullptr)
{
	m_size = 1;
	m_name = "null";
}

AEType::AEType(const std::string& _name, uint32_t _size)
: is_native(false)
, m_userData(nullptr)
{
	m_name = _name;
	m_size = _size;
}

bool AEType::isEnum()
{
	return m_typeKind == KindEnum;
}

bool AEType::isTemplated()
{
	return !m_templateParams.empty();
}

bool AEType::isClass()
{
	return m_typeKind == KindClass;
}

bool AEType::isNative()
{
	return is_native;
}

bool AEType::isPod()
{
	for (auto& field : m_fields)
	{
		if (!field.type.getType()->isPod())
			return false;
	}

	return m_pod;
}

uint32_t AEType::getSize()
{
	return m_size;
}

uint32_t AEType::getNumMethods()
{
	return m_methods.size();
}

uint32_t AEType::getNumFields()
{
	return m_fields.size();
}

uint32_t AEType::getNumProtocols()
{
	return 0;
}

std::string AEType::getName()
{
	return m_name;
}

int AEType::getFunctionId(const std::string& name)
{
	return m_module->m_context->getFunctionIndexByName(name);
}

AEFunction* AEType::getFunction(const std::string& name)
{
	return m_module->m_context->getFunctionByName(name);
}

RzModule* AEType::getModule()
{
	return m_module;
}

bool AEType::isScriptSide()
{
	return true;
}

bool AEType::isHostSide()
{
	return true;
}

bool AEType::isHybrid()
{
	return true;
}

aeField* AEType::getField(std::string name)
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

void AEType::createField(aeField fieldInfo)
{
	if (m_fields.size() == 0)
		m_size = 0;

	fieldInfo.offset = getSize();
	m_fields.push_back(fieldInfo);

	m_size += fieldInfo.size;
}

void AEType::registerMethod(const std::string& name, void* funptr)
{
	MethodInfo methodInfo;
	methodInfo.type = 0;
	methodInfo.name = name;
	m_methods.push_back(methodInfo);
}

void AEType::registerField(const std::string& name, int offset)
{
	aeField fieldInfo;
	fieldInfo.name = name;
	fieldInfo.offset = offset;
	m_fields.push_back(fieldInfo);
}

void AEType::registerEnum(const std::string& name)
{
	EnumInfo enumInfo;
	enumInfo.name = name;
	m_enums.push_back(enumInfo);
}

void AEType::registerEnumProperty(const std::string& name, const std::string& property, int value)
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