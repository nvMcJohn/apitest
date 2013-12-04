#include "console.h"

#include <stdio.h>
#include <Windows.h>

namespace console
{
    void debug(const char* fmt, ...)
    {
        char buf[4096];
        va_list args;
        va_start(args, fmt);
        vsprintf_s(buf, sizeof(buf), fmt, args);
        va_end(args);

        OutputDebugString(buf);
    }
}
