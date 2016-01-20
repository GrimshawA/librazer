#include "aeNodeEnum.h"

aeNodeEnum::aeNodeEnum()
: aeNodeBase()
{
	m_nodeType = Enum;
}

void aeNodeEnum::addField(std::string name)
{
	members.push_back(name);
}

std::string aeNodeEnum::printtext()
{
	return "Enum " + name;
}