#ifndef PAX_CORE_BUMP_HPP
#define PAX_CORE_BUMP_HPP

#include <pax_core/types.hpp>
#include <pax_core/mem/alloc.hpp>

namespace pax
{
    struct Bump {
        byte* head;
        byte* tail;
        byte* curr;
    };

    Bump
    bump_empty();

    Bump
    bump_from(byte* block, isize count);

    isize
    bump_count(const Bump* self);

    isize
    bump_avail(const Bump* self);

    isize
    bump_size(const Bump* self);

    Alloc_Error
    bump_init(Bump* self, Alloc alloc, isize count);

    Alloc_Error
    bump_request(Bump* self, Alloc_Value* value);

    void
    bump_release(Bump* self, Alloc_Value value);

    void
    bump_clear(Bump* self);

    Alloc
    bump_alloc(Bump* self);
} // namespace pax

#endif // PAX_CORE_BUMP_HPP
