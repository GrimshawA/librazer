#ifndef RZFUNCTION_H__
#define RZFUNCTION_H__

#include <razer/runtime/aeSymbol.h>
#include <razer/runtime/RzType.h>

#include <functional>

class AEValueList;

/**
    \class RzFunction
    \brief Information about a compiled function on the module

    When a razer function is compiled into a module, it is just a chunk of bytecode.
    This structure helps identify where functions begin and end, as well as other
    important information.
*/
class RzFunction : public aeSymbol
{
public:
    RzFunction();

	void call();
	void call(const AEValueList& argumentList);

	std::string getName();
	RzType* getStruct();
	RzQualType getReturnType();

	bool isVirtual();
	bool isNative();

	void printByteCode();

public:

	uint32_t    m_offset;            ///< First instruction for this function in the owner module
	uint32_t    m_numInstructions;

	RzType*     m_struct;
	uint32_t    id;
	uint32_t    returnValueSize;   ///< Size the function reserves for the return value
	RzQualType returnType;
	std::vector<RzQualType> params;
	uint32_t    paramsStorageSize; ///< The amount of storage needed to push the params
	std::string decl;
	aeBindMethod fn;
	RzModule* m_module;
	bool m_native;
	bool m_compiled;               ///< Whether or not this function is compiled successfully, with a valid body
	bool m_virtual;
	std::function<void()> tmp;
};

#endif // RZFUNCTION_H__
