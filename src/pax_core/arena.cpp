#include <cstdlib>

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

    const Arr<str8, arena_err_count> ARENA_ERR_TITLE = {
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

        usize value = (usize) addr;
        usize negat = ~value + 1;

        align = align - 1;

        return addr + (negat & align);
    }

    Arena_Buff
    arena_buff_from(byte* addr, isize size)
    {
        pax_trace();
        pax_guard(size >= 0, "`size` is negative");

        Arena_Buff self = {0};

        if ( addr == 0 ) return self;

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

        pax_guard((((usize) addr) & (align - 1)) == 0,
            "The result is not aligned properly");

        isize extra = addr - self.curr;
        isize avail = (self.tail - addr) - extra;
        isize limit = avail / width;

        if ( 0 < count && count <= limit ) {
            isize size = width * count;
            auto  temp = buff_from(addr, size);

            buff_fill(&temp, 0);

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
    arena_init(isize size)
    {
        pax_trace();
        pax_guard(size >= 0, "`size` is negative");

        Arena self = {0};

        self.size = size;

        return self;
    }

    void
    arena_drop(Arena* arena)
    {
        pax_trace();
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;
        auto* buff = self.list;

        while ( buff != 0 )
            buff = arena_detach(arena, buff);

        self.list = 0;
    }

    Arena_Res
    arena_request(Arena* arena, isize width, isize align, isize count)
    {
        pax_trace();
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;
        byte* addr = 0;

        if ( width * count > self.size )
            return {0, 0, 0, 0, arena_err_request_too_big};

        if ( self.list == 0 )
            self.list = arena_attach(arena, self.size);

        auto* buff = self.list;

        while ( addr == 0 ) {
            if ( buff == 0 )
                return {0, 0, 0, 0, arena_err_out_of_memory};

            addr = arena_buff_request(buff,
                width, align, count);

            if ( addr == 0 && buff->next == 0 )
                buff->next = arena_attach(arena, self.size);

            buff = buff->next;
        }

        return {width, align, count, addr, arena_err_none};
    }

    void
    arena_release(Arena* arena)
    {
        pax_trace();
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;
        auto* buff = self.list;

        while ( buff != 0 ) {
            arena_buff_release(buff);

            buff = buff->next;
        }
    }

    //
    //
    // Not exposed.
    //
    //

    Arena_Buff*
    arena_attach(Arena* arena, isize size)
    {
        static constexpr isize WIDTH_BUFF =
            pax_type_width(Arena_Buff);

        pax_trace();
        pax_guard(arena != 0, "`arena` is null");
        pax_guard(size  >= 0, "`size` is negative");

        auto& self = *arena;

        if ( size <= MAX_ISIZE - WIDTH_BUFF ) {
            byte* alloc = (byte*) calloc(size + WIDTH_BUFF, 1);

            auto* buff = (Arena_Buff*) alloc;
            auto* addr = alloc + WIDTH_BUFF;

            if ( alloc != 0 )
                *buff = arena_buff_from(addr, size);

            return buff;
        }

        return 0;
    }

    Arena_Buff*
    arena_detach(Arena* arena, Arena_Buff* buff)
    {
        pax_trace();
        pax_guard(arena != 0, "`arena` is null");

        auto& self = *arena;

        if ( buff == 0 )
            return 0;

        auto* next = buff->next;

        free(buff);

        return next;
    }
} // namespace pax
