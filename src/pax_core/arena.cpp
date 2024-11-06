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
    arena_node_from(byte* addr, isize size);

    byte*
    arena_node_request(Arena_Node* node, Alloc_Info info);

    void
    arena_node_release(Arena_Node* node);

    Arena_Node*
    arena_attach(Arena* arena);

    Arena_Node*
    arena_detach(Arena* arena, Arena_Node* node);

    //
    //
    // Implementation.
    //
    //

    const Array<s8, ARENA_ERR_COUNT> ARENA_ERR_TITLE = {
        "ARENA_ERR_NONE",
        "ARENA_ERR_REQUEST_TOO_BIG",
        "ARENA_ERR_OUT_OF_MEMORY",
    };

    Arena
    arena_init(isize size, Alloc alloc)
    {
        static const isize MAX_ARENA = 65536;

        pax_guard(size >= 0, "`size` isn't positive");

        Arena self;
        isize temp = 1024;

        while ( temp - WIDTH_ARENA_NODE < size ) {
            if ( temp > MAX_ARENA / 2 ) return self;

            temp *= 2;
        }

        self.size  = temp - WIDTH_ARENA_NODE;
        self.alloc = alloc;

        return self;
    }

    void
    arena_drop(Arena* arena)
    {
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;
        auto* node = self.list;

        while ( node != 0 )
            node = arena_detach(arena, node);

        self.alloc = Alloc {};
        self.list  = 0;
    }

    Arena_Res
    arena_request(Arena* arena, Alloc_Info info)
    {
        auto width = info.width;
        auto align = info.align;
        auto count = info.count;

        pax_guard(arena != 0, "`arena` is null");
        pax_guard(width  > 0, "`width` isn't positive");

        auto& self = *arena;
        byte* addr = 0;

        if ( count <= 0 ) return {info, 0, ARENA_ERR_NONE};

        if ( width * count > self.size )
            return {info, 0, ARENA_ERR_REQUEST_TOO_BIG};

        if ( self.list == 0 ) self.list = arena_attach(arena);

        auto* node = self.list;

        while ( addr == 0 ) {
            if ( node == 0 )
                return {info, 0, ARENA_ERR_OUT_OF_MEMORY};

            addr = arena_node_request(node, info);

            if ( addr == 0 && node->next == 0 )
                node->next = arena_attach(arena);

            node = node->next;
        }

        return {info, addr, ARENA_ERR_NONE};
    }

    void
    arena_release(Arena* arena)
    {
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;
        auto* node = self.list;

        while ( node != 0 ) {
            arena_node_release(node);

            node = node->next;
        }
    }

    Alloc
    arena_set_alloc(Arena* arena, Alloc alloc)
    {
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;
        auto  temp = self.alloc;

        self.alloc = alloc;

        return temp;
    }

    //
    //
    // Not exposed.
    //
    //

    byte*
    align_forw(byte* addr, isize align)
    {
        pax_guard(align > 0 && (align & (align - 1)) == 0,
            "`align` is not a power of two");

        isize temp = (isize) addr;
        isize mask = align - 1;

        return addr + (-temp & mask);
    }

    Arena_Node
    arena_node_from(byte* addr, isize size)
    {
        pax_guard(size >= 0, "`size` is negative");

        Arena_Node self;

        if ( addr == 0 || size == 0 )
            return self;

        self.head = addr;
        self.tail = addr + size;
        self.curr = addr;

        return self;
    }

    byte*
    arena_node_request(Arena_Node* node, Alloc_Info info)
    {
        auto width = info.width;
        auto align = info.align;
        auto count = info.count;

        pax_guard(node  != 0, "`node` is null");
        pax_guard(width  > 0, "`width` isn't positive");

        if ( count <= 0 ) return 0;

        auto& self = *node;
        byte* addr = align_forw(self.curr, align);
        usize temp = (usize) addr;

        pax_guard((temp & (align - 1)) == 0,
            "The result is not aligned properly");

        isize extra = addr - self.curr;
        isize avail = self.tail - addr;

        if ( count <= (avail - extra) / width ) {
            isize size = width * count;
            auto  buff = buff_from_addr(addr, size);

            buff_clear(&buff);

            self.curr = addr + size;

            return addr;
        }

        return 0;
    }

    void
    arena_node_release(Arena_Node* node)
    {
        pax_guard(node!= 0, "`node` is null");

        auto& self = *node;

        self.curr = self.head;
    }

    Arena_Node*
    arena_attach(Arena* arena)
    {
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;

        byte* alloc = alloc_request(&self.alloc, {
            self.size + WIDTH_ARENA_NODE, ALIGN_MAX, 1,
        });

        auto* node = (Arena_Node*) alloc;
        auto* addr = alloc + WIDTH_ARENA_NODE;

        if ( node != 0 )
            *node = arena_node_from(addr, self.size);

        return node;
    }

    Arena_Node*
    arena_detach(Arena* arena, Arena_Node* node)
    {
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;
        auto* next = node;
        byte* addr = (byte*) node;

        if ( node != 0 ) {
            next = node->next;

            alloc_release(&self.alloc, {
                self.size, ALIGN_MAX, 1,
            }, addr);
        }

        return next;
    }
} // namespace pax
