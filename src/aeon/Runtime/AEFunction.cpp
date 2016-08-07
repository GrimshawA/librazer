#include <AEON/Runtime/AEFunction.h>
#include <AEON/Runtime/AEModule.h>
#include <AEON/VM/AEVm.h>

AEFunction::AEFunction()
: aeSymbol()
, m_struct(nullptr)
, m_native(false)
, m_compiled(false)
{

}

void AEFunction::call()
{
//	globalVm.call(this);
}

void AEFunction::call(const AEValueList& argumentList)
{
	//globalVm.call(this);
}

std::string AEFunction::getName()
{
	return getSymbolName();
}

AEType* AEFunction::getStruct()
{
	return m_struct;
}

aeQualType AEFunction::getReturnType()
{
	return returnType;
}

bool AEFunction::isVirtual()
{
	return m_virtual;
}

bool AEFunction::isNative()
{
	return m_native;
}