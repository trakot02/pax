#include <pax_core/alloc.hpp>
#include <pax_core/report.hpp>

#include <malloc.h>

namespace pax
{
    byte*
    _base_request(void* self, Alloc_Info info);

    void
    _base_release(void* self, Alloc_Info info, byte* addr);

    //
    //
    // Implementation.
    //
    //

    byte*
    alloc_request(const Alloc* alloc, Alloc_Info info)
    {
        pax_guard(alloc != 0, "`alloc` is null");

        auto& self = *alloc;
        auto* func = self.request_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, info);
    }

    void
    alloc_release(const Alloc* alloc, Alloc_Info info, byte* addr)
    {
        pax_guard(alloc != 0, "`alloc` is null");

        auto& self = *alloc;
        auto* func = self.release_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, info, addr);
    }

    Alloc
    base_alloc()
    {
        Alloc self;

        self.request_func = &_base_request;
        self.release_func = &_base_release;

        return self;
    }

    //
    //
    // Not exposed.
    //
    //

    byte*
    _base_request(void* self, Alloc_Info info)
    {
        auto width = info.width;
        auto align = info.align;
        auto count = info.count;

        pax_guard(self  == 0, "`self` isn't null");
        pax_guard(width  > 0, "`width` isn't positive");

        pax_guard(align > 0 && (align & (align - 1)) == 0,
            "`align` is not a power of two");

        if ( count <= 0 ) return 0;

        byte* addr = (byte*) calloc(count, width);
        usize temp = (usize) addr;

        pax_guard((temp & (align - 1)) == 0,
            "The result is not aligned properly");

        return addr;
    }

    void
    _base_release(void* self, Alloc_Info info, byte* addr)
    {
        auto width = info.width;
        auto align = info.align;
        auto count = info.count;

        pax_guard(self  == 0, "`self` isn't null");
        pax_guard(width  > 0, "`width` isn't positive");

        pax_guard((count <= 0 && addr == 0) ||
                  (count  > 0 && addr != 0),
            "`addr` isn't null");

        usize temp = (usize) addr;

        pax_guard((temp & (align - 1)) == 0,
            "The result is not aligned properly");

        free(addr);
    }
} // namespace pax
