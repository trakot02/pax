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

    const byte&
    s8::operator[](isize index) const
    {
        pax_guard(0 <= index && index < size,
            "`index` is out of bounds");

        return addr[index];
    }

    s8
    s8_from_buff(const Buff* buff)
    {
        s8    self = "";
        isize size = buff_size(buff);

        self.addr = buff->head;
        self.size = size;

        return self;
    }
} // namespace pax
