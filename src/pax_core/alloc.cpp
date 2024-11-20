#include <pax_core/alloc.hpp>
#include <pax_core/report.hpp>

namespace pax
{
    //
    //
    // Exposed.
    //
    //

    void
    alloc_request(Alloc alloc, Alloc_Value* value)
    {
        pax_guard(alloc.func_request != 0,
            "`func_request` is null");

        (*alloc.func_request)(alloc.self, value);
    }

    void
    alloc_release(Alloc alloc, Alloc_Value value)
    {
        pax_guard(alloc.func_release != 0,
            "`func_release` is null");

        (*alloc.func_release)(alloc.self, value);
    }
} // namespace pax
