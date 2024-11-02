#include <pax_core/read.hpp>
#include <pax_core/report.hpp>

namespace pax
{
    //
    //
    // Implementation.
    //
    //

    const Array<s8, _read_err_count> READ_ERR_TITLE = {
        "read_err_none",
        "read_err_overflow",
        "read_err_format",
    };

    Read_Res
    read_s8(const Read* read, s8* value, byte delim)
    {
        pax_trace();
        pax_guard(read != 0, "`read` is null");

        auto& self = *read;
        auto* func = self.s8_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value, delim);
    }

    Read_Res
    read_u64(const Read* read, u64* value, Read_Radix radix)
    {
        pax_trace();
        pax_guard(read != 0, "`read` is null");

        auto& self = *read;
        auto* func = self.u64_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value, radix);
    }
} // namespace pax
