#ifndef aeNodeFunction_h__
#define aeNodeFunction_h__

#include "AEBaseNode.h"
#include "aeNodeValue.h"

class aeNodeExpr;
class aeNodeVarDecl;
class RzQualType;
class aeNodeBlock;
class AEStmtNode;

/**
	\class aeNodeFunction
	\brief All functions are represented by this structure

	This includes:
	- global functions
	- class statics
	- class methods
	- anonymous functions / closures
*/
class aeNodeFunction : public aeNodeValue
{
public:

	std::vector<aeNodeVarDecl*>                      m_parameters;
	std::vector<RzQualType>                       m_paramTypes;
	std::vector<std::unique_ptr<AEStmtNode>> m_statements;
	std::string                                   m_name;
	RzQualType                                    m_returnType;
	std::unique_ptr<aeNodeBlock>                  m_block;
	bool                                          is_constructor;
	bool                                          is_method;
	bool                                          is_destructor;
	bool                                          is_global;
	bool                                          is_static;
	bool                                          is_anon;
	int visibility;

	std::string visibilityText();

public:
	aeNodeFunction();

	RzQualType getReturnType();

	/// Get the fully qualified type of the parameter
	RzQualType getParameterType(uint32_t index);

	/// Is this a global function outside class scope
	bool isGlobalFunction();

	/// Is this function a non static method of a class
	bool isNonStaticMethod();

	/// Is this a static class method
	bool isStaticMethod();

	/// Is this is an anonymous function
	bool isAnonymousFunction();

	std::string str() const;
};

#endif // aeNodeFunction_h__
