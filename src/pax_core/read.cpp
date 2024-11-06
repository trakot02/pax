#include <pax_core/read.hpp>

namespace pax
{
    //
    //
    // Implementation.
    //
    //

    const Array<s8, READ_ERR_COUNT> READ_ERR_TITLE = {
        "READ_ERR_NONE",
        "READ_ERR_OVERFLOW",
        "READ_ERR_SYNTAX",
        "READ_ERR_SYSTEM",
    };

    Read_Res
    read_byte(const Read* read, byte* value)
    {
        pax_guard(read != 0, "`read` is null");

        auto& self = *read;
        auto* func = self.byte_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value);
    }

    Read_Res
    read_buff(const Read* read, Buff* value)
    {
        pax_guard(read != 0, "`read` is null");

        auto& self = *read;
        auto* func = self.buff_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value);
    }

    void
    read_close(const Read* read)
    {
        pax_guard(read != 0, "`read` is null");

        auto& self = *read;
        auto* func = self.close_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self);
    }
} // namespace paxs
