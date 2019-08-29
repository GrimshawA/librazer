#include "ir.hpp"
#include <iostream>

IRValue* IRContext::createType(RzType* ty, const std::string& name, const std::vector<IRType::Field>& fields)
{
    IRValueType* typeValue = new IRValueType();
    typeValue->type = ty;

    IRType t (name);
    t.fields = fields;
    t.ty = typeValue;
    types.push_back(t);

    return typeValue;
}

void IRContext::createExternalType(RzType* ty)
{
    IRType t (ty);
    types.push_back(t);
}

void IRContext::writeToFile(const std::string& filename)
{
    FILE* fp = fopen(filename.c_str(), "w");

    for (auto& t : types)
    {
        fprintf(fp, t.prettyString().c_str());
    }

    for (auto& f : functions)
    {
        fprintf(fp, "%s\n", f.prettyString().c_str());
        for (auto& i : f.instructions)
        {
            fprintf(fp, "%s\n", i->prettyString().c_str());
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}
