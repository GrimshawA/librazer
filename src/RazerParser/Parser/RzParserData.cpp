#include <RazerParser/Parser/RzParser.h>

#include <iostream>
#include <string>

void print2(std::string s)
{
	std::cout << s << std::endl;
}

RzValue ToValue(aeNodeValue* value)
{
	RzValue r;
	return r;
}

void RzParser::parseValue(aeon_lexer& lexer, RzValue& rootValue)
{
	if (lexer.tokens.size() == 0)
	{
		//Log("Parser: No tokens in the stream");
		return;
	}

	m_tokenizer = &lexer;
	i = 0;
	mDataValue = &rootValue;

	Tok = getNextToken();

	while (Tok.type != AETK_EOF)
	{
		rootValue = parseDataValue();
	}
}

RzValue RzParser::parseDataValue()
{
	RzValue objectValue;

	// we are on the first token of a data definition

	std::string ObjectType;
	if (Tok.type == AETK_IDENTIFIER)
	{
		ObjectType = Tok.text;
		getNextToken();
		//objectValue.mRawProperty = ObjectType;
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

	// Property assigned
	if (Tok.type == AETK_COLON)
	{
		getNextToken();

		RzValue val = parseProperty();
		objectValue.setValue(ObjectType, val);
	}
	else if (Tok.type == AETK_OPENBRACKET)
	{
		// Parsing: Object Body
		printf("BODY BEGIN\n");

		getNextToken();

		std::string PropertyName;

		while (Tok.text != "}")
		{
			std::string identifier = Tok.text;
			getNextToken(); //into :

			if (Tok.type == AETK_COLON)
			{
				getNextToken();

				RzValue ChildValue = parseProperty();
				printf("BODY %s\n", ChildValue.c_str());
				objectValue.setValue(identifier, ChildValue);
			}
			else
			{
				RzValue obj = parseDataObjectBody();
				if (obj.property("id"))
				{
					// Identified subobject
					objectValue.setValue(obj.property("id").str(), obj);
					obj.setValue("typename", RzValue(identifier));
				}
			}
		}
				
		getNextToken();

		printf("BODY END\n");

	}

	if (Tok.type == AETK_COMMA || Tok.type == AETK_SEMICOLON)
	{
		getNextToken();
	}

	// We end on the token that belongs to the next object/value
	return objectValue;
}

RzValue RzParser::parseDataObjectBody()
{
	RzValue obj;
	getNextToken();

	while (Tok.type != AETK_CLOSEBRACKET)
	{
               // TODO ERROR: EOF Reached before matching brace
               if(Tok.type == AETK_EOF)
                   return obj;

		std::string identifier = Tok.text;
		getNextToken(); 
		if (Tok.type == AETK_COLON)
		{
			getNextToken();
			RzValue v = parseProperty();
			obj.setValue(identifier, v);

			print2("parsed property for " + identifier);
		}
		else if (Tok.type == AETK_OPENBRACKET)
		{
			RzValue nestedObj = parseDataObjectBody();
		}
	}

	getNextToken();

	return obj;
}

RzValue RzParser::parseProperty()
{
	if (Tok.type == AETK_IDENTIFIER && peekAhead(0).type == AETK_OPENBRACKET)
	{
		// Object value, like myproperty: Obj {}
		std::string identifier = Tok.text;
		getNextToken();
		RzValue obj = parseDataObjectBody();
		obj.setValue("typename", RzValue(identifier));
		return obj;
	}
	if (Tok.type == AETK_STRINGLITERAL || Tok.type == AETK_IDENTIFIER)
	{
		std::string text = Tok.extract_stringliteral();
		getNextToken();
		return RzValue(text);
	}
	if (Tok.type == AETK_INTLITERAL)
	{
		std::string text = Tok.text;
		getNextToken();
		return RzValue(atoi(text.c_str()));
	}
	if (Tok.type == AETK_OPENSQBRACKET)
	{
		return parseArrayValue();
	}

	return RzValue();
}

aeNodeValue* RzParser::parsePropertyValue()
{
	aeNodeValue* returnValue = nullptr;

	if (Tok.type == AETK_OPENSQBRACKET)
	{
		getNextToken();

		aeNodeArray* arrayValue = new aeNodeArray;

		if (Tok.type != AETK_CLOSESQBRACKET)
		{
			do
			{
				aeNodeValue* element = parsePropertyValue();
				arrayValue->addElement(element);

				if (Tok.type == AETK_COMMA)
					getNextToken();
				else break;

			} while (1);
		}

		getNextToken();

		returnValue = arrayValue;
	}
	else if (Tok.type == AETK_OPENBRACKET)
	{
		aeNodeFunction* fn = new aeNodeFunction;
		fn->m_block.reset(parseBlock());
		returnValue = fn;

		getNextToken();
	}
	else {
		// Expression, we will infer what it is
		aeNodeExpr* expr = parseExpression();
		returnValue = expr;
	}

	if (returnValue)
	{
		printf("pvalue: %s\n", returnValue->str().c_str());
	}

	return returnValue;
}

RzValue RzParser::parseArrayValue()
{
	PARSE_EXPECTS(AETK_OPENSQBRACKET, "missing [");
	
	RzValue arrayValue = RzValue::makeArray();

	getNextToken();

	int i = 0;
	while (Tok.type != AETK_CLOSESQBRACKET && Tok.type != AETK_EOF)
	{
		RzValue elem = parseProperty();
		arrayValue.setValue(i++, elem);

		if (Tok.type != AETK_COMMA)
			break;
                else
                    getNextToken();
	}

        getNextToken();

	std::cout << "Parsed array" << arrayValue.str() << std::endl;

	return arrayValue;
}
