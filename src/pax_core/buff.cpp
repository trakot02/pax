#include <pax_core/buff.hpp>
#include <pax_core/report.hpp>
#include <pax_core/arena.hpp>
#include <pax_core/slice.hpp>

namespace pax
{
    Write_Value
    _buff_write_str8(void* buffer, Str8 value);

    Write_Value
    _buff_write_buff(void* buffer, Buff* value);

    void
    _buff_flush(void* buffer);

    Read_Value
    _buff_read_buff(void* buffer, Buff* value);

    void
    _buff_close(void* buffer);

    //
    //
    // Exposed.
    //
    //

    Buff
    buff_init_ptr(byte* block, isize count)
    {
        pax_guard(count >= 0, "`count` is negative");

        Buff buffer;

        if ( block == 0 || count == 0 )
            return buffer;

        buffer.block = block;
        buffer.count = count;
        buffer.head  = block;
        buffer.tail  = block;

        return buffer;
    }

    Arena_Error
    buff_init(Buff* buffer, Arena* arena, isize count)
    {
        pax_guard(buffer != 0, "`buffer` is null");
        pax_guard(count  >= 0, "`count` is negative");

        Alloc_Value value = {
            WIDTH_BYTE, ALIGN_BYTE, count
        };

        auto error = arena_request(arena, &value);

        if ( error != ARENA_ERROR_NONE )
            return error;

        if ( value.block != 0 ) {
            buffer->block = value.block;
            buffer->count = value.count;
            buffer->head  = value.block;
            buffer->tail  = value.block;
        }

        return ARENA_ERROR_NONE;
    }

    isize
    buff_size(const Buff* buffer)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        return buffer->tail - buffer->head;
    }

    isize
    buff_avail(const Buff* buffer)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        return (buffer->block + buffer->count) - buffer->tail;
    }

    void
    buff_shift(Buff* buffer)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        isize size = buff_size(buffer);

        for ( isize i = 0; i < size; i += 1 )
            buffer->block[i] = buffer->head[i];

        buffer->head = buffer->block;
        buffer->tail = buffer->block + size;
    }

    void
    buff_clear(Buff* buffer)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        buffer->head = buffer->block;
        buffer->tail = buffer->block;
    }

    void
    buff_fill(Buff* buffer, byte value)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        isize avail = buff_avail(buffer);

        for ( isize i = 0; i < avail; i += 1 )
            buffer->tail[i] = value;

        buffer->tail += avail;
    }

    void
    buff_fill_ptr(Buff* buffer, byte* value)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        if ( value == 0 ) return;

        isize avail = buff_avail(buffer);

        for ( isize i = 0; i < avail; i += 1 )
            buffer->tail[i] = value[i];

        buffer->tail += avail;
    }

    void
    buff_trim(Buff* buffer)
    {
        buff_trim_head(buffer);
        buff_trim_tail(buffer);
    }

    void
    buff_trim_head(Buff* buffer)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        auto* pntr = buffer->head;

        while ( pntr < buffer->tail ) {
            if ( (*pntr < 0x09 || *pntr > 0x0d) && *pntr != 0x20 )
                break;

            pntr += 1;
        }

        buffer->head = pntr;
    }

    void
    buff_trim_tail(Buff* buffer)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        byte* pntr = buffer->tail;

        do {
            pntr -= 1;

            if ( (*pntr < 0x09 || *pntr > 0x0d) && *pntr != 0x20 )
                break;
        } while ( pntr >= buffer->head );

        buffer->tail = pntr + 1;
    }

    Write
    buff_write(Buff* buffer)
    {
        Write write;

        write.self = buffer;

        write.func_str8  = &_buff_write_str8;
        write.func_buff  = &_buff_write_buff;
        write.func_flush = &_buff_flush;
        write.func_close = &_buff_close;

        return write;
    }

    Read
    buff_read(Buff* buffer)
    {
        Read read;

        read.self = buffer;

        read.func_buff  = &_buff_read_buff;
        read.func_close = &_buff_close;

        return read;
    }

    Write_Value
    buff_write_str8(Buff* buffer, Str8 value)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        isize avail = buff_avail(buffer);
        isize count = value.count;

        if ( count > avail )
            return {0, WRITE_ERROR_OVERFLOW};

        for ( isize i = 0; i < count; i += 1 )
            buffer->tail[i] = value[i];

        buffer->tail += count;

        return {count, WRITE_ERROR_NONE};
    }

    Write_Value
    buff_write_buff(Buff* buffer, Buff* value)
    {
        pax_guard(buffer != 0, "`buffer` is null");
        pax_guard(value  != 0, "`value` is null");

        isize avail = buff_avail(buffer);
        isize count = buff_size(value);

        if ( count > avail )
            return  {0, WRITE_ERROR_OVERFLOW};

        for ( isize i = 0; i < count; i += 1 )
            buffer->tail[i] = value->head[i];

        value->tail = value->block;
        value->head = value->block;

        buffer->tail += count;

        return {count, WRITE_ERROR_NONE};
    }

    Read_Value
    buff_read_buff(Buff* buffer, Buff* value)
    {
        pax_guard(buffer != 0, "`buffer` is null");

        isize avail = buff_avail(value);
        isize count = buff_size(buffer);

        if ( count > avail )
            return {0, READ_ERROR_OVERFLOW};

        for ( isize i = 0; i < count; i += 1 )
            value->tail[i] = buffer->head[i];

        value->tail += count;

        buffer->head = buffer->block;
        buffer->tail = buffer->block;

        return {count, READ_ERROR_NONE};
    }

    //
    //
    // Not exposed.
    //
    //

    Write_Value
    _buff_write_str8(void* buffer, Str8 value)
    {
        return buff_write_str8((Buff*) buffer, value);
    }

    Write_Value
    _buff_write_buff(void* buffer, Buff* value)
    {
        return buff_write_buff((Buff*) buffer, value);
    }

    void
    _buff_flush(void* buffer)
    {
        pax_guard(buffer != 0, "`buffer` is null");
    }

    Read_Value
    _buff_read_buff(void* buffer, Buff* value)
    {
        return buff_read_buff((Buff*) buffer, value);
    }

    void
    _buff_close(void* buffer)
    {
        pax_guard(buffer != 0, "`buffer` is null");
    }
} // namespace pax
