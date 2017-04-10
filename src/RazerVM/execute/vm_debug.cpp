#include <RazerVM/ThreadContext.h>
#include <Rzr/RzEngine.h>
#include <Logger.h>

struct CanarySaveData {
    int esp;
    int ebp;
};

std::vector<CanarySaveData> s_canaryStack;

static inline void DoDebugOp(RzThreadContext& ctx, int a, int b) {
    if (a == 1) {
        // canary begin
        CanarySaveData d;
        d.esp = ctx.relativeStackPointer();
        s_canaryStack.push_back(d);
    }
    else if (a == 2) {
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
    }
    else {
        // debug log
        RZLOG("DEBUG: %s\n", ctx.engine->string_literals[b].c_str());
    }
}
