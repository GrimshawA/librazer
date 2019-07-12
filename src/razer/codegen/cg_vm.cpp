#include "cg_vm.hpp"

void CodeGenVM::build(IRModule& module)
{

}

void CodeGenVM::build(IRFunction& func)
{
    using namespace IR;

    for (auto& inst : func.instructions)
    {
        switch(inst->type)
        {
            case ASSIGN:
                break;

            case ALLOCATE:
                break;
        }
    }
}
