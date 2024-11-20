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

    Str8::Str8(const byte* str)
        : ptr {0}, cnt {0}
    {
        isize idx = 0;

        if ( str == 0 ) return;

        while ( str[idx] != 0 )
            idx += 1;

        ptr = str;
        cnt = idx;
    }

    Str8::Str8(const byte* str, isize cnt)
        : ptr {0}, cnt {0}
    {
        isize idx = 0;

        if ( str == 0 ) return;

        while ( str[idx] != 0 && idx < cnt )
            idx += 1;

        ptr = str;
        cnt = idx;
    }

    const byte&
    Str8::operator[](isize idx) const
    {
        pax_guard(0 <= idx && idx < cnt,
            "`idx` is out of bounds");

        return ptr[idx];
    }

    Str8
    str8_from(const Buff* buf)
    {
        Str8 str = "";

        if ( buf == 0 ) return str;

        str.ptr = buf->head;
        str.cnt = buff_size(buf);

        return str;
    }

    Str8
    str8_trim(Str8 str)
    {
        str = str8_trim_head(str);
        str = str8_trim_tail(str);

        return str;
    }

    Str8
    str8_trim_head(Str8 str)
    {
        auto* head = str.ptr;
        auto* tail = str.ptr + str.cnt;
        auto* ptr  = head;

        while ( ptr < tail ) {
            byte val = ptr[0];

            if ( val != '\t' && val != '\n' &&
                 val != '\v' && val != '\f' &&
                 val != '\r' && val != ' ' )
                break;

            ptr += 1;
        }

        str.ptr = ptr;
        str.cnt = tail - ptr;

        return str;
    }

    Str8
    str8_trim_tail(Str8 str)
    {
        auto* head = str.ptr;
        auto* tail = str.ptr + str.cnt;
        auto* ptr  = tail - 1;

        while ( ptr >= head ) {
            byte val = ptr[0];

            if ( val != '\t' && val != '\n' &&
                 val != '\v' && val != '\f' &&
                 val != '\r' && val != ' ' )
                break;

            ptr -= 1;
        }

        str.ptr = head;
        str.cnt = ptr - head + 1;

        return str;
    }
} // namespace pax
