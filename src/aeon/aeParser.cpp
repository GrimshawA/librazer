#include <AEON/aeParser.h>
#include <AEON/aeTokenizer.h>
#include <Rzr/RzEngine.h>
#include <AEON/AST/Nodes.h>
#include <AEON/AST/aeNodeValue.h>

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


AEStructNode::Visibility TokToVisib(const std::string& str)
{
	if (str == "public")
		return AEStructNode::VISIBILITY_PUBLIC;
	if (str == "protected")
		return AEStructNode::VISIBILITY_PROTECTED;
	if (str == "private")
		return AEStructNode::VISIBILITY_PRIVATE;

	return AEStructNode::VISIBILITY_PUBLIC;
}

std::unique_ptr<aeParser> aeParser::create(const std::string& source, RzEngine* context)
{
	std::unique_ptr<aeParser> parser(new aeParser(source));
	parser->i = 0; parser->ctx = context; parser->getNextToken();
	return std::move(parser);
}

aeParser::aeParser()
{
	pass = Gather;
}

aeParser::aeParser(const std::string& source)
{
	pass = Gather;
	m_customTokenizer.reset(new aeon_lexer());
	m_tokenizer = m_customTokenizer.get();
	m_tokenizer->tokenize(source);
}

AEStmtNode* aeParser::parseStatement()
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
		if (peekAhead(0).type == AETK_IDENTIFIER || peekAhead(0).type == AETK_HANDLE)
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

aeNodeReturn* aeParser::parseReturn()
{
	aeNodeReturn* returnNode = new aeNodeReturn;
	getNextToken();
	return returnNode;
}

aeNodeBranch* aeParser::parseBranch()
{
	aeNodeBranch* branchNode = new aeNodeBranch;

	getNextToken();

	getNextToken(); // go into the first part of the expression
	branchNode->m_expression.reset(parseExpression());
		
	getNextToken();  //getNextToken(); // consume the {
	
	branchNode->m_block.reset(parseBlock());
	
	//getNextToken(); // advance to next statement
	return branchNode;
}

void aeParser::startParse(aeon_lexer& lexer)
{
		root = new aeNodeModule();
		root->m_name = "main";

		m_tokenizer = &lexer;
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
				AEStructNode* classDecl = parseClass();
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
				AEBaseNode* symbol = parseSymbol();
				if (symbol)
				{
					root->add(symbol);
				}
			}
		}
}

	aeNodeNamespace* aeParser::parseNamespace()
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
				AEStructNode* classDecl = parseClass();
				namespace_node->add(classDecl);
			}
			else if (Tok.type == AETK_NAMESPACE)
			{
				aeNodeNamespace* namespace_node = parseNamespace();
				namespace_node->add(namespace_node);
			}
			else
			{
				AEBaseNode* symbol = parseSymbol();
				if (symbol)
				{
					namespace_node->add(symbol);
				}
			}
		}

		getNextToken();

		return namespace_node;
	}

AEStructNode* aeParser::parseClass()
{
	AEStructNode* classDecl = new AEStructNode();
	classDecl->m_name = getNextToken().text;

	// Will be either a : or a { depending on whether the class inherits stuff
	getNextToken();

	// Get the inheritance fathers
	if (Tok.type == AETK_COLON)
	{
			do
			{
				AEStructNode::classparentinfo cpi;

				// Let's get the first keyword, can be either a access level or the identifier
				getNextToken();

				if (Tok.type == AETK_PUBLIC || Tok.type == AETK_PROTECTED || Tok.type == AETK_PRIVATE)
				{
					cpi.visibility = TokToVisib(Tok.text);
					cpi.parentClass = getNextToken().text;
				}
				else
				{
					cpi.visibility = TokToVisib("public");
					cpi.parentClass = Tok.text;
				}

				getNextToken();

				classDecl->parents.push_back(cpi);
			} while (Tok.type == AETK_COMMA);
		}

		parseClassBody(classDecl);
		getNextToken(); // we need to skip the last ;
		getNextToken();

		// debug print
		printf("Parsed class %s\n%s\n", classDecl->m_name.c_str(), classDecl->str().c_str());

		return classDecl;
}

