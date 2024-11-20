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

    struct Write_Value {
        isize       count;
        Write_Error error;
        isize       system;
    };

    struct Write {
        void* self = 0;

        Write_Value (*func_str8) (
            void* self, Str8 value
        ) = 0;

        Write_Value (*func_buff) (
            void* self, Buff* value
        ) = 0;

        void (*func_flush) (
            void* self
        ) = 0;

        void (*func_close) (
            void* self
        ) = 0;
    };

    extern const Array<Str8, WRITE_ERROR_COUNT> WRITE_ERROR_TITLE;

    Write_Value
    write_str8(Write write, Str8 value);

    Write_Value
    write_buff(Write write, Buff* value);

    void
    write_flush(Write write);

    void
    write_close(Write write);
} // namespace pax

#endif // PAX_CORE_WRITE_HPP
