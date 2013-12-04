#pragma once

namespace timer
{
    bool init();

    unsigned long long read();

    unsigned long long to_usec(unsigned long long x);
    unsigned long long to_msec(unsigned long long x);
    double to_sec(unsigned long long x);
}
