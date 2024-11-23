#ifndef PAX_CORE_ARENA_HPP
#define PAX_CORE_ARENA_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>
#include <pax_core/alloc.hpp>
#include <pax_core/base_alloc.hpp>

namespace pax
{
    struct Arena_Node;

    enum Arena_Error : isize {
        ARENA_ERROR_NONE          = 0,
        ARENA_ERROR_OUT_OF_MEMORY = 1,

        ARENA_ERROR_COUNT = 2,
    };

    struct Arena {
        Alloc       alloc;
        Arena_Node* list  = 0;
    };

    extern const Array<Str8, ARENA_ERROR_COUNT> ARENA_ERROR_TITLE;

    Arena
    arena_init(Alloc alloc = base_alloc());

    void
    arena_drop(Arena* arena);

    /*
    
    //
    // Makes sure that x bytes are available before
    // calling many requests afterwards.
    //
    Arena_Error
    arena_reserve(Arena* arena, Alloc_Value value);

    */

    Arena_Error
    arena_request(Arena* arena, Alloc_Value* value);

    void
    arena_clear(Arena* arena);
} // namespace pax

#endif // PAX_CORE_ARENA_HPP
