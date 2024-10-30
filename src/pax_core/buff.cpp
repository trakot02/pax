#include <pax_core/buff.hpp>
#include <pax_core/report.hpp>

namespace pax
{
    Write_Res
    buff_write_byte(void* buffer, byte value);

    Write_Res
    buff_write_buff(void* buffer, Buff* value);

    Write_Res
    buff_write_s8(void* buffer, s8 value);

    Write_Res
    buff_write_u64(void* buffer, u64 value);

    Write_Res
    buff_write_i64(void* buffer, i64 value);

    Write_Res
    buff_write_addr(void* buffer, void* value);

    //
    //
    // Implementation.
    //
    //

    const Array<s8, buff_err_count> BUFF_ERR_TITLE = {
        "buff_err_none",
        "buff_err_overflow",
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
    buff_count(const Buff* buffer)
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

    Buff_Res
    buff_copy_byte(Buff* buffer, byte value, isize count)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");
        pax_guard(count  >= 0, "`count` is negative");

        auto& self  = *buffer;
        isize avail = self.tail - self.curr;

        if ( count > avail )
            return {0, buff_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = value;

        self.curr += count;

        return {count, buff_err_none};
    }

    Buff_Res
    buff_copy(Buff* buffer, Buff* value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");
        pax_guard(value  != 0, "`value` is null");

        auto& self  = *buffer;
        auto& buff  = *value;
        isize avail = self.tail - self.curr;
        isize count = buff.curr - buff.head;

        if ( count > avail )
            return {0, buff_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = buff.head[i];

        self.curr += count;
        buff.curr  = buff.head;

        return {count, buff_err_none};
    }

    Buff_Res
    buff_rcopy(Buff* buffer, Buff* value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");
        pax_guard(value  != 0, "`value` is null");

        auto& self  = *buffer;
        auto& buff  = *value;
        isize avail = self.tail - self.curr;
        isize count = buff.curr - buff.head;

        if ( count > avail )
            return {0, buff_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = buff.head[count - i - 1];

        self.curr += count;
        buff.curr  = buff.head;

        return {count, buff_err_none};
    }

    Buff_Res
    buff_copy_s8(Buff* buffer, s8 value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *buffer;
        isize avail = self.tail - self.curr;
        isize count = value.size;

        if ( count > avail )
            return {0, buff_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = value[i];

        self.curr += count;

        return {count, buff_err_none};
    }

    Buff_Res
    buff_rcopy_s8(Buff* buffer, s8 value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *buffer;
        isize avail = self.tail - self.curr;
        isize count = value.size;

        if ( count > avail )
            return {0, buff_err_overflow};

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = value[count - i - 1];

        self.curr += count;

        return {count, buff_err_none};
    }

    Buff_Res
    buff_copy_u64(Buff* buffer, u64 value)
    {
        static const auto radix = Array<byte, 10> {
            '0', '1', '2', '3', '4',
            '5', '6', '7', '8', '9',
        };

        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *buffer;
        isize avail = self.tail - self.curr;
        isize count = 0;

        auto  array = Array<byte, 64> {};
        u64   temp  = value;
        isize indx  = 0;

        do {
            if ( count > array.size || count > avail )
                return {0, buff_err_overflow};

            indx = temp % radix.size;
            temp = temp / radix.size;

            array[count] = radix[indx];

            count += 1;
        } while ( temp != 0 );

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = array[count - i - 1];

        self.curr += count;

        return {count, buff_err_none};
    }

    Buff_Res
    buff_copy_i64(Buff* buffer, i64 value)
    {
        static const auto radix = Array<byte, 10> {
            '0', '1', '2', '3', '4',
            '5', '6', '7', '8', '9',
        };

        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *buffer;
        isize avail = self.tail - self.curr;
        isize count = 0;

        auto  array = Array<byte, 64> {};
        u64   temp  = value;
        isize indx  = 0;
        byte  sign  = '+';

        avail -= (value != 0);

        if ( value < 0 ) {
            temp = ~temp + 1;
            sign = '-';
        }

        do {
            if ( count > array.size || count > avail )
                return {0, buff_err_overflow};

            indx = temp % radix.size;
            temp = temp / radix.size;

            array[count] = radix[indx];

            count += 1;
        } while ( temp != 0 );

        if ( value != 0 ) {
            self.curr[0]  = sign;
            self.curr    += 1;
        }

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = array[count - i - 1];

        self.curr += count;

        return {count, buff_err_none};
    }

    Buff_Res
    buff_copy_addr(Buff* buffer, void* value)
    {
        static const auto radix = Array<byte, 16> {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
        };

        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *buffer;
        isize avail = (self.tail - self.curr) - 2;
        isize count = 0;

        auto  array = Array<byte, 64> {};
        usize temp  = (usize) value;
        isize indx  = 0;

        do {
            if ( count > array.size || count > avail )
                return {0, buff_err_overflow};

            indx = temp % radix.size;
            temp = temp / radix.size;

            array[count] = radix[indx];

            count += 1;
        } while ( temp != 0 );

        self.curr[0]  = '0';
        self.curr[1]  = 'x';
        self.curr    += 2;

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = array[count - i - 1];

        self.curr += count;

        return {count, buff_err_none};
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
    buff_fill(Buff* buffer, byte value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *buffer;
        isize avail = self.tail - self.curr;

        for ( isize i = 0; i < avail; i += 1 )
            self.curr[i] = value;

        self.curr = self.tail;
    }

    Write
    buff_write(Buff* buffer)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto self = Write {0};

        self.byte_func = &buff_write_byte;
        self.buff_func = &buff_write_buff;
        self.s8_func   = &buff_write_s8;
        self.u64_func  = &buff_write_u64;
        self.i64_func  = &buff_write_i64;
        self.addr_func = &buff_write_addr;
        self.self      = buffer;

        return self;
    }

    //
    //
    // Not exposed.
    //
    //

    Write_Res
    buff_write_byte(void* buffer, byte value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *(Buff*) buffer;
        isize avail = self.tail - self.curr;
        isize count = 1;

        if ( count > avail )
            return {0, write_err_overflow};

        self.curr[0]  = value;
        self.curr    += count;

        return {count, write_err_none};
    }

    Write_Res
    buff_write_buff(void* buffer, Buff* value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");
        pax_guard(value  != 0, "`value` is null");

        auto& self  = *(Buff*) buffer;
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
    buff_write_s8(void* buffer, s8 value)
    {
        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *(Buff*) buffer;
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
    buff_write_u64(void* buffer, u64 value)
    {
        static const auto radix = Array<byte, 10> {
            '0', '1', '2', '3', '4',
            '5', '6', '7', '8', '9',
        };

        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *(Buff*) buffer;
        isize avail = self.tail - self.curr;
        isize count = 0;

        auto  array = Array<byte, 64> {};
        u64   temp  = value;
        isize indx  = 0;

        do {
            if ( count > array.size || count > avail )
                return {0, write_err_overflow};

            indx = temp % radix.size;
            temp = temp / radix.size;

            array[count] = radix[indx];

            count += 1;
        } while ( temp != 0 );

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = array[count - i - 1];

        self.curr += count;

        return {count, write_err_none};
    }

    Write_Res
    buff_write_i64(void* buffer, i64 value)
    {
        static const Array<byte, 10> radix = {
            '0', '1', '2', '3', '4',
            '5', '6', '7', '8', '9',
        };

        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *(Buff*) buffer;
        isize avail = self.tail - self.curr;
        isize count = 0;

        auto  array = Array<byte, 64> {};
        u64   temp  = value;
        isize indx  = 0;
        byte  sign  = '+';

        avail -= (value != 0);

        if ( value < 0 ) {
            temp = ~temp + 1;
            sign = '-';
        }

        do {
            if ( count > array.size || count > avail )
                return {0, write_err_overflow};

            indx = temp % radix.size;
            temp = temp / radix.size;

            array[count] = radix[indx];

            count += 1;
        } while ( temp != 0 );

        if ( value != 0 ) {
            self.curr[0]  = sign;
            self.curr    += 1;
        }

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = array[count - i - 1];

        self.curr += count;

        return {count, write_err_none};
    }

    Write_Res
    buff_write_addr(void* buffer, void* value)
    {
        static const Array<byte, 16> radix = {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
        };

        pax_trace();
        pax_guard(buffer != 0, "`buffer` is null");

        auto& self  = *(Buff*) buffer;
        isize avail = (self.tail - self.curr) - 2;
        isize count = 0;

        auto  array = Array<byte, 64> {};
        usize temp  = (usize) value;
        isize indx  = 0;

        do {
            if ( count > array.size || count > avail )
                return {0, write_err_overflow};

            indx = temp % radix.size;
            temp = temp / radix.size;

            array[count] = radix[indx];

            count += 1;
        } while ( temp != 0 );

        self.curr[0]  = '0';
        self.curr[1]  = 'x';
        self.curr    += 2;

        for ( isize i = 0; i < count; i += 1 )
            self.curr[i] = array[count - i - 1];

        self.curr += count;

        return {count, write_err_none};
    }
} // namespace pax
