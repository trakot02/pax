#include <pax_core/write.hpp>

namespace pax
{
    //
    //
    // Exposed.
    //
    //

    const Array<Str8, WRITE_ERROR_COUNT> WRITE_ERROR_TITLE = {
        "WRITE_ERROR_NONE",
        "WRITE_ERROR_OVERFLOW",
        "WRITE_ERROR_SYSTEM",
    };

    Write_Value
    write_str8(Write write, Str8 value)
    {
        pax_guard(write.func_str8 != 0,
            "`func_str8` is null");

        return (*write.func_str8)(write.self, value);
    }

    Write_Value
    write_buff(Write write, Buff* value)
    {
        pax_guard(write.func_buff != 0,
            "`func_buff` is null");

        return (*write.func_buff)(write.self, value);
    }

    void
    write_flush(Write write)
    {
        pax_guard(write.func_flush != 0,
            "`func_flush` is null");

        (*write.func_flush)(write.self);
    }

    void
    write_close(Write write)
    {
        pax_guard(write.func_close != 0,
            "`func_close` is null");

        (*write.func_close)(write.self);
    }
} // namespace pax
