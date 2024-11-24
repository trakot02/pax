#include <pax_core/report.hpp>

#include <signal.h>
#include <stdio.h>

namespace pax
{
    //
    //
    // Exposed.
    //
    //

    const Report_Level REPORT_LEVEL_BASE = REPORT_LEVEL_SUCCESS;
    const Report_Guard REPORT_GUARD_BASE = REPORT_GUARD_TRUE;

    Report_Level REPORT_LEVEL = REPORT_LEVEL_BASE;
    Report_Guard REPORT_GUARD = REPORT_GUARD_BASE;

    void
    report_error(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_ERROR ) return;

        fprintf(stderr,
            "\x1b[31m[ERROR]\x1b[0m %.*s at %.*s:%i\n",
            (int) report.text.count, report.text.block,
            (int) report.file.count, report.file.block,
            (int) report.line
        );
    }

    void
    report_warning(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_WARNING ) return;

        fprintf(stderr,
            "\x1b[33m[WARNING]\x1b[0m %.*s at %.*s:%i\n",
            (int) report.text.count, report.text.block,
            (int) report.file.count, report.file.block,
            (int) report.line
        );
    }

    void
    report_message(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_MESSAGE ) return;

        fprintf(stderr,
            "[MESSAGE] %.*s at %.*s:%i\n",
            (int) report.text.count, report.text.block,
            (int) report.file.count, report.file.block,
            (int) report.line
        );
    }

    void
    report_success(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_SUCCESS ) return;

        fprintf(stderr,
            "\x1b[32m[SUCCESS]\x1b[0m %.*s at %.*s:%i\n",
            (int) report.text.count, report.text.block,
            (int) report.file.count, report.file.block,
            (int) report.line
        );
    }

    void
    report_debug(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_DEBUG ) return;

        fprintf(stderr,
            "\x1b[34m[DEBUG]\x1b[0m %.*s at %.*s:%i\n",
            (int) report.text.count, report.text.block,
            (int) report.file.count, report.file.block,
            (int) report.line
        );
    }

    void
    report_guard(Str8 expr, Report report)
    {
        if ( REPORT_GUARD != REPORT_GUARD_TRUE ) return;

        fprintf(stderr,
            "\x1b[35m[GUARD]\x1b[0m guard \x1b[36m\"%.*s\"\x1b[0m failed at %.*s:%i\n"
            "    %.*s\n",
            (int) expr.count, expr.block,
            (int) report.file.count, report.file.block,
            (int) report.line,
            (int) report.text.count, report.text.block
        );
    }
} // namespace pax
