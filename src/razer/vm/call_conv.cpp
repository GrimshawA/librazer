#include "call_conv.h"
#include <razer/vm/VirtualMachine.h>


void* RzCallConv::calculateFramePointer(void* esp)
{
    return esp;
}

int RzCallConv::calculateArgumentOffsetFromFramePointer(int argIndex)
{
    // Since the stack grows down, arguments are found at the positive side of frame pointer
    return argIndex * sizeof(RzStackValue);
}
