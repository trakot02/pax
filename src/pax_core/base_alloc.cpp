#include <pax_core/base_alloc.hpp>

#include <stdlib.h>

namespace pax
{
    void
    _base_request(void* alloc, Alloc_Value* value);

    void
    _base_release(void* alloc, Alloc_Value value);

    //
    //
    // Exposed.
    //
    //

    Alloc
    base_alloc()
    {
        Alloc alloc;

        alloc.func_request = &_base_request;
        alloc.func_release = &_base_release;

        return alloc;
    }

    //
    //
    // Hidden.
    //
    //

    void
    _base_request(void* alloc, Alloc_Value* value)
    {
        pax_guard(alloc == 0, "`alloc` isn't null");
        pax_guard(value != 0, "`value` is null");

        isize width = value->width;
        isize align = value->align;
        isize cnt   = value->cnt;
        usize ptr   = 0;

        pax_guard(width > 0, "`width` isn't positive");

        pax_guard(align > 0 && (align & (align - 1)) == 0,
            "`align` is not a power of two");

        if ( cnt <= 0 ) return;

        ptr = (usize) calloc(cnt, width);

        pax_guard((ptr & (align - 1)) == 0,
            "The result is not aligned properly");

        value->ptr = (byte*) ptr;
    }

    void
    _base_release(void* alloc, Alloc_Value value)
    {
        pax_guard(alloc == 0, "`alloc` isn't null");

        isize width = value.width;
        isize align = value.align;
        isize cnt   = value.cnt;
        usize ptr   = (usize) value.ptr;

        pax_guard(width > 0, "`width` isn't positive");

        pax_guard(align > 0 && (align & (align - 1)) == 0,
            "`align` is not a power of two");

        pax_guard((ptr & (align - 1)) == 0,
            "The result is not aligned properly");

        pax_guard((cnt <= 0 && ptr == 0) ||
                  (cnt  > 0 && ptr != 0),
            "`ptr` isn't null");

        free((byte*) ptr);
    }
} // namespace pax
