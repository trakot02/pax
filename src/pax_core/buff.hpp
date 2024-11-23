#ifndef PAX_CORE_BUFF_HPP
#define PAX_CORE_BUFF_HPP

#include <pax_core/types.hpp>
#include <pax_core/arena.hpp>
#include <pax_core/write.hpp>
#include <pax_core/read.hpp>

namespace pax
{
    struct Buff {
        byte* block = 0;
        isize count = 0;
        byte* head  = 0;
        byte* tail  = 0;
    };

    Buff
    buff_init_ptr(byte* block = 0, isize count = 0);

    Arena_Error
    buff_init(Buff* buffer, Arena* arena, isize count);

    isize
    buff_size(const Buff* buffer);

    isize
    buff_avail(const Buff* buffer);

    void
    buff_shift(Buff* buffer);

    void
    buff_clear(Buff* buffer);

    void
    buff_fill(Buff* buffer, byte value = 0);

    void
    buff_fill_ptr(Buff* buffer, byte* value);

    void
    buff_trim(Buff* buffer);

    /*
    ???
    buff_split(Buff* buffer, ???, byte delim);
    */
    void
    buff_trim_head(Buff* buffer);

    void
    buff_trim_tail(Buff* buffer);

    Write
    buff_write(Buff* buffer);

    Read
    buff_read(Buff* buffer);

    Write_Value
    buff_write_str8(Buff* buffer, Str8 value);

    Write_Value
    buff_write_buff(Buff* buffer, Buff* value);

    Read_Value
    buff_read_buff(Buff* buffer, Buff* value);
} // namespace pax

#endif // PAX_CORE_BUFF_HPP
