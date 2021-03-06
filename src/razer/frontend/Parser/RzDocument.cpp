#include <razer/runtime/RzDocument.h>
#include <razer/runtime/RzEngine.h>
#include <razer/frontend/Parser/TokenParser.h>
#include <razer/frontend/Parser/RzParser.h>
#include <razer/runtime/RzValue.h>

RzDocument::RzDocument()
: m_parsingFlags(0)
{

}

RzDocument::~RzDocument()
{

}

void RzDocument::parse(const std::string& source)
{
	RzTokenParser lexer;
	RzParser parser;
	
	lexer.tokenize(source);
	parser.i = 0;
	parser.m_tokenizer = &lexer;
	parser.getNextToken();
	m_root = std::unique_ptr <RzValue>(new RzValue(parser.parseDataValue()));
}

bool RzDocument::load(const std::string& filename)
{
	m_root = std::unique_ptr <RzValue>(new RzValue(RzEngine::readValue(filename)));
	if (m_root->isUndefined())
		return false;
	else
		return true;
}

void RzDocument::save(const std::string& filename)
{
	 
}
 
void RzDocument::setParserFlags(ParsingFlags flags)
{
	m_parsingFlags = flags;
}

RzValue& RzDocument::operator[](const std::string& name)
{
	return *m_root.get();
}

RzValue RzDocument::root()
{
	return *m_root.get();
}

void RzDocument::print()
{
	printf("AEDocument print\n");
	//m_root.debugPrint();
}