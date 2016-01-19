#ifndef aeNodeFunctionCall_h__
#define aeNodeFunctionCall_h__

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

public:

	aeNodeFunctionCall();

	virtual std::string exprstr();

	std::string printtext();
};

#endif // aeNodeFunctionCall_h__
