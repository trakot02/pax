#ifndef PAX_CORE_BUFF_HPP
#define PAX_CORE_BUFF_HPP

#include <pax_core/types.hpp>
#include <pax_core/arena.hpp>
#include <pax_core/write.hpp>
#include <pax_core/read.hpp>

namespace pax
{
    struct Buff {
        byte* ptr  = 0;
        isize cnt  = 0;
        byte* head = 0;
        byte* tail = 0;
    };

    Buff
    buff_from(byte* ptr, isize cnt);

    Arena_Error
    buff_init(Buff* buf, Arena* arena, isize cnt);

    isize
    buff_size(const Buff* buf);

    isize
    buff_avail(const Buff* buf);

    void
    buff_shift(Buff* buf);

    void
    buff_clear(Buff* buf);

    void
    buff_fill(Buff* buf, byte value = 0);

    void
    buff_fill_ptr(Buff* buf, byte* value);

    void
    buff_trim(Buff* buf);

    /*
    ???
    buff_split(Buff* buf, ???, byte delim);
    */
    void
    buff_trim_head(Buff* buf);

    void
    buff_trim_tail(Buff* buf);

    Write
    buff_write(Buff* buf);

    Read
    buff_read(Buff* buf);

    Write_Value
    buff_write_str8(Buff* buf, Str8 value);

    Write_Value
    buff_write_buff(Buff* buf, Buff* value);

    Read_Value
    buff_read_buff(Buff* buf, Buff* value);
} // namespace pax

#endif // PAX_CORE_BUFF_HPP
