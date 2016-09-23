#include <AEON/Compiler/aeCompiler.h>
#include <AEON/Runtime/aeByteCode.h>
#include <AEON/VM/AEVm.h>
#include <AEON/AEContext.h>
#include <AEON/DebugDefs.h>

#include <cassert>


AEFunction* AECompiler::compileFunction(aeNodeFunction* functionNode)
{
	std::string symbol_prefix;
	auto topClass = getTopClassNode();
	if (topClass)
	{
		symbol_prefix = topClass->m_name + ".";
	}

	AEFunction* function = m_env->createFunction(symbol_prefix + functionNode->m_name);
	
#if defined TRACE_STACK
	printf("::: Frame %s : Method\n", functionNode->m_name.c_str());
#endif

	m_OffsetFromBasePtr = 0;

	// Compiled unless some error is thrown
	m_currentFunction = function;
	function->m_compiled = true;

	ScopeLocalData paramsScope;
	paramsScope.offset = 0;
	m_scopes.push_back(paramsScope);

	aeQualType returnType = functionNode->getReturnType();
	if (!returnType.isVoid())
	{
		declareStackVar("__ret", returnType);
	}
	
	// The function must be aware how much time it needs for the return value
	function->returnValueSize = returnType.getSize();

	m_caller = functionNode;

	function->m_offset = m_cursor;
	function->m_absoluteName = symbol_prefix + functionNode->m_name;
	function->m_module = m_module;

	if (functionNode->is_constructor)
	{
		// Constructor specific code
		emitConstructorInjection(functionNode, function);
	}

	// Declare function parameters
	function->params.resize(functionNode->isNonStaticMethod() ? functionNode->m_parameters.size() + 1
															  : functionNode->m_parameters.size());
	if (functionNode->isNonStaticMethod())
	{
		aeQualType thisHandle(m_currentStruct);
		thisHandle.m_handle = true;
		function->params[0] = thisHandle;

		for (int i = 0; i < functionNode->m_parameters.size(); ++i)
		{
			function->params[i+1] = functionNode->m_parameters[i]->m_type;
		}
	}
	else
	{
		for (int i = 0; i < functionNode->m_parameters.size(); ++i)
		{
			function->params[i] = functionNode->m_parameters[i]->m_type;
		}
	}

	if (functionNode->isNonStaticMethod())
	{
		aeQualType thisHandle(m_currentStruct);
		thisHandle.m_handle = true;
		declareStackVar("this", thisHandle);
	}

	for (int i = 0; i < functionNode->m_parameters.size(); ++i)
	{
		declareStackVar(functionNode->m_parameters[i]->m_decls[0].m_name, functionNode->m_parameters[i]->m_type);
	}

	// let's just generate code for the executable block
	emitBlock(functionNode->m_block.get());
	 
	emitReturnCode(nullptr);
	m_caller = nullptr;
	m_currentFunction = nullptr;

	// TODO: Needs to be done by returns
	releaseParametersContext();

	function->m_numInstructions = cursor() - function->m_offset;
	return function;
}
