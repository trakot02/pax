#include <pax_core/report.hpp>

#include <signal.h>
#include <stdio.h>

namespace pax
{
    //
    //
    // Implementation.
    //
    //

    const Report_Level REPORT_LEVEL_BASE = REPORT_LEVEL_SUCCESS;
    const Report_Panic REPORT_PANIC_BASE = REPORT_PANIC_TRUE;
    const Report_Guard REPORT_GUARD_BASE = REPORT_GUARD_TRUE;

    Report_Level REPORT_LEVEL = REPORT_LEVEL_BASE;
    Report_Panic REPORT_PANIC = REPORT_PANIC_BASE;
    Report_Guard REPORT_GUARD = REPORT_GUARD_BASE;

    void
    report_fatal(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_FATAL ) return;

        fprintf(stderr,
            "\x1b[31m[FATAL]\x1b[0m from '%.*s' at {%.*s, %i}:\n"
            "\x1b[31m[FATAL]\x1b[0m     '%.*s'.\n",
            (int) report.func.cnt, report.func.ptr,
            (int) report.file.cnt, report.file.ptr,
            (int) report.line,
            (int) report.text.cnt, report.text.ptr
        );
    }

    void
    report_error(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_ERROR ) return;

        fprintf(stderr,
            "\x1b[31m[ERROR]\x1b[0m from '%.*s' at {%.*s, %i}:\n"
            "\x1b[31m[ERROR]\x1b[0m     '%.*s'.\n",
            (int) report.func.cnt, report.func.ptr,
            (int) report.file.cnt, report.file.ptr,
            (int) report.line,
            (int) report.text.cnt, report.text.ptr
        );
    }

    void
    report_warning(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_WARNING ) return;

        fprintf(stderr,
            "\x1b[33m[WARNING]\x1b[0m from '%.*s' at {%.*s, %i}:\n"
            "\x1b[33m[WARNING]\x1b[0m     '%.*s'.\n",
            (int) report.func.cnt, report.func.ptr,
            (int) report.file.cnt, report.file.ptr,
            (int) report.line,
            (int) report.text.cnt, report.text.ptr
        );
    }

    void
    report_message(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_MESSAGE ) return;

        fprintf(stderr,
            "[MESSAGE] from '%.*s' at {%.*s, %i}:\n"
            "[MESSAGE]     '%.*s'.\n",
            (int) report.func.cnt, report.func.ptr,
            (int) report.file.cnt, report.file.ptr,
            (int) report.line,
            (int) report.text.cnt, report.text.ptr
        );
    }

    void
    report_success(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_SUCCESS ) return;

        fprintf(stderr,
            "\x1b[32m[SUCCESS]\x1b[0m from '%.*s' at {%.*s, %i}:\n"
            "\x1b[32m[SUCCESS]\x1b[0m     '%.*s'.\n",
            (int) report.func.cnt, report.func.ptr,
            (int) report.file.cnt, report.file.ptr,
            (int) report.line,
            (int) report.text.cnt, report.text.ptr
        );
    }

    void
    report_debug(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_DEBUG ) return;

        fprintf(stderr,
            "\x1b[34m[DEBUG]\x1b[0m from '%.*s' at {%.*s, %i}:\n"
            "\x1b[34m[DEBUG]\x1b[0m     '%.*s'.\n",
            (int) report.func.cnt, report.func.ptr,
            (int) report.file.cnt, report.file.ptr,
            (int) report.line,
            (int) report.text.cnt, report.text.ptr
        );
    }

    void
    report_trace(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_TRACE ) return;

        fprintf(stderr,
            "\x1b[34m[TRACE]\x1b[0m from '%.*s' at {%.*s, %i}\n",
            (int) report.func.cnt, report.func.ptr,
            (int) report.file.cnt, report.file.ptr,
            (int) report.line
        );
    }

    void
    report_panic(Report report)
    {
        if ( REPORT_PANIC != REPORT_PANIC_TRUE ) return;

        fprintf(stderr,
            "\x1b[35m[PANIC]\x1b[0m from '%.*s' at {%.*s, %i}:\n"
            "\x1b[35m[PANIC]\x1b[0m     '%.*s'.\n",
            (int) report.func.cnt, report.func.ptr,
            (int) report.file.cnt, report.file.ptr,
            (int) report.line,
            (int) report.text.cnt, report.text.ptr
        );

        raise(SIGABRT);
    }

    void
    report_guard(Str8 expr, Report report)
    {
        if ( REPORT_GUARD != REPORT_GUARD_TRUE ) return;

        fprintf(stderr,
            "\x1b[36m[GUARD]\x1b[0m from '%.*s' at {%.*s, %i}:\n"
            "\x1b[36m[GUARD]\x1b[0m     guard '%.*s' failed.\n"
            "\x1b[36m[GUARD]\x1b[0m\n"
            "\x1b[36m[GUARD]\x1b[0m '%.*s'.\n",
            (int) report.func.cnt, report.func.ptr,
            (int) report.file.cnt, report.file.ptr,
            (int) report.line, (int) expr.cnt, expr.ptr,
            (int) report.text.cnt, report.text.ptr
        );
    }
} // namespace pax
