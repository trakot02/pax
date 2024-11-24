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

    static const Array<Str8, READ_ERROR_COUNT> READ_ERROR_TITLE = {
        "READ_ERROR_NONE",
        "READ_ERROR_OVERFLOW",
        "READ_ERROR_SYSTEM",
    };

    struct Read_Value {
        isize      count;
        Read_Error error;
        isize      system;
    };

    struct Read {
        void* context;

        Read_Value (*func_buff) (
            void* ctxt, Buff* value
        );

        void (*func_close) (void* ctxt);
    };

    Read
    read_empty();

    Read_Value
    read_value_empty();

    Read_Value
    read_buff(Read self, Buff* value);

    void
    read_close(Read self);
} // namespace pax

#endif // PAX_CORE_READ_HPP
