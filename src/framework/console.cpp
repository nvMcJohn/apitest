#include "pch.h"

#include "console.h"

#ifndef OutputDebugString
#   define OutputDebugString(_x)
#endif

#ifndef _WINDEF_
#   define sprintf_s(_buf, _sizeofbuf, _fmt, _args) sprintf(_buf, _fmt, _args) 
#   define vsprintf_s(_buf, _sizeofbuf, _fmt, _args) vsprintf(_buf, _fmt, _args) 
#endif

namespace console
{
    void print(const char* _string)
    {
        OutputDebugString(_string);
        printf("%s", _string);
    }

    // ----------------------------------------------------------------------------------------------------------------
    void debug(const char* _fmt, ...)
    {
        char buff[4096],
            finalBuff[4096];
        va_list args;
        va_start(args, _fmt);
        vsprintf_s(buff, sizeof(buff), _fmt, args);
        va_end(args);

        sprintf_s(finalBuff, sizeof(finalBuff), "Debug: %s\n", buff);
        console::print(finalBuff);
    }

    // ----------------------------------------------------------------------------------------------------------------
    void log(const char* _fmt, ...)
    {
        char buff[4096],
            finalBuff[4096];
        va_list args;
        va_start(args, _fmt);
        vsprintf_s(buff, sizeof(buff), _fmt, args);
        va_end(args);

        sprintf_s(finalBuff, sizeof(finalBuff), "%s\n", buff);
        console::print(finalBuff);
    }

    // ----------------------------------------------------------------------------------------------------------------
    void warn(const char* _fmt, ...)
    {
        char buff[4096],
            finalBuff[4096];
        va_list args;
        va_start(args, _fmt);
        vsprintf_s(buff, sizeof(buff), _fmt, args);
        va_end(args);

        sprintf_s(finalBuff, sizeof(finalBuff), "Warning: %s\n", buff);
        console::print(finalBuff);
    }

    // ----------------------------------------------------------------------------------------------------------------
    void error(const char* _fmt, ...)
    {
        char buff[4096],
            finalBuff[4096];
        va_list args;
        va_start(args, _fmt);
        vsprintf_s(buff, sizeof(buff), _fmt, args);
        va_end(args);

        sprintf_s(finalBuff, sizeof(finalBuff), "ERROR: %s\n", buff);
        console::print(finalBuff);

        exit(-1);
    }
}
