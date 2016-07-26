#ifndef AEDOCUMENT_H__
#define AEDOCUMENT_H__

#include <AEON/Runtime/AEValue.h>

#include <string>

/**
	\class AEDocument
	\brief Memory representation of an AEON data file
*/
class AEDocument
{
public:

	enum ParsingFlags
	{
		ALLOW_EXPRESSIONS = 1 << 0, ///< Parser allows expressions and inline functions
		ALLOW_FUNCTIONS   = 1 << 1,   ///< Parser allows full fledged functions to be parsed 
	};

public:

	AEDocument();
	~AEDocument();

	void parse(const std::string& source);
	void load(const std::string& filename);
	void save(const std::string& filename);

	void setParserFlags(ParsingFlags flags);

	AEValue& operator[](const std::string& name);

	AEValue root();

	// Todo: review
	void print();


private:
	AEValue m_root;
	uint32_t m_parsingFlags;
};
#endif