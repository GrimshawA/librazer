#include <RazerCompiler/AECompiler.h>
#include <RazerRuntime/RzType.h>

#include <RazerParser/AST/aeNodeFunction.h>

void RzCompiler::collect(RzSourceUnit& parseTree)
{
	for (std::size_t i = 0; i < parseTree.m_items.size(); ++i)
	{	
		if (parseTree.m_items[i]->m_nodeType == AEN_CLASS)
		{
			AEStructNode& st = (AEStructNode&)*parseTree.m_items[i];
			collect(st);
		}
	}
}

void RzCompiler::collect(AEStructNode& cls)
{
	// Collect class data
	RzType* typeInfo = new RzType();
	typeInfo->m_name = cls.m_name;
	typeInfo->m_absoluteName = cls.m_name;
	m_module->m_types.push_back(typeInfo);

	for (int i = 0; i < cls.m_functions.size(); ++i)
	{		
		aeNodeFunction& fn = *cls.m_functions[i].get();

		RzType::MethodInfo method;
		method.name = fn.m_name;
		method.methodCallback = 0;
		method.offset = 0;
		typeInfo->m_methods.push_back(method);

        //printf("Collected fn %s\n", fn.m_name.c_str());
		
	}

    //printf("Collected %s\n", typeInfo->m_name.c_str());
}
