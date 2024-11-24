#ifndef PAX_CORE_ARENA_HPP
#define PAX_CORE_ARENA_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>
#include <pax_core/mem/alloc.hpp>

namespace pax
{
    struct Bump_Node;

    struct Arena {
        Alloc      alloc;
        Bump_Node* list;
    };

    Arena
    arena_empty();

    Arena
    arena_from(Alloc alloc);

    void
    arena_drop(Arena* self);

    Alloc_Error
    arena_request(Arena* self, Alloc_Value* value);

    void
    arena_release(Arena* self, Alloc_Value value);

    void
    arena_clear(Arena* self);

    Alloc
    arena_alloc(Arena* self);
} // namespace pax

#endif // PAX_CORE_ARENA_HPP
