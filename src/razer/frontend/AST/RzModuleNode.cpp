#include "RzModuleNode.h"
#include "RzSourceUnit.h"
#include "AEStructNode.h"
#include<razer/utils/Logger.h>

bool RzModuleNode::checkDuplicates()
{
	auto countOccurrences = [&](const std::string& name)
	{
		int count = 0;

		for (auto& m : m_modules)
		{
			for (auto& t : m->m_types)
			{
				if (t->m_name == name)
				{
					++count;
				}
			}
		}

		return count;
	};

	for (auto& m : m_modules)
	{
		for (auto& t : m->m_types)
		{
			int count = countOccurrences(t->m_name);
			if (count > 1)
			{
				RZLOG("Duplicate type: %s\n", t->m_name.c_str());
				return false;
			}
		}
	}

	return true;
}
