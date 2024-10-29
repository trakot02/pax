#ifndef PAX_CORE_REPORT_HPP
#define PAX_CORE_REPORT_HPP

#include <pax_core/types.hpp>
#include <pax_core/debug.hpp>
#include <pax_core/str8.hpp>

namespace pax
{
    struct Report;

    enum Report_Level : isize {
        report_level_none    = 0,
        report_level_panic   = 1,
        report_level_error   = 2,
        report_level_warning = 3,
        report_level_message = 4,
        report_level_success = 5,
        report_level_debug   = 6,
        report_level_trace   = 7,

        report_level_count = 8,
    };

    enum Report_Guard : isize {
        report_guard_false = 0,
        report_guard_true  = 1,

        report_guard_count = 2,
    };

    struct Report {
        str8  text;
        str8  func;
        str8  file;
        isize line;
    };

    extern Report_Level REPORT_LEVEL;
    extern Report_Guard REPORT_GUARD;

    void
    report_panic(Report report);

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
    report_trace(Report report);

    void
    report_guard(str8 expr, Report report);
} // namespace pax

#define pax_panic(text) \
    pax::report_panic({text, __func__, __FILE__, __LINE__})

#define pax_error(text) \
    pax::report_error({text, __func__, __FILE__, __LINE__})

#define pax_warning(text) \
    pax::report_warning({text, __func__, __FILE__, __LINE__})

#define pax_message(text) \
    pax::report_message({text, __func__, __FILE__, __LINE__})

#define pax_success(text) \
    pax::report_success({text, __func__, __FILE__, __LINE__})

#define pax_debug(text) \
    pax::report_debug({text, __func__, __FILE__, __LINE__})

#define pax_trace() \
    pax::report_trace({"", __func__, __FILE__, __LINE__})

#define pax_guard(expr, text) \
    do { \
        if ( (expr) == false ) { \
            pax::report_guard(#expr, { \
                text, __func__, __FILE__, __LINE__, \
            }); \
    \
            pax_debug_break(); \
        } \
    } while ( false );

#endif // PAX_CORE_REPORT_HPP
