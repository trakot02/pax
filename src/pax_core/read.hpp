#ifndef PAX_CORE_READ_HPP
#define PAX_CORE_READ_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>

namespace pax
{
    enum Read_Error : isize {
        READ_ERROR_NONE     = 0,
        READ_ERROR_OVERFLOW = 1,
        READ_ERROR_SYSTEM   = 2,

        READ_ERROR_COUNT = 3,
    };

    struct Read_Value {
        isize      count;
        Read_Error error;
        isize      system;
    };

    struct Read {
        void* self = 0;

        Read_Value (*func_buff) (
            void* self, Buff* value
        ) = 0;

        void (*func_close) (
            void* self
        ) = 0;

    };

    extern const Array<Str8, READ_ERROR_COUNT> READ_ERROR_TITLE;

    Read_Value
    read_buff(Read read, Buff* value);

    void
    read_close(Read read);
} // namespace pax

#endif // PAX_CORE_READ_HPP
