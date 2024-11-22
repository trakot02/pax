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
    align_forw(byte* block, isize align);

    Arena_Node
    arena_node_from(byte* block, isize count);

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
    arena_init(isize count, Alloc alloc)
    {
        pax_guard(count >= 0, "`count` isn't positive");

        Arena arena;

        if ( count > MAX_ISIZE - WIDTH_ARENA_NODE )
            return arena;

        isize temp = 1024;

        while ( temp < (count - WIDTH_ARENA_NODE) / 2 )
            temp *= 2;

        arena.count = temp - WIDTH_ARENA_NODE;
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
        isize count = value->count;

        pax_guard(width > 0, "`value.width` isn't positive");

        if ( count <= 0 ) return ARENA_ERROR_NONE;

        if ( width * count > arena->count )
            return ARENA_ERROR_REQUEST_TOO_BIG;

        if ( arena->list == 0 )
            arena->list = arena_attach(arena);

        auto* node = arena->list;

        while ( value->block == 0 ) {
            if ( node == 0 )
                return ARENA_ERROR_OUT_OF_MEMORY;

            arena_node_request(node, value);

            if ( value->block == 0 && node->next == 0 )
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
    align_forw(byte* block, isize align)
    {
        isize temp = (isize) block;
        isize mask = align - 1;

        pax_guard(align > 0 && (align & (align - 1)) == 0,
            "`align` is not a power of two");

        return block + (-temp & mask);
    }

    Arena_Node
    arena_node_from(byte* block, isize count)
    {
        pax_guard(count >= 0, "`count` is negative");

        Arena_Node node;

        if ( block == 0 || count == 0 )
            return node;

        node.head = block;
        node.tail = block + count;
        node.curr = block;

        return node;
    }

    void
    arena_node_request(Arena_Node* node, Alloc_Value* value)
    {
        pax_guard(node  != 0, "`node` is null");
        pax_guard(value != 0, "`value` is null");

        isize width = value->width;
        isize align = value->align;
        isize count = value->count;

        pax_guard(width > 0, "`value.width` isn't positive");

        if ( count <= 0 ) return;

        byte* block = align_forw(node->curr, align);

        isize extra = block - node->curr;
        isize avail = node->tail - block;

        pax_guard((((usize) block) & (align - 1)) == 0,
            "The result is not aligned properly");

        if ( count <= (avail - extra) / width ) {
            isize total  = width * count;
            auto  buffer = buff_from(block, total);

            buff_fill(&buffer);

            node->curr   = block + total;
            value->block = block;
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

        isize width = arena->count + WIDTH_ARENA_NODE;
        isize align = ALIGN_MAX;

        Alloc_Value value = {width, align, 1};

        alloc_request(arena->alloc, &value);

        auto* node  = (Arena_Node*) value.block;
        auto* block = value.block + WIDTH_ARENA_NODE;

        if ( node != 0 )
            *node = arena_node_from(block, arena->count);

        return node;
    }

    Arena_Node*
    arena_detach(Arena* arena, Arena_Node* node)
    {
        pax_guard(arena != 0, "`arena` is null");

        auto* next  = node;
        isize width = arena->count + WIDTH_ARENA_NODE;
        isize align = ALIGN_MAX;

        Alloc_Value value = {width, align, 1};

        if ( node != 0 ) {
            value.block = (byte*) node;
            next        = node->next;

            alloc_release(arena->alloc, value);
        }

        return next;
    }
} // namespace pax
