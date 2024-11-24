#include <pax_core/file_posix.hpp>
#include <pax_core/buff.hpp>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

namespace pax
{
    Write_Value
    _file_write_str8(void* self, Str8 value);

    Write_Value
    _file_write_buff(void* self, Buff* value);

    void
    _file_flush(void* self);

    Read_Value
    _file_read_buff(void* self, Buff* value);

    void
    _file_close(void* self);

    //
    //
    // Exposed.
    //
    //

    File STDOUT = {STDOUT_FILENO};
    File STDERR = {STDERR_FILENO};
    File STDIN  = {STDIN_FILENO};

    File
    file_empty()
    {
        File file;

        file.handle = -1;

        return file;
    }

    File_Error
    file_open(File* self, Str8 name)
    {
        pax_guard(self != 0, "`self` is null");

        isize handle = self->handle;
        isize mode   = O_RDONLY;

        pax_guard(handle < 0, "The file is already open");

        handle = open(name.block, mode);

        if ( handle < 0 )
            return (File_Error) errno;

        self->handle = handle;

        return FILE_ERROR_NONE;
    }

    File_Error
    file_create(File* self, Str8 name)
    {
        pax_guard(self != 0, "`self` is null");

        isize handle = self->handle;
        isize mode   = O_WRONLY | O_CREAT | O_TRUNC;
        isize perm   = S_IWUSR  | S_IRUSR | S_IRGRP | S_IROTH;

        pax_guard(handle < 0, "The file is already open");

        handle = open(name.block, mode, perm);

        if ( handle < 0 )
            return (File_Error) errno;

        self->handle = handle;

        return FILE_ERROR_NONE;
    }

    void
    file_close(File* self)
    {
        pax_guard(self != 0, "`self` is null");

        if ( self->handle < 0 ) return;

        auto code = close(self->handle);

        pax_guard(code >= 0, "The operation failed");

        self->handle = -1;
    }

    Write_Value
    file_write_str8(File* self, Str8 value)
    {
        pax_guard(self != 0, "`self` is null");

        isize total = value.count;
        isize count = 0;
        isize delta = value.count;

        pax_guard(self->handle >= 0, "The file is already closed");

        const byte* pntr = value.block;

        while ( count < total ) {
            auto code = write(self->handle, pntr, delta);

            if ( code < 0 )
                return {count, WRITE_ERROR_SYSTEM, errno};

            count += code;
            delta -= code;
            pntr  += code;
        }

        return {count, WRITE_ERROR_NONE};
    }

    Write_Value
    file_write_buff(File* self, Buff* value)
    {
        pax_guard(self  != 0, "`self` is null");
        pax_guard(value != 0, "`value` is null");

        isize total = buff_avail(value);
        isize count = 0;
        isize delta = total;

        pax_guard(self->handle >= 0, "The file is already closed");

        byte* pntr = value->head;

        while ( count < total ) {
            auto code = write(self->handle, pntr, delta);

            if ( code < 0 ) {
                value->head += count;

                return {count, WRITE_ERROR_SYSTEM, errno};
            }

            count += code;
            delta -= code;
            pntr  += code;
        }

        value->head = value->block;
        value->tail = value->block;

        return {count, WRITE_ERROR_NONE};
    }

    Write
    file_write(File* self)
    {
        Write write;

        write.context = self;

        write.func_str8  = &_file_write_str8;
        write.func_buff  = &_file_write_buff;
        write.func_flush = &_file_flush;
        write.func_close = &_file_close;

        return write;
    }

    Read_Value
    file_read_buff(File* self, Buff* value)
    {
        pax_guard(self  != 0, "`self` is null");
        pax_guard(value != 0, "`value` is null");

        isize total = buff_avail(value);
        isize count = 0;
        isize delta = total;

        pax_guard(self->handle >= 0, "The file is already closed");

        byte* pntr = value->tail;

        while ( count < total ) {
            auto code = read(self->handle, pntr, delta);

            if ( code == 0 ) break;

            if ( code < 0 )
                return {0, READ_ERROR_SYSTEM, errno};

            count += code;
            delta -= code;
            pntr  += code;
        }

        value->tail += count;

        return {count, READ_ERROR_NONE};
    }

    Read
    file_read(File* self)
    {
        Read read;

        read.context = self;

        read.func_buff  = &_file_read_buff;
        read.func_close = &_file_close;

        return read;
    }

    //
    //
    // Not exposed.
    //
    //

    Write_Value
    _file_write_str8(void* self, Str8 value)
    {
        return file_write_str8((File*) self, value);
    }

    Write_Value
    _file_write_buff(void* self, Buff* value)
    {
        return file_write_buff((File*) self, value);
    }

    void
    _file_flush(void* self)
    {
        pax_guard(self != 0, "`self` is null");
    }

    Read_Value
    _file_read_buff(void* self, Buff* value)
    {
        return file_read_buff((File*) self, value);
    }

    void
    _file_close(void* self)
    {
        file_close((File*) self);
    }
} // namespace pax
