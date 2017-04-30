#include <RazerCore/time/Timer.h>
#include <RazerRuntime/Module.h>
#include <Logger.h>

#include <SDL.h>

RzTimer::RzTimer() {
    RZLOG("TIMER INITIATED %x\n", this);
    m_lastTime = SDL_GetTicks();
}

uint32_t RzTimer::restart() {

    uint32_t now = SDL_GetTicks();
    uint32_t dt = now - m_lastTime;
    m_lastTime = now;

    RZLOG("TIMER restart %x %d\n", this, dt);
    return dt;
}

void RzTimer::registerApi(RzModule* mod) {
    mod->registerType("Timer", sizeof(RzTimer));
    mod->registerTypeConstructor("Timer", [](void* memory, RzVirtualMachine* vm)
    {
        new (memory) RzTimer();
    });

    mod->registerMethod("Timer", "int32 restart()", [](RzGeneric g){
        RzTimer* obj = (RzTimer*)g.popObject();
        uint32_t r = obj->restart();
        g.pushInt32((int32_t)r);
    });
}
