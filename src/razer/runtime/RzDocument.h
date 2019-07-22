#ifndef RZDOCUMENT_H__
#define RZDOCUMENT_H__

#include <razer/runtime/RzValue.h>

#include <string>
#include <memory>

/**
	\class RzDocument
	\brief Document object model (DOM) for a static data file

	The language naturally supports syntax for defining complex objects directly. As long as they don't contain
	any bindings or logic in them, they can be parsed as pure data.
*/
class RzDocument
{
public:

	enum ParsingFlags
	{
		ALLOW_EXPRESSIONS = 1 << 0, ///< Parser allows expressions and inline functions
		ALLOW_FUNCTIONS   = 1 << 1,   ///< Parser allows full fledged functions to be parsed 
	};

public:

	RzDocument();
	~RzDocument();

	void parse(const std::string& source);
	bool load(const std::string& filename);
	void save(const std::string& filename);

	void setParserFlags(ParsingFlags flags);

	RzValue& operator[](const std::string& name);

	RzValue root();

	// Todo: review
	void print();


private:
	std::unique_ptr<RzValue> m_root;
	uint32_t m_parsingFlags;
};
#endif // RZDOCUMENT_H__