#include <AEON/Runtime/AEObject.h>
#include <AEON/Runtime/AEFunction.h>
#include <AEON/VM/AEVm.h>

void AEObject::setProperty(const std::string& name, const std::string& value)
{

}

int AEObject::count() const
{
	return m_properties.size();
}

std::string AEObject::propertyName(int index)
{
	return m_names[index];
}

void AEObject::setProperty(const std::string& name, AEValue value)
{
	m_names.push_back(name);
	m_properties.push_back(value);
}

void AEObject::call(const std::string& name)
{
	AEFunction* fn = getType()->getFunction(m_type->getName() + "." + name);
	printf("%x. %s\n", fn, (m_type->getName() + "." + name).c_str());
	if (fn)
	{
		AEVirtualMachine vm;
		vm.setContext(getType()->m_module->m_context);
//		vm.call(fn);
	}
}

AEType* AEObject::getType()
{
	return m_type;
}

void AEObject::setField(const std::string& field, int32_t value)
{
	aeField* fieldInfo = m_type->getField(field);
	if (fieldInfo)
	{
		memcpy((char*)m_obj + fieldInfo->offset, &value, sizeof(value));
	}
	else
	{
		printf("[setField] Type '%s' has no field named '%s'\n", m_type->getName().c_str(), field.c_str());
	}
}

void AEObject::getField(const std::string& field, int32_t& value) const
{
	aeField* fieldInfo = m_type->getField(field);
	if (fieldInfo)
	{
		memcpy(&value, (char*)m_obj + fieldInfo->offset, sizeof(value));
	}
	else
	{
		printf("[getField] Type '%s' has no field named '%s'\n", m_type->getName().c_str(), field.c_str());
	}
}

void* AEObject::getFieldAddress(const std::string& field) const
{
	aeField* fieldInfo = m_type->getField(field);
	if (fieldInfo)
	{
		return (char*)m_obj + fieldInfo->offset;
	}
	return nullptr;
}

void AEObject::log()
{
	printf("Object %s@%x\n", m_type->m_name.c_str(), m_obj);
}