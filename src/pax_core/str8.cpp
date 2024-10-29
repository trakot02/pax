#include <pax_core/str8.hpp>
#include <pax_core/report.hpp>
#include <pax_core/buff.hpp>

namespace pax
{
    //
    //
    // Implementation.
    //
    //

    str8
    str8_from(const Buff* buffer)
    {
        pax_trace();

        str8  self = "";
        isize size = buff_count(buffer);

        self.addr = buffer->head;
        self.size = size;

        return self;
    }

    const byte&
    str8::operator[](isize index) const
    {
        pax_trace();
        pax_guard(0 <= index && index < size,
            "`index` is out of bounds");

        return addr[index];
    }
} // namespace pax
