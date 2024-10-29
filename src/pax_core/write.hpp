#ifndef PAX_CORE_WRITE_HPP
#define PAX_CORE_WRITE_HPP

#include <pax_core/types.hpp>
#include <pax_core/str8.hpp>
#include <pax_core/arr.hpp>

namespace pax
{
    struct Buff;

    struct Write_Res;
    struct Write;

    enum Write_Err : isize {
        write_err_none     = 0,
        write_err_overflow = 1,

        write_err_count = 2,
    };

    struct Write_Res {
        isize     bytes;
        Write_Err error;
    };

    struct Write {
        Write_Res (*byte_func)
            (void* self, byte value);

        Write_Res (*str8_func)
            (void* self, str8 value);

        Write_Res (*buff_func)
            (void* self, Buff* value);

        Write_Res (*addr_func)
            (void* self, void* value);

        Write_Res (*u64_func)
            (void* self, u64 value);

        Write_Res (*i64_func)
            (void* self, i64 value);

        void* self;
    };

    extern const Arr<str8, write_err_count> WRITE_ERR_TITLE;

    Write_Res
    write_byte(const Write* write, byte value);

    Write_Res
    write_str8(const Write* write, str8 value);

    Write_Res
    write_buff(const Write* write, Buff* value);

    Write_Res
    write_addr(const Write* write, void* value);

    Write_Res
    write_u64(const Write* write, u64 value);

    Write_Res
    write_i64(const Write* write, i64 value);
} // namespace pax

#endif // PAX_CORE_WRITE_HPP
