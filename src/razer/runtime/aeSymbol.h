#ifndef aeSymbol_h__
#define aeSymbol_h__

#include <razer/runtime/QualType.h>

class AEType;
class RzEngine;

#include <vector>
#include <stdint.h>
#include <string>


/**
	\class aeSymbol

	A symbol can be:
	- module
	- namespace
	- class
	- enum
	- struct
	- global/member var
	- function

	All symbols have a unique name so they can be found
	when referenced. It follows the format a.b.c... to mark nested
	constructs within other constructs.
*/
class aeSymbol
{
public:

	std::string m_absoluteName;    ///< Full name of this symbol (myModule.myNamespace.myClass.myFunction)

public:

	/// 
	std::string getSymbolName();
};

class aeField : public aeSymbol
{
public:
	uint16_t offset;   ///< Offset the field appears in the struct
	uint16_t size;     ///< Effective shallow size the field takes
	RzQualType type;   ///< Qualified type of this field
	std::string name;  ///< Local name identifier of the field
};

#endif // aeSymbol_h__
