#include <pax_core/file_windows.hpp>
#include <pax_core/buff.hpp>

#include <windows.h>
#include <fileapi.h>
#include <handleapi.h>

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
    file_open(File* file, Str8 name)
    {
        pax_guard(file != 0, "`file` is null");

        pax_guard(file->handle == 0, "The file is already open");

        void* handle = CreateFileA(name.block,
            GENERIC_READ, 0, 0, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, 0);

        if ( handle != INVALID_HANDLE_VALUE ) {
            file->name   = name;
            file->handle = handle;

            return FILE_ERROR_NONE;
        }

        return (File_Error) GetLastError();
    }

    File_Error
    file_create(File* file, Str8 name)
    {
        pax_guard(file != 0, "`file` is null");

        pax_guard(file->handle == 0, "The file is already open");

        void* handle = CreateFileA(name.block,
            GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, 0);

        if ( handle != INVALID_HANDLE_VALUE ) {
            file->name   = name;
            file->handle = handle;

            return FILE_ERROR_NONE;
        }

        return (File_Error) GetLastError();
    }

    void
    file_close(File* file)
    {
        pax_guard(file != 0, "`file` is null");

        if ( file->handle == 0 ) return;

        auto code = CloseHandle(file->handle);

        pax_guard(code != 0, "The operation failed");

        file->name   = "";
        file->handle = 0;
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

        pax_guard(file->handle != 0, "The file is already closed");

        auto code = WriteFile(file->handle, value.block,
            (DWORD) total, (LPDWORD) &count, 0);

        if ( code == 0 )
            return {count, WRITE_ERROR_SYSTEM, GetLastError()};

        return {count, WRITE_ERROR_NONE};
    }

    Write_Value
    file_write_buff(File* file, Buff* value)
    {
        pax_guard(file  != 0, "`file` is null");
        pax_guard(value != 0, "`value` is null");

        isize total = buff_size(value);
        isize count = 0;

        pax_guard(file->handle != 0, "The file is already closed");

        auto code = WriteFile(file->handle, value->head,
            (DWORD) total, (LPDWORD) &count, 0);

        if ( code == 0 ) {
            value->head += count;

            return {count, WRITE_ERROR_SYSTEM, GetLastError()};
        }

        value->head = value->block;
        value->tail = value->block;

        return {count, WRITE_ERROR_NONE};
    }

    Read_Value
    file_read_buff(File* file, Buff* value)
    {
        pax_guard(file != 0, "`file` is null");
        pax_guard(value != 0, "`value` is null");

        isize total = buff_avail(value);
        isize count = 0;

        pax_guard(file->handle != 0, "The file is already closed");

        auto code = ReadFile(file->handle, value->tail,
            (DWORD) total, (LPDWORD) &count, 0);

        if ( code == 0 )
            return {0, READ_ERROR_SYSTEM, GetLastError()};

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
