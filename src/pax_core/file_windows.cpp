#include <pax_core/file_windows.hpp>
#include <pax_core/buff.hpp>

#include <windows.h>
#include <fileapi.h>
#include <handleapi.h>

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
        "stdout", GetStdHandle(STD_OUTPUT_HANDLE),
    };

    File STDERR = {
        "stderr", GetStdHandle(STD_ERROR_HANDLE),
    };

    File STDIN = {
        "stdin", GetStdHandle(STD_INPUT_HANDLE),
    };

    File_Err
    file_open(File* file, s8 name)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self = *file;

        pax_guard(self.handle == 0, "The file is already open");

        void* handle = CreateFileA(name.addr, GENERIC_READ,
            0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

        if ( handle != INVALID_HANDLE_VALUE ) {
            self.name   = name;
            self.handle = handle;

            return FILE_ERR_NONE;
        }

        return (File_Err) GetLastError();
    }

    File_Err
    file_create(File* file, s8 name)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self = *file;

        pax_guard(self.handle == 0, "The file is already open");

        void* handle = CreateFileA(name.addr, GENERIC_WRITE,
            0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

        if ( handle != INVALID_HANDLE_VALUE ) {
            self.name   = name;
            self.handle = handle;

            return FILE_ERR_NONE;
        }

        return (File_Err) GetLastError();
    }

    void
    file_close(File* file)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self = *file;

        if ( self.handle == 0 ) return;

        auto res = CloseHandle(self.handle);

        pax_guard(res != 0, "The operation failed");

        self.name   = "";
        self.handle = 0;
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
        isize count = 0;

        pax_guard(self.handle != 0, "The file is closed");

        auto res = WriteFile(self.handle, &value,
            (DWORD) avail, (LPDWORD) &count, 0);

        if ( res == 0 )
            return {count, WRITE_ERR_SYSTEM, GetLastError()};

        return {count, WRITE_ERR_NONE};
    }

    Write_Res
    file_write_s8(File* file, s8 value)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self  = *file;
        isize avail = value.size;
        isize count = 0;

        pax_guard(self.handle != 0, "The file is closed");

        auto res = WriteFile(self.handle, value.addr,
            (DWORD) avail, (LPDWORD) &count, 0);

        if ( res == 0 )
            return {count, WRITE_ERR_SYSTEM, GetLastError()};

        return {count, WRITE_ERR_NONE};
    }

    Write_Res
    file_write_buff(File* file, Buff* value)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self  = *file;
        isize avail = buff_size(value);
        isize count = 0;

        pax_guard(self.handle != 0, "The file is closed");

        auto res = WriteFile(self.handle, value->head,
            (DWORD) avail, (LPDWORD) &count, 0);

        if ( res == 0 )
            return {count, WRITE_ERR_SYSTEM, GetLastError()};

        value->curr = value->head;

        return {count, WRITE_ERR_NONE};
    }

    Read_Res
    file_read_byte(File* file, byte* value)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self  = *file;
        isize avail = 1;
        isize count = 0;

        auto res = ReadFile(self.handle, value,
            (DWORD) avail, (LPDWORD) &count, 0);

        if ( res == 0 )
            return {count, READ_ERR_SYSTEM, GetLastError()};

        return {count, READ_ERR_NONE};
    }

    Read_Res
    file_read_buff(File* file, Buff* value)
    {
        pax_guard(file != 0, "`file` is null");

        auto& self  = *file;
        isize avail = buff_avail(value);
        isize count = 0;

        auto res = ReadFile(self.handle, value->curr,
            (DWORD) avail, (LPDWORD) &count, 0);

        if ( res == 0 )
            return {count, READ_ERR_SYSTEM, GetLastError()};

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
