#include <pax_core/buff.hpp>
#include <pax_core/report.hpp>
#include <pax_core/arena.hpp>
#include <pax_core/slice.hpp>

namespace pax
{
    Write_Res
    _buff_write_byte(void* buff, byte value);

    Write_Res
    _buff_write_s8(void* buff, s8 value);

    Write_Res
    _buff_write_buff(void* buff, Buff* value);

    void
    _buff_flush(void* buff);

    Read_Res
    _buff_read_byte(void* buff, byte* value);

    Read_Res
    _buff_read_buff(void* buff, Buff* value);

    void
    _buff_close(void* buff);

    //
    //
    // Implementation.
    //
    //

    static const Array<byte, 10> DIGITS_DEC = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    };

    static const Array<byte, 16> DIGITS_HEX = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
    };

    static const Array<byte, 2> DIGITS_BIN = {
        '0', '1',
    };

    static const Array<byte, 8> DIGITS_OCT = {
        '0', '1', '2', '3', '4', '5', '6', '7',
    };

    static const Array<i8, MAX_BYTE> VALUES_DEC = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
        -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1,
        -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1,
    };

    static const Array<i8, MAX_BYTE> VALUES_HEX = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1,
        -1, 10, 11, 12, 13, 14, 15, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1,
    };

    static const Array<i8, MAX_BYTE> VALUES_BIN = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         0,  1, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1,
        -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1,
        -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1,
    };

    static const Array<i8, MAX_BYTE> VALUES_OCT = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
         0,  1,  2,  3,  4,  5,  6,  7, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1,
        -1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
        -2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1,
    };

    static const Array<Slice<const byte>, BUFF_BASE_COUNT> DIGITS_ARRAY = {
        slice_from(DIGITS_DEC),
        slice_from(DIGITS_HEX),
        slice_from(DIGITS_BIN),
        slice_from(DIGITS_OCT),
    };

    static const Array<s8, BUFF_BASE_COUNT> PREFIX_ARRAY = {
        "", "0x", "0b", "0o",
    };

    static const Array<Slice<const i8>, BUFF_BASE_COUNT> VALUES_ARRAY = {
        slice_from(VALUES_DEC),
        slice_from(VALUES_HEX),
        slice_from(VALUES_BIN),
        slice_from(VALUES_OCT),
    };

    Buff
    buff_from_addr(byte* addr, isize size, Buff_Side side)
    {
        pax_guard(size >= 0, "`size` is negative");

        pax_guard(0 <= side && side < BUFF_SIDE_COUNT,
            "`side` is not a valid option");

        Buff self = {0};

        if ( addr == 0 || size == 0 )
            return self;

        self.head = addr;
        self.tail = addr + size;

        size *= (side == BUFF_SIDE_TAIL);

        self.curr = addr + size;

        return self;
    }

    Buff
    buff_init(Arena* arena, isize size, Buff_Side side)
    {
        pax_guard(size >= 0, "`size` is negative");

        pax_guard(0 <= side && side < BUFF_SIDE_COUNT,
            "`side` is not a valid option");

        Buff self = {0};

        auto res = arena_request(arena, {
            WIDTH_BYTE, ALIGN_BYTE, size
        });

        if ( res.addr == 0 ) return self;

        self.head = res.addr;
        self.tail = res.addr + size;

        size *= (side == BUFF_SIDE_TAIL);

        self.curr = res.addr + size;

        return self;
    }

    isize
    buff_limit(const Buff* buff)
    {
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize limit = self.tail - self.head;

        return limit;
    }

    isize
    buff_size(const Buff* buff)
    {
        pax_guard(buff != 0, "`buff` is null");

        auto& self = *buff;
        isize size = self.curr - self.head;

        return size;
    }

    isize
    buff_avail(const Buff* buff)
    {
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = self.tail - self.curr;

        return avail;
    }

    void
    buff_move(Buff* buff, Buff_Side side)
    {
        pax_guard(buff != 0, "`buff` is null");

        pax_guard(0 <= side && side <= BUFF_SIDE_COUNT,
            "`side` is not a valid option");

        auto& self = *buff;
        byte* addr = self.head;
        isize size = self.tail - self.head;

        size *= (side == BUFF_SIDE_TAIL);

        self.curr = addr + size;
    }

    void
    buff_clear(Buff* buff)
    {
        buff_clear_byte(buff, 0);
    }

    void
    buff_clear_byte(Buff* buff, byte value)
    {
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize limit = self.tail - self.head;

        for ( isize i = 0; i < limit; i += 1 )
            self.curr[i] = value;

        self.curr = self.head;
    }

    void
    buff_clear_addr(Buff* buff, byte* value)
    {
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize limit = self.tail - self.head;

        if ( value == 0 ) return;

        for ( isize i = 0; i < limit; i += 1 )
            self.curr[i] = value[i];

        self.curr = self.head;
    }

    void
    buff_fill(Buff* buff)
    {
        buff_fill_byte(buff, 0);
    }

    void
    buff_fill_byte(Buff* buff, byte value)
    {
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = self.tail - self.curr;

        for ( isize i = 0; i < avail; i += 1 )
            self.curr[i] = value;

        self.curr = self.tail;
    }

    void
    buff_fill_addr(Buff* buff, byte* value)
    {
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = self.tail - self.curr;

        if ( value == 0 ) return;

        for ( isize i = 0; i < avail; i += 1 )
            self.curr[i] = value[i];

        self.curr = self.tail;
    }

    Write
    buff_write(Buff* buff)
    {
        Write self;

        self.byte_func  = &_buff_write_byte;
        self.s8_func    = &_buff_write_s8;
        self.buff_func  = &_buff_write_buff;
        self.flush_func = &_buff_flush;
        self.close_func = &_buff_close;
        self.self       = buff;

        return self;
    }

    Read
    buff_read(Buff* buff)
    {
        Read self;

        self.byte_func  = &_buff_read_byte;
        self.buff_func  = &_buff_read_buff;
        self.close_func = &_buff_close;
        self.self       = buff;

        return self;
    }

    Write_Res
    buff_write_byte(Buff* buff, byte value)
    {
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize count = 1;
        isize avail = self.tail - self.curr;
        byte* addr  = self.curr;

        if ( count > avail )
            return {avail, WRITE_ERR_OVERFLOW};

        for ( isize i = 0; i < count; i += 1 )
            addr[i] = value;

        self.curr += count;

        return {count, WRITE_ERR_NONE};
    }

    Write_Res
    buff_write_s8(Buff* buff, s8 value)
    {
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize count = value.size;
        isize avail = self.tail - self.curr;
        byte* addr  = self.curr;

        if ( count > avail )
            return {avail, WRITE_ERR_OVERFLOW};

        for ( isize i = 0; i < count; i += 1 )
            addr[i] = value[i];

        self.curr += count;

        return {count, WRITE_ERR_NONE};
    }

    Write_Res
    buff_write_buff(Buff* buff, Buff* value)
    {
        pax_guard(buff  != 0, "`buff` is null");
        pax_guard(value != 0, "`value` is null");

        auto& self  = *buff;
        auto& other = *value;
        isize count = other.curr - other.head;
        isize avail = self.tail - self.curr;
        byte* addr  = self.curr;

        if ( count > avail )
            return {avail, WRITE_ERR_OVERFLOW};

        for ( isize i = 0; i < count; i += 1 )
            addr[i] = other.head[i];

        self.curr  += count;
        other.curr  = other.head;

        return {count, WRITE_ERR_NONE};
    }

    Write_Res
    buff_write_u64(Buff* buff, u64 value, Buff_Base base)
    {
        pax_guard(buff != 0, "`buff` is null");

        pax_guard(0 <= base && base < BUFF_BASE_COUNT,
            "`base` is not a valid option");

        auto& digits = DIGITS_ARRAY[base];
        auto& prefix = PREFIX_ARRAY[base];

        auto& self  = *buff;
        isize count = 0;
        isize avail = self.tail - self.curr;
        isize delta = prefix.size;
        byte* addr  = self.curr;

        isize idx = 0;
        u64   val = value;

        if ( delta >= avail )
            return {avail, WRITE_ERR_OVERFLOW};

        for ( isize i = 0; i < delta; i += 1 )
            addr[i] = prefix[i];

        count += delta;

        do {
            if ( count >= avail )
                return {avail, WRITE_ERR_OVERFLOW};

            idx = val % digits.size;
            val = val / digits.size;

            addr[count] = digits[idx];

            count += 1;
        } while ( val != 0 );

        isize i = delta;
        isize j = count - 1;

        for ( ; i < j; i += 1, j -= 1 ) {
            byte temp = addr[i];

            addr[i] = addr[j];
            addr[j] = temp;
        }

        self.curr += count;

        return {count, WRITE_ERR_NONE};
    }

    Write_Res
    buff_write_i64(Buff* buff, i64 value, Buff_Base base)
    {
        pax_guard(buff != 0, "`buff` is null");

        pax_guard(0 <= base && base < BUFF_BASE_COUNT,
            "`base` is not a valid option");

        auto& digits = DIGITS_ARRAY[base];
        auto& prefix = PREFIX_ARRAY[base];

        auto& self  = *buff;
        isize count = 0;
        isize avail = self.tail - self.curr;
        isize delta = prefix.size;
        byte* addr  = self.curr;

        isize idx = 0;
        u64   val = value;
        byte  sig = '+';

        if ( delta >= avail - (value != 0) )
            return {avail, WRITE_ERR_OVERFLOW};

        addr[count] = (value > 0) * '+' +
                      (value < 0) * '-';

        if ( value < 0 ) val = -value;

        count += (value != 0);

        for ( isize i = 0; i < delta; i += 1 )
            addr[count + i] = prefix[i];

        count += delta;

        do {
            if ( count >= avail )
                return {avail, WRITE_ERR_OVERFLOW};

            idx = val % digits.size;
            val = val / digits.size;

            addr[count] = digits[idx];

            count += 1;
        } while ( val != 0 );

        isize i = delta + (value != 0);
        isize j = count - 1;

        for ( ; i < j; i += 1, j -= 1 ) {
            byte temp = addr[i];

            addr[i] = addr[j];
            addr[j] = temp;
        }

        self.curr += count;

        return {count, WRITE_ERR_NONE};
    }

    Write_Res
    buff_write_addr(Buff* buff, void* value)
    {
        return buff_write_u64(buff, (u64) value, BUFF_BASE_HEX);
    }

    Read_Res
    buff_read_byte(Buff* buff, byte* value)
    {
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize count = 1;
        isize avail = self.tail - self.curr;
        byte* addr  = self.curr;

        if ( count > avail )
            return {avail, READ_ERR_OVERFLOW};

        value[0] = addr[0];

        self.curr += count;

        return {count, READ_ERR_NONE};
    }

    Read_Res
    buff_read_buff(Buff* buff, Buff* value)
    {
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        auto& other = *value;
        isize count = other.tail - other.curr;
        isize avail = self.tail - self.curr;
        byte* addr  = self.curr;

        if ( count >= avail ) count = avail;

        for ( isize i = 0; i < count; i += 1 )
            other.curr[i] = addr[i];

        self.curr  += count;
        other.curr += count;

        return {count, READ_ERR_NONE};
    }

    Read_Res
    buff_read_u64(Buff* buff, u64* value, Buff_Base base)
    {
        pax_guard(buff != 0, "`buff` is null");

        pax_guard(0 <= base && base < BUFF_BASE_COUNT,
            "`base` is not a valid option");

        auto& digits = DIGITS_ARRAY[base];
        auto& prefix = PREFIX_ARRAY[base];
        auto& values = VALUES_ARRAY[base];

        auto& self  = *buff;
        byte* addr  = self.curr;
        isize avail = self.tail - self.curr;
        isize count = 0;
        isize delta = 0;
        u64   other = 0;
        auto  error = READ_ERR_NONE;

        if ( count >= avail )
            return {avail, READ_ERR_SYNTAX};

        if ( addr[count] == '+' ) count += 1;

        if ( prefix.size >= avail - count )
            return {avail, READ_ERR_SYNTAX};

        for ( isize i = 0; i < prefix.size; i += 1 ) {
            if ( addr[i + count] != prefix[i] )
                return {count + i, READ_ERR_SYNTAX};
        }

        count += prefix.size;
        delta  = count;

        while ( count < avail ) {
            byte index = addr[count];
            i8   digit = MIN_BYTE;

            if ( index >= 0 ) digit = values[index];

            if ( digit < 0 ) {
                if ( digit == -2 || count == delta )
                    return {count + 1, READ_ERR_SYNTAX};

                break;
            }

            u64 limit = MAX_U64 - digit;

            if ( other <= limit / digits.size )
                other = other * digits.size + digit;
            else
                error = READ_ERR_OVERFLOW;

            count += 1;
        }

        if ( error != READ_ERR_NONE )
            return {count, error};

        if ( value != 0 ) *value = other;

        self.curr += count;

        return {count, READ_ERR_NONE};
    }

    Read_Res
    buff_read_i64(Buff* buff, i64* value, Buff_Base base)
    {
        pax_guard(buff != 0, "`buff` is null");

        pax_guard(0 <= base && base < BUFF_BASE_COUNT,
            "`base` is not a valid option");

        auto& digits = DIGITS_ARRAY[base];
        auto& prefix = PREFIX_ARRAY[base];
        auto& values = VALUES_ARRAY[base];

        auto& self  = *buff;
        byte* addr  = self.curr;
        isize avail = self.tail - self.curr;
        isize count = 0;
        isize delta = 0;
        u64   other = 0;
        u64   extra = 0;
        auto  error = READ_ERR_NONE;

        if ( count >= avail )
            return {avail, READ_ERR_SYNTAX};

        extra = (addr[count] == '-');

        if ( addr[count] == '-' || addr[count] == '+' )
            count += 1;

        if ( prefix.size >= avail - count )
            return {avail, READ_ERR_SYNTAX};

        for ( isize i = 0; i < prefix.size; i += 1 ) {
            if ( addr[i + count] != prefix[i] )
                return {count + i, READ_ERR_SYNTAX};
        }

        count += prefix.size;
        delta  = count;

        while ( count < avail ) {
            byte index = addr[count];
            i8   digit = MIN_BYTE;

            if ( index >= 0 ) digit = values[index];

            if ( digit < 0 ) {
                if ( digit == -2 || count == delta )
                    return {count + 1, READ_ERR_SYNTAX};

                break;
            }

            u64 limit = MAX_I64 + extra - digit;

            if ( other <= limit / digits.size )
                other = other * digits.size + digit;
            else
                error = READ_ERR_OVERFLOW;

            count += 1;
        }

        if ( error != READ_ERR_NONE )
            return {count, error};

        if ( value != 0 ) {
            *value = (extra == 0) * other -
                     (extra == 1) * other;
        }

        self.curr += count;

        return {count, READ_ERR_NONE};
    }

    Read_Res
    buff_read_addr(Buff* buff, void** value)
    {
        return buff_read_u64(buff, (u64*) value, BUFF_BASE_HEX);
    }

    //
    //
    // Not exposed.
    //
    //

    Write_Res
    _buff_write_byte(void* buff, byte value)
    {
        return buff_write_byte((Buff*) buff, value);
    }

    Write_Res
    _buff_write_s8(void* buff, s8 value)
    {
        return buff_write_s8((Buff*) buff, value);
    }

    Write_Res
    _buff_write_buff(void* buff, Buff* value)
    {
        return buff_write_buff((Buff*) buff, value);
    }

    void
    _buff_flush(void* buff)
    {
        pax_guard(buff != 0, "`buff` is null");
    }

    Read_Res
    _buff_read_byte(void* buff, byte* value)
    {
        return buff_read_byte((Buff*) buff, value);
    }

    Read_Res
    _buff_read_buff(void* buff, Buff* value)
    {
        return buff_read_buff((Buff*) buff, value);
    }

    void
    _buff_close(void* buff)
    {
        pax_guard(buff != 0, "`buff` is null");
    }
} // namespace pax
