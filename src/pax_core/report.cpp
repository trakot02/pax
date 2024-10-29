#include <cstdio>
#include <csignal>

#include <pax_core/report.hpp>

namespace pax
{
    //
    //
    // Implementation.
    //
    //

    Report_Level REPORT_LEVEL = report_level_success;
    Report_Guard REPORT_GUARD = report_guard_true;

    void
    report_panic(Report report)
    {
        if ( REPORT_LEVEL < report_level_panic )
            return;

        auto text = report.text;
        auto func = report.func;
        auto file = report.file;
        auto line = report.line;

        fprintf(stderr,
            "\x1b[35m[PANIC]\x1b[0m from %.*s at {%.*s, %li}:\n"
            "\x1b[35m[PANIC]\x1b[0m     %.*s.\n",
            (int) func.size, func.addr,
            (int) file.size, file.addr, line,
            (int) text.size, text.addr);

        raise(SIGABRT);
    }

    void
    report_error(Report report)
    {
        if ( REPORT_LEVEL < report_level_error )
            return;

        auto text = report.text;
        auto func = report.func;
        auto file = report.file;
        auto line = report.line;

        fprintf(stderr,
            "\x1b[31m[ERROR]\x1b[0m from %.*s at {%.*s, %li}:\n"
            "\x1b[31m[ERROR]\x1b[0m     %.*s.\n",
            (int) func.size, func.addr,
            (int) file.size, file.addr, line,
            (int) text.size, text.addr);
    }

    void
    report_warning(Report report)
    {
        if ( REPORT_LEVEL < report_level_warning )
            return;

        auto text = report.text;
        auto func = report.func;
        auto file = report.file;
        auto line = report.line;

        fprintf(stderr,
            "\x1b[33m[WARNING]\x1b[0m from %.*s at {%.*s, %li}:\n"
            "\x1b[33m[WARNING]\x1b[0m     %.*s.\n",
            (int) func.size, func.addr,
            (int) file.size, file.addr, line,
            (int) text.size, text.addr);
    }

    void
    report_message(Report report)
    {
        if ( REPORT_LEVEL < report_level_message )
            return;

        auto text = report.text;
        auto func = report.func;
        auto file = report.file;
        auto line = report.line;

        fprintf(stderr,
            "[MESSAGE] from %.*s at {%.*s, %li}:\n"
            "[MESSAGE]     %.*s.\n",
            (int) func.size, func.addr,
            (int) file.size, file.addr, line,
            (int) text.size, text.addr);
    }

    void
    report_success(Report report)
    {
        if ( REPORT_LEVEL < report_level_success )
            return;

        auto text = report.text;
        auto func = report.func;
        auto file = report.file;
        auto line = report.line;

        fprintf(stderr,
            "\x1b[32m[SUCCESS]\x1b[0m from %.*s at {%.*s, %li}:\n"
            "\x1b[32m[SUCCESS]\x1b[0m     %.*s.\n",
            (int) func.size, func.addr,
            (int) file.size, file.addr, line,
            (int) text.size, text.addr);
    }

    void
    report_debug(Report report)
    {
        if ( REPORT_LEVEL < report_level_debug )
            return;

        auto text = report.text;
        auto func = report.func;
        auto file = report.file;
        auto line = report.line;

        fprintf(stderr,
            "\x1b[36m[DEBUG]\x1b[0m from %.*s at {%.*s, %li}:\n"
            "\x1b[36m[DEBUG]\x1b[0m     %.*s.\n",
            (int) func.size, func.addr,
            (int) file.size, file.addr, line,
            (int) text.size, text.addr);
    }

    void
    report_trace(Report report)
    {
        if ( REPORT_LEVEL < report_level_trace )
            return;

        auto func = report.func;
        auto file = report.file;
        auto line = report.line;

        fprintf(stderr,
            "\x1b[34m[TRACE]\x1b[0m from %.*s at {%.*s, %li}\n",
            (int) func.size, func.addr,
            (int) file.size, file.addr, line);
    }

    void
    report_guard(str8 expr, Report report)
    {
        if ( REPORT_GUARD != report_guard_true )
            return;

        auto text = report.text;
        auto func = report.func;
        auto file = report.file;
        auto line = report.line;

        fprintf(stderr,
            "\x1b[35m[GUARD]\x1b[0m from %.*s at {%.*s, %li}:\n"
            "\x1b[35m[GUARD]\x1b[0m     guard '%.*s' failed.\n"
            "\x1b[35m[GUARD]\x1b[0m\n"
            "\x1b[35m[GUARD]\x1b[0m %.*s.\n",
            (int) func.size, func.addr,
            (int) file.size, file.addr, line,
            (int) expr.size, expr.addr,
            (int) text.size, text.addr);
    }
} // namespace pax
