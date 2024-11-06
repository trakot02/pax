#ifndef PAX_CORE_READ_HPP
#define PAX_CORE_READ_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>

namespace pax
{
    enum Read_Err : isize {
        READ_ERR_NONE     = 0,
        READ_ERR_OVERFLOW = 1,
        READ_ERR_SYNTAX   = 2,
        READ_ERR_SYSTEM   = 3,

        READ_ERR_COUNT = 4,
    };

    struct Read_Res {
        isize    bytes;
        Read_Err error;
        isize    system;
    };

    struct Read {
        Read_Res (*byte_func) (
            void* self, byte* value
        ) = 0;

        Read_Res (*buff_func) (
            void* self, Buff* value
        ) = 0;

        void (*close_func) (
            void* self
        ) = 0;

        void* self = 0;
    };

    extern const Array<s8, READ_ERR_COUNT> READ_ERR_TITLE;

    Read_Res
    read_byte(const Read* read, byte* value);

    Read_Res
    read_buff(const Read* read, Buff* value);

    void
    read_close(const Read* read);
} // namespace pax

#endif // PAX_CORE_READ_HPP
