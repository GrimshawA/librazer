#include "vm_api.h"
#include "call_conv.h"
#include "ThreadContext.h"
#include <razer/vm/ExecDispatch.h>
#include <razer/utils/Logger.h>

void RzCall(RzThreadContext* cx, RzFunction* fn)
{
    RZLOG("RzCall: %s. script func with %d args\n", fn->getName().c_str(), fn->params.size());

    RzCallConv callConv;

    RzStackFrame callinfo;
    callinfo.name = fn->getName();
    callinfo.object = nullptr;
    callinfo.pc = fn->m_offset;
    callinfo.module = fn->m_module;
    callinfo.ebp = cx->esp;
    callinfo.function = fn;
    cx->frames.push_back(callinfo);

    cx->ebp = (unsigned char*)callConv.calculateFramePointer((void*)cx->esp);

    //cx->alloc(fn->returnValueSize);

    // Launch the thread from this entry point
    dispatch_execute(*cx);
}