void aeParser::parseClassBody(AEStructNode* classDeclNode)
{
	std::string currentDefaultAccessLevel = "public";

	getNextToken(); // get first token

	// Parse on element at a time until the end of class shows
	while (Tok.type != AETK_CLOSEBRACKET)
	{
		std::string useVisib;

		begin: 

		if (Tok.type == AETK_PUBLIC || Tok.type == AETK_PRIVATE || Tok.type == AETK_PROTECTED)
		{
			std::string visib = Tok.text;
			getNextToken();

			if (Tok.type == AETK_COLON)
			{
				currentDefaultAccessLevel = visib;
				printf("CHANGED DEFAULT VISIB\n");
				getNextToken();
				goto begin;
			}
			else
				useVisib = visib;
		}

		if (Tok.type == AETK_FUNCTION)
		{
			aeNodeFunction* f = parseFunction();
			f->visibility = useVisib.empty() ? TokToVisib(currentDefaultAccessLevel) : TokToVisib(useVisib);
			f->is_method = true;
			useVisib.clear();
			classDeclNode->m_functions.emplace_back(f);
		}
		else if (Tok.type == AETK_ENUM)
		{
			AEEnumNode* e = parseEnum();
			classDeclNode->m_enums.emplace_back(e);
		}
		else{
			AEFieldNode* field = parseStructField();
			field->visibility = useVisib.empty() ? TokToVisib(currentDefaultAccessLevel) : TokToVisib(useVisib);
			useVisib.clear();			
			classDeclNode->m_fields.push_back(field);			
		}
	}
}

AEFieldNode* aeParser::parseStructField()
{
	AEFieldNode* f = new AEFieldNode;
	f->name = Tok.text;
	getNextToken();

	if (Tok.type == AETK_COLON)
	{
		getNextToken();

		if (Tok.type == AETK_IDENTIFIER && peekAhead(0).type == AETK_OPENBRACKET)
		{
			std::string typeIdentifier = Tok.text;
			getNextToken();
			getNextToken();
			getNextToken();

			f->initializer = new AEFieldInitNode;
			f->initializer->value = new AEObjectInitNode(typeIdentifier);
			f->deduceStaticType(ctx);

			if (!f->type)
			{
				printf("Field '%s': unknown type %s\n", f->name.c_str(), typeIdentifier.c_str());
			}
		}
		else
		{
			auto node = parsePropertyValue();
			f->initializer = new AEFieldInitNode;
			f->initializer->value = node;
			f->deduceStaticType(ctx);
		}
	}
	else
		f->type = ctx->getTypeInfo("var");

	return f;
}

void aeParser::parseClassMember(AEStructNode* classDeclNode)
{
	if (Tok.type == AETK_CLASS || Tok.type == AETK_STRUCT)
	{
		AEStructNode* classDecl = parseClass();
		classDeclNode->add(classDecl);
	}
	else if (Tok.type == AETK_ENUM)
	{
		AEEnumNode* enum_node = parseEnum();
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
		if (funcDecl->m_name == classDeclNode->m_name)
			funcDecl->is_constructor = true;
		classDeclNode->add(funcDecl);
		getNextToken();
	}
	else
	{
		AEBaseNode* symbol = parseSymbol();
		classDeclNode->add(symbol);
	}
}

AEBaseNode* aeParser::parseSymbol()
{
	AEBaseNode* result_node = nullptr;
	bool IsFunctionDecl = false;
	bool IsConst = false;
	std::vector<aeNodeVarDecl*> params;

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
			aeNodeIdentifier* var_node = new aeNodeIdentifier;
			var_node->m_name = SymbolName;
			var_node->VarType = type_node;
			result_node = var_node;
			getNextToken();
		}
	}

	return result_node;
}

bool aeParser::matchesVarDecl()
{
	if (Tok.type == AETK_IDENTIFIER && (peekAhead(0).type == AETK_IDENTIFIER || peekAhead(0).type == AETK_HANDLE || peekAhead(0).text == "<"))
		return true;

	return false;
}

aeNodeFunction* aeParser::parseLambdaFunction()
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

AEEnumNode* aeParser::parseEnum()
{
	AEEnumNode* enum_code = new AEEnumNode;
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

aeQualType aeParser::parseQualType()
{
	aeQualType type;
	type.m_type = ctx->getTypeInfo(Tok.text);
	getNextToken();

	if (Tok.text == "<")
	{
		getNextToken();
		aeQualType templArg = parseQualType();
		getNextToken();
		type.m_templateArgs.push_back(templArg);
	}

	if (Tok.type == AETK_HANDLE)
	{
		type.m_handle = true;
		getNextToken();
	}

	return type;
}

	/// Parses a list of arguments (expressions) to pass to a function for example
	/// Assumes to be already on the first token of the argument list
	std::vector<aeNodeExpr*> aeParser::parseArgsList()
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


std::vector<aeNodeVarDecl*> aeParser::parseParamsList()
{
	std::vector<aeNodeVarDecl*> temp;
	while (Tok.type != AETK_CLOSEPAREN)
	{
		aeNodeVarDecl* varexpr = parseVariableDecl();
		temp.push_back(varexpr);
		if (Tok.type != AETK_COMMA)
			break;
		else
			getNextToken();
	}
	return temp;
}

void aeParser::serialize(const std::string& filename)
{
	
}

void aeParser::print()
{
	root->printSelf(0);
}

aeNodeFunction* aeParser::parseFunction()
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
	getNextToken();

	if (Tok.type == AETK_ARROW)
	{
		getNextToken();

		funcDecl->m_returnType = parseQualType();
		printf("Parsed return of func  %s\n", funcDecl->m_returnType.str().c_str());
	}

	funcDecl->m_block.reset(parseBlock());

	getNextToken();

	return funcDecl;
}

