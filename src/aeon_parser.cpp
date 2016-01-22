#include "aeon_parser.h"
#include "aeon_lexer.h"
#include "aeon_context.h"
#include "nodes/aeNodeClass.h"
#include "nodes/aeNodeModule.h"
#include "nodes/aeNodeNamespace.h"
#include "aeon_lexer.h"
#include <map>

std::map<std::string, int> mOperatorTable = {
		{ "=", 0 },
		{ ">", 10 },
		{ "<", 10 },
		{ ">=", 10 },
		{ "<=", 10 },
		{ "==", 10 },
		{ "+", 20 },
		{ "-", 20 },
		{ "^", 30 },
		{ "*", 30 },
		{ "/", 30 },
		{ "%", 30 },

	};

aeon_parser::aeon_parser()
{
	pass = Gather;
}

	void aeon_parser::parseValue(aeon_lexer& lexer, aeon_value& rootValue)
	{
		if (lexer.tokens.size() == 0)
		{
			//Log("Parser: No tokens in the stream");
			return;
		}

		lex = &lexer;
		i = 0;
		mDataValue = &rootValue;

		Tok = getNextToken();

		while (Tok.type != aeon_token::EndOfFile)
		{
			aeon_value ParsedValue = parseDataObject();
			rootValue.properties.push_back(ParsedValue);
		}
	}

	/// Parses the {} data object
	aeon_value aeon_parser::parseDataObject()
	{
		aeon_value objectValue;
		objectValue.mValueType = aeon_value::EInvalid;

		// we are on the first token of a data definition

		std::string ObjectType;
		if (Tok.type == Tok.Identifier)
		{
			ObjectType = Tok.text;
			getNextToken();
		}

		// Found a member-of property
		std::vector<std::string> ObjectSubProperties;
		if (Tok.type == Tok.Dot)
		{
			while (Tok.type == Tok.Dot)
			{
				getNextToken();
				ObjectSubProperties.push_back(Tok.text);
				getNextToken();
			}
		}

		std::string ObjectName;
		if (Tok.type == Tok.Directive)
		{
			getNextToken();
			ObjectName = Tok.text;
			getNextToken();

			objectValue.CreateString("Name", ObjectName);
		}

		// Property assigned
		if (Tok.type == Tok.InitAssignment || (Tok.type == Tok.Operator && Tok.text == "="))
		{
			getNextToken();

			if (Tok.type == Tok.IntLiteral || Tok.type == Tok.DoubleLiteral || Tok.type == Tok.FloatLiteral)
			{
				std::string NumberLiteralString = Tok.text;
				getNextToken();

				objectValue.SetNumber(ObjectType, NumberLiteralString);
				//Log("Float PROP: %s %s", ObjectType.c_str(), NumberLiteralString.c_str());

				if (Tok.type == Tok.Operator && Tok.text == "%")
				{
					// This is a percentage!!
					getNextToken();
				}
			}
			else if (Tok.type == Tok.False)
			{
				objectValue.mRawValue = "false";
				getNextToken();
			}
			else if (Tok.type == Tok.True)
			{
				objectValue.mRawValue = "true";
				getNextToken();
			}
			else if (Tok.type == Tok.Identifier || Tok.type == Tok.StringLiteral)
			{

				bool IsIdentifier = Tok.type == Tok.Identifier;

				std::string TheStringLiteral = Tok.extract_stringliteral();
				getNextToken();
				//Log("STRING: %s", TheStringLiteral.c_str());

				// We're in the token after the identifier on RHS
				if (IsIdentifier && Tok.type == Tok.ParenOpen)
				{
					getNextToken();
					std::vector<aeon_token> args;
					do
					{
						TheStringLiteral += " " + Tok.extract_stringliteral();

						getNextToken();

						if (Tok.type == Tok.Comma)
							getNextToken();

					} while (Tok.type != Tok.ParenClose);
					getNextToken();

				}

				objectValue.SetString(ObjectType, TheStringLiteral);

			}
		}
		else if (Tok.type == Tok.BracketOpen)
		{
			getNextToken();

			std::string PropertyName;

			while (Tok.text != "}")
			{
				aeon_value ChildValue = parseDataObject();
				objectValue.properties.push_back(ChildValue);

				/*if (Tok.type == Tok.EndOfFile)
					break;*/
			}
			getNextToken();

			// Add its type since its an object
			objectValue.mValueType = aeon_value::EObject;
			objectValue.SetString("Type", ObjectType);
		}

		if (Tok.type == Tok.Comma || Tok.type == Tok.SemiColon)
		{
			getNextToken();
		}

		// We end on the token that belongs to the next object/value
		return objectValue;
	}

	// Go through all the tokens and build an AST along the way
	void aeon_parser::parse(aeon_lexer& lexer)
	{
		root = new aeNodeModule();
		root->m_name = "main";

		lex = &lexer;
		i = 0;

		Tok = getNextToken();

		/// Eventually we will arrive at the end of file, when all nested levels consume their tokens properly
		while (Tok.type != aeon_token::EndOfFile)
		{
			if (Tok.type == aeon_token::Fn)
			{
				aeNodeFunction* funcDecl = parse_function();
				root->add(funcDecl);

				getNextToken(); // get the next thing for the parent while
			}

			else if (Tok.type == Tok.Class || Tok.type == Tok.Struct)
			{
				aeNodeClass* classDecl = parseClass();
				root->add(classDecl);
			}
			else if (Tok.type == Tok.Namespace)
			{
				aeNodeNamespace* namespace_node = parse_namespace();
				root->add(namespace_node);
			}
			else if (Tok.type == Tok.Using)
			{
				/*aeNodeUsing* using_node = new aeNodeUsing();
				getNextToken();
				using_node->arg = parse_identifier_subexpression();
				getNextToken();
				root->add(using_node);*/
			}
			else
			{
				aeNodeBase* symbol = parse_symbol();
				if (symbol)
				{
					root->add(symbol);
				}
			}
		}
	}

	aeNodeNamespace* aeon_parser::parse_namespace()
	{
		aeNodeNamespace* namespace_node = new aeNodeNamespace;
		namespace_node->m_name = getNextToken().text;

		getNextToken(); getNextToken();

		while (Tok.type != Tok.BracketClose)
		{
			if (Tok.type == aeon_token::Fn)
			{
				aeNodeFunction* funcDecl = parse_function();
				namespace_node->add(funcDecl);

				getNextToken(); // get the next thing for the parent while
			}
			else if (Tok.type == aeon_token::Class)
			{
				aeNodeClass* classDecl = parseClass();
				namespace_node->add(classDecl);
			}
			else if (Tok.type == Tok.Namespace)
			{
				aeNodeNamespace* namespace_node = parse_namespace();
				namespace_node->add(namespace_node);
			}
			else
			{
				aeNodeBase* symbol = parse_symbol();
				if (symbol)
				{
					namespace_node->add(symbol);
				}
			}
		}

		getNextToken();

		return namespace_node;
	}

	// Assumes the current token is "class" and stops the token after the ; at the end
	aeNodeClass* aeon_parser::parseClass()
	{
		aeNodeClass* classDecl = new aeNodeClass();
		classDecl->m_name = getNextToken().text;

		// Will be either a : or a { depending on whether the class inherits stuff
		getNextToken();

		// Get the inheritance fathers
		if (Tok.type == Tok.InitAssignment)
		{
			do
			{
				aeNodeClass::classparentinfo cpi;

				// Let's get the first keyword, can be either a access level or the identifier
				getNextToken();

				if (Tok.type == Tok.Public || Tok.type == Tok.Protected || Tok.type == Tok.Private)
				{
					cpi.accessLevel = Tok.text;
					cpi.parentClass = getNextToken().text;
				}
				else
				{
					cpi.accessLevel = "public";
					cpi.parentClass = Tok.text;
				}

				getNextToken();

				classDecl->parents.push_back(cpi);
			} while (Tok.type == Tok.Comma);
		}

		parseClassBody(classDecl);
		getNextToken(); // we need to skip the last ;
		getNextToken();

		return classDecl;
	}

	/// We're inside a class block, last token read was {
	void aeon_parser::parseClassBody(aeNodeClass* classDeclNode)
	{
		std::string currentDefaultAccessLevel = "public";

		getNextToken(); // get first token

		// Parse on element at a time until the end of class shows
		while (Tok.type != Tok.BracketClose)
		{
			/*if (Tok.type == Tok.Public || Tok.type == Tok.Protected || Tok.type == Tok.Private)
			{
			String accessLevel = Tok.text;
			getNextToken();

			if (Tok.type == Tok.InitAssignment)
			{
			currentDefaultAccessLevel = accessLevel;
			getNextToken();
			}
			else
			{
			// just got to get the class element
			parse_class_element(classDeclNode);
			}
			}
			else
			{*/
			parse_class_element(classDeclNode);
			//}
		}
	}

