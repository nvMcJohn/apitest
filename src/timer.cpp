#include "pch.h"

#include "timer.h"

namespace timer
{
    static Uint64 s_freq;
}

bool timer::init()
{
    s_freq = SDL_GetPerformanceFrequency();
    return true;
}

Uint64 timer::read()
{
    return SDL_GetPerformanceCounter();
}

Uint64 timer::to_usec(Uint64 x)
{
    return x * 1000000 / s_freq;
}

Uint64 timer::to_msec(Uint64 x)
{
    return x * 1000 / s_freq;
}

double timer::to_sec(Uint64 x)
{
    return (double)x / (double)s_freq;
}
