#include <pax_core/write.hpp>
#include <pax_core/report.hpp>

namespace pax
{
    //
    //
    // Implementation.
    //
    //

    const Array<s8, _write_err_count> WRITE_ERR_TITLE = {
        "write_err_none",
        "write_err_overflow",
    };

    Write_Res
    write_byte(const Write* write, byte value, isize count)
    {
        pax_trace();
        pax_guard(write != 0, "`write` is null");

        auto& self = *write;
        auto* func = self.byte_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value, count);
    }

    Write_Res
    write_buff(const Write* write, Buff* value)
    {
        pax_trace();
        pax_guard(write != 0, "`write` is null");

        auto& self = *write;
        auto* func = self.buff_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value);
    }

    Write_Res
    write_s8(const Write* write, s8 value)
    {
        pax_trace();
        pax_guard(write != 0, "`write` is null");

        auto& self = *write;
        auto* func = self.s8_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value);
    }

    Write_Res
    write_u64(const Write* write, u64 value, Write_Radix radix)
    {
        pax_trace();
        pax_guard(write != 0, "`write` is null");

        auto& self = *write;
        auto* func = self.u64_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value, radix);
    }

    Write_Res
    write_i64(const Write* write, i64 value, Write_Radix radix)
    {
        pax_trace();
        pax_guard(write != 0, "`write` is null");

        auto& self = *write;
        auto* func = self.i64_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value, radix);
    }

    Write_Res
    write_addr(const Write* write, void* value)
    {
        pax_trace();
        pax_guard(write != 0, "`write` is null");

        auto& self = *write;
        auto* func = self.addr_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value);
    }
} // namespace pax
