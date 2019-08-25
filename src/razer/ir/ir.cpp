#include "ir.hpp"
#include <iostream>

void IRContext::createType(const std::string& name, const std::vector<IRType::Field>& fields)
{
    IRType t (name);
    t.fields = fields;
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
        fprintf(fp, "def %s\n", f.path.c_str());
        for (auto& i : f.instructions)
        {
            fprintf(fp, "%s\n", i->prettyString().c_str());
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}
