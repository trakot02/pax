#include <pax_core/file_posix.hpp>
#include <pax_core/buff.hpp>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

namespace pax
{
    Write_Value
    _file_write_str8(void* file, Str8 value);

    Write_Value
    _file_write_buff(void* file, Buff* value);

    void
    _file_flush(void* file);

    Read_Value
    _file_read_buff(void* file, Buff* value);

    void
    _file_close(void* file);

    //
    //
    // Implementation.
    //
    //

    File STDOUT = {"stdout", STDOUT_FILENO};
    File STDERR = {"stderr", STDERR_FILENO};
    File STDIN  = {"stdin", STDIN_FILENO};

    File_Error
    file_open(File* file, Str8 name)
    {
        pax_guard(file != 0, "`file` is null");

        isize handle = file->handle;
        isize mode   = O_RDONLY;

        pax_guard(handle < 0, "The file is already open");

        handle = open(name.block, mode);

        if ( handle >= 0 ) {
            file->name   = name;
            file->handle = handle;

            return FILE_ERROR_NONE;
        }

        return (File_Error) errno;
    }

    File_Error
    file_create(File* file, Str8 name)
    {
        pax_guard(file != 0, "`file` is null");

        isize handle = file->handle;
        isize mode   = O_WRONLY | O_CREAT | O_TRUNC;
        isize perm   = S_IWUSR  | S_IRUSR | S_IRGRP | S_IROTH;

        pax_guard(handle < 0, "The file is already open");

        handle = open(name.block, mode, perm);

        if ( handle >= 0 ) {
            file->name   = name;
            file->handle = handle;

            return FILE_ERROR_NONE;
        }

        return (File_Error) errno;
    }

    void
    file_close(File* file)
    {
        pax_guard(file != 0, "`file` is null");

        if ( file->handle < 0 ) return;

        auto code = close(file->handle);

        pax_guard(code >= 0, "The operation failed");

        file->name   = "";
        file->handle = -1;
    }

    Write
    file_write(File* file)
    {
        Write write;

        write.self = file;

        write.func_str8  = &_file_write_str8;
        write.func_buff  = &_file_write_buff;
        write.func_flush = &_file_flush;
        write.func_close = &_file_close;

        return write;
    }

    Read
    file_read(File* file)
    {
        Read read;

        read.self = file;

        read.func_buff  = &_file_read_buff;
        read.func_close = &_file_close;

        return read;
    }

    Write_Value
    file_write_str8(File* file, Str8 value)
    {
        pax_guard(file != 0, "`file` is null");

        isize total = value.count;
        isize count = 0;
        isize delta = value.count;

        pax_guard(file->handle >= 0, "The file is already closed");

        const byte* pntr = value.block;

        while ( count < total ) {
            auto code = write(file->handle, pntr, delta);

            if ( code < 0 )
                return {count, WRITE_ERROR_SYSTEM, errno};

            count += code;
            delta -= code;
            pntr  += code;
        }

        return {count, WRITE_ERROR_NONE};
    }

    Write_Value
    file_write_buff(File* file, Buff* value)
    {
        pax_guard(file  != 0, "`file` is null");
        pax_guard(value != 0, "`value` is null");

        isize total = buff_avail(value);
        isize count = 0;
        isize delta = total;

        pax_guard(file->handle >= 0, "The file is already closed");

        byte* pntr = value->head;

        while ( count < total ) {
            auto code = write(file->handle, pntr, delta);

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

    Read_Value
    file_read_buff(File* file, Buff* value)
    {
        pax_guard(file  != 0, "`file` is null");
        pax_guard(value != 0, "`value` is null");

        isize total = buff_avail(value);
        isize count = 0;
        isize delta = total;

        pax_guard(file->handle >= 0, "The file is already closed");

        byte* pntr = value->tail;

        while ( count < total ) {
            auto code = read(file->handle, pntr, delta);

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

    //
    //
    // Not exposed.
    //
    //

    Write_Value
    _file_write_str8(void* file, Str8 value)
    {
        return file_write_str8((File*) file, value);
    }

    Write_Value
    _file_write_buff(void* file, Buff* value)
    {
        return file_write_buff((File*) file, value);
    }

    void
    _file_flush(void* file)
    {
        pax_guard(file != 0, "`file` is null");
    }

    Read_Value
    _file_read_buff(void* file, Buff* value)
    {
        return file_read_buff((File*) file, value);
    }

    void
    _file_close(void* file)
    {
        file_close((File*) file);
    }
} // namespace pax
