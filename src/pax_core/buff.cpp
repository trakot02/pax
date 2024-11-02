#include <pax_core/buff.hpp>
#include <pax_core/report.hpp>
#include <pax_core/slice.hpp>

namespace pax
{
    Write_Res
    ifc_buff_write_byte(void* buff, byte value, isize count);

    Write_Res
    ifc_buff_write_buff(void* buff, Buff* value);

    Write_Res
    ifc_buff_write_s8(void* buff, s8 value);

    Write_Res
    ifc_buff_write_u64(void* buff, u64 value, Write_Radix radix);

    Write_Res
    ifc_buff_write_i64(void* buff, i64 value, Write_Radix radix);

    Write_Res
    ifc_buff_write_addr(void* buff, void* value);

    Read_Res
    ifc_buff_read_s8(void* buff, s8* value, byte delim);

    Read_Res
    ifc_buff_read_u64(void* buff, u64* value, Read_Radix radix);

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

    static const Array<Slice<const byte>, _write_radix_count> DIGITS_ARRAY = {
        slice_from(DIGITS_DEC),
        slice_from(DIGITS_HEX),
        slice_from(DIGITS_BIN),
        slice_from(DIGITS_OCT),
    };

    static const Array<s8, _write_radix_count> PREFIX_ARRAY = {
        "", "0x", "0b", "0o",
    };

    static const Array<Slice<const i8>, _write_radix_count> VALUES_ARRAY = {
        slice_from(VALUES_DEC),
        slice_from(VALUES_HEX),
        slice_from(VALUES_BIN),
        slice_from(VALUES_OCT),
    };

    Buff
    buff_from(byte* addr, isize size, Buff_End end)
    {
        pax_trace();
        pax_guard(size >= 0, "`size` is negative");

        pax_guard(0 <= end && end < _buff_end_count,
            "`end` is not a valid option");

        Buff self = {0};

        if ( addr == 0 || size == 0 )
            return self;

        self.head = addr;
        self.tail = addr + size;

        size *= (end == _buff_end_tail);

        self.curr = addr + size;

        return self;
    }

    isize
    buff_limit(const Buff* buff)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self = *buff;