aeNodeFunction* aeon_parser::parseLambdaFunction()
{
	/*
		Assumes we are on a =>
	*/
	aeNodeFunction* function = new aeNodeFunction();
	function->is_lambda = true;

	getNextToken();
	if(Tok.text == Tok.BracketOpen)
	{
		function->body = parseBlock();
	}
	else
	{
		// Inline anon expression
		function->body = new aeNodeBlock();
		aeNodeExpr* expr = parseExpression();
	}

	return function;
}

aeNodeEnum* aeon_parser::parse_enum()
{
	aeNodeEnum* enum_code = new aeNodeEnum;
	enum_code->name = getNextToken().text;
	getNextToken(); getNextToken();

	while (Tok.type != Tok.BracketClose)
	{
		std::string EnumMember = Tok.text;
		if (getNextToken().type == Tok.Comma)
			{
				enum_code->addField(EnumMember);
				getNextToken();
			}
			else if (Tok.type == Tok.BracketClose)
			{
				enum_code->addField(EnumMember);
			}
	}

	getNextToken();

	return enum_code;
}

	/// Parses one thing inside the class body
	void aeon_parser::parse_class_element(aeNodeClass* classDeclNode)
	{
		if (Tok.type == Tok.Class || Tok.type == Tok.Struct)
		{
			aeNodeClass* classDecl = parseClass();
			classDeclNode->add(classDecl);
		}
		else if (Tok.type == Tok.Enum)
		{
			aeNodeEnum* enum_node = parse_enum();
			classDeclNode->add(enum_node);
		}
		else if (Tok.type == Tok.Typedef)
		{
			/*getNextToken();
			aeNodeTypedef* typedef_node = new aeNodeTypedef;
			typedef_node->typeA = parseQualType();
			typedef_node->typeB = parseQualType();
			classDeclNode->add(typedef_node);*/
		}
		else if (Tok.type == Tok.Identifier && Tok.text == classDeclNode->m_name)
		{
			// Constructor
			aeNodeFunction* constructorDecl = new aeNodeFunction();
			constructorDecl->m_name = classDeclNode->m_name;
			getNextToken();
			getNextToken();

			constructorDecl->m_parameters = parse_parameter_list();
			parseBlock(constructorDecl->m_block.get());
			classDeclNode->add(constructorDecl);
			getNextToken();

			//Log("After constructor %s", Tok.text.c_str());

		}
		else if (Tok.type == Tok.Tilde)
		{
			if (getNextToken().text == classDeclNode->m_name)
			{
				aeNodeFunction* destructorDecl = new aeNodeFunction();
				destructorDecl->m_name = std::string("~") + classDeclNode->m_name;
				getNextToken();
				getNextToken();
				getNextToken();
				parseBlock(destructorDecl->m_block.get());
				classDeclNode->add(destructorDecl);
				getNextToken();
			}
		}
		else if (Tok.type == Tok.Fn)
		{
			aeNodeFunction* funcDecl = parse_function();
			funcDecl->is_method = true;
			funcDecl->is_static = false;
			classDeclNode->add(funcDecl);
			getNextToken();
		}
		else
		{
			aeNodeBase* symbol = parse_symbol();
			classDeclNode->add(symbol);
		}
	}

	/// Parses one symbol, either a variable or a function declaration
	aeNodeBase* aeon_parser::parse_symbol()
	{
		aeNodeBase* result_node = nullptr;
		bool IsFunctionDecl = false;
		bool IsConst = false;
		std::vector<aeNodeExpr*> params;

		if (Tok.type == Tok.Public || Tok.type == Tok.Protected || Tok.type == Tok.Private)
		{
			getNextToken();
		}

		aeQualType type_node = parseQualType();

		std::string SymbolName = Tok.text;

		getNextToken();

		// Let's check if we got a function
		if (Tok.type == Tok.ParenOpen)
		{
			IsFunctionDecl = true;
			getNextToken();
			params = parse_parameter_list();
			getNextToken();
		}

		if (Tok.type == Tok.Const)
		{
			IsConst = true;
			getNextToken();
		}

		// If the brackets are opening, can only be a property or a function body definition
		if (Tok.type == Tok.BracketOpen)
		{
			if (IsFunctionDecl)
			{
				aeNodeFunction* func_node = new aeNodeFunction();
				func_node->m_name = SymbolName;
				func_node->m_parameters = params;
				func_node->is_static = false;
				func_node->is_method = true;
				//func_node->is_method = true;
				parseBlock(func_node->m_block.get());
				result_node = func_node;
				getNextToken();
			}
		}
		else if (Tok.type == Tok.SemiColon)
		{
			// We are done early, this is a function prototype or a variable
			if (IsFunctionDecl)
			{
				aeNodeFunction* func_node = new aeNodeFunction();
				func_node->m_name = SymbolName;
				func_node->is_method = true;
				func_node->is_static = false;
				//parseBlock(func_node->block);
				result_node = func_node;
				getNextToken();
			}
			else
			{
				aeNodeVarRef* var_node = new aeNodeVarRef;
				var_node->m_name = SymbolName;
				var_node->VarType = type_node;
				result_node = var_node;
				getNextToken();
			}
		}

		return result_node;
	}

