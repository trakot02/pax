#include <pax_core/file_posix.hpp>
#include <pax_core/buff.hpp>

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

namespace pax
{
    Write_Res
    _file_write_byte(void* file, byte value);

    Write_Res
    _file_write_s8(void* file, s8 value);

    Write_Res
    _file_write_buff(void* file, Buff* value);

    void
    _file_flush(void* file);

    Read_Res
    _file_read_byte(void* file, byte* value);

    Read_Res
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

    File_Err
    file_open(File* file, s8 name)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self = *file;

        pax_guard(self.handle < 0, "The file is already open");

        isize handle = open(name.addr, O_RDONLY);

        if ( handle >= 0 ) {
            self.name   = name;
            self.handle = handle;

            return FILE_ERR_NONE;
        }

        return (File_Err) errno;
    }

    File_Err
    file_create(File* file, s8 name)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self = *file;

        pax_guard(self.handle < 0, "The file is already open");

        isize handle = open(name.addr, O_WRONLY | O_CREAT | O_TRUNC,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

        if ( handle >= 0 ) {
            self.name   = name;
            self.handle = handle;

            return FILE_ERR_NONE;
        }

        return (File_Err) errno;
    }

    void
    file_close(File* file)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self = *file;

        if ( self.handle < 0 ) return;

        auto res = close(self.handle);

        pax_guard(res >= 0, "The operation failed");

        self.name   = "";
        self.handle = -1;
    }

    Write
    file_write(File* file)
    {
        Write self;

        self.byte_func  = &_file_write_byte;
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

        self.byte_func  = &_file_read_byte;
        self.buff_func  = &_file_read_buff;
        self.close_func = &_file_close;
        self.self       = file;

        return self;
    }

    Write_Res
    file_write_byte(File* file, byte value)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self  = *file;
        isize avail = 1;

        pax_guard(self.handle >= 0, "The file is closed");

        auto res = write(self.handle, &value, avail);

        if ( res < 0 )
            return {0, WRITE_ERR_SYSTEM, errno};

        return {avail, WRITE_ERR_NONE};
    }

    Write_Res
    file_write_s8(File* file, s8 value)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self  = *file;
        isize count = 0;
        isize avail = value.size;

        pax_guard(self.handle >= 0, "The file is closed");

        do {
            auto res = write(self.handle, value.addr + count,
                avail - count);

            if ( res < 0 )
                return {count, WRITE_ERR_SYSTEM, errno};

            count += res;
        } while ( count < avail );

        return {avail, WRITE_ERR_NONE};
    }

    Write_Res
    file_write_buff(File* file, Buff* value)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self  = *file;
        isize count = 0;
        isize avail = buff_size(value);

        pax_guard(self.handle >= 0, "The file is closed");

        do {
            auto res = write(self.handle, value->head + count,
                avail - count);

            if ( res < 0 )
                return {count, WRITE_ERR_SYSTEM, errno};

            count += res;
        } while ( count < avail );

        value->curr = value->head;

        return {avail, WRITE_ERR_NONE};
    }

    Read_Res
    file_read_byte(File* file, byte* value)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self  = *file;
        isize avail = 1;

        auto res = read(self.handle, value, avail);

        if ( res < 0 )
            return {0, READ_ERR_SYSTEM, errno};

        return {res, READ_ERR_NONE};
    }

    Read_Res
    file_read_buff(File* file, Buff* value)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self  = *file;
        isize avail = buff_avail(value);
        isize count = 0;

        do {
            auto res = read(self.handle, value->curr + count,
                avail - count);

            if ( res < 0 )
                return {count, READ_ERR_SYSTEM, errno};

            if ( res == 0 ) break;

            count += res;
        } while ( count < avail );

        value->curr += count;

        return {count, READ_ERR_NONE};
    }

    //
    //
    // Not exposed.
    //
    //

    Write_Res
    _file_write_byte(void* file, byte value)
    {
        return file_write_byte((File*) file, value);
    }

    Write_Res
    _file_write_s8(void* file, s8 value)
    {
        return file_write_s8((File*) file, value);
    }

    Write_Res
    _file_write_buff(void* file, Buff* value)
    {
        return file_write_buff((File*) file, value);
    }

    void
    _file_flush(void* file)
    {
        pax_guard(file != 0, "`file` is null");
    }

    Read_Res
    _file_read_byte(void* file, byte* value)
    {
        return file_read_byte((File*) file, value);
    }

    Read_Res
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
