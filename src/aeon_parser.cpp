#include "aeon_parser.h"
#include "aeon_lexer.h"
#include "aeon_context.h"
#include "nodes/aeNodeClass.h"
#include "nodes/aeNodeModule.h"
#include "nodes/aeNodeNamespace.h"
#include "aeon_lexer.h"
#include <map>
#include <cstdlib>

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

aeNodeStatement* aeon_parser::parseStatement()
{
	switch (Tok.type)
	{
	case AETK_IF:
		return parseBranch();
		break;

	case AETK_FOR:
		return parseForLoop();
		break;

	case AETK_WHILE:
		return parseWhileLoop();
		break;

	case AETK_DO:
		return parseWhileLoop();
		break;

	case AETK_RETURN:
		return parseReturn();
		break;

	case AETK_OPENBRACKET:
		return parseBlock();
		break;

	case AETK_IDENTIFIER:
	{
		if (peekAhead(0).type == AETK_IDENTIFIER)
		{
			return parseVariableDecl();
		}
	}
	default:
	{
		return parseExpression();
	}
		
	}
}

aeNodeReturn* aeon_parser::parseReturn()
{
	aeNodeReturn* returnNode = new aeNodeReturn;
	getNextToken();
	return returnNode;
}

aeNodeBranch* aeon_parser::parseBranch()
{
	aeNodeBranch* branchNode = new aeNodeBranch;
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
	return branchNode;
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

		while (Tok.type != AETK_EOF)
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
		if (Tok.type == AETK_IDENTIFIER)
		{
			ObjectType = Tok.text;
			getNextToken();
		}

		// Found a member-of property
		std::vector<std::string> ObjectSubProperties;
		if (Tok.type == AETK_DOT)
		{
			while (Tok.type == AETK_DOT)
			{
				getNextToken();
				ObjectSubProperties.push_back(Tok.text);
				getNextToken();
			}
		}

		std::string ObjectName;
		if (Tok.type == AETK_DIRECTIVE)
		{
			getNextToken();
			ObjectName = Tok.text;
			getNextToken();

			objectValue.CreateString("Name", ObjectName);
		}

		// Property assigned
		if ((Tok.type == AETK_BINOP && Tok.text == "="))
		{
			getNextToken();

			if (Tok.type == AETK_INTLITERAL || Tok.type == AETK_FLOATLITERAL)
			{
				std::string NumberLiteralString = Tok.text;
				getNextToken();

				objectValue.SetNumber(ObjectType, NumberLiteralString);
				//Log("Float PROP: %s %s", ObjectType.c_str(), NumberLiteralString.c_str());

				if (Tok.type == AETK_BINOP && Tok.text == "%")
				{
					// This is a percentage!!
					getNextToken();
				}
			}
			else if (Tok.type == AETK_FALSE)
			{
				objectValue.mRawValue = "false";
				getNextToken();
			}
			else if (Tok.type == AETK_TRUE)
			{
				objectValue.mRawValue = "true";
				getNextToken();
			}
			else if (Tok.type == AETK_IDENTIFIER || Tok.type == AETK_STRINGLITERAL)
			{

				bool IsIdentifier = Tok.type == AETK_IDENTIFIER;

				std::string TheStringLiteral = Tok.extract_stringliteral();
				getNextToken();
				//Log("STRING: %s", TheStringLiteral.c_str());

				// We're in the token after the identifier on RHS
				if (IsIdentifier && Tok.type == AETK_OPENPAREN)
				{
					getNextToken();
					std::vector<aeon_token> args;
					do
					{
						TheStringLiteral += " " + Tok.extract_stringliteral();

						getNextToken();

						if (Tok.type == AETK_COMMA)
							getNextToken();

					} while (Tok.type != AETK_CLOSEPAREN);
					getNextToken();

				}

				objectValue.SetString(ObjectType, TheStringLiteral);

			}
		}
		else if (Tok.type == AETK_OPENBRACKET)
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

		if (Tok.type == AETK_COMMA || Tok.type == AETK_SEMICOLON)
		{
			getNextToken();
		}

		// We end on the token that belongs to the next object/value
		return objectValue;
	}

	// Go through all the tokens and build an AST along the way
	void aeon_parser::startParse(aeon_lexer& lexer)
	{
		root = new aeNodeModule();
		root->m_name = "main";

		lex = &lexer;
		i = 0;

		Tok = getNextToken();

		/// Eventually we will arrive at the end of file, when all nested levels consume their tokens properly
		while (Tok.type != AETK_EOF)
		{
			if (Tok.type == AETK_FUNCTION)
			{
				aeNodeFunction* funcDecl = parseFunction();
				root->add(funcDecl);

				getNextToken(); // get the next thing for the parent while
			}

			else if (Tok.type == AETK_CLASS || Tok.type == AETK_STRUCT)
			{
				aeNodeClass* classDecl = parseClass();
				root->add(classDecl);
			}
			else if (Tok.type == AETK_NAMESPACE)
			{
				aeNodeNamespace* namespace_node = parseNamespace();
				root->add(namespace_node);
			}
			else if (Tok.type == AETK_USING)
			{
				/*aeNodeUsing* using_node = new aeNodeUsing();
				getNextToken();
				using_node->arg = parse_identifier_subexpression();
				getNextToken();
				root->add(using_node);*/
			}
			else
			{
				aeNodeBase* symbol = parseSymbol();
				if (symbol)
				{
					root->add(symbol);
				}
			}
		}
	}

	aeNodeNamespace* aeon_parser::parseNamespace()
	{
		aeNodeNamespace* namespace_node = new aeNodeNamespace;
		namespace_node->m_name = getNextToken().text;

		getNextToken(); getNextToken();

		while (Tok.type != AETK_CLOSEBRACKET)
		{
			if (Tok.type == AETK_FUNCTION)
			{
				aeNodeFunction* funcDecl = parseFunction();
				namespace_node->add(funcDecl);

				getNextToken(); // get the next thing for the parent while
			}
			else if (Tok.type == AETK_CLASS)
			{
				aeNodeClass* classDecl = parseClass();
				namespace_node->add(classDecl);
			}
			else if (Tok.type == AETK_NAMESPACE)
			{
				aeNodeNamespace* namespace_node = parseNamespace();
				namespace_node->add(namespace_node);
			}
			else
			{
				aeNodeBase* symbol = parseSymbol();
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
		if (Tok.type == AETK_COLON)
		{
			do
			{
				aeNodeClass::classparentinfo cpi;

				// Let's get the first keyword, can be either a access level or the identifier
				getNextToken();

				if (Tok.type == AETK_PUBLIC || Tok.type == AETK_PROTECTED || Tok.type == AETK_PRIVATE)
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
			} while (Tok.type == AETK_COMMA);
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
		while (Tok.type != AETK_CLOSEBRACKET)
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
			parseClassMember(classDeclNode);
			//}
		}
	}

aeNodeFunction* aeon_parser::parseLambdaFunction()
{
	/*
		Assumes we are on a =>
	*/
	aeNodeFunction* function = new aeNodeFunction();
	function->is_anon = true;

	getNextToken();
	if(Tok.type == AETK_OPENBRACKET)
	{
		function->m_block.reset(parseBlock());
	}
	else
	{
		// Inline anon expression
		function->m_block.reset(new aeNodeBlock());
		aeNodeExpr* expr = parseExpression();
	}

	return function;
}

aeNodeEnum* aeon_parser::parseEnum()
{
	aeNodeEnum* enum_code = new aeNodeEnum;
	enum_code->name = getNextToken().text;
	getNextToken(); getNextToken();

	while (Tok.type != AETK_CLOSEBRACKET)
	{
		std::string EnumMember = Tok.text;
		if (getNextToken().type == AETK_COMMA)
			{
				enum_code->addField(EnumMember);
				getNextToken();
			}
			else if (Tok.type == AETK_CLOSEBRACKET)
			{
				enum_code->addField(EnumMember);
			}
	}

	getNextToken();

	return enum_code;
}

	/// Parses one thing inside the class body
	void aeon_parser::parseClassMember(aeNodeClass* classDeclNode)
	{
		if (Tok.type == AETK_CLASS || Tok.type == AETK_STRUCT)
		{
			aeNodeClass* classDecl = parseClass();
			classDeclNode->add(classDecl);
		}
		else if (Tok.type == AETK_ENUM)
		{
			aeNodeEnum* enum_node = parseEnum();
			classDeclNode->add(enum_node);
		}
		else if (Tok.type == AETK_TYPEDEF)
		{
			/*getNextToken();
			aeNodeTypedef* typedef_node = new aeNodeTypedef;
			typedef_node->typeA = parseQualType();
			typedef_node->typeB = parseQualType();
			classDeclNode->add(typedef_node);*/
		}
		else if (Tok.type == AETK_IDENTIFIER && Tok.text == classDeclNode->m_name)
		{
			// Constructor
			aeNodeFunction* constructorDecl = new aeNodeFunction();
			constructorDecl->m_name = classDeclNode->m_name;
			getNextToken();
			getNextToken();

			constructorDecl->m_parameters = parseParamsList();
			constructorDecl->m_block.reset(parseBlock());
			classDeclNode->add(constructorDecl);
			getNextToken();

			//Log("After constructor %s", Tok.text.c_str());

		}
		else if (Tok.type == AETK_TILDE)
		{
			if (getNextToken().text == classDeclNode->m_name)
			{
				aeNodeFunction* destructorDecl = new aeNodeFunction();
				destructorDecl->m_name = std::string("~") + classDeclNode->m_name;
				getNextToken();
				getNextToken();
				getNextToken();
				destructorDecl->m_block.reset(parseBlock());
				classDeclNode->add(destructorDecl);
				getNextToken();
			}
		}
		else if (Tok.type == AETK_FUNCTION)
		{
			aeNodeFunction* funcDecl = parseFunction();
			funcDecl->is_method = true;
			funcDecl->is_static = false;
			classDeclNode->add(funcDecl);
			getNextToken();
		}
		else
		{
			aeNodeBase* symbol = parseSymbol();
			classDeclNode->add(symbol);
		}
	}

	/// Parses one symbol, either a variable or a function declaration
	aeNodeBase* aeon_parser::parseSymbol()
	{
		aeNodeBase* result_node = nullptr;
		bool IsFunctionDecl = false;
		bool IsConst = false;
		std::vector<aeNodeExpr*> params;

		if (Tok.type == AETK_PUBLIC || Tok.type == AETK_PROTECTED || Tok.type == AETK_PRIVATE)
		{
			getNextToken();
		}

		aeQualType type_node = parseQualType();

		std::string SymbolName = Tok.text;

		getNextToken();

		// Let's check if we got a function
		if (Tok.type == AETK_OPENPAREN)
		{
			IsFunctionDecl = true;
			getNextToken();
			params = parseParamsList();
			getNextToken();
		}

		if (Tok.type == AETK_CONST)
		{
			IsConst = true;
			getNextToken();
		}

		// If the brackets are opening, can only be a property or a function body definition
		if (Tok.type == AETK_OPENBRACKET)
		{
			if (IsFunctionDecl)
			{
				aeNodeFunction* func_node = new aeNodeFunction();
				func_node->m_name = SymbolName;
				func_node->m_parameters = params;
				func_node->is_static = false;
				func_node->is_method = true;
				//func_node->is_method = true;
				func_node->m_block.reset(parseBlock());
				result_node = func_node;
				getNextToken();
			}
		}
		else if (Tok.type == AETK_SEMICOLON)
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
				aeNodeRef* var_node = new aeNodeRef;
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
	std::vector<aeNodeExpr*> aeon_parser::parseArgsList()
	{
		std::vector<aeNodeExpr*> temp;

		aeNodeExpr* argExpr = nullptr;
		while (argExpr = parseExpression())
		{
			temp.push_back(argExpr);

			if (Tok.type != AETK_COMMA)
			{
				break;
			}
			else
				getNextToken();
		}

		return temp;
	}


std::vector<aeNodeExpr*> aeon_parser::parseParamsList()
{
	std::vector<aeNodeExpr*> temp;
	while (Tok.type != AETK_CLOSEPAREN)
	{
		aeNodeVarDecl* varexpr = parseVariableDecl();
		//temp.push_back(varexpr);
		if (Tok.type != AETK_COMMA)
			break;
		else
			getNextToken();
	}
	return temp;
}

void aeon_parser::serialize(const std::string& filename)
{
	
}

void aeon_parser::print()
{
	root->printSelf(0);
}

aeNodeFunction* aeon_parser::parseFunction()
{
		aeNodeFunction* funcDecl = new aeNodeFunction();
		funcDecl->m_name = getNextToken().text;

		// We're now in the (
		getNextToken();

		getNextToken();

		while (Tok.type != AETK_CLOSEPAREN)
		{
			funcDecl->m_parameters = parseParamsList();
		}

		getNextToken(); // just opened bracket

		funcDecl->m_block.reset(parseBlock());

		return funcDecl;
}

aeNodeBlock* aeon_parser::parseBlock()
{
	aeNodeBlock* block = new aeNodeBlock;
	getNextToken();

	while (Tok.type != AETK_CLOSEBRACKET)
	{
		block->add(parseStatement());

			
			if (Tok.type == AETK_IDENTIFIER && peekAhead(0).type == AETK_IDENTIFIER) // facing a var declaration
			{
				
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

	return block;
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

	if (Tok.type == AETK_WHILE)
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

	std::string varName = getNextToken().text;
	getNextToken();

	aeNodeVarDecl::Decl varDef;
	varDef.m_name = varName;
	varDecl->m_decls.push_back(varDef);

	// Got an assignment on init
	if (Tok.text == "=")
	{
		getNextToken();

		aeNodeRef* astvar = new aeNodeRef();
		astvar->explicitDeclaration = true;
		astvar->m_name = varName;

		aeNodeBinaryOperator* assignment = new aeNodeBinaryOperator(astvar, parseExpression(), "=");
		varDecl->m_decls[0].m_init = (assignment);
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

			if (peekAhead(0).type == AETK_COMMA)
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

		if (Tok.type != AETK_EOF)
		{
			//Log("im on %s", Tok.text.c_str());
			//Log("parsed function call '%s'", funccall->funcName.c_str());
		}
		return funccall;
	}

aeNodeExpr* aeon_parser::parsePrimaryExpression()
{
	aeNodeExpr* idnt = parseIdentityExpression();
	if (Tok.type == AETK_DOT)
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

	if (Tok.type == AETK_DOT)
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

	if (Tok.type == AETK_OPENPAREN)
	{
		aeNodeFunctionCall* fCall = new aeNodeFunctionCall;

		getNextToken();

		if (Tok.type != AETK_CLOSEPAREN)
		{
			fCall->m_args = parseArgsList();
		}

		getNextToken();

		fCall->m_name = name;
		result = fCall;
	}
	else
	{
		aeNodeRef* varRef = new aeNodeRef;
		varRef->m_name = name;
		result = varRef;
	}

	return result;
}

aeNodeExpr* aeon_parser::parseExpression()
{
		// no way we can get a valid expression here
		if (Tok.type == AETK_CLOSEPAREN || Tok.type == AETK_SEMICOLON || Tok.type == AETK_COMMA)
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
			if (Tok.type == AETK_BINOP && Tok.text == "-")
			{
				unary_wrapper = new aeNodeUnaryOperator();
				unary_wrapper->OperatorString = "-";
				getNextToken();
			}

			if (Tok.type == AETK_DECREMENT) // prefix decrement
			{
				unary_wrapper = new aeNodeUnaryOperator();
				unary_wrapper->OperatorString = "--";
				getNextToken();
			}

			if (Tok.type == AETK_INCREMENT) // prefix increment
			{
				unary_wrapper = new aeNodeUnaryOperator();
				unary_wrapper->OperatorString = "++";
				getNextToken();
			}

			aeNodeExpr* leafexpr = nullptr;

			// Operands

			if (Tok.type == AETK_OPENPAREN)
			{
				getNextToken();
				leafexpr = parseExpression();
			//	Log("subexpr result: %s", leafexpr->exprstr().c_str());
				getNextToken();
			}
			else if (Tok.type == AETK_IDENTIFIER)  // var or func call or any combination of them
			{
				aeNodeExpr* primaryExpr = parsePrimaryExpression();
				leafexpr = primaryExpr;
			}
			else if (Tok.type == AETK_INTLITERAL)
			{
				aeNodeInteger* expr = new aeNodeInteger;
				expr->value = atoi(Tok.text.c_str());
				leafexpr = expr;
				getNextToken();
			}
			else if (Tok.type == AETK_FLOATLITERAL)
			{
				aeNodeFloat* expr = new aeNodeFloat;
				expr->value = atof(Tok.text.c_str());
				leafexpr = expr;
				getNextToken();
			}
			else if (Tok.type == AETK_STRINGLITERAL)
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

		} while (Tok.type == AETK_BINOP);

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
		if (Tok.type == AETK_OPENPAREN)
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

			if (Tok.type != AETK_CLOSEPAREN)
			{
				funccall->m_args = parseArgsList();
				//getNextToken();
			}

			getNextToken();
		}
		else
		{
			aeNodeRef* var = new aeNodeRef;
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

		if (Tok.type == AETK_DOT)
		{
			result_expr = parseMemberAccess(result_expr);
		}

		return result_expr;
}

	bool aeon_parser::checkForFunction()
	{
		// So, the statement is not started, getNextToken() should be returning the return type
		return peekAhead(2).type == AETK_OPENPAREN;
	}



	void aeon_parser::parseTopLevel()
	{

	}


	aeon_token aeon_parser::getNextToken()
	{
		if (Tok.type == AETK_EOF)
			return Tok;;

		Tok = lex->tokens[i++];

		while (Tok.type == AETK_LINECOMMENT || Tok.type == AETK_MULTICOMMENT)
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
