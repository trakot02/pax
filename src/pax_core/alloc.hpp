#ifndef PAX_CORE_ALLOC_HPP
#define PAX_CORE_ALLOC_HPP

#include <pax_core/types.hpp>

namespace pax
{
    struct Alloc {
        byte* (*request_func)
            (void* self, isize width, isize align, isize count);

        void (*release_func)
            (void* self, byte* addr, isize size);

        void* self;
    };

    byte*
    alloc_request(const Alloc* alloc, isize width, isize align, isize count);

    void
    alloc_release(const Alloc* alloc, byte* addr, isize size);

    Alloc
    base_alloc();
} // namespace pax

#endif // PAX_CORE_ALLOC_HPP