aeQualType aeon_parser::parseQualType()
{
	aeQualType type;
	type.m_type = ctx->getTypeInfo(Tok.text);
	getNextToken();

		/*type_node->name = Tok.text;
		getNextToken();

		// Let's see if its a templated type
		if (Tok.type == Tok.MarkOpen)
		{
			getNextToken();

			while (Tok.type != Tok.MarkClose)
			{
				aeQualType* template_type_node = parse_type();

				type_node->templateTypeArguments.push_back(template_type_node);

				if (Tok.type == Tok.Comma)
					getNextToken();
			}

			getNextToken();
		}
		*/
	return type;
}

	/// Parses a list of arguments (expressions) to pass to a function for example
	/// Assumes to be already on the first token of the argument list
	std::vector<aeNodeExpr*> aeon_parser::parse_argument_list()
	{
		std::vector<aeNodeExpr*> temp;

		aeNodeExpr* argExpr = nullptr;
		while (argExpr = parseExpression())
		{
			temp.push_back(argExpr);

			if (Tok.type != Tok.Comma)
			{
				break;
			}
			else
				getNextToken();
		}

		return temp;
	}


std::vector<aeNodeExpr*> aeon_parser::parse_parameter_list()
{
		std::vector<aeNodeExpr*> temp;
		/*
		while (Tok.type != Tok.ParenClose)
		{
			aeon_stmt_vardecl* varexpr = parseVariableDecl();
			temp.push_back(varexpr);

			if (Tok.type != Tok.Comma)
				break;
			else
				getNextToken();
		}*/

		return temp;
}

	aeNodeFunction* aeon_parser::parse_function()
	{
		aeNodeFunction* funcDecl = new aeNodeFunction();
		funcDecl->m_name = getNextToken().text;

		// We're now in the (
		getNextToken();

		getNextToken();

		while (Tok.type != Tok.ParenClose)
		{
			funcDecl->m_parameters = parse_parameter_list();
		}

		getNextToken(); // just opened bracket

		parseBlock(funcDecl->m_block.get());

		return funcDecl;
	}

	/// We're inside a function body scope, parse whatever is allowed in there
	void aeon_parser::parseFunctionScope(aeNodeFunction* funcDeclNode)
	{

		while (Tok.type != aeon_token::BracketClose)
			parseBlock(funcDeclNode->m_block.get());

	//	Log("Function body parse complete");
	}


