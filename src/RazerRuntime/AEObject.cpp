#include <RazerRuntime/AEObject.h>
#include <RazerRuntime/AEFunction.h>
#include <RazerVM/VirtualMachine.h>
#include <Logger.h>

AEObject::AEObject()
: m_ref(0)
{

}

RzValue* AEObject::getValueRef(const std::string& name)
{
	for (int i = 0; i < m_properties.size(); ++i)
	{
		if (m_names[i] == name)
			return &m_properties[i];
	}

	return nullptr;
}

void AEObject::setProperty(const std::string& name, const std::string& value)
{
	m_names.push_back(name);
	m_properties.push_back(value);
}

int AEObject::count() const
{
	return m_properties.size();
}

std::string AEObject::propertyName(int index)
{
	return m_names[index];
}

void AEObject::setProperty(const std::string& name, RzValue value)
{
	m_names.push_back(name);
	m_properties.push_back(value);
}

void AEObject::call(const std::string& name)
{
	if (getType()->isNative())
	{

	}
	else
	{
		AEFunction* fn = getType()->getFunction(m_type->getName() + "." + name);
		RZLOG("%x. %s\n", fn, (m_type->getName() + "." + name).c_str());
		if (fn)
		{
			RzVirtualMachine vm;
			vm.setContext(getType()->m_module->m_context);
			//		vm.call(fn);
		}
	}	
}

RzType* AEObject::getType()
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
		RZLOG("[setField] Type '%s' has no field named '%s'\n", m_type->getName().c_str(), field.c_str());
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
		RZLOG("[getField] Type '%s' has no field named '%s'\n", m_type->getName().c_str(), field.c_str());
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
	RZLOG("Object %s@%x\n", m_type->m_name.c_str(), m_obj);
}

//////////////////////////////////////////////////////////////////////////////////////////////

void AEDynamicObject::setValue(const std::string& name, const RzValue& value)
{
	m_properties[name] = value;
}