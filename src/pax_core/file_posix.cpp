#include <pax_core/file_posix.hpp>
#include <pax_core/buff.hpp>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

namespace pax
{
    Write_Value
    _file_write_s8(void* file, s8 value);

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

    File STDOUT = {
        "stdout", STDOUT_FILENO,
    };

    File STDERR = {
        "stderr", STDERR_FILENO,
    };

    File STDIN = {
        "stdin", STDIN_FILENO,
    };

    File_Error
    file_open(File* file, s8 name)
    {
        pax_trace();
        pax_guard(file != 0, "`file` is null");

        auto& self = *file;

        pax_guard(self.handle < 0, "The file is already open");

        isize handle = open(name.addr, O_RDONLY);

        if ( handle >= 0 ) {
            self.name   = name;
            self.handle = handle;

            return FILE_ERROR_NONE;
        }

        return (File_Error) errno;
    }

    File_Error
    file_create(File* file, s8 name)
    {
        pax_trace();
        pax_guard(file != 0, "`file` is null");

        auto& self = *file;

        pax_guard(self.handle < 0, "The file is already open");

        isize handle = open(name.addr, O_WRONLY | O_CREAT | O_TRUNC,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

        if ( handle >= 0 ) {
            self.name   = name;
            self.handle = handle;

            return FILE_ERROR_NONE;
        }

        return (File_Error) errno;
    }

    void
    file_close(File* file)
    {
        pax_trace();
        pax_guard(file != 0, "`file` is null");

        auto& self = *file;

        if ( self.handle < 0 ) return;

        auto code = close(self.handle);

        pax_guard(code >= 0, "The operation failed");

        self.name   = "";
        self.handle = -1;
    }

    Write
    file_write(File* file)
    {
        Write self;

        self.s8_func    = &_file_write_s8;
        self.buff_func  = &_file_write_buff;
        self.flush_func = &_file_flush;
        self.close_func = &_file_close;
        self.self       = file;

        return self;
    }

    Read
    file_read(File* file)
    {
        Read self;

        self.buff_func  = &_file_read_buff;
        self.close_func = &_file_close;
        self.self       = file;

        return self;
    }

    Write_Value
    file_write_s8(File* file, s8 value)
    {
        pax_trace();
        pax_guard(file != 0, "`file` is null");

        auto& self  = *file;
        isize avail = value.size;
        isize count = value.size;
        auto* addr  = value.addr;

        pax_guard(self.handle >= 0, "The file is closed");

        do {
            auto code = write(self.handle, addr, count);

            if ( code < 0 )
                return {avail - count, WRITE_ERROR_SYSTEM, errno};

            count -= code;
            addr  += code;
        } while ( count > 0 );

        return {avail, WRITE_ERROR_NONE};
    }

    Write_Value
    file_write_buff(File* file, Buff* value)
    {
        pax_trace();
        pax_guard(file  != 0, "`file` is null");
        pax_guard(value != 0, "`value` is null");

        auto& self  = *file;
        isize avail = buff_avail(value);
        isize count = avail;
        byte* addr  = value->head;

        pax_guard(self.handle >= 0, "The file is closed");

        do {
            auto code = write(self.handle, addr, count);

            if ( code < 0 )
                return {avail - count, WRITE_ERROR_SYSTEM, errno};

            count -= code;
            addr  += code;
        } while ( count > 0 );

        value->head = value->addr;
        value->tail = value->addr;

        return {avail, WRITE_ERROR_NONE};
    }

    Read_Value
    file_read_buff(File* file, Buff* value)
    {
        pax_trace();
        pax_guard(file  != 0, "`file` is null");
        pax_guard(value != 0, "`value` is null");

        auto& self  = *file;
        isize avail = buff_avail(value);
        isize count = avail;
        byte* addr  = value->tail;

        do {
            auto code = read(self.handle, addr, count);

            if ( code < 0 )
                return {avail - count, READ_ERROR_SYSTEM, errno};

            if ( code == 0 ) break;

            count -= code;
            addr  += code;
        } while ( count > 0 );

        value->tail += avail;

        return {avail, READ_ERROR_NONE};
    }

    //
    //
    // Not exposed.
    //
    //

    Write_Value
    _file_write_s8(void* file, s8 value)
    {
        return file_write_s8((File*) file, value);
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
