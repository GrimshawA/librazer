#include "ir.hpp"
#include <iostream>

void IRBuilder::dumpToFile(const std::string& filename)
{
    static FILE* fp = fopen(filename.c_str(), "w");

    for (auto& i : instructions)
    {
        fprintf(fp, "INST\n");
    }

    //fclose(fp);
}
