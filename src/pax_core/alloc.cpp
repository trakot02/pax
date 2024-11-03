#include <pax_core/alloc.hpp>
#include <pax_core/report.hpp>

#include <malloc.h>

namespace pax
{
    byte*
    impl_base_request(void* self, isize width, isize align, isize count);

    void
    impl_base_release(void* self, byte* addr, isize size);

    //
    //
    // Implementation.
    //
    //

    byte*
    alloc_request(const Alloc* alloc, isize width, isize align, isize count)
    {
        pax_trace();
        pax_guard(alloc != 0, "`alloc` is null");

        auto& self = *alloc;
        auto* func = self.request_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, width, align, count);
    }

    void
    alloc_release(const Alloc* alloc, byte* addr, isize size)
    {
        pax_trace();
        pax_guard(alloc != 0, "`alloc` is null");

        auto& self = *alloc;
        auto* func = self.release_func;

        pax_guard(func != 0, "The function is null");

        return (*func)(self.self, addr, size);
    }

    Alloc
    base_alloc()
    {
        pax_trace();

        Alloc self = {0};

        self.request_func = &impl_base_request;
        self.release_func = &impl_base_release;

        return self;
    }

    //
    //
    // Not exposed.
    //
    //

    byte*
    impl_base_request(void* self, isize width, isize align, isize count)
    {
        pax_trace();
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
    impl_base_release(void* self, byte* addr, isize size)
    {
        pax_trace();
        pax_guard(self == 0, "`self` isn't null");

        free(addr);
    }
} // namespace pax
