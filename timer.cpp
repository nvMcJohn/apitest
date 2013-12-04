#include "timer.h"

#include <Windows.h>

namespace timer
{
    static unsigned long long s_freq;
}

bool timer::init()
{
    LARGE_INTEGER f;
    if (!QueryPerformanceFrequency(&f))
    {
        return false;
    }

    s_freq = f.QuadPart;
    return true;
}

unsigned long long timer::read()
{
    LARGE_INTEGER f;
    QueryPerformanceCounter(&f);
    return f.QuadPart;
}

unsigned long long timer::to_usec(unsigned long long x)
{
    return x * 1000000 / s_freq;
}

unsigned long long timer::to_msec(unsigned long long x)
{
    return x * 1000 / s_freq;
}

double timer::to_sec(unsigned long long x)
{
    return (double)x / (double)s_freq;
}