void aeon_parser::parseBlock(aeNodeBlock* block)
{
		//Log("Block start");

		getNextToken();

		//Log("First Tok %s", Tok.str());


		// Let's parse statements
		while (Tok.type != Tok.BracketClose)
		{
			if (Tok.type == Tok.If)
			{
				// if (expr) { block }
				//Log("Parsing if () {}");
				/*
				getNextToken();
				aeNodeBranch* ifbranch = new aeNodeBranch();
				block->add(ifbranch);

				getNextToken(); // go into the first part of the expression
				ifbranch->m_expression = parseExpression();
				if (ifbranch->m_expression)
					ifbranch->add(ifbranch->m_expression);

				//Log("Parsing now the if nested block");

				getNextToken();  //getNextToken(); // consume the {
				parseBlock(ifbranch->m_block);
				*/
				//getNextToken(); // advance to next statement
			}
			else if (Tok.type == Tok.For)
			{
				block->add(parseForLoop());
			}
			else if (Tok.type == Tok.While || Tok.type == Tok.Do)
			{
				block->add(parseWhileLoop());
			}
			else if (Tok.type == Tok.Return) // facing a return statement
			{
				aeNodeReturn* astreturn = new aeNodeReturn();
				block->add(astreturn);
				getNextToken();
			}
			else if (Tok.type == Tok.Identifier && peekAhead(0).type == Tok.Identifier) // facing a var declaration
			{
				do
				{
					aeNodeVarDecl* astvar = parseVariableDecl();
					if (astvar)
					{
						block->add(astvar);
					}
				} while (Tok.type == Tok.Comma);
			}
			else
			{
				aeNodeExpr* expr = parseExpression();
				if (expr)
				{
					block->add(expr);
				}
			}

			// get next so outer statement while can process
			getNextToken();
		}
	//	Log("Reached the end of block");
}

