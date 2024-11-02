#ifndef PAX_CORE_DEBUG_HPP
#define PAX_CORE_DEBUG_HPP

#include <pax_core/system.hpp>

#if defined(PAX_COMPILER_MS)

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
