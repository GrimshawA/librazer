#include <AEON/AEDocument.h>
#include <AEON/Runtime/AEContext.h>
#include <AEON/aeTokenizer.h>
#include <AEON/aeParser.h>

AEDocument::AEDocument()
: m_parsingFlags(0)
{

}

AEDocument::~AEDocument()
{

}

void AEDocument::parse(const std::string& source)
{
	aeon_lexer lexer;
	aeParser parser;
	
	lexer.tokenize(source);
	parser.i = 0;
	parser.m_tokenizer = &lexer;
	parser.getNextToken();
	m_root = parser.parseDataValue();
}

void AEDocument::load(const std::string& filename)
{
	m_root = AEContext::readValue(filename);
}

void AEDocument::save(const std::string& filename)
{
	 
}
 
void AEDocument::setParserFlags(ParsingFlags flags)
{
	m_parsingFlags = flags;
}

AEValue& AEDocument::operator[](const std::string& name)
{
	return m_root;
}

AEValue AEDocument::root()
{
	return m_root;
}

void AEDocument::print()
{
	printf("AEDocument print\n");
	//m_root.debugPrint();
}