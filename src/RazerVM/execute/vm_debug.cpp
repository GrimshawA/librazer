#include <RazerVM/ThreadContext.h>
#include <RazerRuntime/RzEngine.h>
#include <Logger.h>

struct CanarySaveData {
    int esp;
    int ebp;
};

static std::vector<CanarySaveData> s_canaryStack;

static inline void DoDebugOp(RzThreadContext& ctx, int a, int b) {

    switch(a) {
    case DBG_LOG: {
        RZLOG("DEBUG: %s\n", ctx.engine->string_literals[b].c_str());
        break;
    }
    case DBG_STACKCANARYBEGIN: {
        // canary begin
        CanarySaveData d;
        d.esp = ctx.relativeStackPointer();
        s_canaryStack.push_back(d);
        break;
    }
    case DBG_STACKCANARYEND: {
        // canary end
        if (s_canaryStack.empty()) {
            RZLOG("Canary end mismatch. No canaries left.\n");
            return;
        }

        CanarySaveData d = s_canaryStack[s_canaryStack.size()-1];

        if (d.esp != ctx.relativeStackPointer()) {
            RZLOG("> > > Canary detected a leak. ESP was %d and now %d\n", d.esp, ctx.relativeStackPointer());
        }

        s_canaryStack.pop_back();
        break;
    }
    case DBG_TRACE: {
        RZLOG("VMTRACE: EBP %x ESP %d\n", ctx.ebp, ctx.relativeStackPointer());
        break;
    }
    }
}