aeNodeFor* aeon_parser::parseForLoop()
{
	aeNodeFor* astfor = new aeNodeFor();

	getNextToken();
	getNextToken();

	aeNodeExpr* initStatement = parseExpression();
	if (initStatement)
		astfor->initExpressions.push_back(initStatement);

	getNextToken();

	astfor->expr.reset(parseExpression());

	getNextToken();

	//astfor->incrExpr = parseExpression();

	//parseBlock(astfor->block);

	return astfor;
}

aeNodeWhile* aeon_parser::parseWhileLoop()
{
	aeNodeWhile* astwhile = new aeNodeWhile();

	if (Tok.type == Tok.While)
	{
		/*astwhile->doWhile = false;
		getNextToken();
		getNextToken();
		astwhile->expr = parseExpression();
		if (astwhile->expr)
			astwhile->add(astwhile->expr);

		getNextToken();
		parseBlock(astwhile->block);*/
	}
	else
	{
		/*astwhile->doWhile = true;
		getNextToken(); // reach {

		parseBlock(astwhile->block);
		getNextToken(); // reach 'while'
		getNextToken(); // reach {
		getNextToken(); // reach first expr token
		astwhile->expr = parseExpression();
		if (astwhile->expr)
			astwhile->add(astwhile->expr);
		getNextToken();*/
	}

	return astwhile;
}

aeNodeVarDecl* aeon_parser::parseVariableDecl()
{
	aeNodeVarDecl* varDecl = new aeNodeVarDecl;
	varDecl->m_type.m_type = ctx->getTypeInfo(Tok.text);

	aeNodeVarRef* astvar = new aeNodeVarRef();
	astvar->explicitDeclaration = true;
	astvar->m_name = getNextToken().text;
	getNextToken();

	varDecl->m_name = astvar->m_name;

	// Got an assignment on init
	if (Tok.text == "=")
	{
		getNextToken();

		aeNodeBinaryOperator* assignment = new aeNodeBinaryOperator(astvar, parseExpression(), "=");
		varDecl->m_initExpr.reset(assignment);
	}

	return varDecl;
}

	/// We're about to read a function call, get it
	aeNodeFunctionCall* aeon_parser::parseFunctionCall()
	{
		aeNodeFunctionCall* funccall = new aeNodeFunctionCall();
		funccall->m_name = Tok.text;
		getNextToken();

		// let's advance to the first token of the expression (arg)
		getNextToken();

		aeNodeExpr* argExpr = nullptr;
		while (argExpr = parseExpression())
		{
			funccall->add(argExpr);

			if (peekAhead(0).type == Tok.Comma)
			{
				getNextToken();
			}
			else
			{
				getNextToken();
			}

			//Log("Parsing arg, %s", argExpr->printtext().c_str());

		}

		getNextToken();

		if (Tok.type != Tok.EndOfFile)
		{
			//Log("im on %s", Tok.text.c_str());
			//Log("parsed function call '%s'", funccall->funcName.c_str());
		}
		return funccall;
	}

