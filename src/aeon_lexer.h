#ifndef aeon_lexer_h__
#define aeon_lexer_h__

#include <string>
#include <vector>

class aeon_token
{
	public:

		enum TokenType
		{
			CommentSingle,
			CommentMulti,
			Identifier,
			TypeID,
			Class,
			Struct,
			Operator,
			Static,
			This,
			Comma,
			Typedef,
			EndOfFile,
			ParenOpen,
			ParenClose,
			BracketOpen,
			BracketClose,
			SemiColon,
			InitAssignment,
			StringLiteral,
			IntLiteral,
			FloatLiteral,
			HexLiteral,
			DoubleLiteral,
			OperatorKeyword,
			Const,
			New,
			Delete,
			SizeOf,
			TypeOf,
			And,
			Is,
			Isnt,
			Not,
			Fn,
			If,
			Else,
			While,
			Switch,
			Case,
			Default,
			For,
			ForEach,
			Do,
			Null,
			True,
			False,
			Return,
			Public,
			Protected,
			Private,
			Break,
			Continue,
			In,
			Goto,
			As,
			Period,
			MarkOpen,
			MarkClose,
			SqBracketOpen,
			SqBracketClose,
			Namespace,
			Using,
			Tilde,
			Enum,
			Union,
			Cast,
			Directive,
			Auto,
			Asm,
			Increment,
			Decrement,
			Unknown
		};

		int type;
		std::string text;

		const char* str()
		{
			return text.c_str();
		}

		std::string extract_stringliteral()
		{
			std::string s = text;
			if (type == StringLiteral)
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
