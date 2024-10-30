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
    s8_from(const Buff* buffer)
    {
        pax_trace();

        auto  self = s8 {""};
        isize size = buff_count(buffer);

        self.addr = buffer->head;
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
