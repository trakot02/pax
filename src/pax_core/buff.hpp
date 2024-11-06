#ifndef PAX_CORE_BUFF_HPP
#define PAX_CORE_BUFF_HPP

#include <pax_core/types.hpp>
#include <pax_core/write.hpp>
#include <pax_core/read.hpp>

namespace pax
{
    struct Arena;

    enum Buff_Side : isize {
        BUFF_SIDE_HEAD = 0,
        BUFF_SIDE_TAIL = 1,

        BUFF_SIDE_COUNT = 2,
    };

    enum Buff_Base : isize {
        BUFF_BASE_DEC = 0,
        BUFF_BASE_HEX = 1,
        BUFF_BASE_BIN = 2,
        BUFF_BASE_OCT = 3,

        BUFF_BASE_COUNT = 4,
    };

    struct Buff {
        byte* head = 0;
        byte* tail = 0;
        byte* curr = 0;
    };

    Buff
    buff_from_addr(byte* addr, isize size, Buff_Side side = BUFF_SIDE_HEAD);

    Buff
    buff_init(Arena* arena, isize size, Buff_Side side = BUFF_SIDE_HEAD);

    isize
    buff_limit(const Buff* buff);

    isize
    buff_size(const Buff* buff);

    isize
    buff_avail(const Buff* buff);

    void
    buff_move(Buff* buff, Buff_Side side);

    void
    buff_clear(Buff* buff);

    void
    buff_clear_byte(Buff* buff, byte value);

    void
    buff_clear_addr(Buff* buff, byte* value);

    void
    buff_fill(Buff* buff);

    void
    buff_fill_byte(Buff* buff, byte value);

    void
    buff_fill_addr(Buff* buff, byte* value);

    Write
    buff_write(Buff* buff);

    Read
    buff_read(Buff* buff);

    Write_Res
    buff_write_byte(Buff* buff, byte value);

    Write_Res
    buff_write_s8(Buff* buff, s8 value);

    Write_Res
    buff_write_buff(Buff* buff, Buff* value);

    Write_Res
    buff_write_u64(Buff* buff, u64 value, Buff_Base base = BUFF_BASE_DEC);

    Write_Res
    buff_write_i64(Buff* buff, i64 value, Buff_Base base = BUFF_BASE_DEC);

    Write_Res
    buff_write_addr(Buff* buff, void* value);

    Read_Res
    buff_read_byte(Buff* buff, byte* value);

    Read_Res
    buff_read_buff(Buff* buff, Buff* value);

    Read_Res
    buff_read_u64(Buff* buff, u64* value, Buff_Base base = BUFF_BASE_DEC);

    Read_Res
    buff_read_i64(Buff* buff, i64* value, Buff_Base base = BUFF_BASE_DEC);

    Read_Res
    buff_read_addr(Buff* buff, void** value);
} // namespace pax

#endif // PAX_CORE_BUFF_HPP
