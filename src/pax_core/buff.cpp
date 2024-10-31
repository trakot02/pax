#include <pax_core/buff.hpp>
#include <pax_core/report.hpp>
#include <pax_core/slice.hpp>

namespace pax
{
    Write_Res
    _buff_write_byte(void* buffer, byte value, isize count);

    Write_Res
    _buff_write_buff(void* buffer, Buff* value);

    Write_Res
    _buff_write_s8(void* buffer, s8 value);

    Write_Res
    _buff_write_u64(void* buffer, u64 value, Write_Radix radix);

    Write_Res
    _buff_write_i64(void* buffer, i64 value, Write_Radix radix);

    Write_Res
    _buff_write_addr(void* buffer, void* value);

    //
    //
    // Implementation.
    //
    //

    static const auto DIGITS_DEC = Array<byte, 10> {
        '0', '1', '2', '3', '4',
        '5', '6', '7', '8', '9',
    };

    static const auto DIGITS_HEX = Array<byte, 16> {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
    };

    static const auto DIGITS_BIN = Array<byte, 2> {
        '0', '1',
    };

    static const auto DIGITS_OCT = Array<byte, 8> {
        '0', '1', '2', '3', '4', '5', '6', '7',
    };

    static const auto DIGITS_ARRAY = Array<Slice<const byte>, write_radix_count> {
        slice_from(DIGITS_DEC),
        slice_from(DIGITS_HEX),
        slice_from(DIGITS_BIN),
        slice_from(DIGITS_OCT),
    };

    static const auto PREFIX_ARRAY = Array<s8, write_radix_count> {
        "", "x0", "b0", "o0",
    };

    Buff
    buff_from(byte* addr, isize size, Buff_Full full)
    {
        pax_trace();
        pax_guard(size >= 0, "`size` is negative");

        pax_guard(0 <= full && full < buff_full_count,
            "`full` is not a valid option");

        auto self = Buff {0};

        if ( addr == 0 ) return self;

        self.head = addr;
        self.tail = addr + size;

        size *= (full == buff_full_true);

        self.curr = addr + size;

        return self;
    }

    isize
    buff_limit(const Buff* buffer)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self = *buffer;

