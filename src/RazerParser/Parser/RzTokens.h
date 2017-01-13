#ifndef aeon_lexer_h__
#define aeon_lexer_h__

#include <string>
#include <vector>

enum TokenType
{
	AETK_LINECOMMENT,
	AETK_MULTICOMMENT,
	AETK_IDENTIFIER,
	AETK_CLASS,
	AETK_STRUCT,
	AETK_BINOP,
	AETK_STATIC,
	AETK_THIS,
	AETK_COMMA,
	AETK_COLON,
	AETK_TYPEDEF,
	AETK_EOF,
	AETK_OPENPAREN,
	AETK_CLOSEPAREN,
	AETK_OPENBRACKET,
	AETK_CLOSEBRACKET,
	AETK_SEMICOLON,
	AETK_STRINGLITERAL,
	AETK_INTLITERAL,
	AETK_FLOATLITERAL,
	AETK_OPERATOR,
	AETK_CONST,
	AETK_NEW,
	AETK_DELETE,
	AETK_SIZEOF,
	AETK_TYPEOF,
	AETK_AND,
	AETK_IS,
	AETK_ISNOT,
	AETK_NOT,
	AETK_FUNCTION,
	AETK_IF,
	AETK_ELSE,
	AETK_WHILE,
	AETK_SWITCH,
	AETK_CASE,
	AETK_DEFAULT,
	AETK_FOR,
	AETK_FOREACH,
	AETK_DO,
	AETK_NULL,
	AETK_TRUE,
	AETK_FALSE,
	AETK_RETURN,
	AETK_PUBLIC,
	AETK_PROTECTED,
	AETK_PRIVATE,
	AETK_BREAK,
	AETK_CONTINUE,
	AETK_IN,
	AETK_GOTO,
	AETK_AS,
	AETK_DOT,
	AETK_OPENDIAMOND,
	AETK_CLOSEDIAMOND,
	AETK_OPENSQBRACKET,
	AETK_CLOSESQBRACKET,
	AETK_NAMESPACE,
	AETK_USING,
	AETK_TILDE,
	AETK_ENUM,
	AETK_UNION,
	AETK_CAST,
	AETK_HANDLE,
	AETK_DIRECTIVE,
	AETK_AUTO,
	AETK_ASM,
	AETK_INCREMENT,
	AETK_DECREMENT,
	AETK_ARROW,
	AETK_LAMBDA, //< =>
	AETK_IMPORT, //< import
	AETK_UNKNOWN
};

class aeon_token
{
	public:		

		int type;
		std::string text;

		const char* str()
		{ 
			return text.c_str();
		}

		std::string extract_stringliteral()
		{
			std::string s = text;
			if (type == AETK_STRINGLITERAL)
			{
				s.erase(s.begin());
				s.erase(s.begin() + s.size() - 1);
			}
			return s;
		}
};

/**
	\class aeon_lexer
	\brief Generates a stream of token from a source program so it can be parsed meaningfully
*/
class aeon_lexer
{
	public:

		std::string             program_source;
		int                     i;
		std::vector<aeon_token> tokens;

public:

	aeon_lexer();

	void print();

	void tokenize(std::string src);

	aeon_token getToken();
};

#endif // aeon_lexer_H__
