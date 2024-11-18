#include <pax_core/file_windows.hpp>
#include <pax_core/buff.hpp>

#include <windows.h>
#include <fileapi.h>
#include <handleapi.h>

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
        "stdout", GetStdHandle(STD_OUTPUT_HANDLE),
    };

    File STDERR = {
        "stderr", GetStdHandle(STD_ERROR_HANDLE),
    };

    File STDIN = {
        "stdin", GetStdHandle(STD_INPUT_HANDLE),
    };

    File_Error
    file_open(File* file, s8 name)
    {
        pax_trace();
        pax_guard(file != 0, "`file` is null");

        auto& self = *file;

        pax_guard(self.handle == 0, "The file is already open");

        void* handle = CreateFileA(name.addr, GENERIC_READ,
            0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

        if ( handle != INVALID_HANDLE_VALUE ) {
            self.name   = name;
            self.handle = handle;

            return FILE_ERROR_NONE;
        }

        return (File_Error) GetLastError();
    }

    File_Error
    file_create(File* file, s8 name)
    {
        pax_trace();
        pax_guard(file != 0, "`file` is null");

        auto& self = *file;

        pax_guard(self.handle == 0, "The file is already open");

        void* handle = CreateFileA(name.addr, GENERIC_WRITE,
            0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

        if ( handle != INVALID_HANDLE_VALUE ) {
            self.name   = name;
            self.handle = handle;

            return FILE_ERROR_NONE;
        }

        return (File_Error) GetLastError();
    }

    void
    file_close(File* file)
    {
        pax_trace();
        pax_guard(file != 0, "`file` is null");

        auto& self = *file;

        if ( self.handle == 0 ) return;

        auto code = CloseHandle(self.handle);

        pax_guard(code != 0, "The operation failed");

        self.name   = "";
        self.handle = 0;
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
        isize count = 0;

        pax_guard(self.handle != 0, "The file is closed");

        auto code = WriteFile(self.handle, value.addr,
            (DWORD) avail, (LPDWORD) &count, 0);

        if ( code == 0 )
            return {count, WRITE_ERROR_SYSTEM, GetLastError()};

        return {count, WRITE_ERROR_NONE};
    }

    Write_Value
    file_write_buff(File* file, Buff* value)
    {
        pax_trace();
        pax_guard(file  != 0, "`file` is null");
        pax_guard(value != 0, "`value` is null");

        auto& self  = *file;
        isize avail = buff_size(value);
        isize count = 0;

        pax_guard(self.handle != 0, "The file is closed");

        auto code = WriteFile(self.handle, value->head,
            (DWORD) avail, (LPDWORD) &count, 0);

        if ( code == 0 )
            return {count, WRITE_ERROR_SYSTEM, GetLastError()};

        value->head = value->addr;
        value->tail = value->addr;

        return {count, WRITE_ERROR_NONE};
    }

    Read_Value
    file_read_buff(File* file, Buff* value)
    {
        pax_trace();
        pax_guard(file != 0, "`file` is null");
        pax_guard(value != 0, "`value` is null");

        auto& self  = *file;
        isize avail = buff_avail(value);
        isize count = 0;

        auto code = ReadFile(self.handle, value->tail,
            (DWORD) avail, (LPDWORD) &count, 0);

        if ( code == 0 )
            return {count, READ_ERROR_SYSTEM, GetLastError()};

        value->tail += count;

        return {count, READ_ERROR_NONE};
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
        pax_trace();
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
