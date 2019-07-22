#include <razer/frontend/compiler/RzCompiler.h>
#include <razer/vm/InstructionSet.h>
#include <razer/vm/VirtualMachine.h>
#include <razer/runtime/RzEngine.h>
#include <razer/utilS/DebugDefs.h>

#include <cassert>

#include <razer/utils/Logger.h>


RzFunction* RzCompiler::compileFunction(aeNodeFunction* functionNode)
{
	std::string symbol_prefix;
	auto topClass = getTopClassNode();
	if (topClass)
	{
		symbol_prefix = topClass->m_name + ".";
	}

    RzFunction* function = m_module->getFunction(symbol_prefix + functionNode->m_name);

	m_OffsetFromBasePtr = 0;

	// Compiled unless some error is thrown
	m_currentFunction = function;
	function->m_compiled = true;

	ScopeLocalData paramsScope;
	paramsScope.offset = 0;
	m_scopes.push_back(paramsScope);

	RzQualType returnType = functionNode->getReturnType();
	if (!returnType.isVoid())
	{
		declareStackVar("__ret", returnType);
	}
	
	// The function must be aware how much time it needs for the return value
	function->returnValueSize = returnType.getSize();

	m_caller = functionNode;

	RzType::MethodInfo* methodInfo = m_currentStruct->getMethod(functionNode->m_name);
	if (methodInfo)
	{
		methodInfo->offset = m_cursor;
	}

	function->m_offset = m_cursor;
	function->m_absoluteName = symbol_prefix + functionNode->m_name;
	function->m_module = m_module;

    // Declare function parameters
	function->params.resize(functionNode->isNonStaticMethod() ? functionNode->m_parameters.size() + 1
															  : functionNode->m_parameters.size());
	if (functionNode->isNonStaticMethod())
	{
		RzQualType thisHandle(m_currentStruct);
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
		RzQualType thisHandle(m_currentStruct);
		thisHandle.m_handle = true;
		declareStackVar("this", thisHandle);
	}

	for (int i = 0; i < functionNode->m_parameters.size(); ++i)
	{
		declareStackVar(functionNode->m_parameters[i]->m_decls[0].m_name, functionNode->m_parameters[i]->m_type);
	}

    if (functionNode->is_constructor)
    {
        // Constructor specific code
        emitConstructorInjection(functionNode, function);
    }

	// let's just generate code for the executable block
    RzCompileResult ret = emitBlock(functionNode->m_block.get());
    if (ret == RzCompileResult::aborted)
        return nullptr;

    // TODO: Needs to be done by returns
    releaseParametersContext();

	emitReturnCode(nullptr);
	m_caller = nullptr;
	m_currentFunction = nullptr;

	function->m_numInstructions = cursor() - function->m_offset;

    // The module needs to be aware of what functions compose it
    m_module->m_functions.push_back(*function);
	return function;
}
