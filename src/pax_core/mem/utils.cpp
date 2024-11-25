#include <pax_core/mem/utils.hpp>
#include <pax_core/report.hpp>

namespace pax
{
    //
    //
    // Exposed.
    //
    //

    byte*
    align_forw(byte* block, isize align)
    {
        isize temp = (isize) block;
        isize mask = align - 1;

        pax_guard(align > 0 && (align & (align -1 )) == 0,
            "`align` is not a power of two");

        return block + (-temp & mask);
    }
} // namespace pax
