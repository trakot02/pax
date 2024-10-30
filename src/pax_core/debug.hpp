#ifndef PAX_CORE_DEBUG_HPP
#define PAX_CORE_DEBUG_HPP

#if defined(_WIN32)

    #define PAX_SYSTEM_WINDOWS

#elif defined(__linux__)

    #define PAX_SYSTEM_LINUX
    #define PAX_SYSTEM_POSIX

#elif defined(__unix__)

    #define PAX_SYSTEM_UNIX
    #define PAX_SYSTEM_POSIX

#else

    #define PAX_SYSTEM_UNKNOWN

#endif

#if defined(_MSC_VER)

    #define pax_debug_break() \
        __debugbreak()

#elif defined(PAX_SYSTEM_POSIX)

    #include <signal.h>

    #define pax_debug_break() \
        raise(SIGTRAP)

#else

    #define pax_debug_break()

#endif

#endif // PAX_CORE_DEBUG_HPP
