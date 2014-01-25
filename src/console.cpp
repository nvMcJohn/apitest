#include "pch.h"

#include "console.h"


#ifdef _WIN32
#   include <Windows.h>
#else
#   define OutputDebugString(_x) printf(_x)
#endif

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

    void error(const char* fmt, ...)
    {
        char buf[4096];
        va_list args;
        va_start(args, fmt);
        vsprintf_s(buf, sizeof(buf), fmt, args);
        va_end(args);

        OutputDebugString(buf);
        exit(-1);
    }
}
