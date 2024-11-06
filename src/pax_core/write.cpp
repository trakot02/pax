#include <pax_core/write.hpp>

namespace pax
{
    //
    //
    // Implementation.
    //
    //

    const Array<s8, WRITE_ERR_COUNT> WRITE_ERR_TITLE = {
        "WRITE_ERR_NONE",
        "WRITE_ERR_OVERFLOW",
        "WRITE_ERR_SYSTEM",
    };

    Write_Res
    write_byte(const Write* write, byte value)
    {
        pax_guard(write != 0, "`write` is null");

        auto& self = *write;
        auto* func = self.byte_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value);
    }

    Write_Res
    write_s8(const Write* write, s8 value)
    {
        pax_guard(write != 0, "`write` is null");

        auto& self = *write;
        auto* func = self.s8_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value);
    }

    Write_Res
    write_buff(const Write* write, Buff* value)
    {
        pax_guard(write != 0, "`write` is null");

        auto& self = *write;
        auto* func = self.buff_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value);
    }

    void
    write_flush(const Write* write)
    {
        pax_guard(write != 0, "`write` is null");

        auto& self = *write;
        auto* func = self.flush_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self);
    }

    void
    write_close(const Write* write)
    {
        pax_guard(write != 0, "`write` is null");

        auto& self = *write;
        auto* func = self.close_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self);
    }
} // namespace pax
