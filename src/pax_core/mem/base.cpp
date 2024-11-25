#include <pax_core/mem/base.hpp>
#include <pax_core/report.hpp>

#include <stdlib.h>

namespace pax
{
    Alloc_Error
    _base_request(void* self, Alloc_Value* value);

    void
    _base_release(void* self, Alloc_Value value);

    void
    _base_clear(void* self);

    //
    //
    // Exposed.
    //
    //

    Alloc_Error
    base_request(Alloc_Value* value)
    {
        pax_guard(value != 0, "`value` is null");

        isize width = value->width;
        isize align = value->align;
        isize count = value->count;
        usize block = 0;

        pax_guard(width > 0, "`value.width` isn't positive");

        pax_guard(align > 0 && (align & (align - 1)) == 0,
            "`value.align` is not a power of two");

        if ( count <= 0 ) return ALLOC_ERROR_NONE;

        block = (usize) calloc(count, width);

        pax_guard((block & (align - 1)) == 0,
            "The result is not aligned properly");

        value->block = (byte*) block;

        return ALLOC_ERROR_NONE;
    }

    void
    base_release(Alloc_Value value)
    {
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

    void
    base_clear()
    {}

    Alloc
    base_alloc()
    {
        auto alloc = alloc_empty();

        alloc.func_request = &_base_request;
        alloc.func_release = &_base_release;
        alloc.func_clear   = &_base_clear;

        return alloc;
    }

    //
    //
    // Not exposed.
    //
    //

    Alloc_Error
    _base_request(void* self, Alloc_Value* value)
    {
        pax_guard(self == 0, "`self` isn't null");

        return base_request(value);
    }

    void
    _base_release(void* self, Alloc_Value value)
    {
        pax_guard(self == 0, "`self` isn't null");

        base_release(value);
    }

    void
    _base_clear(void* self)
    {
        pax_guard(self == 0, "`self` isn't null");

        base_clear();
    }
} // namespace pax
