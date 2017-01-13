#include <RazerParser/Parser/RzTokens.h>

#include <string>
#include <locale>

void myReplace(std::string& str,
	const std::string& oldStr,
	const std::string& newStr)
{
	std::string::size_type pos = 0u;
	while ((pos = str.find(oldStr, pos)) != std::string::npos){
		str.replace(pos, oldStr.length(), newStr);
		pos += newStr.length();
	}
}

void EscapeString(std::string& in)
{
	myReplace(in, "\\n", "\n");
}

aeon_lexer::aeon_lexer()
: i(-1)
{

}

void aeon_lexer::tokenize(std::string src)
{

		//Log("Tokenizing src %s", src.c_str());

		if (src.empty())
		{
			//Log("Lexer: Empty string");
			return;
		}

		program_source = src;
		i = -1;

		aeon_token token = getToken();
		while (token.type != AETK_EOF)
		{
			tokens.push_back(token);
			token = getToken();
		}
		tokens.push_back(token); // push the eof
}

	aeon_token aeon_lexer::getToken()
	{
		// We are at program_source[i], let's get the next token

		aeon_token token;
		char LastChar = ' ';

		// we have room to work, skip whitespace
		while (isspace(LastChar))
			LastChar = program_source[++i];

		// If i is already at the end, we give back the end of file token, which finishes the stream
		if (i >= (program_source.size() - 1))
		{
			token.type = AETK_EOF;
			token.text = "EOF";
			return token;
		}

		// Check for // comments
		if (LastChar == '/')
		{
			if (program_source[i + 1] == '/')
			{
				token.text = "/";

				// we have a comment
				while ((LastChar = program_source[++i]) != '\n')
				{
					token.text += LastChar;
				}

				token.type = AETK_LINECOMMENT;
				return token;
			}
			else if (program_source[i + 1] == '*')
			{
				i++;
				LastChar = program_source[++i];

				//Log("First char %c", LastChar);

				bool foundPair = false;
				while (!foundPair)
				{
					if (LastChar == '*' && program_source[i + 1] == '/')
					{
						++i;
						token.type = AETK_MULTICOMMENT;
						return token;
					}

					LastChar = program_source[++i];
				}
			}
		}

		if (LastChar == '(')
		{
			token.text = "(";
			token.type = AETK_OPENPAREN;
			return token;
		}

		if (LastChar == ')')
		{
			token.text = ")";
			token.type = AETK_CLOSEPAREN;
			return token;
		}

		if (LastChar == ':')
		{
			token.text = ":";
			token.type = AETK_COLON;
			return token;
		}

		if (LastChar == '.')
		{
			token.text = ".";
			token.type = AETK_DOT;
			return token;
		}

		if (LastChar == '[')
		{
			token.text = "[";
			token.type = AETK_OPENSQBRACKET;
			return token;
		}

		if (LastChar == ']')
		{
			token.text = "]";
			token.type = AETK_CLOSESQBRACKET;
			return token;
		}

		if (LastChar == ',')
		{
			token.text = ",";
			token.type = AETK_COMMA;
			return token;
		}

		if (LastChar == '~')
		{
			token.text = "~";
			token.type = AETK_TILDE;
			return token;
		}

		if (LastChar == '{')
		{
			token.text = "{";
			token.type = AETK_OPENBRACKET;
			return token;
		}

		if (LastChar == '}')
		{
			token.text = "}";
			token.type = AETK_CLOSEBRACKET;
			return token;
		}

		if (LastChar == ';')
		{
			token.type = AETK_SEMICOLON;
			token.text = ";";
			return token;
		}

		if (LastChar == '@')
		{
			token.type = AETK_HANDLE;
			token.text = "@";
			return token;
		}

		if (LastChar == '#')
		{
			token.type = AETK_DIRECTIVE;
			token.text = "#";
			return token;
		}

		if (LastChar == '-')
		{
			if (program_source[i + 1] == '>')
			{
				i++;
				token.type = AETK_ARROW;
				token.text = "->";
				return token;
			}
		}

		if (LastChar == '+')
		{
			if (program_source[i + 1] == '+')
			{
				i++;
				token.type = AETK_INCREMENT;
				token.text = "++";
				return token;
			}
		}

		if (LastChar == '-')
		{
			if (program_source[i + 1] == '-')
			{
				i++;
				token.type = AETK_DECREMENT;
				token.text = "--";
				return token;
			}
		}

		if (LastChar == '=' && program_source[i + 1] == '>')
		{
			token.type = AETK_LAMBDA;
			token.text = "=>";
			return token;
		}

		// operators
		if (LastChar == '>' || LastChar == '<' || LastChar == '+' || LastChar == '-' || LastChar == '*' || LastChar == '/' || LastChar == '^' ||
			LastChar == '=' || LastChar == '%')
		{
			token.type = AETK_BINOP;
			token.text += LastChar;
			return token;
		}

		// We're facing a string literal
		if (LastChar == '"')
		{
			token.type = AETK_STRINGLITERAL;
			token.text += LastChar;
			while ((LastChar = program_source[++i]) != '"')
			{
				token.text += LastChar;
			}
			token.text += LastChar;
			EscapeString(token.text);
			return token;
		}

		// We're looking at a number literal
		if (isdigit(LastChar))
		{
			token.text += LastChar;
			while (isdigit((LastChar = program_source[++i])))
			{
				token.text += LastChar;
			}
			// digits are over, it can still be a floating point literal
			if (LastChar == '.')
			{
				token.text += ".";

				// read the remaining digits for the floating point number
				while (isdigit((LastChar = program_source[++i])))
				{
					token.text += LastChar;
				}
				if (LastChar == 'f') // Float
				{
					token.type = AETK_FLOATLITERAL;
				}
				else // double
				{
					token.type = AETK_FLOATLITERAL;
				}
			}
			else
			{
				token.type = AETK_INTLITERAL;
			}

			--i;

			return token;
		}

		// let's find identifier
		if (isalpha(LastChar))
		{
			token.text += LastChar;

			while (isalpha((LastChar = program_source[++i])) || isdigit(LastChar) || LastChar == '_')
			{
				token.text += LastChar;
			}

			// backtrack i by 1, so the next character read is the relevant item
			i--;
			if (token.text == "class")
				token.type = AETK_CLASS;
			else if (token.text == "struct")
				token.type = AETK_STRUCT;
			else if (token.text == "null" || token.text == "nullptr")
				token.type = AETK_NULL;
			else if (token.text == "fn")
				token.type = AETK_FUNCTION;
			else if (token.text == "function")
				token.type = AETK_FUNCTION;
			else if (token.text == "enum")
				token.type = AETK_ENUM;
			else if (token.text == "union")
				token.type = AETK_UNION;
			else if (token.text == "cast")
				token.type = AETK_CAST;
			else if (token.text == "if")
				token.type = AETK_IF;
			else if (token.text == "else")
				token.type = AETK_ELSE;
			else if (token.text == "while")
				token.type = AETK_WHILE;
			else if (token.text == "do")
				token.type = AETK_DO;
			else if (token.text == "const")
				token.type = AETK_CONST;
			else if (token.text == "new")
				token.type = AETK_NEW;
			else if (token.text == "delete")
				token.type = AETK_DELETE;
			else if (token.text == "sizeof")
				token.type = AETK_SIZEOF;
			else if (token.text == "typeof")
				token.type = AETK_TYPEOF;
			else if (token.text == "operator")
				token.type = AETK_OPERATOR;
			else if (token.text == "for")
				token.type = AETK_FOR;
			else if (token.text == "foreach")
				token.type = AETK_FOREACH;
			else if (token.text == "asm")
				token.type = AETK_ASM;
			else if (token.text == "auto")
				token.type = AETK_AUTO;
			else if (token.text == "true")
				token.type = AETK_TRUE;
			else if (token.text == "false")
				token.type = AETK_FALSE;
			else if (token.text == "return")
				token.type = AETK_RETURN;
			else if (token.text == "public")
				token.type = AETK_PUBLIC;
			else if (token.text == "protected")
				token.type = AETK_PROTECTED;
			else if (token.text == "private")
				token.type = AETK_PRIVATE;
			else if (token.text == "namespace")
				token.type = AETK_NAMESPACE;
			else if (token.text == "using")
				token.type = AETK_USING;
			else if (token.text == "break")
				token.type = AETK_BREAK;
			else if (token.text == "goto")
				token.type = AETK_GOTO;
			else if (token.text == "continue")
				token.type = AETK_CONTINUE;
			else if (token.text == "as")
				token.type = AETK_AS;
			else if (token.text == "in")
				token.type = AETK_IN;
			else if (token.text == "switch")
				token.type = AETK_SWITCH;
			else if (token.text == "case")
				token.type = AETK_CASE;
			else if (token.text == "default")
				token.type = AETK_DEFAULT;
			else if (token.text == "static")
				token.type = AETK_STATIC;
			else if (token.text == "typedef")
				token.type = AETK_TYPEDEF;
			else if (token.text == "and")
				token.type = AETK_AND;
			else if (token.text == "is")
				token.type = AETK_IS;
			else if (token.text == "isnt")
				token.type = AETK_ISNOT;
			else if (token.text == "not")
				token.type = AETK_NOT;
			else if (token.text == "import")
				token.type = AETK_IMPORT;
			else
				token.type = AETK_IDENTIFIER;
			return token;
		}

		return token;
	}

void aeon_lexer::print()
{
		for (auto& t : tokens)
		{
			//Log("Token: %s", t.text.c_str());
		}
}
