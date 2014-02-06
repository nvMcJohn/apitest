#include "pch.h"

#include "timer.h"

namespace timer
{
    static Uint64 s_freq;
}

bool timer::Init()
{
    s_freq = SDL_GetPerformanceFrequency();
    return true;
}

Uint64 timer::Read()
{
    return SDL_GetPerformanceCounter();
}

Uint64 timer::TouSec(Uint64 x)
{
    return x * 1000000 / s_freq;
}

Uint64 timer::ToMSec(Uint64 x)
{
    return x * 1000 / s_freq;
}

double timer::ToSec(Uint64 x)
{
    return (double)x / (double)s_freq;
}
