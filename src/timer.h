#pragma once

#include "SDL.h"

namespace timer
{
    bool Init();

    Uint64 Read();

    Uint64 TouSec(Uint64 x);
    Uint64 ToMSec(Uint64 x);
    double ToSec(Uint64 x);
}
