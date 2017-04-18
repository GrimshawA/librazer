#include "AEStructNode.h"
#include "aeNodeFunction.h"
#include <RazerParser/AST/aeNodeBlock.h>
#include <RazerRuntime/RzEngine.h>

AEFieldNode::AEFieldNode()
{
	
}

std::string AEFieldNode::visibilityText()
{
	if (visibility == AEStructNode::VISIBILITY_PUBLIC)
		return "public";
	if (visibility == AEStructNode::VISIBILITY_PROTECTED)
		return "protected";
	if (visibility == AEStructNode::VISIBILITY_PRIVATE)
		return "private";

	return "undefined";
}

void AEFieldNode::deduceStaticType(RzEngine* ctx)
{
	if (!initializer)
	{
		type = ctx->getTypeInfo("var");
		return;
	}

	printf("FIELD VALUE %s\n", initializer->value->str().c_str());

	if (initializer->value->m_nodeType == AEN_INTEGER)
	{
		type = ctx->getTypeInfo("int");
	}
	else if (initializer->value->m_nodeType == AEN_STRING)
	{
		type = ctx->getTypeInfo("string");
	}
	else if (initializer->value->m_nodeType == AEN_OBJECTINIT)
	{
		std::string initName = ((AEObjectInitNode*)initializer->value)->m_typename;
		type = ctx->getTypeInfo(initName);
	}
}

//////////////////////////////////////////////////////////////////////////

AEStructNode::AEStructNode()
: m_interface(false)
{
	m_nodeType = AEN_CLASS;
}

std::string AEStructNode::str() const
{
	std::string s;
	
	for (auto& f : m_functions)
	{
        s += "+ " + f->m_name + " (" + f->visibilityText() +") \n" + f->str();
	}

	for (auto& m : m_fields)
	{
		s += "- " + m->name + " ("+ m->type.str() + ") (" + m->visibilityText() + ") \n";
	}

	for (auto& e : m_enums)
	{
		s += "enum " + e->str() + "\n";
	}

	return s;
}

bool AEStructNode::generatesDestructor()
{
	return false;
}

aeNodeFunction* AEStructNode::getDestructor()
{
	return nullptr;
}

bool AEStructNode::isTemplated()
{
	return m_templated && !templateTypes.empty();
}

bool AEStructNode::isGeneric()
{
	return !m_templated && !templateTypes.empty();
}

bool AEStructNode::isPartial()
{
	return m_partial;
}

bool AEStructNode::generatesDefaultConstructor()
{
	return false;
}

bool AEStructNode::hasDefaultConstructor()
{
	return true;
}

bool AEStructNode::isInterface()
{
	return m_interface;
}

bool AEStructNode::isPrototyped()
{
	for (auto& t : traits)
	{
		if (t.m_builtIn == AETypeTrait::TRAIT_PROTO)
			return true;
	}

	return false;
}

bool AEStructNode::adheresToInterface(AEStructNode* intf)
{
	return false;
}

bool AEStructNode::hasMethod(const std::string& name)
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

aeNodeFunction* AEStructNode::getMethod(const std::string& name)
{
	for (auto item : m_items)
	{
		if (item->m_nodeType == AEN_FUNCTION && ((aeNodeFunction*)item)->m_name == name)
		{
			return ((aeNodeFunction*)item);
		}
	}

    for (auto& fn : m_functions)
    {
        if (fn->m_name == name)
            return fn.get();
    }

	return nullptr;
}

aeNodeFunction* AEStructNode::selectOverload(const std::string& name, std::vector<RzQualType> params)
{
	return nullptr;
}

aeNodeFunction* AEStructNode::createDefaultConstructor()
{
	aeNodeFunction* function = new aeNodeFunction;
	function->m_name = m_name;
	m_functions.push_back(std::unique_ptr<aeNodeFunction>(function));
	return function;
}

aeNodeFunction* AEStructNode::createDestructor()
{
	aeNodeFunction* function = new aeNodeFunction;
	function->m_name = "~" + m_name;
	m_functions.push_back(std::unique_ptr<aeNodeFunction>(function));
	return function;
}

////////////////////////////////////////////////////////////////////////// ENUM


AEEnumNode::AEEnumNode()
: AEBaseNode()
{
	m_nodeType = AEN_ENUM;
}

void AEEnumNode::addField(std::string name)
{
	members.push_back(name);
}

std::string AEEnumNode::str() const
{
	std::string s = "[";

	for (auto& m : members)
	{
		s += m + ",";
	}

	s += "]";
	return s;
}
