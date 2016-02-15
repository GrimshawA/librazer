#include "aeNodeClass.h"
#include "aeNodeFunction.h"
#include <AEON/AST/aeNodeBlock.h>

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
	for (auto& fn : m_functions)
	{
		if (fn->m_name == name)
			return true;
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

aeNodeFunction* aeNodeClass::createDefaultConstructor()
{
	aeNodeFunction* function = new aeNodeFunction;
	function->m_name = m_name;
	m_functions.push_back(std::unique_ptr<aeNodeFunction>(function));
	return function;
}

aeNodeFunction* aeNodeClass::createDestructor()
{
	aeNodeFunction* function = new aeNodeFunction;
	function->m_name = "~" + m_name;
	m_functions.push_back(std::unique_ptr<aeNodeFunction>(function));
	return function;
}