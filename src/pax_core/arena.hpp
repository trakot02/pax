#ifndef PAX_CORE_ARENA_HPP
#define PAX_CORE_ARENA_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>
#include <pax_core/alloc.hpp>

namespace pax
{
    struct Arena_Node;

    enum Arena_Error : isize {
        ARENA_ERROR_NONE            = 0,
        ARENA_ERROR_REQUEST_TOO_BIG = 1,
        ARENA_ERROR_OUT_OF_MEMORY   = 2,

        ARENA_ERROR_COUNT = 3,
    };

    struct Arena_Value {
        Alloc_Info   info;
        byte*        addr;
        Arena_Error  error;
    };

    struct Arena {
        Alloc       alloc;
        isize       size = 0;
        Arena_Node* list = 0;
    };

    extern const Array<s8, ARENA_ERROR_COUNT> ARENA_ERROR_TITLE;

    Arena
    arena_init(isize size, Alloc alloc = base_alloc());

    void
    arena_drop(Arena* arena);

    Arena_Value
    arena_request(Arena* arena, Alloc_Info info);

    void
    arena_release(Arena* arena);

    Alloc
    arena_set_alloc(Arena* arena, Alloc alloc);
} // namespace pax

#endif // PAX_CORE_ARENA_HPP
