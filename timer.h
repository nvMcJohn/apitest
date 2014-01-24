#pragma once

#include "SDL.h"

namespace timer
{
    bool init();

    Uint64 read();

    Uint64 to_usec(Uint64 x);
    Uint64 to_msec(Uint64 x);
    double to_sec(Uint64 x);
}
