#include <RazerRuntime/AEFunction.h>
#include <Rzr/RzModule.h>
#include <RazerVM/VirtualMachine.h>

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

RzType* AEFunction::getStruct()
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

void AEFunction::printByteCode()
{
	printf("bytecode %s\n", getName().c_str());
	for (int i = m_offset; i < m_numInstructions; ++i)
	{
		printf("fn %s\n", inst_opcode_str(m_module->m_code[i]).c_str());
	}
}