        return self.tail - self.head;
    }

    isize
    buff_size(const Buff* buff)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self = *buff;

        return self.curr - self.head;
    }

    isize
    buff_avail(const Buff* buff)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self = *buff;

        return self.tail - self.curr;
    }

    Write_Res
    buff_write_byte(Buff* buff, byte value, isize count)
    {
        pax_trace();
        pax_guard(buff  != 0, "`buff` is null");
        pax_guard(count >= 0, "`count` is negative");

        auto& self  = *buff;
        byte* addr  = self.curr;
        isize avail = self.tail - self.curr;

        if ( count > avail )
            return {0, _write_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            addr[i] = value;

        self.curr += count;

        return {count, _write_err_none};
    }

    Write_Res
    buff_write_buff(Buff* buff, Buff* value)
    {
        pax_trace();
        pax_guard(buff  != 0, "`buff` is null");
        pax_guard(value != 0, "`value` is null");

        auto& self  = *buff;
        auto& other = *value;
        byte* addr  = self.curr;
        isize avail = self.tail - self.curr;
        isize count = other.curr - other.head;

        if ( count > avail )
            return {0, _write_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            addr[i] = other.head[i];

        self.curr  += count;
        other.curr  = other.head;

        return {count, _write_err_none};
    }

    Write_Res
    buff_rwrite_buff(Buff* buff, Buff* value)
    {
        pax_trace();
        pax_guard(buff  != 0, "`buff` is null");
        pax_guard(value != 0, "`value` is null");

        auto& self  = *buff;
        auto& other = *value;
        byte* addr  = self.curr;
        isize avail = self.tail - self.curr;
        isize count = other.curr - other.head;

        if ( count > avail )
            return {0, _write_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            addr[i] = other.head[count - i - 1];

        self.curr  += count;
        other.curr  = other.head;

        return {count, _write_err_none};
    }

    Write_Res
    buff_write_s8(Buff* buff, s8 value)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        byte* addr  = self.curr;
        isize avail = self.tail - self.curr;
        isize count = value.size;

        if ( count > avail )
            return {0, _write_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            addr[i] = value[i];

        self.curr += count;

        return {count, _write_err_none};
    }

    Write_Res
    buff_rwrite_s8(Buff* buff, s8 value)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        byte* addr  = self.curr;
        isize avail = self.tail - self.curr;
        isize count = value.size;

        if ( count > avail )
            return {0, _write_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            addr[i] = value[count - i - 1];

        self.curr += count;

        return {count, _write_err_none};
    }

    Write_Res
    buff_write_u64(Buff* buff, u64 value, Write_Radix radix)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        pax_guard(0 <= radix && radix < _write_radix_count,
            "`radix` is not a valid option");

        auto& digits = DIGITS_ARRAY[radix];
        auto& prefix = PREFIX_ARRAY[radix];

        auto& self  = *buff;
        byte* addr  = self.curr;
        isize avail = self.tail - self.curr;
        isize count = 0;
        isize index = 0;
        u64   other  = value;

        do {
            if ( count >= avail )
                return {0, _write_err_overflow};

            index = other % digits.size;
            other = other / digits.size;

            addr[count] = digits[index];

            count += 1;
        } while ( other != 0 );

        if ( count > avail - prefix.size )
            return {0, _write_err_overflow};

        count += prefix.size;

        for ( isize i = 0; i < prefix.size; i += 1 )
            addr[i] = prefix[prefix.size - i - 1];

        isize i = 0;
        isize j = count - 1;

        for ( ; i < j; i += 1, j -= 1 ) {
            byte temp = addr[i];

            addr[i] = addr[j];
            addr[j] = temp;
        }

        self.curr += count;

        return {count, _write_err_none};
    }

    Write_Res
    buff_write_i64(Buff* buff, i64 value, Write_Radix radix)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        pax_guard(0 <= radix && radix < _write_radix_count,
            "`radix` is not a valid option");

        auto& digits = DIGITS_ARRAY[radix];
        auto& prefix = PREFIX_ARRAY[radix];

        auto& self  = *buff;
        byte* addr  = self.curr;
        isize avail = self.tail - self.curr;
        isize count = 0;
        isize index = 0;
        u64   other = value;
        byte  sign  = '+';

        if ( value < 0 ) {
            other = -value;
            sign  = '-';
        }

        do {
            if ( count >= avail )
                return {0, _write_err_overflow};

            index = other % digits.size;
            other = other / digits.size;

            addr[count] = digits[index];

            count += 1;
        } while ( other != 0 );

        if ( count > avail - prefix.size - (value != 0) )
            return {0, _write_err_overflow};

        count += prefix.size;

        for ( isize i = 0; i < prefix.size; i += 1 )
            addr[i] = prefix[prefix.size - i - 1];

        if ( value != 0 ) {
            addr[count]  = sign;
            count       += 1;
        }

        isize i = 0;
        isize j = count - 1;

        for ( ; i < j; i += 1, j -= 1 ) {
            byte temp = addr[i];

            addr[i] = addr[j];
            addr[j] = temp;
        }

        self.curr += count;

        return {count, _write_err_none};
    }

    Write_Res
    buff_write_addr(Buff* buff, void* value)
    {
        return buff_write_u64(buff, (usize) value, _write_radix_hex);
    }

    Read_Res
    buff_read_s8(Buff* buff, s8* value, byte delim)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        byte* addr  = self.curr;
        isize avail = self.tail - self.curr;
        isize count = 0;

        while ( count < avail ) {
            if ( addr[count] == delim )
                break;

            count += 1;
        }

        if ( value != 0 ) {
            value->addr = addr;
            value->size = count;
        }

        if ( addr[count] == delim )
            count += 1;

        self.curr += count;

        return {count, _read_err_none};
    }

    Read_Res
    buff_read_u64(Buff* buff, u64* value, Read_Radix radix)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        pax_guard(0 <= radix && radix < _read_radix_count,
            "`radix` is not a valid option");

        auto& digits = DIGITS_ARRAY[radix];
        auto& prefix = PREFIX_ARRAY[radix];
        auto& values = VALUES_ARRAY[radix];

        auto& self  = *buff;
        byte* addr  = self.curr;
        isize avail = self.tail - self.curr;
        isize count = 0;
        u64   other = 0;
        auto  error = _read_err_none;

        if ( avail != 0 && self.curr[count] == '+' )
            count += 1;

        for ( isize i = 0; i < prefix.size; i += 1 ) {
            if ( count + i > avail )
                return {avail, _read_err_format};

            if ( addr[i + count] != prefix[i] )
                return {count + i, _read_err_format};
        }

        count += prefix.size;

        while ( count < avail ) {
            byte index = self.curr[count];
            i8   digit = -1;

            if ( index >=  0 ) digit = values[index];
            if ( digit == -1 ) break;

            if ( digit == -2 )
                return {count, _read_err_format};

            if ( other <= (MAX_U64 - digit) / digits.size )
                other = other * digits.size + digit;
            else
                error = _read_err_overflow;

            count += 1;
        }

        if ( error != _read_err_none )
            return {count, error};

        if ( value != 0 ) *value = other;

        self.curr += count;

        return {count, _read_err_none};
    }

    void
    buff_move(Buff* buff, Buff_End end)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        pax_guard(0 <= end && end <= _buff_end_count,
            "`end` is not a valid option");

        auto& self = *buff;
        byte* addr = self.head;
        isize size = self.tail - self.head;

        size *= (end == _buff_end_tail);

        self.curr = addr + size;
    }

    void
    buff_empty(Buff* buff)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self = *buff;

        self.curr = self.head;
    }

    void
    buff_fill_byte(Buff* buff, byte value)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = self.tail - self.curr;

        for ( isize i = 0; i < avail; i += 1 )
            self.curr[i] = value;

        self.curr = self.tail;
    }

    void
    buff_fill_addr(Buff* buff, byte* addr)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = self.tail - self.curr;

        if ( addr == 0 ) return;

        for ( isize i = 0; i < avail; i += 1 )
            self.curr[i] = addr[i];

        self.curr = self.tail;
    }

    Write
    buff_write(Buff* buff)
    {
        pax_trace();

        Write self = {0};

        self.byte_func = &ifc_buff_write_byte;
        self.buff_func = &ifc_buff_write_buff;
        self.s8_func   = &ifc_buff_write_s8;
        self.u64_func  = &ifc_buff_write_u64;
        self.i64_func  = &ifc_buff_write_i64;
        self.addr_func = &ifc_buff_write_addr;
        self.self      = buff;

        return self;
    }

    Read
    buff_read(Buff* buff)
    {
        pax_trace();

        Read self = {0};

        self.s8_func  = &ifc_buff_read_s8;
        self.u64_func = &ifc_buff_read_u64;
        self.self     = buff;

        return self;
    }

    //
    //
    // Not exposed.
    //
    //

    Write_Res
    ifc_buff_write_byte(void* buff, byte value, isize count)
    {
        return buff_write_byte((Buff*) buff, value, count);
    }

    Write_Res
    ifc_buff_write_buff(void* buff, Buff* value)
    {
        return buff_write_buff((Buff*) buff, value);
    }

    Write_Res
    ifc_buff_write_s8(void* buff, s8 value)
    {
        return buff_write_s8((Buff*) buff, value);
    }

    Write_Res
    ifc_buff_write_u64(void* buff, u64 value, Write_Radix radix)
    {
        return buff_write_u64((Buff*) buff, value, radix);
    }

    Write_Res
    ifc_buff_write_i64(void* buff, i64 value, Write_Radix radix)
    {
        return buff_write_i64((Buff*) buff, value, radix);
    }

    Write_Res
    ifc_buff_write_addr(void* buff, void* value)
    {
        return buff_write_addr((Buff*) buff, value);
    }

    Read_Res
    ifc_buff_read_s8(void* buff, s8* value, byte delim)
    {
        return buff_read_s8((Buff*) buff, value, delim);
    }

    Read_Res
    ifc_buff_read_u64(void* buff, u64* value, Read_Radix radix)
    {
        return buff_read_u64((Buff*) buff, value, radix);
    }
} // namespace pax
