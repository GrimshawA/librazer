#include "aeNodeClass.h"
#include "aeNodeFunction.h"

aeNodeClass::aeNodeClass()
{
	m_nodeType = AEN_CLASS;
}

bool aeNodeClass::hasMethod(const std::string& name)
{
	for (auto item : m_items)
	{
		if (item->m_nodeType == AEN_FUNCTION && ((aeNodeFunction*)item)->m_name == name)
		{
			return true;
		}
	}
	return false;
}

aeNodeFunction* aeNodeClass::getMethod(const std::string& name)
{
	for (auto item : m_items)
	{
		if (item->m_nodeType == AEN_FUNCTION && ((aeNodeFunction*)item)->m_name == name)
		{
			return ((aeNodeFunction*)item);
		}
	}
	return nullptr;
}