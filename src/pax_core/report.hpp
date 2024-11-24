#ifndef PAX_CORE_REPORT_HPP
#define PAX_CORE_REPORT_HPP

#include <pax_core/debug.hpp>
#include <pax_core/types.hpp>

// todo (trakot02):
//
// Implement a way to give extra information
// about functions and types.

namespace pax
{
    enum Report_Level : isize {
        REPORT_LEVEL_NONE    = 0,
        REPORT_LEVEL_ERROR   = 1,
        REPORT_LEVEL_WARNING = 2,
        REPORT_LEVEL_MESSAGE = 3,
        REPORT_LEVEL_SUCCESS = 4,
        REPORT_LEVEL_DEBUG   = 5,

        REPORT_LEVEL_COUNT = 6,
    };

    enum Report_Guard : isize {
        REPORT_GUARD_FALSE = 0,
        REPORT_GUARD_TRUE  = 1,

        REPORT_GUARD_COUNT = 2,
    };

    struct Report {
        Str8  text;
        Str8  file;
        usize line;
    };

    extern const Report_Level REPORT_LEVEL_BASE;
    extern const Report_Guard REPORT_GUARD_BASE;

    extern Report_Level REPORT_LEVEL;
    extern Report_Guard REPORT_GUARD;

    void
    report_error(Report report);

    void
    report_warning(Report report);

    void
    report_message(Report report);

    void
    report_success(Report report);

    void
    report_debug(Report report);

    void
    report_guard(Str8 expr, Report report);
} // namespace pax

#define pax_error(text) \
    pax::report_error({text, __FILE__, (usize) __LINE__})

#define pax_warning(text) \
    pax::report_warning({text, __FILE__, (usize) __LINE__})

#define pax_message(text) \
    pax::report_message({text, __FILE__, (usize) __LINE__})

#define pax_success(text) \
    pax::report_success({text, __FILE__, (usize) __LINE__})

#define pax_debug(text) \
    pax::report_debug({text, __FILE__, (usize) __LINE__})

#define pax_guard(expr, text) \
    do { \
        if ( (expr) == false ) { \
            pax::report_guard(#expr, { \
                text, __FILE__, (usize) __LINE__, \
            }); \
    \
            pax_debug_break(); \
        } \
    } while ( false );

#endif // PAX_CORE_REPORT_HPP
