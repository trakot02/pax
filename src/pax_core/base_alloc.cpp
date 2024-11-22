#include <pax_core/base_alloc.hpp>
#include <pax_core/report.hpp>

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
        isize count = value->count;
        usize block = 0;

        pax_guard(width > 0, "`value.width` isn't positive");

        pax_guard(align > 0 && (align & (align - 1)) == 0,
            "`value.align` is not a power of two");

        if ( count <= 0 ) return;

        block = (usize) calloc(count, width);

        pax_guard((block & (align - 1)) == 0,
            "The result is not aligned properly");

        value->block = (byte*) block;
    }

    void
    _base_release(void* alloc, Alloc_Value value)
    {
        pax_guard(alloc == 0, "`alloc` isn't null");

        isize width = value.width;
        isize align = value.align;
        isize count = value.count;
        usize block = (usize) value.block;

        pax_guard(width > 0, "`value.width` isn't positive");

        pax_guard(align > 0 && (align & (align - 1)) == 0,
            "`value.align` is not a power of two");

        pax_guard((block & (align - 1)) == 0,
            "The result is not aligned properly");

        pax_guard((count <= 0 && block == 0) ||
                  (count  > 0 && block != 0),
            "`value.block` isn't null");

        free((byte*) block);
    }
} // namespace pax
