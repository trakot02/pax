#ifndef PAX_CORE_BUFF_HPP
#define PAX_CORE_BUFF_HPP

#include <pax_core/types.hpp>
#include <pax_core/write.hpp>
#include <pax_core/read.hpp>

namespace pax
{
    struct Arena;

    struct Buff {
        byte* addr = 0;
        isize size = 0;
        byte* head = 0;
        byte* tail = 0;
    };

    Buff
    buff_from(byte* addr, isize size);

    Buff
    buff_init(Arena* arena, isize size);

    isize
    buff_size(const Buff* buff);

    isize
    buff_avail(const Buff* buff);

    void
    buff_clear(Buff* buff);

    void
    buff_fill(Buff* buff, byte value = 0);

    void
    buff_fill_addr(Buff* buff, byte* value);

    void
    buff_shift(Buff* buff);

    void
    buff_trim(Buff* buff);

    void
    buff_trim_head(Buff* buff);

    void
    buff_trim_tail(Buff* buff);

    /*
    ???
    buff_split(Buff* buff, ???, byte delim);
    */

    Write
    buff_write(Buff* buff);

    Read
    buff_read(Buff* buff);

    Write_Value
    buff_write_s8(Buff* buff, s8 value);

    Write_Value
    buff_write_buff(Buff* buff, Buff* value);

    Read_Value
    buff_read_buff(Buff* buff, Buff* value);
} // namespace pax

#endif // PAX_CORE_BUFF_HPP
