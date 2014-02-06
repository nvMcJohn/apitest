#include "pch.h"

#include "console.h"

#ifndef OutputDebugString
#   define OutputDebugString(_x) printf("%s\n", _x)
#endif

#ifndef _WINDEF_
    #define vsprintf_s(_buf, _sizeofbuf, _fmt, _args) vsprintf(_buf, _fmt, _args) 
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
