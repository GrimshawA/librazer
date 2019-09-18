#ifndef CODEGEN_VM_HPP_
#define CODEGEN_VM_HPP_

#include <razer/ir/ir.hpp>

class RzEngine;
class RzModule;
class RzCallConv;

class CodeGenVM
{
public:
    explicit CodeGenVM(RzEngine* e);

    void build(IRContext& module);
    void build(IRFunction& func);

    void buildStore(IRInstructionStore& inst);
    void buildLoad(IRInstructionLoad& inst);
    void buildCall(IRInstructionCall& inst);
    void buildLabel(IRInstructionLabel& inst);
    void buildJump(IRInstructionJump& inst);
    void buildDestructure(IRInstructionDestructure& inst);
    void buildAlloc(IRInstructionStackAlloc& inst);

    // Does whatever is required to get the IR value on the stack
    void loadValueToStack(IRValue* value);

public: // Args related
    bool isArgument(IRValue* val);
    int getArgumentOffsetFromEbp(IRValue* val);

public: // Value usage inspection
    int countReads(IRValue* val);

public: // Call convention
    RzCallConv* deduceCallConvention(IRInstructionCall& inst);

public: // Bytecode emission
    uint32_t emitInstruction(uint8_t opcode, int8_t arg0 = 0, int8_t arg1 = 0, int8_t arg2 = 0);
    uint32_t emitInstructionB(uint8_t opcode, uint16_t arg0, uint8_t arg1);

private:
    RzEngine& engine;
    RzModule* m_module = nullptr;
    int m_cursor = 0;
    IRFunction* currentFunc = nullptr;

    std::unordered_map<IRValue*, int> m_labels;
};

#endif // CODEGEN_VM_HPP_
