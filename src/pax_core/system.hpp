#ifndef PAX_CORE_SYSTEM_HPP
#define PAX_CORE_SYSTEM_HPP

#if defined(_WIN32)

    #define PAX_SYSTEM_WINDOWS

#elif defined(__linux__)

    #define PAX_SYSTEM_LINUX
    #define PAX_SYSTEM_POSIX

#elif defined(__unix__)

    #define PAX_SYSTEM_UNIX
    #define PAX_SYSTEM_POSIX

#else

    #define PAX_SYSTEM_OTHER

#endif

#if defined(_MSC_VER)

    #define PAX_COMPILER_MS

#else

    #define PAX_COMPILER_OTHER

#endif

#endif // PAX_CORE_SYSTEM_HPP
