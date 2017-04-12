#include <RazerCore/time/Timer.h>
#include <Rzr/RzModule.h>
#include <Logger.h>

RzTimer::RzTimer() {
    RZLOG("TIMER INITIATED %x\n", this);
    //m_lastTime = SDL_GetTicks();
}

uint32_t RzTimer::restart() {
    RZLOG("TIMER restart %x\n", this);
    uint32_t now = 0;//SDL_GetTicks();
    uint32_t dt = now - m_lastTime;
    m_lastTime = now;
    return dt;
}

void RzTimer::registerApi(RzModule* mod) {
    mod->registerType("Timer", sizeof(RzTimer));
    mod->registerTypeConstructor("Timer", [](void* memory, RzVirtualMachine* vm)
    {
        new (memory) RzTimer();
    });

    mod->registerMethod("Timer", "int restart()", [](AEGeneric g){
        RzTimer* obj = (RzTimer*)g.unpack_ptr();
        uint32_t r = obj->restart();
        g.pack_uint32(r);
    });
}