aeNodeExpr* aeon_parser::parsePrimaryExpression()
{
	aeNodeExpr* idnt = parseIdentityExpression();
	if (Tok.type == Tok.Dot)
	{
		return parseMemberAccess(idnt);
	}
	return idnt;
}

aeNodeAccessOperator* aeon_parser::parseMemberAccess(aeNodeExpr* left)
{
	// We are on a '.', which HAS to form an operator
	getNextToken();

	aeNodeAccessOperator* dotOp = new aeNodeAccessOperator();
	dotOp->m_a = left;
	dotOp->m_b = parseIdentityExpression();

	if (Tok.type == Tok.Dot)
	{
		return parseMemberAccess(dotOp);
	}
	else
		return dotOp;
}

aeNodeExpr* aeon_parser::parseIdentityExpression()
{
	aeNodeExpr* result = nullptr;
	std::string name = Tok.text;
	getNextToken();

	printf("IDENTITY IS %s\n", name.c_str());

	if (Tok.type == Tok.ParenOpen)
	{
		getNextToken();
		getNextToken();

		aeNodeFunctionCall* fCall = new aeNodeFunctionCall;
		fCall->m_name = name;
		result = fCall;
	}
	else
	{
		aeNodeVarRef* varRef = new aeNodeVarRef;
		varRef->m_name = name;
		result = varRef;
	}

	return result;
}

aeNodeExpr* aeon_parser::parseExpression()
{
		// no way we can get a valid expression here
		if (Tok.type == Tok.ParenClose || Tok.type == Tok.SemiColon || Tok.type == Tok.Comma)
			return nullptr;

		// var_a + var_b + 10 * 10 / 5 + var_c > var_d - var_e++ * (var_c - var_a)

		std::vector<aeNodeExpr*> operands;
		std::vector<aeon_token> operators;

		do
		{
			// handle operator
			if (operands.size() > 0)
			{
				//Log("EXPRESSION OP: %s", Tok.str());
				operators.push_back(Tok);
				getNextToken();
			}

			aeNodeUnaryOperator* unary_wrapper = nullptr;

			// Unary operators prefix
			if (Tok.type == Tok.Operator && Tok.text == "-")
			{
				unary_wrapper = new aeNodeUnaryOperator();
				unary_wrapper->OperatorString = "-";
				getNextToken();
			}

			if (Tok.type == Tok.Decrement) // prefix decrement
			{
				unary_wrapper = new aeNodeUnaryOperator();
				unary_wrapper->OperatorString = "--";
				getNextToken();
			}

			if (Tok.type == Tok.Increment) // prefix increment
			{
				unary_wrapper = new aeNodeUnaryOperator();
				unary_wrapper->OperatorString = "++";
				getNextToken();
			}

			aeNodeExpr* leafexpr = nullptr;

			// Operands

			if (Tok.type == Tok.ParenOpen)
			{
				getNextToken();
				leafexpr = parseExpression();
			//	Log("subexpr result: %s", leafexpr->exprstr().c_str());
				getNextToken();
			}
			else if (Tok.type == Tok.Identifier)  // var or func call or any combination of them
			{
				aeNodeExpr* primaryExpr = parsePrimaryExpression();
				leafexpr = primaryExpr;
			}
			else if (Tok.type == Tok.IntLiteral)
			{
				aeNodeInteger* expr = new aeNodeInteger;
				expr->value = atoi(Tok.text.c_str());
				leafexpr = expr;
				getNextToken();
			}
			else if (Tok.type == Tok.FloatLiteral)
			{
				aeNodeFloat* expr = new aeNodeFloat;
				expr->value = atof(Tok.text.c_str());
				leafexpr = expr;
				getNextToken();
			}
			else if (Tok.type == Tok.DoubleLiteral)
			{
				aeNodeFloat* expr = new aeNodeFloat;
				expr->value = atof(Tok.text.c_str());
				leafexpr = expr;
				getNextToken();
			}
			else if (Tok.type == Tok.StringLiteral)
			{
				aeNodeString* expr = new aeNodeString;
				expr->value = Tok.extract_stringliteral();
				leafexpr = expr;
				getNextToken();
			}

			// Wrap the sub expr if applicable
			if (unary_wrapper)
			{
				unary_wrapper->Operand = leafexpr;
				leafexpr = unary_wrapper;
			}

			operands.push_back(leafexpr);

		} while (Tok.type == Tok.Operator);

		if (operands.size() == 1)
			return operands[0];

		//Log("generating ast for %d ops and %d operands", operators.size(), operands.size());

		// Let's generate the hierarchy
		aeNodeExpr* lhs = operands[0];
		for (int i = 0; i < operators.size(); ++i)
		{
			aeNodeExpr* rhs = operands[i + 1];

			if (i == operators.size() - 1)
			{
				// last op
				lhs = new aeNodeBinaryOperator(lhs, rhs, operators[i].text);
			}
			else if (mOperatorTable[operators[i].text] >= mOperatorTable[operators[i + 1].text])
			{
				aeNodeExpr* rhs = operands[i + 1];
				lhs = new aeNodeBinaryOperator(lhs, rhs, operators[i].text);
			}
			else
			{
				aeNodeExpr* rhs = new aeNodeBinaryOperator(operands[i + 1], operands[i + 2], operators[i + 1].text);
				lhs = new aeNodeBinaryOperator(lhs, rhs, operators[i].text);
				i++;
			}
		}

		//Log("parseExpression generated %s, current tok %s", lhs->exprstr().c_str(), Tok.str());
		return lhs;
	}

