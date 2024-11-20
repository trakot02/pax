#include <pax_core/read.hpp>

namespace pax
{
    //
    //
    // Exposed.
    //
    //

    const Array<Str8, READ_ERROR_COUNT> READ_ERROR_TITLE = {
        "READ_ERROR_NONE",
        "READ_ERROR_OVERFLOW",
        "READ_ERROR_SYSTEM",
    };

    Read_Value
    read_buff(Read read, Buff* value)
    {
        pax_guard(read.func_buff != 0,
            "`func_buff` is null");

        return (*read.func_buff)(read.self, value);
    }

    void
    read_close(Read read)
    {
        pax_guard(read.func_close != 0,
            "`func_close` is null");

        (*read.func_close)(read.self);
    }
} // namespace pax
