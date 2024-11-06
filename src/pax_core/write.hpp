#ifndef PAX_CORE_WRITE_HPP
#define PAX_CORE_WRITE_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>

namespace pax
{
    enum Write_Err : isize {
        WRITE_ERR_NONE     = 0,
        WRITE_ERR_OVERFLOW = 1,
        WRITE_ERR_SYSTEM   = 2,

        WRITE_ERR_COUNT = 3,
    };

    struct Write_Res {
        isize     bytes;
        Write_Err error;
        isize     system;
    };

    struct Write {
        Write_Res (*byte_func) (
            void* self, byte value
        ) = 0;

        Write_Res (*s8_func) (
            void* self, s8 value
        ) = 0;

        Write_Res (*buff_func) (
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

    extern const Array<s8, WRITE_ERR_COUNT> WRITE_ERR_TITLE;

    Write_Res
    write_byte(const Write* write, byte value);

    Write_Res
    write_s8(const Write* write, s8 value);

    Write_Res
    write_buff(const Write* write, Buff* value);

    void
    write_flush(const Write* write);

    void
    write_close(const Write* write);
} // namespace pax

#endif // PAX_CORE_WRITE_HPP
