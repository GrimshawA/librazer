#ifndef aeon_parser_h__
#define aeon_parser_h__

#include "aeon_tree.h"
#include "aeon_value.h"
#include "aeon_lexer.h"
#include "aeon_expression.h"
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
		aeNodeModule*   root;
		aeon_value*   mDataValue;

	public:

		aeon_parser();

		/// Parse the AST for gather pass only
		void parse_gather_pass(aeon_lexer& lexer);

		/// Parse the full AST with context sensitivity
		void parse(aeon_lexer& lexer);

		void parseValue(aeon_lexer& lexer, aeon_value& rootValue);

		/// Parses the {} data object
		aeon_value parseDataObject();

		/// Parses one symbol in either declarative scope like a namespace, class or global
		/// Symbols can be: functions, function prototypes, variables, properties
		aeNodeBase* parse_symbol();

		/// Parse a potentiall complex type with nested generics
		aeNodeTypeDecl* parse_type();

		aeNodeExpr* parseExpression();

		void parseTopLevel();

		aeNodeNamespace* parse_namespace();

		aeNodeEnum* parse_enum();

		/// Returns a class node. Requires the current token to be "class"
		aeNodeClass* parseClass();

		/// With the cursor on a 'for' token, prepares the for loop node
		aeNodeFor* parseForLoop();

		/// With the cursor on a 'while' or 'do' token, returns the ready while node
		aeNodeWhile* parseWhileLoop();

		/// We're inside a class block, last token read was {
		void parseClassBody(aeNodeClass* classDeclNode);

		/// Parses one thing inside the class body
		void parse_class_element(aeNodeClass* classDeclNode);

		/// Parse a identifier subexpression, any combination of func calls
		aeNodeExpr* parse_identifier_subexpression();

		aeNodeFunction* parse_function();

		/// Parses a list of arguments (expressions) to pass to a function for example
		std::vector<aeNodeExpr*> parse_argument_list();

		/// Parses a list of arguments (expressions) to pass to a function for example
		std::vector<aeNodeExpr*> parse_parameter_list();

		/// We're inside a function body scope, parse whatever is allowed in there
		void parseFunctionScope(aeNodeFunction* funcDeclNode);

		/// We're about to read a function call, get it
		aeNodeFunctionCall* parseFunctionCall();

		aeNodeVarDecl* parseVariableDecl();

		/// Parses one block of executable code (inside a function)
		void parseBlock(aeNodeBlock* block);

		bool checkForFunction();

		void printAST();

	private:

		/// Peeks ahead from 1 to N tokens
		aeon_token peekAhead(int count);

		aeon_token getNextToken();
};
#endif // aeon_parser_h__
