#include "ir.hpp"
#include <iostream>

void IRContext::writeToFile(const std::string& filename)
{
    FILE* fp = fopen(filename.c_str(), "w");

    for (auto& f : functions)
    {
        fprintf(fp, "def %s\n", f.path.c_str());
        for (auto& i : f.instructions)
        {
            fprintf(fp, "%s\n", i->prettyString().c_str());
        }
    }

    fclose(fp);
}
