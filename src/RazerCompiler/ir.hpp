#ifndef IR_HPP
#define IR_HPP

#include <vector>
#include <string>

class IRInstruction
{

};

class IRInstructionDestructure : public IRInstruction
{

};

class IRBuilder
{
public:

    void Dummy()
    {
        instructions.push_back(new IRInstructionDestructure());
    }

    void Assign() {}

    void dumpToFile(const std::string& filename);

    std::vector<IRInstruction*> instructions;
};

#endif // IR_HPP
