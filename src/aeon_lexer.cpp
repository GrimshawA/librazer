#include "aeon_lexer.h"
#include <string>
#include <locale>

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
		while (token.type != aeon_token::EndOfFile)
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
			token.type = aeon_token::EndOfFile;
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

				token.type = aeon_token::CommentSingle;
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
						token.type = aeon_token::CommentMulti;
						return token;
					}

					LastChar = program_source[++i];
				}
			}
		}

		if (LastChar == '(')
		{
			token.text = "(";
			token.type = aeon_token::ParenOpen;
			return token;
		}

		if (LastChar == ')')
		{
			token.text = ")";
			token.type = aeon_token::ParenClose;
			return token;
		}

		if (LastChar == ':')
		{
			token.text = ":";
			token.type = aeon_token::InitAssignment;
			return token;
		}

		if (LastChar == '.')
		{
			token.text = ".";
			token.type = aeon_token::Period;
			return token;
		}

		if (LastChar == '[')
		{
			token.text = "[";
			token.type = aeon_token::SqBracketOpen;
			return token;
		}

		if (LastChar == ']')
		{
			token.text = "]";
			token.type = aeon_token::SqBracketClose;
			return token;
		}

		if (LastChar == ',')
		{
			token.text = ",";
			token.type = aeon_token::Comma;
			return token;
		}

		if (LastChar == '~')
		{
			token.text = "~";
			token.type = aeon_token::Tilde;
			return token;
		}

		if (LastChar == '{')
		{
			token.text = "{";
			token.type = aeon_token::BracketOpen;
			return token;
		}

		if (LastChar == '}')
		{
			token.text = "}";
			token.type = aeon_token::BracketClose;
			return token;
		}

		if (LastChar == ';')
		{
			token.type = aeon_token::SemiColon;
			token.text = ";";
			return token;
		}

		if (LastChar == '#')
		{
			token.type = aeon_token::Directive;
			token.text = "#";
			return token;
		}

		if (LastChar == '+')
		{
			if (program_source[i + 1] == '+')
			{
				i++;
				token.type = aeon_token::Increment;
				token.text = "++";
				return token;
			}
		}

		if (LastChar == '-')
		{
			if (program_source[i + 1] == '-')
			{
				i++;
				token.type = aeon_token::Decrement;
				token.text = "--";
				return token;
			}
		}

		// operators
		if (LastChar == '>' || LastChar == '<' || LastChar == '+' || LastChar == '-' || LastChar == '*' || LastChar == '/' || LastChar == '^' ||
			LastChar == '=' || LastChar == '%')
		{
			token.type = aeon_token::Operator;
			token.text += LastChar;
			return token;
		}

		// We're facing a string literal
		if (LastChar == '"')
		{
			token.type = aeon_token::StringLiteral;
			token.text += LastChar;
			while ((LastChar = program_source[++i]) != '"')
			{
				token.text += LastChar;
			}
			token.text += LastChar;
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
					token.type = aeon_token::FloatLiteral;
				}
				else // double
				{
					token.type = aeon_token::DoubleLiteral;
				}
			}
			else
			{
				token.type = aeon_token::IntLiteral;
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
				token.type = aeon_token::Class;
			else if (token.text == "struct")
				token.type = aeon_token::Struct;
			else if (token.text == "null" || token.text == "nullptr")
				token.type = aeon_token::Null;
			else if (token.text == "fn")
				token.type = aeon_token::Fn;
			else if (token.text == "function")
				token.type = aeon_token::Fn;
			else if (token.text == "enum")
				token.type = aeon_token::Enum;
			else if (token.text == "union")
				token.type = aeon_token::Union;
			else if (token.text == "cast")
				token.type = aeon_token::Cast;
			else if (token.text == "if")
				token.type = aeon_token::If;
			else if (token.text == "else")
				token.type = aeon_token::Else;
			else if (token.text == "while")
				token.type = aeon_token::While;
			else if (token.text == "do")
				token.type = aeon_token::Do;
			else if (token.text == "const")
				token.type = aeon_token::Const;
			else if (token.text == "new")
				token.type = aeon_token::New;
			else if (token.text == "delete")
				token.type = aeon_token::Delete;
			else if (token.text == "sizeof")
				token.type = aeon_token::SizeOf;
			else if (token.text == "typeof")
				token.type = aeon_token::TypeOf;
			else if (token.text == "operator")
				token.type = aeon_token::OperatorKeyword;
			else if (token.text == "for")
				token.type = aeon_token::For;
			else if (token.text == "foreach")
				token.type = aeon_token::ForEach;
			else if (token.text == "asm")
				token.type = aeon_token::Asm;
			else if (token.text == "auto")
				token.type = aeon_token::Auto;
			else if (token.text == "true")
				token.type = aeon_token::True;
			else if (token.text == "false")
				token.type = aeon_token::False;
			else if (token.text == "return")
				token.type = aeon_token::Return;
			else if (token.text == "public")
				token.type = aeon_token::Public;
			else if (token.text == "protected")
				token.type = aeon_token::Protected;
			else if (token.text == "private")
				token.type = aeon_token::Private;
			else if (token.text == "namespace")
				token.type = aeon_token::Namespace;
			else if (token.text == "using")
				token.type = aeon_token::Using;
			else if (token.text == "break")
				token.type = aeon_token::Break;
			else if (token.text == "goto")
				token.type = aeon_token::Goto;
			else if (token.text == "continue")
				token.type = aeon_token::Continue;
			else if (token.text == "as")
				token.type = aeon_token::As;
			else if (token.text == "in")
				token.type = aeon_token::In;
			else if (token.text == "switch")
				token.type = aeon_token::Switch;
			else if (token.text == "case")
				token.type = aeon_token::Case;
			else if (token.text == "default")
				token.type = aeon_token::Default;
			else if (token.text == "static")
				token.type = aeon_token::Static;
			else if (token.text == "typedef")
				token.type = aeon_token::Typedef;
			else if (token.text == "and")
				token.type = aeon_token::And;
			else if (token.text == "is")
				token.type = aeon_token::Is;
			else if (token.text == "isnt")
				token.type = aeon_token::Isnt;
			else if (token.text == "not")
				token.type = aeon_token::Not;
			else
				token.type = aeon_token::Identifier;
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