#include <pax_core/report.hpp>
#include <pax_core/buff.hpp>
#include <pax_core/file.hpp>

#include <signal.h>

namespace pax
{
    //
    //
    // Implementation.
    //
    //

    static Array<byte, 1024> temp = {};

    static Buff buff = buff_from_addr(temp.addr, temp.size);

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

        buff_clear(&buff);
        buff_write_s8(&buff, "\x1b[35m[FATAL]\x1b[0m from ");
        buff_write_s8(&buff, report.func);
        buff_write_s8(&buff, " at {");
        buff_write_s8(&buff, report.file);
        buff_write_s8(&buff, ", ");
        buff_write_u64(&buff, report.line);
        buff_write_s8(&buff, "}:\n");
        buff_write_s8(&buff, "\x1b[35m[FATAL]\x1b[0m     '");
        buff_write_s8(&buff, report.text);
        buff_write_s8(&buff, "'.\n");

        stderr_write_buff(&buff);
    }

    void
    report_error(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_ERROR ) return;

        buff_clear(&buff);
        buff_write_s8(&buff, "\x1b[31m[ERROR]\x1b[0m from ");
        buff_write_s8(&buff, report.func);
        buff_write_s8(&buff, " at {");
        buff_write_s8(&buff, report.file);
        buff_write_s8(&buff, ", ");
        buff_write_u64(&buff, report.line);
        buff_write_s8(&buff, "}:\n");
        buff_write_s8(&buff, "\x1b[31m[ERROR]\x1b[0m     '");
        buff_write_s8(&buff, report.text);
        buff_write_s8(&buff, "'.\n");

        stderr_write_buff(&buff);
    }

    void
    report_warning(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_WARNING ) return;

        buff_clear(&buff);
        buff_write_s8(&buff, "\x1b[33m[WARNING]\x1b[0m from ");
        buff_write_s8(&buff, report.func);
        buff_write_s8(&buff, " at {");
        buff_write_s8(&buff, report.file);
        buff_write_s8(&buff, ", ");
        buff_write_u64(&buff, report.line);
        buff_write_s8(&buff, "}:\n");
        buff_write_s8(&buff, "\x1b[33m[WARNING]\x1b[0m     '");
        buff_write_s8(&buff, report.text);
        buff_write_s8(&buff, "'.\n");

        stderr_write_buff(&buff);
    }

    void
    report_message(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_MESSAGE ) return;

        buff_clear(&buff);
        buff_write_s8(&buff, "[MESSAGE] from ");
        buff_write_s8(&buff, report.func);
        buff_write_s8(&buff, " at {");
        buff_write_s8(&buff, report.file);
        buff_write_s8(&buff, ", ");
        buff_write_u64(&buff, report.line);
        buff_write_s8(&buff, "}:\n");
        buff_write_s8(&buff, "[MESSAGE]     '");
        buff_write_s8(&buff, report.text);
        buff_write_s8(&buff, "'.\n");

        stderr_write_buff(&buff);
    }

    void
    report_success(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_SUCCESS ) return;

        buff_clear(&buff);
        buff_write_s8(&buff, "\x1b[32m[SUCCESS]\x1b[0m from ");
        buff_write_s8(&buff, report.func);
        buff_write_s8(&buff, " at {");
        buff_write_s8(&buff, report.file);
        buff_write_s8(&buff, ", ");
        buff_write_u64(&buff, report.line);
        buff_write_s8(&buff, "}:\n");
        buff_write_s8(&buff, "\x1b[32m[SUCCESS]\x1b[0m     '");
        buff_write_s8(&buff, report.text);
        buff_write_s8(&buff, "'.\n");

        stderr_write_buff(&buff);
    }

    void
    report_debug(Report report)
    {
        if ( REPORT_LEVEL < REPORT_LEVEL_DEBUG ) return;

        buff_clear(&buff);
        buff_write_s8(&buff, "\x1b[34m[DEBUG]\x1b[0m from ");
        buff_write_s8(&buff, report.func);
        buff_write_s8(&buff, " at {");
        buff_write_s8(&buff, report.file);
        buff_write_s8(&buff, ", ");
        buff_write_u64(&buff, report.line);
        buff_write_s8(&buff, "}:\n");
        buff_write_s8(&buff, "\x1b[34m[DEBUG]\x1b[0m     '");
        buff_write_s8(&buff, report.text);
        buff_write_s8(&buff, "'.\n");

        stderr_write_buff(&buff);
    }

    void
    report_panic(Report report)
    {
        if ( REPORT_PANIC != REPORT_PANIC_TRUE ) return;

        buff_clear(&buff);
        buff_write_s8(&buff, "\x1b[35m[PANIC]\x1b[0m from ");
        buff_write_s8(&buff, report.func);
        buff_write_s8(&buff, " at {");
        buff_write_s8(&buff, report.file);
        buff_write_s8(&buff, ", ");
        buff_write_u64(&buff, report.line);
        buff_write_s8(&buff, "}:\n");
        buff_write_s8(&buff, "\x1b[35m[PANIC]\x1b[0m     '");
        buff_write_s8(&buff, report.text);
        buff_write_s8(&buff, "'.\n");

        stderr_write_buff(&buff);

        raise(SIGABRT);
    }

    void
    report_guard(s8 expr, Report report)
    {
        if ( REPORT_GUARD != REPORT_GUARD_TRUE ) return;

        buff_clear(&buff);
        buff_write_s8(&buff, "\x1b[35m[GUARD]\x1b[0m from ");
        buff_write_s8(&buff, report.func);
        buff_write_s8(&buff, " at {");
        buff_write_s8(&buff, report.file);
        buff_write_s8(&buff, ", ");
        buff_write_u64(&buff, report.line);
        buff_write_s8(&buff, "}:\n");
        buff_write_s8(&buff, "\x1b[35m[GUARD]\x1b[0m     guard '");
        buff_write_s8(&buff, expr);
        buff_write_s8(&buff, "' failed.\n");
        buff_write_s8(&buff, "\x1b[35m[GUARD]\x1b[0m\n");
        buff_write_s8(&buff, "\x1b[35m[GUARD]\x1b[0m '");
        buff_write_s8(&buff, report.text);
        buff_write_s8(&buff, "'.\n");

        stderr_write_buff(&buff);
    }
} // namespace pax
