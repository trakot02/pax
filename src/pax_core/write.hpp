#ifndef PAX_CORE_WRITE_HPP
#define PAX_CORE_WRITE_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>

namespace pax
{
    enum Write_Error : isize {
        WRITE_ERROR_NONE     = 0,
        WRITE_ERROR_OVERFLOW = 1,
        WRITE_ERROR_SYSTEM   = 2,

        WRITE_ERROR_COUNT = 3,
    };

    static const Array<Str8, WRITE_ERROR_COUNT> WRITE_ERROR_TITLE = {
        "WRITE_ERROR_NONE",
        "WRITE_ERROR_OVERFLOW",
        "WRITE_ERROR_SYSTEM",
    };

    struct Write_Value {
        isize       count;
        Write_Error error;
        isize       system;
    };

    struct Write {
        void* context;

        Write_Value (*func_str8) (
            void* ctxt, Str8 value
        );

        Write_Value (*func_buff) (
            void* ctxt, Buff* value
        );

        void (*func_flush) (void* ctxt);

        void (*func_close) (void* ctxt);
    };

    Write
    write_empty();

    Write_Value
    write_value_empty();

    Write_Value
    write_str8(Write self, Str8 value);

    Write_Value
    write_buff(Write self, Buff* value);

    void
    write_flush(Write self);

    void
    write_close(Write self);
} // namespace pax

#endif // PAX_CORE_WRITE_HPP
