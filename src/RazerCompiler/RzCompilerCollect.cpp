#include <RazerCompiler/RzCompiler.h>
#include <RazerRuntime/RzType.h>

#include <RazerParser/AST/aeNodeFunction.h>

void RzCompiler::collect(RzSourceUnit& parseTree)
{
    for (std::size_t i = 0; i < parseTree.m_types.size(); ++i)
	{	
        AEStructNode& st = (AEStructNode&)*parseTree.m_types[i].get();
        collect(st);
	}
}

void RzCompiler::collect(AEStructNode& cls)
{
	// Collect class data
    RzType* typeInfo = new RzType(*m_module, cls.m_name, 0);
	typeInfo->m_absoluteName = cls.m_name;
	m_module->m_types.push_back(typeInfo);

	for (int i = 0; i < cls.m_functions.size(); ++i)
	{		
		aeNodeFunction& fn = *cls.m_functions[i].get();

		RzType::MethodInfo method;
		method.name = fn.m_name;
		method.methodCallback = 0;
		method.offset = 0;
        method.args.resize(fn.m_parameters.size()); //TODO
		typeInfo->m_methods.push_back(method);

        // Collect the functions to the module now
        RzFunction* f = m_module->createFunction(fn.m_name);
        f->m_absoluteName = cls.m_name + "." + fn.m_name;
	}

    //printf("Collected %s\n", typeInfo->m_name.c_str());
}
