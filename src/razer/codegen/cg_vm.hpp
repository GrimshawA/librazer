#ifndef CODEGEN_VM_HPP_
#define CODEGEN_VM_HPP_

#include <razer/ir/ir.hpp>

class RzEngine;
class RzModule;

class CodeGenVM
{
public:
    explicit CodeGenVM(RzEngine* e);

    void build(IRContext& module);
    void build(IRFunction& func);

    void load(IRValue* value);

public: // Bytecode emission
    uint32_t emitInstruction(uint8_t opcode, int8_t arg0 = 0, int8_t arg1 = 0, int8_t arg2 = 0);

private:
    RzEngine& engine;
    RzModule* m_module = nullptr;
    int m_cursor = 0;
};

#endif // CODEGEN_VM_HPP_
