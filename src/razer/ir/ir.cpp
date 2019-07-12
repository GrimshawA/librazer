#include "ir.hpp"
#include <iostream>

void IRBuilder::beginBlock()
{

}

void IRBuilder::endBlock()
{

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
