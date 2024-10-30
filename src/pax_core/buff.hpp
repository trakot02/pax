#ifndef PAX_CORE_BUFF_HPP
#define PAX_CORE_BUFF_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>
#include <pax_core/write.hpp>

namespace pax
{
    enum Buff_Err : isize {
        buff_err_none     = 0,
        buff_err_overflow = 1,

        buff_err_count = 2,
    };

    enum Buff_Full : isize {
        buff_full_false = 0,
        buff_full_true  = 1,

        buff_full_count = 2,
    };

    struct Buff_Res {
        isize    bytes;
        Buff_Err error;
    };

    struct Buff {
        byte* head;
        byte* tail;
        byte* curr;
    };

    extern const Array<s8, buff_err_count> BUFF_ERR_TITLE;

    Buff
    buff_from(byte* addr, isize size, Buff_Full full = buff_full_false);

    isize
    buff_limit(const Buff* buffer);

    isize
    buff_count(const Buff* buffer);

    isize
    buff_avail(const Buff* buffer);

    Buff_Res
    buff_copy_byte(Buff* buffer, byte value, isize count = 1);

    Buff_Res
    buff_copy(Buff* buffer, Buff* value);

    Buff_Res
    buff_rcopy(Buff* buffer, Buff* value);

    Buff_Res
    buff_copy_s8(Buff* buffer, s8 value);

    Buff_Res
    buff_rcopy_s8(Buff* buffer, s8 value);

    Buff_Res
    buff_copy_u64(Buff* buffer, u64 value);

    Buff_Res
    buff_copy_i64(Buff* buffer, i64 value);

    Buff_Res
    buff_copy_addr(Buff* buffer, void* value);

    void
    buff_empty(Buff* buffer);

    void
    buff_fill(Buff* buffer, byte value);

    Write
    buff_write(Buff* buffer);
} // namespace pax

#endif // PAX_CORE_BUFF_HPP
