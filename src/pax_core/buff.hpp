#ifndef PAX_CORE_BUFF_HPP
#define PAX_CORE_BUFF_HPP

#include <pax_core/types.hpp>
#include <pax_core/write.hpp>
#include <pax_core/read.hpp>
#include <pax_core/mem/alloc.hpp>

namespace pax
{
    struct Buff {
        byte* block;
        isize count;
        byte* head;
        byte* tail;
    };

    Buff
    buff_empty();

    Buff
    buff_from(byte* block, isize count);

    Alloc_Error
    buff_init(Buff* self, Alloc alloc, isize count);

    isize
    buff_count(const Buff* self);

    isize
    buff_avail(const Buff* self);

    isize
    buff_size(const Buff* self);

    void
    buff_shift(Buff* self);

    void
    buff_clear(Buff* self);

    void
    buff_fill(Buff* self, byte value = 0);

    void
    buff_fill_ptr(Buff* self, byte* value);

    void
    buff_trim(Buff* self);

    void
    buff_trim_head(Buff* self);

    void
    buff_trim_tail(Buff* self);

    Write_Value
    buff_write_str8(Buff* self, Str8 value);

    Write_Value
    buff_write_buff(Buff* self, Buff* value);

    Write
    buff_write(Buff* self);

    Read_Value
    buff_read_buff(Buff* self, Buff* value);

    Read
    buff_read(Buff* self);
} // namespace pax

#endif // PAX_CORE_BUFF_HPP
