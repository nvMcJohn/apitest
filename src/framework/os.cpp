#include "pch.h"

#include "os.h"
#if defined(_WIN32)
#   include <direct.h>
#elif defined(POSIX)
#   include <unistd.h>
#else
#   error "Need chdir functionality for this platform."
#endif

// --------------------------------------------------------------------------------------------------------------------
static inline std::string rstrip_min(std::string _strToStrip, const char* _charsToStrip, size_t _minLength)
{
    bool keepPopping = true;

    while (_strToStrip.length() > _minLength && keepPopping) {
        keepPopping = false;
        for (int i = 0; _charsToStrip[i]; ++i) {
            if (_strToStrip.back() == _charsToStrip[i]) {
                keepPopping = true;
                break;
            }
        }

        if (keepPopping) {
            _strToStrip.pop_back();
        }
    }

    return _strToStrip;
}

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
void os::chdir(std::string _newdir)
{
#if defined(_WIN32)
    _chdir(_newdir.c_str());

#elif defined(POSIX)
    ::chdir(_newdir.c_str());
#else
#   error "Implment os::chdir for this platform."
#endif
}

namespace os {
    namespace path {
        std::tuple<std::string, std::string> split(std::string _path)
        {
            #if defined(_WIN32)
                const char* kPathSeps = "\\/";
            #elif defined(POSIX)
                const char* kPathSeps = "\\/";
            #else
            #   error "Need to know path separators for this platform."
            #endif
            
            std::string::size_type splitPos = 1 + _path.find_last_of(kPathSeps);
            std::string head = _path.substr(0, splitPos);
            std::string tail = _path.substr(splitPos);

            head = rstrip_min(head, kPathSeps, 1);
            return std::make_tuple(head, tail);
        }
    }
}

// --------------------------------------------------------------------------------------------------------------------
std::string os::path::dirname(std::string _path)
{
    return std::get<0>(os::path::split(_path));
}
