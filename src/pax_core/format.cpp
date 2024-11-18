#include <pax_core/format.hpp>
#include <pax_core/array.hpp>
#include <pax_core/slice.hpp>
#include <pax_core/buff.hpp>

namespace pax
{
    Format_Value
    format_usize(Buff* buff, usize value, Slice<const byte> digits, s8 prefix);

    Format_Value
    format_isize(Buff* buff, isize value, Slice<const byte> digits);

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

    static const Array<Slice<const byte>, FORMAT_BASE_COUNT> DIGITS = {
        slice_from(DIGITS_DEC),
        slice_from(DIGITS_HEX),
        slice_from(DIGITS_BIN),
        slice_from(DIGITS_OCT),
    };

    static const Array<s8, FORMAT_BASE_COUNT> PREFIX = {
        "", "0x", "0b", "0o",
    };

    Format_Value
    format_byte(Buff* buff, byte value, isize times)
    {
        pax_trace();
        pax_guard(buff  != 0, "`buff` is null");
        pax_guard(times >= 0, "`times` is negative");

        auto& self  = *buff;
        isize avail = buff_avail(buff);

        if ( times > avail )
            return {avail, FORMAT_ERROR_OVERFLOW};

        for ( isize i = 0; i < times; i += 1 )
            self.tail[i] = value;

        self.tail += times;

        return {times, FORMAT_ERROR_NONE};
    }

    Format_Value
    format_s8(Buff* buff, s8 value, isize index, isize count, isize times)
    {
        pax_trace();
        pax_guard(buff  != 0, "`buff` is null");
        pax_guard(times >= 0, "`times` is negative");

        pax_guard(count <= value.size,
            "`count` is out of bounds");

        auto& self  = *buff;
        isize avail = buff_avail(buff);

        if ( count < 0 ) count = value.size;

        pax_guard(0 <= index && index < value.size - count,
            "`index` is out of bounds");

        isize total = count * times;

        if ( total == 0 ) return {0, FORMAT_ERROR_NONE};

        if ( total > avail )
            return {avail, FORMAT_ERROR_OVERFLOW};

        for ( isize i = 0; i < times; i += 1 ) {
            for ( isize b = 0; b < count; b += 1 )
                self.tail[count * i + b] = value.addr[b + index];
        }

        self.tail += total;

        return {total, FORMAT_ERROR_NONE};
    }

    Format_Value
    format_u64(Buff* buff, u64 value, Format_Base base)
    {
        pax_trace();
        pax_guard(0 <= base && base < FORMAT_BASE_COUNT,
            "`base` is not a valid option");

        auto& digits = DIGITS[base];
        auto& prefix = PREFIX[base];

        return format_usize(buff, value, digits, prefix);
    }

    Format_Value
    format_u32(Buff* buff, u32 value, Format_Base base)
    {
        pax_trace();
        pax_guard(0 <= base && base < FORMAT_BASE_COUNT,
            "`base` is not a valid option");

        auto& digits = DIGITS[base];
        auto& prefix = PREFIX[base];

        return format_usize(buff, value, digits, prefix);
    }

    Format_Value
    format_u16(Buff* buff, u16 value, Format_Base base)
    {
        pax_trace();
        pax_guard(0 <= base && base < FORMAT_BASE_COUNT,
            "`base` is not a valid option");

        auto& digits = DIGITS[base];
        auto& prefix = PREFIX[base];

        return format_usize(buff, value, digits, prefix);
    }

    Format_Value
    format_u8(Buff* buff, u8 value, Format_Base base)
    {
        pax_trace();
        pax_guard(0 <= base && base < FORMAT_BASE_COUNT,
            "`base` is not a valid option");

        auto& digits = DIGITS[base];
        auto& prefix = PREFIX[base];

        return format_usize(buff, value, digits, prefix);
    }

    Format_Value
    format_i64(Buff* buff, i64 value)
    {
        auto& digits = DIGITS[FORMAT_BASE_DEC];

        return format_isize(buff, value, digits);
    }

    Format_Value
    format_i32(Buff* buff, i32 value)
    {
        auto& digits = DIGITS[FORMAT_BASE_DEC];

        return format_isize(buff, value, digits);
    }

    Format_Value
    format_i16(Buff* buff, i16 value)
    {
        auto& digits = DIGITS[FORMAT_BASE_DEC];

        return format_isize(buff, value, digits);
    }

    Format_Value
    format_i8(Buff* buff, i8 value)
    {
        auto& digits = DIGITS[FORMAT_BASE_DEC];

        return format_isize(buff, value, digits);
    }

    Format_Value
    format_bool(Buff* buff, bool value)
    {
        if ( value == false )
            return format_s8(buff, "false");

        return format_s8(buff, "true");
    }

    //
    //
    // Not exposed.
    //
    //

    Format_Value
    format_usize(Buff* buff, usize value, Slice<const byte> digits, s8 prefix)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_avail(buff);
        isize count = 0;

        if ( prefix.size >= avail )
            return {avail, FORMAT_ERROR_OVERFLOW};

        for ( isize i = 0; i < prefix.size; i += 1 )
            self.tail[i] = prefix[i];

        count += prefix.size;

        do {
            isize index = 0;

            if ( count >= avail )
                return {avail, FORMAT_ERROR_OVERFLOW};

            index = value % digits.size;
            value = value / digits.size;

            self.tail[count] = digits[index];

            count += 1;
        } while ( value != 0 );

        isize i = prefix.size;
        isize j = count - 1;

        for ( ; i < j; i += 1, j -= i ) {
            byte temp = self.tail[i];

            self.tail[i] = self.tail[j];
            self.tail[j] = temp;
        }

        self.tail += count;

        return {count, FORMAT_ERROR_NONE};
    }

    Format_Value
    format_isize(Buff* buff, isize value, Slice<const byte> digits)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_avail(buff);
        isize count = 0;
        isize delta = (value != 0);

        u64 other = (value >= 0) * value -
                    (value  < 0) * value;

        if ( delta >= avail )
            return {avail, FORMAT_ERROR_OVERFLOW};

        self.tail[count] = (value > 0) * '+' +
                           (value < 0) * '-';

        count += delta;

        do {
            isize index = 0;

            if ( count >= avail )
                return {avail, FORMAT_ERROR_OVERFLOW};

            index = other % digits.size;
            other = other / digits.size;

            self.tail[count] = digits[index];

            count += 1;
        } while ( other != 0 );

        isize i = delta;
        isize j = count - 1;

        for ( ; i < j; i += 1, j -= i ) {
            byte temp = self.tail[i];

            self.tail[i] = self.tail[j];
            self.tail[j] = temp;
        }

        self.tail += count;

        return {count, FORMAT_ERROR_NONE};
    }
} // namespace pax
