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

    s8
    s8_from(const Buff* buff)
    {
        pax_trace();

        s8    self = "";
        isize size = buff_size(buff);

        self.addr = buff->head;
        self.size = size;

        return self;
    }

    const byte&
    s8::operator[](isize index) const
    {
        pax_trace();
        pax_guard(0 <= index && index < size,
            "`index` is out of bounds");

        return addr[index];
    }
} // namespace pax
