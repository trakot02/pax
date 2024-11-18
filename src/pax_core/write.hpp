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
        Write_Value (*s8_func) (
            void* self, s8 value
        ) = 0;

        Write_Value (*buff_func) (
            void* self, Buff* value
        ) = 0;

        void (*flush_func) (
            void* self
        ) = 0;

        void (*close_func) (
            void* self
        ) = 0;

        void* self = 0;
    };

    extern const Array<s8, WRITE_ERROR_COUNT> WRITE_ERROR_TITLE;

    Write_Value
    write_s8(const Write* write, s8 value);

    Write_Value
    write_buff(const Write* write, Buff* value);

    void
    write_flush(const Write* write);

    void
    write_close(const Write* write);
} // namespace pax

#endif // PAX_CORE_WRITE_HPP
