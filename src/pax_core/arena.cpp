#include <pax_core/arena.hpp>
#include <pax_core/report.hpp>
#include <pax_core/buff.hpp>

namespace pax
{
    struct Arena_Node {
        byte*       head = 0;
        byte*       tail = 0;
        byte*       curr = 0;
        Arena_Node* next = 0;
    };

    static const isize WIDTH_ARENA_NODE = pax_type_width(Arena_Node);

    byte*
    align_forw(byte* addr, isize align);

    Arena_Node
    arena_node_from(byte* ptr, isize cnt);

    void
    arena_node_request(Arena_Node* node, Alloc_Value* value);

    void
    arena_node_resize(Arena_Node* node, Alloc_Value* value);

    void
    arena_node_release(Arena_Node* node, Alloc_Value value);

    void
    arena_node_clear(Arena_Node* node);

    Arena_Node*
    arena_attach(Arena* arena);

    Arena_Node*
    arena_detach(Arena* arena, Arena_Node* node);

    //
    //
    // Exposed.
    //
    //

    const Array<Str8, ARENA_ERROR_COUNT> ARENA_ERROR_TITLE = {
        "ARENA_ERROR_NONE",
        "ARENA_ERROR_REQUEST_TOO_BIG",
        "ARENA_ERROR_OUT_OF_MEMORY",
    };

    Arena
    arena_init(isize size, Alloc alloc)
    {
        pax_guard(size >= 0, "`size` isn't positive");

        Arena arena;

        if ( size > MAX_ISIZE - WIDTH_ARENA_NODE )
            return arena;

        isize temp = 1024;

        while ( temp < (size - WIDTH_ARENA_NODE) / 2 )
            temp *= 2;

        arena.size  = temp - WIDTH_ARENA_NODE;
        arena.alloc = alloc;

        return arena;
    }

    void
    arena_drop(Arena* arena)
    {
        pax_guard(arena != 0, "`arena` is null");

        auto* node = arena->list;

        while ( node != 0 )
            node = arena_detach(arena, node);

        arena->alloc = {};
        arena->list  = 0;
    }

    Arena_Error
    arena_request(Arena* arena, Alloc_Value* value)
    {
        pax_guard(arena != 0, "`arena` is null");
        pax_guard(value != 0, "`value` is null");

        isize width = value->width;
        isize align = value->align;
        isize cnt   = value->cnt;
        byte* ptr   = 0;

        pax_guard(width > 0, "`width` isn't positive");

        if ( cnt <= 0 ) return ARENA_ERROR_NONE;

        if ( width * cnt > arena->size )
            return ARENA_ERROR_REQUEST_TOO_BIG;

        if ( arena->list == 0 )
            arena->list = arena_attach(arena);

        auto* node = arena->list;

        while ( ptr == 0 ) {
            if ( node == 0 )
                return ARENA_ERROR_OUT_OF_MEMORY;

            arena_node_request(node, value);

            if ( value->ptr == 0 && node->next == 0 )
                node->next = arena_attach(arena);

            node = node->next;
        }

        return ARENA_ERROR_NONE;
    }

    Arena_Error
    arena_resize(Arena* arena, Alloc_Value* value)
    {
        return ARENA_ERROR_NONE;
    }

    void
    arena_release(Arena* arena, Alloc_Value value)
    {}

    void
    arena_clear(Arena* arena)
    {
        pax_guard(arena != 0, "`arena` is null");

        auto* node = arena->list;

        while ( node != 0 ) {
            arena_node_clear(node);

            node = node->next;
        }
    }

    Alloc
    arena_set_alloc(Arena* arena, Alloc alloc)
    {
        pax_guard(arena != 0, "`arena` is null");

        auto temp = arena->alloc;

        arena->alloc = alloc;

        return temp;
    }

    //
    //
    // Hidden.
    //
    //

    byte*
    align_forw(byte* addr, isize align)
    {
        isize temp = (isize) addr;
        isize mask = align - 1;

        pax_guard(align > 0 && (align & (align - 1)) == 0,
            "`align` is not a power of two");

        return addr + (-temp & mask);
    }

    Arena_Node
    arena_node_from(byte* ptr, isize cnt)
    {
        pax_guard(cnt >= 0, "`cnt` is negative");

        Arena_Node node;

        if ( ptr == 0 || cnt == 0 )
            return node;

        node.head = ptr;
        node.tail = ptr + cnt;
        node.curr = ptr;

        return node;
    }

    void
    arena_node_request(Arena_Node* node, Alloc_Value* value)
    {
        pax_guard(node  != 0, "`node` is null");
        pax_guard(value != 0, "`value` is null");

        isize width = value->width;
        isize align = value->align;
        isize cnt   = value->cnt;
        byte* ptr   = 0;

        pax_guard(width > 0, "`width` isn't positive");

        if ( cnt <= 0 ) return;

        ptr = align_forw(node->curr, align);

        isize extra = ptr - node->curr;
        isize avail = node->tail - ptr;

        pax_guard((((usize) ptr) & (align - 1)) == 0,
            "The result is not aligned properly");

        if ( cnt <= (avail - extra) / width ) {
            isize size = width * cnt;
            auto  buf  = buff_from(ptr, size);

            buff_fill(&buf);

            node->curr = ptr + size;
            value->ptr = ptr;
        }
    }

    void
    arena_node_resize(Arena_Node* node, Alloc_Value* value)
    {}

    void
    arena_node_release(Arena_Node* node, Alloc_Value value)
    {}

    void
    arena_node_clear(Arena_Node* node)
    {
        pax_guard(node!= 0, "`node` is null");

        node->curr = node->head;
    }

    Arena_Node*
    arena_attach(Arena* arena)
    {
        pax_guard(arena != 0, "`arena` is null");

        isize width = arena->size + WIDTH_ARENA_NODE;
        isize align = ALIGN_MAX;

        Alloc_Value value = {width, align, 1};

        alloc_request(arena->alloc, &value);

        auto* node = (Arena_Node*) value.ptr;
        auto* addr = value.ptr + WIDTH_ARENA_NODE;

        if ( node != 0 )
            *node = arena_node_from(addr, arena->size);

        return node;
    }

    Arena_Node*
    arena_detach(Arena* arena, Arena_Node* node)
    {
        pax_guard(arena != 0, "`arena` is null");

        auto* next  = node;
        isize width = arena->size + WIDTH_ARENA_NODE;
        isize align = ALIGN_MAX;

        Alloc_Value value = {width, align, 1};

        if ( node != 0 ) {
            value.ptr = (byte*) node;
            next      = node->next;

            alloc_release(arena->alloc, value);
        }

        return next;
    }
} // namespace pax
