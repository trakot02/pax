#include <pax_core/buff.hpp>
#include <pax_core/report.hpp>
#include <pax_core/arena.hpp>
#include <pax_core/slice.hpp>

namespace pax
{
    Write_Value
    _buff_write_s8(void* buff, s8 value);

    Write_Value
    _buff_write_buff(void* buff, Buff* value);

    void
    _buff_flush(void* buff);

    Read_Value
    _buff_read_buff(void* buff, Buff* value);

    void
    _buff_close(void* buff);

    //
    //
    // Implementation.
    //
    //

    Buff
    buff_from(byte* addr, isize size)
    {
        pax_trace();
        pax_guard(size >= 0, "`size` is negative");

        Buff self = {0};

        if ( addr == 0 || size == 0 )
            return self;

        self.addr = addr;
        self.size = size;
        self.head = addr;
        self.tail = addr;

        return self;
    }

    Buff
    buff_init(Arena* arena, isize size)
    {
        pax_trace();
        pax_guard(size >= 0, "`size` is negative");

        Buff self = {0};

        auto value = arena_request(arena, {
            WIDTH_BYTE, ALIGN_BYTE, size
        });

        if ( value.addr == 0 ) return self;

        self.addr = value.addr;
        self.size = size;
        self.head = value.addr;
        self.tail = value.addr;

        return self;
    }

    isize
    buff_size(const Buff* buff)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self = *buff;

        return self.tail - self.head;
    }

    isize
    buff_avail(const Buff* buff)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self = *buff;

        return (self.addr + self.size) - self.tail;
    }

    void
    buff_clear(Buff* buff)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self = *buff;

        self.head = self.addr;
        self.tail = self.addr;
    }

    void
    buff_fill(Buff* buff, byte value)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = (self.addr + self.size) - self.tail;

        for ( isize i = 0; i < avail; i += 1 )
            self.tail[i] = value;

        self.tail += avail;
    }

    void
    buff_fill_addr(Buff* buff, byte* value)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        if ( value == 0 ) return;

        auto& self  = *buff;
        isize avail = (self.addr + self.size) - self.tail;

        for ( isize i = 0; i < avail; i += 1 )
            self.tail[i] = value[i];

        self.tail += avail;
    }

    void
    buff_shift(Buff* buff)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self = *buff;
        isize size = self.tail - self.head;

        for ( isize i = 0; i < size; i += 1 )
            self.addr[i] = self.head[i];

        self.head = self.addr;
        self.tail = self.addr + size;
    }

    void
    buff_trim(Buff* buff)
    {
        buff_trim_head(buff);
        buff_trim_tail(buff);
    }

    void
    buff_trim_head(Buff* buff)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self = *buff;
        byte* addr = self.head;

        while ( addr < self.tail ) {
            byte temp = addr[0];

            if ( temp != '\t' && temp != '\n' &&
                 temp != '\v' && temp != '\f' &&
                 temp != '\r' && temp != ' ' )
                break;

            addr += 1;
        }

        self.head = addr;
    }

    void
    buff_trim_tail(Buff* buff)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self = *buff;
        byte* addr = self.tail - 1;

        while ( addr >= self.head ) {
            byte temp = addr[0];

            if ( temp != '\t' && temp != '\n' &&
                 temp != '\v' && temp != '\f' &&
                 temp != '\r' && temp != ' ' )
                break;

            addr -= 1;
        }

        self.tail = addr + 1;
    }

    Write
    buff_write(Buff* buff)
    {
        Write self;

        self.s8_func    = &_buff_write_s8;
        self.buff_func  = &_buff_write_buff;
        self.flush_func = &_buff_flush;
        self.close_func = &_buff_close;
        self.self       = buff;

        return self;
    }

    Read
    buff_read(Buff* buff)
    {
        Read self;

        self.buff_func  = &_buff_read_buff;
        self.close_func = &_buff_close;
        self.self       = buff;

        return self;
    }

    Write_Value
    buff_write_s8(Buff* buff, s8 value)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = (self.addr + self.size) - self.tail;
        isize count = value.size;

        if ( count > avail )
            return {avail, WRITE_ERROR_OVERFLOW};

        for ( isize i = 0; i < count; i += 1 )
            self.tail[i] = value[i];

        self.tail += count;

        return {count, WRITE_ERROR_NONE};
    }

    Write_Value
    buff_write_buff(Buff* buff, Buff* value)
    {
        pax_trace();
        pax_guard(buff  != 0, "`buff` is null");
        pax_guard(value != 0, "`value` is null");

        auto& self  = *buff;
        isize avail = (self.addr + self.size) - self.tail;
        isize count = value->tail - value->head;

        if ( count > avail )
            return {avail, WRITE_ERROR_OVERFLOW};

        for ( isize i = 0; i < count; i += 1 )
            self.tail[i] = value->head[i];

        value->tail = value->addr;
        value->head = value->addr;

        self.tail += count;

        return {count, WRITE_ERROR_NONE};
    }

    Read_Value
    buff_read_buff(Buff* buff, Buff* value)
    {
        pax_trace();
        pax_guard(buff != 0, "`buff` is null");

        auto& self  = *buff;
        isize avail = (value->addr + value->size) - value->tail;
        isize count = self.tail - self.head;

        if ( count > avail )
            return {avail, READ_ERROR_OVERFLOW};

        for ( isize i = 0; i < count; i += 1 )
            value->tail[i] = self.head[i];

        value->tail += count;

        self.head = self.addr;
        self.tail = self.addr;

        return {count, READ_ERROR_NONE};
    }

    //
    //
    // Not exposed.
    //
    //

    Write_Value
    _buff_write_s8(void* buff, s8 value)
    {
        return buff_write_s8((Buff*) buff, value);
    }

    Write_Value
    _buff_write_buff(void* buff, Buff* value)
    {
        return buff_write_buff((Buff*) buff, value);
    }

    void
    _buff_flush(void* buff)
    {
        pax_guard(buff != 0, "`buff` is null");
    }

    Read_Value
    _buff_read_buff(void* buff, Buff* value)
    {
        return buff_read_buff((Buff*) buff, value);
    }

    void
    _buff_close(void* buff)
    {
        pax_guard(buff != 0, "`buff` is null");
    }
} // namespace pax
