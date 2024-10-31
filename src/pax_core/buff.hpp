#ifndef PAX_CORE_BUFF_HPP
#define PAX_CORE_BUFF_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>
#include <pax_core/write.hpp>

namespace pax
{
    enum Buff_Full : isize {
        buff_full_false = 0,
        buff_full_true  = 1,

        buff_full_count = 2,
    };

    struct Buff {
        byte* head;
        byte* tail;
        byte* curr;
    };

    Buff
    buff_from(byte* addr, isize size, Buff_Full full = buff_full_false);

    isize
    buff_limit(const Buff* buffer);

    isize
    buff_size(const Buff* buffer);

    isize
    buff_avail(const Buff* buffer);

    Write_Res
    buff_write_byte(Buff* buffer, byte value, isize count = 1);

    Write_Res
    buff_write_buff(Buff* buffer, Buff* value);

    Write_Res
    buff_rwrite_buff(Buff* buffer, Buff* value);

    Write_Res
    buff_write_s8(Buff* buffer, s8 value);

    Write_Res
    buff_rwrite_s8(Buff* buffer, s8 value);

    Write_Res
    buff_write_u64(Buff* buffer, u64 value, Write_Radix radix = write_radix_dec);

    Write_Res
    buff_write_i64(Buff* buffer, i64 value, Write_Radix radix = write_radix_dec);

    Write_Res
    buff_write_addr(Buff* buffer, void* value);

    void
    buff_empty(Buff* buffer);

    void
    buff_fill_byte(Buff* buffer, byte value);

    void
    buff_fill_addr(Buff* buffer, byte* addr);

    Write
    buff_write(Buff* buffer);
} // namespace pax

#endif // PAX_CORE_BUFF_HPP
