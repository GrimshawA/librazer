/*#include <AEON/aeon.h>
#include <AEON/aeon_compiler.h>
#include <Nephilim/Foundation/File.h>

namespace AEON
{

	datavalue ReadAtomDataFile(const String& filename)
	{
		datavalue v;

		aeon_lex lexer;
		lexer.tokenize(getTextFileContents(filename));
		//lexer.print();

		aeon_parser parser;
		parser.parseValue(lexer, v);

		//Log("the root value came with %d child", v.properties.size());
		//v.debugPrint();
		return v;
	}

	datavalue CreateValueFromBuffer(const String& buffer)
	{
		datavalue v;

		aeon_lex lexer;
		lexer.tokenize(buffer);
		aeon_parser parser;
		parser.parseValue(lexer, v);
		return v;
	}

	aeon_module* atomQuickTest(const char* filename, aeon_vm* vm, aeon_env* env)
	{
		aeon_lex lexer;
		lexer.tokenize(getTextFileContents(filename));
		lexer.print();
		Log("-- lexer is done. %d tokens", lexer.tokens.size());

		aeon_parser parser;
		parser.parse(lexer);
		parser.printAST();
		Log("-- parser is done, it generated source tree");

		atom_compiler compiler;
		compiler.vm = vm;
		compiler.env = env;
		compiler.generate(parser.root);

		Log("-- the script was compiled");

		return compiler.mod;
	}
};
*/