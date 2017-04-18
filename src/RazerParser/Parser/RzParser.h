#ifndef aeon_parser_h__
#define aeon_parser_h__

#include <RazerParser/AST/Nodes.h>
#include <RazerRuntime/RzValue.h>
#include <RazerParser/Parser/TokenParser.h>
#include <RazerCompiler/SymbolTypename.h>

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
	static std::shared_ptr<RzSourceUnit> getParseTree(const std::string& source, RzEngine& ctx);

public:
		enum ParsingPass
		{
			Gather,
			Build
		};

		int           pass;
		int           i = 0;
        RzTokenParser*   m_tokenizer;
        std::unique_ptr<RzTokenParser> m_customTokenizer; ///< When we manage our own
        RzToken    Tok;              ///< Current token cursor
		RzEngine* ctx;              ///< The context of the application/library
		RzSourceUnit* root;
		RzValue*   mDataValue;

		/// Tokenizes the source and is fully ready to use
		static std::unique_ptr<RzParser> create(const std::string& source, RzEngine* context);

	public:

		RzParser();

		/// Create with a custom tokenizer
		RzParser(const std::string& source);

        void                     startGather(RzTokenParser& lexer);
        bool                     startParse(RzTokenParser& lexer);

		bool                     matchesVarDecl();

        void                     parseValue(RzTokenParser& lexer, RzValue& rootValue);
		aeNodeValue*             parsePropertyValue(); ///< Parses any of the language's values in form property: <value>
		AEStmtNode*         parseStatement();
		aeNodeBranch*            parseBranch();
		AEBaseNode*              parseSymbol();
		RzValue               parseDataValue();
		RzValue                  parseDataObjectBody();
		RzValue                  parseArrayValue();
		RzValue                  parseProperty();
		RzQualType               parseQualType();
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
        aeNodeImport*            parseImport();

		void                     serialize(const std::string& filename);
		void                     print();

		/// Parse a identifier subexpression, any combination of func calls
		aeNodeExpr* parse_identifier_subexpression();

		/// We're about to read a function call, get it
		aeNodeFunctionCall* parseFunctionCall();

        SymbolTypename parseTypename();

        // Skip the current token if its a newline and all the subsequent ones
        void skipNewlines();

		bool checkForFunction();

		void printAST();

//private:

	/// Peeks ahead from 1 to N tokens
    RzToken peekAhead(int count);

    RzToken getNextToken();
};
#endif // aeon_parser_h__