aeNodeExpr* aeon_parser::parse_identifier_subexpression()
{
		aeNodeExpr* result_expr = nullptr;

		aeon_token identifier = Tok;

		// What's next? . or <
		getNextToken();

		std::vector<std::string> TemplateTypeString;
		bool IsTemplated = false;
		/*if (Tok.type == Tok.Operator && Tok.text == "<")
		{
		// The type identifier
		do
		{
		TemplateTypeString.push_back(getNextToken().text);
		getNextToken();

		} while (Tok.type == Tok.Comma);

		// Go to the next thing
		getNextToken();

		IsTemplated = true;
		}*/

		bool IsFunctionCall = false;
		if (Tok.type == Tok.ParenOpen)
		{
			IsFunctionCall = true;

			aeNodeFunctionCall* funccall = new aeNodeFunctionCall();
			funccall->m_name = identifier.text;
			result_expr = funccall;

			if (IsTemplated)
			{
				for (std::size_t i = 0; i < TemplateTypeString.size(); ++i)
				{
					aeNodeFunctionCall::TemplateTypeArgument tta;
					tta.TypeString = TemplateTypeString[i];
					funccall->templateTypeArguments.push_back(tta);
				}
			}

			// Go to the first token of the expression
			getNextToken();

			if (Tok.type != Tok.ParenClose)
			{
				funccall->m_args = parse_argument_list();
				//getNextToken();
			}

			getNextToken();
		}
		else
		{
			aeNodeVarRef* var = new aeNodeVarRef;
			var->m_name = identifier.text;
			result_expr = var;

			//getNextToken();
		}

		// Now we might have an array subscript
		/*if (Tok.type == Tok.SqBracketOpen)
		{
			getNextToken();

			aeNodeExpr* subscript_expression = parseExpression();
			if (subscript_expression)
			{
				result_expr->subscriptArgument = subscript_expression;
			}

			// Go beyond the ]
			getNextToken();
		}*/

		if (Tok.type == Tok.Dot)
		{
			result_expr = parseMemberAccess(result_expr);
		}

		return result_expr;
}

	bool aeon_parser::checkForFunction()
	{
		// So, the statement is not started, getNextToken() should be returning the return type
		return peekAhead(2).type == aeon_token::ParenOpen;
	}



	void aeon_parser::parseTopLevel()
	{

	}


	aeon_token aeon_parser::getNextToken()
	{
		if (Tok.type == Tok.EndOfFile)
			return Tok;;

		Tok = lex->tokens[i++];

		while (Tok.type == aeon_token::CommentSingle || Tok.type == aeon_token::CommentMulti)
			Tok = getNextToken();

		return Tok;
	}

	/// Peeks ahead from 1 to N tokens
	aeon_token aeon_parser::peekAhead(int count)
	{
		return lex->tokens[i + count];
	}

void aeon_parser::printAST()
{
	root->printSelf(0);
}
