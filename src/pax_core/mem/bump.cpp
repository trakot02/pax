#include <pax_core/mem/bump.hpp>
#include <pax_core/mem/utils.hpp>

namespace pax
{
    Alloc_Error
    _bump_request(void* self, Alloc_Value* value);

    void
    _bump_release(void* self, Alloc_Value value);

    void
    _bump_clear(void* self);

    //
    //
    // Exposed.
    //
    //

    Bump
    bump_empty()
    {
        Bump bump;

        bump.head = 0;
        bump.tail = 0;
        bump.curr = 0;

        return bump;
    }

    Bump
    bump_from(byte* block, isize count)
    {
        pax_guard(count >= 0, "`count` is negative");

        auto bump = bump_empty();

        if ( block == 0 || count == 0 )
            return bump;

        bump.head = block;
        bump.tail = block + count;
        bump.curr = block;

        return bump;
    }

    isize
    bump_count(const Bump* self)
    {
        if ( self != 0 )
            return self->curr - self->head;

        return 0;
    }

    isize
    bump_avail(const Bump* self)
    {
        if ( self != 0 )
            return self->tail - self->curr;

        return 0;
    }

    isize
    bump_size(const Bump* self)
    {
        if ( self != 0 )
            return self->tail - self->head;

        return 0;
    }

    Alloc_Error
    bump_init(Bump* self, Alloc alloc, isize count)
    {
        pax_guard(self  != 0, "`self` is null");
        pax_guard(count >= 0, "`count` is negative");

        auto value = alloc_value_from(WIDTH_BYTE,
            ALIGN_BYTE, count);

        auto error = alloc_request(alloc, &value);

        if ( error != ALLOC_ERROR_NONE )
            return error;

        self->head = value.block;
        self->tail = value.block + count;
        self->curr = value.block;

        return ALLOC_ERROR_NONE;
    }

    Alloc_Error
    bump_request(Bump* self, Alloc_Value* value)
    {
        pax_guard(self  != 0, "`self` is null");
        pax_guard(value != 0, "`value` is null");

        isize width = value->width;
        isize align = value->align;
        isize count = value->count;
        isize total = width * count;

        pax_guard(width >= 0, "`value.width` is negative");

        if ( count <= 0 ) return ALLOC_ERROR_NONE;

        byte* block = align_forw(self->curr, align);

        isize avail = self->tail - block;
        isize extra = block - self->curr;

        pax_guard((((usize) block) & (align - 1)) == 0,
            "The result is not aligned properly");

        if ( count > (avail - extra) / width )
            return ALLOC_ERROR_OUT_OF_MEMORY;

        byte* pntr = block;

        for ( ; pntr < block + total; pntr += 1 )
            *pntr = 0;

        self->curr   = pntr;
        value->block = block;

        return ALLOC_ERROR_NONE;
    }

    void
    bump_release(Bump* self, Alloc_Value value)
    {
        pax_guard(self != 0, "`self` is null");
    }

    void
    bump_clear(Bump* self)
    {
        pax_guard(self != 0, "`self` is null");

        self->curr = self->head;
    }

    Alloc
    bump_alloc(Bump* self)
    {
        auto alloc = alloc_empty();

        alloc.context = self;

        alloc.func_request = &_bump_request;
        alloc.func_release = &_bump_release;
        alloc.func_clear   = &_bump_clear;

        return alloc;
    }

    // 
    //
    //
    //
    // 
    
    Alloc_Error
    _bump_request(void* self, Alloc_Value* value)
    {
        return bump_request((Bump*) self, value);
    }

    void
    _bump_release(void* self, Alloc_Value value)
    {
        return bump_release((Bump*) self, value);
    }

    void
    _bump_clear(void* self)
    {
        return bump_clear((Bump*) self);
    }
} // namespace pax
