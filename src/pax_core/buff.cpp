#include <pax_core/buff.hpp>
#include <pax_core/report.hpp>
#include <pax_core/slice.hpp>

namespace pax
{
    Write_Value
    _buff_write_str8(void* self, Str8 value);

    Write_Value
    _buff_write_buff(void* self, Buff* value);

    void
    _buff_flush(void* self);

    Read_Value
    _buff_read_buff(void* self, Buff* value);

    void
    _buff_close(void* self);

    //
    //
    // Exposed.
    //
    //

    Buff
    buff_empty()
    {
        Buff buff;

        buff.block = 0;
        buff.count = 0;
        buff.head  = 0;
        buff.tail  = 0;

        return buff;
    }

    Buff
    buff_from(byte* block, isize count)
    {
        pax_guard(count >= 0, "`count` is negative");

        auto buff = buff_empty();

        if ( block == 0 || count == 0 )
            return buff;

        buff.block = block;
        buff.count = count;
        buff.head  = block;
        buff.tail  = block;

        return buff;
    }

    Alloc_Error
    buff_init(Buff* self, Alloc alloc, isize count)
    {
        pax_guard(self  != 0, "`self` is null");
        pax_guard(count >= 0, "`count` is negative");

        auto value = alloc_value_from(WIDTH_BYTE,
            ALIGN_BYTE, count);

        auto error = alloc_request(alloc, &value);

        if ( error != ALLOC_ERROR_NONE )
            return error;

        self->block = value.block;
        self->count = value.count;
        self->head  = value.block;
        self->tail  = value.block;

        return ALLOC_ERROR_NONE;
    }

    isize
    buff_count(const Buff* self)
    {
        if ( self != 0 )
            return self->tail - self->head;

        return 0;
    }

    isize
    buff_avail(const Buff* self)
    {
        if ( self != 0 )
            return (self->block + self->count) - self->tail;

        return 0;
    }

    isize
    buff_size(const Buff* self)
    {
        if ( self != 0 )
            return self->count;

        return 0;
    }

    void
    buff_shift(Buff* self)
    {
        pax_guard(self != 0, "`self` is null");

        byte* tail = self->tail;
        byte* pntr = self->head;
        byte* dest = self->block;

        for ( ; pntr < tail; pntr += 1, dest += 1 )
            *dest = *pntr;

        self->head = self->block;
        self->tail = dest;
    }

    void
    buff_clear(Buff* self)
    {
        pax_guard(self != 0, "`self` is null");

        self->head = self->block;
        self->tail = self->block;
    }

    void
    buff_fill(Buff* self, byte value)
    {
        pax_guard(self != 0, "`self` is null");

        byte* pntr = self->tail;
        byte* tail = self->block + self->count;

        for ( ; pntr < tail; pntr += 1 )
            *pntr = value;

        self->tail = pntr;
    }

    void
    buff_fill_ptr(Buff* self, byte* value)
    {
        pax_guard(self != 0, "`self` is null");

        if ( value == 0 ) return;

        byte* pntr = self->tail;
        byte* tail = self->block + self->count;

        for ( ; pntr < tail; pntr += 1, value += 1 )
            *pntr = *value;

        self->tail = pntr;
    }

    void
    buff_trim(Buff* self)
    {
        buff_trim_head(self);
        buff_trim_tail(self);
    }

    void
    buff_trim_head(Buff* self)
    {
        pax_guard(self != 0, "`self` is null");

        auto* pntr = self->head;

        while ( pntr < self->tail ) {
            if ( (*pntr < 0x09 || *pntr > 0x0d) && *pntr != 0x20 )
                break;

            pntr += 1;
        }

        self->head = pntr;
    }

    void
    buff_trim_tail(Buff* self)
    {
        pax_guard(self != 0, "`self` is null");

        byte* pntr = self->tail;

        do {
            pntr -= 1;

            if ( (*pntr < 0x09 || *pntr > 0x0d) && *pntr != 0x20 )
                break;
        } while ( pntr >= self->head );

        self->tail = pntr + 1;
    }

    Write_Value
    buff_write_str8(Buff* self, Str8 value)
    {
        pax_guard(self != 0, "`self` is null");

        byte* dest = self->tail;
        byte* tail = self->block + self->count;

        const byte* pntr = value.block;

        if ( dest + value.count >= tail )
            return {0, WRITE_ERROR_OVERFLOW};

        for ( ; dest < tail; dest += 1, pntr += 1 )
            *dest = *pntr;

        self->tail = dest;

        return {value.count, WRITE_ERROR_NONE};
    }

    Write_Value
    buff_write_buff(Buff* self, Buff* value)
    {
        pax_guard(self  != 0, "`self` is null");
        pax_guard(value != 0, "`value` is null");

        isize count = buff_count(value);

        byte* dest = self->tail;
        byte* tail = self->block + self->count;
        byte* pntr = value->head;

        if ( dest + count >= tail )
            return {0, WRITE_ERROR_OVERFLOW};

        tail = self->block + count;

        for ( ; dest < tail; dest += 1, pntr += 1 )
            *dest = *pntr;

        value->tail = value->block;
        value->head = value->block;

        self->tail = dest;

        return {count, WRITE_ERROR_NONE};
    }

    Write
    buff_write(Buff* self)
    {
        Write write;

        write.context = self;

        write.func_str8  = &_buff_write_str8;
        write.func_buff  = &_buff_write_buff;
        write.func_flush = &_buff_flush;
        write.func_close = &_buff_close;

        return write;
    }

    Read_Value
    buff_read_buff(Buff* self, Buff* value)
    {
        pax_guard(self != 0, "`self` is null");

        isize count = buff_count(self);

        byte* dest = value->tail;
        byte* tail = value->block + value->count;
        byte* pntr = self->head;

        if ( dest + count >= tail )
            return {0, READ_ERROR_OVERFLOW};

        tail = value->block + count;

        for ( ; dest < tail; dest += 1, pntr += 1 )
            *dest = *pntr;

        value->tail = dest;

        self->head = self->block;
        self->tail = self->block;

        return {count, READ_ERROR_NONE};
    }

    Read
    buff_read(Buff* self)
    {
        Read read;

        read.context = self;

        read.func_buff  = &_buff_read_buff;
        read.func_close = &_buff_close;

        return read;
    }

    //
    //
    // Not exposed.
    //
    //

    Write_Value
    _buff_write_str8(void* self, Str8 value)
    {
        return buff_write_str8((Buff*) self, value);
    }

    Write_Value
    _buff_write_buff(void* self, Buff* value)
    {
        return buff_write_buff((Buff*) self, value);
    }

    void
    _buff_flush(void* self)
    {
        pax_guard(self != 0, "`self` is null");
    }

    Read_Value
    _buff_read_buff(void* self, Buff* value)
    {
        return buff_read_buff((Buff*) self, value);
    }

    void
    _buff_close(void* self)
    {
        pax_guard(self != 0, "`self` is null");
    }
} // namespace pax
