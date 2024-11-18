#include <pax_core/parse.hpp>
#include <pax_core/report.hpp>
#include <pax_core/buff.hpp>

namespace pax
{
    Parse_Value
    parse_s8(Buff* buff, s8* value, byte delim)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = buff_size(buff);
        isize count = -1;

        for ( isize i = 0; i < avail; i += 1 ) {
            if ( self.head[i] == delim )
                count = i, i = avail;
        }

        if ( count < 0 )
            return {avail, PARSE_ERROR_OVERFLOW};

        if ( value != 0 ) {
            value->addr = self.head;
            value->size = count;
        }

        self.head += count + 1;

        return {count + 1, PARSE_ERROR_NONE};
    }

/*
    Read_Value
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
        auto  error = READ_ERROR_NONE;

        if ( count >= avail )
            return {avail, READ_ERROR_SYNTAX};

        if ( addr[count] == '+' ) count += 1;

        if ( prefix.size >= avail - count )
            return {avail, READ_ERROR_SYNTAX};

        for ( isize i = 0; i < prefix.size; i += 1 ) {
            if ( addr[i + count] != prefix[i] )
                return {count + i, READ_ERROR_SYNTAX};
        }

        count += prefix.size;
        delta  = count;

        while ( count < avail ) {
            byte index = addr[count];
            i8   digit = MIN_BYTE;

            if ( index >= 0 ) digit = values[index];

            if ( digit < 0 ) {
                if ( digit == -2 || count == delta )
                    return {count + 1, READ_ERROR_SYNTAX};

                break;
            }

            u64 limit = MAX_U64 - digit;

            if ( other <= limit / digits.size )
                other = other * digits.size + digit;
            else
                error = READ_ERROR_OVERFLOW;

            count += 1;
        }

        if ( error != READ_ERROR_NONE )
            return {count, error};

        if ( value != 0 ) *value = other;

        self.curr += count;

        return {count, READ_ERROR_NONE};
    }

    Read_Value
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
        auto  error = READ_ERROR_NONE;

        if ( count >= avail )
            return {avail, READ_ERROR_SYNTAX};

        extra = (addr[count] == '-');

        if ( addr[count] == '-' || addr[count] == '+' )
            count += 1;

        if ( prefix.size >= avail - count )
            return {avail, READ_ERROR_SYNTAX};

        for ( isize i = 0; i < prefix.size; i += 1 ) {
            if ( addr[i + count] != prefix[i] )
                return {count + i, READ_ERROR_SYNTAX};
        }

        count += prefix.size;
        delta  = count;

        while ( count < avail ) {
            byte index = addr[count];
            i8   digit = MIN_BYTE;

            if ( index >= 0 ) digit = values[index];

            if ( digit < 0 ) {
                if ( digit == -2 || count == delta )
                    return {count + 1, READ_ERROR_SYNTAX};

                break;
            }

            u64 limit = MAX_I64 + extra - digit;

            if ( other <= limit / digits.size )
                other = other * digits.size + digit;
            else
                error = READ_ERROR_OVERFLOW;

            count += 1;
        }

        if ( error != READ_ERROR_NONE )
            return {count, error};

        if ( value != 0 ) {
            *value = (extra == 0) * other -
                     (extra == 1) * other;
        }

        self.curr += count;

        return {count, READ_ERROR_NONE};
    }

    Read_Value
    buff_read_addr(Buff* buff, void** value)
    {
        return buff_read_u64(buff, (u64*) value, BUFF_BASE_HEX);
    }
*/
} // namespace pax
