#include "ir.hpp"
#include <iostream>

void IRBuilder::beginBlock()
{

}

void IRBuilder::endBlock()
{

}

void IRBuilder::call()
{
    func.instructions.push_back(new IRInstructionCall());
}

IRValue* IRBuilder::binaryOp(std::string op, IRValue* lhs, IRValue* rhs)
{
    auto* result = makeValue();
    result->name = "temp" + std::to_string(tempId++);
    func.instructions.push_back(new IRInstructionBinaryOp(op, lhs, rhs, result));
    return result;
}

IRValue* IRBuilder::newObject()
{
    auto* tmp = makeTempValue();
    func.instructions.push_back(new IRInstructionNew());
    return tmp;
}

IRValue* IRBuilder::newType(const std::string& name)
{
    auto* tmp = makeTempValue();
    func.instructions.push_back(new IRInstructionType(name));
    return tmp;
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

IRValue* IRBuilder::getValue(const std::string& name)
{
    return nullptr;
}

void IRBuilder::dumpToFile(const std::string& filename)
{
    static FILE* fp = fopen(filename.c_str(), "w");

    for (auto& i : func.instructions)
    {
        fprintf(fp, "%s\n", i->prettyString().c_str());
    }

    //fclose(fp);
}
