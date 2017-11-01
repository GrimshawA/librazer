#ifndef RAZERCORETIMER_H__
#define RAZERCORETIMER_H__

#include <stdint.h>

class RzModule;

/*
 *  Simple timer utility to track elapsed time
 */
class RzTimer {
public:
    RzTimer();

    uint32_t restart();

    static void registerApi(RzModule* mod);

private:
    uint32_t m_lastTime;
};

#endif
