#ifndef PAX_CORE_BUFF_HPP
#define PAX_CORE_BUFF_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>
#include <pax_core/write.hpp>
#include <pax_core/read.hpp>

namespace pax
{
    enum Buff_End : isize {
        _buff_end_head = 0,
        _buff_end_tail  = 1,

        _buff_end_count = 2,
    };

    struct Buff {
        byte* head;
        byte* tail;
        byte* curr;
    };

    Buff
    buff_from(byte* addr, isize size, Buff_End end = _buff_end_head);

    isize
    buff_limit(const Buff* buff);

    isize
    buff_size(const Buff* buff);

    isize
    buff_avail(const Buff* buff);

    Write_Res
    buff_write_byte(Buff* buff, byte value, isize count = 1);

    Write_Res
    buff_write_buff(Buff* buff, Buff* value);

    Write_Res
    buff_write_s8(Buff* buff, s8 value);

    Write_Res
    buff_write_u64(Buff* buff, u64 value, Write_Radix radix = _write_radix_dec);

    Write_Res
    buff_write_i64(Buff* buff, i64 value, Write_Radix radix = _write_radix_dec);

    Write_Res
    buff_write_addr(Buff* buff, void* value);

    Read_Res
    buff_read_buff(Buff* buff, Buff* value, byte delim);

    Read_Res
    buff_read_s8(Buff* buff, s8* value, byte delim);

    Read_Res
    buff_read_u64(Buff* buff, u64* value, Read_Radix radix = _read_radix_dec);

    Read_Res
    buff_read_i64(Buff* buff, i64* value, Read_Radix radix = _read_radix_dec);

    void
    buff_move(Buff* buff, Buff_End end);

    void
    buff_empty(Buff* buff);

    void
    buff_fill_byte(Buff* buff, byte value);

    void
    buff_fill_addr(Buff* buff, byte* addr);

    Write
    buff_write(Buff* buff);

    Read
    buff_read(Buff* buff);
} // namespace pax

#endif // PAX_CORE_BUFF_HPP