aeNodeBlock* aeParser::parseBlock()
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

aeNodeFor* aeParser::parseForLoop()
{
	aeNodeFor* astfor = new aeNodeFor();
	getNextToken();
	getNextToken();

	if (matchesVarDecl())
		astfor->initStatement.reset(parseVariableDecl());
	else
		astfor->initStatement.reset(parseExpression());

	getNextToken();

	astfor->expr.reset(parseExpression());

	getNextToken();

	astfor->incrExpr.reset(parseExpression());

	getNextToken();

	astfor->block.reset(parseBlock());

	return astfor;
}

aeNodeWhile* aeParser::parseWhileLoop()
{
	aeNodeWhile* whileNode = new aeNodeWhile();

	if (Tok.type == AETK_WHILE)
	{
		whileNode->doWhile = false;

		getNextToken();
		getNextToken();
		whileNode->expr.reset(parseExpression());
		getNextToken();
		whileNode->block.reset(parseBlock());
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

	return whileNode;
}

aeNodeVarDecl* aeParser::parseVariableDecl()
{
	aeNodeVarDecl* varDecl = new aeNodeVarDecl;
	varDecl->m_type = parseQualType();

	//printf("var decl type %s\n", varDecl->m_type.str().c_str());

	std::string varName = Tok.text;
	getNextToken();

	aeNodeVarDecl::Decl varDef;
	varDef.m_name = varName;
	varDecl->m_decls.push_back(varDef);

	// Got an assignment on init
	if (Tok.text == "=")
	{
		getNextToken();

		aeNodeIdentifier* astvar = new aeNodeIdentifier();
		astvar->explicitDeclaration = true;
		astvar->m_name = varName;

		aeNodeBinaryOperator* assignment = new aeNodeBinaryOperator(astvar, parseExpression(), "=");
		varDecl->m_decls[0].m_init = (assignment);
	}

	return varDecl;
}

	/// We're about to read a function call, get it
	aeNodeFunctionCall* aeParser::parseFunctionCall()
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

aeNodeExpr* aeParser::parsePrimaryExpression()
{
	aeNodeExpr* idnt = parseIdentityExpression();
	if (Tok.type == AETK_DOT)
	{
		return parseMemberAccess(idnt);
	}
	return idnt;
}

aeNodeAccessOperator* aeParser::parseMemberAccess(aeNodeExpr* left)
{
	// We are on a '.', which HAS to form an operator
	getNextToken();

	aeNodeAccessOperator* dotOp = new aeNodeAccessOperator();
	dotOp->m_a = left;
	dotOp->m_b = parseIdentityExpression();

	dotOp->m_a->m_parentExpr = dotOp;
	dotOp->m_b->m_parentExpr = dotOp;

	if (Tok.type == AETK_DOT)
	{
		return parseMemberAccess(dotOp);
	}
	else
		return dotOp;
}

aeNodeExpr* aeParser::parseIdentityExpression()
{
	aeNodeExpr* result = nullptr;
	std::string name = Tok.text;
	getNextToken();

	//printf("IDENTITY IS %s\n", name.c_str());

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
		aeNodeIdentifier* varRef = new aeNodeIdentifier;
		varRef->m_name = name;
		result = varRef;
	}

	return result;
}

aeNodeExpr* aeParser::parseExpression()
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

aeNodeExpr* aeParser::parse_identifier_subexpression()
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
			aeNodeIdentifier* var = new aeNodeIdentifier;
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

	bool aeParser::checkForFunction()
	{
		// So, the statement is not started, getNextToken() should be returning the return type
		return peekAhead(2).type == AETK_OPENPAREN;
	}



	void aeParser::parseTopLevel()
	{

	}


	aeon_token aeParser::getNextToken()
	{
		if (Tok.type == AETK_EOF)
			return Tok;;

		Tok = m_tokenizer->tokens[i++];

		while (Tok.type == AETK_LINECOMMENT || Tok.type == AETK_MULTICOMMENT)
			Tok = getNextToken();

		return Tok;
	}

	/// Peeks ahead from 1 to N tokens
	aeon_token aeParser::peekAhead(int count)
	{
		return m_tokenizer->tokens[i + count];
	}

void aeParser::printAST()
{
	root->printSelf(0);
}
