#ifndef PAX_CORE_ALLOC_HPP
#define PAX_CORE_ALLOC_HPP

#include <pax_core/types.hpp>

namespace pax
{
    struct Alloc_Value {
        isize width;
        isize align;
        isize count;
        byte* block;
    };

    struct Alloc {
        void* self = 0;

        void (*func_request) (
            void* self, Alloc_Value* value
        ) = 0;

        void (*func_release) (
            void* self, Alloc_Value value
        ) = 0;
    };

    void
    alloc_request(Alloc alloc, Alloc_Value* value);

    void
    alloc_release(Alloc alloc, Alloc_Value value);
} // namespace pax

#endif // PAX_CORE_ALLOC_HPP
