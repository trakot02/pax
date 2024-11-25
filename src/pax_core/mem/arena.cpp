#include <pax_core/mem/arena.hpp>
#include <pax_core/mem/bump.hpp>
#include <pax_core/report.hpp>

namespace pax
{
    struct Bump_Node {
        Bump_Node* next;
        Bump       bump;
    };

    static const isize WIDTH_BUMP_NODE = pax_type_width(Bump_Node);

    Bump_Node
    _bump_node_empty();

    Bump_Node
    _bump_node_from(byte* block, isize count);

    Bump_Node*
    _arena_attach(Arena* self, Alloc_Value* value);

    void
    _arena_detach(Arena* self, Bump_Node* node);

    Alloc_Error
    _arena_request(void* self, Alloc_Value* value);

    void
    _arena_release(void* self, Alloc_Value value);

    void
    _arena_clear(void* self);

    //
    //
    // Exposed.
    //
    //

    Arena
    arena_empty()
    {
        Arena arena;

        arena.alloc = alloc_empty();
        arena.list  = 0;

        return arena;
    }

    Arena
    arena_from(Alloc alloc)
    {
        auto arena = arena_empty();

        arena.alloc = alloc;

        return arena;
    }

    void
    arena_drop(Arena* self)
    {
        pax_guard(self != 0, "`self` is null");

        auto* node = self->list;
        auto* temp = node;

        for ( ; node != 0; node = temp ) {
            temp = node->next;

            _arena_detach(self, node);
        }

        *self = arena_empty();
    }

    Alloc_Error
    arena_request(Arena* self, Alloc_Value* value)
    {
        pax_guard(self  != 0, "`self` is null");
        pax_guard(value != 0, "`value` is null");

        isize width = value->width;
        isize align = value->align;
        isize count = value->count;

        auto copy = *value;

        pax_guard(width > 0, "`value.width` isn't positive");

        if ( count <= 0 ) return ALLOC_ERROR_NONE;

        if ( self->list == 0 )
            self->list = _arena_attach(self, &copy);

        auto* node = self->list;

        while ( value->block == 0 ) {
            if ( node == 0 )
                return ALLOC_ERROR_OUT_OF_MEMORY;

            bump_request(&node->bump, value);

            if ( value->block == 0 && node->next == 0 )
                node->next = _arena_attach(self, &copy);

            node = node->next;
        }

        return ALLOC_ERROR_NONE;
    }

    void
    arena_release(Arena* self, Alloc_Value value)
    {
        pax_guard(self != 0, "`self` is null");
    }

    void
    arena_clear(Arena* self)
    {
        pax_guard(self != 0, "`self` is null");

        auto* node = self->list;

        for ( ; node != 0; node = node->next )
            bump_clear(&node->bump);
    }

    Alloc
    arena_alloc(Arena* self)
    {
        auto alloc = alloc_empty();

        alloc.context = self;

        alloc.func_request = &_arena_request;
        alloc.func_release = &_arena_release;
        alloc.func_clear   = &_arena_clear;

        return alloc;
    }

    //
    //
    // Not exposed.
    //
    //

    Bump_Node
    _bump_node_empty()
    {
        Bump_Node node;

        node.bump = bump_empty();
        node.next = 0;

        return node;
    }

    Bump_Node
    _bump_node_from(byte* block, isize count)
    {
        pax_guard(count >= 0, "`count` is negative");

        auto node = _bump_node_empty();

        node.bump = bump_from(block, count);

        return node;
    }

    Bump_Node*
    _arena_attach(Arena* self, Alloc_Value* value)
    {
        pax_guard(self  != 0, "`self` is null");
        pax_guard(value != 0, "`value` is null");

        isize size = 1024;

        if ( value->count < MAX_ISIZE / 2 ) {
            while ( size <= value->count )
                size *= 2;
        } else
            return 0;

        *value = alloc_value_from(1, ALIGN_MAX, size);

        alloc_request(self->alloc, value);

        auto* node  = (Bump_Node*) value->block;
        isize delta = WIDTH_BUMP_NODE;

        if ( node != 0 ) {
            byte* block = value->block + delta;
            isize count = size - delta;

            *node = _bump_node_from(block, count);
        }

        return node;
    }

    void
    _arena_detach(Arena* self, Bump_Node* node)
    {
        pax_guard(self != 0, "`self` is null");

        auto  value = alloc_value_from(0, ALIGN_MAX, 1);
        isize delta = WIDTH_BUMP_NODE;

        if ( node != 0 ) {
            isize size = bump_size(&node->bump);

            value.width = size + delta;
            value.block = (byte*) node;

            alloc_release(self->alloc, value);
        }
    }

    Alloc_Error
    _arena_request(void* self, Alloc_Value* value)
    {
        return arena_request((Arena*) self, value);
    }

    void
    _arena_release(void* self, Alloc_Value value)
    {
        return arena_release((Arena*) self, value);
    }

    void
    _arena_clear(void* self)
    {
        return arena_clear((Arena*) self);
    }
} // namespace pax
