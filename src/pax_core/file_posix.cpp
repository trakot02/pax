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

        handle = open(name.ptr, mode);

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

        handle = open(name.ptr, mode, perm); 

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

        isize avail = value.cnt;
        isize cnt   = value.cnt;
        auto* ptr   = value.ptr;

        pax_guard(file->handle >= 0, "The file is closed");

        do {
            auto code = write(file->handle, ptr, cnt);

            if ( code < 0 )
                return {avail - cnt, WRITE_ERROR_SYSTEM, errno};

            cnt -= code;
            ptr += code;
        } while ( cnt > 0 );

        return {avail, WRITE_ERROR_NONE};
    }

    Write_Value
    file_write_buff(File* file, Buff* value)
    {
        pax_guard(file  != 0, "`file` is null");
        pax_guard(value != 0, "`value` is null");

        isize avail = buff_avail(value);
        isize cnt   = avail;
        byte* ptr   = value->head;

        pax_guard(file->handle >= 0, "The file is closed");

        do {
            auto code = write(file->handle, ptr, cnt);

            if ( code < 0 )
                return {avail - cnt, WRITE_ERROR_SYSTEM, errno};

            cnt -= code;
            ptr += code;
        } while ( cnt > 0 );

        value->head = value->ptr;
        value->tail = value->ptr;

        return {avail, WRITE_ERROR_NONE};
    }

    Read_Value
    file_read_buff(File* file, Buff* value)
    {
        pax_guard(file  != 0, "`file` is null");
        pax_guard(value != 0, "`value` is null");

        isize avail = buff_avail(value);
        isize cnt   = avail;
        byte* ptr   = value->tail;

        pax_guard(file->handle >= 0, "The file is closed");

        do {
            auto code = read(file->handle, ptr, cnt);

            if ( code < 0 )
                return {avail - cnt, READ_ERROR_SYSTEM, errno};

            if ( code == 0 ) break;

            cnt -= code;
            ptr += code;
        } while ( cnt > 0 );

        value->tail += avail - cnt;

        return {avail - cnt, READ_ERROR_NONE};
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
