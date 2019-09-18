#include "call_conv.h"
#include <razer/vm/VirtualMachine.h>

void RzCallConv::iterateArgs(const std::vector<IRValue*>& args, const ArgsVisitor& visitor)
{
    // Right to left pushing
    for (int i = args.size() - 1; i >= 0; i--)
    {
        visitor(args[i]);
    }
}

void* RzCallConv::calculateFramePointer(void* esp)
{
    return esp;
}

int RzCallConv::calculateArgumentOffsetFromFramePointer(int argIndex)
{
    // Since the stack grows down, arguments are found at the positive side of frame pointer
    return argIndex * sizeof(RzStackValue);
}
