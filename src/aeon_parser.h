#ifndef aeon_parser_h__
#define aeon_parser_h__

#include "aeon_tree.h"
#include "aeon_value.h"
#include "aeon_lexer.h"
#include <vector>

class aeon_context;

/**
	\class aeon_parser
	\brief Parses streams of tokens into meaningful structures

	The parser's job is to build an abstract syntax tree(AST)
	for program sources.

	This is done in two passes.
	1) Gather
		Extract types, typedefs, name spaces, metadata and any other useful info
		about the sources. Doesn't parse complex expressions and inner code.
		This pass is entirely context-free. Also known as API gathering.
	2) Full parse
		Do a full AST build from program sources, already using the nodes
		from the gather pass as input. This pass is context sensitive,
		and throws errors immediately if problems occur.
		One of the main reasons for the gather pass is that type usage can already
		be properly detected and errors thrown in case unknown types are used.
		Context information is also used to match against native types.

	Pre-compiled modules that are not being compiled right now, still have their public API
	declared in aeon_module structs, belonging to the same aeon_context. These types are also considered
	in pass 2.
*/
class aeon_parser
{
	public:

		enum ParsingPass
		{
			Gather,
			Build
		};

		int           pass;
		int           i = 0;
		aeon_lexer*   lex = nullptr;
		aeon_token    Tok;              ///< Current token cursor
		aeon_context* ctx;              ///< The context of the application/library
		aeNodeModule* root;
		aeon_value*   mDataValue;

	public:

		aeon_parser();

		void                     startGather(aeon_lexer& lexer);
		void                     startParse(aeon_lexer& lexer);
		void                     parseValue(aeon_lexer& lexer, aeon_value& rootValue);
		aeNodeStatement*         parseStatement();
		aeNodeBranch*            parseBranch();
		aeNodeBase*              parseSymbol();
		aeon_value               parseDataObject();
		aeQualType               parseQualType();
		aeNodeExpr*              parseExpression();
		aeNodeExpr*              parsePrimaryExpression();
		aeNodeAccessOperator*    parseMemberAccess(aeNodeExpr* left);
		void                     parseTopLevel();
		aeNodeExpr*              parseIdentityExpression();
		aeNodeNamespace*         parseNamespace();
		aeNodeEnum*              parseEnum();
		aeNodeClass*             parseClass();
		aeNodeFor*               parseForLoop();
		aeNodeWhile*             parseWhileLoop();
		void                     parseClassBody(aeNodeClass* classDeclNode);
		aeNodeFunction*          parseLambdaFunction();
		void                     parseClassMember(aeNodeClass* classDeclNode);
		aeNodeReturn*            parseReturn();
		aeNodeBlock*             parseBlock();
		aeNodeFunction*          parseFunction();
		aeNodeVarDecl*           parseVariableDecl();
		std::vector<aeNodeExpr*> parseArgsList();
		std::vector<aeNodeExpr*> parseParamsList();

		void                     serialize(const std::string& filename);
		void                     print();

		/// Parse a identifier subexpression, any combination of func calls
		aeNodeExpr* parse_identifier_subexpression();

		/// We're about to read a function call, get it
		aeNodeFunctionCall* parseFunctionCall();

		bool checkForFunction();

		void printAST();

private:

	/// Peeks ahead from 1 to N tokens
	aeon_token peekAhead(int count);

	aeon_token getNextToken();
};
#endif // aeon_parser_h__
