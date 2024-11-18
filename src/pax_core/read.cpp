#include <pax_core/read.hpp>

namespace pax
{
    //
    //
    // Implementation.
    //
    //

    const Array<s8, READ_ERROR_COUNT> READ_ERROR_TITLE = {
        "READ_ERROR_NONE",
        "READ_ERROR_OVERFLOW",
        "READ_ERROR_SYSTEM",
    };

    Read_Value
    read_buff(const Read* read, Buff* value)
    {
        pax_trace();
        pax_guard(read != 0, "`read` is null");

        auto& self = *read;
        auto* func = self.buff_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, value);
    }

    void
    read_close(const Read* read)
    {
        pax_trace();
        pax_guard(read != 0, "`read` is null");

        auto& self = *read;
        auto* func = self.close_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self);
    }
} // namespace pax
