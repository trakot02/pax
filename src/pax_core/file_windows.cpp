#include <pax_core/file_windows.hpp>
#include <pax_core/buff.hpp>

#include <windows.h>
#include <fileapi.h>
#include <handleapi.h>

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

    File STDOUT = {GetStdHandle(STD_OUTPUT_HANDLE)};
    File STDERR = {GetStdHandle(STD_ERROR_HANDLE)};
    File STDIN  = {GetStdHandle(STD_INPUT_HANDLE)};

    File
    file_empty()
    {
        File file;

        file.handle = 0;

        return file;
    }

    File_Error
    file_open(File* self, Str8 name)
    {
        pax_guard(self != 0, "`self` is null");

        pax_guard(self->handle == 0, "The file is already open");

        void* handle = CreateFileA(name.block,
            GENERIC_READ, 0, 0, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, 0);

        if ( handle == INVALID_HANDLE_VALUE )
            return (File_Error) GetLastError();

        self->handle = handle;

        return FILE_ERROR_NONE;
    }

    File_Error
    file_create(File* self, Str8 name)
    {
        pax_guard(self != 0, "`self` is null");

        pax_guard(self->handle == 0, "The file is already open");

        void* handle = CreateFileA(name.block,
            GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL, 0);

        if ( handle == INVALID_HANDLE_VALUE )
            return (File_Error) GetLastError();

        self->handle = handle;

        return FILE_ERROR_NONE;
    }

    void
    file_close(File* self)
    {
        pax_guard(self != 0, "`self` is null");

        if ( self->handle == 0 ) return;

        auto code = CloseHandle(self->handle);

        pax_guard(code != 0, "The operation failed");

        self->handle = 0;
    }

    Write_Value
    file_write_str8(File* self, Str8 value)
    {
        pax_guard(self != 0, "`self` is null");

        isize total = value.count;
        isize count = 0;

        pax_guard(self->handle != 0, "The file is already closed");

        auto code = WriteFile(self->handle, value.block,
            (DWORD) total, (LPDWORD) &count, 0);

        if ( code == 0 )
            return {count, WRITE_ERROR_SYSTEM, GetLastError()};

        return {count, WRITE_ERROR_NONE};
    }

    Write_Value
    file_write_buff(File* self, Buff* value)
    {
        pax_guard(self  != 0, "`self` is null");
        pax_guard(value != 0, "`value` is null");

        isize total = buff_count(value);
        isize count = 0;

        pax_guard(self->handle != 0, "The file is already closed");

        auto code = WriteFile(self->handle, value->head,
            (DWORD) total, (LPDWORD) &count, 0);

        if ( code == 0 ) {
            value->head += count;

            return {count, WRITE_ERROR_SYSTEM, GetLastError()};
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

        pax_guard(self->handle != 0, "The file is already closed");

        auto code = ReadFile(self->handle, value->tail,
            (DWORD) total, (LPDWORD) &count, 0);

        if ( code == 0 )
            return {0, READ_ERROR_SYSTEM, GetLastError()};

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
