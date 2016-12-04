#ifndef AEFunction_h__
#define AEFunction_h__

#include <AEON/Runtime/aeSymbol.h>
#include <AEON/Runtime/AEType.h>

#include <functional>

class AEValueList;

/**
	\class AEFunction
	\brief Encapsulates one callable compiled function

	All functions are just a contiguous range of instructions within the module.
	This structure provides utility to locate functions in the module code, as well as
	additional information about them.
*/
class AEFunction : public aeSymbol
{
public:
	AEFunction();

	void call();
	void call(const AEValueList& argumentList);

	std::string getName();
	AEType* getStruct();
	aeQualType getReturnType();

	bool isVirtual();
	bool isNative();

	void printByteCode();

public:

	uint32_t    m_offset;            ///< First instruction for this function in the owner module
	uint32_t    m_numInstructions;

	AEType*     m_struct;
	uint32_t    id;
	uint32_t    returnValueSize;   ///< Size the function reserves for the return value
	aeQualType returnType;
	std::vector<aeQualType> params;
	uint32_t    paramsStorageSize; ///< The amount of storage needed to push the params
	std::string decl;
	aeBindMethod fn;
	RzModule* m_module;
	bool m_native;
	bool m_compiled;               ///< Whether or not this function is compiled successfully, with a valid body
	bool m_virtual;
	std::function<void()> tmp;
};

#endif // AEFunction_h__
