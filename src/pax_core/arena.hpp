#ifndef PAX_CORE_ARENA_HPP
#define PAX_CORE_ARENA_HPP

#include <pax_core/types.hpp>
#include <pax_core/array.hpp>
#include <pax_core/alloc.hpp>

// todo (trakot02):
//
// Implement a way to ask the arena for a buffer
// to use manually. This way the allocations are
// still associated to an arena but managed outside
// of it.

namespace pax
{
    enum Arena_Err : isize {
        arena_err_none            = 0,
        arena_err_request_too_big = 1,
        arena_err_out_of_memory   = 2,

        arena_err_count = 3,
    };

    struct Arena_Res {
        isize     width;
        isize     align;
        isize     count;
        byte*     addr;
        Arena_Err error;
    };

    struct Arena_Buff {
        byte*       head;
        byte*       tail;
        byte*       curr;
        Arena_Buff* next;
    };

    struct Arena {
        isize       size;
        Alloc       alloc;
        Arena_Buff* list;
    };

    extern const Array<s8, arena_err_count> ARENA_ERR_TITLE;

    byte*
    align_forw(byte* addr, isize align);

    Arena_Buff
    arena_buff_from(byte* addr, isize size);

    byte*
    arena_buff_request(Arena_Buff* buffer, isize width, isize align, isize count);

    void
    arena_buff_release(Arena_Buff* buffer);

    Arena
    arena_init(isize size, Alloc alloc = base_alloc());

    void
    arena_drop(Arena* arena);

    Arena_Res
    arena_request(Arena* arena, isize width, isize align, isize count);

    void
    arena_release(Arena* arena);

    Alloc
    arena_set_alloc(Arena* arena, Alloc alloc);
} // namespace pax

#endif // PAX_CORE_ARENA_HPP
