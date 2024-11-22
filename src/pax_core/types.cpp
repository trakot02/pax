#include <pax_core/types.hpp>
#include <pax_core/report.hpp>
#include <pax_core/buff.hpp>

namespace pax
{
    //
    //
    // Exposed.
    //
    //

    Str8::Str8(const byte* string)
        : block {0}, count {0}
    {
       const byte* copy = string; 

        if ( string == 0 ) return;

        while ( *copy != 0 )
            copy += 1;

        block = string;
        count = copy - string;
    }

    Str8::Str8(const byte* string, isize count)
        : block {0}, count {0}
    {
        const byte* copy = string; 

        if ( string == 0 ) return;

        while ( *copy != 0 && copy - string <= count )
            copy += 1;

        block = string;
        count = copy - string;
    }

    const byte&
    Str8::operator[](isize index) const
    {
        pax_guard(0 <= index && index < count,
            "`index` is out of bounds");

        return block[index];
    }

    Str8
    str8_from(const Buff* buffer)
    {
        Str8 string = "";

        if ( buffer == 0 ) return string;

        string.block = buffer->head;
        string.count = buff_size(buffer);

        return string;
    }

    Str8
    str8_trim(Str8 string)
    {
        string = str8_trim_head(string);
        string = str8_trim_tail(string);

        return string;
    }

    Str8
    str8_trim_head(Str8 string)
    {
        const byte* head = string.block;
        const byte* tail = string.block + string.count;
        const byte* pntr = head;

        while ( pntr < tail ) {
            if ( (*pntr < 0x09 || *pntr > 0x0d) && *pntr != 0x20 )
                break;

            pntr += 1;
        }

        string.block = pntr;
        string.count = tail - pntr;

        return string;
    }

    Str8
    str8_trim_tail(Str8 string)
    {
        const byte* head = string.block;
        const byte* tail = string.block + string.count;
        const byte* pntr = tail;

        do {
            pntr -= 1;

            if ( (*pntr < 0x09 || *pntr > 0x0d) && *pntr != 0x20 )
                break;
        } while ( pntr >= head );

        string.block = head;
        string.count = pntr - head + 1;

        return string;
    }
} // namespace pax
