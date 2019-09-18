#include "cg_vm.hpp"
#include <razer/vm/call_conv.h>
#include <razer/runtime/RzEngine.h>

#include <cassert>

CodeGenVM::CodeGenVM(RzEngine* e)
    : engine(*e)
{

}

void CodeGenVM::build(IRContext& module)
{
    m_module = engine.createModule("test");

    for (auto& func : module.functions)
    {
        build(func);
    }
}

void CodeGenVM::build(IRFunction& func)
{
    using namespace IR;

    currentFunc = &func;

    auto* vmFun = m_module->createFunction("App." + func.name);
    vmFun->m_offset = m_cursor;
    vmFun->m_compiled = true;
    vmFun->m_module = m_module;

    for (auto& inst : func.instructions)
    {
        switch(inst->type)
        {

        case ASSIGN:
            break;

        case Destructure:
            buildDestructure(static_cast<IRInstructionDestructure&>(*inst));
            break;

        case StackAlloc:
            buildAlloc(static_cast<IRInstructionStackAlloc&>(*inst));
            break;

        case New:
            emitInstruction(OP_NEW, 0, 0 , 0);
            break;

        case Store:
            buildStore(static_cast<IRInstructionStore&>(*inst));
            break;

        case Load:
            buildLoad(static_cast<IRInstructionLoad&>(*inst));
            break;

        case Label:
            buildLabel(static_cast<IRInstructionLabel&>(*inst));
            break;

        case Jump:
            buildJump(static_cast<IRInstructionJump&>(*inst));
            break;

        case Return:
            emitInstruction(OP_RETURN, 0, 0 , 0);
            break;

        case CALL:
            buildCall(static_cast<IRInstructionCall&>(*inst));
            break;
        }
    }

    currentFunc = nullptr;
}

void CodeGenVM::buildStore(IRInstructionStore& inst)
{


    emitInstruction(OP_SET, 0, 0, AEP_PTR);
}

void CodeGenVM::buildLoad(IRInstructionLoad& inst)
{
    emitInstruction(OP_DEREF, AEP_PTR);
}

void CodeGenVM::buildCall(IRInstructionCall& inst)
{
    int moduleIndex = 0;
    int functionIndex = 0;

    auto* func = static_cast<IRValueFunc*>(inst.funcValue);

    if (!func)
        return;

    RzFunction* vmFun = engine.getFunctionByName(func->fullName);
    moduleIndex = engine.getModuleIndex(vmFun->m_module);
    functionIndex = vmFun->m_module->getFunctionIndex(vmFun);

    // We're stack based, left-to-right push?
    auto* callConv = deduceCallConvention(inst);

    callConv->iterateArgs(inst.args, [&](IRValue* arg) {
        loadValueToStack(arg);
    });

    if (vmFun->isNative())
        emitInstruction(OP_NATIVECALL, moduleIndex, functionIndex, 0);
    else
        emitInstruction(OP_CALL, moduleIndex, functionIndex);
}

void CodeGenVM::buildLabel(IRInstructionLabel& inst)
{
    auto pos = m_cursor;
    m_labels[inst.value] = pos;
}

void CodeGenVM::buildJump(IRInstructionJump& inst)
{
    int pos = m_labels[inst.target];
    int diff = m_cursor - pos;
    diff += 1;
    emitInstruction(OP_JMP, -diff, 0, 0);
}

void CodeGenVM::buildDestructure(IRInstructionDestructure& inst)
{
    assert (inst.ty);
    assert (inst.ty->isType());

    IRValueType* ty = static_cast<IRValueType*>(inst.ty);

    int offset = 0;

    if (isArgument(inst.basePtr))
    {
        offset = getArgumentOffsetFromEbp(inst.basePtr);
    }

    int memberOffset = ty->type->m_fields[inst.fieldIndex].offset;
    //emitInstruction(OP_LOADADDR, AEK_EBP, offset, memberOffset);

    // Actually do basePtr + offset
    //emitInstruction(OP_LOADADDR, AEK_THIS, 0, 0);

    // Load the "this" value into the stack
    emitInstruction(OP_LOAD, AEK_EBP, offset, AEP_PTR);

    // Actually destructure the member out of the "this" value
    emitInstruction(OP_LOADADDR, AEK_THIS, memberOffset, 0);
}

void CodeGenVM::buildAlloc(IRInstructionStackAlloc& inst)
{
    emitInstructionB(OP_ALLOC, inst.size, 0);

    int numAccesses = countReads(inst.value);

    if (numAccesses > 1)
        emitInstruction(OP_DUP);
}

void CodeGenVM::loadValueToStack(IRValue* value)
{
    // Make a backtracker which finds the origin IRValue*
    // and a list of instructions that transform it into what we need


    if (dynamic_cast<IRValueInt*>(value))
    {
        IRValueInt* integer = static_cast<IRValueInt*>(value);
        auto idx = engine.getIntegerLiteral(integer->value);
        emitInstruction(OP_LOADK, AEK_INT, idx);
    }
}

bool CodeGenVM::isArgument(IRValue* val)
{
    for (auto& a : currentFunc->args)
    {
        if (a == val)
            return true;
    }
    return false;
}

int CodeGenVM::getArgumentOffsetFromEbp(IRValue* val)
{
    return 0;
}

int CodeGenVM::countReads(IRValue* val)
{
    int c = 0;
    for (auto& inst : currentFunc->instructions)
    {
        c += inst->doesReadValue(val) ? 1 : 0;
    }
    return c;
}

RzCallConv* CodeGenVM::deduceCallConvention(IRInstructionCall& inst)
{
    static RzCallConv conv;
    return &conv;
}

uint32_t CodeGenVM::emitInstruction(uint8_t opcode, int8_t arg0, int8_t arg1, int8_t arg2)
{
    RzInstruction instr;
    instr.opcode = opcode;
    instr.a.arg0 = arg0;
    instr.a.arg1 = arg1;
    instr.a.arg2 = arg2;
    m_module->m_code.push_back(instr);
    return m_cursor++;
}

uint32_t CodeGenVM::emitInstructionB(uint8_t opcode, uint16_t arg0, uint8_t arg1)
{
    RzInstruction instr;
    instr.opcode = opcode;
    instr.b.arg0 = arg0;
    instr.b.arg1 = arg1;
    m_module->m_code.push_back(instr);
    return m_cursor++;
}
