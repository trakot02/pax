#include <pax_core/types.hpp>
#include <pax_core/report.hpp>
#include <pax_core/buff.hpp>

namespace pax
{
    //
    //
    // Implementation.
    //
    //

    s8::s8(const byte* string)
        : addr {0}
        , size {0}
    {
        isize count = 0;

        if ( string == 0 ) return;

        while ( string[count] != 0 )
            count += 1;

        addr = string;
        size = count;
    }

    s8::s8(const byte* string, isize size)
        : addr {0}
        , size {0}
    {
        isize count = 0;

        if ( string == 0 ) return;

        while ( string[count] != 0 && count < size )
            count += 1;

        addr = string;
        size = count;
    }

    const byte&
    s8::operator[](isize index) const
    {
        pax_trace();
        pax_guard(0 <= index && index < size,
            "`index` is out of bounds");

        return addr[index];
    }

    s8
    s8_from(const Buff* buff)
    {
        s8    self = "";
        isize size = buff_size(buff);

        self.addr = buff->head;
        self.size = size;

        return self;
    }

    s8
    s8_trim(s8 string)
    {
        return s8_trim_tail(s8_trim_head(string));
    }

    s8
    s8_trim_head(s8 string)
    {
        auto* head = string.addr;
        auto* tail = string.addr + string.size;
        auto* addr = string.addr;

        while ( addr < tail ) {
            byte temp = addr[0];

            if ( temp != '\t' && temp != '\n' &&
                 temp != '\v' && temp != '\f' &&
                 temp != '\r' && temp != ' ' )
                break;

            addr += 1;
        }

        string.size -= addr - head;
        string.addr  = addr;

        return string;
    }

    s8
    s8_trim_tail(s8 string)
    {
        auto* head = string.addr;
        auto* tail = string.addr + string.size;
        auto* addr = tail - 1;

        while ( addr >= head ) {
            byte temp = addr[0];

            if ( temp != '\t' && temp != '\n' &&
                 temp != '\v' && temp != '\f' &&
                 temp != '\r' && temp != ' ' )
                break;

            addr -= 1;
        }

        string.size -= tail - (addr + 1);

        return string;
    }
} // namespace pax
