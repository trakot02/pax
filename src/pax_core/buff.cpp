#include <pax_core/buff.hpp>
#include <pax_core/report.hpp>
#include <pax_core/arena.hpp>
#include <pax_core/slice.hpp>

namespace pax
{
    Write_Value
    _buff_write_str8(void* buf, Str8 value);

    Write_Value
    _buff_write_buff(void* buf, Buff* value);

    void
    _buff_flush(void* buf);

    Read_Value
    _buff_read_buff(void* buf, Buff* value);

    void
    _buff_close(void* buf);

    //
    //
    // Exposed.
    //
    //

    Buff
    buff_from(byte* ptr, isize cnt)
    {
        pax_guard(cnt >= 0, "`cnt` is negative");

        Buff buf;

        if ( ptr == 0 || cnt == 0 )
            return buf;

        buf.ptr  = ptr;
        buf.cnt  = cnt;
        buf.head = ptr;
        buf.tail = ptr;

        return buf;
    }

    Arena_Error
    buff_init(Buff* buf, Arena* arena, isize cnt)
    {
        pax_guard(buf != 0, "`buf` is null");
        pax_guard(cnt >= 0, "`cnt` is negative");

        isize width = WIDTH_BYTE;
        isize align = ALIGN_BYTE;

        Alloc_Value value = {width, align, cnt};

        auto error = arena_request(arena, &value);

        if ( error != ARENA_ERROR_NONE )
            return error;

        if ( value.ptr != 0 ) {
            buf->ptr  = value.ptr;
            buf->cnt  = value.cnt;
            buf->head = value.ptr;
            buf->tail = value.ptr;
        }

        return ARENA_ERROR_NONE;
    }

    isize
    buff_size(const Buff* buf)
    {
        pax_guard(buf != 0, "`buf` is null");

        return buf->tail - buf->head;
    }

    isize
    buff_avail(const Buff* buf)
    {
        pax_guard(buf != 0, "`buf` is null");

        return (buf->ptr + buf->cnt) - buf->tail;
    }

    void
    buff_shift(Buff* buf)
    {
        pax_guard(buf != 0, "`buf` is null");

        isize size = buff_size(buf);

        for ( isize i = 0; i < size; i += 1 )
            buf->ptr[i] = buf->head[i];

        buf->head = buf->ptr;
        buf->tail = buf->ptr + size;
    }

    void
    buff_clear(Buff* buf)
    {
        pax_guard(buf != 0, "`buf` is null");

        buf->head = buf->ptr;
        buf->tail = buf->ptr;
    }

    void
    buff_fill(Buff* buf, byte value)
    {
        pax_guard(buf != 0, "`buf` is null");

        isize avail = buff_avail(buf);

        for ( isize i = 0; i < avail; i += 1 )
            buf->tail[i] = value;

        buf->tail += avail;
    }

    void
    buff_fill_ptr(Buff* buf, byte* value)
    {
        pax_guard(buf != 0, "`buf` is null");

        if ( value == 0 ) return;

        isize avail = buff_avail(buf);

        for ( isize i = 0; i < avail; i += 1 )
            buf->tail[i] = value[i];

        buf->tail += avail;
    }

    void
    buff_trim(Buff* buf)
    {
        buff_trim_head(buf);
        buff_trim_tail(buf);
    }

    void
    buff_trim_head(Buff* buf)
    {
        pax_guard(buf != 0, "`buf` is null");

        auto* ptr = buf->head;

        while ( ptr < buf->tail ) {
            byte val = ptr[0];

            if ( val != '\t' && val != '\n' &&
                 val != '\v' && val != '\f' &&
                 val != '\r' && val != ' ' )
                break;

            ptr += 1;
        }

        buf->head = ptr;
    }

    void
    buff_trim_tail(Buff* buf)
    {
        pax_guard(buf != 0, "`buf` is null");

        byte* ptr = buf->tail - 1;

        while ( ptr >= buf->head ) {
            byte val = ptr[0];

            if ( val != '\t' && val != '\n' &&
                 val != '\v' && val != '\f' &&
                 val != '\r' && val != ' ' )
                break;

            ptr -= 1;
        }

        buf->tail = ptr + 1;
    }

    Write
    buff_write(Buff* buf)
    {
        Write write;

        write.self = buf;

        write.func_str8  = &_buff_write_str8;
        write.func_buff  = &_buff_write_buff;
        write.func_flush = &_buff_flush;
        write.func_close = &_buff_close;

        return write;
    }

    Read
    buff_read(Buff* buf)
    {
        Read read;

        read.self = buf;

        read.func_buff  = &_buff_read_buff;
        read.func_close = &_buff_close;

        return read;
    }

    Write_Value
    buff_write_str8(Buff* buf, Str8 value)
    {
        pax_guard(buf != 0, "`buf` is null");

        isize avail = buff_avail(buf);
        isize count = value.cnt;

        if ( count > avail )
            return {avail, WRITE_ERROR_OVERFLOW};

        for ( isize i = 0; i < count; i += 1 )
            buf->tail[i] = value[i];

        buf->tail += count;

        return {count, WRITE_ERROR_NONE};
    }

    Write_Value
    buff_write_buff(Buff* buf, Buff* value)
    {
        pax_guard(buf   != 0, "`buf` is null");
        pax_guard(value != 0, "`value` is null");

        isize avail = buff_avail(buf);
        isize count = buff_size(value);

        if ( count > avail )
            return {avail, WRITE_ERROR_OVERFLOW};

        for ( isize i = 0; i < count; i += 1 )
            buf->tail[i] = value->head[i];

        value->tail = value->ptr;
        value->head = value->ptr;

        buf->tail += count;

        return {count, WRITE_ERROR_NONE};
    }

    Read_Value
    buff_read_buff(Buff* buf, Buff* value)
    {
        pax_guard(buf != 0, "`buf` is null");

        isize avail = buff_avail(value);
        isize count = buff_size(buf);

        if ( count > avail )
            return {avail, READ_ERROR_OVERFLOW};

        for ( isize i = 0; i < count; i += 1 )
            value->tail[i] = buf->head[i];

        value->tail += count;

        buf->head = buf->ptr;
        buf->tail = buf->ptr;

        return {count, READ_ERROR_NONE};
    }

    //
    //
    // Not exposed.
    //
    //

    Write_Value
    _buff_write_str8(void* buf, Str8 value)
    {
        return buff_write_str8((Buff*) buf, value);
    }

    Write_Value
    _buff_write_buff(void* buf, Buff* value)
    {
        return buff_write_buff((Buff*) buf, value);
    }

    void
    _buff_flush(void* buf)
    {
        pax_guard(buf != 0, "`buf` is null");
    }

    Read_Value
    _buff_read_buff(void* buf, Buff* value)
    {
        return buff_read_buff((Buff*) buf, value);
    }

    void
    _buff_close(void* buf)
    {
        pax_guard(buf != 0, "`buf` is null");
    }
} // namespace pax