        return self.tail - self.head;
    }

    isize
    buff_size(const Buff* buffer)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self = *buffer;

        return self.curr - self.head;
    }

    isize
    buff_avail(const Buff* buffer)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self = *buffer;

        return self.tail - self.curr;
    }

    Write_Res
    buff_write_byte(Buff* buffer, byte value, isize count)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");
        pax_guard(count  >= 0, "`count` is negative");

        auto& self  = *buffer;
        isize avail = self.tail - self.curr;

        if ( count > avail )
            return {0, write_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = value;

        self.curr += count;

        return {count, write_err_none};
    }

    Write_Res
    buff_write_buff(Buff* buffer, Buff* value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");
        pax_guard(value  != 0, "`value` is null");

        auto& self  = *buffer;
        auto& buff  = *value;
        isize avail = self.tail - self.curr;
        isize count = buff.curr - buff.head;

        if ( count > avail )
            return {0, write_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = buff.head[i];

        self.curr += count;
        buff.curr  = buff.head;

        return {count, write_err_none};
    }

    Write_Res
    buff_rwrite_buff(Buff* buffer, Buff* value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");
        pax_guard(value  != 0, "`value` is null");

        auto& self  = *buffer;
        auto& buff  = *value;
        isize avail = self.tail - self.curr;
        isize count = buff.curr - buff.head;

        if ( count > avail )
            return {0, write_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = buff.head[count - i - 1];

        self.curr += count;
        buff.curr  = buff.head;

        return {count, write_err_none};
    }

    Write_Res
    buff_write_s8(Buff* buffer, s8 value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *buffer;
        isize avail = self.tail - self.curr;
        isize count = value.size;

        if ( count > avail )
            return {0, write_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = value[i];

        self.curr += count;

        return {count, write_err_none};
    }

    Write_Res
    buff_rwrite_s8(Buff* buffer, s8 value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *buffer;
        isize avail = self.tail - self.curr;
        isize count = value.size;

        if ( count > avail )
            return {0, write_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = value[count - i - 1];

        self.curr += count;

        return {count, write_err_none};
    }

    Write_Res
    buff_write_u64(Buff* buffer, u64 value, Write_Radix radix)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        pax_guard(0 <= radix && radix < write_radix_count,
            "`radix` is not a valid option");

        auto& prefix = PREFIX_ARRAY[radix];
        auto& digits = DIGITS_ARRAY[radix];

        auto& self  = *buffer;
        isize avail = self.tail - self.curr;
        isize count = 0;
        isize index = 0;

        u64 other = value;

        do {
            if ( count >= avail )
                return {0, write_err_overflow};

            index = other % digits.size;
            other = other / digits.size;

            self.curr[count] = digits[index];

            count += 1;
        } while ( other != 0 );

        if ( count > avail - prefix.size )
            return {0, write_err_overflow};

        for ( isize i = 0; i < prefix.size; i += 1 )
            self.curr[i + count] = prefix[i];

        count += prefix.size;

        for ( isize i = 0; i < count / 2; i += 1 ) {
            byte  temp = self.curr[i];
            isize j    = count - i - 1;

            self.curr[i] = self.curr[j];
            self.curr[j] = temp;
        }

        self.curr += count;

        return {count, write_err_none};
    }

    Write_Res
    buff_write_i64(Buff* buffer, i64 value, Write_Radix radix)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        pax_guard(0 <= radix && radix < write_radix_count,
            "`radix` is not a valid option");

        auto& prefix = PREFIX_ARRAY[radix];
        auto& digits = DIGITS_ARRAY[radix];

        auto& self  = *buffer;
        isize avail = self.tail - self.curr - prefix.size;
        isize count = 0;
        isize index = 0;

        u64  other = value;
        byte sign  = '+';

        if ( value < 0 ) {
            other = ~other + 1;
            sign  = '-';
        }

        do {
            if ( count >= avail )
                return {0, write_err_overflow};

            index = other % digits.size;
            other = other / digits.size;

            self.curr[count] = digits[index];

            count += 1;
        } while ( other != 0 );

        if ( count > avail - prefix.size - (value != 0) )
            return {0, write_err_overflow};

        for ( isize i = 0; i < prefix.size; i += 1 )
            self.curr[i + count] = prefix[i];

        count += prefix.size;

        if ( value != 0 )
            self.curr[count] = sign;

        count += (value != 0);

        for ( isize i = 0; i < count / 2; i += 1 ) {
            byte  temp = self.curr[i];
            isize j    = count - i - 1;

            self.curr[i] = self.curr[j];
            self.curr[j] = temp;
        }

        self.curr += count;

        return {count, write_err_none};
    }

    Write_Res
    buff_write_addr(Buff* buffer, void* value)
    {
        return buff_write_u64(buffer, (usize) value, write_radix_hex);
    }

    void
    buff_empty(Buff* buffer)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self = *buffer;

        self.curr = self.head;
    }

    void
    buff_fill_byte(Buff* buffer, byte value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *buffer;
        isize avail = self.tail - self.curr;

        for ( isize i = 0; i < avail; i += 1 )
            self.curr[i] = value;

        self.curr = self.tail;
    }

    void
    buff_fill_addr(Buff* buffer, byte* addr)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *buffer;
        isize avail = self.tail - self.curr;

        if ( addr == 0 ) return;

        for ( isize i = 0; i < avail; i += 1 )
            self.curr[i] = addr[i];

        self.curr = self.tail;
    }

    Write
    buff_write(Buff* buffer)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto self = Write {0};

        self.byte_func = &_buff_write_byte;
        self.buff_func = &_buff_write_buff;
        self.s8_func   = &_buff_write_s8;
        self.u64_func  = &_buff_write_u64;
        self.i64_func  = &_buff_write_i64;
        self.addr_func = &_buff_write_addr;
        self.self      = buffer;

        return self;
    }

    //
    //
    // Not exposed.
    //
    //

    Write_Res
    _buff_write_byte(void* buffer, byte value, isize count)
    {
        return buff_write_byte((Buff*) buffer, value, count);
    }

    Write_Res
    _buff_write_buff(void* buffer, Buff* value)
    {
        return buff_write_buff((Buff*) buffer, value);
    }

    Write_Res
    _buff_write_s8(void* buffer, s8 value)
    {
        return buff_write_s8((Buff*) buffer, value);
    }

    Write_Res
    _buff_write_u64(void* buffer, u64 value, Write_Radix radix)
    {
        return buff_write_u64((Buff*) buffer, value, radix);
    }

    Write_Res
    _buff_write_i64(void* buffer, i64 value, Write_Radix radix)
    {
        return buff_write_i64((Buff*) buffer, value, radix);
    }

    Write_Res
    _buff_write_addr(void* buffer, void* value)
    {
        return buff_write_addr((Buff*) buffer, value);
    }
} // namespace pax
