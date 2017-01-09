#ifndef aeon_parser_h__
#define aeon_parser_h__

#include <AEON/AST/Nodes.h>
#include <Rzr/RzValue.h>
#include <AEON/Parser/RzTokens.h>

#include <vector>

#define PARSE_EXPECTS(toks, msg) (void) 0;

class RzEngine;

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

	Notes:
	Some of the functions have the node suffix while others have the value suffix. Nodes are always part of the AST,
	while values are runtime variants, usable for other purposes.
*/
class RzParser
{
	public:

		enum ParsingPass
		{
			Gather,
			Build
		};

		int           pass;
		int           i = 0;
		aeon_lexer*   m_tokenizer;
		std::unique_ptr<aeon_lexer> m_customTokenizer; ///< When we manage our own
		aeon_token    Tok;              ///< Current token cursor
		RzEngine* ctx;              ///< The context of the application/library
		aeNodeModule* root;
		RzValue*   mDataValue;

		/// Tokenizes the source and is fully ready to use
		static std::unique_ptr<RzParser> create(const std::string& source, RzEngine* context);

	public:

		RzParser();

		/// Create with a custom tokenizer
		RzParser(const std::string& source);

		void                     startGather(aeon_lexer& lexer);
		void                     startParse(aeon_lexer& lexer);

		bool                     matchesVarDecl();

		void                     parseValue(aeon_lexer& lexer, RzValue& rootValue);
		aeNodeValue*             parsePropertyValue(); ///< Parses any of the language's values in form property: <value>
		AEStmtNode*         parseStatement();
		aeNodeBranch*            parseBranch();
		AEBaseNode*              parseSymbol();
		RzValue               parseDataValue();
		RzValue                  parseDataObjectBody();
		RzValue                  parseArrayValue();
		RzValue                  parseProperty();
		aeQualType               parseQualType();
		aeNodeExpr*              parseExpression();
		aeNodeExpr*              parsePrimaryExpression();
		aeNodeAccessOperator*    parseMemberAccess(aeNodeExpr* left);
		void                     parseTopLevel();
		aeNodeExpr*              parseIdentityExpression();
		aeNodeNamespace*         parseNamespace();
		AEEnumNode*              parseEnum();
		AEStructNode*            parseClass();
		aeNodeFor*               parseForLoop();
		aeNodeWhile*             parseWhileLoop();
		void                     parseClassBody(AEStructNode* classDeclNode);
		AEFieldNode*             parseStructField();
		aeNodeFunction*          parseLambdaFunction();
		void                     parseClassMember(AEStructNode* classDeclNode);
		aeNodeReturn*            parseReturn();
		aeNodeBlock*             parseBlock();
		aeNodeFunction*          parseFunction();
		aeNodeVarDecl*           parseVariableDecl();
		std::vector<aeNodeExpr*> parseArgsList();
		std::vector<aeNodeVarDecl*> parseParamsList();
		aeNodeNew*               parseNew();

		void                     serialize(const std::string& filename);
		void                     print();

		/// Parse a identifier subexpression, any combination of func calls
		aeNodeExpr* parse_identifier_subexpression();

		/// We're about to read a function call, get it
		aeNodeFunctionCall* parseFunctionCall();

		bool checkForFunction();

		void printAST();

//private:

	/// Peeks ahead from 1 to N tokens
	aeon_token peekAhead(int count);

	aeon_token getNextToken();
};
#endif // aeon_parser_h__