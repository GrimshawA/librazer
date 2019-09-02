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

IRValue* IRBuilder::createCall(IRValue* funcVal, const std::vector<IRValue*>& args)
{
    auto* call = new IRInstructionCall(funcVal);
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

IRValue* IRBuilder::createHeapAlloc(std::size_t bytes)
{
    auto* ptrValue = makeValue();

    auto* alloc = new IRInstructionStackAlloc();
    alloc->kind = IRInstructionStackAlloc::Heap;
    alloc->size = bytes;
    alloc->value = ptrValue;
    func.instructions.push_back(alloc);

    return ptrValue;
}

IRValue* IRBuilder::createBinaryOp(std::string op, IRValue* lhs, IRValue* rhs)
{
    auto* result = makeValue();
    result->name = "temp" + std::to_string(tempId++);
    func.instructions.push_back(new IRInstructionBinaryOp(op, lhs, rhs, result));
    return result;
}

IRValue* IRBuilder::createDestructure(RzType* type, int fieldIndex, IRValue* base)
{
    IRValue* ty = nullptr;

    // If this module already contains the type declaration, reuse the IRValue
    for (auto& t : ctx.types)
    {
        if (t.is(type))
        {
            ty = t.ty;
            break;
        }
    }

    if (!ty)
    {
        // Create a new type declaration, of sorts
        auto* typeValue = new IRValueType();
        typeValue->type = type;
        ty = typeValue;
    }


    auto* destructure = new IRInstructionDestructure();
    destructure->fieldIndex = fieldIndex;
    destructure->ty = ty;
    destructure->basePtr = base;
    func.instructions.push_back(destructure);

    return ty;
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
    auto* labelValue = new IRValueLabel;

	auto* label = new IRInstructionLabel();
    label->value = labelValue;
	func.instructions.push_back(label);

    return labelValue;
}

IRValue* IRBuilder::createLoad(IRValue* ptr)
{
    auto* loadedValue = new IRValue();

    auto* loadInst = new IRInstructionLoad();
    loadInst->value = loadedValue;
    loadInst->ptr = ptr;
    func.instructions.push_back(loadInst);

    return loadedValue;
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

IRValue* IRBuilder::makeFuncValue(RzFunction* func)
{
    auto* val = new IRValueFunc();
    val->func = func;
    return val;
}

IRValue* IRBuilder::makeFuncValue(const std::string& fullName)
{
    auto* val = new IRValueFunc();
    val->fullName = fullName;
    return val;
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
