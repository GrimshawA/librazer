#include "aeon_parser.h"
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
		if (Tok.type == Tok.Period)
		{
			while (Tok.type == Tok.Period)
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
		root = new ast_module();
		root->ModuleName = "main";

		lex = &lexer;
		i = 0;

		Tok = getNextToken();

		/// Eventually we will arrive at the end of file, when all nested levels consume their tokens properly
		while (Tok.type != aeon_token::EndOfFile)
		{
			if (Tok.type == aeon_token::Fn)
			{
				ast_func* funcDecl = parse_function();
				root->add(funcDecl);

				getNextToken(); // get the next thing for the parent while
			}

			else if (Tok.type == Tok.Class || Tok.type == Tok.Struct)
			{
				ast_class* classDecl = parseClass();
				root->add(classDecl);
			}
			else if (Tok.type == Tok.Namespace)
			{
				ast_namespace* namespace_node = parse_namespace();
				root->add(namespace_node);
			}
			else if (Tok.type == Tok.Using)
			{
				ast_using* using_node = new ast_using();
				getNextToken();
				using_node->arg = parse_identifier_subexpression();
				getNextToken();
				root->add(using_node);
			}
			else
			{
				atom_ast_node* symbol = parse_symbol();
				if (symbol)
				{
					root->add(symbol);
				}
			}
		}
	}

	ast_namespace* aeon_parser::parse_namespace()
	{
		ast_namespace* namespace_node = new ast_namespace;
		namespace_node->Name = getNextToken().text;

		getNextToken(); getNextToken();

		while (Tok.type != Tok.BracketClose)
		{
			if (Tok.type == aeon_token::Fn)
			{
				ast_func* funcDecl = parse_function();
				namespace_node->add(funcDecl);

				getNextToken(); // get the next thing for the parent while
			}
			else if (Tok.type == aeon_token::Class)
			{
				ast_class* classDecl = parseClass();
				namespace_node->add(classDecl);
			}
			else if (Tok.type == Tok.Namespace)
			{
				ast_namespace* namespace_node = parse_namespace();
				namespace_node->add(namespace_node);
			}
			else
			{
				atom_ast_node* symbol = parse_symbol();
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
	ast_class* aeon_parser::parseClass()
	{
		ast_class* classDecl = new ast_class();
		classDecl->Name = getNextToken().text;

		// Will be either a : or a { depending on whether the class inherits stuff
		getNextToken();

		// Get the inheritance fathers
		if (Tok.type == Tok.InitAssignment)
		{
			do
			{
				ast_class::classparentinfo cpi;

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
	void aeon_parser::parseClassBody(ast_class* classDeclNode)
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

	ast_enum* aeon_parser::parse_enum()
	{
		ast_enum* enum_code = new ast_enum;
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
	void aeon_parser::parse_class_element(ast_class* classDeclNode)
	{
		if (Tok.type == Tok.Class || Tok.type == Tok.Struct)
		{
			ast_class* classDecl = parseClass();
			classDeclNode->add(classDecl);
		}
		else if (Tok.type == Tok.Enum)
		{
			ast_enum* enum_node = parse_enum();
			classDeclNode->add(enum_node);
		}
		else if (Tok.type == Tok.Typedef)
		{
			getNextToken();
			ast_typedef* typedef_node = new ast_typedef;
			typedef_node->typeA = parse_type();
			typedef_node->typeB = parse_type();
			classDeclNode->add(typedef_node);
		}
		else if (Tok.type == Tok.Identifier && Tok.text == classDeclNode->Name)
		{
			// Constructor
			ast_func* constructorDecl = new ast_func();
			constructorDecl->Name = classDeclNode->Name;
			getNextToken();
			getNextToken();

			constructorDecl->parameters = parse_parameter_list();
			parseBlock(constructorDecl->block);
			classDeclNode->add(constructorDecl);
			getNextToken();

			//Log("After constructor %s", Tok.text.c_str());

		}
		else if (Tok.type == Tok.Tilde)
		{
			if (getNextToken().text == classDeclNode->Name)
			{
				ast_func* destructorDecl = new ast_func();
				destructorDecl->Name = std::string("~") + classDeclNode->Name;
				getNextToken();
				getNextToken();
				getNextToken();
				parseBlock(destructorDecl->block);
				classDeclNode->add(destructorDecl);
				getNextToken();
			}
		}
		else if (Tok.type == Tok.Fn)
		{
			ast_func* funcDecl = parse_function();
			classDeclNode->add(funcDecl);
			getNextToken();
		}
		else
		{
			atom_ast_node* symbol = parse_symbol();
			classDeclNode->add(symbol);
		}
	}

	/// Parses one symbol, either a variable or a function declaration
	atom_ast_node* aeon_parser::parse_symbol()
	{
		atom_ast_node* result_node = nullptr;
		bool IsFunctionDecl = false;
		bool IsConst = false;
		std::vector<ast_expr*> params;

		if (Tok.type == Tok.Public || Tok.type == Tok.Protected || Tok.type == Tok.Private)
		{
			getNextToken();
		}

		ast_type* type_node = parse_type();

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
				ast_func* func_node = new ast_func();
				func_node->Name = SymbolName;
				func_node->parameters = params;
				parseBlock(func_node->block);
				result_node = func_node;
				getNextToken();
			}
		}
		else if (Tok.type == Tok.SemiColon)
		{
			// We are done early, this is a function prototype or a variable
			if (IsFunctionDecl)
			{
				ast_func* func_node = new ast_func();
				func_node->Name = SymbolName;
				//parseBlock(func_node->block);
				result_node = func_node;
				getNextToken();
			}
			else
			{
				ast_varexpr* var_node = new ast_varexpr;
				var_node->Name = SymbolName;
				var_node->VarType = type_node;
				result_node = var_node;
				getNextToken();
			}
		}

		return result_node;
	}

	/// Parse a potentiall complex type with nested generics
	ast_type* aeon_parser::parse_type()
	{
		ast_type* type_node = new ast_type();
		type_node->name = Tok.text;
		getNextToken();

		// Let's see if its a templated type
		if (Tok.type == Tok.MarkOpen)
		{
			getNextToken();

			while (Tok.type != Tok.MarkClose)
			{
				ast_type* template_type_node = parse_type();

				type_node->templateTypeArguments.push_back(template_type_node);

				if (Tok.type == Tok.Comma)
					getNextToken();
			}

			getNextToken();
		}

		return type_node;
	}

	/// Parses a list of arguments (expressions) to pass to a function for example
	/// Assumes to be already on the first token of the argument list
	std::vector<ast_expr*> aeon_parser::parse_argument_list()
	{
		std::vector<ast_expr*> temp;

		ast_expr* argExpr = nullptr;
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

	/// Parses a list of arguments (expressions) to pass to a function for example
	std::vector<ast_expr*> aeon_parser::parse_parameter_list()
	{
		std::vector<ast_expr*> temp;

		while (Tok.type != Tok.ParenClose)
		{
			ast_expr* varexpr = parseVariableDecl();
			temp.push_back(varexpr);

			if (Tok.type != Tok.Comma)
				break;
			else
				getNextToken();
		}

		return temp;
	}

	ast_func* aeon_parser::parse_function()
	{
		ast_func* funcDecl = new ast_func();
		funcDecl->Name = getNextToken().text;

		// We're now in the (
		getNextToken();

		getNextToken();

		while (Tok.type != Tok.ParenClose)
		{
			funcDecl->parameters = parse_parameter_list();
		}

		getNextToken(); // just opened bracket

		parseBlock(funcDecl->block);

		return funcDecl;
	}

	/// We're inside a function body scope, parse whatever is allowed in there
	void aeon_parser::parseFunctionScope(ast_func* funcDeclNode)
	{

		while (Tok.type != aeon_token::BracketClose)
			parseBlock(funcDeclNode->block);

	//	Log("Function body parse complete");
	}

	/// Parse a top-level expression (inside a code execution scope, a function)
	void aeon_parser::parseBlock(ast_codeblock* block)
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

				getNextToken();
				ast_ifbranch* ifbranch = new ast_ifbranch();
				block->add(ifbranch);

				getNextToken(); // go into the first part of the expression
				ifbranch->expr = parseExpression();
				if (ifbranch->expr)
					ifbranch->add(ifbranch->expr);

				//Log("Parsing now the if nested block");

				getNextToken();  //getNextToken(); // consume the {
				parseBlock(ifbranch->block);

				//getNextToken(); // advance to next statement
			}
			else if (Tok.type == Tok.For)
			{
				ast_for* astfor = new ast_for();
				block->add(astfor);

				getNextToken();
				getNextToken();

				astfor->expr = parseExpression();
				if (astfor->expr)
					astfor->add(astfor->expr);

				getNextToken();
				parseBlock(astfor->block);
			}
			else if (Tok.type == Tok.While || Tok.type == Tok.Do) // while or do-while
			{
				ast_while* astwhile = new ast_while();
				block->add(astwhile);

				if (Tok.type == Tok.While)
				{
					astwhile->doWhile = false;
					getNextToken();
					getNextToken();
					astwhile->expr = parseExpression();
					if (astwhile->expr)
						astwhile->add(astwhile->expr);

					getNextToken();
					parseBlock(astwhile->block);
				}
				else
				{
					astwhile->doWhile = true;
					getNextToken(); // reach {

					parseBlock(astwhile->block);
					getNextToken(); // reach 'while'
					getNextToken(); // reach {
					getNextToken(); // reach first expr token
					astwhile->expr = parseExpression();
					if (astwhile->expr)
						astwhile->add(astwhile->expr);
					getNextToken();
				}
			}
			else if (Tok.type == Tok.Return) // facing a return statement
			{
				ast_return* astreturn = new ast_return();
				block->add(astreturn);
				getNextToken();
			}
			else if (Tok.type == Tok.Identifier && peekAhead(0).type == Tok.Identifier) // facing a var declaration
			{
				do
				{
					ast_expr* astvar = parseVariableDecl();
					if (astvar)
					{
						block->add(astvar);
					}
				} while (Tok.type == Tok.Comma);
			}
			else
			{
				ast_expr* expr = parseExpression();
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

	ast_expr* aeon_parser::parseVariableDecl()
	{
		ast_varexpr* astvar = new ast_varexpr();
		astvar->explicitDeclaration = true;
		astvar->TypeString = Tok.text;
		astvar->Name = getNextToken().text;
		getNextToken();

		// Got an assignment on init
		if (Tok.text == "=")
		{
			getNextToken();

			ast_binaryop* assignment = new ast_binaryop(astvar, parseExpression(), "=");
			return assignment;
		}

		return astvar;
	}

	/// We're about to read a function call, get it
	ast_funccall* aeon_parser::parseFunctionCall()
	{
		ast_funccall* funccall = new ast_funccall();
		funccall->funcName = Tok.text;
		getNextToken();

		// let's advance to the first token of the expression (arg)
		getNextToken();

		ast_expr* argExpr = nullptr;
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

	ast_expr* aeon_parser::parseExpression()
	{
		// no way we can get a valid expression here
		if (Tok.type == Tok.ParenClose || Tok.type == Tok.SemiColon || Tok.type == Tok.Comma)
			return nullptr;


		// var_a + var_b + 10 * 10 / 5 + var_c > var_d - var_e++ * (var_c - var_a)

		std::vector<ast_expr*> operands;
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

			ast_unaryop* unary_wrapper = nullptr;

			// Unary operators prefix
			if (Tok.type == Tok.Operator && Tok.text == "-")
			{
				unary_wrapper = new ast_unaryop();
				unary_wrapper->OperatorString = "-";
				getNextToken();
			}

			if (Tok.type == Tok.Decrement) // prefix decrement
			{
				unary_wrapper = new ast_unaryop();
				unary_wrapper->OperatorString = "--";
				getNextToken();
			}

			if (Tok.type == Tok.Increment) // prefix increment
			{
				unary_wrapper = new ast_unaryop();
				unary_wrapper->OperatorString = "++";
				getNextToken();
			}

			ast_expr* leafexpr = nullptr;

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
				ast_expr* subexpr = parse_identifier_subexpression();
				leafexpr = subexpr;
			}
			else if (Tok.type == Tok.IntLiteral)
			{
				ast_intexpr* expr = new ast_intexpr;
				expr->value = atoi(Tok.text.c_str());
				leafexpr = expr;
				getNextToken();
			}
			else if (Tok.type == Tok.FloatLiteral)
			{
				ast_floatexpr* expr = new ast_floatexpr;
				expr->value = atof(Tok.text.c_str());
				leafexpr = expr;
				getNextToken();
			}
			else if (Tok.type == Tok.DoubleLiteral)
			{
				ast_floatexpr* expr = new ast_floatexpr;
				expr->value = atof(Tok.text.c_str());
				leafexpr = expr;
				getNextToken();
			}
			else if (Tok.type == Tok.StringLiteral)
			{
				ast_stringexpr* expr = new ast_stringexpr;
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
		ast_expr* lhs = operands[0];
		for (int i = 0; i < operators.size(); ++i)
		{
			ast_expr* rhs = operands[i + 1];

			if (i == operators.size() - 1)
			{
				// last op
				lhs = new ast_binaryop(lhs, rhs, operators[i].text);
			}
			else if (mOperatorTable[operators[i].text] >= mOperatorTable[operators[i + 1].text])
			{
				ast_expr* rhs = operands[i + 1];
				lhs = new ast_binaryop(lhs, rhs, operators[i].text);
			}
			else
			{
				ast_expr* rhs = new ast_binaryop(operands[i + 1], operands[i + 2], operators[i + 1].text);
				lhs = new ast_binaryop(lhs, rhs, operators[i].text);
				i++;
			}
		}

		//Log("parseExpression generated %s, current tok %s", lhs->exprstr().c_str(), Tok.str());
		return lhs;
	}

	/// Parse a identifier subexpression, any combination of func calls
	ast_expr* aeon_parser::parse_identifier_subexpression()
	{
		ast_expr* result_expr = nullptr;

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

			ast_funccall* funccall = new ast_funccall();
			funccall->funcName = identifier.text;
			result_expr = funccall;

			if (IsTemplated)
			{
				for (std::size_t i = 0; i < TemplateTypeString.size(); ++i)
				{
					ast_funccall::TemplateTypeArgument tta;
					tta.TypeString = TemplateTypeString[i];
					funccall->templateTypeArguments.push_back(tta);
				}
			}

			// Go to the first token of the expression
			getNextToken();

			if (Tok.type != Tok.ParenClose)
			{
				funccall->args = parse_argument_list();
				//getNextToken();
			}

			getNextToken();
		}
		else
		{
			ast_varexpr* var = new ast_varexpr;
			var->Name = identifier.text;
			result_expr = var;
		}

		// Now we might have an array subscript
		if (Tok.type == Tok.SqBracketOpen)
		{
			getNextToken();

			ast_expr* subscript_expression = parseExpression();
			if (subscript_expression)
			{
				result_expr->subscriptArgument = subscript_expression;
			}

			// Go beyond the ]
			getNextToken();
		}

		if (Tok.type == Tok.Period)
		{
			// There is more!
			getNextToken();
			ast_expr* structmember = parse_identifier_subexpression();
			result_expr->add(structmember);
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