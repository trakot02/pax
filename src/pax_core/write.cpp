#include <pax_core/write.hpp>

namespace pax
{
    //
    //
    // Exposed.
    //
    //

    Write
    write_empty()
    {
        Write write;

        write.context = 0;

        write.func_str8  = 0;
        write.func_buff  = 0;
        write.func_flush = 0;
        write.func_close = 0;

        return write;
    }

    Write_Value
    write_value_empty()
    {
        Write_Value value;

        value.count  = 0;
        value.error  = WRITE_ERROR_NONE;
        value.system = 0;

        return value;
    }

    Write_Value
    write_str8(Write alloc, Str8 value)
    {
        pax_guard(alloc.func_str8 != 0,
            "`alloc.func_str8` is null");

        return (*alloc.func_str8)(alloc.context, value);
    }

    Write_Value
    write_buff(Write alloc, Buff* value)
    {
        pax_guard(alloc.func_buff != 0,
            "`alloc.func_buff` is null");

        return (*alloc.func_buff)(alloc.context, value);
    }

    void
    write_flush(Write alloc)
    {
        pax_guard(alloc.func_flush != 0,
            "`alloc.func_flush` is null");

        return (*alloc.func_flush)(alloc.context);
    }

    void
    write_close(Write alloc)
    {
        pax_guard(alloc.func_close != 0,
            "`alloc.func_close` is null");

        return (*alloc.func_close)(alloc.context);
    }
} // namespace pax
