#ifndef PAX_CORE_ARENA_HPP
#define PAX_CORE_ARENA_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>
#include <pax_core/alloc.hpp>

namespace pax
{
    struct Arena_Node;

    enum Arena_Err : isize {
        ARENA_ERR_NONE            = 0,
        ARENA_ERR_REQUEST_TOO_BIG = 1,
        ARENA_ERR_OUT_OF_MEMORY   = 2,

        ARENA_ERR_COUNT = 3,
    };

    struct Arena_Res {
        Alloc_Info info;
        byte*      addr;
        Arena_Err  error;
    };

    struct Arena {
        Alloc       alloc;
        isize       size = 0;
        Arena_Node* list = 0;
    };

    extern const Array<s8, ARENA_ERR_COUNT> ARENA_ERR_TITLE;

    Arena
    arena_init(isize size, Alloc alloc = base_alloc());

    void
    arena_drop(Arena* arena);

    Arena_Res
    arena_request(Arena* arena, Alloc_Info info);

    void
    arena_release(Arena* arena);

    Alloc
    arena_set_alloc(Arena* arena, Alloc alloc);
} // namespace pax

#endif // PAX_CORE_ARENA_HPP
