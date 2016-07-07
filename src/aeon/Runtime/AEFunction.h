#ifndef AEFunction_h__
#define AEFunction_h__

#include <AEON/aeSymbol.h>
#include <AEON/Runtime/AEType.h>

class AEValueList;

/**
	\class AEFunction
	\brief Encapsulates one callable compiled function
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



	AEType*     m_struct;
	uint32_t    id;
	uint32_t    returnValueSize;   ///< Size the function reserves for the return value
	aeQualType returnType;
	std::vector<aeQualType> params;
	uint32_t    paramsStorageSize; ///< The amount of storage needed to push the params
	uint32_t    offset;            ///< which instruction this function starts in the code
	std::string decl;
	aeBindMethod fn;
	AEModule* m_module;
	bool m_native;
	bool m_compiled;               ///< Whether or not this function is compiled successfully, with a valid body
	bool m_virtual;
};

#endif // AEFunction_h__
