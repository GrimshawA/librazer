#include "ir_builder.hpp"

IRBuilder::IRBuilder(IRContext& ctx)
    : ctx(ctx)
{

}

IRBuilder::~IRBuilder()
{
    ctx.functions.push_back(func);
}

void IRBuilder::beginBlock()
{

}

void IRBuilder::endBlock()
{

}

void IRBuilder::createStore(IRValue* memory, IRValue* value)
{
    func.instructions.push_back(new IRInstructionStore(memory, value));
}

void IRBuilder::createJumpStmt(IRValue* target)
{
	auto* jump = new IRInstructionJump(target);
	func.instructions.push_back(jump);
}

IRValue* IRBuilder::createCall(const std::vector<IRValue*>& args)
{
    auto* call = new IRInstructionCall();
    call->args = args;
    func.instructions.push_back(call);
    return makeValue();
}

IRValue* IRBuilder::createLocalAlloc()
{
    auto* alloc = new IRInstructionStackAlloc();
    func.instructions.push_back(alloc);
    return makeValue();
}

IRValue* IRBuilder::binaryOp(std::string op, IRValue* lhs, IRValue* rhs)
{
    auto* result = makeValue();
    result->name = "temp" + std::to_string(tempId++);
    func.instructions.push_back(new IRInstructionBinaryOp(op, lhs, rhs, result));
    return result;
}

IRValue* IRBuilder::newObject(IRValue* typeValue)
{
    auto* tmp = makeTempValue();
    func.instructions.push_back(new IRInstructionNew());
    return tmp;
}

IRValue* IRBuilder::createReturn()
{
    func.instructions.push_back(new IRInstructionReturn());
    return nullptr;
}

IRValue* IRBuilder::createLabelStmt()
{
	auto* label = new IRInstructionLabel();
	func.instructions.push_back(label);
	return makeValue();
}

IRValue* IRBuilder::makeValue()
{
    return new IRValue();
}

IRValue* IRBuilder::makeTempValue()
{
    auto* result = makeValue();
    result->name = "temp" + std::to_string(tempId++);
    return result;
}

IR::Type IRBuilder::getLastStmtType()
{
    if (func.instructions.empty())
    {
        return IR::Undefined;
    }

    return func.instructions.back()->type;
}

IRValue* IRBuilder::getValue(const std::string& name)
{
    return nullptr;
}
