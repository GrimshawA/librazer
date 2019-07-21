#include <RazerRuntime/Function.h>
#include <RazerRuntime/Module.h>
#include <razer/vm/VirtualMachine.h>

#include <razer/utils/Logger.h>

RzFunction::RzFunction()
: aeSymbol()
, m_struct(nullptr)
, m_native(false)
, m_compiled(false) {

}

void RzFunction::call() {
//	globalVm.call(this);
}

void RzFunction::call(const AEValueList& argumentList) {
	//globalVm.call(this);
}

std::string RzFunction::getName() {
	return getSymbolName();
}

RzType* RzFunction::getStruct() {
	return m_struct;
}

RzQualType RzFunction::getReturnType() {
	return returnType;
}

bool RzFunction::isVirtual() {
	return m_virtual;
}

bool RzFunction::isNative() {
	return m_native;
}

void RzFunction::printByteCode() {
	RZLOG("bytecode %s\n", getName().c_str());
	for (int i = m_offset; i < m_numInstructions; ++i)
	{
		RZLOG("fn %s\n", inst_opcode_str(m_module->m_code[i]).c_str());
	}
}
