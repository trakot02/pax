#include <pax_core/arena.hpp>
#include <pax_core/report.hpp>
#include <pax_core/buff.hpp>

namespace pax
{
    Arena_Buff*
    arena_attach(Arena* arena, isize size);

    Arena_Buff*
    arena_detach(Arena* arena, Arena_Buff* buff);

    //
    //
    // Implementation.
    //
    //

    const Array<s8, _arena_err_count> ARENA_ERR_TITLE = {
        "arena_err_none",
        "arena_err_request_too_big",
        "arena_err_out_of_memory",
    };

    byte*
    align_forw(byte* addr, isize align)
    {
        pax_trace();
        pax_guard(align > 0 && (align & (align - 1)) == 0,
            "`align` is not a power of two");

        isize temp = (isize) addr;
        isize mask = align - 1;

        return addr + (-temp & mask);
    }

    Arena_Buff
    arena_buff_from(byte* addr, isize size)
    {
        pax_trace();
        pax_guard(size >= 0, "`size` is negative");

        Arena_Buff self = {0};

        if ( addr == 0 || size == 0 )
            return self;

        self.head = addr;
        self.tail = addr + size;
        self.curr = addr;

        return self;
    }

    byte*
    arena_buff_request(Arena_Buff* buff, isize width, isize align, isize count)
    {
        pax_trace();
        pax_guard(buff  != 0, "`buff` is null");
        pax_guard(width  > 0, "`width` isn't positive");

        auto& self = *buff;
        byte* addr = align_forw(self.curr, align);
        usize temp = (usize) addr;

        pax_guard((temp & (align - 1)) == 0,
            "The result is not aligned properly");

        isize extra = addr - self.curr;
        isize avail = (self.tail - addr) - extra;
        isize limit = avail / width;

        if ( 0 < count && count <= limit ) {
            isize size = width * count;
            auto  bufr = buff_from(addr, size);

            buff_fill_byte(&bufr, 0);

            self.curr = addr + size;

            return addr;
        }

        return 0;
    }

    void
    arena_buff_release(Arena_Buff* buff)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self = *buff;

        self.curr = self.head;
    }

    Arena
    arena_init(isize size, Alloc alloc)
    {
        pax_trace();
        pax_guard(size >= 0, "`size` is negative");

        Arena self = {0};

        self.size  = size;
        self.alloc = alloc;

        return self;
    }

    void
    arena_drop(Arena* arena)
    {
        pax_trace();
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;
        auto* node = self.list;

        while ( node != 0 )
            node = arena_detach(arena, node);

        self.alloc = Alloc {0};
        self.list  = 0;
    }

    Arena_Res
    arena_request(Arena* arena, isize width, isize align, isize count)
    {
        pax_trace();
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;
        byte* addr = 0;

        if ( width * count > self.size )
            return {0, 0, 0, 0, _arena_err_request_too_big};

        if ( self.list == 0 )
            self.list = arena_attach(arena, self.size);

        auto* node = self.list;

        while ( addr == 0 ) {
            if ( node == 0 )
                return {0, 0, 0, 0, _arena_err_out_of_memory};

            addr = arena_buff_request(node,
                width, align, count);

            if ( addr == 0 && node->next == 0 )
                node->next = arena_attach(arena, self.size);

            node = node->next;
        }

        return {width, align, count, addr, _arena_err_none};
    }

    void
    arena_release(Arena* arena)
    {
        pax_trace();
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;
        auto* node = self.list;

        while ( node != 0 ) {
            arena_buff_release(node);

            node = node->next;
        }
    }

    Alloc
    arena_set_alloc(Arena* arena, Alloc alloc)
    {
        pax_trace();
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

    // todo (trakot02):
    //
    // Allocate the smallest power of two bigger than size:
    //
    // 1. No fragmentation;
    // 2. Bigger than size;
    // 3. Extra room for WIDTH_BUFF.

    Arena_Buff*
    arena_attach(Arena* arena, isize size)
    {
        static const isize WIDTH_BUFF =
            pax_type_width(Arena_Buff);

        pax_trace();
        pax_guard(arena != 0, "`arena` is null");
        pax_guard(size  >= 0, "`size` is negative");

        auto& self = *arena;

        if ( size <= MAX_ISIZE - WIDTH_BUFF ) {
            byte* pntr = alloc_request(&self.alloc,
                size + WIDTH_BUFF, ALIGN_MAX, 1);

            auto* node = (Arena_Buff*) pntr;
            auto* addr = pntr + WIDTH_BUFF;

            if ( node != 0 )
                *node = arena_buff_from(addr, size);

            return node;
        }

        return 0;
    }

    Arena_Buff*
    arena_detach(Arena* arena, Arena_Buff* buff)
    {
        static const isize WIDTH_BUFF =
            pax_type_width(Arena_Buff);

        pax_trace();
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;
        auto* node = buff;
        auto* next = buff;

        if ( node != 0 ) {
            isize size = node->tail - node->head;
            byte* addr = (byte*) node;

            next = node->next;

            alloc_release(&self.alloc, addr,
                size + WIDTH_BUFF);
        }

        return next;
    }
} // namespace pax
