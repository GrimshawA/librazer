#ifndef aeNodeFunction_h__
#define aeNodeFunction_h__

#include "aeNodeBase.h"

class aeNodeExpr;
class aeQualType;
class aeNodeBlock;
class aeNodeStatement;

/**
	\class aeNodeFunction
	\brief All functions are represented by this structure

	This includes:
	- global functions
	- class statics
	- class methods
	- anonymous functions / closures
*/
class aeNodeFunction : public aeNodeBase
{
public:

	std::vector<aeNodeExpr*>                      m_parameters;
	std::vector<aeQualType>                       m_paramTypes;
	std::vector<std::unique_ptr<aeNodeStatement>> m_statements;
	std::string                                   m_name;
	aeQualType                                    m_returnType;
	std::unique_ptr<aeNodeBlock>                  m_block;
	bool                                          is_constructor;
	bool                                          is_method;
	bool                                          is_destructor;
	bool                                          is_global;
	bool                                          is_static;
	bool                                          is_anon;

public:
	aeNodeFunction();

	aeQualType getReturnType();

	/// Get the fully qualified type of the parameter
	aeQualType getParameterType(uint32_t index);

	/// Is this a global function outside class scope
	bool isGlobalFunction();

	/// Is this function a non static method of a class
	bool isNonStaticMethod();

	/// Is this a static class method
	bool isStaticMethod();

	/// Is this is an anonymous function
	bool isAnonymousFunction();

	std::string str();
};

#endif // aeNodeFunction_h__
