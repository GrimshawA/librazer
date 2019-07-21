#ifndef aeNodeFunctionCall_h__
#define aeNodeFunctionCall_h__

#include <RazerRuntime/Module.h>
#include "aeNodeExpr.h"

class aeNodeFunction;

/**
	\class aeNodeFunctionCall
	\brief Represents a call to some function in code
*/
class aeNodeFunctionCall : public aeNodeExpr
{
public:

	std::vector<aeNodeExpr*> m_args;        ///< List of arguments to this function call
	aeNodeFunction*          m_callee;      ///< The node we are calling to execute
	bool                     m_is_method;   ///< This node calls a method on an object or static class function
	std::string              m_name;


	struct TemplateTypeArgument
	{
		std::string TypeString;
	};

	std::vector<TemplateTypeArgument> templateTypeArguments;
	aeNodeFunction* m_function;

    RzFunction* m_fn;

public:

	aeNodeFunctionCall();

	/// Get the qualified type of the argument
	RzQualType getArgType(uint32_t index);

	uint32_t getReturnTypeSize();

	/// Does this node already know exactly what function is to be called?
	bool hasLinkedFunction();

	/// Returns a simple string representation
	std::string str() const;
};

#endif // aeNodeFunctionCall_h__
