#ifndef PAX_CORE_ALLOC_HPP
#define PAX_CORE_ALLOC_HPP

#include <pax_core/types.hpp>

namespace pax
{
    struct Alloc_Info {
        isize width;
        isize align;
        isize count;
    };

    struct Alloc {
        byte* (*request_func) (
            void* self, Alloc_Info info
        ) = 0;

        void (*release_func) (
            void* self, Alloc_Info info, byte* addr
        ) = 0;

        void* self = 0;
    };

    byte*
    alloc_request(const Alloc* alloc, Alloc_Info info);

    void
    alloc_release(const Alloc* alloc, Alloc_Info info, byte* addr);

    Alloc
    base_alloc();
} // namespace pax

#endif // PAX_CORE_ALLOC_HPP
